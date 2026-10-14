// Host check only; not a build input.
static int calls; static unsigned char* seen;
extern "C" void ReorderPalette__9VISurface(unsigned char* s) { ++calls; seen = s; }
#include "lock_palette.cpp"
#include <cstdio>
static int& I(unsigned char* s, int off) { return *reinterpret_cast<int*>(s + off); }
int main() {
    alignas(4) unsigned char s[0x140] = {};
    int a = 11, b = 12; unsigned c = 13;
    I(s, 28) = 3;
    bool ok = LockPalette__9VISurfaceRiR12VIPixelStorePPUc(s, &a, &b, &c) == -1 && calls == 0 && a == 11 && b == 12 && c == 13 && I(s, 32) == 0;
    I(s, 28) = -1; I(s, 64) = 9; I(s, 40) = 0; I(s, 16) = 7; I(s, 44) = 1000; I(s, 52) = 5; I(s, 312) = 4000;
    ok = ok && LockPalette__9VISurfaceRiR12VIPixelStorePPUc(s, &a, &b, &c) == 0 && calls == 1 && seen == s && I(s, 28) == 9 && I(s, 32) == 1
        && a == 0 && b == 7 && c == 0;
    I(s, 28) = -1; I(s, 40) = 256;
    ok = ok && LockPalette__9VISurfaceRiR12VIPixelStorePPUc(s, &a, &b, &c) == 0 && calls == 2 && a == 256 && b == 5 && c == 5000;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
