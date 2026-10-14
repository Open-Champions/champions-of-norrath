// Observed hudFinishGold__Fv at 0x001c44f8, 216 bytes.
// Three passes, each over objects whose layouts are only partly known.
// 1. Eight 36-byte records starting at goldNumber (0x007d0588). A nonzero word
//    at record offset 28 is cleared and added to the word at offset 232 of the
//    64-bit-aligned player record selected by the signed byte at record offset
//    32 (player records are 23984 bytes apart, base 0x00408600). Records whose
//    word is zero are skipped.
// 2. Four words at countUp (0x007d0900). A positive entry is cleared and added
//    to offset 232 of player record i. Zero and negative entries are skipped.
// 3. Four words each at goldAlpha (0x007d08e0) and playerGettinIt (0x007d08f0)
//    are set to zero.
// What offset 232 holds is not established here (the additions suggest a
// running total). None of the four objects is defined in this unit.
extern "C" unsigned char goldNumber[];
extern "C" unsigned char player[];
extern "C" int countUp[];
extern "C" int goldAlpha[];
extern "C" int playerGettinIt[];

static int* player_word_232(int index) {
    return reinterpret_cast<int*>(player + index * 23984 + 232);
}

extern "C" void hudFinishGold__Fv() {
    unsigned char* record = goldNumber;
    for (int n = 0; n < 8; ++n, record += 36) {
        int amount = *reinterpret_cast<int*>(record + 28);
        if (amount == 0) continue;
        signed char owner = static_cast<signed char>(record[32]);
        *reinterpret_cast<int*>(record + 28) = 0;
        *player_word_232(owner) += amount;
    }
    for (int i = 0; i < 4; ++i) {
        int amount = countUp[i];
        if (amount <= 0) continue;
        countUp[i] = 0;
        *player_word_232(i) += amount;
    }
    for (int i = 0; i < 4; ++i) {
        goldAlpha[i] = 0;
        playerGettinIt[i] = 0;
    }
}
