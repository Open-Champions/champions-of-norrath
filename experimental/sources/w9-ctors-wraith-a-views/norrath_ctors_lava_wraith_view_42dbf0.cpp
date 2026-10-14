// Observed .data symbol skelightResourceMap at 0x42dbf0, 80 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "skele_b.lmp";
static char text_1[] = "skeleb.vif";
static char text_2[] = "skeleb.tex";
static char text_3[] = "atak1.anm";
static char text_4[] = "atak2.anm";
static char text_5[] = "death.anm";
static char text_6[] = "idle.anm";
static char text_7[] = "rise.anm";
static char text_8[] = "walk.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42dbf0[10] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {nullptr, 0u},
};
}
