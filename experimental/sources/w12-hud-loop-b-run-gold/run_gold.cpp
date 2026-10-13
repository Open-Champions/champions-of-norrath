// Observed runGold__Fv at 0x001c4a50, 196 bytes (a file-local function in the image, called from the
// HUD frame code). No arguments, no result.
//  1. Eight 36-byte records start at goldNumber. For each record whose word at +28 is nonzero and
//     for which (frameNm - word at +24) * 3 is 128 or more (signed), the +28 word is cleared and
//     added to countUp[owner], owner being the signed byte at +32.
//  2. For each of the four players, a positive countUp[i] gives step = (countUp[i] + 7) >> 3
//     (arithmetic shift): the word at offset 232 of the 23984-byte player record i rises by step
//     and countUp[i] falls by step.
// Inferred: record +24 is a start frame, +28 an amount and +32 the receiving player index; countUp
// holds the gold still to be counted into each player's total, so the player's figure climbs by
// an eighth of the remainder (rounded up) per call.
// Unknown: the meaning of the 128 / 3 frame limit beyond this test, and the purpose of owners
// outside 0..3 (the index is not range-checked).
extern "C" {
extern unsigned char goldNumber[];
extern unsigned char player[];
extern int countUp[];
extern int frameNm;

void runGold__Fv() {
    unsigned char* record = goldNumber;
    for (int n = 0; n < 8; ++n, record += 36) {
        const int amount = *reinterpret_cast<int*>(record + 28);
        if (amount == 0) continue;
        if ((frameNm - *reinterpret_cast<int*>(record + 24)) * 3 < 128) continue;
        *reinterpret_cast<int*>(record + 28) = 0;
        countUp[static_cast<signed char>(record[32])] += amount;
    }
    for (int i = 0; i < 4; ++i) {
        const int remaining = countUp[i];
        if (remaining <= 0) continue;
        const int step = (remaining + 7) >> 3;
        *reinterpret_cast<int*>(player + i * 23984 + 232) += step;
        countUp[i] = remaining - step;
    }
}
}
