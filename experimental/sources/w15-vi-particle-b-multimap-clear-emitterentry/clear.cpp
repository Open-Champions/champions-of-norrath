// Observed: Clear__t10VIMultiMap2ZfZ14VIEmitterEntry at 0x01226698, 116 bytes (a weak template
// instantiation). Offsets are bytes from the map.
//  1. When the word at +8 is nonzero: call DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(map). A
//     negative result returns -1 at once with nothing else changed. Otherwise +8 is read again and,
//     if still nonzero, passed to __builtin_vec_delete.
//  2. Stores zero at +0, +4, +8 and +12 and -1 at +16, and returns 0. These stores also run when
//     +8 was zero at entry.
// Inferred: +8 is the node array (112-byte nodes), +4 the capacity, +0 the node count, +12 the root
// index and +16 the head of the free chain. Unknown: the member names.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* const norrath_host_arena_base;
static inline void* array_of(unsigned word) { return norrath_host_arena_base + word; }
#else
static inline void* array_of(unsigned word) { return reinterpret_cast<void*>(static_cast<unsigned long>(word)); }
#endif

extern "C" {
int DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map);
void __builtin_vec_delete(void* pointer);

int Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map) {
    unsigned* words = static_cast<unsigned*>(map);
    if (words[2] != 0) {
        if (DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(map) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(array_of(words[2]));
    }
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[4] = static_cast<unsigned>(-1);
    words[3] = 0;
    return 0;
}
}
