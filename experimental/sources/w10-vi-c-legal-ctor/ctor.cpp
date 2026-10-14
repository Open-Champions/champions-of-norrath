// Observed: __10VIWndLegal at 0x010bf8e0 is 48 bytes. With the object as this it calls
// __5VIWnd(this), then stores the address of _vt$10VIWndLegal (0x0120aa38) at this+32, and
// returns nothing.
// Inferred: this is the constructor of a VIWnd-derived class whose only own step is installing
// its virtual table. Unknown: any other member of the class; no other field is written here.
extern "C" {
void __5VIWnd(void* self);
extern unsigned char _vt$10VIWndLegal[];

void __10VIWndLegal(void* self) {
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$10VIWndLegal));
}
}
