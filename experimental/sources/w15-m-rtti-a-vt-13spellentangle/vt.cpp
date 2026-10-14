// Observed _vt$13SpellEntangle at 0x004534e0, 48 bytes (6 slots of 8 bytes) in the image data.
// Each slot is a zero adjustment word and a function word. The function words, in slot order, are the
// image symbols named below; a null function word is written as nullptr. The bodies of these functions
// are not in this unit, so every slot refers to the observed name and this unit adds no definitions.
// Inferred: the first slot is the type-info function of the class and the second its destructor, as in
// the other tables of this kind. Unknown: the owner of each named function beyond the name itself.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf13SpellEntangle");
void vt_fn_1() asm("_$_13SpellEntangle");
void vt_fn_2() asm("run__13SpellEntangleG6Point3s");
void vt_fn_3() asm("draw__5Spell");
void vt_fn_4() asm("canQueue__5Spell");
VtSlot vt_13SpellEntangle[6] asm("_vt$13SpellEntangle") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, nullptr},
};
}
