// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42b548[29];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42b548 != 29u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[0].name, "ghoul.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[1].name, "ghoul.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[2].name, "ghoul.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[3].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[4].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[5].name, "attack3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[6].name, "crawl.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[7].name, "crouch.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[8].name, "dead.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[9].name, "dead2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[10].name, "dead3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[11].name, "dead4.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[12].name, "dead5.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[13].name, "dead6.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[14].name, "dead7.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[15].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[16].name, "hitreact.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[17].name, "hop_fwd.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[18].name, "hop_bk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[19].name, "hop_lft.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[20].name, "hop_ryt.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[21].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[21].name, "idle1a.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[22].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[22].name, "idle1b.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[23].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[23].name, "idle1c.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[24].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[24].name, "idle1d.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[25].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[25].name, "run1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[26].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[26].name, "spasm.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[27].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b548[27].name, "twitch.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[28].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42b548[28].name != nullptr) return 1;
    return 0;
}
