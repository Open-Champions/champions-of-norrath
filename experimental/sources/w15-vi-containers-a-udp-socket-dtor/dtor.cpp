// Observed: _$_11VIUDPSocket at 0x0102a1c8, 124 bytes, returns nothing. With (this, flags):
//  1. Close__11VIUDPSocket(this).
//  2. When the word at +4 is nonzero: __wrap_free of it, +4 set to zero, then sceLibnetTerminate(this + 8).
//  3. The words at +68 and +0 are set to -1.
//  4. When bit 0 of flags is set, __builtin_delete(this) is tail-called.
// Inferred: +4 is a heap buffer allocated by the socket's open or setup path, +8 a library network context that is torn down together
// with it, and +0 and +68 hold handle-like values reset to -1 (the usual "none" value); flags bit 0 is the delete-after flag.
// Unknown: what the context at +8 is beyond its use as the argument of sceLibnetTerminate, and how the buffer is allocated.
#ifdef NORRATH_HOST_CONTRACT
extern "C" void* norrath_host_buffer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_buffer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Close__11VIUDPSocket(void* self);
void __wrap_free(void* pointer);
int sceLibnetTerminate(void* context);
void __builtin_delete(void* pointer);

void _$_11VIUDPSocket(void* self, int flags) {
    unsigned* words = static_cast<unsigned*>(self);
    Close__11VIUDPSocket(self);
    if (words[1] != 0) {
        __wrap_free(NORRATH_WORD_TO_POINTER(words[1]));
        words[1] = 0;
        sceLibnetTerminate(words + 2);
    }
    words[17] = static_cast<unsigned>(-1);
    words[0] = static_cast<unsigned>(-1);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
