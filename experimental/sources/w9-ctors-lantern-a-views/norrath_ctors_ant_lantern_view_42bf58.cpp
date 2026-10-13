// Observed .data symbol antQueenResourceMap at 0x42bf58, 208 bytes.
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
static char text_0[] = "antqueen.lmp";
static char text_1[] = "antqueen.vif";
static char text_2[] = "antqueen.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2_center.anm";
static char text_5[] = "attack2_left.anm";
static char text_6[] = "attack2_right.anm";
static char text_7[] = "fly.anm";
static char text_8[] = "fly_left.anm";
static char text_9[] = "fly_right.anm";
static char text_10[] = "hit_back.anm";
static char text_11[] = "hit_front.anm";
static char text_12[] = "hit_left.anm";
static char text_13[] = "hit_right.anm";
static char text_14[] = "hover.anm";
static char text_15[] = "idle_a.anm";
static char text_16[] = "idle_b.anm";
static char text_17[] = "idle_c.anm";
static char text_18[] = "landing.anm";
static char text_19[] = "sting.anm";
static char text_20[] = "takeoff.anm";
static char text_21[] = "walk.anm";
static char text_22[] = "walk_left.anm";
static char text_23[] = "walk_right.anm";
static char text_24[] = "death.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42bf58[26] = {
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
    {text_22, 0u},
    {text_23, 0u},
    {text_24, 0u},
    {nullptr, 0u},
};
}
