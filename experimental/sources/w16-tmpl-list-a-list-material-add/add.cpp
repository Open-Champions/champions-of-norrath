// Observed: Add__t6VIList1Z10VIMaterial at 0x01211d10, 140 bytes (per the symbol table). Offsets are bytes from the
// list: +0 element count, +4 head node, +8 tail node. A node is 496 bytes with its next link at +480 and
// its previous link at +484.
//  1. __builtin_new(496) is called. A null result returns -1 (and nothing else changes).
//  2. Otherwise __10VIMaterial(node) is called on the new block.
//  3. The node's next link is set to 0 and its previous link to the list's tail. If the tail is
//     nonzero its next link is set to the node. The tail then becomes the node, and the head
//     also becomes the node when the head was zero. The count is incremented.
//  4. The result is the node address (the tail word, re-read).
// Inferred: this appends a default-constructed element to a doubly linked list whose nodes embed
// the element, and returns the new node. The element's own fields are not touched here.
// Unknown: the element's members, and the template parameters beyond the mangled name.
extern "C" {
void* __builtin_new(unsigned size);
void __10VIMaterial(void* self);

void* Add__t6VIList1Z10VIMaterial(void* list) {
    unsigned* w = static_cast<unsigned*>(list);
    unsigned char* node = static_cast<unsigned char*>(__builtin_new(496));
    if (node != 0) __10VIMaterial(node);
    if (node == 0) return reinterpret_cast<void*>(-1);
    unsigned* next = reinterpret_cast<unsigned*>(node + 480);
    unsigned* previous = reinterpret_cast<unsigned*>(node + 484);
    const unsigned self = static_cast<unsigned>(reinterpret_cast<unsigned long>(node));
    *next = 0;
    *previous = w[2];
    if (w[2] != 0) {
        *reinterpret_cast<unsigned*>(
            reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(w[2])) + 480) = self;
    }
    w[2] = self;
    if (w[1] == 0) w[1] = self;
    w[0] = w[0] + 1;
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w[2]));
}
}
