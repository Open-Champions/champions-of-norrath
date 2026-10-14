#include "end_prim_group.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int n : {0, 1, 41, -1, 0x7fffffff}) {
        unsigned char o[64], want[64];
        memset(o, 0x21, sizeof o);
        memcpy(o + 44, &n, 4);
        memcpy(want, o, sizeof o);
        int expect = static_cast<int>(static_cast<unsigned>(n) + 1u);
        memcpy(want + 44, &expect, 4);
        if (EndPrimGroup__12VICollBuffer(o) != 0) return 1;
        if (memcmp(o, want, sizeof o) != 0) return 2;
    }
    return 0;
}
