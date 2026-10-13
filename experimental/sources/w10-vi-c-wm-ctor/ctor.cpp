// Observed: __15VIWindowManager at 0x0109b5f8 is 48 bytes. It calls __5VIWnd(this), then stores
// the address of _vt$15VIWindowManager (0x01207b58) at this+32. Nothing is returned.
// Inferred: a VIWnd-derived constructor whose only own step is installing its table.
extern "C" {
void __5VIWnd(void* self);
extern unsigned char _vt$15VIWindowManager[];

void __15VIWindowManager(void* self) {
    __5VIWnd(self);
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 32) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$15VIWindowManager));
}
}
