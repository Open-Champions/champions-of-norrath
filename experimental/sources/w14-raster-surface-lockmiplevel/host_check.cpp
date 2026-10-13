// Host check only; not a build input.
#include "lock_mip_level.cpp"
#include <cstdio>
int main() {
    alignas(8) unsigned char surface[512] = {};
    int* w = reinterpret_cast<int*>(surface);
    w[7] = -1; w[4] = 0x55; w[78] = 0x40000;
    for (int i = 0; i < 3; ++i) { w[(64 + 24 * i + 12) / 4] = 0x100 * (i + 1); w[(64 + 24 * i + 16) / 4] = 7 * (i + 1); }
    int a = 0, b = 0; unsigned c = 0;
    bool ok = LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 2, &a, &b, &c) == 0 && w[7] == 2 && w[8] == 1 && a == 21 && b == 0x55 && c == 0x40300;
    a = 1; b = 1; c = 1;
    ok = ok && LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &a, &b, &c) == -1 && w[7] == 2 && a == 1 && b == 1 && c == 1;   // already locked
    w[7] = -1;
    ok = ok && LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &a, &b, &c) == 0 && a == 7 && c == 0x40100 && w[7] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
