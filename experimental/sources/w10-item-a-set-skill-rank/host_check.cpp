// Host check for playerSetSkillRank__Fiii: update, append at the -1 pair,
// no terminator written, other record untouched, halfword truncation.
extern "C" void playerSetSkillRank__Fiii(int, int, int);
extern "C" int playerGetSkillRank__Fii(int, int);
extern "C" unsigned char player[3 * 23984];
unsigned char player[3 * 23984];
static short* pairs(int owner) { return reinterpret_cast<short*>(player + owner * 23984 + 23712); }
static void put(int owner, int index, short id, short value) { pairs(owner)[2 * index] = id; pairs(owner)[2 * index + 1] = value; }
extern "C" int playerGetSkillRank__Fii(int owner, int skill) {
    const short* p = pairs(owner);
    for (; p[0] != -1; p += 2) if (p[0] == skill) return p[1];
    return 0;
}
#include "set_skill_rank.cpp"
int main() {
    put(1, 0, 4, 10); put(1, 1, 9, -3); put(1, 2, -1, 0x1234); put(1, 3, 77, 77);
    put(0, 0, -1, 0); put(0, 1, 5, 5);
    put(2, 0, 4, 1); put(2, 1, -1, 0);
    playerSetSkillRank__Fiii(1, 9, 42);                 // update existing
    if (pairs(1)[3] != 42 || pairs(1)[2] != 9) return 1;
    if (playerGetSkillRank__Fii(1, 9) != 42 || playerGetSkillRank__Fii(1, 4) != 10) return 2;
    playerSetSkillRank__Fiii(1, 31, -2);                // append at the -1 pair
    if (pairs(1)[4] != 31 || pairs(1)[5] != -2) return 3;
    if (pairs(1)[6] != 77 || pairs(1)[7] != 77) return 5; // the pair after the new one is untouched (no terminator added)
    playerSetSkillRank__Fiii(0, 8, 0x12345);            // first pair free; value truncated to a halfword
    if (pairs(0)[0] != 8 || pairs(0)[1] != 0x2345 || pairs(0)[2] != 5) return 6;
    playerSetSkillRank__Fiii(2, 4, 9);
    if (pairs(2)[1] != 9 || pairs(2)[2] != -1) return 7;
    if (pairs(1)[0] != 4 || pairs(1)[1] != 10) return 8; // other records untouched
    return 0;
}
