// Observed .data symbol cloudgiantResourceMap at 0x42ae60, 272 bytes.
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
static char text_0[] = "cloud_g.lmp";
static char text_1[] = "g1.vif";
static char text_2[] = "g1.tex";
static char text_3[] = "180atak.anm";
static char text_4[] = "atakwind1.anm";
static char text_5[] = "atakwind2.anm";
static char text_6[] = "boltconjure.anm";
static char text_7[] = "bolthrow.anm";
static char text_8[] = "boltsalm.anm";
static char text_9[] = "boltwalk.anm";
static char text_10[] = "death_a.anm";
static char text_11[] = "death_b.anm";
static char text_12[] = "hamtak1.anm";
static char text_13[] = "hamtak2.anm";
static char text_14[] = "hitleft.anm";
static char text_15[] = "hitright.anm";
static char text_16[] = "idle1.anm";
static char text_17[] = "idle2.anm";
static char text_18[] = "kick.anm";
static char text_19[] = "quake.anm";
static char text_20[] = "throw.anm";
static char text_21[] = "walk.anm";
static char text_22[] = "bolt.vif";
static char text_23[] = "bolt.tex";
static char text_24[] = "hammer.vif";
static char text_25[] = "hammer.tex";
static char text_26[] = "rock.vif";
static char text_27[] = "rock.tex";
static char text_28[] = "spel_cloudg_bolt_conjure.anm";
static char text_29[] = "spel_cloudg_bolt_hit.anm";
static char text_30[] = "spel_cloudg_bolt_slam.anm";
static char text_31[] = "spel_cloudg_throw.anm";
static char text_32[] = "spel_cloudg_wind.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42ae60[34] = {
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
    {text_28, 0u},
    {text_29, 0u},
    {text_30, 0u},
    {text_31, 0u},
    {text_32, 0u},
    {nullptr, 0u},
};
}
