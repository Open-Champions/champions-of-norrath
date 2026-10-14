// Observed: VIInitUSocketExApi__Fv at 0x01048d70, 136 bytes, pinned SLUS_205.65, takes no argument and returns nothing.
//  1. sMessagePool (0x011e5c18) = __wrap_malloc(0x8240).
//  2. ll_init on sFreeMessages (0x011e5c20) and then on sUsedMessages (0x011e5c30).
//  3. Sixteen times, ll_add_tail(sFreeMessages, pool + n * 2084) for n = 0..15, re-reading sMessagePool for each node.
// The pool size is 16 * 2084 bytes exactly. It is the counterpart of VIFreeUSocketExApi__Fv, which frees sMessagePool.
// Inferred: the pool is sixteen fixed-size message nodes, all starting on the free list, with an empty used list.
// Unknown: the node layout (only the link fields used by ll_add_tail are known), who takes nodes from the lists, and the
// meaning of the 2084-byte size. The three objects are file-local in the image and are left as imports here, like the
// sMessagePool import of VIFreeUSocketExApi__Fv. ll_init and ll_add_tail are not defined by this unit.
extern "C" {
struct llist;
struct lnode;
extern void* sMessagePool;
extern unsigned char sFreeMessages[];
extern unsigned char sUsedMessages[];
void* __wrap_malloc(unsigned size);
void ll_init__FP5llist(llist* list);
void ll_add_tail__FP5llistP5lnode(llist* list, lnode* node);

void VIInitUSocketExApi__Fv() {
    sMessagePool = __wrap_malloc(0x8240);
    ll_init__FP5llist(reinterpret_cast<llist*>(sFreeMessages));
    ll_init__FP5llist(reinterpret_cast<llist*>(sUsedMessages));
    for (int index = 0; index < 16; ++index) {
        ll_add_tail__FP5llistP5lnode(reinterpret_cast<llist*>(sFreeMessages),
                                     reinterpret_cast<lnode*>(static_cast<unsigned char*>(sMessagePool) + index * 2084));
    }
}
}
