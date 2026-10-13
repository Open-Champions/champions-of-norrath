// Observed: Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x0120a4b8, 92 bytes. Calls the map's Clear with the map; a
// negative result returns -1 straight away. Otherwise stores the integer argument plus one at
// map+4 and calls Allocate for the same map type; the result is -1 when that is negative, else 0.
// Inferred: the argument is a requested count and the stored value is one larger (for example
// a spare slot); the mangled name is the only evidence for the key and value types.
extern "C" {
int Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);
int Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map);

int Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int count) {
    if (Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map) < 0) return -1;
    *(reinterpret_cast<int*>(map) + 1) = count + 1;
    return Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(map) < 0 ? -1 : 0;
}
}
