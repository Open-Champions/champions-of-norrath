// Host check only; not an EE build input. Build the source with -DsceGsSetDefDrawEnv=w12_de.
#include <cstdio>
#include <cstring>
extern "C" {
static int zcalls, za, zb, zc, zret;
int sceGszbufaddr(int a, int b, int c) { ++zcalls; za = a; zb = b; zc = c; return zret; }
int w12_de(unsigned long long*, int, int, int, int, int);
}
typedef unsigned long long u64;
int main() {
    u64 e[16];
    std::memset(e, 0, sizeof e);
    for (int i = 8; i < 16; i += 2) e[i] = 0x8000000000000000ull + i;   // pre-existing bits kept
    zret = 0x8c0;
    if (w12_de(e, 0, 650, 448, 1, 0x31) != 8) return 1;
    if (zcalls != 1 || za != 0 || zb != 650 || zc != 448) return 2;
    if (e[0] != (u64(11) << 16) || e[1] != 76) return 3;                    // (650+63)>>6 = 11
    if (e[2] != (0x8c0u | (u64(1) << 24)) || e[3] != 78) return 4;          // ztest != 0: no mask bit
    if (e[4] != ((u64(2048 - 325) << 4) | (u64(2048 - 224) << 36)) || e[5] != 24) return 5;
    if (e[6] != ((u64(649) << 16) | (u64(447) << 48)) || e[7] != 64) return 6;
    if (e[8] != (0x8000000000000008ull | 1) || e[9] != 26) return 7;
    if (e[10] != (0x800000000000000aull | 1) || e[11] != 70) return 8;
    if (e[12] != (0x800000000000000cull & ~1ull) || e[13] != 69) return 9;  // psm bit 1 clear
    if (e[14] != (u64(1) << 17 | 0x10000) || e[15] != 71) return 10;
    zret = -3;
    w12_de(e, 0x12, 0x10280, 0x10000 + 480, 0, 0x13);                       // arguments are 16-bit
    if (e[2] != ((u64)(long long)(short)-3 | (u64(3) << 24) | (u64(1) << 32))) return 11;
    if (e[12] != (0x800000000000000cull | 1)) return 12;                    // psm bit 1 set
    if (e[14] != 0x30000) return 13;                                       // ztest == 0
    if (e[0] != ((u64(((0x280 + 63) >> 6) & 0x3f) << 16) | (u64(2) << 24))) return 14;
    w12_de(e, 0, 640, 448, 6, 0);
    if (e[14] != (u64(2) << 17 | 0x10000)) return 15;                      // ztest & 3
    std::puts("sceGsSetDefDrawEnv ok");
    return 0;
}
