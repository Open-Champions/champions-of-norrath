// Observed: Erase__t6VIList1ZQ27VIScene12VISceneOccupi at 0x01225270, 156 bytes. Arguments: the list and the
// address of one node. Offsets are bytes from the list or from the node; the node's link words are at
// +24 (next) and +28 (previous) in this instantiation.
//  1. next = node+24. If next is nonzero, next+28 receives node+28; otherwise list+8 does.
//  2. prev = node+28. If prev is nonzero, prev+24 receives node+24; otherwise list+4 does.
//  3. __builtin_delete(node).
//  4. list+0 is decremented. The result is next when it is nonzero, else the result of End__Ct6VIList1ZQ27VIScene12VISceneOccup(list) (a tail call).
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
int End__Ct6VIList1ZQ27VIScene12VISceneOccup(const void* list);
void __builtin_delete(void* pointer);

int Erase__t6VIList1ZQ27VIScene12VISceneOccupi(void* list, unsigned node) {
    unsigned* l = static_cast<unsigned*>(list);
    unsigned* n = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(node));
    unsigned next = n[6];
    if (next != 0) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(next))[7] = n[7];
    else l[2] = n[7];
    unsigned prev = n[7];
    if (prev != 0) reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(prev))[6] = n[6];
    else l[1] = n[6];
    __builtin_delete(n);
    l[0] = l[0] - 1;
    if (next == 0) return End__Ct6VIList1ZQ27VIScene12VISceneOccup(list);
    return static_cast<int>(next);
}
}
