// Observed .data symbol vampireResourceMap at 0x42d650, 176 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "vampire.lmp";
static char text_1[] = "nasvara.vif";
static char text_2[] = "nasvara.tex";
static char text_3[] = "attak1.anm";
static char text_4[] = "attak2.anm";
static char text_5[] = "attak3.anm";
static char text_6[] = "crawl.anm";
static char text_7[] = "death1.anm";
static char text_8[] = "death2.anm";
static char text_9[] = "death3.anm";
static char text_10[] = "evad3l.anm";
static char text_11[] = "evad3r.anm";
static char text_12[] = "evade.anm";
static char text_13[] = "hitreact.anm";
static char text_14[] = "hitreact2.anm";
static char text_15[] = "idle1.anm";
static char text_16[] = "idle2.anm";
static char text_17[] = "idle3.anm";
static char text_18[] = "idle4.anm";
static char text_19[] = "jump.anm";
static char text_20[] = "tocrawl.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d650[22] = {
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
    {nullptr, 0u},
};
}
