// Host check only; not a build input.
#include "effectvolume_surface.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) unsigned char object[2048];
    std::memset(object, 0x5a, sizeof object);
    for (int k = 0; k < 4; ++k) {
        int v = 0x7000 + k;
        std::memcpy(object + 64 + 4 * k, &v, 4);
    }
    for (int k = 0; k < 4; ++k)
        if (Surface__C14VIEffectVolumei(object, k) != 0x7000 + k) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
