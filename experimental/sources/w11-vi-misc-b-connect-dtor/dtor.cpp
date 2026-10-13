// Observed: _$_12VIWndConnect at 0x010a2a78 is 164 bytes. With (this, flags) it stores the
// address of _vt$12VIWndConnect (0x01207db8) at this+32, then if the word at this+1912 is nonzero
// calls RemoveChild__5VIWndP5VIWnd(this, word) and stores 0 there, then runs
// _$_10VIUIWindow(this+1544, 2), then runs _$_10VIUIWindow(this+1176, 2), then runs
// _$_12VIWndMessage(this+40, 2), then runs _$_5VIWnd(this, 0), and when bit 0 of flags is set
// tail-calls __builtin_delete(this). Nothing is returned.
extern "C" {
void _$_10VIUIWindow(void* self, int flags);
void _$_12VIWndMessage(void* self, int flags);
void _$_5VIWnd(void* self, int flags);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void __builtin_delete(void* pointer);
extern unsigned char _vt$12VIWndConnect[];

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}

void dtor_12VIWndConnect(void* self, int flags) asm("_$_12VIWndConnect");
void dtor_12VIWndConnect(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndConnect));
    if (unsigned child = *reinterpret_cast<unsigned*>(bytes + 1912)) {
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(child));
        *reinterpret_cast<unsigned*>(bytes + 1912) = 0;
    }
    _$_10VIUIWindow(bytes + 1544, 2);
    _$_10VIUIWindow(bytes + 1176, 2);
    _$_12VIWndMessage(bytes + 40, 2);
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
