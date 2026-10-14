// Observed .data symbol wallEyeResourceMap at 0x42c710, 80 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "cdanims.lmp";
static char text_1[] = "bigeye.vif";
static char text_2[] = "bigeye.tex";
static char text_3[] = "biglens.vif";
static char text_4[] = "biglens.tex";
static char text_5[] = "biglid.vif";
static char text_6[] = "biglid.tex";
static char text_7[] = "bigeye.anm";
static char text_8[] = "biglid.anm";
extern "C" {
ResourceSlot norrath_actor_view_42c710[10] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {nullptr, 0u},
};
}
