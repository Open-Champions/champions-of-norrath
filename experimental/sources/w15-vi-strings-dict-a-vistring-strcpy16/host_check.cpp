#include "strcpy16.cpp"
#include <string.h>
int main() {
    const unsigned short a[] = {0};
    const unsigned short b[] = {0x41, 0};
    const unsigned short c[] = {0x41, 0x4242, 0xffff, 0x20ac, 0};
    const unsigned short* srcs[] = {a, b, c};
    const unsigned lens[] = {0, 1, 4};
    for (int si = 0; si < 3; ++si) {
        for (unsigned limit = 0; limit < 14; ++limit) {
            unsigned short buf[10]; for (auto& x : buf) x = 0x5a5a;
            unsigned short* r = strcpy16__8VIStringPUsUiPCUs(buf + 1, limit, srcs[si]);
            if (r != buf + 1) return 1;
            unsigned k = 0;
            while (k < lens[si] && 2 * (k + 1) < limit) ++k;
            for (unsigned i = 0; i < 10; ++i) {
                unsigned short want = 0x5a5a;
                if (i >= 1 && i < 1 + k) want = srcs[si][i - 1];
                else if (i == 1 + k) want = 0;
                if (buf[i] != want) return 2;
            }
        }
    }
    return 0;
}
