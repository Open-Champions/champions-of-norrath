// Observed .data symbol udkResourceMap at 0x42d578, 216 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "udk.lmp";
static char text_1[] = "udk.vif";
static char text_2[] = "udk.tex";
static char text_3[] = "attak1.anm";
static char text_4[] = "attak2.anm";
static char text_5[] = "attak3.anm";
static char text_6[] = "attak4.anm";
static char text_7[] = "block.anm";
static char text_8[] = "conjure.anm";
static char text_9[] = "hitreaction.anm";
static char text_10[] = "idlea.anm";
static char text_11[] = "idleb.anm";
static char text_12[] = "taunt.anm";
static char text_13[] = "walk.anm";
static char text_14[] = "walkatak.anm";
static char text_15[] = "death.anm";
static char text_16[] = "hh_atak1.anm";
static char text_17[] = "hh_atak2.anm";
static char text_18[] = "hh_atak3.anm";
static char text_19[] = "hh_rearup.anm";
static char text_20[] = "hh_run.anm";
static char text_21[] = "hh_trot.anm";
static char text_22[] = "sword.vif";
static char text_23[] = "sword.tex";
static char text_24[] = "shield.vif";
static char text_25[] = "shield.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d578[27] = {
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
    {nullptr, 0u},
};
}
