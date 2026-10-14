// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42c7b0[5];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42c7b0 != 5u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42c7b0[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7b0[0].name, "chest3.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7b0[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7b0[1].name, "chest3.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7b0[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7b0[2].name, "chest3.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7b0[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c7b0[3].name, "chest3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c7b0[4].name != nullptr || norrath_ctors_ant_lantern_view_42c7b0[4].unused != 0u) return 1;
    return 0;
}
