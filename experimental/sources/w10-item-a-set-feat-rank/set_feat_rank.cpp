// Observed playerSetFeatRank__Fixi at 0x001abbe0, 128 bytes (int index, 64-bit
// feat id, int rank). The player record is player + index * 23984 (no index
// range check). The word at record+80 selects a class row of gSkillTree, which
// starts at gSkillTree + 40 + row * 3592 and holds 64 entries of 56 bytes (the
// same addressing playerGetFeatRank__Fix uses). The first 8 bytes of each entry
// are compared with the feat id. For the first matching entry i, the signed byte
// at record+243+i is replaced by `rank` only if it is currently smaller (signed
// compare); a stored value is the low byte of `rank`. Later entries are never
// examined and no match changes nothing. Returns nothing.
// Unknown: what the rank bytes mean beyond being a per-entry counter.
extern "C" {
extern unsigned char player[];
extern unsigned char gSkillTree[];

void playerSetFeatRank__Fixi(int index, long long feat, int rank) {
    unsigned char* record = player + index * 23984;
    const unsigned char* entry = gSkillTree + 40 + *(int*)(record + 80) * 3592;
    signed char* slot = (signed char*)(record + 243);
    for (int i = 0; i < 64; ++i, entry += 56, ++slot) {
        if (*(const long long*)entry == feat) {
            if (*slot < rank) *slot = (signed char)rank;
            return;
        }
    }
}
}
