// Observed: Allocate__t6VIPool1ZP13VIHSpriteAnim at 0x01225930, 304 bytes. Offsets are bytes from the pool; entries are 12 bytes.
//  1. Capacity is the word at +4; zero returns 0 at once.
//  2. When the storage word at +8 is zero: __builtin_vec_new(capacity * 12) is stored at +8 (before the
//     result is tested); a null result returns -1, otherwise go to step 4.
//  3. Otherwise a new block of the same size is requested (null returns -1, nothing stored). For each of
//     the first word[0] entries, the 12 bytes are copied to the same slot of the new block unless that
//     address is null. Then Begin__Ct6VIPool1ZP13VIHSpriteAnim, Next__Ct6VIPool1ZP13VIHSpriteAnimi and End__Ct6VIPool1ZP13VIHSpriteAnim walk the pool
//     exactly as DestroyPool does (End is called again every time) with nothing done per entry; the old
//     block, if the word at +8 is still nonzero, goes to __builtin_vec_delete, and the new block is
//     stored at +8.
//  4. LinkFreeChain__t6VIPool1ZP13VIHSpriteAnimi(pool, word[0]) runs; a negative result returns -1, otherwise 0.
// Inferred: +0 is the number of live entries and +8 the entry block (both used this way by Clear and Add).
// The walk is the body of DestroyPool compiled inline.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int Begin__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool);
int End__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool);
int Next__Ct6VIPool1ZP13VIHSpriteAnimi(const void* pool, int cursor);
int LinkFreeChain__t6VIPool1ZP13VIHSpriteAnimi(void* pool, int count);

int Allocate__t6VIPool1ZP13VIHSpriteAnim(void* pool) {
    struct Entry { unsigned a, b, c; };
    unsigned* w = static_cast<unsigned*>(pool);
    int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* block = __builtin_vec_new(capacity * 12);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
        if (block == 0) return -1;
    } else {
        Entry* fresh = static_cast<Entry*>(__builtin_vec_new(capacity * 12));
        if (fresh == 0) return -1;
        for (int i = 0; i < static_cast<int>(w[0]); ++i) {
            Entry* to = fresh + i;
            if (to != 0) *to = static_cast<Entry*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])))[i];
        }
        int cursor = Begin__Ct6VIPool1ZP13VIHSpriteAnim(pool);
        while (cursor != End__Ct6VIPool1ZP13VIHSpriteAnim(pool)) cursor = Next__Ct6VIPool1ZP13VIHSpriteAnimi(pool, cursor);
        if (w[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])));
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    }
    return LinkFreeChain__t6VIPool1ZP13VIHSpriteAnimi(pool, static_cast<int>(w[0])) < 0 ? -1 : 0;
}
}
