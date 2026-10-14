// Observed: Erase__t6VIPool1ZP16VIStaticLightingi at 0x01223188, 204 bytes. Arguments: the pool and an entry index. Offsets are bytes
// from the pool; entries are 12 bytes each, starting at the address in pool+8, and entry words are
// named here by their byte offset inside the entry.
//  1. e = entry[index]; next = e+4.
//  2. If next is not -1, entry[next]+8 receives e+8; otherwise pool+20 receives e+8.
//  3. If e+8 is not -1, entry[e+8]+4 receives next; otherwise pool+16 receives next.
//  4. e+8 is set to -2, e+4 receives pool+12, pool+12 receives index and pool+0 is decremented.
//  5. The result is next when next is not -1, else 0x7fffffff (the constant is inline; no call is made).
// Inferred: pool+12 heads a chain of free entries linked through +4 (-2 in +8 marking a free entry), and
// pool+16/pool+20 are the first and last live entries of a chain doubly linked through +4 and +8.
// Unknown: the entry type layout beyond those two words, and what pool+4 holds.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Erase__t6VIPool1ZP16VIStaticLightingi(void* pool, int index) {
    unsigned* p = static_cast<unsigned*>(pool);
    unsigned* e = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2])) + 3 * index;
    unsigned next = e[1];
    if (next != 0xffffffffu) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2]))[3 * next + 2] = e[2];
    else p[5] = e[2];
    unsigned* e2 = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2])) + 3 * index;
    unsigned prev = e2[2];
    if (prev != 0xffffffffu) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2]))[3 * prev + 1] = e2[1];
    else p[4] = e2[1];
    unsigned* e3 = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2])) + 3 * index;
    unsigned head = p[3];
    e3[2] = static_cast<unsigned>(-2);
    e3[1] = head;
    p[3] = static_cast<unsigned>(index);
    p[0] = p[0] - 1;
    return next == 0xffffffffu ? 0x7fffffff : static_cast<int>(next);
}
}
