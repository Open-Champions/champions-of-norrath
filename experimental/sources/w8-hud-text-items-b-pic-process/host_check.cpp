#include <cstdint>
#include <cstring>

extern "C" {
signed char _4item$picsToLoad;
int _4item$picInProgress;
signed char _4item$picLoadFinished;
int drawFrameNm;
alignas(16) unsigned char _4item$picList[64 * 192];
alignas(16) unsigned char _4item$picLoadBuffer[16384];
alignas(16) unsigned char _4item$picBuffer[256 * 1280];
void picProcess__4itemv();
}

static int kick_calls, kick_result_count;
static int kick_results[4];
static int kick_free_slot = -1;
static int find_calls;
static const unsigned char* find_result;
static const char* find_seen[8];
static int load_calls;
static const char* load_file;
static void* load_buffer;
static void* load_done;
static int load_a, load_b;
static int busy;
static int busy_calls;

extern "C" {
int picKickOneOut__4itemv() {
    int r = kick_calls < 4 ? kick_results[kick_calls] : 0;
    ++kick_calls;
    if (r && kick_free_slot >= 0) _4item$picBuffer[kick_free_slot * 1280] = 0;
    return r;
}
const unsigned char* picFind__4itemPc(const char* name) {
    if (find_calls < 8) find_seen[find_calls] = name;
    ++find_calls;
    return find_result;
}
void cdLoadAsync__FPcPvT0ii(const char* file, void* buffer, void* done, int a, int b) {
    ++load_calls;
    load_file = file;
    load_buffer = buffer;
    load_done = done;
    load_a = a;
    load_b = b;
}
int cdIsBusy__Fv() {
    ++busy_calls;
    return busy;
}
}

