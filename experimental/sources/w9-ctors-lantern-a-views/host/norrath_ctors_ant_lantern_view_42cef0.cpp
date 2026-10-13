// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42cef0[27];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42cef0 != 27u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[0].name, "goblin.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[1].name, "goblin.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[2].name, "goblin.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[3].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[4].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[5].name, "attack_xy.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[6].name, "bowattak.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[7].name, "bowidle.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[8].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[9].name, "hit1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[10].name, "hit2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[11].name, "hit3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[12].name, "idlea.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[13].name, "idleb.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[14].name, "run.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[15].name, "shmnidle.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[16].name, "shmnspell.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[17].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[18].name, "block.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[19].name, "blockhit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[20].name, "knokdown.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[21].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[21].name, "knokinto.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[22].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[22].name, "knokdwn_l1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[23].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[23].name, "knokdwn_l2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[24].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[24].name, "knokdwn_r1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[25].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42cef0[25].name, "knokdwn_r2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[26].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42cef0[26].name != nullptr) return 1;
    return 0;
}
