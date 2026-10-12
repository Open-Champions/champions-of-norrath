// Observed playerGetFeatRank__Fix at 0x001abb48, 148 bytes (int index, 64-bit
// feat id). An index of 5 or more (unsigned compare) returns 0. Otherwise the
// player record is player + index * 23984; the word at record+80 selects a
// class row. Row r of gSkillTree starts at gSkillTree + 40 + r * 3592 and holds
// 64 entries of 56 bytes; the first 8 bytes of each entry are compared with the
// feat id. For the first matching entry i, the signed byte at record+243+i is
// returned, with a negative byte reported as 0. No match returns 0. The class
// word is not range checked. What the other entry fields or the rank bytes
// otherwise hold is not established.
extern "C" {
extern unsigned char player[];
extern unsigned char gSkillTree[];

int playerGetFeatRank__Fix(int index, long long feat) {
    if ((unsigned)index >= 5u) return 0;
    unsigned char* record = player + index * 23984;
    int row = *(int*)(record + 80);
    const unsigned char* entry = gSkillTree + 40 + row * 3592;
    const signed char* rank = (const signed char*)(record + 243);
    for (int i = 0; i < 64; ++i, entry += 56, ++rank) {
        if (*(const long long*)entry == feat) {
            int value = *rank;
            return value < 0 ? 0 : value;
        }
    }
    return 0;
}
}
