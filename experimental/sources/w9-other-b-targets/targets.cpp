// Observed: targets is ten 636-byte records at 0x00437038 in initialized data (symbol size 6360, next symbol
// playerSelectInfo; a local symbol of its original object). Each record starts with four string pointers, then
// four bytes and one more string pointer, then 612 zero bytes. Record i, with its file values:
//   i  first  second         third      fourth       bytes16..19  fifth
//   0  bm     RaceBarbarian  Warrior    SexMale      0 0 1 1      longaxe15
//   1  bf     RaceBarbarian  Warrior    SexFemale    1 0 0 0      longaxe15
//   2  wem    RaceWoodElf    Bard       SexMale      2 3 0 0      short_sword11
//   3  wef    RaceWoodElf    Bard       SexFemale    3 3 0 0      short_sword11
//   4  hem    RaceHighElf    Cleric     SexMale      4 1 0 0      mace15
//   5  hef    RaceHighElf    Cleric     SexFemale    5 1 0 0      mace15
//   6  em     RaceErudite    Wizard     SexMale      6 4 1 1      quarter_staff13
//   7  ef     RaceErudite    Wizard     SexFemale    7 4 1 0      quarter_staff13
//   8  dem    RaceDarkElf    SKnight    SexMale      8 2 0 1      short_sword11
//   9  def    RaceDarkElf    SKnight    SexFemale    9 2 0 0      short_sword11
// changeSkin__Fii, getHairAndSkin__Fi, init__Fv, changeHairStyle__Fii and playerSelectionTask__Fv use the
// table; playerSelectionTask__Fv loads bytes at offsets 17 and 19, getHairStyle__Fi the byte at 18,
// changeSkin__Fii the byte at 24, changeHairColor__Fii the byte at 26 and changeBeard__Fii the byte at 27;
// those bytes at 24, 26 and 27 are zero in the file.
// Inferred: the first string of each record is a short object name used to find the character (the
// existing authored init passes it to objectFindObject__FPc). The record is 24 bytes of known fields plus
// 612 bytes of working state.
// Unknown: the meaning of the four byte fields (the first of them counts 0 to 9) and of the working state.
// Field names below describe the stored strings, not known original names.

struct Target {
    const char* name;
    const char* raceText;
    const char* classText;
    const char* sexText;
    unsigned char index;
    unsigned char byte17;
    unsigned char byte18;
    unsigned char byte19;
    const char* weaponText;
    unsigned char working[612];
};
#if defined(__mips__)
static_assert(sizeof(Target) == 636, "targets stride");
#endif

extern "C" Target targets[10] = {
    {"bm", "RaceBarbarian", "Warrior", "SexMale", 0, 0, 1, 1, "longaxe15", {}},
    {"bf", "RaceBarbarian", "Warrior", "SexFemale", 1, 0, 0, 0, "longaxe15", {}},
    {"wem", "RaceWoodElf", "Bard", "SexMale", 2, 3, 0, 0, "short_sword11", {}},
    {"wef", "RaceWoodElf", "Bard", "SexFemale", 3, 3, 0, 0, "short_sword11", {}},
    {"hem", "RaceHighElf", "Cleric", "SexMale", 4, 1, 0, 0, "mace15", {}},
    {"hef", "RaceHighElf", "Cleric", "SexFemale", 5, 1, 0, 0, "mace15", {}},
    {"em", "RaceErudite", "Wizard", "SexMale", 6, 4, 1, 1, "quarter_staff13", {}},
    {"ef", "RaceErudite", "Wizard", "SexFemale", 7, 4, 1, 0, "quarter_staff13", {}},
    {"dem", "RaceDarkElf", "SKnight", "SexMale", 8, 2, 0, 1, "short_sword11", {}},
    {"def", "RaceDarkElf", "SKnight", "SexFemale", 9, 2, 0, 0, "short_sword11", {}},
};
