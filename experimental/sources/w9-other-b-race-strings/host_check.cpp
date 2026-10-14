// Host check for race_strings.cpp; not an EE plan input.
#include "race_strings.cpp"

int main() {
    static const char* const expected[10] = {
        "RaceBarbarianMale", "RaceBarbarianFemale", "RaceWoodElfMale", "RaceWoodElfFemale", "RaceHighElfMale",
        "RaceHighElfFemale", "RaceEruditeMale", "RaceEruditeFemale", "RaceDarkElfMale", "RaceDarkElfFemale",
    };
    for (int i = 0; i < 10; ++i) {
        const char* a = raceStrings[i];
        const char* b = expected[i];
        while (*a && *a == *b) { ++a; ++b; }
        if (*a != *b) return 1 + i;
    }
    return 0;
}
