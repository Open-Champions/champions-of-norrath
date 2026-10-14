// Observed: Find__12VIDictionaryUiR14VIResourceTypeRi at 0x010bf2e0, 144 bytes. With (this, key, typeOut, valueOut): copies key to a
// local word and calls Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(this, &local), then
// End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(this). When the two results are equal it returns 0 and writes nothing. Otherwise,
// with B the word at this+8 and I the found result, it reads the two words at B + 28 * I + 20 (first W0, then W1 at +4), stores W1
// into *valueOut and W0 into *typeOut, and returns 1.
// Inferred: a lookup of a 32-bit key in the dictionary's map; each entry holds a resource type word (+20 of the 28-byte node)
// followed by an int value, handed back through the two references. The result is a found flag.
// Unknown: what the value is (index, id or address), the type's meaning, and node words other than those two.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(const void* map, const unsigned* key);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map);

int Find__12VIDictionaryUiR14VIResourceTypeRi(void* self, unsigned key, unsigned* typeOut, int* valueOut) {
    unsigned local = key;
    const unsigned* words = static_cast<const unsigned*>(self);
    int found = Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(self, &local);
    if (found == End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(self)) return 0;
    const unsigned* entry = reinterpret_cast<const unsigned*>(at(words[2]) + 28 * found + 20);
    *valueOut = static_cast<int>(entry[1]);
    *typeOut = entry[0];
    return 1;
}
}
