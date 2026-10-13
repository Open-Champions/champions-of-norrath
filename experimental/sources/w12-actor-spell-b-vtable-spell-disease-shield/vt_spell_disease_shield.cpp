// Observed _vt$18SpellDiseaseShield at 0x450c20, 48 bytes in .data (weak object in the image).
// 6 slots of 8 bytes: a zero adjustment word and a function word, the last
// function word being zero. The function words, in order, are the symbols
// listed in the slot table of docs/agents/w12-actor-spell-b.md (5 distinct names).
// Each name is the sized function symbol at that address in the image; the
// bodies behind them are not in this unit, so every slot refers to the
// observed name and this unit adds no function definitions of its own.
// Inferred: the constructor of SpellDiseaseShield stores this table's address at object+72.
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
void vt_fn_0() asm("__tf18SpellDiseaseShield");
void vt_fn_1() asm("_$_18SpellDiseaseShield");
void vt_fn_2() asm("run__18SpellDiseaseShieldG6Point3s");
void vt_fn_3() asm("draw__5Spell");
void vt_fn_4() asm("canQueue__5Spell");
VtSlot vt_spell_disease_shield[6] asm("_vt$18SpellDiseaseShield") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, nullptr},
};
}
#if defined(__mips__)
static_assert(sizeof(vt_spell_disease_shield) == 48, "table size is 48 bytes");
#endif
