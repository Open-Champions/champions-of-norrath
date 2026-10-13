// Observed: Clear__t6VIList1ZQ27VIScene12VISceneOccup at 0x01224a98, 100 bytes. Offsets are bytes from the list; each node
// is linked through the word at node+24.
//  1. Walk from the word at list+4: read the node's next word, __builtin_delete the node, then
//     store the next node at list+4. No destructor is called.
//  2. Store zero at list+8, list+0 and list+4; return 0.
// Inferred: head at +4, tail at +8, count at +0.
extern "C" {
void __builtin_delete(void* pointer);

static unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }

int Clear__t6VIList1ZQ27VIScene12VISceneOccup(void* list) {
    unsigned* l = static_cast<unsigned*>(list);
    for (unsigned node = l[1]; node != 0;) {
        unsigned next = at(node)[6];
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
