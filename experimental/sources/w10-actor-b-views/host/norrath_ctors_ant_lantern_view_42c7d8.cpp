// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42c7d8[5];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42c7d8 != 5u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42c7d8[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7d8[0].name, "chestcav.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7d8[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7d8[1].name, "chestcave.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7d8[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7d8[2].name, "chestcave.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7d8[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7d8[3].name, "chestcave.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7d8[4].name != nullptr || norrath_ctors_ant_lantern_view_42c7d8[4].unused != 0u) return 1;
    return 0;
}
