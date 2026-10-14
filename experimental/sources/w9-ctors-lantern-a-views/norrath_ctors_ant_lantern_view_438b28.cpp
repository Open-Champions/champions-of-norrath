// Observed .data symbol femDarkElfResourceMap at 0x438b28, 328 bytes.
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
static char text_0[] = "dark_fem.lmp";
static char text_1[] = "darkfem.vif";
static char text_2[] = "darkfem.tex";
static char text_3[] = "evade.anm";
static char text_4[] = "knkdown_1a.anm";
static char text_5[] = "knkdown_1b.anm";
static char text_6[] = "knkdown_2a.anm";
static char text_7[] = "knkdown_2b.anm";
static char text_8[] = "knkdown_3a.anm";
static char text_9[] = "knkdown_3b.anm";
static char text_10[] = "knkdown_4a.anm";
static char text_11[] = "knkdown_4b.anm";
static char text_12[] = "nec_blitz.anm";
static char text_13[] = "nec_cast1.anm";
static char text_14[] = "nec_cast2.anm";
static char text_15[] = "nec_hitreaction.anm";
static char text_16[] = "nec_idlea.anm";
static char text_17[] = "nec_run.anm";
static char text_18[] = "sld_atak1.anm";
static char text_19[] = "sld_atak2.anm";
static char text_20[] = "sld_atak3.anm";
static char text_21[] = "sld_atak4.anm";
static char text_22[] = "sld_block.anm";
static char text_23[] = "sld_blockhit.anm";
static char text_24[] = "sld_fake.anm";
static char text_25[] = "sld_hitreaction.anm";
static char text_26[] = "sld_idlea.anm";
static char text_27[] = "sld_run.anm";
static char text_28[] = "walk.anm";
static char text_29[] = "sword1.vif";
static char text_30[] = "sword1.tex";
static char text_31[] = "sword2.vif";
static char text_32[] = "sword2.tex";
static char text_33[] = "shield1.vif";
static char text_34[] = "shield1.tex";
static char text_35[] = "shield2.vif";
static char text_36[] = "shield2.tex";
static char text_37[] = "shield3.vif";
static char text_38[] = "shield3.tex";
static char text_39[] = "spel_poisonrain.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_438b28[41] = {
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
    {nullptr, 0u},
};
}
