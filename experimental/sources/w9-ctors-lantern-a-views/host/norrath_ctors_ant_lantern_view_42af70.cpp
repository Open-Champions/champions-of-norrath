// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42af70[20];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42af70 != 20u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[0].name, "ldemon.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[1].name, "ldemon.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[2].name, "ldemon.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[3].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[4].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[5].name, "attack3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[6].name, "block.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[7].name, "blockhit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[8].name, "guard.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[9].name, "hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[10].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[11].name, "idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[12].name, "run.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[13].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[14].name, "torturea.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[15].name, "tortureb.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[16].name, "torturec.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[17].name, "mgrabber.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42af70[18].name, "mgrabber.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[19].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42af70[19].name != nullptr) return 1;
    return 0;
}
