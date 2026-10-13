// Observed: Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a758, 396 bytes. Offsets are bytes from the map; entries are 28 bytes.
//  1. Capacity is the word at +4; zero returns 0 at once.
//  2. When the storage word at +8 is zero: __builtin_vec_new(capacity * 28) is stored at +8 (before the
//     result is tested); a null result returns -1; otherwise the word at +0 is incremented.
//  3. Otherwise a new block of the same size is requested (null returns -1, nothing stored). For slots 1
//     up to word[0]-1 the 28 bytes are copied to the same slot of the new block unless that address is
//     null; slot 0 is not copied. Then Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry, Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi and End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry walk the map with nothing
//     done per entry (End is called again every time); the old block, if the word at +8 is still nonzero,
//     goes to __builtin_vec_delete, and the new block is stored at +8. The word at +0 is not changed.
//  4. In both cases slot 0 of the block at +8 then gets zero in its words 0, 1, 2 and 3, and
//     LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, n) runs with n = word[0], or 1 when word[0] is below 1. A negative result
//     returns -1, otherwise 0.
// Inferred: slot 0 acts as a header/sentinel entry that is counted in +0 but never copied. The walk is
// the body of DestroyPool compiled inline.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);
int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void* map, int cursor);
int LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int count);

int Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map) {
    struct Entry { unsigned w[7]; };
    unsigned* w = static_cast<unsigned*>(map);
    int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* block = __builtin_vec_new(capacity * 28);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
        if (block == 0) return -1;
        w[0] = w[0] + 1;
    } else {
        Entry* fresh = static_cast<Entry*>(__builtin_vec_new(capacity * 28));
        if (fresh == 0) return -1;
        for (int i = 1; i < static_cast<int>(w[0]); ++i) {
            Entry* to = fresh + i;
            if (to != 0) *to = static_cast<Entry*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])))[i];
        }
        int cursor = Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map);
        while (cursor != End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map)) cursor = Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, cursor);
        if (w[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])));
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    }
    unsigned* head = static_cast<unsigned*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])));
    head[2] = 0;
    head[0] = 0;
    static_cast<unsigned*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])))[1] = 0;
    static_cast<unsigned*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])))[3] = 0;
    int count = static_cast<int>(w[0]);
    return LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, count < 1 ? 1 : count) < 0 ? -1 : 0;
}
}
