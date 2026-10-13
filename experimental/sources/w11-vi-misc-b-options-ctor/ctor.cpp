// Observed: __12VIWndOptions at 0x010c61a8 is 92 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$12VIWndOptions (0x0120bd88) at this+32, calls
// __10VIUIWindow(this+40), calls __10VIUIWindow(this+408), calls __10VIUIWindow(this+776), calls
// __13VIBattleMusic(this+1200), calls __12VIWndMessage(this+1232) and stores 0 at this+1196.
// Nothing is returned.
// Inferred: three embedded UI windows (+40, +408, +776), a VIBattleMusic at +1200, a message window at +1232
// and a child word at +1196 (the destructor and Free remove it from the window when nonzero).
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
void __13VIBattleMusic(void* self);
void __12VIWndMessage(void* self);
extern unsigned char _vt$12VIWndOptions[];

void __12VIWndOptions(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndOptions));
    __10VIUIWindow(bytes + 40);
    __10VIUIWindow(bytes + 408);
    __10VIUIWindow(bytes + 776);
    __13VIBattleMusic(bytes + 1200);
    __12VIWndMessage(bytes + 1232);
    *reinterpret_cast<unsigned*>(bytes + 1196) = 0;
}
}
