// Observed: _$_9VIWndEula at 0x010b5720 is 132 bytes. With (this, flags) it stores the address
// of _vt$9VIWndEula (0x01208a78) at this+32, calls Free__9VIWndEula(this), then
// _$_12VIWndMessage(this+408, 2), _$_10VIUIWindow(this+40, 2), _$_5VIWnd(this, 0), and when bit
// 0 of flags is set tail-calls __builtin_delete(this). Nothing is returned.
extern "C" {
void Free__9VIWndEula(void* self);
void _$_5VIWnd(void* self, int flags);
void _$_12VIWndMessage(void* self, int flags);
void _$_10VIUIWindow(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$9VIWndEula[];

void dtor_9VIWndEula(void* self, int flags) asm("_$_9VIWndEula");
void dtor_9VIWndEula(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEula));
    Free__9VIWndEula(self);
    _$_12VIWndMessage(bytes + 408, 2);
    _$_10VIUIWindow(bytes + 40, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
