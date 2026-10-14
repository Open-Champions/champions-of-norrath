// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42bf58[26];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42bf58 != 26u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[0].name, "antqueen.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[1].name, "antqueen.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[2].name, "antqueen.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[3].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[4].name, "attack2_center.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[5].name, "attack2_left.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[6].name, "attack2_right.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[7].name, "fly.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[8].name, "fly_left.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[9].name, "fly_right.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[10].name, "hit_back.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[11].name, "hit_front.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[12].name, "hit_left.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[13].name, "hit_right.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[14].name, "hover.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[15].name, "idle_a.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[16].name, "idle_b.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[17].name, "idle_c.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[18].name, "landing.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[19].name, "sting.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[20].name, "takeoff.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[21].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[21].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[22].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[22].name, "walk_left.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[23].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[23].name, "walk_right.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[24].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42bf58[24].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[25].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42bf58[25].name != nullptr) return 1;
    return 0;
}
