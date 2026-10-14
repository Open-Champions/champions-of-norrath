// Observed _vt$18SpellHammerOfWrath at 0x00452a20, 48 bytes in .data (weak object in the image).
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
void vt_spellhammerofwrath_fn_0() asm("__tf18SpellHammerOfWrath");
void vt_spellhammerofwrath_fn_1() asm("_$_18SpellHammerOfWrath");
void vt_spellhammerofwrath_fn_2() asm("run__18SpellHammerOfWrathG6Point3s");
void vt_spellhammerofwrath_fn_3() asm("draw__5Spell");
void vt_spellhammerofwrath_fn_4() asm("canQueue__5Spell");
VtSlot vt_spellhammerofwrath[6] asm("_vt$18SpellHammerOfWrath") = {
    {0x0u, vt_spellhammerofwrath_fn_0},
    {0x0u, vt_spellhammerofwrath_fn_1},
    {0x0u, vt_spellhammerofwrath_fn_2},
    {0x0u, vt_spellhammerofwrath_fn_3},
    {0x0u, vt_spellhammerofwrath_fn_4},
    {0x0u, nullptr},
};
}
