// Observed: MergeFreeSpuMem__13VISoundDevice at 0x01126bc8, 156 bytes. Argument: the device. The free-block list is the one at
// device + 3312, as FreeSpuMem__13VISoundDeviceUii walks it: the list's first word points at its first node, a node's first word
// is its next link, and a node whose next link is zero ends the list. Node words +16 and +20 hold a block address and a length.
// The routine keeps `previous` (starting at the first node) and `current` (the node after it). It returns at once when there is no
// node after the first. For each pass:
//   - if current's next link is zero, current is the end node and only becomes `previous`;
//   - otherwise, when previous.address + previous.length equals current.address, current takes previous's address and the sum of
//     the two lengths (both stored before the call below), previous is unlinked with Remove__7VILNode, and then handed to
//     AddTail__7VILListP7VILNode(device + 3324, previous), the spare-node list; either way current becomes `previous`.
//   The next pass starts from previous's next link and ends when that link is zero.
// Inferred: adjacent free blocks are coalesced and the freed list node goes back to the spare pool.
// Unknown: the member names and whether the end node's own address and length words are meaningful.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned& word_at(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }
extern "C" {
void Remove__7VILNode(unsigned node);
void AddTail__7VILListP7VILNode(unsigned char* list, unsigned node);

void MergeFreeSpuMem__13VISoundDevice(unsigned char* device) {
    unsigned previous = word_at(device, 3312);
    unsigned current = word_at(bytes_at(previous), 0);
    if (current == 0) return;
    do {
        if (word_at(bytes_at(current), 0) != 0) {
            if (word_at(bytes_at(previous), 16) + word_at(bytes_at(previous), 20) == word_at(bytes_at(current), 16)) {
                word_at(bytes_at(current), 16) = word_at(bytes_at(previous), 16);
                word_at(bytes_at(current), 20) = word_at(bytes_at(current), 20) + word_at(bytes_at(previous), 20);
                Remove__7VILNode(previous);
                AddTail__7VILListP7VILNode(device + 3324, previous);
            }
        }
        previous = current;
        current = word_at(bytes_at(previous), 0);
    } while (current != 0);
}
}
