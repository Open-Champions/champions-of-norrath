// Observed: Add__t6VIPool1ZP9VISurfaceRCP9VISurface at 0x012121d8, 264 bytes. Offsets are bytes from the pool: +0 element
// count, +4 capacity, +8 element block, +12 free-chain head, +16 and +20 first and last live index
// (both -1 while empty). Entries are 12 bytes: word a at +0, word b at +4, word c at +8. The
// second argument is a reference to a pointer (passed as its address); the pointer it holds is
// what gets stored.
//  1. When the free-chain head is -1 the capacity is doubled (set to 1 when it is zero) and
//     Allocate__t6VIPool1ZP9VISurface(pool) is called; a negative result returns -1.
//  2. Let f be the free-chain head. Unless the entry address (block + 12 * f) is null, its word a
//     gets the pointer the reference holds. Its word c gets the last-live word (+20) in either case.
//  3. When the last-live word is not -1 the word b of the entry it names gets f; otherwise both
//     the first-live (+16) and last-live (+20) words get f.
//  4. The last-live word gets f. The free-chain head takes the entry's word b and that word
//     becomes -1; the count at +0 is incremented. The result is the last-live word, which is f.
// Inferred: the free chain runs through word b of free entries, the live entries form a list
// through b with word c as the back link, and word a is the stored value. The seven VIPool Add
// instances for pointer element types have identical bodies apart from the Allocate callee.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
int Allocate__t6VIPool1ZP9VISurface(void* pool);

int Add__t6VIPool1ZP9VISurfaceRCP9VISurface(void* pool, const void* const& item) {
    struct Entry { unsigned a, b, c; };
    unsigned* w = static_cast<unsigned*>(pool);
    if (static_cast<int>(w[3]) == -1) {
        w[1] = w[1] != 0 ? w[1] * 2 : 1;
        if (Allocate__t6VIPool1ZP9VISurface(pool) < 0) return -1;
    }
    Entry* e = reinterpret_cast<Entry*>(static_cast<unsigned long>(w[2])) + static_cast<int>(w[3]);
    if (e != 0) e->a = static_cast<unsigned>(reinterpret_cast<unsigned long>(item));
    e->c = w[5];
    if (static_cast<int>(w[5]) != -1) {
        reinterpret_cast<Entry*>(static_cast<unsigned long>(w[2]))[static_cast<int>(w[5])].b = w[3];
    } else {
        w[4] = w[3];
        w[5] = w[3];
    }
    w[5] = w[3];
    Entry* g = reinterpret_cast<Entry*>(static_cast<unsigned long>(w[2])) + static_cast<int>(w[5]);
    w[3] = g->b;
    g->b = static_cast<unsigned>(-1);
    w[0] = w[0] + 1;
    return static_cast<int>(w[5]);
}
}
