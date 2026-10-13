// Host contract for changeHairStyle__Fii. Not an EE plan input.
#include "change_hair_style.cpp"
extern "C" { HairStyleTarget targets[10]; signed char hairStyleSetting[16]; }
int main() {
    targets[3].count = 5; targets[3].flag = 0;       // range [0,4]
    targets[4].count = 5; targets[4].flag = 1;       // range [-1,3]
    hairStyleSetting[3] = 2;
    if (changeHairStyle__Fii(3, 1) != 1 || hairStyleSetting[3] != 3) return 1;
    if (changeHairStyle__Fii(3, 1) != 1 || hairStyleSetting[3] != 4) return 2;
    if (changeHairStyle__Fii(3, 1) != 0 || hairStyleSetting[3] != 4) return 3;
    hairStyleSetting[3] = 0;
    if (changeHairStyle__Fii(3, -1) != 0 || hairStyleSetting[3] != 0) return 4;
    hairStyleSetting[4] = 0;
    if (changeHairStyle__Fii(4, -1) != 1 || hairStyleSetting[4] != -1) return 5;
    if (changeHairStyle__Fii(4, -1) != 0 || hairStyleSetting[4] != -1) return 6;
    hairStyleSetting[4] = 2;
    if (changeHairStyle__Fii(4, 1) != 1 || hairStyleSetting[4] != 3) return 7;
    if (changeHairStyle__Fii(4, 1) != 0) return 8;
    if (changeHairStyle__Fii(4, 0) != 0 || hairStyleSetting[4] != 0) return 9;
    return 0;
}
