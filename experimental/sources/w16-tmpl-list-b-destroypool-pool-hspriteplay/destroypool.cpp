// Observed: DestroyPool__t6VIPool1Z13VIHSpritePlay at 0x01202af8, 112 bytes. The pool is the only argument. Offsets are bytes from the pool.
//  1. cursor = the word at +16; a value of -1 is replaced by 0x7fffffff (the end marker).
//  2. The word at +8 is read once. While the cursor is not 0x7fffffff: element = that word + 104 * cursor, the
//     word at element+96 becomes the next cursor (again -1 -> 0x7fffffff).
//  3. Returns 0. Nothing is written, freed or called.
// Inferred: +8 is the entry array and +16 the first used index; the walk follows the chain of next-index
// words and each element's teardown is a no-op for this element type. The element size comes from the index
// scaling and the template argument only from the mangled name.
// Unknown: what the next-index word means beyond being a chain link. A chain that never reaches -1 loops
// forever here as it does in the image.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int DestroyPool__t6VIPool1Z13VIHSpritePlay(void* pool) {
    unsigned* words = static_cast<unsigned*>(pool);
    int cursor = static_cast<int>(words[4]);
    if (cursor == -1) cursor = 0x7fffffff;
    const unsigned char* entries = static_cast<const unsigned char*>(at(words[2]));
    while (cursor != 0x7fffffff) {
        int next = *reinterpret_cast<const volatile int*>(entries + 104 * cursor + 96);
        cursor = next == -1 ? 0x7fffffff : next;
    }
    return 0;
}
}
