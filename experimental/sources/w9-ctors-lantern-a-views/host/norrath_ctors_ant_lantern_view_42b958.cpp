// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42b958[21];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42b958 != 21u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[0].name, "froglock.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[1].name, "froglock.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[2].name, "froglock.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[3].name, "blowatak.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[4].name, "death1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[5].name, "death2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[6].name, "hitreact.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[7].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[8].name, "idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[9].name, "idle3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[10].name, "idle4.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[11].name, "jump.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[12].name, "pokeatak.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[13].name, "sniper.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[14].name, "swim.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[15].name, "walk1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[16].name, "blowgun.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[17].name, "trident.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[18].name, "blowgun.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b958[19].name, "trident.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[20].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42b958[20].name != nullptr) return 1;
    return 0;
}
