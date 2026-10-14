// Observed .data symbol soulResourceMap at 0x42b3d8, 160 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "soul.lmp";
static char text_1[] = "soul1.vif";
static char text_2[] = "soul1.tex";
static char text_3[] = "soul2.vif";
static char text_4[] = "soul2.tex";
static char text_5[] = "hitreact.anm";
static char text_6[] = "idle1.anm";
static char text_7[] = "idle2.anm";
static char text_8[] = "idle3.anm";
static char text_9[] = "run.anm";
static char text_10[] = "catch_so.anm";
static char text_11[] = "club_so.anm";
static char text_12[] = "flight_so.anm";
static char text_13[] = "idle_so.anm";
static char text_14[] = "shoot_so.anm";
static char text_15[] = "walk_so.anm";
static char text_16[] = "pelys_so.anm";
static char text_17[] = "killed.anm";
static char text_18[] = "free.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42b3d8[20] = {
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
