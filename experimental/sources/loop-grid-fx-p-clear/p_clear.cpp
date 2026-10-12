// Observed P_Clear__Fv at 0x180588, extent 660.
// Order of effects:
//  1. A nonzero pushLevel (0x484304) calls P_Pop__Fv first.
//  2. Descriptor slots 1 to 31: every descListMark word (0x780280) that is not
//     0xffffffff is reset to 0xffffffff, and the record pointed to by the
//     word at descList + slot * 64 + 24 gets 0xffffffff at +4 and zero at +8.
//  3. nmSingle (0x4869f8) and nmLines (0x4869fc) become zero.
//  4. The 256-byte globalSystem table (0x37f730) is 4 groups of 4 entries of
//     16 bytes: head 0, tail 4, then words 1 and 257 at 8 and 12 once cleared.
//     An entry with a nonzero head is a chain linked by word 0. It is walked
//     from the head, and each node is pushed on the free block list
//     (freeBlocks, 0x4869f0) by storing the list in node word 0, until the
//     pushed node is the tail. The list is stored back, then all entries are
//     reset (head 0, tail 0, 1, 257).
//  5. Each of the 16 PS records (0x6f5ad0, stride 312) whose word at +304 is
//     nonzero gets the same treatment for its first 256 bytes, continuing the
//     same free list.
//  6. blockList (0x780080, 128 words) is relinked so word i holds the address of
//     word i + 1, the last word is zero, and freeBlocks is the table address.
//  7. initTrails__Fv, then initParticleSystems__Fv.
extern "C" {
extern unsigned char pushLevel[];
extern unsigned char freeBlocks[];
extern unsigned char globalSystem[];
extern unsigned char blockList[];
extern unsigned char descListMark[];
extern unsigned char descList[];
extern unsigned char nmSingle[];
extern unsigned char nmLines[];
extern unsigned char PS[];
void P_Pop__Fv();
void initTrails__Fv();
void initParticleSystems__Fv();
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

// Frees every chain of one 256-byte system onto `list` and returns the list.
unsigned release_chains(Address system, unsigned list) {
    for (unsigned group = 0u; group < 4u; ++group) {
        for (unsigned entry = 0u; entry < 4u; ++entry) {
            const Address record = system + group * 64u + entry * 16u;
            const unsigned head = read_word(record);
            if (head == 0u) continue;
            const unsigned tail = read_word(record + 4u);
            unsigned current = head;
            do {
                const unsigned node = current;
                current = read_word(node);
                write_word(node, list);
                list = node;
            } while (list != tail);
        }
    }
    return list;
}

void reset_entries(Address system) {
    for (unsigned group = 0u; group < 4u; ++group) {
        for (unsigned entry = 0u; entry < 4u; ++entry) {
            const Address record = system + group * 64u + entry * 16u;
            write_word(record, 0u);
            write_word(record + 4u, 0u);
            write_word(record + 8u, 1u);
            write_word(record + 12u, 257u);
        }
    }
}
}

extern "C" void P_Clear__Fv() {
    if (read_word(address(pushLevel)) != 0u) P_Pop__Fv();
    for (unsigned slot = 1u; slot < 32u; ++slot) {
        const Address mark = address(descListMark) + slot * 4u;
        if (read_word(mark) != 0xffffffffu) {
            const Address record = read_word(address(descList) + slot * 64u + 24u);
            write_word(record + 4u, 0xffffffffu);
            write_word(record + 8u, 0u);
            write_word(mark, 0xffffffffu);
        }
    }
    write_word(address(nmSingle), 0u);
    write_word(address(nmLines), 0u);
    unsigned list = release_chains(address(globalSystem), read_word(address(freeBlocks)));
    write_word(address(freeBlocks), list);
    reset_entries(address(globalSystem));
    list = read_word(address(freeBlocks));
    for (unsigned index = 0u; index < 16u; ++index) {
        const Address record = address(PS) + index * 312u;
        if (read_word(record + 304u) == 0u) continue;
        list = release_chains(record, list);
        reset_entries(record);
    }
    const Address table = address(blockList);
    write_word(address(freeBlocks), static_cast<unsigned>(table));
    for (unsigned index = 0u; index < 127u; ++index) write_word(table + index * 4u, static_cast<unsigned>(table + (index + 1u) * 4u));
    write_word(table + 127u * 4u, 0u);
    initTrails__Fv();
    initParticleSystems__Fv();
}
