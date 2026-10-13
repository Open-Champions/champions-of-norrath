// Observed .data symbol orc1ResourceMap at 0x42c8e0, 376 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "orc.lmp";
static char text_1[] = "orc_low.vif";
static char text_2[] = "orc_low.tex";
static char text_3[] = "odeath1.anm";
static char text_4[] = "odeath2.anm";
static char text_5[] = "odeath3.anm";
static char text_6[] = "odeath4.anm";
static char text_7[] = "odeath5.anm";
static char text_8[] = "odeath6.anm";
static char text_9[] = "faceplant1.anm";
static char text_10[] = "faceplant2.anm";
static char text_11[] = "ohitreact1.anm";
static char text_12[] = "ohitreact2.anm";
static char text_13[] = "ohitreact3.anm";
static char text_14[] = "ohitreact4.anm";
static char text_15[] = "orun1.anm";
static char text_16[] = "orun2.anm";
static char text_17[] = "orun3.anm";
static char text_18[] = "orun4.anm";
static char text_19[] = "oattack1.anm";
static char text_20[] = "oattack2.anm";
static char text_21[] = "oattack3.anm";
static char text_22[] = "oattack4.anm";
static char text_23[] = "o6attack.anm";
static char text_24[] = "oattackseq1.anm";
static char text_25[] = "oattackseq2.anm";
static char text_26[] = "oattackseq3.anm";
static char text_27[] = "attack_xy.anm";
static char text_28[] = "oblock.anm";
static char text_29[] = "oroar.anm";
static char text_30[] = "o1idlea.anm";
static char text_31[] = "o1idleb.anm";
static char text_32[] = "o2idlea.anm";
static char text_33[] = "o2idleb.anm";
static char text_34[] = "o3idlea.anm";
static char text_35[] = "o3idleb.anm";
static char text_36[] = "o5idlea.anm";
static char text_37[] = "o5idleb.anm";
static char text_38[] = "o4idlea.anm";
static char text_39[] = "o4idleb.anm";
static char text_40[] = "o6idlea.anm";
static char text_41[] = "owalk1.anm";
static char text_42[] = "owalk2.anm";
static char text_43[] = "owalk3.anm";
static char text_44[] = "owalk4.anm";
static char text_45[] = "spel_poisonrain.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42c8e0[47] = {
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
    {text_41, 0u},
    {text_42, 0u},
    {text_43, 0u},
    {text_44, 0u},
    {text_45, 0u},
    {nullptr, 0u},
};
}
