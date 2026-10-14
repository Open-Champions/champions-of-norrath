// Observed: __12VIWndPatcher at 0x010ccab0 is 84 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$12VIWndPatcher (0x0120c830) at this+32, runs
// __10VIUIWindow on this+40, this+408 and this+776, __12VIWndMessage on this+1176, and
// tail-calls __16VIWndReadMessage on this+2312. Nothing is returned.
// Unknown: what each embedded window is used for.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
void __12VIWndMessage(void* self);
void __16VIWndReadMessage(void* self);
extern unsigned char _vt$12VIWndPatcher[];

void __12VIWndPatcher(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndPatcher));
    __10VIUIWindow(bytes + 40);
    __10VIUIWindow(bytes + 408);
    __10VIUIWindow(bytes + 776);
    __12VIWndMessage(bytes + 1176);
    __16VIWndReadMessage(bytes + 2312);
}
}
