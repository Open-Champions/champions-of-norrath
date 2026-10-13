// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42be08[21];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42be08 != 21u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[0].name, "ant.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[1].name, "ant.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[2].name, "ant.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[3].name, "ant_black.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[4].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[5].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[6].name, "death1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[7].name, "death2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[8].name, "death3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[9].name, "hit_reaction.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[10].name, "idle_a.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[11].name, "idle_b.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[12].name, "idle_c.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[13].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[14].name, "spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[15].name, "projectile.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[16].name, "projectile.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[17].name, "projectile.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[18].name, "spel_ant_spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42be08[19].name, "spel_ant_spit_hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[20].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42be08[20].name != nullptr) return 1;
    return 0;
}
