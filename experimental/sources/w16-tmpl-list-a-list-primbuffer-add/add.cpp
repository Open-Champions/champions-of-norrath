// Observed: Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer at 0x01211bf0, 144 bytes (per the symbol
// table). Offsets are bytes from the list: +0 element count, +4 head node, +8 tail node. The
// second argument is a reference to a pointer (passed as its address). A node is 12 bytes: the
// stored pointer at +0, the next link at +4 and the previous link at +8.
//  1. __builtin_new(12) is called. A null result returns -1 (and nothing else changes).
//  2. Otherwise the pointer the reference holds is stored in the node's first word.
//  3. The node's next link is set to 0 and its previous link to the list's tail. If the tail is
//     nonzero its next link is set to the node. The tail then becomes the node, and the head
//     also becomes the node when the head was zero. The count is incremented.
//  4. The result is the node address (the tail word, re-read).
// Inferred: this appends a node holding a VIPrimBuffer pointer to a doubly linked list and returns
// the new node.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
void* __builtin_new(unsigned size);

void* Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer(void* list, const void* const& item) {
    unsigned* w = static_cast<unsigned*>(list);
    unsigned* node = static_cast<unsigned*>(__builtin_new(12));
    if (node == 0) return reinterpret_cast<void*>(-1);
    const unsigned self = static_cast<unsigned>(reinterpret_cast<unsigned long>(node));
    node[0] = static_cast<unsigned>(reinterpret_cast<unsigned long>(item));
    node[1] = 0;
    node[2] = w[2];
    if (w[2] != 0) {
        reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(w[2]))[1] = self;
    }
    w[2] = self;
    if (w[1] == 0) w[1] = self;
    w[0] = w[0] + 1;
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w[2]));
}
}
