// Observed .data symbol nataslaResourceMap at 0x42d0c0, 120 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "natasla.lmp";
static char text_1[] = "natasla.vif";
static char text_2[] = "natasla.tex";
static char text_3[] = "idle.anm";
static char text_4[] = "parts_1.vif";
static char text_5[] = "parts_1.tex";
static char text_6[] = "parts_2.vif";
static char text_7[] = "parts_2.tex";
static char text_8[] = "parts_3.vif";
static char text_9[] = "parts_3.tex";
static char text_10[] = "parts_4.vif";
static char text_11[] = "parts_4.tex";
static char text_12[] = "parts_5.vif";
static char text_13[] = "parts_5.tex";
extern "C" {
ResourceSlot norrath_actor_view_42d0c0[15] = {
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
    {nullptr, 0u},
};
}
