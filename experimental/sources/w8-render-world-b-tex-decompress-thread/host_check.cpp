#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

struct Packet {
    unsigned char* cursor;
    unsigned char* unknown;
    unsigned char* last;
};

extern "C" {
int decodeQHead;
int decodeQTail;
unsigned char textureUncompBufferLocked;
unsigned char clearing;
unsigned char texturesAreDecompressing;
int engineFullFrameMode;
int uncompSpin;
int uncompCache;
int drawFrameNm;
unsigned char* texDecodeStarted;
unsigned char decodeQ[64 * 8];
unsigned char uncompMap[1280 * 8];
unsigned char unscrambleTable[256];
void texDecompressThread__FPv(void*);
}

alignas(16) static unsigned char arena[1 << 20];
alignas(16) static unsigned char scratch[0x8000];
unsigned char* norrath_texdec_arena = arena;
unsigned char* norrath_texdec_scratch = scratch;

struct Done {};

static unsigned timer_value;
static int timer_reads, timer_read_limit;
static int sleep_calls, sleep_limit;
static int vq_calls, vq_result, vq_extra;
static unsigned char* vq_texture;
static int put_calls, put_slice_after;
static std::vector<unsigned char> captured;
static int put_last_flags;
static int put_output_value;
static unsigned char chain_word_storage[8];

extern "C" unsigned norrath_texdec_read_timer() {
    ++timer_reads;
    if (timer_read_limit && timer_reads > timer_read_limit) return 99999u;
    return timer_value;
}
extern "C" int SleepThread() {
    ++sleep_calls;
    if (sleep_limit && sleep_calls >= sleep_limit) throw Done();
    timer_value = 0;     // a new frame begins whenever the thread has slept
    return 0;
}
extern "C" int vqDecompress__FP8_texturei(void* texture, int extra) {
    ++vq_calls; vq_texture = static_cast<unsigned char*>(texture); vq_extra = extra;
    return vq_result;
}
extern "C" void putBlock__FP8_textureP6PacketPPP9uncompMapPPii(void* texture, Packet* packet, void*** list, int** slot, int last) {
    ++put_calls;
    put_last_flags = (put_last_flags << 1) | (last ? 1 : 0);
    if (texture == nullptr) put_last_flags += 1000;
    int used = static_cast<int>(packet->cursor - scratch);
    int take = used;
    if (!last && take > 4080) take = 4080;
    captured.insert(captured.end(), scratch, scratch + take);
    std::memmove(scratch, scratch + take, static_cast<size_t>(used - take));
    packet->cursor -= take;
    **slot = put_output_value;
    *list = reinterpret_cast<void**>(chain_word_storage);
    if (put_slice_after && put_calls == put_slice_after) timer_value = 50000;   // out of time after this call
}

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static unsigned short& H(int off) { return *reinterpret_cast<unsigned short*>(arena + off); }
static void put64(std::vector<unsigned char>& v, unsigned long long x) {
    unsigned char b[8]; std::memcpy(b, &x, 8); v.insert(v.end(), b, b + 8);
}

// ---- reference encoder ---------------------------------------------------
static const int kSymbols = 20;
static const int kLengths[kSymbols] = {2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11};
static int symbol_value[kSymbols];
static unsigned code_value[kSymbols];
static int first_code[17], count_len[17], first_index[17];
static unsigned char context_table[2048];
static const signed char kTab[5] = {-1, -16, -17, -15, -2};

static void build_code() {
    // Symbols: some byte values, two copy symbols, and context symbols.
    const int values[kSymbols] = {0x10, 0x20, 0x31, 0x42, 0x53, 0x64, 0x75, 0x86, 0x97, 0xa8,
                                  0xb9, 0xca, 0xdb, 0xec, 0xfd, 256, 257, 261, 262, 267};
    for (int i = 0; i < kSymbols; ++i) symbol_value[i] = values[i];
    std::memset(count_len, 0, sizeof count_len);
    for (int i = 0; i < kSymbols; ++i) ++count_len[kLengths[i]];
    int code = 0, index = 0;
    for (int len = 1; len <= 16; ++len) {
        first_code[len] = code;
        first_index[len] = index;
        for (int i = 0; i < kSymbols; ++i)
            if (kLengths[i] == len) code_value[i] = static_cast<unsigned>(first_code[len] + (i - first_index[len]));
        code = (first_code[len] + count_len[len]) << 1;
        index += count_len[len];
    }
}

