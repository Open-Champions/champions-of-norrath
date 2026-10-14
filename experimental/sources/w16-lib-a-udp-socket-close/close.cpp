// Observed: Close__11VIUDPSocket at 0x0102a510, 92 bytes, pinned SLUS_205.65. With (this):
//  1. printf of a one-line "Socket close" log message (no further arguments).
//  2. The word at +68 is read. When it is -1 the function returns -1 and changes nothing else.
//  3. Otherwise sceInetClose(this + 8, word at +4, the word at +68, 0) is called, the word at +68 is set to -1,
//     and the result of sceInetClose is returned.
// Inferred: +68 holds the handle of an open socket, +8 is the library network context and +4 its buffer (the destructor of
// this class passes the same two fields to sceLibnetTerminate and __wrap_free), so this closes the socket once.
// Unknown: the meaning of the final zero argument, and the class name's other fields.
#ifdef NORRATH_HOST_CONTRACT
extern "C" void* norrath_host_buffer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_buffer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int printf(const char* format, ...);
int sceInetClose(void* context, void* buffer, int handle, int flags);

int Close__11VIUDPSocket(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    printf("Socket close\n");
    int result = -1;
    if (words[17] != static_cast<unsigned>(-1)) {
        result = sceInetClose(words + 2, NORRATH_WORD_TO_POINTER(words[1]), static_cast<int>(words[17]), 0);
        words[17] = static_cast<unsigned>(-1);
    }
    return result;
}
}
