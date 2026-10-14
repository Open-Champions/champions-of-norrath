// Observed: Add__t6VIPool1ZP8VISpriteRCP8VISprite at 0x01222e10, 264 bytes. Offsets are bytes from the pool; entries are 12 bytes (words a, b, c)
// in the block at +8. The second argument is a reference to a pointer (passed as its address); the pointer is what gets stored.
//  1. When the word at +12 is -1: the word at +4 is doubled (set to 1 when it is zero) and stored,
//     then Allocate__t6VIPool1ZP8VISprite(pool) is called; a negative result returns -1.
//  2. Let f be the word at +12. Unless the entry address (block + f*12) is null, its word a gets the
//     pointer the reference holds. Its word c gets the pool word at +20 in either case.
//  3. When the word at +20 is not -1, the word b of entry [word at +20] gets f; otherwise the words at
//     +16 and +20 both get f.
//  4. The word at +20 gets f. The entry's word b is moved to +12 and replaced by -1, and the word at +0
//     is incremented. Result: the word at +20, which is f.
// Inferred: +12 heads a free chain linked through word b, +16/+20 are the first and last live index, word c
// is a back link and word a the stored value. These names stay in comments only.
extern "C" {
int Allocate__t6VIPool1ZP8VISprite(void* pool);

int Add__t6VIPool1ZP8VISpriteRCP8VISprite(void* pool, const void* const& item) {
    struct Entry { unsigned a, b, c; };
    unsigned* w = static_cast<unsigned*>(pool);
    if (static_cast<int>(w[3]) == -1) {
        w[1] = w[1] != 0 ? w[1] * 2 : 1;
        if (Allocate__t6VIPool1ZP8VISprite(pool) < 0) return -1;
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
