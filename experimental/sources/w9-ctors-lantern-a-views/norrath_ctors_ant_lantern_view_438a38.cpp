// Observed .data symbol cthuluResourceMap at 0x438a38, 216 bytes.
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
static char text_0[] = "cthulu.lmp";
static char text_1[] = "cthulu.vif";
static char text_2[] = "cthulu.tex";
static char text_3[] = "atak_1.anm";
static char text_4[] = "atak_2.anm";
static char text_5[] = "atak_3.anm";
static char text_6[] = "atak_4.anm";
static char text_7[] = "crawfast.anm";
static char text_8[] = "crawl.anm";
static char text_9[] = "idle.anm";
static char text_10[] = "run.anm";
static char text_11[] = "spit.anm";
static char text_12[] = "test1.anm";
static char text_13[] = "up.anm";
static char text_14[] = "death.anm";
static char text_15[] = "hitreact.anm";
static char text_16[] = "idlecalm.anm";
static char text_17[] = "roar.anm";
static char text_18[] = "upatak.anm";
static char text_19[] = "upidle.anm";
static char text_20[] = "wakeup.anm";
static char text_21[] = "projectile.vif";
static char text_22[] = "projectile.tex";
static char text_23[] = "projectile.anm";
static char text_24[] = "spel_cthulu_spit.anm";
static char text_25[] = "spel_cthulu_spit_hit.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_438a38[27] = {
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
    {text_25, 0u},
    {nullptr, 0u},
};
}
