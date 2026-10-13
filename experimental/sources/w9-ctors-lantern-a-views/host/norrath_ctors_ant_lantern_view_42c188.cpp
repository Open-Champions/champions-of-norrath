// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_ctors_ant_lantern_view_42c188[20];
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42c188 != 20u * sizeof(ResourceSlot)) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[0].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[0].name, "blkwidow.lmp")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[1].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[1].name, "spider.vif")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[2].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[2].name, "spider.tex")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[3].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[3].name, "attack1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[4].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[4].name, "attack2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[5].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[5].name, "attack2demo.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[6].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[6].name, "attack3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[7].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[7].name, "capture.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[8].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[8].name, "death1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[9].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[9].name, "death2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[10].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[10].name, "death3.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[11].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[11].name, "death4.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[12].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[12].name, "eat.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[13].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[13].name, "hit.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[14].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[14].name, "idle1.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[15].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[15].name, "idle2.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[16].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[16].name, "webshot.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[17].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[17].name, "walk.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[18].unused != 0u) return 1;
    if (!same(norrath_ctors_ant_lantern_view_42c188[18].name, "climbup.anm")) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[19].unused != 0u) return 1;
    if (norrath_ctors_ant_lantern_view_42c188[19].name != nullptr) return 1;
    return 0;
}
