// Observed playerSetSkillRank__Fiii at 0x001ac2c0, 148 bytes (player number,
// skill number, rank). Starting at record+23712 of the 23984-byte player record
// it walks pairs of signed halfwords (4 bytes each), the same list
// playerGetSkillRank__Fii reads. If the first halfword of a pair equals the
// skill number, that pair's second halfword is set to the low 16 bits of `rank`.
// If the walk reaches a pair whose first halfword is -1, that pair becomes
// (skill number, rank) in place; the pair after it is not written, so the list
// does not gain a new terminator. Nothing is returned.
// Unknown: the capacity of the list; the image checks none.
extern "C" unsigned char player[];

extern "C" void playerSetSkillRank__Fiii(int owner, int skill, int rank) {
    short* pair = reinterpret_cast<short*>(player + owner * 23984 + 23712);
    while (pair[0] != -1) {
        if (pair[0] == skill) {
            pair[1] = static_cast<short>(rank);
            return;
        }
        pair += 2;
    }
    pair[0] = static_cast<short>(skill);
    pair[1] = static_cast<short>(rank);
}
