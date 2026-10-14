// Observed: Erase__t6VIPool1ZP15VIParticleMotifi at 0x01208690, 204 bytes (a weak template
// instantiation). Arguments: the pool and an entry index. Offsets are bytes from the pool; entries
// are 12 bytes each, starting at the address in pool+8; entry words are named by byte offset.
//  1. e = entry[index]; next = e+4.
//  2. If next is not -1, entry[next]+8 receives e+8; otherwise pool+20 receives e+8.
//  3. If e+8 is not -1, entry[e+8]+4 receives next; otherwise pool+16 receives next.
//  4. e+8 is set to -2, e+4 receives pool+12, pool+12 receives index and pool+0 is decremented.
//  5. The result is next when next is not -1, else 0x7fffffff.
// Inferred: pool+12 heads a chain of free entries linked through +4 (-2 in +8 marks a free entry);
// pool+16 and pool+20 are the first and last live entries of a chain doubly linked through +4
// (next) and +8 (previous); the constant is the end cursor, as in End__Ct6VIPool1ZP15VIParticleMotif.
// Unknown: the entry type beyond those two words.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* const norrath_host_arena_base;
static inline unsigned* entries_of(const unsigned* pool) {
    return reinterpret_cast<unsigned*>(norrath_host_arena_base + pool[2]);
}
#else
static inline unsigned* entries_of(const unsigned* pool) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned long>(pool[2]));
}
#endif

extern "C" {
int Erase__t6VIPool1ZP15VIParticleMotifi(void* pool, int index) {
    unsigned* p = static_cast<unsigned*>(pool);
    unsigned* e = entries_of(p) + 3 * index;
    unsigned next = e[1];
    if (next != 0xffffffffu) entries_of(p)[3 * next + 2] = e[2];
    else p[5] = e[2];
    unsigned* e2 = entries_of(p) + 3 * index;
    unsigned prev = e2[2];
    if (prev != 0xffffffffu) entries_of(p)[3 * prev + 1] = e2[1];
    else p[4] = e2[1];
    unsigned* e3 = entries_of(p) + 3 * index;
    unsigned head = p[3];
    e3[2] = static_cast<unsigned>(-2);
    e3[1] = head;
    p[3] = static_cast<unsigned>(index);
    p[0] = p[0] - 1;
    if (next == 0xffffffffu) return 0x7fffffff;
    return static_cast<int>(next);
}
}
