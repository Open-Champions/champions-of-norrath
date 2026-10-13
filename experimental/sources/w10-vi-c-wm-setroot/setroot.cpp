// Observed: SetRoot__15VIWindowManagerP5VIWnd at 0x0109b6d0 is 8 bytes. It stores its second
// argument as a 32-bit word at this+36 and returns nothing.
// Unknown: the lifetime of the stored window; Update__15VIWindowManager reads this word.
extern "C" void SetRoot__15VIWindowManagerP5VIWnd(void* self, void* root) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 36) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(root));
}
