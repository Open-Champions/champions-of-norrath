// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_438a38[27];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_438a38 != 27u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[0].name, "cthulu.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[1].name, "cthulu.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[2].name, "cthulu.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[3].name, "atak_1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[4].name, "atak_2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[5].name, "atak_3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[6].name, "atak_4.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[7].name, "crawfast.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[8].name, "crawl.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[9].name, "idle.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[10].name, "run.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[11].name, "spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[12].name, "test1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[13].name, "up.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[14].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[15].name, "hitreact.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[16].name, "idlecalm.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[17].name, "roar.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[18].name, "upatak.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[19].name, "upidle.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[20].name, "wakeup.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[21].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[21].name, "projectile.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[22].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[22].name, "projectile.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[23].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[23].name, "projectile.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[24].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[24].name, "spel_cthulu_spit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[25].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_438a38[25].name, "spel_cthulu_spit_hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[26].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_438a38[26].name != nullptr) return 1;
    return 0;
}
