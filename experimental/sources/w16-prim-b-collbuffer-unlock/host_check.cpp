#include "unlock.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int start : {0, 1, -1, 7}) {
        unsigned char o[32];
        memset(o, 0x44, sizeof o);
        memcpy(o + 12, &start, 4);
        unsigned char want[32];
        memcpy(want, o, sizeof o);
        int expect_ret = start ? 0 : -1;
        int zero = 0;
        if (start) memcpy(want + 12, &zero, 4);
        if (Unlock__12VICollBuffer(o) != expect_ret) return 1;
        if (memcmp(o, want, sizeof o) != 0) return 2;
    }
    return 0;
}
