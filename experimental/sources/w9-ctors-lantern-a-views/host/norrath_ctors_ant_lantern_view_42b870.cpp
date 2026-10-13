// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42b870[29];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42b870 != 29u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[0].name, "innoruuk.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[1].name, "innoruuk.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[2].name, "innoruuk.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[3].name, "cast1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[4].name, "death.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[5].name, "drophamr.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[6].name, "evileye1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[7].name, "evileye2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[8].name, "evileye3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[9].name, "ham180.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[10].name, "hamtak1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[11].name, "hamtak2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[12].name, "hateaura.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[13].name, "hitreact1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[14].name, "idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[15].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[16].name, "lightning.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[17].name, "stomp.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[18].name, "talk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[19].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[19].name, "walk1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[20].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[20].name, "walk2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[21].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[21].name, "hammer.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[22].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[22].name, "hammer.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[23].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[23].name, "spel_innoruuk_lightning.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[24].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[24].name, "spel_innoruuk_shock.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[25].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[25].name, "spel_innoruuk_shock_hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[26].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[26].name, "spel_innoruuk_stomp.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[27].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42b870[27].name, "spel_innoruuk_summon.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[28].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42b870[28].name != nullptr) return 1;
    return 0;
}
