// Host contract for the zero-initialized data objects. Not an EE plan input.
#include "game_show_enemies_in_map.cpp"
#include "interface_mode.cpp"
#include "interface_on.cpp"
#include "jumping.cpp"
#include "last_warp_pos.cpp"
#include "layer0_dir.cpp"
#include "layer1_dir.cpp"
#include "level_seeds.cpp"
#include "loading_screen_request_exit.cpp"
#include "no_monsters.cpp"
#include "opt_tbl.cpp"
static bool zero(const void* p, unsigned n) {
    const unsigned char* b = static_cast<const unsigned char*>(p);
    for (unsigned i = 0; i < n; ++i) if (b[i]) return false;
    return true;
}
int main() {
    if (sizeof layer0Dir != 24580 || sizeof layer1Dir != 24580 || sizeof levelSeeds != 4096 || sizeof optTbl != 40) return 1;
    if (!zero(layer0Dir, sizeof layer0Dir) || !zero(layer1Dir, sizeof layer1Dir) || !zero(levelSeeds, sizeof levelSeeds) ||
        !zero(optTbl, sizeof optTbl)) return 2;
    if (gameShowEnemiesInMap || interfaceMode || interfaceOn || jumping || lastWarpPos || loadingScreenRequestExit ||
        noMonsters) return 3;
    return 0;
}
