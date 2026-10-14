// Observed: _$_10VIWndLegal at 0x010bf910 is 100 bytes. It stores the address of
// _vt$10VIWndLegal (0x0120aa38) at this+32, calls _$_5VIWnd(this, 0), and when bit 0 of the
// second argument is set tail-calls __builtin_delete(this). Nothing is returned.
// Inferred: a destructor with the usual "also delete" flag in bit 0. Unknown: what flag
// bits other than bit 0 mean to callers; they have no effect here.
extern "C" {
void _$_5VIWnd(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$10VIWndLegal[];

void dtor_10VIWndLegal(void* self, int flags) asm("_$_10VIWndLegal");
void dtor_10VIWndLegal(void* self, int flags) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$10VIWndLegal));
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
