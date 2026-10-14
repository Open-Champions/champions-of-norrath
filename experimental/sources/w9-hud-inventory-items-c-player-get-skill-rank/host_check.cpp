// Host check for playerGetSkillRank__Fii: terminator, hit, miss, sign extension, stride.
extern "C" int playerGetSkillRank__Fii(int, int);
extern "C" unsigned char player[3 * 23984];
unsigned char player[3 * 23984];
static void put(int owner, int index, short id, short value) {
    short* p = reinterpret_cast<short*>(player + owner * 23984 + 23712) + 2 * index;
    p[0] = id; p[1] = value;
}
int main() {
    // record 1 holds three pairs then the -1 terminator, then a pair that must stay unseen
    put(1, 0, 4, 10); put(1, 1, 9, -3); put(1, 2, 7, 300); put(1, 3, -1, 0); put(1, 4, 5, 99);
    put(0, 0, 4, 55); put(0, 1, -1, 0);
    put(2, 0, -1, 0); put(2, 1, 4, 77);
    if (playerGetSkillRank__Fii(1, 4) != 10) return 1;
    if (playerGetSkillRank__Fii(1, 9) != -3) return 2;
    if (playerGetSkillRank__Fii(1, 7) != 300) return 3;
    if (playerGetSkillRank__Fii(1, 5) != 0) return 4;       // beyond the terminator
    if (playerGetSkillRank__Fii(1, 8) != 0) return 5;
    if (playerGetSkillRank__Fii(0, 4) != 55) return 6;      // other record, same id
    if (playerGetSkillRank__Fii(2, 4) != 0) return 7;       // terminator first
    if (playerGetSkillRank__Fii(1, 0x10004) != 0) return 8; // compared as a signed halfword value, not truncated skill
    return 0;
}
