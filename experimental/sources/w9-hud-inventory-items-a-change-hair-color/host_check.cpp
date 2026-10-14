// Host contract for changeHairColor__Fii. Not an EE plan input.
#include "change_hair_color.cpp"
extern "C" { HairColorTarget targets[10]; signed char hairColorSetting[16]; }
int main() {
    targets[2].count = 3;                              // range [0,2]
    if (changeHairColor__Fii(2, 1) != 1 || hairColorSetting[2] != 1) return 1;
    if (changeHairColor__Fii(2, 1) != 1 || hairColorSetting[2] != 2) return 2;
    if (changeHairColor__Fii(2, 1) != 0 || hairColorSetting[2] != 2) return 3;
    if (changeHairColor__Fii(2, -1) != 1 || hairColorSetting[2] != 1) return 4;
    if (changeHairColor__Fii(2, -1) != 1 || hairColorSetting[2] != 0) return 5;
    if (changeHairColor__Fii(2, -1) != 0 || hairColorSetting[2] != 0) return 6;
    hairColorSetting[2] = 2;
    if (changeHairColor__Fii(2, 0) != 0 || hairColorSetting[2] != 0) return 7;
    return 0;
}