struct BlockData {
    std::vector<unsigned char> bytes;     // decoded 256 bytes
    std::vector<unsigned short> stream;   // packed halfwords
};

static BlockData make_block(unsigned seed) {
    BlockData block;
    std::vector<int> picked;
    unsigned state = seed * 2654435761u + 12345u;
    int previous = 0;
    std::vector<unsigned char>& out = block.bytes;
    std::vector<char> bits;
    for (int i = 0; i < 256; ++i) {
        for (;;) {
            state = state * 1664525u + 1013904223u;
            const int pick = static_cast<int>((state >> 16) % kSymbols);
            const int sym = symbol_value[pick];
            unsigned char value;
            if (sym < 256) value = static_cast<unsigned char>(sym);
            else if (sym < 261) {
                const int back = i + kTab[sym - 256];
                if (back < 0) continue;
                value = out[static_cast<size_t>(back)];
            } else value = context_table[previous * 8 + sym - 261];
            out.push_back(value);
            previous = value;
            for (int b = kLengths[pick] - 1; b >= 0; --b) bits.push_back(static_cast<char>((code_value[pick] >> b) & 1));
            break;
        }
    }
    while (bits.size() % 16) bits.push_back(0);
    for (int i = 0; i < 4 * 16; ++i) bits.push_back(0);
    for (size_t i = 0; i < bits.size(); i += 16) {
        unsigned short h = 0;
        for (int b = 0; b < 16; ++b) h = static_cast<unsigned short>((h << 1) | bits[i + b]);
        block.stream.push_back(h);
    }
    return block;
}

static int next_block_offset;
static std::vector<unsigned char> palette(1024);

// Builds a texture whose stream holds the given strips; returns the expected
// captured bytes for a complete uninterrupted decode.
struct Strip { int x1, y1, x2, y2; };

