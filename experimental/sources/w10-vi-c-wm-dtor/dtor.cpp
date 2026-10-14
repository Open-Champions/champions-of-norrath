// Observed: _$_15VIWindowManager at 0x0109b628 is 100 bytes. It stores the address of
// _vt$15VIWindowManager (0x01207b58) at this+32, calls _$_5VIWnd(this, 0), and when bit 0 of
// the second argument is set tail-calls __builtin_delete(this). Nothing is returned.
extern "C" {
void _$_5VIWnd(void* self, int flags);
void __builtin_delete(void* pointer);
extern unsigned char _vt$15VIWindowManager[];

void dtor_15VIWindowManager(void* self, int flags) asm("_$_15VIWindowManager");
void dtor_15VIWindowManager(void* self, int flags) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$15VIWindowManager));
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
