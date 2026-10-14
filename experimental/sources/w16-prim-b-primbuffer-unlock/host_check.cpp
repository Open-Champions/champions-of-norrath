#include <initializer_list>
#include "unlock.cpp"
#include <string.h>

static unsigned bits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

int main() {
    if (bits(0.0078125f) != 0x3c000000u || bits(1.0f / 127.0f) != 0x3c010204u) return 9;
    for (int mode : {0, 1, 2, 3, 4, 5, 6, 8, 9, -1}) {
        for (int lock : {0, 1, 7}) {
            unsigned char o[80];
            memset(o, 0x44, sizeof o);
            memcpy(o + 8, &mode, 4);
            memcpy(o + 12, &lock, 4);
            unsigned char want[80];
            memcpy(want, o, sizeof o);
            int ret = -1;
            if (lock) {
                ret = 0;
                unsigned v60 = mode == 2 || mode == 4 || mode == 8 ? 0x3c000000u : mode == 5 ? 0x3c010204u : 0x3f800000u;
                memcpy(want + 60, &v60, 4);
                if (mode == 8) { unsigned v = 0x3c000000u; memcpy(want + 56, &v, 4); }
                int z = 0;
                memcpy(want + 12, &z, 4);
            }
            if (Unlock__12VIPrimBuffer(o) != ret) return 1;
            if (memcmp(o, want, sizeof o) != 0) return 2;
        }
    }
    return 0;
}