static std::vector<unsigned char> build_texture(int tex_offset, const std::vector<Strip>& strips, unsigned seed, int width_field) {
    const int header = 0x2000;
    const int stream_offset = 0x1000;
    std::memset(arena + tex_offset, 0, 64);
    H(tex_offset + 0) = static_cast<unsigned short>(width_field);
    H(tex_offset + 6) = 256;
    H(tex_offset + 8) = 0;
    W(tex_offset + 16) = stream_offset;
    W(tex_offset + 20) = 0;
    // header block
    for (int i = 0; i < 1024; ++i) palette[i] = static_cast<unsigned char>(i * 7 + 3);
    std::memcpy(arena + header, palette.data(), 1024);
    for (int i = 0; i < 2048; ++i) context_table[i] = static_cast<unsigned char>(i * 13 + 5);
    std::memcpy(arena + header + 1024, context_table, 2048);
    W(header + 3072) = kSymbols;
    for (int i = 0; i < kSymbols; ++i) H(header + 3076 + i * 2) = static_cast<unsigned short>(symbol_value[i]);
    const int offsets = header + 3076 + kSymbols * 2;
    const int limits = offsets + 72;
    for (int len = 0; len <= 17; ++len) {
        const int l = len;
        int limit = -1, offset = 0;
        if (l >= 1 && l <= 16) {
            limit = count_len[l] ? first_code[l] + count_len[l] - 1 : -1;
            offset = first_index[l] - first_code[l];
        }
        W(offsets + l * 4) = static_cast<unsigned>(offset);
        W(limits + l * 4) = static_cast<unsigned>(limit);
    }
    // stream
    int at = stream_offset;
    W(at) = header; at += 4;
    next_block_offset = 0x4000;
    std::vector<unsigned char> expected;
    // packet 1
    put64(expected, 0x1000000000008004ull); put64(expected, 14);
    put64(expected, 1ull << 48); put64(expected, 80);
    put64(expected, 0); put64(expected, 81);
    put64(expected, 0x1000000010ull); put64(expected, 82);
    put64(expected, 0); put64(expected, 83);
    put64(expected, 0x0800000000008040ull); put64(expected, 0);
    expected.insert(expected.end(), palette.begin(), palette.end());
    put64(expected, 0x1000000000008001ull); put64(expected, 14);
    put64(expected, static_cast<unsigned long long>((width_field + 127) >> 7) << 48); put64(expected, 80);
    unsigned block_seed = seed;
    for (const Strip& s : strips) {
        arena[at + 0] = static_cast<unsigned char>(s.x1); arena[at + 1] = static_cast<unsigned char>(s.y1);
        arena[at + 2] = static_cast<unsigned char>(s.x2); arena[at + 3] = static_cast<unsigned char>(s.y2);
        at += 4;
        const int cols = s.x2 - s.x1 + 1;
        const int row_width = cols * 8;
        put64(expected, 0x1000000000008003ull); put64(expected, 14);
        put64(expected, static_cast<unsigned long long>(row_width) | (static_cast<unsigned long long>((s.y2 - s.y1) * 8 + 8) << 32)); put64(expected, 82);
        put64(expected, (static_cast<unsigned long long>(s.x1 * 8) << 32) | (static_cast<unsigned long long>(s.y1 * 8) << 48)); put64(expected, 81);
        put64(expected, 0); put64(expected, 83);
        const int quads = (((s.y2 - s.y1) * 2048 + 2048) * cols) >> 7;
        put64(expected, static_cast<unsigned long long>(quads) | 0x8000ull | 0x0800000000000000ull); put64(expected, 0);
        for (int y = s.y1; y <= s.y2; ++y) {
            std::vector<unsigned char> row(static_cast<size_t>(cols) * 256);
            for (int c = 0; c < cols; ++c) {
                BlockData block = make_block(block_seed++);
                W(at) = static_cast<unsigned>(next_block_offset); at += 4;
                std::memcpy(arena + next_block_offset, block.stream.data(), block.stream.size() * 2);
                next_block_offset += static_cast<int>((block.stream.size() * 2 + 15) & ~15u);
                for (int k = 0; k < 256; ++k) {
                    const int line = k / 32, pair = (k % 32) / 4, byte = k % 4;
                    const int dest = (c * 8 + line * row_width) * 4 + pair * 4 + byte;
                    row[static_cast<size_t>(dest)] = block.bytes[unscrambleTable[k]];
                }
            }
            expected.insert(expected.end(), row.begin(), row.end());
        }
    }
    arena[at] = 0xff; arena[at + 1] = 0; arena[at + 2] = 0; arena[at + 3] = 0;
    return expected;
}

static void reset_state() {
    std::memset(arena, 0, 0x300);
    std::memset(decodeQ, 0, sizeof decodeQ);
    std::memset(uncompMap, 0, sizeof uncompMap);
    std::memset(scratch, 0x5a, sizeof scratch);
    decodeQHead = decodeQTail = 0;
    textureUncompBufferLocked = clearing = 0; texturesAreDecompressing = 1; engineFullFrameMode = 0;
    uncompSpin = 5; uncompCache = 0x100000; drawFrameNm = 100; texDecodeStarted = nullptr;
    timer_value = 0; timer_reads = 0; timer_read_limit = 0; sleep_calls = 0; sleep_limit = 0;
    vq_calls = 0; vq_result = 1; put_calls = 0; put_slice_after = 0; captured.clear(); put_last_flags = 0;
    put_output_value = 0x7777;
    std::memset(chain_word_storage, 0x11, sizeof chain_word_storage);
}
static void enqueue(int slot, unsigned tex_offset, unsigned extra) {
    *reinterpret_cast<unsigned*>(decodeQ + slot * 8) = tex_offset;
    *reinterpret_cast<unsigned*>(decodeQ + slot * 8 + 4) = extra;
    decodeQHead = slot + 1;
}
static bool run_thread() {
    try { texDecompressThread__FPv(nullptr); } catch (Done&) { return true; }
    return false;
}

