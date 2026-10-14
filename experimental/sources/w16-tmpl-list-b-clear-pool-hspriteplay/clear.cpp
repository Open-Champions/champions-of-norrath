// Observed: Clear__t6VIPool1Z13VIHSpritePlay at 0x01203660, 104 bytes. Offsets are bytes from the pool.
//  1. When the word at +8 is nonzero: call DestroyPool__t6VIPool1Z13VIHSpritePlay with the pool, then
//     read +8 again and, if still nonzero, pass it to __builtin_vec_delete. DestroyPool's result is not looked at.
//  2. Stores zero at +0, +4 and +8 and -1 at +20, +12 and +16 (the -1 stores happen whether or not +8 was
//     nonzero); returns 0.
// Inferred: +8 is the entry array, +4 the capacity set by Init, +16 the first index. Same body as the
// other VIPool instances; template arguments come only from the mangled name.
// Unknown: the meaning of +0, +12 and +20 beyond their reset values.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int DestroyPool__t6VIPool1Z13VIHSpritePlay(void* pool);
void __builtin_vec_delete(void* pointer);

int Clear__t6VIPool1Z13VIHSpritePlay(void* pool) {
    unsigned* words = static_cast<unsigned*>(pool);
    if (words[2] != 0) {
        DestroyPool__t6VIPool1Z13VIHSpritePlay(pool);
        if (words[2] != 0) __builtin_vec_delete(at(words[2]));
    }
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[5] = static_cast<unsigned>(-1);
    words[3] = static_cast<unsigned>(-1);
    words[4] = static_cast<unsigned>(-1);
    return 0;
}
}
