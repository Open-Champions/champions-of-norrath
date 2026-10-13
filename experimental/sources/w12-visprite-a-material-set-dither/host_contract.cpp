#include "set_dither.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int enable = 0; enable < 3; ++enable) {
        for (unsigned start : {0u, 0xffffffffu, 0x10u, 0x5au}) {
            unsigned char object[64];
            memset(object, 0x33, sizeof object);
            memcpy(object + 12, &start, 4);
            unsigned char want[64];
            memcpy(want, object, sizeof object);
            unsigned expect = enable ? (start | 0x10u) : (start & ~0x10u);
            memcpy(want + 12, &expect, 4);
            if (SetDither__10VIMateriali(object, enable) != 0) return 1;
            if (memcmp(want, object, sizeof object) != 0) return 2;
        }
    }
    return 0;
}
