// Observed P_DeleteParticleSystem__Fi at 0x00180468, 272 bytes.
//  1. An id of zero returns. Otherwise the record is PS (0x006f5ad0) plus
//     (id & 0xff) * 312. If the word at record+304 is not equal to the whole id,
//     the function returns. The index has no upper bound check.
//  2. The first 256 bytes of the record are four groups of four 16-byte entries:
//     head word at 0, tail word at 4, and words 1 and 257 at 8 and 12 once
//     cleared. Groups run in order, entries in order. An entry with a nonzero
//     head is a chain linked through word 0. It is walked from the head and each
//     node is pushed on the free block list (freeBlocks, 0x004869f0) by storing
//     the list in node word 0, until the node just pushed is the tail. The next
//     link is read before it is overwritten. The list is stored back to
//     freeBlocks after all sixteen entries.
//  3. Every entry is reset to head 0, tail 0, 1, 257, and the word at +304 is
//     cleared.
// The same chain release is in P_Clear__Fv for the global system. Fresh
// compilation is not an original-byte match.
extern "C" {
extern unsigned char freeBlocks[];
extern unsigned char PS[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(a)));
#else
    return reinterpret_cast<unsigned char*>(a);
#endif
}
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void P_DeleteParticleSystem__Fi(int id) {
    if (id == 0) return;
    const Address record = address(PS) + (static_cast<unsigned>(id) & 0xffu) * 312u;
    if (read_word(record + 304u) != static_cast<unsigned>(id)) return;
    unsigned list = read_word(address(freeBlocks));
    for (unsigned group = 0u; group < 4u; ++group) {
        for (unsigned entry = 0u; entry < 4u; ++entry) {
            const Address slot = record + group * 64u + entry * 16u;
            const unsigned head = read_word(slot);
            if (head == 0u) continue;
            const unsigned tail = read_word(slot + 4u);
            unsigned current = head;
            do {
                const unsigned node = current;
                current = read_word(node);
                write_word(node, list);
                list = node;
            } while (list != tail);
        }
    }
    write_word(address(freeBlocks), list);
    for (unsigned group = 0u; group < 4u; ++group) {
        for (unsigned entry = 0u; entry < 4u; ++entry) {
            const Address slot = record + group * 64u + entry * 16u;
            write_word(slot, 0u);
            write_word(slot + 4u, 0u);
            write_word(slot + 8u, 1u);
            write_word(slot + 12u, 257u);
        }
    }
    write_word(record + 304u, 0u);
}
