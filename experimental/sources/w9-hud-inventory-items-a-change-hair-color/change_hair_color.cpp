// Observed changeHairColor__Fii at 0x00280760, 172 bytes, in the pinned SLUS_205.65.
// (index, delta). Records of the targets table at 0x00437038 are 636 bytes; the
// signed byte at +26 is a count.
// delta 0: hairColorSetting[index] becomes 0 and the result is 0.
// delta > 0: if the setting (signed) is below count-1 it gains delta; result 1.
// delta < 0: if the setting is above 0 it gains delta; result 1.
// Otherwise the result is 0.
// Unknown: what the count means (the caller is the character select screen).
extern "C" {
struct HairColorTarget { unsigned char head[26]; signed char count; unsigned char tail[609]; };
extern HairColorTarget targets[];
extern signed char hairColorSetting[];

int changeHairColor__Fii(int index, int delta) {
    if (delta == 0) {
        hairColorSetting[index] = 0;
        return 0;
    }
    const int max = targets[index].count - 1;
    if (delta > 0) {
        if (hairColorSetting[index] < max) {
            hairColorSetting[index] = static_cast<signed char>(hairColorSetting[index] + delta);
            return 1;
        }
    }
    if (delta < 0) {
        if (hairColorSetting[index] > 0) {
            hairColorSetting[index] = static_cast<signed char>(hairColorSetting[index] + delta);
            return 1;
        }
    }
    return 0;
}
}
