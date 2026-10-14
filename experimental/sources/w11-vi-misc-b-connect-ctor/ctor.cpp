// Observed: __12VIWndConnect at 0x010a2a00 is 120 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$12VIWndConnect (0x01207db8) at this+32, calls
// __12VIWndMessage(this+40), calls __10VIUIWindow(this+1176), calls __10VIUIWindow(this+1544),
// stores -1 at this+1952, stores 0 at this+1964, stores -1 at this+1928, stores -1 at this+1932,
// stores -1 at this+1936, stores -1 at this+1940, stores -1 at this+1944, stores -1 at this+1948,
// stores 0 at this+1912, stores 0 at this+1956 and stores 0 at this+1960. Nothing is returned.
// Inferred: a message window at +40, two UI windows at +1176 and +1544, a child word at +1912 and six
// words at +1928..+1948 plus one at +1952 that start at -1. Unknown: what any of these words mean.
extern "C" {
void __5VIWnd(void* self);
void __12VIWndMessage(void* self);
void __10VIUIWindow(void* self);
extern unsigned char _vt$12VIWndConnect[];

void __12VIWndConnect(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndConnect));
    __12VIWndMessage(bytes + 40);
    __10VIUIWindow(bytes + 1176);
    __10VIUIWindow(bytes + 1544);
    *reinterpret_cast<unsigned*>(bytes + 1952) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1964) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1928) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1932) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1936) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1940) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1944) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1948) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 1912) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1956) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1960) = 0;
}
}
