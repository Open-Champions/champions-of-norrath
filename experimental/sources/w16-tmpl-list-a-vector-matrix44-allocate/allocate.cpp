// Observed: Allocate__t8VIVector1Z10VIMatrix44 at 0x012031a8. The symbol table says 224 bytes; the
// code runs to 0x12032e0 (a 320-byte span including the closing delay slot). Offsets are bytes from
// the vector: +0 element count, +4 capacity, +8 element block; elements are 64 bytes.
//  1. A capacity of zero returns 0 at once.
//  2. When the block word is zero: __builtin_vec_new(capacity * 64) is stored at +8 before the
//     result is tested; the result is 0, or -1 for a null block.
//  3. Otherwise a fresh block is requested (null returns -1, nothing stored). For each index below
//     the count (the count word is read again after every copy) the slot in the fresh block is
//     copy-constructed from the same slot of the old block with __10VIMatrix44RC10VIMatrix44,
//     skipped when the slot address is null.
//  4. The old elements are then destroyed one at a time from index 0 upward, with
//     _$_10VIMatrix44(element, 2), as long as the iteration index is not the last one
//     (count - 1, with the count read before each call); an empty vector destroys nothing. The
//     old block word is read again on every call.
//  5. If the old block word is nonzero it goes to __builtin_vec_delete; the fresh block is stored at
//     +8 and the result is 0.
// Inferred: this is the grow step of VIVector<VIMatrix44>. The element loop of step 4 is an
// inlined iterator whose past-the-end value is 0x7fffffff (the same value the pool Begin
// functions hand back); only that sentinel can end it.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
void* __10VIMatrix44RC10VIMatrix44(void* destination, const void* source);
void _$_10VIMatrix44(void* self, int flags);

static inline unsigned char* block(const unsigned* w) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(w[2]));
}

int Allocate__t8VIVector1Z10VIMatrix44(void* vector) {
    unsigned* w = static_cast<unsigned*>(vector);
    const int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* fresh = __builtin_vec_new(static_cast<unsigned>(capacity) << 6);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
        return fresh == 0 ? -1 : 0;
    }
    unsigned char* fresh = static_cast<unsigned char*>(__builtin_vec_new(static_cast<unsigned>(capacity) << 6));
    if (fresh == 0) return -1;
    int count = static_cast<int>(w[0]);
    for (int index = 0; index < count;) {
        unsigned char* slot = fresh + index * 64;
        ++index;
        if (slot != 0) {
            __10VIMatrix44RC10VIMatrix44(slot, block(w) + (index - 1) * 64);
            count = static_cast<int>(w[0]);
        }
    }
    int iterator = count != 0 ? 0 : 0x7fffffff;
    while (iterator != 0x7fffffff) {
        const int index = iterator;
        const int last = count - 1;
        iterator = index != last ? index + 1 : 0x7fffffff;
        _$_10VIMatrix44(block(w) + index * 64, 2);
        count = static_cast<int>(w[0]);
    }
    if (w[2] != 0) __builtin_vec_delete(block(w));
    w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    return 0;
}
}
