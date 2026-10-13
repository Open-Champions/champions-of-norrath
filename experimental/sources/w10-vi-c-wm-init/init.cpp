// Observed: Init__15VIWindowManagerP5VIPadP10VIKeyboard at 0x0109b690 is 48 bytes. With
// (this, pad, keyboard) it stores pad at this+40 and keyboard at this+44, clears 84 bytes
// starting at this+48 with memset, and returns 0.
// Inferred: the 84-byte block is the per-button state that Update__15VIWindowManager walks
// (20 words from this+52, plus a word at this+48); this is read from that routine, not proven.
extern "C" {
void* memset(void* destination, int value, unsigned count);

int Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* self, void* pad, void* keyboard) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 40) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pad));
    *reinterpret_cast<unsigned*>(bytes + 44) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(keyboard));
    memset(bytes + 48, 0, 84);
    return 0;
}
}
