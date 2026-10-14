// Host check only; not a build input.
#include "calc_surface_offsets.cpp"
#include <cstdio>
#include <cstring>
static int strideCalls, strideFail;
extern "C" int CalcMipLevelStride__9VISurfacei(unsigned char*, int w) { ++strideCalls; return w == strideFail ? -1 : w * 4; }
static int& at(int* s, int off) { return s[off / 4]; }
int main() {
    int s[100];
    for (int i = 0; i < 100; ++i) s[i] = 0x7777;
    unsigned char* self = reinterpret_cast<unsigned char*>(s);
    at(s, 8) = 64; at(s, 12) = 64; at(s, 52) = 2; at(s, 40) = 16; at(s, 64) = 3;
    strideCalls = 0; strideFail = 0;
    bool ok = CalcSurfaceOffsets__9VISurface(self) == 0 && strideCalls == 3;
    ok = ok && at(s, 44) == 64 && at(s, 48) == 128 && at(s, 56) == 0 && at(s, 60) == 8;
    ok = ok && at(s, 68) == 64 && at(s, 72) == 64 && at(s, 76) == 192 && at(s, 80) == 256 && at(s, 84) == 128 && at(s, 88) == 1028;
    ok = ok && at(s, 92) == 32 && at(s, 96) == 32 && at(s, 100) == 16640 && at(s, 104) == 128 && at(s, 108) == 16576 && at(s, 112) == 260;
    ok = ok && at(s, 116) == 16 && at(s, 120) == 16 && at(s, 124) == 20736 + 64 && at(s, 128) == 64 && at(s, 132) == 20736 && at(s, 136) == 68;
    ok = ok && at(s, 308) == 21824 && at(s, 64) == 3;
    // 16-bit palette of 5 entries: 10 bytes -> 16 + 64; unsupported kind: no palette block; small surface: no levels
    at(s, 52) = 5; at(s, 40) = 5; at(s, 64) = 1; strideCalls = 0;
    ok = ok && CalcSurfaceOffsets__9VISurface(self) == 0 && at(s, 48) == 80 && at(s, 60) == 5 && at(s, 64) == 1 && strideCalls == 1 && at(s, 84) == 80;
    at(s, 52) = 1; at(s, 40) = 99;
    ok = ok && CalcSurfaceOffsets__9VISurface(self) == 0 && at(s, 44) == 0 && at(s, 48) == 0 && at(s, 60) == 0 && at(s, 56) == 0 && at(s, 84) == 0 && at(s, 76) == 64;
    at(s, 8) = 15; at(s, 64) = 4; strideCalls = 0;
    ok = ok && CalcSurfaceOffsets__9VISurface(self) == 0 && strideCalls == 0 && at(s, 64) == 0 && at(s, 308) == 0;
    at(s, 8) = 64; at(s, 64) = 4; at(s, 308) = 1234; strideFail = 64;
    ok = ok && CalcSurfaceOffsets__9VISurface(self) == -1 && at(s, 308) == 1234 && at(s, 64) == 4;   // neither written on failure
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
