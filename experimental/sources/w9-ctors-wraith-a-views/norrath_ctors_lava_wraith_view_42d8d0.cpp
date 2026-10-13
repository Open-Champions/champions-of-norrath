// Observed .data symbol skeletonResMap at 0x42d8d0, 800 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "skelcom.lmp";
static char text_1[] = "1_ataka.anm";
static char text_2[] = "1_atakb.anm";
static char text_3[] = "1_atakc.anm";
static char text_4[] = "1_blockhit.anm";
static char text_5[] = "1_blok.anm";
static char text_6[] = "1_hitreact.anm";
static char text_7[] = "1_idlea.anm";
static char text_8[] = "1_idleb.anm";
static char text_9[] = "1_knokbk.anm";
static char text_10[] = "1_march.anm";
static char text_11[] = "1_rally.anm";
static char text_12[] = "1_run.anm";
static char text_13[] = "1_walk.anm";
static char text_14[] = "2_ataka.anm";
static char text_15[] = "2_atakb.anm";
static char text_16[] = "2_blockhit.anm";
static char text_17[] = "2_block.anm";
static char text_18[] = "2_hitreact.anm";
static char text_19[] = "2_idlea.anm";
static char text_20[] = "2_idleb.anm";
static char text_21[] = "2_knokbk.anm";
static char text_22[] = "2_march.anm";
static char text_23[] = "2_rally.anm";
static char text_24[] = "2_run.anm";
static char text_25[] = "2_walk.anm";
static char text_26[] = "3_atakb.anm";
static char text_27[] = "3_ataka.anm";
static char text_28[] = "3_blockhit.anm";
static char text_29[] = "3_block.anm";
static char text_30[] = "3_hitreact.anm";
static char text_31[] = "3_idlea.anm";
static char text_32[] = "3_idleb.anm";
static char text_33[] = "3_knokbk.anm";
static char text_34[] = "3_march.anm";
static char text_35[] = "3_rally.anm";
static char text_36[] = "3_run.anm";
static char text_37[] = "3_walk.anm";
static char text_38[] = "4_atakb.anm";
static char text_39[] = "4_ataka.anm";
static char text_40[] = "4_blockhit.anm";
static char text_41[] = "4_block.anm";
static char text_42[] = "4_hitreact.anm";
static char text_43[] = "4_idlea.anm";
static char text_44[] = "4_idleb.anm";
static char text_45[] = "4_knokbk.anm";
static char text_46[] = "4_march.anm";
static char text_47[] = "4_rally.anm";
static char text_48[] = "4_run.anm";
static char text_49[] = "4_walk.anm";
static char text_50[] = "5_ataka.anm";
static char text_51[] = "5_atakb.anm";
static char text_52[] = "5_hitreact.anm";
static char text_53[] = "5_idlea.anm";
static char text_54[] = "5_idleb.anm";
static char text_55[] = "5_bkstep.anm";
static char text_56[] = "5_march.anm";
static char text_57[] = "5_run.anm";
static char text_58[] = "5_walk.anm";
static char text_59[] = "1_command.anm";
static char text_60[] = "4_bkstep.anm";
static char text_61[] = "death1.anm";
static char text_62[] = "death2.anm";
static char text_63[] = "death3.anm";
static char text_64[] = "death4.anm";
static char text_65[] = "death5.anm";
static char text_66[] = "hop_l.anm";
static char text_67[] = "hop_r.anm";
static char text_68[] = "stumble_a.anm";
static char text_69[] = "stumble_b.anm";
static char text_70[] = "1_axe.vif";
static char text_71[] = "1_axe.tex";
static char text_72[] = "swd1.vif";
static char text_73[] = "swd1.tex";
static char text_74[] = "arrow.vif";
static char text_75[] = "arrow.tex";
static char text_76[] = "bow.vif";
static char text_77[] = "bow.tex";
static char text_78[] = "dhaxe1.vif";
static char text_79[] = "dhaxe1.tex";
static char text_80[] = "dhaxe2.vif";
static char text_81[] = "dhaxe2.tex";
static char text_82[] = "dhswd1.vif";
static char text_83[] = "dhswd1.tex";
static char text_84[] = "shield1.vif";
static char text_85[] = "shield1.tex";
static char text_86[] = "shield2.vif";
static char text_87[] = "shield2.tex";
static char text_88[] = "spear1.vif";
static char text_89[] = "spear1.tex";
static char text_90[] = "spear2.vif";
static char text_91[] = "spear2.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d8d0[100] = {
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
    {text_50, 0u},
    {text_52, 0u},
    {text_53, 0u},
    {text_52, 0u},
    {text_53, 0u},
    {text_54, 0u},
    {text_55, 0u},
    {text_56, 0u},
    {text_53, 0u},
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
    {text_68, 0u},
    {text_69, 0u},
    {text_70, 0u},
    {text_71, 0u},
    {text_72, 0u},
    {text_73, 0u},
    {text_74, 0u},
    {text_75, 0u},
    {text_76, 0u},
    {text_77, 0u},
    {text_78, 0u},
    {text_79, 0u},
    {text_80, 0u},
    {text_81, 0u},
    {text_82, 0u},
    {text_83, 0u},
    {text_84, 0u},
    {text_85, 0u},
    {text_86, 0u},
    {text_87, 0u},
    {text_88, 0u},
    {text_89, 0u},
    {text_90, 0u},
    {text_91, 0u},
    {nullptr, 0u},
};
}
