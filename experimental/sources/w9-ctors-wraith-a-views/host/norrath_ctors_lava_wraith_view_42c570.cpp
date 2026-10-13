// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42c570[4];
int main() {
    if (norrath_ctors_lava_wraith_view_42c570[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c570[0].name, "savepnt.lmp") || norrath_ctors_lava_wraith_view_42c570[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c570[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c570[1].name, "savepoint.vif") || norrath_ctors_lava_wraith_view_42c570[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c570[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c570[2].name, "savepoint.tex") || norrath_ctors_lava_wraith_view_42c570[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c570[3].name != nullptr || norrath_ctors_lava_wraith_view_42c570[3].unused != 0u) return 1;
    return 0;
}
