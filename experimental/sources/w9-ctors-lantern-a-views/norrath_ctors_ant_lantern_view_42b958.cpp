// Observed .data symbol froglockResourceMap at 0x42b958, 168 bytes.
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
static char text_0[] = "froglock.lmp";
static char text_1[] = "froglock.vif";
static char text_2[] = "froglock.tex";
static char text_3[] = "blowatak.anm";
static char text_4[] = "death1.anm";
static char text_5[] = "death2.anm";
static char text_6[] = "hitreact.anm";
static char text_7[] = "idle1.anm";
static char text_8[] = "idle2.anm";
static char text_9[] = "idle3.anm";
static char text_10[] = "idle4.anm";
static char text_11[] = "jump.anm";
static char text_12[] = "pokeatak.anm";
static char text_13[] = "sniper.anm";
static char text_14[] = "swim.anm";
static char text_15[] = "walk1.anm";
static char text_16[] = "blowgun.vif";
static char text_17[] = "trident.vif";
static char text_18[] = "blowgun.tex";
static char text_19[] = "trident.tex";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42b958[21] = {
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
