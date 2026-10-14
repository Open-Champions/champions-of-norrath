// Observed: _vt$5VIWnd at 0x0120ab60 is an eight-entry table of adjustment
// words and function pointers. Its entries name the base type function,
// destructor, OnDraw, OnDrawSelf, OnKeyUp, OnKeyDown and OnMessage; the eighth
// function pointer is null. Each adjustment word is zero. The address order
// was checked against the pinned image's symbols. Unknown: slot 0's runtime use.
struct Slot { unsigned adjustment; void (*function)(); };
#if defined(__mips__)
static_assert(sizeof(Slot) == 8, "EE vtable slot must occupy eight bytes");
#endif
extern "C" {
void slot_fn_0() __asm__("__tf5VIWnd");
void slot_fn_1() __asm__("_$_5VIWnd");
void slot_fn_2() __asm__("OnDraw__5VIWnd");
void slot_fn_3() __asm__("OnDrawSelf__5VIWnd");
void slot_fn_4() __asm__("OnKeyUp__5VIWndi");
void slot_fn_5() __asm__("OnKeyDown__5VIWndi");
void slot_fn_6() __asm__("OnMessage__5VIWndP5VIWndiii");

Slot vtable[8] __asm__("_vt$5VIWnd") = {
    {0u, slot_fn_0}, {0u, slot_fn_1}, {0u, slot_fn_2}, {0u, slot_fn_3},
    {0u, slot_fn_4}, {0u, slot_fn_5}, {0u, slot_fn_6}, {0u, nullptr},
};
}
