// Observed .data symbol _vt$22SpellPlanarSteelEffect at 0x453720, 48 bytes (weak in the image). It is a table of 6 slots of 8 bytes:
// an adjustment word (zero in every slot) and a function address. Slot 5 is null. The function names are the sized ELF
// function symbols at the addresses stored in the image, kept as assembler names. Slots whose
// name carries the class name SpellPlanarSteelEffect are 0, 1, 2; the other slots name routines of
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
void slot_fn_0() asm("__tf22SpellPlanarSteelEffect");
void slot_fn_1() asm("_$_22SpellPlanarSteelEffect");
void slot_fn_2() asm("run__22SpellPlanarSteelEffectG6Point3s");
void slot_fn_3() asm("draw__5Spell");
void slot_fn_4() asm("canQueue__5Spell");
VirtualSlot _vt_22SpellPlanarSteelEffect[6] asm("_vt$22SpellPlanarSteelEffect") = {
    {0x0u, slot_fn_0},
    {0x0u, slot_fn_1},
    {0x0u, slot_fn_2},
    {0x0u, slot_fn_3},
    {0x0u, slot_fn_4},
    {0x0u, nullptr},
};
}
