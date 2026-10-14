// Observed .data symbol vampirelordResourceMap at 0x42d700, 208 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "vamplord.lmp";
static char text_1[] = "vamplord.vif";
static char text_2[] = "vamplord.tex";
static char text_3[] = "block.anm";
static char text_4[] = "flyup.anm";
static char text_5[] = "idle1.anm";
static char text_6[] = "idle2.anm";
static char text_7[] = "idleswd.anm";
static char text_8[] = "rushatak.anm";
static char text_9[] = "skydive.anm";
static char text_10[] = "swdbhit.anm";
static char text_11[] = "swdblock.anm";
static char text_12[] = "swdrush.anm";
static char text_13[] = "swdtak1.anm";
static char text_14[] = "swdtak2.anm";
static char text_15[] = "swdtak3.anm";
static char text_16[] = "swdwalk.anm";
static char text_17[] = "talking.anm";
static char text_18[] = "tornado.anm";
static char text_19[] = "walk.anm";
static char text_20[] = "stabfinish.anm";
static char text_21[] = "sword.vif";
static char text_22[] = "sword.tex";
static char text_23[] = "spel_vamplord_chant.anm";
static char text_24[] = "spel_vamplord_switch.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42d700[26] = {
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
    {nullptr, 0u},
};
}
