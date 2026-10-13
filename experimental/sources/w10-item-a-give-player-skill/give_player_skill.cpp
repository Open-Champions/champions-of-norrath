// Observed givePlayerSkill__Fii at 0x001be0b0, 412 bytes (int skill, int player;
// the skill number is the first argument). `skills` (0x0034b440) is an array of
// pointers to skill objects; the word at +8 of the object skills[skill] is its
// kind, and kind 3 ends the function at once. With R = player + player * 23984:
//   R+22008 starts a table of 15 entries 104 bytes apart whose first halfword is a
//   skill number, -1 meaning an unused entry (the entry itself starts 32 bytes
//   earlier, at R+21976 + 104 * n).
//   1. The first unused entry is looked up (none found: index -1).
//   2. All 15 entries are then scanned for the skill number; any hit cancels
//      the operation (index -1). No unused entry also ends the function.
//   3. The chosen entry is zeroed (104 bytes with memset), then: bytes 0..8 get
//      the nine bytes at norrath_player_skill_placeholder_view (the string
//      "newstyle" and its terminator), the halfword at +32 the skill number, the
//      byte at +36 the value 1, and the byte at +37 the low byte of the kind word
//      of the skill object.
//   4. playerSetSkillRank__Fiii(player, skill, max(rank, 1)) where rank is
//      playerGetSkillRank__Fii(player, skill).
// Unknown: what the table entries are used for (apart from the skill number) and
// what the kind values mean. The skill index is not range checked.
extern "C" {
extern unsigned char player[];
extern unsigned char* skills[];
extern const unsigned char norrath_player_skill_placeholder_view[];
void* memset(void*, int, unsigned);
int playerGetSkillRank__Fii(int, int);
void playerSetSkillRank__Fiii(int, int, int);

void givePlayerSkill__Fii(int skill, int owner) {
    if (*reinterpret_cast<int*>(skills[skill] + 8) == 3) return;
    unsigned char* record = player + owner * 23984;
    int free_index = -1;
    for (int i = 0; i < 15; ++i) {
        if (*reinterpret_cast<short*>(record + 22008 + 104 * i) == -1) {
            free_index = i;
            break;
        }
    }
    for (int i = 0; i < 15; ++i) {
        if (*reinterpret_cast<short*>(record + 22008 + 104 * i) == skill) free_index = -1;
    }
    if (free_index == -1) return;
    unsigned char* entry = record + 21976 + 104 * free_index;
    memset(entry, 0, 104);
    for (int i = 0; i < 9; ++i) entry[i] = norrath_player_skill_placeholder_view[i];
    *reinterpret_cast<short*>(entry + 32) = static_cast<short>(skill);
    entry[36] = 1;
    entry[37] = skills[skill][8];
    const int rank = playerGetSkillRank__Fii(owner, skill);
    playerSetSkillRank__Fiii(owner, skill, rank < 1 ? 1 : rank);
}
}
