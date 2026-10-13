// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42b3d8[20];
int main() {
    if (norrath_ctors_lava_wraith_view_42b3d8[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[0].name, "soul.lmp") || norrath_ctors_lava_wraith_view_42b3d8[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[1].name, "soul1.vif") || norrath_ctors_lava_wraith_view_42b3d8[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[2].name, "soul1.tex") || norrath_ctors_lava_wraith_view_42b3d8[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[3].name, "soul2.vif") || norrath_ctors_lava_wraith_view_42b3d8[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[4].name, "soul2.tex") || norrath_ctors_lava_wraith_view_42b3d8[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[5].name, "hitreact.anm") || norrath_ctors_lava_wraith_view_42b3d8[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[6].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42b3d8[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[7].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42b3d8[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[8].name, "idle3.anm") || norrath_ctors_lava_wraith_view_42b3d8[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[9].name, "run.anm") || norrath_ctors_lava_wraith_view_42b3d8[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[10].name, "catch_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[11].name, "club_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[12].name, "flight_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[13].name, "idle_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[14].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[14].name, "shoot_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[14].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[15].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[15].name, "walk_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[15].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[16].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[16].name, "pelys_so.anm") || norrath_ctors_lava_wraith_view_42b3d8[16].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[17].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[17].name, "killed.anm") || norrath_ctors_lava_wraith_view_42b3d8[17].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[18].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b3d8[18].name, "free.anm") || norrath_ctors_lava_wraith_view_42b3d8[18].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b3d8[19].name != nullptr || norrath_ctors_lava_wraith_view_42b3d8[19].unused != 0u) return 1;
    return 0;
}
