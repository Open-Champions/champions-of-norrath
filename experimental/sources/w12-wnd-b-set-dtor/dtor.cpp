// Observed: _$_t5VISet1ZP5VIWnd at 0x0120ba38 is 116 bytes (weak). With (this, flags): when the word
// at +8 is nonzero it calls DestroyPool__t5VISet1ZP5VIWnd(this), re-reads the word at +8 and, if it
// is still nonzero, calls __builtin_vec_delete on it. Then, when bit 0 of flags is set, it tail-calls
// __builtin_delete(this). Nothing is returned.
// Inferred: this is the destructor of the VISet template instance for window pointers; the +8 word
// is an array block owned by the set.
// Unknown: what DestroyPool does with the block before the delete, and what flags bits other than
// bit 0 mean (they are ignored).
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void DestroyPool__t5VISet1ZP5VIWnd(void* self);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t5VISet1ZP5VIWnd(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<unsigned*>(bytes + 8) != 0) {
        DestroyPool__t5VISet1ZP5VIWnd(self);
        if (unsigned block = *reinterpret_cast<unsigned*>(bytes + 8)) __builtin_vec_delete(NORRATH_TRANSLATE(block));
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
