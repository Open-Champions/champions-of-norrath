// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" ResourceSlot norrath_ctors_lava_wraith_view_42b330[21];
int main() {
    if (norrath_ctors_lava_wraith_view_42b330[0].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[0].name, "scorpion.lmp") || norrath_ctors_lava_wraith_view_42b330[0].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[1].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[1].name, "scorpion.vif") || norrath_ctors_lava_wraith_view_42b330[1].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[2].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[2].name, "scorpion.tex") || norrath_ctors_lava_wraith_view_42b330[2].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[3].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[3].name, "scorpion_black.tex") || norrath_ctors_lava_wraith_view_42b330[3].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[4].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[4].name, "aggression.anm") || norrath_ctors_lava_wraith_view_42b330[4].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[5].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[5].name, "attack1.anm") || norrath_ctors_lava_wraith_view_42b330[5].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[6].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[6].name, "attack2.anm") || norrath_ctors_lava_wraith_view_42b330[6].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[7].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[7].name, "attack3.anm") || norrath_ctors_lava_wraith_view_42b330[7].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[8].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[8].name, "attack4.anm") || norrath_ctors_lava_wraith_view_42b330[8].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[9].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[9].name, "block.anm") || norrath_ctors_lava_wraith_view_42b330[9].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[10].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[10].name, "block_hit.anm") || norrath_ctors_lava_wraith_view_42b330[10].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[11].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[11].name, "death1.anm") || norrath_ctors_lava_wraith_view_42b330[11].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[12].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[12].name, "death2.anm") || norrath_ctors_lava_wraith_view_42b330[12].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[13].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[13].name, "flank_left.anm") || norrath_ctors_lava_wraith_view_42b330[13].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[14].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[14].name, "flank_right.anm") || norrath_ctors_lava_wraith_view_42b330[14].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[15].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[15].name, "hit_reaction.anm") || norrath_ctors_lava_wraith_view_42b330[15].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[16].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[16].name, "idle1.anm") || norrath_ctors_lava_wraith_view_42b330[16].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[17].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[17].name, "idle2.anm") || norrath_ctors_lava_wraith_view_42b330[17].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[18].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[18].name, "walk1.anm") || norrath_ctors_lava_wraith_view_42b330[18].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[19].name == nullptr || !same(norrath_ctors_lava_wraith_view_42b330[19].name, "walk2.anm") || norrath_ctors_lava_wraith_view_42b330[19].unused != 0u) return 1;
    if (norrath_ctors_lava_wraith_view_42b330[20].name != nullptr || norrath_ctors_lava_wraith_view_42b330[20].unused != 0u) return 1;
    return 0;
}
