#include "set_tessellate.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int enable = 0; enable < 3; ++enable) {
        for (unsigned start : {0u, 0xffffffffu, 0x20u, 0x5au}) {
            unsigned char m[64];
            memset(m, 0x33, sizeof m);
            memcpy(m + 12, &start, 4);
            unsigned char want[64];
            memcpy(want, m, sizeof m);
            unsigned expect = enable ? (start | 0x20u) : (start & ~0x20u);
            memcpy(want + 12, &expect, 4);
            if (SetTessellate__10VIMateriali(m, enable) != 0) return 1;
            if (memcmp(want, m, sizeof m) != 0) return 2;
        }
    }
    return 0;
}
