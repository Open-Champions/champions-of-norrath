// Observed: loadPlayerLumps__Fv at 0x001cb820 is 1056 bytes (a local symbol) and returns an int.
// The records of player are 23984 bytes apart; the word at record byte 84 is the character class,
// an index below 10, the signed bytes at 32, 33, 34 and 35 are appearance choices (32 hair style,
// 33 hair color letter, 34 beard style, 35 body variant, with -1 in 32 and 34 meaning none).
// n is nmPlayers, read again where it is used.
//  1. For each of ten class names (the local table playerLumpName), flag[i] is 1 when the name is
//     not null and lumpQuery__FPc(name) is nonzero, else 0. For each player the flag of its class
//     is decreased by one. unchanged is true when all ten flags are then zero and
//     playerCheckArmorCacheRaces__Fv() (called in any case) is nonzero.
//  2. When unchanged, the result is 0 unless MC_GetAsyncMode__Fv() is 7. In every other case the
//     lumps are reloaded:
//  3. lumpClear__Fi(5); playerDeallocateArmorCaches__Fv(); playerInitArmorCache__Fv(). Then for
//     each player in order, with k its index and c its class:
//     a. the class name playerLumpName[c] is copied to a buffer, replaced by playerLowLumpName[c]
//        when n >= 3 or coreForceSmallCharacters is nonzero;
//     b. lumpLoad__FPc(buffer); lumpRename__FPcT0(buffer, playerLumpName[c]);
//     c. s is "select2.lmp" when n >= 2 or coreForceSmallCharacters is nonzero, else "select.lmp";
//        every load below is lumpPartialLoad__FPcT0(s, text) with text from sprintf and
//        p = racePrefix[c]:
//     d. when byte 32 is not -1: "%s_hair%da.vif" (p, byte 32 + 1), then "%s_hair%d%c.tex"
//        (p, byte 32 + 1, byte 33 + 97); when either load returns zero byte 32 becomes -1;
//     e. when byte 34 is not -1: "%s_beard%da.vif" (p, byte 34 + 1), then "%s_beard%d%c.tex"
//        (p, byte 34 + 1, byte 33 + 97); when either returns zero byte 34 becomes -1;
//     f. "%s_bodyv1.vif" (p), then "%s_bodyv%d.tex" (p, byte 35 + 1);
//     g. playerAllocateArmorCache__Fii(k, c).
//  4. lumpLock__Fi(5); the result is 1.
// Inferred: the function makes the character-model lump set match the players present, and drops
// hair or beard choices whose files are absent. The table contents are the file names in the image.
// Unknown: the scratch buffer sizes of the original (160 bytes for the lump name, 64 for the text).

extern "C" {
extern unsigned char player[];
extern int nmPlayers;
extern signed char coreForceSmallCharacters;
extern char* racePrefix[10];
void* lumpQuery__FPc(const char* name);
void lumpClear__Fi(int id);
void lumpLock__Fi(int id);
void* lumpLoad__FPc(const char* name);
void lumpRename__FPcT0(const char* from, const char* to);
int lumpPartialLoad__FPcT0(const char* lump, const char* name);
int playerCheckArmorCacheRaces__Fv();
void playerDeallocateArmorCaches__Fv();
void playerInitArmorCache__Fv();
void playerAllocateArmorCache__Fii(int index, int race);
int MC_GetAsyncMode__Fv();
int sprintf(char* out, const char* format, ...);
char* strcpy(char* to, const char* from);
}

namespace {
const char* playerLumpName[10] = {"ragnar2.lmp", "sonya2.lmp", "elf_mwo2.lmp", "elf_fwo2.lmp", "hem_high.lmp",
                            "elf_fhi2.lmp", "eru_male.lmp", "eru_fem.lmp", "elf_mdk.lmp", "elf_fdk2.lmp"};
const char* playerLowLumpName[10] = {"ragnar2l.lmp", "sonya2l.lmp", "elf_mwo2.lmp", "elf_fwo2.lmp", "hem_high.lmp",
                               "elf_fhi2.lmp", "eru_male.lmp", "eru_fem.lmp", "elf_mdkl.lmp", "elf_fdkl.lmp"};
}

extern "C" int loadPlayerLumps__Fv() {
    int flag[10];
    for (int i = 0; i < 10; ++i) flag[i] = (playerLumpName[i] != nullptr && lumpQuery__FPc(playerLumpName[i]) != nullptr) ? 1 : 0;
    for (int k = 0; k < nmPlayers; ++k) flag[*reinterpret_cast<int*>(player + 23984 * k + 84)] -= 1;
    bool unchanged = true;
    for (int i = 0; i < 10; ++i) {
        if (flag[i] != 0) unchanged = false;
    }
    if (playerCheckArmorCacheRaces__Fv() == 0) unchanged = false;
    if (unchanged && MC_GetAsyncMode__Fv() != 7) return 0;
    lumpClear__Fi(5);
    playerDeallocateArmorCaches__Fv();
    playerInitArmorCache__Fv();
    char name[160];
    char text[64];
    for (int k = 0; k < nmPlayers; ++k) {
        signed char* record = reinterpret_cast<signed char*>(player + 23984 * k);
        int c = *reinterpret_cast<int*>(record + 84);
        strcpy(name, playerLumpName[c]);
        if (nmPlayers >= 3 || coreForceSmallCharacters) strcpy(name, playerLowLumpName[c]);
        lumpLoad__FPc(name);
        lumpRename__FPcT0(name, playerLumpName[c]);
        const char* s = (nmPlayers >= 2 || coreForceSmallCharacters) ? "select2.lmp" : "select.lmp";
        if (record[32] != -1) {
            sprintf(text, "%s_hair%da.vif", racePrefix[*reinterpret_cast<int*>(record + 84)], record[32] + 1);
            int first = lumpPartialLoad__FPcT0(s, text);
            sprintf(text, "%s_hair%d%c.tex", racePrefix[*reinterpret_cast<int*>(record + 84)], record[32] + 1,
                    record[33] + 97);
            int second = lumpPartialLoad__FPcT0(s, text);
            if (first == 0 || second == 0) record[32] = -1;
        }
        if (record[34] != -1) {
            sprintf(text, "%s_beard%da.vif", racePrefix[*reinterpret_cast<int*>(record + 84)], record[34] + 1);
            int first = lumpPartialLoad__FPcT0(s, text);
            sprintf(text, "%s_beard%d%c.tex", racePrefix[*reinterpret_cast<int*>(record + 84)], record[34] + 1,
                    record[33] + 97);
            int second = lumpPartialLoad__FPcT0(s, text);
            if (first == 0 || second == 0) record[34] = -1;
        }
        sprintf(text, "%s_bodyv1.vif", racePrefix[*reinterpret_cast<int*>(record + 84)]);
        lumpPartialLoad__FPcT0(s, text);
        sprintf(text, "%s_bodyv%d.tex", racePrefix[*reinterpret_cast<int*>(record + 84)], record[35] + 1);
        lumpPartialLoad__FPcT0(s, text);
        playerAllocateArmorCache__Fii(k, *reinterpret_cast<int*>(record + 84));
    }
    lumpLock__Fi(5);
    return 1;
}
