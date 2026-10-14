// Observed getHairStyle__Fi at 0x002808d0, 52 bytes, no frame. It returns the sum of two signed bytes for the
// character index in the argument: hairStyleSetting[character] (a byte array at 0x008d5b38) and the byte at offset
// 18 of the character's 636-byte record in the `targets` table (0x00437038).
// Inferred: the sum is the effective hair style (a per-character setting plus a base value in the target record).
// Unknown: the name of the target-record field at +18 and whether the sum is range checked by its callers.
extern "C" {
extern signed char hairStyleSetting[];
extern unsigned char targets[];

int getHairStyle__Fi(int character) {
    return hairStyleSetting[character] + static_cast<signed char>(targets[636 * character + 18]);
}
}
