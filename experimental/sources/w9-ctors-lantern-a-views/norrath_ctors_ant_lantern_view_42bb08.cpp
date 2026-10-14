// Observed .data symbol cyclopsResourceMap at 0x42bb08, 432 bytes.
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
static char text_0[] = "cyclops.lmp";
static char text_1[] = "cyclops.vif";
static char text_2[] = "cyclops.tex";
static char text_3[] = "attack1.anm";
static char text_4[] = "attackfart.anm";
static char text_5[] = "attackleft.anm";
static char text_6[] = "attackright.anm";
static char text_7[] = "attackroar.anm";
static char text_8[] = "attacksand.anm";
static char text_9[] = "attackspit.anm";
static char text_10[] = "block.anm";
static char text_11[] = "hitreaction.anm";
static char text_12[] = "idle1.anm";
static char text_13[] = "idlepassive.anm";
static char text_14[] = "turn_ccw.anm";
static char text_15[] = "turn_cw.anm";
static char text_16[] = "walk_xy.anm";
static char text_17[] = "walkstop.anm";
static char text_18[] = "death.anm";
static char text_19[] = "projectile.vif";
static char text_20[] = "projectile.tex";
static char text_21[] = "projectile.anm";
static char text_22[] = "spel_cyclops_spit.anm";
static char text_23[] = "spel_cyclops_spit_hit.anm";
static char text_24[] = "spel_cyclops_fart.anm";
static char text_25[] = "spel_cyclops_sand.anm";
static char text_27[] = "sickcyc.lmp";
static char text_28[] = "sickcyc.vif";
static char text_29[] = "sickcyc.tex";
static char text_30[] = "attack1.anm";
static char text_31[] = "attackfart.anm";
static char text_32[] = "attackleft.anm";
static char text_33[] = "attackright.anm";
static char text_34[] = "attackroar.anm";
static char text_35[] = "attacksand.anm";
static char text_36[] = "attackspit.anm";
static char text_37[] = "block.anm";
static char text_38[] = "hitreaction.anm";
static char text_39[] = "idle1.anm";
static char text_40[] = "idlepassive.anm";
static char text_41[] = "turn_ccw.anm";
static char text_42[] = "turn_cw.anm";
static char text_43[] = "walk_xy.anm";
static char text_44[] = "walkstop.anm";
static char text_45[] = "death.anm";
static char text_46[] = "projectile.vif";
static char text_47[] = "projectile.tex";
static char text_48[] = "projectile.anm";
static char text_49[] = "spel_cyclops_spit.anm";
static char text_50[] = "spel_cyclops_spit_hit.anm";
static char text_51[] = "spel_cyclops_fart.anm";
static char text_52[] = "spel_cyclops_sand.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42bb08[54] = {
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
    {text_43, 0u},
    {text_44, 0u},
    {text_45, 0u},
    {text_46, 0u},
    {text_47, 0u},
    {text_48, 0u},
    {text_49, 0u},
    {text_50, 0u},
    {text_51, 0u},
    {text_52, 0u},
    {nullptr, 0u},
};
}
