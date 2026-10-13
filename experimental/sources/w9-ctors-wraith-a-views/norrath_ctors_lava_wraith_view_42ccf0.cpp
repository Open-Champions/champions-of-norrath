// Observed .data symbol orc2WeaponResourceMap at 0x42ccf0, 288 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "orc_wep2.lmp";
static char text_1[] = "oaxe1.vif";
static char text_2[] = "obow1.vif";
static char text_3[] = "ocleever.vif";
static char text_4[] = "ohook.vif";
static char text_5[] = "omace.vif";
static char text_6[] = "oquiver.vif";
static char text_7[] = "oscimitar.vif";
static char text_8[] = "oshield1.vif";
static char text_9[] = "oshield2.vif";
static char text_10[] = "oshield3.vif";
static char text_11[] = "oshield4.vif";
static char text_12[] = "oshield5.vif";
static char text_13[] = "ospear.vif";
static char text_14[] = "osword1.vif";
static char text_15[] = "osword2.vif";
static char text_16[] = "osword3.vif";
static char text_17[] = "oarrow.vif";
static char text_18[] = "oaxe1.tex";
static char text_19[] = "obow1.tex";
static char text_20[] = "ocleever.tex";
static char text_21[] = "ohook.tex";
static char text_22[] = "omace.tex";
static char text_23[] = "oquiver.tex";
static char text_24[] = "oscimitar.tex";
static char text_25[] = "oshield1.tex";
static char text_26[] = "oshield2.tex";
static char text_27[] = "oshield3.tex";
static char text_28[] = "oshield4.tex";
static char text_29[] = "oshield5.tex";
static char text_30[] = "ospear.tex";
static char text_31[] = "osword1.tex";
static char text_32[] = "osword2.tex";
static char text_33[] = "osword3.tex";
static char text_34[] = "oarrow.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42ccf0[36] = {
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
    {nullptr, 0u},
};
}
