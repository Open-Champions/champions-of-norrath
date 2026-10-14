// Observed .data symbol _vt$18SkillPoisonWeapons at 0x44d768, 144 bytes (weak in the image). It is a table of 18 slots of 8 bytes:
// an adjustment word (zero in every slot) and a function address. Slot 17 is null. The function names are the sized ELF
// function symbols at the addresses stored in the image, kept as assembler names. Slots whose
// name carries the class name SkillPoisonWeapons are 0, 10; the other slots name routines of
// other classes. What the slots do is only what their names say; none of the slot bodies is
// defined in this unit. Unknown: how the table is selected and called.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf18SkillPoisonWeapons");
void slot_fn_1() asm("canCast__5SkillP8Creature");
void slot_fn_2() asm("cast__5SkilliP8CreatureG6Point3s");
void slot_fn_3() asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
void slot_fn_4() asm("getRankDescription__5SkillPUsi");
void slot_fn_5() asm("getSkillPointCost__5Skilli");
void slot_fn_6() asm("getLevelRequirement__5Skilli");
void slot_fn_7() asm("getEnergyCost__5Skilli");
void slot_fn_8() asm("getIcon__5Skill");
void slot_fn_9() asm("getRampDamage__5SkilliifiPiT5");
void slot_fn_10() asm("initRamps__18SkillPoisonWeapons");
void slot_fn_11() asm("evaluateRampAtRank__5Skillii");
void slot_fn_12() asm("evaluateCreatureRampAtRank__5Skillii");
void slot_fn_13() asm("evaluateRampAtRank__5SkilliiP10GameObject");
void slot_fn_14() asm("evaluateRampAtRank__5SkilliiP8ListHead");
void slot_fn_15() asm("tweekRank__5Skillii");
void slot_fn_16() asm("energyCostRamp4__5Skillii");
VirtualSlot _vt_18SkillPoisonWeapons[18] asm("_vt$18SkillPoisonWeapons") = {
    {0x0u, slot_fn_0},
    {0x0u, slot_fn_1},
    {0x0u, slot_fn_2},
    {0x0u, slot_fn_3},
    {0x0u, slot_fn_4},
    {0x0u, slot_fn_5},
    {0x0u, slot_fn_6},
    {0x0u, slot_fn_7},
    {0x0u, slot_fn_8},
    {0x0u, slot_fn_9},
    {0x0u, slot_fn_10},
    {0x0u, slot_fn_11},
    {0x0u, slot_fn_12},
    {0x0u, slot_fn_13},
    {0x0u, slot_fn_14},
    {0x0u, slot_fn_15},
    {0x0u, slot_fn_16},
    {0x0u, nullptr},
};
}
