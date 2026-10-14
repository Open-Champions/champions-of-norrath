// Observed: Erase__t6VIList1Z7VIActori at 0x01225120, 188 bytes. Arguments: the list and the address of one node (an int). Offsets are
// bytes from the list or from the node; the node is a VIActor, whose list links sit at +160 (next) and +164 (previous).
//  1. next = node+160. If next is nonzero, next+164 receives node+164; otherwise list+8 does.
//  2. prev = node+164, read afresh. If prev is nonzero, prev+160 receives node+160 (read afresh); otherwise list+4 does.
//  3. When the node address is nonzero: _$_7VIActor(node, 2) is called, then __builtin_delete(node).
//  4. list+0 is read afresh and stored decremented by one.
//  5. The result is next when it is nonzero, else End__Ct6VIList1Z7VIActor(list) (a tail call).
// Inferred: a doubly linked list with head at list+4, tail at list+8 and count at list+0 whose nodes are the actors themselves; the
// result is the iterator after the erased element. Flags 2 on the destructor call is the usual "destroy in place, do not delete".
// Unknown: ownership beyond this call and whether a null node is ever passed (the link words are read before the null test).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
static inline unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
static inline void* object_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned* at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
static inline void* object_at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void _$_7VIActor(void* actor, int flags);
void __builtin_delete(void* pointer);
int End__Ct6VIList1Z7VIActor(const void* list);

int Erase__t6VIList1Z7VIActori(void* list, int node) {
    unsigned* l = static_cast<unsigned*>(list);
    unsigned address = static_cast<unsigned>(node);
    // The link words are addressed from node+160 so that the later null test on the node is kept.
    unsigned next = at(address + 160)[0];
    if (next != 0) at(next + 164)[0] = at(address + 164)[0];
    else l[2] = at(address + 164)[0];
    unsigned prev = at(address + 164)[0];
    if (prev != 0) at(prev + 160)[0] = at(address + 160)[0];
    else l[1] = at(address + 160)[0];
    if (node != 0) {
        _$_7VIActor(object_at(address), 2);
        __builtin_delete(object_at(address));
    }
    l[0] = l[0] - 1;
    if (next == 0) return End__Ct6VIList1Z7VIActor(list);
    return static_cast<int>(next);
}
}
