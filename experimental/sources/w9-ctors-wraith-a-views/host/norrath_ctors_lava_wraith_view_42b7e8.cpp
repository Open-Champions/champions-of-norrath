// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42b7e8[17];
int main() {
    if (norrath_ctors_lava_wraith_view_42b7e8[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[0].name, "crusher.lmp") || norrath_ctors_lava_wraith_view_42b7e8[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[1].name, "crusher.vif") || norrath_ctors_lava_wraith_view_42b7e8[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[2].name, "crusher.tex") || norrath_ctors_lava_wraith_view_42b7e8[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[3].name, "block.anm") || norrath_ctors_lava_wraith_view_42b7e8[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[4].name, "crush.anm") || norrath_ctors_lava_wraith_view_42b7e8[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[5].name, "csh_atak1.anm") || norrath_ctors_lava_wraith_view_42b7e8[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[6].name, "csh_atak2.anm") || norrath_ctors_lava_wraith_view_42b7e8[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[7].name, "csh_idle1.anm") || norrath_ctors_lava_wraith_view_42b7e8[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[8].name, "csh_idle2.anm") || norrath_ctors_lava_wraith_view_42b7e8[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[9].name, "death.anm") || norrath_ctors_lava_wraith_view_42b7e8[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[10].name, "hitreact.anm") || norrath_ctors_lava_wraith_view_42b7e8[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[11].name, "run.anm") || norrath_ctors_lava_wraith_view_42b7e8[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[12].name, "spawn.anm") || norrath_ctors_lava_wraith_view_42b7e8[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[13].name, "stepleft.anm") || norrath_ctors_lava_wraith_view_42b7e8[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[14].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[14].name, "stepright.anm") || norrath_ctors_lava_wraith_view_42b7e8[14].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[15].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b7e8[15].name, "walk.anm") || norrath_ctors_lava_wraith_view_42b7e8[15].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b7e8[16].name != nullptr || norrath_ctors_lava_wraith_view_42b7e8[16].unused != 0u) return 1;
    return 0;
}
