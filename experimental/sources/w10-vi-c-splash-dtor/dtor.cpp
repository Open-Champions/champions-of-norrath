// Observed: _$_11VIWndSplash at 0x010d6e68 is 148 bytes. With (this, flags) it stores the
// address of _vt$11VIWndSplash (0x0120f640) at this+32, then runs _$_12VIWndMessage(this+4776,
// 2), _$_12VIWndOptions(this+2408, 2), _$_12VIWndConnect(this+416, 2), _$_10VIUIWindow(this+40,
// 2), then _$_5VIWnd(this, 0), and when bit 0 of flags is set tail-calls
// __builtin_delete(this). Nothing is returned.
extern "C" {
void _$_5VIWnd(void* self, int flags);
void _$_12VIWndMessage(void* self, int flags);
void _$_12VIWndOptions(void* self, int flags);
void _$_12VIWndConnect(void* self, int flags);
void _$_10VIUIWindow(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$11VIWndSplash[];

void dtor_11VIWndSplash(void* self, int flags) asm("_$_11VIWndSplash");
void dtor_11VIWndSplash(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$11VIWndSplash));
    _$_12VIWndMessage(bytes + 4776, 2);
    _$_12VIWndOptions(bytes + 2408, 2);
    _$_12VIWndConnect(bytes + 416, 2);
    _$_10VIUIWindow(bytes + 40, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
