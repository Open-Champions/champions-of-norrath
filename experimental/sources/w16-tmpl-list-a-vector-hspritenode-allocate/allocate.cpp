// Observed: Allocate__t8VIVector1Z13VIHSpriteNode at 0x01202f78, 560 bytes. Offsets are bytes from
// the vector: +0 element count, +4 capacity, +8 element block; elements are 256 bytes.
//  1. A capacity of zero returns 0 at once.
//  2. When the block word is zero: __builtin_vec_new(capacity * 256) is stored at +8 before the
//     result is tested; the result is 0, or -1 for a null block.
//  3. Otherwise a fresh block is requested (null returns -1, nothing stored). For each index below
//     the count (the count word is read after the last field copy of each element) the element in
//     the fresh block is filled from the same element of the old block unless its address is
//     null. The copy is member by member, as 32-bit words:
//       +0..+15 (4 words), +16..+35 (5), +36..+47 (3), +48..+111 by
//       __10VIMatrix44RC10VIMatrix44(destination + 48, source + 48), +112..+123 (3),
//       +124..+143 (5), +144..+155 (3), +160..+223 by the same matrix copy call at +160,
//       +224..+243 (5), +244..+255 (3).
//     Bytes +156..+159 are not copied.
//  4. DestroyPool__t8VIVector1Z13VIHSpriteNode(vector) is called; a negative result returns -1
//     (the fresh block is not released). Otherwise a nonzero old block word goes to
//     __builtin_vec_delete, the fresh block is stored at +8 and the result is 0.
// Inferred: the grow step of VIVector<VIHSpriteNode>; the word groups are the copy constructor of
// the element, with two embedded VIMatrix44 members at +48 and +160.
// Unknown: the element's field meanings, and the template parameters beyond the mangled name.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
void* __10VIMatrix44RC10VIMatrix44(void* destination, const void* source);
int DestroyPool__t8VIVector1Z13VIHSpriteNode(void* vector);

static inline void copy_words(unsigned char* to, const unsigned char* from, int offset, int words) {
    for (int i = 0; i < words; ++i) {
        *reinterpret_cast<unsigned*>(to + offset + 4 * i) =
            *reinterpret_cast<const unsigned*>(from + offset + 4 * i);
    }
}

int Allocate__t8VIVector1Z13VIHSpriteNode(void* vector) {
    unsigned* w = static_cast<unsigned*>(vector);
    const int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* fresh = __builtin_vec_new(static_cast<unsigned>(capacity) << 8);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
        return fresh == 0 ? -1 : 0;
    }
    unsigned char* fresh = static_cast<unsigned char*>(__builtin_vec_new(static_cast<unsigned>(capacity) << 8));
    if (fresh == 0) return -1;
    int count = static_cast<int>(w[0]);
    for (int index = 0; index < count;) {
        unsigned char* to = fresh + index * 256;
        const unsigned char* from =
            reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(w[2])) + index * 256;
        ++index;
        if (to != 0) {
            copy_words(to, from, 0, 4);
            copy_words(to, from, 16, 5);
            copy_words(to, from, 36, 3);
            __10VIMatrix44RC10VIMatrix44(to + 48, from + 48);
            copy_words(to, from, 112, 3);
            copy_words(to, from, 124, 5);
            copy_words(to, from, 144, 3);
            __10VIMatrix44RC10VIMatrix44(to + 160, from + 160);
            copy_words(to, from, 224, 5);
            copy_words(to, from, 244, 3);
            count = static_cast<int>(w[0]);
        }
    }
    if (DestroyPool__t8VIVector1Z13VIHSpriteNode(vector) < 0) return -1;
    if (w[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w[2])));
    w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    return 0;
}
}
