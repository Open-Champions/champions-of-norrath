// Observed .data symbol ulthorkResourceMap at 0x42b478, 208 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "ulthork.lmp";
static char text_1[] = "ulthork.vif";
static char text_2[] = "ulthork.tex";
static char text_3[] = "ulthorkb.tex";
static char text_4[] = "ulthorkc.tex";
static char text_5[] = "attack1.anm";
static char text_6[] = "attack2.anm";
static char text_7[] = "attack3.anm";
static char text_8[] = "death1.anm";
static char text_9[] = "death2.anm";
static char text_10[] = "hit1.anm";
static char text_11[] = "hit2.anm";
static char text_12[] = "hit3.anm";
static char text_13[] = "hit4.anm";
static char text_14[] = "idle1.anm";
static char text_15[] = "idle2.anm";
static char text_16[] = "run.anm";
static char text_17[] = "walk.anm";
static char text_18[] = "cast.anm";
static char text_19[] = "boneaxe.vif";
static char text_20[] = "boneaxe.tex";
static char text_21[] = "boneclub.vif";
static char text_22[] = "boneclub.tex";
static char text_23[] = "bonestaff.vif";
static char text_24[] = "bonestaff.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42b478[26] = {
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
    {nullptr, 0u},
};
}
