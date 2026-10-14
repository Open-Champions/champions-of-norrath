/* Storage that the pad, font, DMA-environment and player-select units import, defined from
 * what the executable image shows. Addresses are in the private reference executable; extents
 * are the symbol size, or the gap to the next symbol when the symbol size is 0.
 *
 *   padFilter         0x00484074, .data, 4 bytes, image value 0. Read by padProcessData.
 *   reglistLoopCount  0x00486994, .sbss word, 4 bytes to nmChars. Holds a cursor the font
 *                     routines advance by 32 per record.
 *   rdata             0x006e1cc0, .bss, 128 bytes to bigblank.762. There are three same-named
 *                     objects in the original; this is the first one, which padGetUnprocessedData
 *                     indexes in 32-byte steps. The other two are not defined here.
 *   sceDmaCurrentEnv  0x008dd830, .bss, 80 bytes to padsif. The copy of the last environment
 *                     sceDmaPutEnv stored.
 *   raceStrings       0x004293e8, .data, 40 bytes: ten pointers to the NUL-terminated names
 *                     RaceBarbarianMale, RaceBarbarianFemale, RaceWoodElfMale, RaceWoodElfFemale,
 *                     RaceHighElfMale, RaceHighElfFemale, RaceEruditeMale, RaceEruditeFemale,
 *                     RaceDarkElfMale, RaceDarkElfFemale, in that order (the symbol name is
 *                     raceStrings.1135; the dot makes the published name a view).
 *   the word at 0x004852d8 lies inside the .sdata literal "Color". setSRSInfo reads it as an int,
 *                     so the first four bytes 'C','o','l','o' are the value 0x6f6c6f43.
 *                     Defined as a const int because the literal has no symbol of its own.
 * Inferred: that the table is indexed by a race/gender selector; the names say so, no caller was
 * executed. Unknown: what the pad rdata contents and the sceDmaCurrentEnv layout mean beyond the
 * copy sceDmaPutEnv makes.
 */
extern "C" {
int padFilter = 0;
unsigned reglistLoopCount;
alignas(64) unsigned char rdata[128];
alignas(16) unsigned char sceDmaCurrentEnv[80];

static const char race_barbarian_male[] = "RaceBarbarianMale";
static const char race_barbarian_female[] = "RaceBarbarianFemale";
static const char race_wood_elf_male[] = "RaceWoodElfMale";
static const char race_wood_elf_female[] = "RaceWoodElfFemale";
static const char race_high_elf_male[] = "RaceHighElfMale";
static const char race_high_elf_female[] = "RaceHighElfFemale";
static const char race_erudite_male[] = "RaceEruditeMale";
static const char race_erudite_female[] = "RaceEruditeFemale";
static const char race_dark_elf_male[] = "RaceDarkElfMale";
static const char race_dark_elf_female[] = "RaceDarkElfFemale";

extern const char* const norrath_sce_d_race_strings_view_4293e8[10];
const char* const norrath_sce_d_race_strings_view_4293e8[10] = {
    race_barbarian_male, race_barbarian_female, race_wood_elf_male, race_wood_elf_female,
    race_high_elf_male, race_high_elf_female, race_erudite_male, race_erudite_female,
    race_dark_elf_male, race_dark_elf_female,
};

extern const int norrath_sce_d_unknown_sdata_word_view_4852d8;
const int norrath_sce_d_unknown_sdata_word_view_4852d8 = 0x6f6c6f43;
}
