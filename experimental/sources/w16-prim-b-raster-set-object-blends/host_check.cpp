// Host check only; not a build input. DMA addresses are arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <string.h>
unsigned ObjectBlendsBuffer __asm__("_8VIRaster$ObjectBlendsBuffer");
#include "set_object_blends.cpp"

alignas(16) static unsigned char arena[0x40000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    static unsigned char mats[100 * 64];
    for (unsigned i = 0; i < sizeof mats; ++i) mats[i] = static_cast<unsigned char>(i * 7 + 3);
    for (int n : {0, 1, 5, 31, 32, 33, 64, 70, 100}) {
        memset(arena, 0xee, sizeof arena);
        ObjectBlendsBuffer = 0x5000;
        unsigned char* o = arena + 0x30000;
        unsigned slot = 0x100;
        memcpy(o + 19492 - 0x0, &slot, 4);
        static unsigned char want[0x40000];
        memcpy(want, arena, sizeof want);
        int chunks = n / 32, rest = n - chunks * 32;
        int words = n * 4 + chunks + 1;
        want[slot + 3] = 16;
        memset(want + slot + 4, 0, 4);
        unsigned short w16 = static_cast<unsigned short>(words);
        memcpy(want + slot, &w16, 2);
        unsigned newslot = slot + 16 + words * 16;
        memcpy(want + 0x30000 + 19492, &newslot, 4);
        unsigned w = slot + 16;
        int done = 0;
        for (int c = 0; c < chunks; ++c) {
            unsigned h[4] = {0, 0, 0x10000000u, (0x5000u + done * 4) | 0x6c800000u};
            memcpy(want + w, h, 16);
            memcpy(want + w + 16, mats + done * 64, 2048);
            w += 2064;
            done += 32;
        }
        if (rest) {
            unsigned h[4] = {0, 0, 0x10000000u, (0x5000u + done * 4) | (rest << 18) | 0x6c000000u};
            h[1] = 0;
            memcpy(want + w, h, 16);
            memcpy(want + w + 16, mats + done * 64, rest * 64);
        }
        if (SetObjectBlends__8VIRasteriP10VIMatrix44(o, n, mats) != 0) return 1;
        if (memcmp(arena, want, sizeof want) != 0) return 2;
    }
    return 0;
}
