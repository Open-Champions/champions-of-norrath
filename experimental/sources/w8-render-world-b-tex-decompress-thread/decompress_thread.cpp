// Observed texDecompressThread__FPv at 0x0016f8b8, 2788 bytes.
// The body of the texture decompression thread. It never returns. The thread
// is created by textureInternalInit (another package) and woken by
// texProcessDecompress. Static names x1, y1, x2, y2, y and tab come from the
// original function-local statics; the symbol table spells them x1.846 and so
// on, here they are local statics.
//
// Loop. Each pass is skipped (the thread sleeps and, when the queue is empty or
// textureUncompBufferLocked or clearing is set, texturesAreDecompressing is
// cleared) while decodeQHead == decodeQTail, textureUncompBufferLocked,
// clearing, or the word at 0x10000000 (a timer count) exceeds 8264, or 17654
// when engineFullFrameMode is nonzero. Otherwise the 8-byte queue entry at
// decodeQ[decodeQTail] is taken: {texture, second word}. A null texture or one
// whose word at +20 is nonzero is just dequeued (tail = (tail + 1) & 63). A
// texture whose half at +8 has bit 0 set goes to vqDecompress(texture, second
// word); a zero result leaves it queued, otherwise it is dequeued. All
// other textures use the table decoder below, which is time-sliced and
// resumable through the texDecodeStarted pointer and thread-local state.
//
// Table decoder, start (texDecodeStarted == 0). The scratchpad cursor is
// 0x70000000. A rotating index (uncompSpin) over the 1280 8-byte uncompMap
// entries {owner texture, next} is walked to claim ((half at +6) * 16 + 4095
// >> 12) + 4 entries: an entry that is free or whose owner was last used at
// least 3 frames before drawFrameNm is taken (an owner has its +20 word zeroed
// and its linked entries cleared); fewer than 1279 steps are tried or the
// thread goes back to the loop. texturesAreDecompressing is set. The stream is
// the pointer at texture+16: a word pointing at a header, then 4-byte strip
// headers and 4-byte block pointers. A GS upload packet (BITBLTBUF, TRXPOS,
// TRXREG, TRXDIR, then a 16x16 image of 1024 bytes copied from the header) is
// placed at the cursor, followed by a BITBLTBUF packet with
// DBW = (half at 0 + 127) >> 7. The header holds 1024 palette bytes, 2048
// bytes of context bytes, a word count N, N half-word symbols, 18 limits ints
// and 18 offset ints (the tables used below).
// Each pass (start or resume) rebuilds a 256-entry lookup at 0x70003800 from
// the limits and offsets: for each 8-bit prefix, the first code length L in
// 1..8 with !(limit[L] < prefix >> (8 - L)); entries are {symbol, L} or {0, 0}.
//
// Strips and rows. A strip header is four signed bytes x1 y1 x2 y2 (x1 == -1
// ends the data). Its packet holds TRXREG, TRXPOS, TRXDIR and an IMAGE tag whose
// quad count is ((y2-y1)*2048 + 2048) * (x2-x1+1) >> 7. For each row y the
// stream gives (x2-x1+1) block pointers; each block is 256 symbols decoded with
// the canonical code (longer codes use the limit/offset ints), symbols < 256
// are bytes, 256..260 copy from tab[symbol-256] bytes back, larger symbols read
// the context byte table indexed by (previous byte * 8 + symbol - 261). The
// 256 bytes are permuted through unscrambleTable and written as eight rows of
// 8 pixels (32 bytes) at a stride of ((x2-x1)*8+8) pixels. When more than 4096
// bytes sit in the scratchpad putBlock is called repeatedly until at most 4096
// remain; after the final row of the final strip any remaining bytes are
// flushed with the last flag. When the data ends texture+20 gets the value
// putBlock produced, the chain end is cleared and texDecodeStarted is zeroed.
// Time checks use the timer word with budgets 8264 / 17654 between rows and
// 9390 / 18780 between strips; they are skipped while the uncompressed buffer
// is locked or clearing is set. The meaning of the GS values is inferred from
// register numbers only. Nothing here was executed on hardware.
#if defined(NORRATH_HOST_TEXDEC)
extern unsigned char* norrath_texdec_arena;
extern unsigned char* norrath_texdec_scratch;
extern "C" unsigned norrath_texdec_read_timer();
#define TEXDEC_SCRATCH (norrath_texdec_scratch)
#define TEXDEC_TIMER (norrath_texdec_read_timer())
#else
#define TEXDEC_SCRATCH (reinterpret_cast<unsigned char*>(0x70000000u))
#define TEXDEC_TIMER (*reinterpret_cast<volatile unsigned*>(0x10000000u))
#endif

