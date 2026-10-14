// Observed _vt$9bad_alloc at 0x00453950, 32 bytes (4 slots of 8 bytes) in the image data.
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
void vt_fn_0() asm("__tf9bad_alloc");
void vt_fn_1() asm("_$_9bad_alloc");
void vt_fn_2() asm("what__C9bad_alloc");
VtSlot vt_9bad_alloc[4] asm("_vt$9bad_alloc") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, nullptr},
};
}
