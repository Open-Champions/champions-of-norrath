// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42cfc8[16];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42cfc8 != 16u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[0].name, "goblin_z.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[1].name, "g_arrow.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[2].name, "g_staff.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[3].name, "g_bow.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[4].name, "g_shield.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[5].name, "g_axe.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[6].name, "g_sword1.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[7].name, "g_sword2.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[8].name, "g_arrow.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[9].name, "g_staff.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[10].name, "g_bow.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[11].name, "g_shield.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[12].name, "g_axe.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[13].name, "g_sword1.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cfc8[14].name, "g_sword2.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[15].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42cfc8[15].name != nullptr) return 1;
    return 0;
}
