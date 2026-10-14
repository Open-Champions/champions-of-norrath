// Observed: Clear__t6VIList1ZP12VIPrimBuffer at 0x01211788, 100 bytes. Offsets are bytes from the list; each node is linked
// through the word at node+4.
//  1. Walk from the word at list+4: read the node's next word, __builtin_delete the node, then
//     store the next node at list+4. No destructor is called and the node count is not read.
//  2. Store zero at list+8, list+0 and list+4; return 0.
// Inferred: head at +4, tail at +8, count at +0, as in the other VIList instances. Template argument
// types come only from the mangled name.
// Unknown: the node layout beyond the link word.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void __builtin_delete(void* pointer);

int Clear__t6VIList1ZP12VIPrimBuffer(void* list) {
    unsigned* l = static_cast<unsigned*>(list);
    for (unsigned node = l[1]; node != 0;) {
        unsigned next = static_cast<unsigned*>(at(node))[1];
        __builtin_delete(at(node));
        l[1] = next;
        node = next;
    }
    l[2] = 0;
    l[0] = 0;
    l[1] = 0;
    return 0;
}
}
