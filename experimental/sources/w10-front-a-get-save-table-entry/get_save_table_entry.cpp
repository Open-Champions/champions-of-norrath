// Observed: getSaveTableEntry__FPc at 0x0025b740 is 244 bytes. With name the argument and act
// the int curAct (re-read on every pass): when curAct is -1 it returns -1. Otherwise it walks
// level = 0, 1, ... while level is below actLevelCount[curAct]; the entry is the 8-byte record
// at allActs[curAct] + 8 * level, whose first word is a string pointer p. The first level for
// which strncmp(p, name, strlen(p)) is zero is returned. Reaching the end returns -1. The
// act-count bound for level 0 is read once before the loop, so an act with no levels returns -1.
// Inferred: same table and match rule as MC_NameToAct__FPc, here limited to the current act:
// a match means the given name begins with the level's name.
// Unknown: what the second word of each 8-byte entry holds.

extern "C" {
extern int curAct;
extern int actLevelCount[];
extern unsigned char* allActs[];
__SIZE_TYPE__ strlen(const char* text);
int strncmp(const char* left, const char* right, __SIZE_TYPE__ count);
}

extern "C" int getSaveTableEntry__FPc(char* name) {
    if (curAct == -1) return -1;
    for (int level = 0; level < actLevelCount[curAct]; ++level) {
        const char* entry = *reinterpret_cast<const char* const*>(allActs[curAct] + level * 8);
        if (strncmp(entry, name, strlen(entry)) == 0) return level;
    }
    return -1;
}
