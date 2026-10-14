// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42d048[15];
int main() {
    if (norrath_ctors_lava_wraith_view_42d048[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[0].name, "rondo.lmp") || norrath_ctors_lava_wraith_view_42d048[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[1].name, "rondo.vif") || norrath_ctors_lava_wraith_view_42d048[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[2].name, "rondo.tex") || norrath_ctors_lava_wraith_view_42d048[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[3].name, "attack.anm") || norrath_ctors_lava_wraith_view_42d048[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[4].name, "hitreaction.anm") || norrath_ctors_lava_wraith_view_42d048[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[5].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42d048[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[6].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42d048[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[7].name, "idle3.anm") || norrath_ctors_lava_wraith_view_42d048[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[8].name, "idle4.anm") || norrath_ctors_lava_wraith_view_42d048[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[9].name, "run.anm") || norrath_ctors_lava_wraith_view_42d048[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[10].name, "talk1.anm") || norrath_ctors_lava_wraith_view_42d048[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[11].name, "talk2.anm") || norrath_ctors_lava_wraith_view_42d048[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[12].name, "talk3.anm") || norrath_ctors_lava_wraith_view_42d048[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42d048[13].name, "walk.anm") || norrath_ctors_lava_wraith_view_42d048[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42d048[14].name != nullptr || norrath_ctors_lava_wraith_view_42d048[14].unused != 0u) return 1;
    return 0;
}
