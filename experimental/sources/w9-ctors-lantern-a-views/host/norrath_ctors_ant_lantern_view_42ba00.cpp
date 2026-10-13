// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42ba00[21];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42ba00 != 21u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[0].name, "g_gneric.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[1].name, "gnomes.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[2].name, "gnomes.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[3].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[4].name, "idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[5].name, "idle3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[6].name, "run.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[7].name, "s_attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[8].name, "s_attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[9].name, "s_attack3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[10].name, "s_block.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[11].name, "s_blockhit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[12].name, "s_idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[13].name, "talk1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[14].name, "talk2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[15].name, "talk3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[16].name, "w_hammer.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[17].name, "w_idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[18].name, "w_idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42ba00[19].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[20].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42ba00[20].name != nullptr) return 1;
    return 0;
}
