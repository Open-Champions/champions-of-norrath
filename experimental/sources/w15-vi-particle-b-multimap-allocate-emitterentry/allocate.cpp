// Observed: Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry at 0x011f4a08, 376 bytes (a weak template
// instantiation). Offsets are bytes from the map; nodes are 112-byte records. Let capacity be the
// word at +4 and count the word at +0.
//  1. Capacity zero: return 0 with nothing changed.
//  2. No node array yet (+8 is zero): the array is __builtin_vec_new(capacity * 112). A null result
//     returns -1; otherwise the pointer is stored at +8 and the count is incremented by one.
//  3. Otherwise a new array of the same size is requested (null returns -1). For each of the count
//     existing nodes the new node, if its address is nonzero, receives the words at +0, +4, +8,
//     +12, +16, +32 and +36 of the old node and then
//     __10VIMatrix44RC10VIMatrix44(new + 48, old + 48) runs on it; the other bytes are left alone.
//     Then DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(map) runs (a negative result returns -1);
//     the old array, if any, goes to __builtin_vec_delete and the new pointer is stored at +8.
//     The count is not changed in this path.
//  4. Common tail: node 0 gets zero at +0, +4, +8 and +12. LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi
//     is called with the count, or 1 when the count is below one; a negative result returns -1,
//     otherwise 0 is returned.
// Inferred: this grows (or first creates) the node array of the map and rebuilds the free chain;
// node 0 is the null node. Unknown: the node fields other than the matrix member, and the member names.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* const norrath_host_arena_base;
static inline unsigned char* array_of(unsigned word) { return norrath_host_arena_base + word; }
static inline unsigned to_word(void* p) { return static_cast<unsigned>(static_cast<unsigned char*>(p) - norrath_host_arena_base); }
#else
static inline unsigned char* array_of(unsigned word) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word)); }
static inline unsigned to_word(void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
#endif

extern "C" {
void* __builtin_vec_new(unsigned long size);
void __builtin_vec_delete(void* pointer);
int DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map);
int LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int count);
void __10VIMatrix44RC10VIMatrix44(void* to, const void* from);

int Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map) {
    unsigned* words = static_cast<unsigned*>(map);
    if (words[1] == 0) return 0;
    if (words[2] == 0) {
        unsigned char* fresh = static_cast<unsigned char*>(__builtin_vec_new(words[1] * 112UL));
        if (fresh == 0) return -1;
        words[2] = to_word(fresh);
        words[0] = words[0] + 1;
    } else {
        unsigned char* fresh = static_cast<unsigned char*>(__builtin_vec_new(words[1] * 112UL));
        if (fresh == 0) return -1;
        for (int i = 0; i < static_cast<int>(words[0]); ++i) {
            unsigned char* to = fresh + 112 * i;
            if (to == 0) continue;
            const unsigned char* from = array_of(words[2]) + 112 * i;
            for (int off = 0; off < 20; off += 4)
                *reinterpret_cast<unsigned*>(to + off) = *reinterpret_cast<const unsigned*>(from + off);
            *reinterpret_cast<unsigned*>(to + 32) = *reinterpret_cast<const unsigned*>(from + 32);
            *reinterpret_cast<unsigned*>(to + 36) = *reinterpret_cast<const unsigned*>(from + 36);
            __10VIMatrix44RC10VIMatrix44(to + 48, from + 48);
        }
        if (DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(map) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(array_of(words[2]));
        words[2] = to_word(fresh);
    }
    unsigned* node0 = reinterpret_cast<unsigned*>(array_of(words[2]));
    node0[2] = 0;
    node0[0] = 0;
    node0[1] = 0;
    node0[3] = 0;
    int count = static_cast<int>(words[0]);
    if (LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, count < 1 ? 1 : count) < 0) return -1;
    return 0;
}
}
