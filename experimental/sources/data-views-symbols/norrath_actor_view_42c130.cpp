// Observed .data symbol smallSpiderResourceMap at 0x42c130, 88 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
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
static char text_3[] = "death1.anm";
static char text_4[] = "death2.anm";
static char text_5[] = "death3.anm";
static char text_6[] = "death4.anm";
static char text_7[] = "idle1.anm";
static char text_8[] = "idle2.anm";
static char text_9[] = "walk.anm";
extern "C" {
ResourceSlot norrath_actor_view_42c130[11] = {
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
    {nullptr, 0u},
};
}
