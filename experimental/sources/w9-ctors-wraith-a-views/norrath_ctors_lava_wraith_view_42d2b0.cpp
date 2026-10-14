// Observed .data symbol mummyResourceMap at 0x42d2b0, 464 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "mummy.lmp";
static char text_1[] = "mummy.vif";
static char text_2[] = "mummy.tex";
static char text_3[] = "dhw_attack1.anm";
static char text_4[] = "dhw_attack2.anm";
static char text_5[] = "dhw_block.anm";
static char text_6[] = "dhw_blockhit.anm";
static char text_7[] = "dhw_death1.anm";
static char text_8[] = "dhw_death2.anm";
static char text_9[] = "dhw_hit.anm";
static char text_10[] = "dhw_idle1.anm";
static char text_11[] = "dhw_run.anm";
static char text_12[] = "dhw_sneak.anm";
static char text_13[] = "dhw_walk.anm";
static char text_14[] = "ohw_attack1.anm";
static char text_15[] = "ohw_attack2.anm";
static char text_16[] = "ohw_combo1.anm";
static char text_17[] = "ohw_attack1b.anm";
static char text_18[] = "ohw_attack2b.anm";
static char text_19[] = "ohw_combo1b.anm";
static char text_20[] = "ohw_block.anm";
static char text_21[] = "ohw_blockhit.anm";
static char text_22[] = "ohw_death1.anm";
static char text_23[] = "ohw_death2.anm";
static char text_24[] = "ohw_hit.anm";
static char text_25[] = "ohw_idle1.anm";
static char text_26[] = "ohw_idle2.anm";
static char text_27[] = "ohw_idle3.anm";
static char text_28[] = "ohw_idle4.anm";
static char text_29[] = "ohw_run.anm";
static char text_30[] = "ohw_sneak.anm";
static char text_31[] = "ohw_walk.anm";
static char text_32[] = "s_attack1.anm";
static char text_33[] = "s_attack2.anm";
static char text_34[] = "s_attack3.anm";
static char text_35[] = "s_block.anm";
static char text_36[] = "s_blockhit.anm";
static char text_37[] = "s_death1.anm";
static char text_38[] = "s_death2.anm";
static char text_39[] = "s_hit.anm";
static char text_40[] = "s_idle1.anm";
static char text_41[] = "s_run.anm";
static char text_42[] = "s_walk.anm";
static char text_43[] = "s_teleport.anm";
static char text_44[] = "falchion.vif";
static char text_45[] = "falchion.tex";
static char text_46[] = "scimitar.vif";
static char text_47[] = "scimitar.tex";
static char text_48[] = "shaman_staff1.vif";
static char text_49[] = "shaman_staff1.tex";
static char text_50[] = "shaman_staff2.vif";
static char text_51[] = "shaman_staff2.tex";
static char text_52[] = "spel_mummy_fire.anm";
static char text_53[] = "spel_mummy_shoot.anm";
static char text_54[] = "spel_mummy_shoot_hit.anm";
static char text_55[] = "spel_mummy_teleport.anm";
static char text_56[] = "spel_poisonrain.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d2b0[58] = {
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
    {nullptr, 0u},
};
}
