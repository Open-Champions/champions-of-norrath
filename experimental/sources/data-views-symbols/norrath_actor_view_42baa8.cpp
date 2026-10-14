// Observed .data symbol gnomenavigatorResourceMap at 0x42baa8, 96 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "g_naviga.lmp";
static char text_1[] = "navigator.vif";
static char text_2[] = "navigator.tex";
static char text_3[] = "idle1.anm";
static char text_4[] = "idle2.anm";
static char text_5[] = "idle3.anm";
static char text_6[] = "run.anm";
static char text_7[] = "talk1.anm";
static char text_8[] = "talk2.anm";
static char text_9[] = "talk3.anm";
static char text_10[] = "walk.anm";
extern "C" {
ResourceSlot norrath_actor_view_42baa8[12] = {
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
    {nullptr, 0u},
};
}
