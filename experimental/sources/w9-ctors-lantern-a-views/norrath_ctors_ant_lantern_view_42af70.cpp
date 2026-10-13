// Observed .data symbol demonResourceMap at 0x42af70, 160 bytes.
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
static char text_0[] = "ldemon.lmp";
static char text_1[] = "ldemon.vif";
static char text_2[] = "ldemon.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2.anm";
static char text_5[] = "attack3.anm";
static char text_6[] = "block.anm";
static char text_7[] = "blockhit.anm";
static char text_8[] = "guard.anm";
static char text_9[] = "hit.anm";
static char text_10[] = "idle1.anm";
static char text_11[] = "idle2.anm";
static char text_12[] = "run.anm";
static char text_13[] = "walk.anm";
static char text_14[] = "torturea.anm";
static char text_15[] = "tortureb.anm";
static char text_16[] = "torturec.anm";
static char text_17[] = "mgrabber.vif";
static char text_18[] = "mgrabber.tex";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42af70[20] = {
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
    {nullptr, 0u},
};
}
