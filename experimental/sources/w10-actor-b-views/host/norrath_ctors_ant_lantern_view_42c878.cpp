// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42c878[5];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42c878 != 5u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42c878[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c878[0].name, "baskets.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42c878[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c878[1].name, "wickbasketb.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42c878[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c878[2].name, "wickbasketb.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42c878[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c878[3].name, "wickbasketb_open.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c878[4].name != nullptr || norrath_ctors_ant_lantern_view_42c878[4].unused != 0u) return 1;
    return 0;
}
