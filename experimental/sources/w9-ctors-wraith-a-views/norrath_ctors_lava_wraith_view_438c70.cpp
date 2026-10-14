// Observed .data symbol mDarkElfResourceMap at 0x438c70, 552 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "dark_mal.lmp";
static char text_1[] = "darkmale.vif";
static char text_2[] = "darkmale.tex";
static char text_3[] = "asas_atak1.anm";
static char text_4[] = "asas_atak2.anm";
static char text_5[] = "asas_blok1.anm";
static char text_6[] = "asas_blok2.anm";
static char text_7[] = "asas_evade.anm";
static char text_8[] = "asas_evade2.anm";
static char text_9[] = "asas_idle.anm";
static char text_10[] = "asas_move.anm";
static char text_11[] = "asas_throw1.anm";
static char text_12[] = "asas_throw2.anm";
static char text_13[] = "dhw_atak1.anm";
static char text_14[] = "dhw_atak2.anm";
static char text_15[] = "dhw_atak3.anm";
static char text_16[] = "dhw_blok.anm";
static char text_17[] = "dhw_blokhit.anm";
static char text_18[] = "dhw_idlea.anm";
static char text_19[] = "ohw_atak1.anm";
static char text_20[] = "ohw_atak2.anm";
static char text_21[] = "ohw_atak3.anm";
static char text_22[] = "ohw_block.anm";
static char text_23[] = "ohw_blockhit.anm";
static char text_24[] = "ohw_idlea.anm";
static char text_25[] = "hitreact.anm";
static char text_26[] = "run.anm";
static char text_27[] = "walk.anm";
static char text_28[] = "spman_atak1.anm";
static char text_29[] = "spman_atak2.anm";
static char text_30[] = "spman_hitreact.anm";
static char text_31[] = "spman_idle.anm";
static char text_32[] = "spman_run.anm";
static char text_33[] = "spman_walk.anm";
static char text_34[] = "bow_atak1.anm";
static char text_35[] = "bow_hitreact.anm";
static char text_36[] = "bow_idlea.anm";
static char text_37[] = "bow_walk.anm";
static char text_38[] = "knokdown1.anm";
static char text_39[] = "knokdown1b.anm";
static char text_40[] = "knokdown2.anm";
static char text_41[] = "knokdown2b.anm";
static char text_42[] = "knokdown3.anm";
static char text_43[] = "knokdown3b.anm";
static char text_44[] = "knokdown4.anm";
static char text_45[] = "knokdown4b.anm";
static char text_46[] = "shield1.vif";
static char text_47[] = "shield1.tex";
static char text_48[] = "shield2.vif";
static char text_49[] = "shield2.tex";
static char text_50[] = "shield3.vif";
static char text_51[] = "shield3.tex";
static char text_52[] = "spear1.vif";
static char text_53[] = "spear1.tex";
static char text_54[] = "star_1.vif";
static char text_55[] = "star_1.tex";
static char text_56[] = "star_2.vif";
static char text_57[] = "star_2.tex";
static char text_58[] = "sword_2hnd.vif";
static char text_59[] = "sword_2hnd.tex";
static char text_60[] = "sword_dw.vif";
static char text_61[] = "sword_dw.tex";
static char text_62[] = "sword_rhand.vif";
static char text_63[] = "sword_rhand.tex";
static char text_64[] = "bow.vif";
static char text_65[] = "bow.tex";
static char text_66[] = "arrow.vif";
static char text_67[] = "arrow.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_438c70[69] = {
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
    {text_56, 0u},
    {text_57, 0u},
    {text_58, 0u},
    {text_59, 0u},
    {text_60, 0u},
    {text_61, 0u},
    {text_62, 0u},
    {text_63, 0u},
    {text_64, 0u},
    {text_65, 0u},
    {text_66, 0u},
    {text_67, 0u},
    {nullptr, 0u},
};
}
