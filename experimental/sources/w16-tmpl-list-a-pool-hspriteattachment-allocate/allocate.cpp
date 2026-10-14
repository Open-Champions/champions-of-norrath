// Observed: Allocate__t6VIPool1Z19VIHSpriteAttachment at 0x01202b68, 264 bytes. Offsets are bytes from the pool: +0 element
// count, +4 capacity, +8 element block; elements are 16 bytes.
//  1. A capacity of zero returns 0 at once.
//  2. When the block word is zero: __builtin_vec_new(capacity * 16) is stored at +8 before the
//     result is tested; a null block returns -1 and a good one continues at step 5.
//  3. Otherwise a fresh block is requested (null returns -1, nothing stored). For each index below
//     the count (the count word is read again after every element copy) the 16-byte element of
//     the old block is copied to the same slot of the fresh block (skipped for a null slot address).
//  4. DestroyPool__t6VIPool1Z19VIHSpriteAttachment(pool) is called; a negative result returns -1 and the fresh block is not
//     released. A nonzero old block word then goes to __builtin_vec_delete and the fresh block is
//     stored at +8.
//  5. LinkFreeChain__t6VIPool1Z19VIHSpriteAttachmenti(pool, count) is called with the count word at +0; the result is -1 when it is
//     negative and 0 otherwise.
// Inferred: this is the grow step of a slab pool whose free chain is rebuilt by LinkFreeChain; the
// copy is a plain byte copy of the element (the original uses doubleword loads and stores).
// Unknown: the element's fields, and the template parameters beyond the mangled name.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int DestroyPool__t6VIPool1Z19VIHSpriteAttachment(void* pool);
int LinkFreeChain__t6VIPool1Z19VIHSpriteAttachmenti(void* pool, int from);

int Allocate__t6VIPool1Z19VIHSpriteAttachment(void* pool) {
    unsigned* w = static_cast<unsigned*>(pool);
    const int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* fresh = __builtin_vec_new(static_cast<unsigned>(capacity) * 16);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
        if (fresh == 0) return -1;
    } else {
        unsigned char* fresh = static_cast<unsigned char*>(__builtin_vec_new(static_cast<unsigned>(capacity) * 16));
        if (fresh == 0) return -1;
        int count = static_cast<int>(w[0]);
        for (int index = 0; index < count;) {
            unsigned char* to = fresh + index * 16;
            const unsigned char* from =
                reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(w[2])) + index * 16;
            ++index;
            if (to != 0) {
                for (int i = 0; i < 16; i += 4) {
                    *reinterpret_cast<unsigned*>(to + i) = *reinterpret_cast<const unsigned*>(from + i);
                }
                count = static_cast<int>(w[0]);
            }
        }
        if (DestroyPool__t6VIPool1Z19VIHSpriteAttachment(pool) < 0) return -1;
        if (w[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w[2])));
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    }
    return LinkFreeChain__t6VIPool1Z19VIHSpriteAttachmenti(pool, static_cast<int>(w[0])) < 0 ? -1 : 0;
}
}
