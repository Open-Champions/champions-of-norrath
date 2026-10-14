// Observed: _vt$9VIWndDnas at 0x01207e70 is 64 bytes of writable data in the pinned SLUS-20565
// image. It holds eight slots of two words each: a zero adjustment word followed by a function
// address. The function words, in order, are: 0 __tf9VIWndDnas, 1 _$_9VIWndDnas, 2
// OnDraw__5VIWnd, 3 OnDrawSelf__9VIWndDnas, 4 OnKeyUp__9VIWndDnasi, 5 OnKeyDown__9VIWndDnasi, 6
// OnMessage__9VIWndDnasP5VIWndiii; slot 7 has a zero function word. The first four function words
// are at offsets 4, 12, 20 and 28; the destructor slot (+8) is the one the embedding destructors
// call with a delete flag, and the entry at +48 is the OnMessage slot that SetFocus calls.
// Inferred: the table layout follows the other class tables (adjustment word, then pointer).
// Unknown: the meaning of slot 0, and whether the zero slot 7 is a terminator or an unused entry.
// The bodies behind these names are not defined in this unit; they stay imports. Nothing here ran
// on the EE; dispatch through this table is not exercised.
struct Slot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(Slot) == 8, "a slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() __asm__("__tf9VIWndDnas");
void slot_fn_1() __asm__("_$_9VIWndDnas");
void slot_fn_2() __asm__("OnDraw__5VIWnd");
void slot_fn_3() __asm__("OnDrawSelf__9VIWndDnas");
void slot_fn_4() __asm__("OnKeyUp__9VIWndDnasi");
void slot_fn_5() __asm__("OnKeyDown__9VIWndDnasi");
void slot_fn_6() __asm__("OnMessage__9VIWndDnasP5VIWndiii");

Slot vtable[8] __asm__("_vt$9VIWndDnas") = {
    {0u, slot_fn_0},
    {0u, slot_fn_1},
    {0u, slot_fn_2},
    {0u, slot_fn_3},
    {0u, slot_fn_4},
    {0u, slot_fn_5},
    {0u, slot_fn_6},
    {0u, nullptr},
};
}
