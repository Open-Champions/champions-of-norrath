// Observed .data symbol _vt$9type_info at 0x453850, 24 bytes.
// Each slot is the adjustment word followed by a function pointer.
// A zero function word is a null slot. Names are the sized ELF
// function symbols at those addresses. Unknown: dispatch and overrides.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf9type_info");
void slot_fn_1() asm("_$_9type_info");
VirtualSlot norrath_rtti_entities_a_view_453850[3] = {
    {0x0u, slot_fn_0},
    {0x0u, slot_fn_1},
    {0x0u, nullptr},
};
}
