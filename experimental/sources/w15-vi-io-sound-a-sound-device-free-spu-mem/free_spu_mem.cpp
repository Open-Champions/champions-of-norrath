// Observed: FreeSpuMem__13VISoundDeviceUii at 0x01126b08, 188 bytes. Arguments: the device, an address A and a size S. The device
// holds two lists of VILNode records: the list at +3324 supplies spare nodes and the list at +3312 is the free-block list kept in
// ascending address order (the first word of a node is its next link and the second its previous link; a node whose next link
// is zero ends the walk).
//  1. R = (S + 63) / 64 * 64 with a signed divide (S + 63 below zero is adjusted before the shift, so the quotient truncates
//     toward zero).
//  2. node = RemHead__7VILList(device + 3324). The word at node+16 is set to A and the word at node+20 to R.
//  3. Walk the list at +3312 from its first node: stop at a node whose next link is zero; for each other node, if A is below the
//     word at its +16 then InsertAfter__7VILNodeP7VILNode(node, that node's previous link) places the new node before it and the
//     walk ends; otherwise move to the next node.
//  4. If the walk ended without inserting, AddTail__7VILListP7VILNode(device + 3312, node).
//  5. MergeFreeSpuMem__13VISoundDevice(device). No result.
// Inferred: this returns a block of sound memory to the free list, rounded up to 64-byte units, keeping the list sorted by address
// and merging neighbours afterwards. The spare-node list is a pool of list nodes. The word at +16 is a block address and +20 its
// length in bytes.
// Unknown: the member names, whether S is meant to be non-negative, and the empty-pool case (RemHead's result is used unchecked).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
unsigned RemHead__7VILList(unsigned char* list);
void InsertAfter__7VILNodeP7VILNode(unsigned node, unsigned after);
void AddTail__7VILListP7VILNode(unsigned char* list, unsigned node);
void MergeFreeSpuMem__13VISoundDevice(unsigned char* device);

void FreeSpuMem__13VISoundDeviceUii(unsigned char* device, unsigned address, int size) {
    int rounded = (size + 63) / 64 * 64;
    unsigned node = RemHead__7VILList(device + 3324);
    *reinterpret_cast<unsigned*>(bytes_at(node) + 16) = address;
    *reinterpret_cast<int*>(bytes_at(node) + 20) = rounded;
    bool inserted = false;
    for (unsigned cursor = word_at(device, 3312); word_at(bytes_at(cursor), 0) != 0; cursor = word_at(bytes_at(cursor), 0)) {
        if (word_at(bytes_at(node), 16) < word_at(bytes_at(cursor), 16)) {
            InsertAfter__7VILNodeP7VILNode(node, word_at(bytes_at(cursor), 4));
            inserted = true;
            break;
        }
    }
    if (!inserted) AddTail__7VILListP7VILNode(device + 3312, node);
    MergeFreeSpuMem__13VISoundDevice(device);
}
}
