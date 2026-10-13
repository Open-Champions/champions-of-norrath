// Host check only; not an EE build input. Build the source with -DsceGsSetDefClear=w12_clr.
#include <cstdio>
extern "C" int w12_clr(unsigned long long*, int, int, int, int, int, int, int, int, int, unsigned);
typedef unsigned long long u64;
int main() {
    u64 e[12];
    if (w12_clr(e, 0, 100, 200, 640, 448, 0x11, 0x22, 0x133, 0x44, 0x80000000u) != 6) return 1;
    if (e[0] != 0x30000 || e[1] != 71 || e[2] != 6 || e[3] != 0) return 2;
    if (e[4] != (0x3f80000000000000ull | 0x44221100ull + 0x330000ull - 0x330000ull + 0x330000ull - 0x330000ull | (u64(0x33) << 16)
                 )) { /* blue masked to 0x33 */ }
    const u64 colour = 0x3f80000000000000ull | 0x11u | 0x22u << 8 | u64(0x33) << 16 | u64(0x44) << 24;
    if (e[4] != colour || e[5] != 1) return 3;
    const u64 q = u64(0x80000000u) << 32;
    if (e[6] != ((u64(100 << 4)) | (u64(200 << 4) << 16) | q) || e[7] != 5) return 4;
    if (e[8] != ((u64(740 << 4)) | (u64(648 << 4) << 16) | q) || e[9] != 5) return 5;
    if (e[10] != 0x30000 || e[11] != 71) return 6;
    w12_clr(e, 3, -1, 0x10001, 1, 1, 0, 0, 0, 0, 0);                   // 16-bit sign extension
    if (e[6] != ((u64)(long long)(-16) | (u64(16) << 16))) return 8;     // sign bits of x spill upward
    if (e[8] != (u64(0) | (u64(32) << 16))) return 9;                  // x + width == 0
    if (e[10] != (u64(3) << 17 | 0x10000)) return 10;
    w12_clr(e, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (e[10] != (u64(3) << 17 | 0x10000)) return 11;                   // ztest & 3
    std::puts("sceGsSetDefClear ok");
    return 0;
}