int main() {
    for (int i = 0; i < 256; ++i) unscrambleTable[i] = static_cast<unsigned char>((i * 37 + 11) & 255);
    build_code();

    // 1. Empty queue: sleeps, clears the flag, decodes nothing.
    reset_state(); sleep_limit = 2;
    if (!run_thread() || sleep_calls != 2 || texturesAreDecompressing != 0 || put_calls || vq_calls) return 1;
    // 2. Entry present but locked, clearing, or late: sleeps; the flag is cleared only for locked/clearing.
    reset_state(); sleep_limit = 2; enqueue(0, 0x100, 9); textureUncompBufferLocked = 1;
    if (!run_thread() || texturesAreDecompressing != 0 || vq_calls) return 2;
    reset_state(); sleep_limit = 2; enqueue(0, 0x100, 9); clearing = 1;
    if (!run_thread() || texturesAreDecompressing != 0 || vq_calls) return 3;
    reset_state(); sleep_limit = 2; enqueue(0, 0x100, 9); timer_value = 8265;
    // the sleep resets the timer, so the second pass proceeds to the queue entry
    std::memset(arena, 0, 0x300); H(0x100 + 8) = 1; W(0x100 + 20) = 5; vq_result = 1;
    if (!run_thread() || vq_calls != 0) return 4;
    // 3. A texture with the VQ bit goes to vqDecompress; zero keeps it queued.
    reset_state(); sleep_limit = 2; timer_read_limit = 20; H(0x100 + 8) = 1; enqueue(0, 0x100, 42); vq_result = 0;
    if (!run_thread() || vq_calls < 1 || vq_extra != 42 || vq_texture != arena + 0x100 || decodeQTail != 0) return 5;
    reset_state(); sleep_limit = 2; H(0x100 + 8) = 1; enqueue(0, 0x100, 43); vq_result = 1;
    if (!run_thread() || vq_calls != 1 || vq_extra != 43 || decodeQTail != 1) return 6;
    // null and already-decoded textures are skipped
    reset_state(); sleep_limit = 2; enqueue(0, 0, 0);
    if (!run_thread() || decodeQTail != 1 || vq_calls) return 7;
    reset_state(); sleep_limit = 2; W(0x100 + 20) = 99; enqueue(0, 0x100, 0);
    if (!run_thread() || decodeQTail != 1 || vq_calls || put_calls) return 8;
    // the queue index wraps at 64
    reset_state(); sleep_limit = 2; W(0x100 + 20) = 99; enqueue(0, 0x100, 0);
    decodeQTail = 63; *reinterpret_cast<unsigned*>(decodeQ + 63 * 8) = 0x100; decodeQHead = 0;
    if (!run_thread() || decodeQTail != 0) return 9;

    // 4. A full decode of two strips.
    {
        reset_state(); sleep_limit = 2;
        const std::vector<Strip> strips = {{0, 0, 3, 3}, {4, 0, 4, 0}};
        const std::vector<unsigned char> expected = build_texture(0x100, strips, 1, 300);
        enqueue(0, 0x100, 0);
        if (!run_thread()) return 10;
        if (captured != expected) {
            size_t i = 0;
            while (i < captured.size() && i < expected.size() && captured[i] == expected[i]) ++i;
            std::printf("mismatch at %zu of %zu/%zu\n", i, captured.size(), expected.size());
            return 11;
        }
        if (W(0x100 + 20) != 0x7777u) return 12;
        if (texDecodeStarted != nullptr) return 13;
        if (decodeQTail != 1) return 14;
        if (*reinterpret_cast<unsigned*>(chain_word_storage) != 0u) return 15;
        if ((put_last_flags & 1) != 1) return 16;       // final flush used the last flag
        if (texturesAreDecompressing != 0) return 17;
        if (put_calls < 2) return 18;
    }

    // 5. Time slicing: out of time after the first putBlock, then resumed after sleeping.
    {
        reset_state(); sleep_limit = 3; put_slice_after = 1;
        const std::vector<Strip> strips = {{0, 0, 3, 3}, {4, 0, 4, 0}};
        const std::vector<unsigned char> expected = build_texture(0x100, strips, 2, 300);
        enqueue(0, 0x100, 0);
        if (!run_thread()) return 19;
        if (captured != expected) return 20;
        if (W(0x100 + 20) != 0x7777u || texDecodeStarted != nullptr || decodeQTail != 1) return 21;
        if (sleep_calls < 3) return 22;
    }

    // 6. Entry claiming: recent owners are skipped, a stale owner and its chain are released.
    {
        reset_state(); sleep_limit = 2;
        const std::vector<Strip> strips = {{0, 0, 0, 0}};
        const std::vector<unsigned char> expected = build_texture(0x100, strips, 3, 64);
        W(0x300 + 28) = 100;                 // recent owner A (frame 100, now 100)
        W(0x340 + 28) = 10;                  // stale owner B
        W(0x340 + 20) = 0x100000 + (20 << 12);   // its decoded pointer: chain starts at entry 20
        for (int e : {5, 6}) { W(0) = 0; *reinterpret_cast<unsigned*>(uncompMap + e * 8) = 0x300; }
        *reinterpret_cast<unsigned*>(uncompMap + 7 * 8) = 0x340;
        *reinterpret_cast<unsigned*>(uncompMap + 20 * 8) = 0x340; *reinterpret_cast<unsigned*>(uncompMap + 20 * 8 + 4) = 21 * 8;
        *reinterpret_cast<unsigned*>(uncompMap + 21 * 8) = 0x340; *reinterpret_cast<unsigned*>(uncompMap + 21 * 8 + 4) = 22 * 8;
        *reinterpret_cast<unsigned*>(uncompMap + 22 * 8) = 0x340; *reinterpret_cast<unsigned*>(uncompMap + 22 * 8 + 4) = 0;
        enqueue(0, 0x100, 0);
        if (!run_thread()) return 23;
        if (W(0x340 + 20) != 0u) return 24;
        for (int e : {20, 21, 22}) if (*reinterpret_cast<unsigned*>(uncompMap + e * 8) != 0u) return 25;
        if (*reinterpret_cast<unsigned*>(uncompMap + 5 * 8) != 0x300 || *reinterpret_cast<unsigned*>(uncompMap + 6 * 8) != 0x300) return 26;
        if (*reinterpret_cast<unsigned*>(uncompMap + 7 * 8) != 0x340) return 27;   // only the chain is cleared
        if (captured != expected) return 28;
        if (W(0x300 + 28) != 100u) return 29;
    }
    // 7. Every entry recent: the claim gives up without starting a decode.
    {
        reset_state(); sleep_limit = 2; timer_read_limit = 40;
        const std::vector<Strip> strips = {{0, 0, 0, 0}};
        build_texture(0x100, strips, 4, 64);
        W(0x300 + 28) = 100;
        for (int e = 0; e < 1280; ++e) *reinterpret_cast<unsigned*>(uncompMap + e * 8) = 0x300;
        enqueue(0, 0x100, 0);
        if (!run_thread()) return 30;
        // Idle only because of the timer: the flag stays as it was.
        if (texDecodeStarted != nullptr || put_calls != 0 || texturesAreDecompressing != 1 || decodeQTail != 0) return 31;
    }
    // 8. Empty strip list: nothing is uploaded after the header packets, the texture still completes.
    {
        reset_state(); sleep_limit = 2;
        const std::vector<Strip> strips = {};
        const std::vector<unsigned char> expected = build_texture(0x100, strips, 5, 128);
        enqueue(0, 0x100, 0);
        if (!run_thread()) return 32;
        if (captured != expected || W(0x100 + 20) != 0x7777u) return 33;
    }
    return 0;
}