static unsigned char* rec(int i) { return _4item$picList + i * 192; }
static short& state_of(int i) { return *reinterpret_cast<short*>(rec(i) + 64); }
static int& stamp_of(int i) { return *reinterpret_cast<int*>(rec(i) + 124); }
static unsigned char* block(int i) { return rec(i) + 128; }
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned short half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static unsigned low32(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned char* slot(int i) { return _4item$picBuffer + i * 1280; }

static unsigned char descriptor[8];

static void reset() {
    std::memset(_4item$picList, 0, sizeof _4item$picList);
    std::memset(_4item$picLoadBuffer, 0, sizeof _4item$picLoadBuffer);
    std::memset(_4item$picBuffer, 0, sizeof _4item$picBuffer);
    _4item$picsToLoad = 1;
    _4item$picInProgress = -1;
    _4item$picLoadFinished = 0;
    drawFrameNm = 1234;
    kick_calls = 0;
    for (int& r : kick_results) r = 0;
    kick_free_slot = -1;
    find_calls = 0;
    std::memset(descriptor, 0, sizeof descriptor);
    descriptor[2] = 7;
    descriptor[4] = 9;
    find_result = descriptor;
    load_calls = 0;
    busy = 0;
    busy_calls = 0;
}

static void use(int i, bool on = true) { slot(i)[0] = on ? 1 : 0; }

static void header(unsigned count, int data_offset) {
    unsigned char* b = _4item$picLoadBuffer;
    b[6] = count & 0xff;
    b[7] = count >> 8;
    std::memcpy(b + 16, &data_offset, 4);
    b[0] = 0x55;
    b[63] = 0x66;
    for (unsigned i = 0; i < count * 16 + 64; ++i) b[data_offset + i] = static_cast<unsigned char>(i * 7 + 3);
}

int main() {
    // Disabled: nothing happens.
    reset();
    _4item$picsToLoad = 0;
    _4item$picInProgress = 2;
    _4item$picLoadFinished = 1;
    picProcess__4itemv();
    if (busy_calls != 0 || load_calls != 0 || stamp_of(2) != 0) return 1;

    // Kick: skips unused records and records whose state is not -1.
    reset();
    rec(1)[0] = 'a'; state_of(1) = 5;
    rec(3)[0] = 'b'; state_of(3) = -1;
    rec(5)[0] = 'c'; state_of(5) = -1;
    picProcess__4itemv();
    if (find_calls != 1 || find_seen[0] != reinterpret_cast<const char*>(rec(3))) return 2;
    if (load_calls != 1 || std::strcmp(load_file, "piccache.dat") != 0) return 3;
    if (load_buffer != _4item$picLoadBuffer || load_done != &_4item$picLoadFinished) return 4;
    if (load_a != (9 << 11) || load_b != (7 << 11)) return 5;
    if (_4item$picsToLoad != 1 || _4item$picInProgress != 3 || stamp_of(3) != 1234) return 6;
    if (stamp_of(5) != 0 || state_of(5) != -1) return 7;

    // A record picFind rejects gets -2 and the scan goes on to the next one.
    reset();
    rec(2)[0] = 'a'; state_of(2) = -1;
    rec(4)[0] = 'b'; state_of(4) = -1;
    find_result = nullptr;
    picProcess__4itemv();
    if (find_calls != 2 || state_of(2) != -2 || state_of(4) != -2) return 8;
    if (load_calls != 0 || _4item$picsToLoad != 0) return 9;

    // Busy: nothing is cleared and nothing starts.
    reset();
    rec(2)[0] = 'a'; state_of(2) = -1;
    busy = 1;
    picProcess__4itemv();
    if (busy_calls != 1 || _4item$picsToLoad != 1 || find_calls != 0 || load_calls != 0) return 10;

    // The in-progress record is stamped before anything else.
    reset();
    _4item$picInProgress = 6;
    busy = 1;
    picProcess__4itemv();
    if (stamp_of(6) != 1234 || stamp_of(0) != 0) return 11;

    // Finished load, 100 quadwords: chunks of 79 and 21 into the first free slots.
    reset();
    _4item$picInProgress = 2;
    _4item$picLoadFinished = 1;
    header(100, 64);
    use(0); use(1);
    busy = 1;
    picProcess__4itemv();
    short* list = reinterpret_cast<short*>(rec(2) + 64);
    if (list[0] != 2 || list[1] != 3 || list[2] != -1) return 12;
    if (half(block(2) + 10) != 0xa7fb || word(block(2) + 16) != 0) return 13;
    if (_4item$picInProgress != static_cast<int>(0xfff0bdc0u) || _4item$picLoadFinished != 0) return 14;
    if (word(block(2) + 20) != low32(slot(2))) return 15;
    if (word(slot(2)) != (79u | 0x20000000u) || word(slot(2) + 4) != low32(slot(3))) return 16;
    if (word(slot(3)) != (21u | 0x60000000u)) return 17;
    if (block(2)[0] != 0x55 || block(2)[63] != 0x66) return 18;
    if (stamp_of(2) != 1234) return 19;
    for (int i = 0; i < 79 * 16; ++i) {
        if (slot(2)[16 + i] != _4item$picLoadBuffer[64 + i]) return 20;
    }
    for (int i = 0; i < 21 * 16; ++i) {
        if (slot(3)[16 + i] != _4item$picLoadBuffer[64 + 79 * 16 + i]) return 21;
    }
    if (busy_calls != 1 || _4item$picsToLoad != 1) return 22;

    // Exactly 79 quadwords: one chunk, last. 80: 79 then 1. Zero: no chunks.
    reset();
    _4item$picInProgress = 0;
    _4item$picLoadFinished = 1;
    header(79, 64);
    busy = 1;
    picProcess__4itemv();
    list = reinterpret_cast<short*>(rec(0) + 64);
    if (list[0] != 0 || list[1] != -1 || word(slot(0)) != (79u | 0x60000000u)) return 23;
    reset();
    _4item$picInProgress = 0;
    _4item$picLoadFinished = 1;
    header(80, 64);
    busy = 1;
    picProcess__4itemv();
    list = reinterpret_cast<short*>(rec(0) + 64);
    if (list[0] != 0 || list[1] != 1 || list[2] != -1) return 24;
    if (word(slot(0)) != (79u | 0x20000000u) || word(slot(1)) != (1u | 0x60000000u)) return 25;
    reset();
    _4item$picInProgress = 1;
    _4item$picLoadFinished = 1;
    header(0, 64);
    busy = 1;
    picProcess__4itemv();
    list = reinterpret_cast<short*>(rec(1) + 64);
    if (list[0] != -1 || half(block(1) + 10) != 0xa7fb || word(block(1) + 20) != 0) return 26;
    if (_4item$picLoadFinished != 0) return 27;

    // All slots taken: picKickOneOut frees one on its first call, then the scan succeeds.
    reset();
    _4item$picInProgress = 4;
    _4item$picLoadFinished = 1;
    header(10, 64);
    for (int i = 0; i < 256; ++i) use(i);
    kick_results[0] = 1;
    kick_free_slot = 200;
    busy = 1;
    picProcess__4itemv();
    list = reinterpret_cast<short*>(rec(4) + 64);
    if (kick_calls != 1 || list[0] != 200 || list[1] != -1) return 28;

    // No slot and nothing to kick out after a first chunk: assigned slots are released.
    reset();
    _4item$picInProgress = 4;
    _4item$picLoadFinished = 1;
    header(160, 64);                       // 79 + 79 + 2
    for (int i = 0; i < 256; ++i) use(i);
    use(10, false);
    use(20, false);
    kick_results[0] = 0;
    picProcess__4itemv();
    list = reinterpret_cast<short*>(rec(4) + 64);
    if (kick_calls != 1) return 29;
    if (slot(10)[0] != 0 || slot(20)[0] != 0 || slot(11)[0] == 0) return 30;
    if (list[0] != -1) return 31;
    if (_4item$picLoadFinished != 1 || _4item$picInProgress != 4) return 32;
    if (busy_calls != 0 || half(block(4) + 10) != 0) return 33;
    return 0;
}
