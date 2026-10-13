// Observed: _$_16VIWndReadMessage at 0x010d1468 is 116 bytes. With (this, flags) it stores the
// address of _vt$16VIWndReadMessage (0x0120c968) at this+32, then calls
// Clear__16VIWndReadMessage(this), then runs _$_10VIUIWindow(this+40, 2), then runs
// _$_5VIWnd(this, 0), and when bit 0 of flags is set tail-calls __builtin_delete(this). Nothing
// is returned.
extern "C" {
void _$_10VIUIWindow(void* self, int flags);
void _$_5VIWnd(void* self, int flags);
void Clear__16VIWndReadMessage(void* self);
void __builtin_delete(void* pointer);
extern unsigned char _vt$16VIWndReadMessage[];

void dtor_16VIWndReadMessage(void* self, int flags) asm("_$_16VIWndReadMessage");
void dtor_16VIWndReadMessage(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16VIWndReadMessage));
    Clear__16VIWndReadMessage(self);
    _$_10VIUIWindow(bytes + 40, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
