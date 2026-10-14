// Observed .data symbol mummykingResourceMap at 0x42d480, 168 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "mumyking.lmp";
static char text_1[] = "mormaz.vif";
static char text_2[] = "mormaz.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2.anm";
static char text_5[] = "death.anm";
static char text_6[] = "giveheart.anm";
static char text_7[] = "hitreact.anm";
static char text_8[] = "idle1.anm";
static char text_9[] = "idle2.anm";
static char text_10[] = "sh_launch.anm";
static char text_11[] = "shield.anm";
static char text_12[] = "spinsaw.anm";
static char text_13[] = "walk.anm";
static char text_14[] = "staff.vif";
static char text_15[] = "staff.tex";
static char text_16[] = "spel_mumyking_snake.anm";
static char text_17[] = "spel_mumyking_snake_hit.anm";
static char text_18[] = "spel_mumyking_summon.anm";
static char text_19[] = "spel_mumyking_teleport.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d480[21] = {
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
    {nullptr, 0u},
};
}
