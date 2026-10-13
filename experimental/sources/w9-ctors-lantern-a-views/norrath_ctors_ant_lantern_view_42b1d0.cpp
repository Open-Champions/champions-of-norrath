// Observed .data symbol badgerResourceMap at 0x42b1d0, 352 bytes.
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
static char text_0[] = "badger.lmp";
static char text_1[] = "gobdog.vif";
static char text_2[] = "gobdog.tex";
static char text_3[] = "gobdog_b.tex";
static char text_4[] = "atak_front.anm";
static char text_5[] = "atak_left.anm";
static char text_6[] = "atak_mid.anm";
static char text_7[] = "atak_right.anm";
static char text_8[] = "backup.anm";
static char text_9[] = "charge.anm";
static char text_10[] = "chargetak.anm";
static char text_11[] = "death1.anm";
static char text_12[] = "death2.anm";
static char text_13[] = "hit2front.anm";
static char text_14[] = "hit2left.anm";
static char text_15[] = "hit2right.anm";
static char text_16[] = "idle1.anm";
static char text_17[] = "roar.anm";
static char text_18[] = "walk.anm";
static char text_19[] = "walk_trn_lft.anm";
static char text_20[] = "walk_trn_ryt.anm";
static char text_22[] = "snobadge.lmp";
static char text_23[] = "gobdog.vif";
static char text_24[] = "gobdog.tex";
static char text_25[] = "gobdog_b.tex";
static char text_26[] = "atak_front.anm";
static char text_27[] = "atak_left.anm";
static char text_28[] = "atak_mid.anm";
static char text_29[] = "atak_right.anm";
static char text_30[] = "backup.anm";
static char text_31[] = "charge.anm";
static char text_32[] = "chargetak.anm";
static char text_33[] = "death1.anm";
static char text_34[] = "death2.anm";
static char text_35[] = "hit2front.anm";
static char text_36[] = "hit2left.anm";
static char text_37[] = "hit2right.anm";
static char text_38[] = "idle1.anm";
static char text_39[] = "roar.anm";
static char text_40[] = "walk.anm";
static char text_41[] = "walk_trn_lft.anm";
static char text_42[] = "walk_trn_ryt.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42b1d0[44] = {
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
    {text_33, 0u},
    {text_34, 0u},
    {text_35, 0u},
    {text_36, 0u},
    {text_37, 0u},
    {text_38, 0u},
    {text_39, 0u},
    {text_40, 0u},
    {text_41, 0u},
    {text_42, 0u},
    {nullptr, 0u},
};
}
