// Observed .data symbol catResourceMap at 0x42b118, 184 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "cat.lmp";
static char text_1[] = "cata.vif";
static char text_2[] = "cata.tex";
static char text_3[] = "catb.tex";
static char text_4[] = "catc.tex";
static char text_5[] = "catd.tex";
static char text_6[] = "cate.tex";
static char text_7[] = "catf.tex";
static char text_8[] = "catg.tex";
static char text_9[] = "cath.tex";
static char text_10[] = "cati.tex";
static char text_11[] = "cat_gallop.anm";
static char text_12[] = "cat_gallopleft.anm";
static char text_13[] = "cat_gallopright.anm";
static char text_14[] = "cat_idlea.anm";
static char text_15[] = "cat_idleb.anm";
static char text_16[] = "cat_sitidlea.anm";
static char text_17[] = "cat_sitidleb.anm";
static char text_18[] = "cat_sitidlec.anm";
static char text_19[] = "cat_walk.anm";
static char text_20[] = "cat_walkleft.anm";
static char text_21[] = "cat_walkright.anm";
extern "C" {
ResourceSlot norrath_actor_view_42b118[23] = {
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
    {nullptr, 0u},
};
}
