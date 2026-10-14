// Observed _vt$5Spell at 0x0044e670, 48 bytes in .data (weak object in the image).
// 6 slots of 8 bytes: a zero adjustment word and a function word. The last slot is null.
// The 5 function words, in slot order, are the observed symbol names listed below; each
// name is the image symbol at that address. The bodies are not in this unit.
// Inferred: a constructor of this class stores this table address in the object (another unit).
// Unknown: dispatch order beyond the table itself, and the role of the null last slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_spell_fn_0() asm("__tf5Spell");
void vt_spell_fn_1() asm("_$_5Spell");
void vt_spell_fn_2() asm("run__5SpellG6Point3s");
void vt_spell_fn_3() asm("draw__5Spell");
void vt_spell_fn_4() asm("canQueue__5Spell");
VtSlot vt_spell[6] asm("_vt$5Spell") = {
    {0x0u, vt_spell_fn_0},
    {0x0u, vt_spell_fn_1},
    {0x0u, vt_spell_fn_2},
    {0x0u, vt_spell_fn_3},
    {0x0u, vt_spell_fn_4},
    {0x0u, nullptr},
};
}
