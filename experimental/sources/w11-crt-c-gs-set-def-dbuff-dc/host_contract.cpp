// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp gs_set_def_dbuff_dc.cpp
#include <cstring>
#include <vector>
extern "C" {
alignas(16) static unsigned char db[816];
alignas(16) static unsigned char gparam[16];
static std::vector<int> events;           // order of callee kinds
static std::vector<int> args;              // flattened arguments, per call
static int zbuf_result;
unsigned char* sceGsGetGParam() { events.push_back(0); return gparam; }
void sceGsSetDefDispEnv(unsigned char* e, int psm, int w, int h, int dx, int dy) {
    events.push_back(1); args.insert(args.end(), {static_cast<int>(e - db), psm, w, h, dx, dy});
}
void sceGsSetDefDrawEnv(unsigned char* e, int psm, int w, int h, int zt, int zp) {
    events.push_back(2); args.insert(args.end(), {static_cast<int>(e - db), psm, w, h, zt, zp});
}
void sceGsSetDefDrawEnv2(unsigned char* e, int psm, int w, int h, int zt, int zp) {
    events.push_back(3); args.insert(args.end(), {static_cast<int>(e - db), psm, w, h, zt, zp});
}
void sceGsSetDefClear(unsigned char* e, int zt, int x, int y, int w, int h, int r, int g, int b, int a, int q) {
    events.push_back(4); args.insert(args.end(), {static_cast<int>(e - db), zt, x, y, w, h, r, g, b, a, q});
}
int sceGszbufaddr(int psm, int w, int h) { events.push_back(5); args.insert(args.end(), {psm, w, h}); return zbuf_result; }
void sceGsSetDefDBuffDc(unsigned char* d, int psm, int w, int h, int zt, int zp, int clear);
}
static unsigned long long get(int offset) { unsigned long long v; std::memcpy(&v, db + offset, 8); return v; }
static void reset(unsigned long long first_param_word, int zbuf) {
    for (int i = 0; i < 816; ++i) db[i] = static_cast<unsigned char>(i * 7 + 3);
    std::memcpy(gparam, &first_param_word, 8);
    events.clear(); args.clear(); zbuf_result = zbuf;
}
int main() {
    // clear requested, frame mode (word is exactly 1 in both halves)
    reset(0x0000000100000001ull, 0x1234);
    unsigned char before[816];
    std::memcpy(before, db, sizeof db);
    sceGsSetDefDBuffDc(db, 10, 640, 448, 3, 49, 1);
    const std::vector<int> want_events = {0, 1, 1, 2, 3, 2, 3, 4, 4, 5};
    if (events != want_events) return 1;
    const std::vector<int> want_args = {
        0, 10, 640, 448, 0, 0,  40, 10, 640, 448, 0, 0,                    // two display environments
        96, 10, 640, 448, 3, 49,  224, 10, 640, 448, 3, 49,                 // draw, draw2
        464, 10, 640, 448, 3, 49,  592, 10, 640, 448, 3, 49,
        352, 3, 2048 - 320, 2048 - 224, 640, 448, 0, 0, 0, 0, 0,            // clears
        720, 3, 2048 - 320, 2048 - 224, 640, 448, 0, 0, 0, 0, 0,
        10, 640, 448};
    if (args != want_args) return 2;
    if (get(80) != (22ull | 0x8000ull | (1ull << 60)) || get(88) != 14 || get(448) != (22ull | 0x8000ull | (1ull << 60)) || get(456) != 14) return 3;
    const unsigned long long half = 0x1234 >> 1;
    // adjusted doublewords: low nine bits replaced
    unsigned long long b;
    for (int off : {56, 96, 224}) {
        std::memcpy(&b, before + off, 8);
        if (get(off) != ((b & ~0x1ffull) | (half & 0x1ff))) return 4;
    }
    // everything else untouched (these stubs write nothing)
    for (int i = 0; i < 816; ++i) {
        bool touched = (i >= 80 && i < 96) || (i >= 448 && i < 464) || (i >= 56 && i < 64) || (i >= 96 && i < 104) || (i >= 224 && i < 232);
        if (!touched && db[i] != before[i]) return 5;
    }

    // no clear: no clear packets, header count 16
    reset(0x0000000100000001ull, 4);
    sceGsSetDefDBuffDc(db, 0, 640, 224, 1, 49, 0);
    if (events != std::vector<int>({0, 1, 1, 2, 3, 2, 3, 5})) return 6;
    if (get(80) != (16ull | 0x8000ull | (1ull << 60)) || get(448) != get(80)) return 7;
    // the clear flag is 16 bits: 0x10000 counts as zero, 0x8000 (negative) as nonzero
    reset(0x0000000100000001ull, 4);
    sceGsSetDefDBuffDc(db, 0, 640, 224, 1, 49, 0x10000);
    if (events.size() != 8) return 8;
    reset(0x0000000100000001ull, 4);
    sceGsSetDefDBuffDc(db, 0, 640, 224, 1, 49, 0x8000);
    if (events.size() != 10) return 9;
    // sign-extended 16-bit arguments
    reset(0x0000000100000001ull, 4);
    sceGsSetDefDBuffDc(db, 0x1000a, 0x10280, 0xffff & 0xfe40, 0x10003, 0x10031, 1);
    if (args[1] != 10 || args[2] != 640 || args[3] != -448 || args[4] != 0 || args[5] != 0) return 10;
    if (args[36] != 352 || args[38] != 2048 - 320 || args[39] != 2048 + 224 || args[40] != 640 || args[41] != -448) return 11;  // halves use arithmetic shift

    reset(0x0000000100000001ull, 4);
    sceGsSetDefDBuffDc(db, 0, 641, -447, 1, 49, 1);
    if (args[38] != 2048 - 320 || args[39] != 2048 + 224) return 14;       // odd negative height halves toward minus infinity

    // adjustment condition
    const unsigned long long cases[][2] = {
        {0x0000000100000001ull, 1}, {0xabcd0001abcd0001ull, 1}, {0x0000000200000001ull, 0}, {0x0000000100000002ull, 0},
        {0x0000000000000000ull, 1}, {0xffffffff00000000ull, 1}, {0x0000000100010000ull, 1}, {0x0000000500000005ull, 0},
        {0x0001000000000002ull, 0}, {0x0000000100000000ull, 1}};
    for (auto& c : cases) {
        reset(c[0], 0x1ff0 + 0x2);
        std::memcpy(before, db, sizeof db);
        sceGsSetDefDBuffDc(db, 0, 640, 224, 1, 49, 0);
        unsigned long long b96; std::memcpy(&b96, before + 96, 8);
        const unsigned long long expect = (b96 & ~0x1ffull) | ((0x1ff2 >> 1) & 0x1ff);
        if (c[1] ? get(96) != expect : get(96) != b96) return 12;
    }
    // negative address halves arithmetically
    reset(0, -4);
    std::memcpy(before, db, sizeof db);
    sceGsSetDefDBuffDc(db, 0, 640, 224, 1, 49, 0);
    unsigned long long b224; std::memcpy(&b224, before + 224, 8);
    if (get(224) != ((b224 & ~0x1ffull) | (static_cast<unsigned long long>(-4 >> 1) & 0x1ff))) return 13;
    return 0;
}
