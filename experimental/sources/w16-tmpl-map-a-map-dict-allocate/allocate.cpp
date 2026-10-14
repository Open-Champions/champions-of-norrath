// Observed: Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a758 is 396 bytes. The
// map holds a used count at +0, a capacity at +4, the element array base at +8 (elements are 28
// bytes), the root index at +12 and the free-chain head at +16. With (map) it returns 0 at once
// when the capacity is zero. When there is no array it asks __builtin_vec_new for capacity * 28
// bytes, returns -1 if that is null, stores the block as the base and increments the used count.
// When an array exists it asks for capacity * 28 bytes (-1 on null), copies elements 1 up to (not
// including) the used count from the old array into the new block (28 bytes each, re-reading the
// count every pass), walks the map once with Begin/Next/End (a loop with no other effect), frees
// the old array with __builtin_vec_delete (when it is non-null) and installs the new block. Both
// paths then clear element 0's colour, parent, left and right words and call
// LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, max(used count, 1)); it
// returns 0 when that result is not negative and -1 otherwise.
// Inferred: the Begin/Next/End walk is a destructor pass over the old elements, which are plain
// data here; element 0 is the nil element, so a nonempty array is grown with its used count
// already including it. Unknown: template parameters beyond the mangled name.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);
int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void* map, int index);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);
int LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int from);

static inline unsigned* word(void* map, unsigned offset) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(map) + offset);
}

int Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map) {
    const unsigned capacity = *word(map, 4);
    if (capacity == 0) return 0;
    if (*word(map, 8) == 0) {
        void* block = __builtin_vec_new(capacity * 28);
        if (block == nullptr) return -1;
        *word(map, 8) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
        *word(map, 0) += 1;
    } else {
        unsigned* block = static_cast<unsigned*>(__builtin_vec_new(capacity * 28));
        if (block == nullptr) return -1;
        for (int i = 1; i < static_cast<int>(*word(map, 0)); ++i) {
            const unsigned* from = reinterpret_cast<const unsigned*>(
                static_cast<__UINTPTR_TYPE__>(*word(map, 8)) + i * 28);
            unsigned* to = block + i * 7;
            for (int w = 0; w < 7; ++w) to[w] = from[w];
        }
        for (int it = Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map);
             it != End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map);
             it = Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, it)) {
        }
        if (*word(map, 8) != 0)
            __builtin_vec_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*word(map, 8))));
        *word(map, 8) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
    }
    unsigned* nil = reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(*word(map, 8)));
    nil[2] = 0;
    nil[0] = 0;
    nil[1] = 0;
    nil[3] = 0;
    const int used = static_cast<int>(*word(map, 0));
    return LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, used < 1 ? 1 : used) >= 0 ? 0 : -1;
}
}
