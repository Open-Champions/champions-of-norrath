// Observed: Erase__t6VIList1ZP9VIGUIPagei at 0x01209628, 136 bytes. Arguments: the list and the
// address of one node (an int). Offsets are bytes from the list or from the node.
//  1. next = node+4. If next is nonzero, next+8 receives node+8; otherwise list+8 does.
//  2. prev = node+8. If prev is nonzero, prev+4 receives node+4; otherwise list+4 does.
//  3. __builtin_delete(node).
//  4. list+0 is decremented. The result is next when it is nonzero, else 0x7fffffff.
// Inferred: a doubly linked list with head at list+4, tail at list+8, count at list+0; the
// result is the iterator after the erased node.
extern "C" {
void __builtin_delete(void* pointer);

static unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }

int Erase__t6VIList1ZP9VIGUIPagei(void* list, int node) {
    unsigned* l = static_cast<unsigned*>(list);
    unsigned* n = at(static_cast<unsigned>(node));
    unsigned next = n[1];
    if (next != 0) at(next)[2] = n[2];
    else l[2] = n[2];
    unsigned prev = n[2];
    if (prev != 0) at(prev)[1] = n[1];
    else l[1] = n[1];
    __builtin_delete(n);
    l[0] = l[0] - 1;
    return next != 0 ? static_cast<int>(next) : 0x7fffffff;
}
}
