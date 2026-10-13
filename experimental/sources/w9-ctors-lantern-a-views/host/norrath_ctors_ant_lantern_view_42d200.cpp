// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42d200[22];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42d200 != 22u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[0].name, "a_beast.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[1].name, "arenabeast.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[2].name, "arenabeast.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[3].name, "atak_left.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[4].name, "atak_ryt.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[5].name, "atakclo1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[6].name, "atakclo2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[7].name, "atakclo22.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[8].name, "atakclo3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[9].name, "atakkill.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[10].name, "ataklng1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[11].name, "ataklng2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[12].name, "ataklng3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[13].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[14].name, "hitreact1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[15].name, "hitreact2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[16].name, "hitreact3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[17].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[18].name, "roar.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[19].name, "taunt.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42d200[20].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[21].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42d200[21].name != nullptr) return 1;
    return 0;
}
