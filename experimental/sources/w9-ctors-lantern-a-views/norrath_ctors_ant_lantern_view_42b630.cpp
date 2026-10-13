// Observed .data symbol ghoulBlueResourceMap at 0x42b630, 232 bytes.
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
static char text_0[] = "ghoulblu.lmp";
static char text_1[] = "ghoul.vif";
static char text_2[] = "ghoul.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2.anm";
static char text_5[] = "attack3.anm";
static char text_6[] = "crawl.anm";
static char text_7[] = "crouch.anm";
static char text_8[] = "dead.anm";
static char text_9[] = "dead2.anm";
static char text_10[] = "dead3.anm";
static char text_11[] = "dead4.anm";
static char text_12[] = "dead5.anm";
static char text_13[] = "dead6.anm";
static char text_14[] = "dead7.anm";
static char text_15[] = "death.anm";
static char text_16[] = "hitreact.anm";
static char text_17[] = "hop_fwd.anm";
static char text_18[] = "hop_bk.anm";
static char text_19[] = "hop_lft.anm";
static char text_20[] = "hop_ryt.anm";
static char text_21[] = "idle1a.anm";
static char text_22[] = "idle1b.anm";
static char text_23[] = "idle1c.anm";
static char text_24[] = "idle1d.anm";
static char text_25[] = "run1.anm";
static char text_26[] = "spasm.anm";
static char text_27[] = "twitch.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42b630[29] = {
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
    {text_26, 0u},
    {text_27, 0u},
    {nullptr, 0u},
};
}
