// Observed: Erase__t6VIList1ZP10VIFlatFilei at 0x011f3c00, 136 bytes. Arguments: the list and the
// address of one node. Offsets are bytes from the list or from the node; the node's link words are at
// +4 (next) and +8 (previous) in this instantiation.
//  1. next = node+4. If next is nonzero, next+8 receives node+8; otherwise list+8 does.
//  2. prev = node+8. If prev is nonzero, prev+4 receives node+4; otherwise list+4 does.
//  3. __builtin_delete(node).
//  4. list+0 is decremented. The result is next when it is nonzero, else 0x7fffffff (inline constant; no call).
// Inferred: a doubly linked list with head at list+4, tail at list+8, count at list+0; the
// result is the iterator after the erased node.
// Unknown: the rest of the node.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void __builtin_delete(void* pointer);

int Erase__t6VIList1ZP10VIFlatFilei(void* list, unsigned node) {
    unsigned* l = static_cast<unsigned*>(list);
    unsigned* n = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(node));
    unsigned next = n[1];
    if (next != 0) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(next))[2] = n[2];
    else l[2] = n[2];
    unsigned prev = n[2];
    if (prev != 0) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(prev))[1] = n[1];
    else l[1] = n[1];
    __builtin_delete(n);
    l[0] = l[0] - 1;
    return next != 0 ? static_cast<int>(next) : 0x7fffffff;
}
}
