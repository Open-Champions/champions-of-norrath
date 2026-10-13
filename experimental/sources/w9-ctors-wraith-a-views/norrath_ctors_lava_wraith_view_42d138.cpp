// Observed .data symbol minionResourceMap at 0x42d138, 200 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "minion.lmp";
static char text_1[] = "beezabub.vif";
static char text_2[] = "beezabub.tex";
static char text_3[] = "atak_bk.anm";
static char text_4[] = "atak_f1.anm";
static char text_5[] = "atak_f2.anm";
static char text_6[] = "atak_lf.anm";
static char text_7[] = "atak_rt.anm";
static char text_8[] = "dethbreth.anm";
static char text_9[] = "hitl_adpt.anm";
static char text_10[] = "hitleft.anm";
static char text_11[] = "hitr.anm";
static char text_12[] = "hitr_adpt.anm";
static char text_13[] = "idle1.anm";
static char text_14[] = "roar.anm";
static char text_15[] = "so_catch.anm";
static char text_16[] = "so_club.anm";
static char text_17[] = "so_shoot.anm";
static char text_18[] = "death.anm";
static char text_19[] = "torment.anm";
static char text_20[] = "walk.anm";
static char text_21[] = "bow.vif";
static char text_22[] = "bow.tex";
static char text_23[] = "spel_minion_rain.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d138[25] = {
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
    {nullptr, 0u},
};
}
