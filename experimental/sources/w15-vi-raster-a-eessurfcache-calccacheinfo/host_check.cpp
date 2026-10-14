// Host check only; not a build input.
#include "calc_cache_info.cpp"
#include <cstdio>
#include <cstring>
struct Fixture { int s[20]; int d[32]; };
static int run(Fixture& f) { return CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(nullptr, reinterpret_cast<unsigned char*>(f.s), reinterpret_cast<unsigned char*>(f.d)); }
int main() {
    bool ok = true;
    Fixture f;
    // type 0 selects format 20; 256x256 with 3 levels: 256*256/2 = 32768, 128*128/2 = 8192, then 64x64 (width <= 128: width
    // padded to 128, height kept) 128*64/2 = 4096; the sum 45056 rounds up to 49152.
    std::memset(&f, 0, sizeof f);
    f.s[2] = 256; f.s[3] = 256; f.s[4] = 0; f.s[16] = 3;
    ok = ok && run(f) == 0 && f.d[5] == 20 && f.d[8] == 0 && f.d[9] == 32768 && f.d[10] == 32768 + 8192 && f.d[7] == 3
        && f.d[6] == 49152 && f.d[18] == -1 && f.d[19] == -1 && f.d[20] == -1 && f.d[1] == 0 && f.d[2] == 0;
    // palette: mode 3, 10 entries -> 40 bytes -> 256
    std::memset(&f, 0, sizeof f);
    f.s[2] = 16; f.s[3] = 16; f.s[4] = 3; f.s[10] = 10; f.s[13] = 3; f.s[16] = 1;
    ok = ok && run(f) == 0 && f.d[5] == 1 && f.d[1] == 1 && f.d[2] == 256 && f.d[7] == 1 && f.d[6] == 8192;   // 64*32*4 = 8192
    // unsupported palette mode / type
    std::memset(&f, 0, sizeof f); f.s[10] = 1; f.s[13] = 7; ok = ok && run(f) == -1;
    std::memset(&f, 0, sizeof f); f.s[4] = 4; ok = ok && run(f) == -1;
    std::memset(&f, 0, sizeof f); f.s[4] = 9; ok = ok && run(f) == -1;
    // too small to hold any level: only rounding of zero
    std::memset(&f, 0, sizeof f); f.s[2] = 8; f.s[3] = 64; f.s[4] = 2; f.s[16] = 4;
    ok = ok && run(f) == 0 && f.d[7] == 0 && f.d[6] == 0 && f.d[5] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
