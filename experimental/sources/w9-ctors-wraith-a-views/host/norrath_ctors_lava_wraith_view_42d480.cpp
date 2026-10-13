// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42d480[21];
int main() {
    if (norrath_ctors_lava_wraith_view_42d480[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[0].name, "mumyking.lmp") || norrath_ctors_lava_wraith_view_42d480[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[1].name, "mormaz.vif") || norrath_ctors_lava_wraith_view_42d480[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[2].name, "mormaz.tex") || norrath_ctors_lava_wraith_view_42d480[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[3].name, "attack1.anm") || norrath_ctors_lava_wraith_view_42d480[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[4].name, "attack2.anm") || norrath_ctors_lava_wraith_view_42d480[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[5].name, "death.anm") || norrath_ctors_lava_wraith_view_42d480[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[6].name, "giveheart.anm") || norrath_ctors_lava_wraith_view_42d480[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[7].name, "hitreact.anm") || norrath_ctors_lava_wraith_view_42d480[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[8].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42d480[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[9].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42d480[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[10].name, "sh_launch.anm") || norrath_ctors_lava_wraith_view_42d480[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[11].name, "shield.anm") || norrath_ctors_lava_wraith_view_42d480[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[12].name, "spinsaw.anm") || norrath_ctors_lava_wraith_view_42d480[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[13].name, "walk.anm") || norrath_ctors_lava_wraith_view_42d480[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[14].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[14].name, "staff.vif") || norrath_ctors_lava_wraith_view_42d480[14].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[15].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[15].name, "staff.tex") || norrath_ctors_lava_wraith_view_42d480[15].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[16].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[16].name, "spel_mumyking_snake.anm") || norrath_ctors_lava_wraith_view_42d480[16].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[17].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[17].name, "spel_mumyking_snake_hit.anm") || norrath_ctors_lava_wraith_view_42d480[17].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[18].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[18].name, "spel_mumyking_summon.anm") || norrath_ctors_lava_wraith_view_42d480[18].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[19].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d480[19].name, "spel_mumyking_teleport.anm") || norrath_ctors_lava_wraith_view_42d480[19].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d480[20].name != nullptr || norrath_ctors_lava_wraith_view_42d480[20].unused != 0u) return 1;
    return 0;
}
