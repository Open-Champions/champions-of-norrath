// Observed .data symbol gnomeResourceMap at 0x42ba00, 168 bytes.
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
static char text_0[] = "g_gneric.lmp";
static char text_1[] = "gnomes.vif";
static char text_2[] = "gnomes.tex";
static char text_3[] = "idle1.anm";
static char text_4[] = "idle2.anm";
static char text_5[] = "idle3.anm";
static char text_6[] = "run.anm";
static char text_7[] = "s_attack1.anm";
static char text_8[] = "s_attack2.anm";
static char text_9[] = "s_attack3.anm";
static char text_10[] = "s_block.anm";
static char text_11[] = "s_blockhit.anm";
static char text_12[] = "s_idle1.anm";
static char text_13[] = "talk1.anm";
static char text_14[] = "talk2.anm";
static char text_15[] = "talk3.anm";
static char text_16[] = "w_hammer.anm";
static char text_17[] = "w_idle1.anm";
static char text_18[] = "w_idle2.anm";
static char text_19[] = "walk.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42ba00[21] = {
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
    {nullptr, 0u},
};
}
