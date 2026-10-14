// Host contract for changeBeard__Fii. Not an EE plan input.
#include "change_beard.cpp"
extern "C" { BeardTarget targets[10]; signed char beardSetting[16]; }
int main() {
    targets[1].count = 4;                              // setting rises while below 2
    beardSetting[1] = -1;
    if (changeBeard__Fii(1, 1) != 1 || beardSetting[1] != 0) return 1;
    if (changeBeard__Fii(1, 1) != 1 || beardSetting[1] != 1) return 2;
    if (changeBeard__Fii(1, 1) != 1 || beardSetting[1] != 2) return 3;
    if (changeBeard__Fii(1, 1) != 0 || beardSetting[1] != 2) return 4;
    beardSetting[1] = 0;
    if (changeBeard__Fii(1, -1) != 1 || beardSetting[1] != -1) return 5;
    if (changeBeard__Fii(1, -1) != 0 || beardSetting[1] != -1) return 6;
    beardSetting[1] = 2;
    if (changeBeard__Fii(1, 0) != 0 || beardSetting[1] != -1) return 7;
    return 0;
}
