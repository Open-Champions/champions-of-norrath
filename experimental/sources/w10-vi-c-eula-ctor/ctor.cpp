// Observed: __9VIWndEula at 0x010b56d8 is 68 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$9VIWndEula (0x01208a78) at this+32, runs
// __10VIUIWindow on this+40 and __12VIWndMessage on this+408, then stores 0 at this+1552.
// Nothing is returned.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
void __12VIWndMessage(void* self);
extern unsigned char _vt$9VIWndEula[];

void __9VIWndEula(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEula));
    __10VIUIWindow(bytes + 40);
    __12VIWndMessage(bytes + 408);
    *reinterpret_cast<unsigned*>(bytes + 1552) = 0;
}
}
