// Observed: _$_12VIWndPatcher at 0x010ccb08 is 164 bytes. With (this, flags) it stores the
// address of _vt$12VIWndPatcher (0x0120c830) at this+32, then runs _$_16VIWndReadMessage(
// this+2312, 2), _$_12VIWndMessage(this+1176, 2), _$_10VIUIWindow(this+776, 2), (this+408, 2),
// (this+40, 2), then _$_5VIWnd(this, 0), and when bit 0 of flags is set tail-calls
// __builtin_delete(this). Nothing is returned.
extern "C" {
void _$_5VIWnd(void* self, int flags);
void _$_16VIWndReadMessage(void* self, int flags);
void _$_12VIWndMessage(void* self, int flags);
void _$_10VIUIWindow(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$12VIWndPatcher[];

void dtor_12VIWndPatcher(void* self, int flags) asm("_$_12VIWndPatcher");
void dtor_12VIWndPatcher(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndPatcher));
    _$_16VIWndReadMessage(bytes + 2312, 2);
    _$_12VIWndMessage(bytes + 1176, 2);
    _$_10VIUIWindow(bytes + 776, 2);
    _$_10VIUIWindow(bytes + 408, 2);
    _$_10VIUIWindow(bytes + 40, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
