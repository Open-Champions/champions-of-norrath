// Observed: DestroyPool__t6VIPool1ZP8VISprite at 0x01224298, 104 bytes. Offsets are bytes from the pool; entries are 12 bytes.
//  1. The cursor is the word at +16, or 0x7fffffff when that word is -1. If the cursor is 0x7fffffff the
//     function returns 0 at once.
//  2. Otherwise, with the entry block taken from the word at +8, the cursor repeatedly becomes the word at +4 of
//     entry [cursor] (12-byte steps), or 0x7fffffff when that word is -1, until it is 0x7fffffff.
//  3. Returns 0. Nothing is written or freed.
// Inferred: this is the Begin/End/Next walk of the other pool instances compiled inline: the word at +16 is the first
// index, +8 the entry block, word b of each entry the next index, 0x7fffffff the end marker. The walk does nothing per entry, so it
// is what remains of a per-entry teardown. The template argument comes only from the mangled name.
extern "C" {
int DestroyPool__t6VIPool1ZP8VISprite(void* pool) {
    struct Entry { unsigned a; int next; unsigned c; };
    const unsigned* words = static_cast<const unsigned*>(pool);
    int cursor = static_cast<int>(words[4]);
    if (cursor == -1) cursor = 0x7fffffff;
    if (cursor != 0x7fffffff) {
        const Entry* entries = reinterpret_cast<const Entry*>(static_cast<unsigned long>(words[2]));
        do {
            int next = entries[cursor].next;
            cursor = next == -1 ? 0x7fffffff : next;
            __asm__ volatile("" : "+r"(cursor));
        } while (cursor != 0x7fffffff);
    }
    return 0;
}
}
