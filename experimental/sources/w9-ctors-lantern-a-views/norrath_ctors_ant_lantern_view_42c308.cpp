// Observed .data symbol fireBeetleResourceMap at 0x42c308, 456 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Constructors pass this address as the resource map. Unknown: which slot
// indexes the game uses. Pointer identity with other copies of these names
// is not preserved.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "beetle.lmp";
static char text_1[] = "beetle.vif";
static char text_2[] = "beetle.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "death1.anm";
static char text_5[] = "death2.anm";
static char text_6[] = "death3.anm";
static char text_7[] = "death4.anm";
static char text_8[] = "fly.anm";
static char text_9[] = "hitreact.anm";
static char text_10[] = "idle1a.anm";
static char text_11[] = "idle1b.anm";
static char text_12[] = "idle2.anm";
static char text_13[] = "landing.anm";
static char text_14[] = "takeoff.anm";
static char text_15[] = "walk.anm";
static char text_16[] = "spel_beetle_spit.anm";
static char text_17[] = "spel_beetle_spit_hit.anm";
static char text_19[] = "lbeetle.lmp";
static char text_20[] = "beetle.vif";
static char text_21[] = "beetle.tex";
static char text_22[] = "attack1.anm";
static char text_23[] = "death1.anm";
static char text_24[] = "death2.anm";
static char text_25[] = "death3.anm";
static char text_26[] = "death4.anm";
static char text_27[] = "fly.anm";
static char text_28[] = "hitreact.anm";
static char text_29[] = "idle1a.anm";
static char text_30[] = "idle1b.anm";
static char text_31[] = "idle2.anm";
static char text_32[] = "landing.anm";
static char text_33[] = "takeoff.anm";
static char text_34[] = "walk.anm";
static char text_35[] = "spel_beetle_spit.anm";
static char text_36[] = "spel_beetle_spit_hit.anm";
static char text_38[] = "bbeetle.lmp";
static char text_39[] = "beetle.vif";
static char text_40[] = "beetle.tex";
static char text_41[] = "attack1.anm";
static char text_42[] = "death1.anm";
static char text_43[] = "death2.anm";
static char text_44[] = "death3.anm";
static char text_45[] = "death4.anm";
static char text_46[] = "fly.anm";
static char text_47[] = "hitreact.anm";
static char text_48[] = "idle1a.anm";
static char text_49[] = "idle1b.anm";
static char text_50[] = "idle2.anm";
static char text_51[] = "landing.anm";
static char text_52[] = "takeoff.anm";
static char text_53[] = "walk.anm";
static char text_54[] = "spel_beetle_spit.anm";
static char text_55[] = "spel_beetle_spit_hit.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c308[57] = {
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
    {nullptr, 0u},
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
    {nullptr, 0u},
    {text_38, 0u},
    {text_39, 0u},
    {text_40, 0u},
    {text_41, 0u},
    {text_42, 0u},
    {text_43, 0u},
    {text_44, 0u},
    {text_45, 0u},
    {text_46, 0u},
    {text_47, 0u},
    {text_48, 0u},
    {text_49, 0u},
    {text_50, 0u},
    {text_51, 0u},
    {text_52, 0u},
    {text_53, 0u},
    {text_54, 0u},
    {text_55, 0u},
    {nullptr, 0u},
};
}
