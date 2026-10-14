// Observed .data symbol scorpionResourceMap at 0x42b330, 168 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "scorpion.lmp";
static char text_1[] = "scorpion.vif";
static char text_2[] = "scorpion.tex";
static char text_3[] = "scorpion_black.tex";
static char text_4[] = "aggression.anm";
static char text_5[] = "attack1.anm";
static char text_6[] = "attack2.anm";
static char text_7[] = "attack3.anm";
static char text_8[] = "attack4.anm";
static char text_9[] = "block.anm";
static char text_10[] = "block_hit.anm";
static char text_11[] = "death1.anm";
static char text_12[] = "death2.anm";
static char text_13[] = "flank_left.anm";
static char text_14[] = "flank_right.anm";
static char text_15[] = "hit_reaction.anm";
static char text_16[] = "idle1.anm";
static char text_17[] = "idle2.anm";
static char text_18[] = "walk1.anm";
static char text_19[] = "walk2.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42b330[21] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_14, 0u},
    {text_15, 0u},
    {text_16, 0u},
    {text_17, 0u},
    {text_18, 0u},
    {text_19, 0u},
    {nullptr, 0u},
};
}
