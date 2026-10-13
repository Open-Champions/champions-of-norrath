// Observed .data symbol superorcResourceMap at 0x42ce60, 144 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "superorc.lmp";
static char text_1[] = "superorc.vif";
static char text_2[] = "superorc.tex";
static char text_3[] = "atak1.anm";
static char text_4[] = "atak2.anm";
static char text_5[] = "atakrush.anm";
static char text_6[] = "atakwlk.anm";
static char text_7[] = "idle1.anm";
static char text_8[] = "idle2.anm";
static char text_9[] = "talk.anm";
static char text_10[] = "taunt.anm";
static char text_11[] = "walk.anm";
static char text_12[] = "playdead.anm";
static char text_13[] = "wakeup.anm";
static char text_14[] = "dead.anm";
static char text_15[] = "cleever.vif";
static char text_16[] = "cleever.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42ce60[18] = {
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
    {nullptr, 0u},
};
}
