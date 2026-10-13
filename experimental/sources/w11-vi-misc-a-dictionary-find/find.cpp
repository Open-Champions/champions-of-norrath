// Observed: Find__12VIDictionaryUiR14VIResourceTypeRi at 0x010bf2e0 is 140 bytes.
// The unsigned key is stored on the stack and passed by address to the map's
// Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(this, &key), whose result is
// an index; End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(this) gives the end
// index. If they are equal it returns 0 and writes nothing. Otherwise the entry is
// at (word 8 of the object) + 20 + 28 * index; its word at 0 is stored through the
// third argument (the resource type reference) and its word at 4 through the fourth
// (the int reference), and it returns 1. The entry stride is 28 bytes.
// Unknown: the other five words of each 28-byte entry and the 20-byte header.
extern "C" {
int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(void* map, const unsigned* key);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);

int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned key, int* type, int* value) {
    int index = Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(dictionary, &key);
    if (index == End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(dictionary)) return 0;
    const int* entry = reinterpret_cast<const int*>(
        *reinterpret_cast<char**>(static_cast<char*>(dictionary) + 8) + 20 + 28 * index);
    *value = entry[1];
    *type = entry[0];
    return 1;
}
}
