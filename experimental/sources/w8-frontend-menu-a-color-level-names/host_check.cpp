#include <cstdio>
#include <cstring>
extern "C" {
char levelName[322];
int calls;
const char* seen[16];
int MC_NameToAct__FPc(const char* n) {
    seen[calls++] = n;
    if (!std::strcmp(n, "alpha")) return 0;
    if (!std::strcmp(n, "beta")) return 2;
    if (!std::strcmp(n, "gamma")) return 5;
    return -1;
}
}
#include "color_level_names.cpp"
int main() {
    norrath_menu_record m[5] = {{"alpha", 99, 11, 12}, {"beta", 99, 13, 14}, {"nope", 99, 15, 16},
                                {"gamma", 99, 17, 18}, {0, 99, 19, 20}};
    std::strcpy(levelName + 81, "beta");
    colorLevelNames__FP4Menu(m);
    if (m[0].color != 6 || m[1].color != 0 || m[2].color != 7 || m[3].color != 6) return 1;
    if (m[4].color != 99 || m[0].third != 11 || m[3].fourth != 18) return 2;
    if (calls != 5 || seen[0] != levelName + 81 || seen[1] != m[0].label) return 3;
    // unknown current level: unknown labels still 7, known labels differ -> 6
    calls = 0; std::strcpy(levelName + 81, "zzz");
    colorLevelNames__FP4Menu(m);
    if (m[0].color != 6 || m[1].color != 6 || m[2].color != 7) return 4;
    // empty list: only the lookup of the level name happens
    norrath_menu_record e = {0, 5, 0, 0};
    calls = 0; colorLevelNames__FP4Menu(&e);
    if (calls != 1 || e.color != 5) return 5;
    std::puts("colorLevelNames__FP4Menu check passed");
    return 0;
}
