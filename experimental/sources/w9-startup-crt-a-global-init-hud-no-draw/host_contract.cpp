#include "global_init_hud_no_draw.cpp"

#include <cstdio>
#include <cstring>

static void* seen_map[4];
static const char* seen_group[4];
static const char* seen_name[4];
static int seen_count;

extern "C" void __11ResourceMapPcT1(void* map, const char* group, const char* name) {
    if (seen_count < 3) {
        seen_map[seen_count] = map;
        seen_group[seen_count] = group;
        seen_name[seen_count] = name;
    }
    ++seen_count;
}

int main() {
    _GLOBAL_$I$hudNoDraw();
    if (seen_count != 3) return 1;
    if (seen_map[0] != hudBox || seen_map[1] != spellBook || seen_map[2] != emptySpell) return 2;
    if (std::strcmp(seen_group[0], "hud.lmp") != 0 || std::strcmp(seen_name[0], "hudbox.tex") != 0) return 3;
    if (std::strcmp(seen_group[1], "hud.lmp") != 0 || std::strcmp(seen_name[1], "spellbook.tex") != 0) return 4;
    if (std::strcmp(seen_group[2], "icons.lmp") != 0 || std::strcmp(seen_name[2], "icblank.tex") != 0) return 5;
    if (sizeof hudBox != 16 || sizeof spellBook != 16 || sizeof emptySpell != 16) return 6;
    // Each call repeats the three constructions.
    _GLOBAL_$I$hudNoDraw();
    if (seen_count != 6) return 7;
    std::puts("hud no draw initializer contract passed");
    return 0;
}
