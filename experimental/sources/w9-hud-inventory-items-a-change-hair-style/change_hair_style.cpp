// Observed changeHairStyle__Fii at 0x0027ec78, 212 bytes, in the pinned SLUS_205.65.
// (index, delta). The targets table is 636-byte records at 0x00437038; for record
// index the signed byte at +18 is a flag and the signed byte at +25 is a count.
// delta 0: hairStyleSetting[index] becomes 0 and the result is 0.
// Otherwise the permitted range is [min, max]: with the +18 flag clear min is 0 and
// max is count-1; with it set min is -1 and max is count-2.
// delta > 0: if the setting (signed) is below max it gains delta and the result is 1.
// delta < 0: if min is below the setting it gains delta and the result is 1.
// Otherwise the result is 0. The sum is stored as a byte, without a bound check on delta.
// Unknown: what the flag and the count mean (the caller is the character select screen).
extern "C" {
struct HairStyleTarget { unsigned char head[18]; signed char flag; unsigned char gap0[6]; signed char count; unsigned char tail[610]; };
extern HairStyleTarget targets[];
extern signed char hairStyleSetting[];

int changeHairStyle__Fii(int index, int delta) {
    if (delta == 0) {
        hairStyleSetting[index] = 0;
        return 0;
    }
    const HairStyleTarget& target = targets[index];
    int min = 0;
    int max = target.count - 1;
    if (target.flag != 0) {
        max = target.count - 2;
        min = -1;
    }
    if (delta > 0) {
        if (hairStyleSetting[index] < max) {
            hairStyleSetting[index] = static_cast<signed char>(hairStyleSetting[index] + delta);
            return 1;
        }
    }
    if (delta < 0) {
        if (min < hairStyleSetting[index]) {
            hairStyleSetting[index] = static_cast<signed char>(hairStyleSetting[index] + delta);
            return 1;
        }
    }
    return 0;
}
}
