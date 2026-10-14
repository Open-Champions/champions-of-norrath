#include "set_trail_width.cpp"
#include <string.h>

extern "C" unsigned char trail[64 * 1040];
unsigned char trail[64 * 1040];

int main() {
    memset(trail, 0x33, sizeof trail);
    *reinterpret_cast<short*>(trail + 5 * 1040 + 1024) = 0x305;
    P_SetTrailWidth__Fii(0x305, 40);
    if (trail[5 * 1040 + 1039] != 10) return 1;
    P_SetTrailWidth__Fii(0x305, -9);                    // arithmetic shift: -3 -> 0xfd
    if (trail[5 * 1040 + 1039] != 0xfd) return 2;
    trail[5 * 1040 + 1039] = 0x33;
    P_SetTrailWidth__Fii(0x105, 40);                    // same record, stale stamp
    if (trail[5 * 1040 + 1039] != 0x33) return 3;
    P_SetTrailWidth__Fii(0, 40);                        // id 0 is ignored
    *reinterpret_cast<short*>(trail + 1024) = 0;
    P_SetTrailWidth__Fii(0, 40);
    if (trail[1039] != 0x33) return 4;
    *reinterpret_cast<short*>(trail + 63 * 1040 + 1024) = -193;   // 0xff3f: low byte 63
    P_SetTrailWidth__Fii(-193, 1000);
    if (trail[63 * 1040 + 1039] != 250) return 6;
    trail[63 * 1040 + 1039] = 0x33;
    for (int i = 0; i < 64; i++) {
        if (i == 5) continue;
        if (trail[i * 1040 + 1039] != 0x33) return 5;
    }
    return 0;
}
