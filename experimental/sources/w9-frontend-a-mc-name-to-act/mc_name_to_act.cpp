// Observed: MC_NameToAct__FPc at 0x0025aea0 is 276 bytes. For each act below actCount, and for
// each level below actLevelCount[act], it reads the name pointer in word 0 of the 8-byte entry
// allActs[act] + level * 8 and runs strncmp(entry name, name, strlen(entry name)). The first
// act holding a zero result is returned. When the walk ends without a match the result is -1,
// decided by comparing the act index with actCount: a negative actCount enters no loop and
// returns 0 rather than -1.
// Inferred: a match means the given name begins with a level's name, because the compared length
// is the entry's own length.
// Unknown: what the second word of each 8-byte entry holds.

extern "C" {
extern int actCount;
extern int actLevelCount[];
extern unsigned char* allActs[];
__SIZE_TYPE__ strlen(const char* text);
int strncmp(const char* left, const char* right, __SIZE_TYPE__ count);
}

extern "C" int MC_NameToAct__FPc(char* name) {
    int act = 0;
    while (act < actCount) {
        bool found = false;
        for (int level = 0; level < actLevelCount[act]; ++level) {
            const char* entry = *reinterpret_cast<const char* const*>(allActs[act] + level * 8);
            if (strncmp(entry, name, strlen(entry)) == 0) {
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
        act += 1;
    }
    return act != actCount ? act : -1;
}
