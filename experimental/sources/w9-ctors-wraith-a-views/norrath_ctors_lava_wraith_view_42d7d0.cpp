// Observed .data symbol skeletonResourceMap at 0x42d7d0, 256 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "skelnorm.lmp";
static char text_1[] = "skeleton.vif";
static char text_2[] = "skeleton.tex";
static char text_3[] = "1_rally.anm";
static char text_4[] = "2_rally.anm";
static char text_5[] = "3_rally.anm";
static char text_6[] = "4_rally.anm";
static char text_7[] = "skelwat.lmp";
static char text_8[] = "skelred.lmp";
static char text_9[] = "skele_v3.vif";
static char text_10[] = "skele_v3.tex";
static char text_11[] = "skelblue.lmp";
static char text_12[] = "skele_v2.vif";
static char text_13[] = "skele_v2.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d7d0[32] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {nullptr, 0u},
    {text_7, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {nullptr, 0u},
    {text_8, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {nullptr, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {nullptr, 0u},
};
}
