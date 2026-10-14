// Observed: __16VIWndReadMessage at 0x010d1420 is 68 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$16VIWndReadMessage (0x0120c968) at this+32, calls
// __10VIUIWindow(this+40), stores 0 at this+408 and stores 1 at this+16804. Nothing is returned.
// Inferred: one embedded UI window at +40. Unknown: what the words at +408 and +16804 hold.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
extern unsigned char _vt$16VIWndReadMessage[];

void __16VIWndReadMessage(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16VIWndReadMessage));
    __10VIUIWindow(bytes + 40);
    *reinterpret_cast<unsigned*>(bytes + 408) = 0;
    *reinterpret_cast<unsigned*>(bytes + 16804) = 1;
}
}
