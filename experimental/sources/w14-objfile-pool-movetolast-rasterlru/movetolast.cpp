// Observed: MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi at 0x01210ae0, 240 bytes. Arguments: the pool and an entry index I. Offsets
// are bytes from the pool; entries are 24 bytes each, starting at the address in pool+8, and only the entry words at +16 and +20
// are used (called A and B here). All index values are compared with -1 as "none".
//  1. If A(I) is not -1, B(A(I)) receives B(I); otherwise pool+20 receives B(I).
//  2. If B(I) is not -1, A(B(I)) receives A(I); otherwise pool+16 receives A(I).
//  3. B(I) receives the word at pool+20. If that word (read again) is not -1, A(of that entry) receives I; otherwise both pool+20 and
//     pool+16 receive I.
//  4. A(I) is set to -1 and pool+20 to I. Nothing is returned.
// Inferred: entries form a doubly linked list: A is the link towards the pool+20 end and B the link away from it, so steps 1 and 2
// unlink entry I and steps 3 and 4 make it the new pool+20 end (the most recently touched one); pool+16 is the other end. The base
// address in pool+8 is read again before each group of entry accesses, as in the image.
// Unknown: which end is the "last" of the name, the entry words outside +16/+20, and the pool's other fields.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static inline unsigned* entry(const unsigned* pool, unsigned index) {
    return reinterpret_cast<unsigned*>(at(pool[2]) + 24 * index);
}

extern "C" {
void MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(void* pool, int index) {
    unsigned* p = static_cast<unsigned*>(pool);
    unsigned* e = entry(p, index);
    unsigned a = e[4];
    if (a != 0xffffffffu) entry(p, a)[5] = e[5];
    else p[5] = e[5];
    e = entry(p, index);
    unsigned b = e[5];
    if (b != 0xffffffffu) entry(p, b)[4] = e[4];
    else p[4] = e[4];
    entry(p, index)[5] = p[5];
    if (p[5] != 0xffffffffu) entry(p, p[5])[4] = static_cast<unsigned>(index);
    else {
        p[5] = static_cast<unsigned>(index);
        p[4] = static_cast<unsigned>(index);
    }
    entry(p, index)[4] = 0xffffffffu;
    p[5] = static_cast<unsigned>(index);
}
}
