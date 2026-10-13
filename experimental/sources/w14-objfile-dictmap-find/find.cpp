// Observed: Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi at 0x0120a5a8, 124 bytes (a const member). With (this, keyRef):
// let I be the word at this+12 and B the word at this+8 (a base address). When I is zero the result is that of
// End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(this) (a tail call). Otherwise K is the unsigned word read through keyRef and the
// loop visits records of 28 bytes at B + 28 * I: with R the record, its word at +16 is compared with K as unsigned values; when
// K is below it, I becomes the word at R+8; when it is below K, I becomes the word at R+12; when equal, I is returned. The loop
// ends when I is zero, and then the result is again End(this).
// Inferred: a binary search tree stored in an array of 28-byte nodes, index 0 meaning "none": the root index is at +12, the node
// array at +8, a node's key at +16 and its left/right child indices at +8/+12. End is the "not found" iterator.
// Unknown: the remaining node words (+0..+4 and +20..+24 are not touched here), the table's size fields, and node ownership.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);

int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(const void* map, const unsigned* key) {
    const unsigned* words = static_cast<const unsigned*>(map);
    unsigned index = words[3];
    if (index == 0) return End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map);
    unsigned wanted = *key;
    unsigned char* base = at(words[2]);
    do {
        const unsigned* node = reinterpret_cast<const unsigned*>(base + 28 * index);
        unsigned found = node[4];
        if (wanted < found) index = node[2];
        else if (found < wanted) index = node[3];
        else return static_cast<int>(index);
    } while (index != 0);
    return End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map);
}
}
