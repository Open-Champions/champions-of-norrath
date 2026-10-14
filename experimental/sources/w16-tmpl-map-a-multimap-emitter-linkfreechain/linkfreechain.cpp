// Observed: LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4fa8 is 172 bytes. With
// (map, from) it stores from as the free-chain head at map+16. If from is not below the capacity
// at map+4 it instead stores -1 as the head and returns. Otherwise it chains the 112-byte elements
// (array base at map+8) from index from up to the last one: each element below capacity - 1 gets
// +4 = its index + 1 and +8 = -2, and the last element gets +4 = -1 and +8 = -2. It returns 0.
// Inferred: builds a singly linked list of free elements through +4, with -2 in the left link
// marking a free element; signed index and capacity compares follow the instruction sequence.
// Unknown: template parameters beyond the mangled name.
extern "C" {
static inline unsigned* word(void* map, unsigned offset) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(map) + offset);
}
static inline unsigned* field(void* map, int index, unsigned offset) {
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*word(map, 8)));
    return reinterpret_cast<unsigned*>(elements + index * 112 + offset);
}

int LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int from) {
    *word(map, 16) = static_cast<unsigned>(from);
    if (from >= static_cast<int>(*word(map, 4))) {
        *word(map, 16) = 0xffffffffu;
        return 0;
    }
    int index = from;
    while (index < static_cast<int>(*word(map, 4)) - 1) {
        *field(map, index, 4) = static_cast<unsigned>(index + 1);
        *field(map, index, 8) = 0xfffffffeu;
        ++index;
    }
    *field(map, index, 4) = 0xffffffffu;
    *field(map, index, 8) = 0xfffffffeu;
    return 0;
}
}
