// Observed .data symbol spiderQueenResourceMap at 0x42c228, 224 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "spidboss.lmp";
static char text_1[] = "spiderboss.vif";
static char text_2[] = "spiderboss.tex";
static char text_3[] = "aggression.anm";
static char text_4[] = "bite.anm";
static char text_5[] = "death.anm";
static char text_6[] = "grabnbite.anm";
static char text_7[] = "hit_back.anm";
static char text_8[] = "hit_front.anm";
static char text_9[] = "hit_left.anm";
static char text_10[] = "hit_right.anm";
static char text_11[] = "idle.anm";
static char text_12[] = "jump.anm";
static char text_13[] = "slashcenter.anm";
static char text_14[] = "slashleft.anm";
static char text_15[] = "slashright.anm";
static char text_16[] = "spit.anm";
static char text_17[] = "tantrum.anm";
static char text_18[] = "walk.anm";
static char text_19[] = "webprep.anm";
static char text_20[] = "webpull.anm";
static char text_21[] = "webshot.anm";
static char text_22[] = "projectile.vif";
static char text_23[] = "projectile.tex";
static char text_24[] = "projectile.anm";
static char text_25[] = "spel_spidboss_spit.anm";
static char text_26[] = "spel_spidboss_spit_hit.anm";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42c228[28] = {
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
    {nullptr, 0u},
};
}
