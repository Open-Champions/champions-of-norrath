// Observed _vt$13VIActorFilter at 0x012018e0, 24 bytes (3 slots of 8 bytes) in the image data.
// Each slot is a zero adjustment word and a function word. The function words, in slot order, are the
// image symbols named below; the last slot is null. The bodies of these functions are not in this unit.
// Inferred: the first function word is the type-info function of the class, then the virtual functions in
// the order the image lists them. Unknown: the owner of each named function beyond the names in this unit.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf13VIActorFilter");
void vt_fn_1() asm("FilterActor__13VIActorFilterR7VIScenei");
VtSlot vt_13VIActorFilter[3] asm("_vt$13VIActorFilter") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, nullptr},
};
}
