// Observed: MC_SetActByLevelName__FPc at 0x00250c10 is 308 bytes. It repeats the walk of
// MC_NameToAct__FPc inline: for each act below actCount and each level below actLevelCount[act],
// strncmp(entry name, name, strlen(entry name)) over allActs[act] + level * 8 (name pointer in
// word 0). The act index is then taken as found unless it equals actCount, in which case it is
// -1 (a negative actCount therefore yields act 0). A -1 result returns 0. A result equal to the
// word curAct returns 0. Otherwise MC_SetNewAct__Fi(act) runs and the function returns 1.
// Inferred: the return value tells the caller whether the act changed.
// Unknown: what the second word of each 8-byte entry holds.

extern "C" {
extern int actCount;
extern int actLevelCount[];
extern unsigned char* allActs[];
extern int curAct;
__SIZE_TYPE__ strlen(const char* text);
int strncmp(const char* left, const char* right, __SIZE_TYPE__ count);
void MC_SetNewAct__Fi(int act);
}

extern "C" int MC_SetActByLevelName__FPc(char* name) {
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
    const int result = act != actCount ? act : -1;
    if (result == -1) {
        return 0;
    }
    if (curAct == result) {
        return 0;
    }
    MC_SetNewAct__Fi(result);
    return 1;
}
