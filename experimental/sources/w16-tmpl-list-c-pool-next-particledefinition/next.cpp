// Observed: Next__Ct6VIPool1ZP20VIParticleDefinitioni at 0x01209038, 68 bytes. With (pool, index) it reads the
// word at pool+8, indexes it in 12-byte steps, loads the word at +4 of that element and returns it. When that word is -1 the result is that of End__Ct6VIPool1ZP20VIParticleDefinition(pool) (a tail call); otherwise it is the word.
// Inferred: element +4 is the next index of a chain, -1 ends it, and 0x7fffffff is the iterator end marker.
// Unknown: the other fields of the 12-byte element.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int End__Ct6VIPool1ZP20VIParticleDefinition(const void* pool);
int Next__Ct6VIPool1ZP20VIParticleDefinitioni(const void* pool, int index) {
    const unsigned char* elements = NORRATH_WORD_TO_POINTER(*(static_cast<const unsigned*>(pool) + 2));
    int next = *reinterpret_cast<const int*>(elements + index * 12 + 4);
    if (next == -1) return End__Ct6VIPool1ZP20VIParticleDefinition(pool);
    return next;
}
}
