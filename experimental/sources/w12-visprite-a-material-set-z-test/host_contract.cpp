#include "set_z_test.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int enable = 0; enable < 3; ++enable) {
        for (unsigned start : {0u, 0xffffffffu, 0x2u, 0x5au}) {
            unsigned char object[64];
            memset(object, 0x33, sizeof object);
            memcpy(object + 12, &start, 4);
            unsigned char want[64];
            memcpy(want, object, sizeof object);
            unsigned expect = enable ? (start | 1u) : (start & ~1u);
            memcpy(want + 12, &expect, 4);
            unsigned one = 1;
            memcpy(want + 32, &one, 4);
            if (SetZTest__10VIMateriali(object, enable) != 0) return 1;
            if (memcmp(want, object, sizeof object) != 0) return 2;
        }
    }
    return 0;
}
