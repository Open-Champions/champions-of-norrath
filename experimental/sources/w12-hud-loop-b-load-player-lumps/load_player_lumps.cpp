// Observed loadPlayerLumps__Fv at 0x001cb820, 1056 bytes (a file-local function in the image),
// no arguments, int result. Player records are 23984 bytes apart. In each record the word at +84 is
// the character index (0..9, used to index name tables) and the signed bytes at +32, +33, +34, +35
// are appearance choices; -1 in +32 or +34 means none.
//  1. flag[i] is 1 when playerLumpName[i] is non-null and lumpQuery__FPc(name) is nonzero, else 0.
//     Each present player lowers the flag of its character by one. The set is "unchanged" when
//     every flag is zero and playerCheckArmorCacheRaces__Fv() (called in every case) is nonzero.
//  2. When unchanged the result is 0, unless MC_GetAsyncMode__Fv() is 7; then the work below runs.
//  3. lumpClear__Fi(5), playerDeallocateArmorCaches__Fv(), playerInitArmorCache__Fv(), then for each
//     player k with character c and prefix p = racePrefix[c]:
//       - name = playerLumpName[c]; with three or more players, or coreForceSmallCharacters set, the
//         name is playerLowLumpName[c];
//       - lumpLoad__FPc(name); lumpRename__FPcT0(name, playerLumpName[c]);
//       - source lump s is "select2.lmp" with two or more players or coreForceSmallCharacters set,
//         else "select.lmp"; each load below is lumpPartialLoad__FPcT0(s, text);
//       - hair (byte +32 not -1): "%s_hair%da.vif" (p, +32 plus 1) then "%s_hair%d%c.tex"
//         (p, +32 plus 1, +33 plus 97); either load returning zero sets byte +32 to -1;
//       - beard (byte +34 not -1): "%s_beard%da.vif" then "%s_beard%d%c.tex" with byte +34 and the
//         same colour letter from +33; either returning zero sets +34 to -1;
//       - "%s_bodyv1.vif" (p) then "%s_bodyv%d.tex" (p, +35 plus 1);
//       - playerAllocateArmorCache__Fii(k, c).
//     Then lumpLock__Fi(5) and the result is 1.
// Both loads of a pair are always issued before the pair is judged.
// nmPlayers is read afresh wherever it is used.
// Unknown: the scratch buffer sizes of the original; this source uses 160 bytes for the lump name
// and 64 for the text.
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
const char* const playerLumpName[10] = {"ragnar2.lmp", "sonya2.lmp", "elf_mwo2.lmp", "elf_fwo2.lmp", "hem_high.lmp",
                                        "elf_fhi2.lmp", "eru_male.lmp", "eru_fem.lmp", "elf_mdk.lmp", "elf_fdk2.lmp"};
const char* const playerLowLumpName[10] = {"ragnar2l.lmp", "sonya2l.lmp", "elf_mwo2.lmp", "elf_fwo2.lmp", "hem_high.lmp",
                                           "elf_fhi2.lmp", "eru_male.lmp", "eru_fem.lmp", "elf_mdkl.lmp", "elf_fdkl.lmp"};

// Loads an optional part (a vif and a texture) and drops the appearance choice when either is absent.
void loadChoice(const char* source, char* text, const char* vifFormat, const char* texFormat, const char* prefix,
                signed char* choice, int colour) {
    sprintf(text, vifFormat, prefix, *choice + 1);
    const int first = lumpPartialLoad__FPcT0(source, text);
    sprintf(text, texFormat, prefix, *choice + 1, colour + 97);
    const int second = lumpPartialLoad__FPcT0(source, text);
    if (first == 0 || second == 0) *choice = -1;
}
}

extern "C" int loadPlayerLumps__Fv() {
    int flag[10];
    for (int i = 0; i < 10; ++i) flag[i] = (playerLumpName[i] != nullptr && lumpQuery__FPc(playerLumpName[i]) != nullptr) ? 1 : 0;
    for (int k = 0; k < nmPlayers; ++k) flag[*reinterpret_cast<int*>(player + 23984 * k + 84)] -= 1;
    bool unchanged = true;
    for (int i = 0; i < 10; ++i)
        if (flag[i] != 0) unchanged = false;
    if (playerCheckArmorCacheRaces__Fv() == 0) unchanged = false;
    if (unchanged && MC_GetAsyncMode__Fv() != 7) return 0;

    lumpClear__Fi(5);
    playerDeallocateArmorCaches__Fv();
    playerInitArmorCache__Fv();
    char name[160];
    char text[64];
    for (int k = 0; k < nmPlayers; ++k) {
        signed char* record = reinterpret_cast<signed char*>(player + 23984 * k);
        const int c = *reinterpret_cast<int*>(record + 84);
        strcpy(name, playerLumpName[c]);
        if (nmPlayers >= 3 || coreForceSmallCharacters != 0) strcpy(name, playerLowLumpName[c]);
        lumpLoad__FPc(name);
        lumpRename__FPcT0(name, playerLumpName[c]);
        const char* source = (nmPlayers >= 2 || coreForceSmallCharacters != 0) ? "select2.lmp" : "select.lmp";
        if (record[32] != -1) loadChoice(source, text, "%s_hair%da.vif", "%s_hair%d%c.tex", racePrefix[c], &record[32], record[33]);
        if (record[34] != -1) loadChoice(source, text, "%s_beard%da.vif", "%s_beard%d%c.tex", racePrefix[c], &record[34], record[33]);
        sprintf(text, "%s_bodyv1.vif", racePrefix[c]);
        lumpPartialLoad__FPcT0(source, text);
        sprintf(text, "%s_bodyv%d.tex", racePrefix[c], record[35] + 1);
        lumpPartialLoad__FPcT0(source, text);
        playerAllocateArmorCache__Fii(k, c);
    }
    lumpLock__Fi(5);
    return 1;
}
