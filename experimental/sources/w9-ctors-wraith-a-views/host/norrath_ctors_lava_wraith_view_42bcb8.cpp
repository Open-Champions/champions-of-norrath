// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42bcb8[12];
int main() {
    if (norrath_ctors_lava_wraith_view_42bcb8[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[0].name, "wraith.lmp") || norrath_ctors_lava_wraith_view_42bcb8[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[1].name, "wraith.vif") || norrath_ctors_lava_wraith_view_42bcb8[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[2].name, "wraith.tex") || norrath_ctors_lava_wraith_view_42bcb8[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[3].name, "attack1.anm") || norrath_ctors_lava_wraith_view_42bcb8[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[4].name, "death.anm") || norrath_ctors_lava_wraith_view_42bcb8[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[5].name, "deathtouch.anm") || norrath_ctors_lava_wraith_view_42bcb8[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[6].name, "fly.anm") || norrath_ctors_lava_wraith_view_42bcb8[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[7].name, "forward.anm") || norrath_ctors_lava_wraith_view_42bcb8[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[8].name, "hitreaction.anm") || norrath_ctors_lava_wraith_view_42bcb8[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[9].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42bcb8[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42bcb8[10].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42bcb8[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42bcb8[11].name != nullptr || norrath_ctors_lava_wraith_view_42bcb8[11].unused != 0u) return 1;
    return 0;
}
