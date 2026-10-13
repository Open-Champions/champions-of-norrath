// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42c630[4];
int main() {
    if (norrath_ctors_lava_wraith_view_42c630[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c630[0].name, "rolldoor.lmp") || norrath_ctors_lava_wraith_view_42c630[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c630[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c630[1].name, "rolldoor.vif") || norrath_ctors_lava_wraith_view_42c630[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c630[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42c630[2].name, "rolldoor.tex") || norrath_ctors_lava_wraith_view_42c630[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42c630[3].name != nullptr || norrath_ctors_lava_wraith_view_42c630[3].unused != 0u) return 1;
    return 0;
}
