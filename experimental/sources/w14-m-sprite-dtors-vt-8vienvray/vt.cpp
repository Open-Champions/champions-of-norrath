// Observed _vt$8VIEnvRay at 0x012267d8, 24 bytes (3 slots of 8 bytes) in the image data.
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
void vt_fn_0() asm("__tf8VIEnvRay");
void vt_fn_1() asm("Collide__8VIEnvRayUifR7VIVect3");
VtSlot vt_8VIEnvRay[3] asm("_vt$8VIEnvRay") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, nullptr},
};
}
