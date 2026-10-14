// Observed: Next__Ct6VIPool1Z19VIHSpriteAttachmenti at 0x01203768 (the symbol size says 36 bytes; the code runs to 0x120379f, 64 bytes
// with the closing nop). With (pool, index) it reads the word at pool+8, indexes it in 16-byte steps and loads the word at +8 of that
// element. A value other than -1 is returned; -1 makes it tail-call End__Ct6VIPool1Z19VIHSpriteAttachment with the pool.
// Inferred: element +8 is the next index of a chain, -1 ends it; the end marker comes from the End routine.
// Unknown: the other three words of each 16-byte element.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int End__Ct6VIPool1Z19VIHSpriteAttachment(const void* pool);

int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const void* pool, int index) {
    const unsigned char* elements = NORRATH_WORD_TO_POINTER(*(static_cast<const unsigned*>(pool) + 2));
    int next = *reinterpret_cast<const int*>(elements + index * 16 + 8);
    if (next != -1) return next;
    return End__Ct6VIPool1Z19VIHSpriteAttachment(pool);
}
}
