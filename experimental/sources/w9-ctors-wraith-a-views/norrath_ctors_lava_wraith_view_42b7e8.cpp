// Observed .data symbol seamonsterResourceMap at 0x42b7e8, 136 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "crusher.lmp";
static char text_1[] = "crusher.vif";
static char text_2[] = "crusher.tex";
static char text_3[] = "block.anm";
static char text_4[] = "crush.anm";
static char text_5[] = "csh_atak1.anm";
static char text_6[] = "csh_atak2.anm";
static char text_7[] = "csh_idle1.anm";
static char text_8[] = "csh_idle2.anm";
static char text_9[] = "death.anm";
static char text_10[] = "hitreact.anm";
static char text_11[] = "run.anm";
static char text_12[] = "spawn.anm";
static char text_13[] = "stepleft.anm";
static char text_14[] = "stepright.anm";
static char text_15[] = "walk.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42b7e8[17] = {
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
    {nullptr, 0u},
};
}
