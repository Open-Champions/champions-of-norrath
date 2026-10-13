// innoruukResourceMap at 0x0042b870, 232 bytes of initialized .data.
// Observed: 29 slots of 8 bytes. Each of the first 28 is a pointer to a file name followed by a zero
// word; the last slot is two zero words. The 28 names use 28 distinct strings (repeats share
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
static char text_0[] = "innoruuk.lmp";
static char text_1[] = "innoruuk.vif";
static char text_2[] = "innoruuk.tex";
static char text_3[] = "cast1.anm";
static char text_4[] = "death.anm";
static char text_5[] = "drophamr.anm";
static char text_6[] = "evileye1.anm";
static char text_7[] = "evileye2.anm";
static char text_8[] = "evileye3.anm";
static char text_9[] = "ham180.anm";
static char text_10[] = "hamtak1.anm";
static char text_11[] = "hamtak2.anm";
static char text_12[] = "hateaura.anm";
static char text_13[] = "hitreact1.anm";
static char text_14[] = "idle2.anm";
static char text_15[] = "idle1.anm";
static char text_16[] = "lightning.anm";
static char text_17[] = "stomp.anm";
static char text_18[] = "talk.anm";
static char text_19[] = "walk1.anm";
static char text_20[] = "walk2.anm";
static char text_21[] = "hammer.vif";
static char text_22[] = "hammer.tex";
static char text_23[] = "spel_innoruuk_lightning.anm";
static char text_24[] = "spel_innoruuk_shock.anm";
static char text_25[] = "spel_innoruuk_shock_hit.anm";
static char text_26[] = "spel_innoruuk_stomp.anm";
static char text_27[] = "spel_innoruuk_summon.anm";
extern "C" {
ResourceSlot innoruukResourceMap[29] = {
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
    {nullptr, 0u},
};
}
