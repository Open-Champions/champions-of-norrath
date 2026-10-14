// Observed: AddTail__7VILListP7VILNode at 0x01072810, 36 bytes. With (list, node): when node is null nothing happens.
// Otherwise, with T the word at list+8: node+0 = list+4, node+4 = T, the word at T+0 = node, and list+8 = node.
// Inferred: appends the node at the tail of the list set up by Init__7VILList (T is the previous tail, or the list header
// itself when the list is empty, in which case the head link at list+0 takes the node). Nodes read as {next, previous}.
// Unknown: ownership of the node, duplicate-insert handling (none observed), and members beyond the node's first two words.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
static inline unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
static inline unsigned address_of(const void* pointer) { return norrath_host_address(pointer); }
#else
static inline unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
static inline unsigned address_of(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer)); }
#endif

extern "C" {
void AddTail__7VILListP7VILNode(void* list, void* node) {
    if (node == 0) return;
    unsigned* l = static_cast<unsigned*>(list);
    unsigned* n = static_cast<unsigned*>(node);
    unsigned* tail = at(l[2]);
    n[0] = address_of(l) + 4;
    n[1] = address_of(tail);
    tail[0] = address_of(n);
    l[2] = address_of(n);
}
}
