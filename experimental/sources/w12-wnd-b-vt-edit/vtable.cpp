// Observed: _vt$9VIWndEdit at 0x01208040 is an eight-entry table of zero
// adjustments and function pointers. The seven nonzero pointers are the edit
// type function, destructor, base OnDraw, and the Edit draw/key/message
// overrides. Slot 7 has a null function pointer. Runtime use of slot 0 is
// unknown.
struct Slot { unsigned adjustment; void (*function)(); };
#if defined(__mips__)
static_assert(sizeof(Slot) == 8, "EE vtable slot must occupy eight bytes");
#endif
extern "C" {
void slot_fn_0() __asm__("__tf9VIWndEdit");
void slot_fn_1() __asm__("_$_9VIWndEdit");
void slot_fn_2() __asm__("OnDraw__5VIWnd");
void slot_fn_3() __asm__("OnDrawSelf__9VIWndEdit");
void slot_fn_4() __asm__("OnKeyUp__9VIWndEditi");
void slot_fn_5() __asm__("OnKeyDown__9VIWndEditi");
void slot_fn_6() __asm__("OnMessage__9VIWndEditP5VIWndiii");

Slot vtable[8] __asm__("_vt$9VIWndEdit") = {
    {0u, slot_fn_0}, {0u, slot_fn_1}, {0u, slot_fn_2}, {0u, slot_fn_3},
    {0u, slot_fn_4}, {0u, slot_fn_5}, {0u, slot_fn_6}, {0u, nullptr},
};
}
