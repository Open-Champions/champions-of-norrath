// femaleWoodElfResourceMap at 0x00421bf0, 904 bytes of initialized .data.
// Observed: 113 slots of 8 bytes. Each of the first 112 is a pointer to a file name followed by a zero
// word; the last slot is two zero words. The 112 names use 34 distinct strings (repeats share
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
static char text_0[] = "elf_fwo2.lmp";
static char text_1[] = "nw_death.anm";
static char text_2[] = "silent.vag";
static char text_3[] = "nw_walk.anm";
static char text_4[] = "ohw_idlea.anm";
static char text_5[] = "ohw_walk.anm";
static char text_6[] = "ohw_jog.anm";
static char text_7[] = "ohw_atakseq1.anm";
static char text_8[] = "ohw_hitreaction.anm";
static char text_9[] = "ohw_block.anm";
static char text_10[] = "ohw_blockhit.anm";
static char text_11[] = "ohw_blockop.anm";
static char text_12[] = "ohw_blockgo.anm";
static char text_13[] = "ohw_evade.anm";
static char text_14[] = "b_idlea.anm";
static char text_15[] = "b_walk.anm";
static char text_16[] = "b_jog.anm";
static char text_17[] = "b_attacka.anm";
static char text_18[] = "b_hitreaction.anm";
static char text_19[] = "nw_idlea.anm";
static char text_20[] = "nw_jog.anm";
static char text_21[] = "nw_atakseq1.anm";
static char text_22[] = "nw_hitreaction.anm";
static char text_23[] = "throw.anm";
static char text_24[] = "dw_idlea.anm";
static char text_25[] = "dw_walk.anm";
static char text_26[] = "dw_jog.anm";
static char text_27[] = "dw_atakseq1.anm";
static char text_28[] = "dw_hitreaction.anm";
static char text_29[] = "dw_block.anm";
static char text_30[] = "dw_blockhit.anm";
static char text_31[] = "dw_blockop.anm";
static char text_32[] = "dw_blockgo.anm";
static char text_33[] = "dw_evade.anm";
extern "C" {
ResourceSlot femaleWoodElfResourceMap[113] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_3, 0u},
    {text_3, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_4, 0u},
    {text_14, 0u},
    {text_15, 0u},
    {text_16, 0u},
    {text_16, 0u},
    {text_16, 0u},
    {text_16, 0u},
    {text_17, 0u},
    {text_18, 0u},
    {text_14, 0u},
    {text_14, 0u},
    {text_14, 0u},
    {text_14, 0u},
    {text_14, 0u},
    {text_19, 0u},
    {text_3, 0u},
    {text_20, 0u},
    {text_20, 0u},
    {text_20, 0u},
    {text_20, 0u},
    {text_21, 0u},
    {text_22, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_6, 0u},
    {text_23, 0u},
    {text_8, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_24, 0u},
    {text_25, 0u},
    {text_26, 0u},
    {text_26, 0u},
    {text_26, 0u},
    {text_26, 0u},
    {text_27, 0u},
    {text_28, 0u},
    {text_29, 0u},
    {text_30, 0u},
    {text_31, 0u},
    {text_32, 0u},
    {text_33, 0u},
    {nullptr, 0u},
};
}
