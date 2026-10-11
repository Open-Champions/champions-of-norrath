// memDeleteMarked__Fi at 0x00144868, 124 bytes.
// Walks the ring at GP-4412. A block whose halfword at +2 is nonzero and
// whose tag word at +20 has the selected bit is freed through its payload,
// 28 bytes after the header. One free ends the pass. The next pass starts
// again at the ring word, so a link changed by free is the link that is followed.
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4, "32-bit heap words");

extern "C" {
void memFree__FPv(void* block);
extern unsigned char norrath_heap_ring_word_view[4];
#if defined(NORRATH_DELETE_MARKED_HOST_CHECK)
extern unsigned long long norrath_delete_marked_host_address_base;
#endif
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned read_half(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
unsigned char* from_low(unsigned address) {
#if defined(NORRATH_DELETE_MARKED_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_delete_marked_host_address_base + address);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
}

extern "C" void memDeleteMarked__Fi(int bit) {
    const unsigned mask_shift = static_cast<unsigned>(bit) & 31u;
    for (;;) {
        unsigned header = read_word(norrath_heap_ring_word_view);
        const unsigned start = header;
        bool freed = false;
        for (;;) {
            unsigned char* bytes = from_low(header);
            if (read_half(bytes + 2) != 0u) {
                const unsigned tag = read_word(bytes + 20);
                if (((tag >> mask_shift) & 1u) != 0u) {
                    memFree__FPv(from_low(header + 28u));
                    freed = true;
                    break;
                }
            }
            header = read_word(from_low(header) + 12);
            if (header == start) break;
        }
        if (!freed) return;
    }
}
