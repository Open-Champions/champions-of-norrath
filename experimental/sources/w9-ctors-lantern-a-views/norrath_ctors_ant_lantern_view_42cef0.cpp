// Observed .data symbol goblinResourceMap at 0x42cef0, 216 bytes.
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
static char text_0[] = "goblin.lmp";
static char text_1[] = "goblin.vif";
static char text_2[] = "goblin.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attack2.anm";
static char text_5[] = "attack_xy.anm";
static char text_6[] = "bowattak.anm";
static char text_7[] = "bowidle.anm";
static char text_8[] = "death.anm";
static char text_9[] = "hit1.anm";
static char text_10[] = "hit2.anm";
static char text_11[] = "hit3.anm";
static char text_12[] = "idlea.anm";
static char text_13[] = "idleb.anm";
static char text_14[] = "run.anm";
static char text_15[] = "shmnidle.anm";
static char text_16[] = "shmnspell.anm";
static char text_17[] = "walk.anm";
static char text_18[] = "block.anm";
static char text_19[] = "blockhit.anm";
static char text_20[] = "knokdown.anm";
static char text_21[] = "knokinto.anm";
static char text_22[] = "knokdwn_l1.anm";
static char text_23[] = "knokdwn_l2.anm";
static char text_24[] = "knokdwn_r1.anm";
static char text_25[] = "knokdwn_r2.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42cef0[27] = {
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
