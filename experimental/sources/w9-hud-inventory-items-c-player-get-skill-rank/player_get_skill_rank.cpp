// Observed playerGetSkillRank__Fii at 0x001ac260, 92 bytes.
// Arguments: player number and a skill number. Starting at record+23712 of the
// 23984-byte player record, it walks pairs of signed halfwords (4 bytes each).
// The walk ends at the first pair whose first halfword is -1 and then returns
// 0; a pair whose first halfword equals the skill number returns its second
// halfword (sign-extended).
// Unknown: what the pairs mean (the name follows the symbol, not a decoded
// table) and how many pairs the record can hold.
extern "C" unsigned char player[];

extern "C" int playerGetSkillRank__Fii(int owner, int skill) {
    const short* pair = reinterpret_cast<const short*>(player + owner * 23984 + 23712);
    for (; pair[0] != -1; pair += 2) {
        if (pair[0] == skill) return pair[1];
    }
    return 0;
}