struct TexDecodePacket {
    unsigned char* cursor;
    unsigned char* unknown;
    unsigned char* last;
};

extern "C" {
extern int decodeQHead;
extern int decodeQTail;
extern unsigned char textureUncompBufferLocked;
extern unsigned char clearing;
extern unsigned char texturesAreDecompressing;
extern int engineFullFrameMode;
extern int uncompSpin;
extern int uncompCache;
extern int drawFrameNm;
extern unsigned char* texDecodeStarted;
extern unsigned char decodeQ[];
extern unsigned char uncompMap[];
extern unsigned char unscrambleTable[];

int SleepThread();
int vqDecompress__FP8_texturei(void* texture, int extra);
void putBlock__FP8_textureP6PacketPPP9uncompMapPPii(void* texture, TexDecodePacket* packet, void*** list, int** slot, int last);
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_dword(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}
inline unsigned long long load_dword(const unsigned char* where) {
    return *reinterpret_cast<const unsigned long long*>(where);
}
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_TEXDEC)
    return norrath_texdec_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned char* chain_address(unsigned bits) {
#if defined(NORRATH_HOST_TEXDEC)
    return uncompMap + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" void texDecompressThread__FPv(void*) {
    static int x1, y1, x2, y2, y;
    static signed char tab[5] = {-1, -16, -17, -15, -2};

    TexDecodePacket packet;
    unsigned char buffer[256];
    unsigned char* stream = nullptr;
    unsigned char* context = nullptr;
    unsigned char* symbols = nullptr;
    unsigned char* offsets = nullptr;
    unsigned char* limits = nullptr;
    int started = 0;
    int* slot0 = nullptr;
    void** slot1 = nullptr;
    int output = 0;
    unsigned char* texture = nullptr;
    int extra = 0;
    int row_width = 0;
    int last_flag = 0;
    unsigned char* row_base = nullptr;
    int index_a1 = 0;
    int steps = 0;
    int needed = 0;

    packet.cursor = TEXDEC_SCRATCH;
    packet.unknown = nullptr;
    packet.last = nullptr;
    SleepThread();

top:
    if (decodeQHead == decodeQTail || textureUncompBufferLocked != 0 || clearing != 0) goto idle;
    {
        const unsigned limit = engineFullFrameMode == 0 ? 8264u : 17654u;
        if (limit < TEXDEC_TIMER) goto idle;
    }
    goto work;

idle:
    if (decodeQHead == decodeQTail || textureUncompBufferLocked != 0 || clearing != 0) texturesAreDecompressing = 0;
    SleepThread();
    goto top;

work:
    texture = address(word(decodeQ, decodeQTail * 8));
    extra = static_cast<int>(word(decodeQ, decodeQTail * 8 + 4));
    if (word(decodeQ, decodeQTail * 8) == 0u || word(texture, 20) != 0u) goto advance;
    if ((*reinterpret_cast<const unsigned short*>(texture + 8) & 1u) != 0) {
        if (vqDecompress__FP8_texturei(texture, extra) == 0) goto top;
        goto advance;
    }
    if (texDecodeStarted != nullptr) goto resume;

    // Start a table decode.
    packet.cursor = TEXDEC_SCRATCH;
    started = 0;
    needed = (((*reinterpret_cast<const unsigned short*>(texture + 6) << 4) + 4095) >> 12) + 4;
    index_a1 = uncompSpin;
    steps = 0;
    {
        const int threshold = drawFrameNm - 3;
        do {
            unsigned char* entry = uncompMap + index_a1 * 8;
            bool take = word(entry, 0) == 0u || !(threshold < static_cast<int>(word(address(word(entry, 0)), 28)));
            while (!take) {
                const int before = steps;
                ++index_a1;
                if (index_a1 >= 1280) index_a1 = 0;
                ++steps;
                if (!(before < 1279)) goto top;
                entry = uncompMap + index_a1 * 8;
                take = word(entry, 0) == 0u || !(threshold < static_cast<int>(word(address(word(entry, 0)), 28)));
            }
            if (word(entry, 0) != 0u) {
                unsigned char* owner = address(word(entry, 0));
                unsigned char* slot = uncompMap + ((static_cast<int>(word(owner, 20)) - uncompCache) >> 12) * 8;
                store_word(owner, 20, 0u);
                store_word(slot, 0, 0u);
                for (unsigned next = word(slot, 4); next != 0u; next = word(slot, 4)) {
                    slot = chain_address(next);
                    store_word(slot, 0, 0u);
                }
            }
            {
                const int before = steps;
                ++index_a1;
                if (index_a1 >= 1280) index_a1 = 0;
                ++steps;
                if (!(before < 1279)) goto top;
            }
            --needed;
        } while (needed > 0);
    }

    {
        unsigned char* base = packet.cursor;
        unsigned char* header;
        texturesAreDecompressing = 1;
        stream = address(word(texture, 16));
        store_dword(base, 0x1000000000008000ull);
        store_dword(base + 8, 14ull);
        store_dword(base + 16, 1ull << 48);
        store_dword(base + 24, 80ull);
        store_dword(base + 32, 0ull);
        store_dword(base + 40, 81ull);
        store_dword(base + 48, 0x1000000010ull);
        store_dword(base + 56, 82ull);
        store_dword(base + 64, 0ull);
        store_dword(base + 72, 83ull);
        store_dword(base + 80, 0x0800000000008040ull);
        store_dword(base + 88, 0ull);
        packet.cursor = base + 96;
        header = address(word(stream, 0));
        stream += 4;
        packet.last = base;
        slot0 = &output;
        slot1 = nullptr;
        {
            const int gif_loop = (static_cast<int>(80) / 8 - 2) >> 1;
            store_dword(base, load_dword(base) | static_cast<unsigned long long>(gif_loop));
        }
        for (int i = 0; i < 1024; ++i) packet.cursor[i] = header[i];
        {
            unsigned char* second = packet.cursor + 1024;
            const int width = *reinterpret_cast<const short*>(texture);
            const int count = static_cast<int>(word(header, 3072));
            packet.cursor = second;
            store_dword(second, 0x1000000000008000ull);
            store_dword(second + 8, 14ull);
            store_dword(second + 16, static_cast<unsigned long long>(static_cast<long long>((width + 127) >> 7)) << 48);
            store_dword(second + 24, 80ull);
            packet.last = second;
            store_dword(second, load_dword(second) | 1ull);
            packet.cursor = second + 32;
            context = header + 1024;
            symbols = header + 3076;
            offsets = symbols + count * 2;
            limits = offsets + 72;
            texDecodeStarted = texture;
        }
    }

resume:
    {
        // Rebuild the 256-entry prefix table at scratchpad offset 0x3800.
        unsigned char* entry = TEXDEC_SCRATCH + 0x3800;
        for (int prefix = 0; prefix < 256; ++prefix, entry += 4) {
            int length = 1;
            int shifted = prefix >> 7;
            bool found = !(static_cast<int>(word(limits, 4)) < shifted);
            while (!found) {
                ++length;
                if (length >= 9) break;
                shifted = prefix >> (8 - length);
                found = !(static_cast<int>(word(limits, length * 4)) < shifted);
            }
            if (found) {
                const int symbol_index = shifted + static_cast<int>(word(offsets, length * 4));
                *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(length);
                *reinterpret_cast<unsigned short*>(entry) =
                    *reinterpret_cast<const unsigned short*>(symbols + symbol_index * 2);
            } else {
                *reinterpret_cast<unsigned short*>(entry) = 0;
                *reinterpret_cast<unsigned short*>(entry + 2) = 0;
            }
        }
    }
    goto after_pass;

advance:
    decodeQTail = (decodeQTail + 1) & 0x3f;
    goto top;

after_pass:
    // Time check between strips.
    if (textureUncompBufferLocked == 0 && clearing == 0) {
        const unsigned limit2 = engineFullFrameMode == 0 ? 9390u : 18780u;
        if (limit2 < TEXDEC_TIMER) goto finish;
    }
    // Start a strip or resume the current one.
    if (started != 0) goto resume_strip;
    if (static_cast<signed char>(stream[0]) == -1) goto finish;
    {
        unsigned char* base = packet.cursor;
        x1 = static_cast<signed char>(stream[0]);
        y1 = static_cast<signed char>(stream[1]);
        x2 = static_cast<signed char>(stream[2]);
        y2 = static_cast<signed char>(stream[3]);
        stream += 4;
        started = 1;
        packet.last = base;
        store_dword(base, 0x1000000000008000ull);
        store_dword(base + 8, 14ull);
        store_dword(base + 16, static_cast<unsigned long long>(static_cast<unsigned>((x2 - x1) * 8 + 8)) |
                                   (static_cast<unsigned long long>(static_cast<unsigned>((y2 - y1) * 8 + 8)) << 32));
        store_dword(base + 24, 82ull);
        store_dword(base + 32, (static_cast<unsigned long long>(static_cast<unsigned>(x1 * 8)) << 32) |
                                   (static_cast<unsigned long long>(static_cast<unsigned>(y1 * 8)) << 48));
        store_dword(base + 40, 81ull);
        store_dword(base + 48, 0ull);
        store_dword(base + 56, 83ull);
        store_dword(base, load_dword(base) | static_cast<unsigned long long>((64 / 8 - 2) >> 1));
        packet.cursor = base + 80;
        {
            int quads = ((y2 - y1) * 2048 + 2048) * (x2 - x1 + 1);
            if (quads < 0) quads += 127;
            quads >>= 7;
            store_dword(base + 64, static_cast<unsigned long long>(static_cast<unsigned>(quads)) | 0x8000ull |
                                       0x0800000000000000ull);
            store_dword(base + 72, 0ull);
        }
        y = y1;
    }

resume_strip:
    row_width = (x2 - x1) * 8 + 8;
    goto row_check;

row_check:
    {
        bool over = false;
        if (textureUncompBufferLocked == 0 && clearing == 0) {
            const unsigned limit = engineFullFrameMode == 0 ? 8264u : 17654u;
            over = limit < TEXDEC_TIMER;
        }
        if (!over) goto row;
    }
    goto after_pass;

row:
    row_base = packet.cursor;
    packet.cursor = row_base + (x2 - x1) * 256 + 256;
    if (x2 - x1 >= 0) {
        int column_offset = 0;
        for (int block = 0; block <= x2 - x1; ++block, column_offset += 8) {
            unsigned char* data = address(word(stream, 0));
            int bit = 0;
            int previous = 0;
            stream += 4;
            for (int out = 0; out < 256; ++out) {
                const unsigned short* half = reinterpret_cast<const unsigned short*>(data + ((bit >> 4) << 1));
                unsigned window = (static_cast<unsigned>(half[0]) << 16) | half[1];
                window = (window << (bit & 15)) >> 16;
                const unsigned char* entry = TEXDEC_SCRATCH + 0x3800 + (window >> 8) * 4;
                int symbol;
                const int length = *reinterpret_cast<const short*>(entry + 2);
                if (length != 0) {
                    symbol = *reinterpret_cast<const short*>(entry);
                    bit += length;
                } else {
                    int shifted = static_cast<int>(window >> 7);
                    if (!(static_cast<int>(word(limits, 36)) < shifted)) {
                        bit += 9;
                        symbol = *reinterpret_cast<const short*>(
                            symbols + (shifted + static_cast<int>(word(offsets, 36))) * 2);
                    } else {
                        int code_length = 9;
                        do {
                            ++code_length;
                            shifted = static_cast<int>(window >> (16 - code_length));
                        } while (static_cast<int>(word(limits, code_length * 4)) < shifted);
                        bit += code_length;
                        symbol = *reinterpret_cast<const short*>(
                            symbols + (shifted + static_cast<int>(word(offsets, code_length * 4))) * 2);
                    }
                }
                unsigned char value;
                if (symbol < 256) {
                    value = static_cast<unsigned char>(symbol);
                } else if (symbol < 261) {
                    value = buffer[out + tab[symbol - 256]];
                } else {
                    value = context[previous * 8 + symbol - 261];
                }
                buffer[out] = value;
                previous = value;
            }
            {
                int source = 0;
                int row_offset = 0;
                for (int line = 0; line < 8; ++line, row_offset += row_width) {
                    unsigned char* out = row_base + (column_offset + row_offset) * 4;
                    for (int pair = 0; pair < 8; ++pair, out += 4) {
                        out[0] = buffer[unscrambleTable[source]];
                        out[1] = buffer[unscrambleTable[source + 1]];
                        out[2] = buffer[unscrambleTable[source + 2]];
                        out[3] = buffer[unscrambleTable[source + 3]];
                        source += 4;
                    }
                }
            }
        }
    }

    last_flag = 0;
    if (y == y2) {
        last_flag = 1;
        if (static_cast<signed char>(stream[0]) != -1) last_flag = 0;
    }
    if (static_cast<int>(packet.cursor - TEXDEC_SCRATCH) >= 4097) {
        do {
            putBlock__FP8_textureP6PacketPPP9uncompMapPPii(texture, &packet, &slot1, &slot0, last_flag);
        } while (static_cast<int>(packet.cursor - TEXDEC_SCRATCH) >= 4097);
    }
    ++y;
    if (y2 < y) {
        started = 0;
        goto after_pass;
    }
    goto row_check;

finish:
    if (started != 0) goto top;
    if (static_cast<signed char>(stream[0]) == -1) {
        while (static_cast<int>(packet.cursor - TEXDEC_SCRATCH) > 0) {
            putBlock__FP8_textureP6PacketPPP9uncompMapPPii(texture, &packet, &slot1, &slot0, 1);
        }
    }
    if (started != 0) goto top;
    if (static_cast<signed char>(stream[0]) != -1) goto top;
    store_word(texture, 20, static_cast<unsigned>(output));
    *slot1 = nullptr;
    texDecodeStarted = nullptr;
    goto top;
}
