// Observed .data symbol rondoResourceMap at 0x42d048, 120 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "rondo.lmp";
static char text_1[] = "rondo.vif";
static char text_2[] = "rondo.tex";
static char text_3[] = "attack.anm";
static char text_4[] = "hitreaction.anm";
static char text_5[] = "idle1.anm";
static char text_6[] = "idle2.anm";
static char text_7[] = "idle3.anm";
static char text_8[] = "idle4.anm";
static char text_9[] = "run.anm";
static char text_10[] = "talk1.anm";
static char text_11[] = "talk2.anm";
static char text_12[] = "talk3.anm";
static char text_13[] = "walk.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d048[15] = {
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
    {nullptr, 0u},
};
}
