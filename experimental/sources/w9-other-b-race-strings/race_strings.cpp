// Observed: raceStrings is a table of ten 32-bit pointers at 0x0042e250 in initialized data (symbol size 40;
// in the image it is a local symbol of its original object). The file pointers lead to the strings
// RaceBarbarianMale, RaceBarbarianFemale, RaceWoodElfMale, RaceWoodElfFemale, RaceHighElfMale,
// RaceHighElfFemale, RaceEruditeMale, RaceEruditeFemale, RaceDarkElfMale and RaceDarkElfFemale, in that
// order. asyncChangePlayers__Fv takes the table address and runBrowser__FPUs loads a pointer from it.
// Inferred: the strings are lookup keys, since their form matches the "Race..." names used with other text.
// Unknown: what the keys index; the original string storage addresses are not reproduced.

extern "C" const char* raceStrings[10] = {
    "RaceBarbarianMale", "RaceBarbarianFemale", "RaceWoodElfMale", "RaceWoodElfFemale", "RaceHighElfMale",
    "RaceHighElfFemale", "RaceEruditeMale", "RaceEruditeFemale", "RaceDarkElfMale", "RaceDarkElfFemale",
};
#if defined(__mips__)
static_assert(sizeof(raceStrings) == 40, "raceStrings");
#endif
