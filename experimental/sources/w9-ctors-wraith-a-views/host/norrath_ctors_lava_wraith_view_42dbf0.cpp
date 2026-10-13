// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42dbf0[10];
int main() {
    if (norrath_ctors_lava_wraith_view_42dbf0[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[0].name, "skele_b.lmp") || norrath_ctors_lava_wraith_view_42dbf0[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[1].name, "skeleb.vif") || norrath_ctors_lava_wraith_view_42dbf0[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[2].name, "skeleb.tex") || norrath_ctors_lava_wraith_view_42dbf0[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[3].name, "atak1.anm") || norrath_ctors_lava_wraith_view_42dbf0[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[4].name, "atak2.anm") || norrath_ctors_lava_wraith_view_42dbf0[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[5].name, "death.anm") || norrath_ctors_lava_wraith_view_42dbf0[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[6].name, "idle.anm") || norrath_ctors_lava_wraith_view_42dbf0[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[7].name, "rise.anm") || norrath_ctors_lava_wraith_view_42dbf0[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42dbf0[8].name, "walk.anm") || norrath_ctors_lava_wraith_view_42dbf0[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42dbf0[9].name != nullptr || norrath_ctors_lava_wraith_view_42dbf0[9].unused != 0u) return 1;
    return 0;
}
