// Observed .data symbol goblinWeaponResourceMap at 0x42cfc8, 128 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Constructors pass this address as the resource map. Unknown: which slot
// indexes the game uses. Pointer identity with other copies of these names
// is not preserved.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "goblin_z.lmp";
static char text_1[] = "g_arrow.vif";
static char text_2[] = "g_staff.vif";
static char text_3[] = "g_bow.vif";
static char text_4[] = "g_shield.vif";
static char text_5[] = "g_axe.vif";
static char text_6[] = "g_sword1.vif";
static char text_7[] = "g_sword2.vif";
static char text_8[] = "g_arrow.tex";
static char text_9[] = "g_staff.tex";
static char text_10[] = "g_bow.tex";
static char text_11[] = "g_shield.tex";
static char text_12[] = "g_axe.tex";
static char text_13[] = "g_sword1.tex";
static char text_14[] = "g_sword2.tex";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42cfc8[16] = {
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
    {nullptr, 0u},
};
}
