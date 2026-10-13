// Observed: Clear__t6VIList1Z7VIActor at 0x01224a10, 132 bytes. Offsets are bytes from the list; the nodes are
// the VIActor objects themselves, linked through the word at node+160.
//  1. Walk from the word at list+4: read the node's next word, call the VIActor destructor
//     with flag 2, __builtin_delete the node, then store the next node at list+4.
//  2. Store zero at list+8, list+0 and list+4; return 0.
// Inferred: head at +4, tail at +8, count at +0.
extern "C" {
void _$_7VIActor(void* actor, int flags);
void __builtin_delete(void* pointer);

static unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }

int Clear__t6VIList1Z7VIActor(void* list) {
    unsigned* l = static_cast<unsigned*>(list);
    for (unsigned node = l[1]; node != 0;) {
        unsigned next = at(node)[40];
        _$_7VIActor(at(node), 2);
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
