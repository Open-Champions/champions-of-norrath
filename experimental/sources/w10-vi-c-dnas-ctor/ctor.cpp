// Observed: __9VIWndDnas at 0x010a8428 is 76 bytes. With the object as this it calls
// __5VIWnd(this), stores the address of _vt$9VIWndDnas (0x01207e70) at this+32, runs
// __10VIUIWindow on this+40, this+408 and this+784, then tail-calls __12VIWndMessage on
// this+1176. Nothing is returned.
// Inferred: three embedded UI windows and one message window. Unknown: what each is used for.
extern "C" {
void __5VIWnd(void* self);
void __10VIUIWindow(void* self);
void __12VIWndMessage(void* self);
extern unsigned char _vt$9VIWndDnas[];

void __9VIWndDnas(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndDnas));
    __10VIUIWindow(bytes + 40);
    __10VIUIWindow(bytes + 408);
    __10VIUIWindow(bytes + 784);
    __12VIWndMessage(bytes + 1176);
}
}
