// Observed: Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode at 0x01202510, 428 bytes. Arguments (pool, source). The
// pool object is words: +0 a count, +4 a capacity, +8 the element pointer, +12, +16 and +20 index words that start at -1. When the
// element pointer is non-zero it calls DestroyPool__t6VIPool1Z17VIHSpritePlayNode(pool) (the result is ignored) and passes the pointer to
// __builtin_vec_delete when it is still non-zero. It then stores zero at +0, +4 and +8 and -1 at +12, +16 and +20, copies the source
// capacity to +4 and calls Allocate__t6VIPool1Z17VIHSpritePlayNode(pool); a negative result returns -1.
// It then iterates the source with Begin__Ct6VIPool1Z17VIHSpritePlayNode(source) and End__Ct6VIPool1Z17VIHSpritePlayNode(source), the latter
// called again for every comparison; for each index it copies the 52-byte element at source.data + index*52 to pool.data + index*52
// (skipped when that address is zero) -- words at +0, +4, +8, +44 and +48, and floats at +12..+28 (five) and +32..+40 (three) -- and
// advances to the word at element+44, calling End for the result when that word is -1. The source words +0, +12, +16 and +20 are then
// copied to the pool. When source +12 is -1 it returns zero. Otherwise it follows the free chain from source +12: for each index it stores
// the source element's word at +44 into the pool element's +44 and -2 into its +48, then continues with the word now at +44 of the pool
// element, until it reads -1. It returns zero.
// Inferred: a copy-initialiser for a slab pool of 52-byte records with a live chain (head at +16) and a free chain (head at +12), -2 being
// the free-slot marker; the same shape as the other VIPool copy initialisers, but here Begin and End are real calls.
// Unknown: the role of word +20, the record's field meanings, and the callees' behaviour beyond the calls seen.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int DestroyPool__t6VIPool1Z17VIHSpritePlayNode(void* pool);
int Allocate__t6VIPool1Z17VIHSpritePlayNode(void* pool);
int Begin__Ct6VIPool1Z17VIHSpritePlayNode(const void* pool);
int End__Ct6VIPool1Z17VIHSpritePlayNode(const void* pool);
void __builtin_vec_delete(void* pointer);

int Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(void* pool, const void* source) {
    unsigned* words = static_cast<unsigned*>(pool);
    const unsigned* from = static_cast<const unsigned*>(source);
    if (words[2] != 0) {
        DestroyPool__t6VIPool1Z17VIHSpritePlayNode(pool);
        if (words[2] != 0) __builtin_vec_delete(NORRATH_WORD_TO_POINTER(words[2]));
    }
    words[1] = 0;
    words[5] = static_cast<unsigned>(-1);
    words[0] = 0;
    words[3] = static_cast<unsigned>(-1);
    words[4] = static_cast<unsigned>(-1);
    words[2] = 0;
    words[1] = from[1];
    if (Allocate__t6VIPool1Z17VIHSpritePlayNode(pool) < 0) return -1;
    int index = Begin__Ct6VIPool1Z17VIHSpritePlayNode(source);
    while (index != End__Ct6VIPool1Z17VIHSpritePlayNode(source)) {
        unsigned char* destination = NORRATH_WORD_TO_POINTER(words[2]) + index * 52;
        if (destination) {
            const unsigned char* sourceElement = NORRATH_WORD_TO_POINTER(from[2]) + index * 52;
            *reinterpret_cast<float*>(destination + 12) = *reinterpret_cast<const float*>(sourceElement + 12);
            *reinterpret_cast<unsigned*>(destination + 4) = *reinterpret_cast<const unsigned*>(sourceElement + 4);
            *reinterpret_cast<float*>(destination + 16) = *reinterpret_cast<const float*>(sourceElement + 16);
            *reinterpret_cast<unsigned*>(destination + 8) = *reinterpret_cast<const unsigned*>(sourceElement + 8);
            *reinterpret_cast<float*>(destination + 20) = *reinterpret_cast<const float*>(sourceElement + 20);
            *reinterpret_cast<unsigned*>(destination + 44) = *reinterpret_cast<const unsigned*>(sourceElement + 44);
            *reinterpret_cast<unsigned*>(destination + 0) = *reinterpret_cast<const unsigned*>(sourceElement + 0);
            *reinterpret_cast<float*>(destination + 24) = *reinterpret_cast<const float*>(sourceElement + 24);
            *reinterpret_cast<float*>(destination + 28) = *reinterpret_cast<const float*>(sourceElement + 28);
            *reinterpret_cast<float*>(destination + 32) = *reinterpret_cast<const float*>(sourceElement + 32);
            *reinterpret_cast<float*>(destination + 36) = *reinterpret_cast<const float*>(sourceElement + 36);
            *reinterpret_cast<float*>(destination + 40) = *reinterpret_cast<const float*>(sourceElement + 40);
            *reinterpret_cast<unsigned*>(destination + 48) = *reinterpret_cast<const unsigned*>(sourceElement + 48);
        }
        int next = *reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(from[2]) + index * 52 + 44);
        index = next == -1 ? End__Ct6VIPool1Z17VIHSpritePlayNode(source) : next;
    }
    words[0] = from[0];
    words[3] = from[3];
    words[4] = from[4];
    words[5] = from[5];
    int free = static_cast<int>(from[3]);
    if (free == -1) return 0;
    do {
        unsigned char* destination = NORRATH_WORD_TO_POINTER(words[2]) + free * 52;
        const unsigned char* sourceElement = NORRATH_WORD_TO_POINTER(from[2]) + free * 52;
        int next = *reinterpret_cast<const int*>(sourceElement + 44);
        *reinterpret_cast<int*>(destination + 48) = -2;
        *reinterpret_cast<int*>(destination + 44) = next;
        free = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(words[2]) + free * 52 + 44);
    } while (free != -1);
    return 0;
}
}
