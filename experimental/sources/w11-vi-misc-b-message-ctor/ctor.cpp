// Observed: __12VIWndMessage at 0x010c13b0 is 68 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$12VIWndMessage (0x0120aac8) at this+32, calls
// __10VIUIWindow(this+40), stores 0 at this+988 and stores 0 at this+992. Nothing is returned.
// Inferred: one embedded UI window at +40 and two words at +988 and +992 (initialized empty).
// Unknown: what the two words hold.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
extern unsigned char _vt$12VIWndMessage[];

void __12VIWndMessage(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndMessage));
    __10VIUIWindow(bytes + 40);
    *reinterpret_cast<unsigned*>(bytes + 988) = 0;
    *reinterpret_cast<unsigned*>(bytes + 992) = 0;
}
}
