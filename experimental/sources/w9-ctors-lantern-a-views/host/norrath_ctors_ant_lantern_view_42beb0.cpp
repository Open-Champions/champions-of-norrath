// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42beb0[21];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42beb0 != 21u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[0].name, "sant.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[1].name, "sant.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[2].name, "sant.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[3].name, "sant.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[4].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[5].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[6].name, "death1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[7].name, "death2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[8].name, "death3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[9].name, "hit_reaction.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[10].name, "idle_a.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[11].name, "idle_b.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[12].name, "idle_c.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[13].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[14].name, "spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[15].name, "projectile.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[16].name, "projectile.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[17].name, "projectile.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[18].name, "spel_ant_spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42beb0[19].name, "spel_ant_spit_hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[20].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42beb0[20].name != nullptr) return 1;
    return 0;
}
