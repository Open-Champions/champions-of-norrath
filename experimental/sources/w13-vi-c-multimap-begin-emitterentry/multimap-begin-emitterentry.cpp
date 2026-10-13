// Observed: Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry at 0x011f4df8, 72 bytes (a weak
// template instantiation). With the map as this: the word at +12 is an entry index (zero
// means none) and the entry table is the pointer at +8, with 112-byte entries whose word at
// +8 is another entry index. The result is the end of the chain obtained by following that
// +8 word from the index at +12: zero if the word at +12 is zero, otherwise the last index
// whose own +8 word is zero. Nothing is written.
// Inferred: the +12 word is the root of the index-linked tree that Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi
// walks (+8 of an entry being its left child), so this is the first entry in order.
// Unknown: member names.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" const unsigned char* norrath_host_pointer(unsigned word);
#define NORRATH_POINTER_AT(address) norrath_host_pointer(*reinterpret_cast<const unsigned*>(address))
#else
#define NORRATH_POINTER_AT(address) (*reinterpret_cast<const unsigned char* const*>(address))
#endif

extern "C" {
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    int node = *reinterpret_cast<const int*>(bytes + 12);
    if (node == 0) return 0;
    const unsigned char* table = NORRATH_POINTER_AT(bytes + 8);
    while (*reinterpret_cast<const int*>(table + node * 112 + 8) != 0)
        node = *reinterpret_cast<const int*>(table + node * 112 + 8);
    return node;
}
}
