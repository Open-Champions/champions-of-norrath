#include "global_init.cpp"
#include <cstdint>
#include <initializer_list>
#include <cstring>
alignas(16) unsigned char player[4 * 23984];
struct Call {
    void* self;
    int i[8];
    unsigned f[3];
    int t[3];
    void (*cb)(void*);
};
static Call calls[8];
static int n;
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* s, int a, int b, int c, int d, int e, int f, int g, int h,
                                                    float x, float y, float z, int i, int j, int k,
                                                    void (*callback)(void*)) {
    if (n >= 8) return;
    Call& c0 = calls[n++];
    c0.self = s;
    int v[8] = {a, b, c, d, e, f, g, h};
    for (int q = 0; q < 8; ++q) c0.i[q] = v[q];
    float fl[3] = {x, y, z};
    for (int q = 0; q < 3; ++q) std::memcpy(&c0.f[q], &fl[q], 4);
    c0.t[0] = i; c0.t[1] = j; c0.t[2] = k;
    c0.cb = callback;
}
static bool tuple(int idx, const int (&ints)[8], const unsigned (&floats)[3], int a, int b, int c) {
    const Call& k = calls[idx];
    for (int q = 0; q < 8; ++q) if (k.i[q] != ints[q]) return false;
    for (int q = 0; q < 3; ++q) if (k.f[q] != floats[q]) return false;
    return k.t[0] == a && k.t[1] == b && k.t[2] == c && k.cb == 0;
}
int main() {
    std::memset(player, 0xaa, sizeof player);
    _GLOBAL_$I$nmPlayers();
    // Expected element pattern, built independently of the source loops.
    unsigned char want[72];
    std::memset(want, 0xaa, sizeof want);
    auto put16 = [&](int off, unsigned short v) { std::memcpy(want + off, &v, 2); };
    for (int off : {36, 44, 52, 60}) put16(off, 0xffff);
    want[32] = want[33] = 0xff;
    for (int off : {16, 18, 20, 24, 26, 28}) put16(off, 0);
    for (int r = 0; r < 4; ++r) {
        unsigned char* rec = player + r * 23984;
        for (int i = 0; i < 328; ++i) if (rec[i] != 0xaa) return 1;
        for (int b = 0; b < 3; ++b) {
            unsigned char* blk = rec + 328 + b * 4616;
            for (int e = 0; e < 64; ++e)
                if (std::memcmp(blk + e * 72, want, 72) != 0) return 2;
            for (int i = 64 * 72; i < 4616; ++i) if (blk[i] != 0xaa) return 3;
        }
        for (int i = 328 + 3 * 4616; i < 23984; ++i) if (rec[i] != 0xaa) return 4;
    }
    if (n != 6) return 5;
    for (int q = 0; q < 6; ++q) for (int p = q + 1; p < 6; ++p) if (calls[q].self == calls[p].self) return 6;
    if (!tuple(0, {200, 200, 150, 300, 200, 175, 100, 0}, {0x40000000, 0x40a00000, 0xc1a00000}, 1, 2, 2)) return 7;
    if (!tuple(1, {57, 59, 100, 154, 56, 43, 100, -74}, {0x4141ba5e, 0x4114b851, 0xc063e76c}, 1, 1, 48)) return 8;
    if (!tuple(2, {100, 57, 59, 154, 100, 56, 43, -74}, {0x4141ba5e, 0x4114b851, 0xc063e76c}, 1, 1, 9)) return 9;
    if (!tuple(3, {255, 255, 255, 255, 0, 0, 0, 0}, {0x41000000, 0x40400000, 0xc0a00000}, 1, 1, 0)) return 10;
    if (!tuple(4, {100, 44, 0, 23, -77, -54, 0, 0}, {0x41d00000, 0x41a80000, 0xc07851eb}, 2, 0, 5)) return 11;
    if (!tuple(5, {228, 150, 150, 150, 255, 0, 0, 0}, {0xc1000000, 0x41700000, 0x80000000}, 2, 2, 6)) return 12;
    return 0;
}
