// Observed _vt$11SkillCharge at 0x450860, 144 bytes in .data (weak object in the image).
// 18 slots of 8 bytes: a zero adjustment word and a function word, the last
// function word being zero. The function words, in order, are the symbols
// listed in the slot table of docs/agents/w12-skill-a.md (17 distinct names).
// Each name is the sized function symbol at that address in the image; the
// bodies behind them are not in this unit, so every slot refers to the
// observed name and this unit adds no function definitions of its own.
// Inferred: the constructor of SkillCharge stores this table's address at object+72.
// That store is made by a different unit and is not exercised here.
// Unknown: dispatch through this table, and the role of the last (null) slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf11SkillCharge");
void vt_fn_1() asm("canCast__5SkillP8Creature");
void vt_fn_2() asm("cast__11SkillChargeiP8CreatureG6Point3s");
void vt_fn_3() asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
void vt_fn_4() asm("getRankDescription__11SkillChargePUsi");
void vt_fn_5() asm("getSkillPointCost__5Skilli");
void vt_fn_6() asm("getLevelRequirement__5Skilli");
void vt_fn_7() asm("getEnergyCost__11SkillChargei");
void vt_fn_8() asm("getIcon__11SkillCharge");
void vt_fn_9() asm("getRampDamage__5SkilliifiPiT5");
void vt_fn_10() asm("initRamps__11SkillCharge");
void vt_fn_11() asm("evaluateRampAtRank__5Skillii");
void vt_fn_12() asm("evaluateCreatureRampAtRank__5Skillii");
void vt_fn_13() asm("evaluateRampAtRank__5SkilliiP10GameObject");
void vt_fn_14() asm("evaluateRampAtRank__5SkilliiP8ListHead");
void vt_fn_15() asm("tweekRank__5Skillii");
void vt_fn_16() asm("energyCostRamp4__5Skillii");
VtSlot vt_skill_charge[18] asm("_vt$11SkillCharge") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, vt_fn_5},
    {0x0u, vt_fn_6},
    {0x0u, vt_fn_7},
    {0x0u, vt_fn_8},
    {0x0u, vt_fn_9},
    {0x0u, vt_fn_10},
    {0x0u, vt_fn_11},
    {0x0u, vt_fn_12},
    {0x0u, vt_fn_13},
    {0x0u, vt_fn_14},
    {0x0u, vt_fn_15},
    {0x0u, vt_fn_16},
    {0x0u, nullptr},
};
}
#if defined(__mips__)
static_assert(sizeof(vt_skill_charge) == 144, "table size is 144 bytes");
#endif
