// Observed .data symbol lavaMonsterResourceMap at 0x42b010, 264 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "lavabst.lmp";
static char text_1[] = "lavabeast.vif";
static char text_2[] = "lavabeast.tex";
static char text_3[] = "lb_block.vif";
static char text_4[] = "lb_block.tex";
static char text_5[] = "death2.anm";
static char text_6[] = "death1.anm";
static char text_7[] = "hit1.anm";
static char text_8[] = "hit2.anm";
static char text_9[] = "hit3.anm";
static char text_10[] = "idle1.anm";
static char text_11[] = "lava_launch.anm";
static char text_12[] = "pivot.anm";
static char text_13[] = "rise.anm";
static char text_14[] = "roar.anm";
static char text_15[] = "run.anm";
static char text_16[] = "slam.anm";
static char text_17[] = "slap_left.anm";
static char text_18[] = "slap_right.anm";
static char text_19[] = "swipe_combo.anm";
static char text_20[] = "swipe_left.anm";
static char text_21[] = "swipe_right.anm";
static char text_22[] = "taunt.anm";
static char text_23[] = "throw.anm";
static char text_24[] = "throw2.anm";
static char text_25[] = "underground.anm";
static char text_26[] = "walk.anm";
static char text_27[] = "block.anm";
static char text_28[] = "lavarock.vif";
static char text_29[] = "lavarock.tex";
static char text_30[] = "spel_lavabst_throw.anm";
static char text_31[] = "spel_lavabst_throw_hit.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42b010[33] = {
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
    {text_20, 0u},
    {text_21, 0u},
    {text_22, 0u},
    {text_23, 0u},
    {text_24, 0u},
    {text_25, 0u},
    {text_26, 0u},
    {text_27, 0u},
    {text_28, 0u},
    {text_29, 0u},
    {text_30, 0u},
    {text_31, 0u},
    {nullptr, 0u},
};
}
