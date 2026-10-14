// Observed _vt$10bad_typeid at 0x00453810, 32 bytes (4 slots of 8 bytes) in the image data.
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
void vt_fn_0() asm("__tf10bad_typeid");
void vt_fn_1() asm("_$_10bad_typeid");
void vt_fn_2() asm("what__C9exception");
VtSlot vt_10bad_typeid[4] asm("_vt$10bad_typeid") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, nullptr},
};
}
