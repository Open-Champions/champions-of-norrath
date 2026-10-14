// Observed: __11VIWndSplash at 0x010d6e18 is 76 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$11VIWndSplash (0x0120f640) at this+32, runs
// __10VIUIWindow on this+40, __12VIWndConnect on this+416, __12VIWndOptions on this+2408, and
// tail-calls __12VIWndMessage on this+4776. Nothing is returned.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
void __12VIWndConnect(void* self);
void __12VIWndOptions(void* self);
void __12VIWndMessage(void* self);
extern unsigned char _vt$11VIWndSplash[];

void __11VIWndSplash(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$11VIWndSplash));
    __10VIUIWindow(bytes + 40);
    __12VIWndConnect(bytes + 416);
    __12VIWndOptions(bytes + 2408);
    __12VIWndMessage(bytes + 4776);
}
}
