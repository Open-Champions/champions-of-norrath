// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313780 (8 bytes):
// return the second integer argument unchanged; the receiver and first integer are not read.
struct Skill;
extern "C" int tweek_rank(const Skill*, int, int) asm("tweekRank__5Skillii");
extern "C" int tweek_rank(const Skill*, int, int rank) {
    return rank;
}
