// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42ce60[18];
int main() {
    if (norrath_ctors_lava_wraith_view_42ce60[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[0].name, "superorc.lmp") || norrath_ctors_lava_wraith_view_42ce60[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[1].name, "superorc.vif") || norrath_ctors_lava_wraith_view_42ce60[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[2].name, "superorc.tex") || norrath_ctors_lava_wraith_view_42ce60[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[3].name, "atak1.anm") || norrath_ctors_lava_wraith_view_42ce60[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[4].name, "atak2.anm") || norrath_ctors_lava_wraith_view_42ce60[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[5].name, "atakrush.anm") || norrath_ctors_lava_wraith_view_42ce60[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[6].name, "atakwlk.anm") || norrath_ctors_lava_wraith_view_42ce60[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[7].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42ce60[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[8].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42ce60[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[9].name, "talk.anm") || norrath_ctors_lava_wraith_view_42ce60[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[10].name, "taunt.anm") || norrath_ctors_lava_wraith_view_42ce60[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[11].name, "walk.anm") || norrath_ctors_lava_wraith_view_42ce60[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[12].name, "playdead.anm") || norrath_ctors_lava_wraith_view_42ce60[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[13].name, "wakeup.anm") || norrath_ctors_lava_wraith_view_42ce60[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[14].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[14].name, "dead.anm") || norrath_ctors_lava_wraith_view_42ce60[14].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[15].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[15].name, "cleever.vif") || norrath_ctors_lava_wraith_view_42ce60[15].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[16].name == nullptr || !same(norrath_ctors_lava_wraith_view_42ce60[16].name, "cleever.tex") || norrath_ctors_lava_wraith_view_42ce60[16].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42ce60[17].name != nullptr || norrath_ctors_lava_wraith_view_42ce60[17].unused != 0u) return 1;
    return 0;
}
