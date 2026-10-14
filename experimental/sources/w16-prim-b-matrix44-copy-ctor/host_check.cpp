#include "ctor.cpp"
#include <string.h>

int main() {
    alignas(16) float src[16], dst[16];
    for (int i = 0; i < 16; ++i) { src[i] = 1.5f * i - 3.0f; dst[i] = 99.0f; }
    __10VIMatrix44RC10VIMatrix44(dst, src);
    if (memcmp(dst, src, sizeof src) != 0) return 1;
    // overlapping source: the zero fill happens first, so the object ends up all zero
    alignas(16) float self[16];
    for (int i = 0; i < 16; ++i) self[i] = 2.0f + i;
    __10VIMatrix44RC10VIMatrix44(self, self);
    for (int i = 0; i < 16; ++i)
        if (self[i] != 0.0f) return 2;
    return 0;
}
