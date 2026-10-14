// Observed: Clear__t6VIList1Z10VIMaterial at 0x01211980, 188 bytes. Offsets are bytes from the list; nodes are linked through the word
// at node+480.
//  1. Walk from the word at list+4. For each node the next word (node+480) is read first. Unless
//     node+48 is zero (a 32-bit wrap of the address), three 144-byte members starting at node+48 are
//     torn down last to first: _$_10VIMatrix33(member+16, 2) for member offsets 336, 192 and 48.
//     Then __builtin_delete(node) and the next node is stored at list+4.
//  2. Store zero at list+8, list+0 and list+4; return 0.
// Inferred: this is VIMaterial's destructor inlined (a member array of three 144-byte elements, each
// holding a VIMatrix33 at +16); the node's own destructor is not called as a function. The meaning
// of the other node fields is unknown. The null-member test cannot be reached by a real node.
extern "C" {
void _$_10VIMatrix33(void* matrix, int flags);
void __builtin_delete(void* pointer);

int Clear__t6VIList1Z10VIMaterial(void* list) {
    unsigned* l = static_cast<unsigned*>(list);
    for (unsigned node = l[1]; node != 0;) {
        unsigned next = static_cast<unsigned*>(reinterpret_cast<void*>(static_cast<unsigned long>(node)))[120];
        if (node + 48 != 0) {
            unsigned member = node + 480;
            while (member != node + 48) {
                member -= 144;
                _$_10VIMatrix33(reinterpret_cast<void*>(static_cast<unsigned long>(member + 16)), 2);
            }
        }
        __builtin_delete(reinterpret_cast<void*>(static_cast<unsigned long>(node)));
        l[1] = next;
        node = next;
    }
    l[2] = 0;
    l[0] = 0;
    l[1] = 0;
    return 0;
}
}
