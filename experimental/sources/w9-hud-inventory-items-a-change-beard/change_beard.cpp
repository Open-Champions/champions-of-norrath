// Observed changeBeard__Fii at 0x00280810, 188 bytes, in the pinned SLUS_205.65.
// (index, delta). Records of the targets table at 0x00437038 are 636 bytes; the
// signed byte at +27 is a count.
// delta 0: beardSetting[index] becomes -1 and the result is 0.
// delta > 0: if the setting (signed) is below count-2 it gains delta; result 1.
// delta < 0: if the setting is not negative it gains delta; result 1.
// Otherwise the result is 0.
// Unknown: what the count means (the caller is the character select screen).
extern "C" {
struct BeardTarget { unsigned char head[27]; signed char count; unsigned char tail[608]; };
extern BeardTarget targets[];
extern signed char beardSetting[];

int changeBeard__Fii(int index, int delta) {
    if (delta == 0) {
        beardSetting[index] = -1;
        return 0;
    }
    if (delta > 0) {
        if (beardSetting[index] < targets[index].count - 2) {
            beardSetting[index] = static_cast<signed char>(beardSetting[index] + delta);
            return 1;
        }
    }
    if (delta < 0) {
        if (beardSetting[index] >= 0) {
            beardSetting[index] = static_cast<signed char>(beardSetting[index] + delta);
            return 1;
        }
    }
    return 0;
}
}
