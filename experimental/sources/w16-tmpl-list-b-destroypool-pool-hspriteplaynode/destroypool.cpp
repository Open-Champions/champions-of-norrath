// Observed: DestroyPool__t6VIPool1Z17VIHSpritePlayNode at 0x01203968, 112 bytes. The pool is the only argument. Offsets are bytes from the pool.
//  1. cursor = Begin__Ct6VIPool1Z17VIHSpritePlayNode(pool).
//  2. While cursor differs from End__Ct6VIPool1Z17VIHSpritePlayNode(pool) (called again on every test): read the word at +8
//     afresh; element = that word + 52 * cursor; the word at element+44 is the next cursor, except that -1
//     is replaced by a fresh End__Ct6VIPool1Z17VIHSpritePlayNode(pool) call.
//  3. Returns 0. Nothing is written, freed or destroyed.
// Inferred: Begin/End/Next are inlined or called as the iterator of this pool; the chain-link word at
// element+44 plays the role the Next accessor reads. Element size 52 comes from the index scaling.
// Unknown: what End returns besides the end marker and whether a destructor was dropped from the loop.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Begin__Ct6VIPool1Z17VIHSpritePlayNode(const void* pool);
int End__Ct6VIPool1Z17VIHSpritePlayNode(const void* pool);

int DestroyPool__t6VIPool1Z17VIHSpritePlayNode(void* pool) {
    unsigned* words = static_cast<unsigned*>(pool);
    int cursor = Begin__Ct6VIPool1Z17VIHSpritePlayNode(pool);
    while (cursor != End__Ct6VIPool1Z17VIHSpritePlayNode(pool)) {
        const unsigned char* entries = static_cast<const unsigned char*>(at(words[2]));
        int next = *reinterpret_cast<const int*>(entries + 52 * cursor + 44);
        cursor = next == -1 ? End__Ct6VIPool1Z17VIHSpritePlayNode(pool) : next;
    }
    return 0;
}
}
