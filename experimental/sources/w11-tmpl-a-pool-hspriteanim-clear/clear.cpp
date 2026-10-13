// Observed: Clear__t6VIPool1ZP13VIHSpriteAnim at 0x01224e70, 104 bytes. Offsets are bytes from the pool.
//  1. When the word at +8 is nonzero: call DestroyPool for the same type with the pool,
//     then read +8 again and, if still nonzero, pass it to __builtin_vec_delete.
//     DestroyPool's result is not looked at.
//  2. Stores zero at +0, +4 and +8 and -1 at +20, +12 and +16; returns 0.
// Inferred: +8 is the entry array, +4 the capacity set by Init, +16 the first index.
extern "C" {
void DestroyPool__t6VIPool1ZP13VIHSpriteAnim(void* pool);
void __builtin_vec_delete(void* pointer);

int Clear__t6VIPool1ZP13VIHSpriteAnim(void* pool) {
    unsigned* words = static_cast<unsigned*>(pool);
    if (words[2] != 0) {
        DestroyPool__t6VIPool1ZP13VIHSpriteAnim(pool);
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
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
