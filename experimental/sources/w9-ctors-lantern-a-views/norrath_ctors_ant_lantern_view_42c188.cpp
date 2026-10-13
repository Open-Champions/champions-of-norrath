// Observed .data symbol blackWidowResourceMap at 0x42c188, 160 bytes.
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
static char text_0[] = "blkwidow.lmp";
static char text_1[] = "spider.vif";
static char text_2[] = "spider.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2.anm";
static char text_5[] = "attack2demo.anm";
static char text_6[] = "attack3.anm";
static char text_7[] = "capture.anm";
static char text_8[] = "death1.anm";
static char text_9[] = "death2.anm";
static char text_10[] = "death3.anm";
static char text_11[] = "death4.anm";
static char text_12[] = "eat.anm";
static char text_13[] = "hit.anm";
static char text_14[] = "idle1.anm";
static char text_15[] = "idle2.anm";
static char text_16[] = "webshot.anm";
static char text_17[] = "walk.anm";
static char text_18[] = "climbup.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c188[20] = {
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
