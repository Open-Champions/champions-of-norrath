// Observed: Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay at 0x01202328, 372 bytes. Arguments (pool, source). The pool object is words: +0 a count, +4 a capacity, +8 the element
// pointer, +12, +16 and +20 three index words that start at -1 (+16 is what Begin reads as the first live entry). When the element
// pointer is non-zero it calls DestroyPool__t6VIPool1Z13VIHSpritePlay(pool) (the result is ignored) and passes the pointer to __builtin_vec_delete
// when it is still non-zero. It then stores zero at +0, +4 and +8 and -1 at +12, +16 and +20, copies the source capacity to +4 and calls
// Allocate__t6VIPool1Z13VIHSpritePlay(pool); a negative result returns -1. It then walks the source's live chain: first = source word +16, with -1
// meaning 0x7fffffff (the End value); while the index is not 0x7fffffff it copies the 104 bytes (three runs of 32 bytes in 64-bit moves, then 8 more bytes) element at source.data + index*104 to
// pool.data + index*104 (skipped when that address is zero) and moves to the word at element+96, again mapping -1 to 0x7fffffff. The source
// words +0, +12, +16 and +20 are then copied to the pool. When source +12 is -1 it returns zero. Otherwise it follows the free chain from
// source +12: for each index it stores the source element's word at +96 into the pool element's +96 and -2 into its +100, then continues with the
// word now at +96 of the pool element, until it reads -1. It returns zero.
// Inferred: a copy-initialiser for a slab pool of 104-byte records with an intrusive live chain (head at +16) and free chain (head at
// +12), -2 marking a free slot. Begin, Next and End of this instantiation are inlined here; their separate bodies are other symbols.
// Unknown: the role of word +20 (copied only), the field layout of the record, and whether DestroyPool can fail (its result is not read).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int DestroyPool__t6VIPool1Z13VIHSpritePlay(void* pool);
int Allocate__t6VIPool1Z13VIHSpritePlay(void* pool);
void __builtin_vec_delete(void* pointer);

int Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(void* pool, const void* source) {
    unsigned* words = static_cast<unsigned*>(pool);
    const unsigned* from = static_cast<const unsigned*>(source);
    if (words[2] != 0) {
        DestroyPool__t6VIPool1Z13VIHSpritePlay(pool);
        if (words[2] != 0) __builtin_vec_delete(NORRATH_WORD_TO_POINTER(words[2]));
    }
    words[1] = 0;
    words[0] = 0;
    words[3] = static_cast<unsigned>(-1);
    words[4] = static_cast<unsigned>(-1);
    words[5] = static_cast<unsigned>(-1);
    words[2] = 0;
    words[1] = from[1];
    if (Allocate__t6VIPool1Z13VIHSpritePlay(pool) < 0) return -1;
    int index = static_cast<int>(from[4]) == -1 ? 0x7fffffff : static_cast<int>(from[4]);
    while (index != 0x7fffffff) {
        unsigned char* destination = NORRATH_WORD_TO_POINTER(words[2]) + index * 104;
        const unsigned char* sourceElement = NORRATH_WORD_TO_POINTER(from[2]) + index * 104;
        if (destination) {
            const unsigned* s = reinterpret_cast<const unsigned*>(sourceElement);
            unsigned* d = reinterpret_cast<unsigned*>(destination);
            for (int k = 0; k < 26; ++k) d[k] = s[k];
        }
        int next = *reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(from[2]) + index * 104 + 96);
        index = next == -1 ? 0x7fffffff : next;
    }
    words[0] = from[0];
    words[3] = from[3];
    words[4] = from[4];
    words[5] = from[5];
    int free = static_cast<int>(from[3]);
    if (free == -1) return 0;
    do {
        unsigned char* destination = NORRATH_WORD_TO_POINTER(words[2]) + free * 104;
        const unsigned char* sourceElement = NORRATH_WORD_TO_POINTER(from[2]) + free * 104;
        int next = *reinterpret_cast<const int*>(sourceElement + 96);
        *reinterpret_cast<int*>(destination + 100) = -2;
        *reinterpret_cast<int*>(destination + 96) = next;
        free = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(words[2]) + free * 104 + 96);
    } while (free != -1);
    return 0;
}
}
