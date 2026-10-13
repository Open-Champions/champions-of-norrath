// Observed: Clear__t6VIList1ZP12VIPrimBuffer at 0x01211788, 100 bytes. Offsets are bytes from the list; each node is linked
// through the word at node+4.
//  1. Walk from the word at list+4: read the node's next word, __builtin_delete the node, then
//     store the next node at list+4. No destructor is called.
//  2. Store zero at list+8, list+0 and list+4; return 0.
// Inferred: head at +4, tail at +8, count at +0; the next/previous words at node+4 and node+8 match the
// unlink code of VIList<VIGUIPage*>::Erase. Template argument types come only from the mangled name.
extern "C" {
void __builtin_delete(void* pointer);

int Clear__t6VIList1ZP12VIPrimBuffer(void* list) {
    unsigned* l = static_cast<unsigned*>(list);
    for (unsigned node = l[1]; node != 0;) {
        unsigned next = static_cast<unsigned*>(reinterpret_cast<void*>(static_cast<unsigned long>(node)))[1];
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
