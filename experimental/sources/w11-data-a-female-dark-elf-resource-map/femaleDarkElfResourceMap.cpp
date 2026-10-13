// femaleDarkElfResourceMap at 0x00421158, 904 bytes of initialized .data.
// Observed: 113 slots of 8 bytes. Each of the first 112 is a pointer to a file name followed by a zero
// word; the last slot is two zero words. The 112 names use 72 distinct strings (repeats share
// one string in the image, kept here by naming each distinct string once). The names are file
// names with the extensions lmp, vif, tex, anm and vag. Pointer identity with copies of the same
// names in other tables is not preserved.
// Inferred: the zero word is a per-slot resource cache that the loader fills in; the slot
// order is the order the loader indexes. Unknown: what each index means.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "elf_fdk2.lmp";
static char text_1[] = "nw_death.anm";
static char text_2[] = "dha_death.anm";
static char text_3[] = "silent.vag";
static char text_4[] = "nw_walk.anm";
static char text_5[] = "ohw_idlea.anm";
static char text_6[] = "ohw_walk.anm";
static char text_7[] = "ohw_jog.anm";
static char text_8[] = "ohw_cast.anm";
static char text_9[] = "ohw_atakseq1.anm";
static char text_10[] = "ohw_hitreaction.anm";
static char text_11[] = "ohw_block.anm";
static char text_12[] = "ohw_blockhit.anm";
static char text_13[] = "ohw_blockop.anm";
static char text_14[] = "ohw_blockgo.anm";
static char text_15[] = "ohw_evade.anm";
static char text_16[] = "dhs_idlea.anm";
static char text_17[] = "dhs_walk.anm";
static char text_18[] = "dhs_jog.anm";
static char text_19[] = "dhs_cast.anm";
static char text_20[] = "dhs_atakseq1.anm";
static char text_21[] = "dhs_hitreaction.anm";
static char text_22[] = "dhs_block.anm";
static char text_23[] = "dhs_blockhit.anm";
static char text_24[] = "dhs_blockop.anm";
static char text_25[] = "dhs_blockgo.anm";
static char text_26[] = "dhs_evade.anm";
static char text_27[] = "dha_idlea.anm";
static char text_28[] = "dha_walk.anm";
static char text_29[] = "dha_jog.anm";
static char text_30[] = "dha_cast.anm";
static char text_31[] = "dha_atakseq1.anm";
static char text_32[] = "dha_hitreaction.anm";
static char text_33[] = "dha_block.anm";
static char text_34[] = "dha_blockhit.anm";
static char text_35[] = "dha_blockop.anm";
static char text_36[] = "dha_blockgo.anm";
static char text_37[] = "dha_evade.anm";
static char text_38[] = "l_idlea.anm";
static char text_39[] = "l_walk.anm";
static char text_40[] = "l_jog.anm";
static char text_41[] = "l_cast.anm";
static char text_42[] = "l_atakseq1.anm";
static char text_43[] = "l_hitreaction.anm";
static char text_44[] = "l_block.anm";
static char text_45[] = "l_blockhit.anm";
static char text_46[] = "l_blockop.anm";
static char text_47[] = "l_blockgo.anm";
static char text_48[] = "l_evade.anm";
static char text_49[] = "b_idlea.anm";
static char text_50[] = "b_walk.anm";
static char text_51[] = "b_jog.anm";
static char text_52[] = "b_cast.anm";
static char text_53[] = "b_attacka.anm";
static char text_54[] = "b_hitreaction.anm";
static char text_55[] = "nw_idlea.anm";
static char text_56[] = "nw_jog.anm";
static char text_57[] = "nw_cast.anm";
static char text_58[] = "nw_atakseq1.anm";
static char text_59[] = "nw_hitreaction.anm";
static char text_60[] = "throw.anm";
static char text_61[] = "dw_idlea.anm";
static char text_62[] = "dw_walk.anm";
static char text_63[] = "dw_jog.anm";
static char text_64[] = "dw_cast.anm";
static char text_65[] = "dw_atakseq1.anm";
static char text_66[] = "dw_hitreaction.anm";
static char text_67[] = "dw_block.anm";
static char text_68[] = "dw_blockhit.anm";
static char text_69[] = "dw_blockop.anm";
static char text_70[] = "dw_blockgo.anm";
static char text_71[] = "dw_evade.anm";
extern "C" {
ResourceSlot femaleDarkElfResourceMap[113] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_7, 0u},
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
    {text_18, 0u},
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
    {text_29, 0u},
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
    {text_40, 0u},
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
    {text_51, 0u},
    {text_51, 0u},
    {text_52, 0u},
    {text_53, 0u},
    {text_54, 0u},
    {text_54, 0u},
    {text_54, 0u},
    {text_54, 0u},
    {text_49, 0u},
    {text_54, 0u},
    {text_55, 0u},
    {text_4, 0u},
    {text_56, 0u},
    {text_56, 0u},
    {text_56, 0u},
    {text_57, 0u},
    {text_58, 0u},
    {text_59, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_14, 0u},
    {text_15, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_7, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {text_60, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_14, 0u},
    {text_15, 0u},
    {text_61, 0u},
    {text_62, 0u},
    {text_63, 0u},
    {text_63, 0u},
    {text_63, 0u},
    {text_64, 0u},
    {text_65, 0u},
    {text_66, 0u},
    {text_67, 0u},
    {text_68, 0u},
    {text_69, 0u},
    {text_70, 0u},
    {text_71, 0u},
    {nullptr, 0u},
};
}
