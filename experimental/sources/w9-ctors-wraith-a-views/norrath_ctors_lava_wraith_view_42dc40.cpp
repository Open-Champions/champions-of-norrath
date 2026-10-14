// Observed .data symbol wesResourceMap at 0x42dc40, 336 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "we_sldr.lmp";
static char text_1[] = "wes_low.vif";
static char text_2[] = "wes_low.tex";
static char text_3[] = "wes0_idle.anm";
static char text_4[] = "wes0_talk.anm";
static char text_5[] = "wes1_idle.anm";
static char text_6[] = "wes1_talk.anm";
static char text_7[] = "wes2_idle.anm";
static char text_8[] = "wes2_talk.anm";
static char text_9[] = "wes3_idle.anm";
static char text_10[] = "wes3_talk.anm";
static char text_11[] = "wes4_idle.anm";
static char text_12[] = "wes4_talk.anm";
static char text_13[] = "wes5_idle.anm";
static char text_14[] = "wes5_talk.anm";
static char text_15[] = "wes6_idle.anm";
static char text_16[] = "wes6_talk.anm";
static char text_17[] = "wes7_atak.anm";
static char text_18[] = "wes7_idle.anm";
static char text_19[] = "wes7_jog.anm";
static char text_20[] = "wes7_walk.anm";
static char text_21[] = "wes8_idle1.anm";
static char text_22[] = "wes8_idle2.anm";
static char text_23[] = "wes8_jog.anm";
static char text_24[] = "wes8_shot1.anm";
static char text_25[] = "wes8_shot2.anm";
static char text_26[] = "wes8_walk.anm";
static char text_27[] = "wes9_atak.anm";
static char text_28[] = "wes9_bhit.anm";
static char text_29[] = "wes9_blok.anm";
static char text_30[] = "wes9_idle.anm";
static char text_31[] = "wes10_idle.anm";
static char text_32[] = "wes10_idle1.anm";
static char text_33[] = "wes10_jog.anm";
static char text_34[] = "wes10_shot1.anm";
static char text_35[] = "wes10_kick.anm";
static char text_36[] = "wes10_open.anm";
static char text_37[] = "wes10_walk.anm";
static char text_38[] = "wes_death.anm";
static char text_39[] = "wes_arrow.vif";
static char text_40[] = "wes_arrow.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42dc40[42] = {
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
    {text_32, 0u},
    {text_33, 0u},
    {text_34, 0u},
    {text_35, 0u},
    {text_36, 0u},
    {text_37, 0u},
    {text_38, 0u},
    {text_39, 0u},
    {text_40, 0u},
    {nullptr, 0u},
};
}
