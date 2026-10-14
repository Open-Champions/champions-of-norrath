// Observed updatePS__FP14ParticleSystem at 0x00181080, 608 bytes. The argument
// is a 256-byte system: four groups of four 16-byte entries (the same layout
// P_Clear__Fv resets): head node word at 0, tail node word at 4, current slot
// index at 8, end slot index at 12. The result is 1 once any entry has had a
// particle tested, otherwise 0.
// Nodes are words of blockList (0x00780080); a node's own word links to the next
// node. Node n owns the 4096-byte page at block (0x00700080) plus the node's word
// index times 4096, of 256 particle slots of 16 bytes. Each entry n (0 to 3) of a
// group uses lifeTimes[n] (0x0037f720). For every entry with a nonzero head:
//  - Head, tail, slot and end are read once. If head == tail and slot == end the
//    scan stops. Otherwise the particle is at page + slot * 16 and its signed
//    halfword at +6 plus lifeTimes[n] is compared with the signed halfword
//    P_FrameNum. The result is set to 1 here, whichever way the test goes.
//  - A particle whose sum is below P_FrameNum is expired. Its descriptor is the
//    word at descList (0x00780300) + 64 * (signed halfword at +14, shifted right
//    by 2) + 24. If the descriptor's word at +12 is nonzero it is called with the
//    particle address. Then the halfword at particle+14 is cleared and the word
//    at descriptor+8 is decremented; when it reaches zero, descListMark
//    (0x00780280) word [descriptor+4] and the word at descriptor+4 become
//    0xffffffff. The slot index advances by one. When it is 31 modulo 32 it
//    advances by two more, unless it is 255. At 255, a node that is not the tail is
//    pushed on freeBlocks and the scan continues with the next node at slot 1; at
//    the tail the slot index advances by two (to 257).
//  - A particle that is not expired, or a head == tail and slot == end stop, ends
//    the scan: the head word and slot word of the entry are written back with
//    the current node and slot. If the node is the tail as now stored in the
//    entry and the slot equals the entry's end as now stored, the entry is
//    retired: its slot word becomes 1, its end word 257, the node word receives
//    freeBlocks, freeBlocks receives the node, and head and tail become zero.
// Fresh compilation is not an original-byte match.
extern "C" {
extern unsigned char blockList[];
extern unsigned char block[];
extern unsigned char descList[];
extern unsigned char descListMark[];
extern unsigned char freeBlocks[];
extern unsigned short P_FrameNum;
extern unsigned lifeTimes[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
extern "C" void norrath_host_call(unsigned function, unsigned argument);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
unsigned low32(const void* p) { return static_cast<unsigned>(reinterpret_cast<Address>(p)); }
unsigned char* bytes(unsigned a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(a));
#else
    return reinterpret_cast<unsigned char*>(static_cast<Address>(a));
#endif
}
unsigned read_word(unsigned a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int read_half(unsigned a) {
    const unsigned char* p = bytes(a);
    return static_cast<int>(((unsigned(p[0]) | (unsigned(p[1]) << 8)) ^ 0x8000u)) - 0x8000;
}
void write_half(unsigned a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
void call_particle_function(unsigned function, unsigned particle) {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_call(function, particle);
#else
    reinterpret_cast<void (*)(unsigned)>(static_cast<Address>(function))(particle);
#endif
}
unsigned page_of(unsigned node) {
    return ((node - low32(blockList)) << 10) + low32(block);
}
}

extern "C" int updatePS__FP14ParticleSystem(void* system) {
    unsigned result = 0u;
    for (unsigned group = 0u; group < 4u; ++group) {
        for (unsigned entry = 0u; entry < 4u; ++entry) {
            const unsigned slot = low32(system) + group * 64u + entry * 16u;
            unsigned node = read_word(slot);
            if (node == 0u) continue;
            const unsigned tail = read_word(slot + 4u);
            unsigned index = read_word(slot + 8u);
            const unsigned end = read_word(slot + 12u);
            const unsigned life = lifeTimes[entry];
            unsigned page = page_of(node);
            for (;;) {
                if (node == tail && index == end) {
                    // fall to the write-back below
                } else {
                    const unsigned particle = page + index * 16u;
                    const int frame = static_cast<short>(P_FrameNum);
                    const int sum = read_half(particle + 6u) + static_cast<int>(life);
                    result = 1u;
                    if (sum >= frame) {
                        // not expired: fall to the write-back below
                    } else {
                        const int kind = read_half(particle + 14u) >> 2;
                        const unsigned descriptor = read_word(low32(descList) + static_cast<unsigned>(kind) * 64u + 24u);
                        const unsigned function = read_word(descriptor + 12u);
                        if (function != 0u) call_particle_function(function, particle);
                        const unsigned count = read_word(descriptor + 8u);
                        write_half(particle + 14u, 0u);
                        write_word(descriptor + 8u, count - 1u);
                        if (count - 1u == 0u) {
                            write_word(low32(descListMark) + read_word(descriptor + 4u) * 4u, 0xffffffffu);
                            write_word(descriptor + 4u, 0xffffffffu);
                        }
                        index += 1u;
                        if ((index & 0x1fu) != 31u) continue;
                        if (index != 255u) { index += 2u; continue; }
                        if (node == tail) { index += 2u; continue; }
                        const unsigned old = node;
                        node = read_word(node);
                        const unsigned list = read_word(low32(freeBlocks));
                        index = 1u;
                        page = page_of(node);
                        write_word(old, list);
                        write_word(low32(freeBlocks), old);
                        continue;
                    }
                }
                break;
            }
            const unsigned stored_tail = read_word(slot + 4u);
            write_word(slot, node);
            write_word(slot + 8u, index);
            if (node != stored_tail) continue;
            if (index != read_word(slot + 12u)) continue;
            const unsigned list = read_word(low32(freeBlocks));
            write_word(slot + 8u, 1u);
            write_word(node, list);
            write_word(slot + 12u, 257u);
            write_word(low32(freeBlocks), read_word(slot));
            write_word(slot, 0u);
            write_word(slot + 4u, 0u);
        }
    }
    return static_cast<int>(result);
}
