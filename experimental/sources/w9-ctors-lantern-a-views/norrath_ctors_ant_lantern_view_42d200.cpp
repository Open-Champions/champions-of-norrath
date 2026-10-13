// Observed .data symbol arenabeastResourceMap at 0x42d200, 176 bytes.
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
static char text_0[] = "a_beast.lmp";
static char text_1[] = "arenabeast.vif";
static char text_2[] = "arenabeast.tex";
static char text_3[] = "atak_left.anm";
static char text_4[] = "atak_ryt.anm";
static char text_5[] = "atakclo1.anm";
static char text_6[] = "atakclo2.anm";
static char text_7[] = "atakclo22.anm";
static char text_8[] = "atakclo3.anm";
static char text_9[] = "atakkill.anm";
static char text_10[] = "ataklng1.anm";
static char text_11[] = "ataklng2.anm";
static char text_12[] = "ataklng3.anm";
static char text_13[] = "death.anm";
static char text_14[] = "hitreact1.anm";
static char text_15[] = "hitreact2.anm";
static char text_16[] = "hitreact3.anm";
static char text_17[] = "idle1.anm";
static char text_18[] = "roar.anm";
static char text_19[] = "taunt.anm";
static char text_20[] = "walk.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42d200[22] = {
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
