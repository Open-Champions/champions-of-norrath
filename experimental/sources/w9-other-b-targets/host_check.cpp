// Host check for targets.cpp; not an EE plan input.
#include "targets.cpp"

int main() {
    static const char* const names[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
    static const char* const races[5] = {"RaceBarbarian", "RaceWoodElf", "RaceHighElf", "RaceErudite", "RaceDarkElf"};
    static const char* const classes[5] = {"Warrior", "Bard", "Cleric", "Wizard", "SKnight"};
    static const char* const weapons[5] = {"longaxe15", "short_sword11", "mace15", "quarter_staff13", "short_sword11"};
    static const unsigned char bytes[10][3] = {{0, 1, 1}, {0, 0, 0}, {3, 0, 0}, {3, 0, 0}, {1, 0, 0},
                                               {1, 0, 0}, {4, 1, 1}, {4, 1, 0}, {2, 0, 1}, {2, 0, 0}};
    auto same = [](const char* a, const char* b) {
        while (*a && *a == *b) { ++a; ++b; }
        return *a == *b;
    };
    for (int i = 0; i < 10; ++i) {
        const Target& t = targets[i];
        if (!same(t.name, names[i])) return 1;
        if (!same(t.raceText, races[i / 2])) return 2;
        if (!same(t.classText, classes[i / 2])) return 3;
        if (!same(t.sexText, (i & 1) ? "SexFemale" : "SexMale")) return 4;
        if (!same(t.weaponText, weapons[i / 2])) return 5;
        if (t.index != i || t.byte17 != bytes[i][0] || t.byte18 != bytes[i][1] || t.byte19 != bytes[i][2]) return 6;
        for (unsigned j = 0; j < sizeof(t.working); ++j) if (t.working[j] != 0) return 7;
    }
    return 0;
}
