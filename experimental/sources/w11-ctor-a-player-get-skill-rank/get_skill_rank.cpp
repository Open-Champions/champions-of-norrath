// Observed getSkillRank__6Playeri at 0x30e830, 24 bytes. Arguments: the Player object and a skill
// number. It reads the word at object+960 and returns playerGetSkillRank__Fii(that word, skill);
// the callee's result is the result of this function (the original jumps to it).
// Inferred: the word at +960 is the player number that indexes the player record array, since
// playerGetSkillRank__Fii uses its first argument as such an index.
extern "C" {
int playerGetSkillRank__Fii(int owner, int skill);
int getSkillRank__6Playeri(void* self, int skill);
}

extern "C" int getSkillRank__6Playeri(void* self, int skill) {
    return playerGetSkillRank__Fii(*reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 960), skill);
}
