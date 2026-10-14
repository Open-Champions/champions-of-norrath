// Observed: FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x0120a710, 68 bytes. Arguments: the map and a node index I. The
// node address is the word at map+8 plus 28 * I. Steps in order: node+4 receives the word at map+16; the word at map+8 is read again
// and node+8 (computed from the fresh base) receives -2; map+16 receives I; map+0 is decremented by one. The result is 0.
// Inferred: pushes the node on a free chain headed at map+16 (chained through the node's +4 word) and marks it with -2 at +8;
// map+0 is the live node count. Unknown: the -2 marker's reader and whether the base can move between the two stores (it is re-read
// as the image does).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int index) {
    unsigned* m = static_cast<unsigned*>(map);
    unsigned offset = 28 * static_cast<unsigned>(index);
    unsigned head = m[4];
    reinterpret_cast<unsigned*>(at(m[2]) + offset)[1] = head;
    reinterpret_cast<unsigned*>(at(m[2]) + offset)[2] = static_cast<unsigned>(-2);
    m[4] = static_cast<unsigned>(index);
    m[0] = m[0] - 1;
    return 0;
}
}
