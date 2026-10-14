// memScrambleFree__Fv at 0x00144ba8, 80 bytes.
// Walks the ring at GP-4412. A block whose halfword at +2 is zero has its
// payload, 28 bytes after the header, filled with 0xA5 for the size word at
// +4. After that fill the ring word is loaded again and becomes the end of
// the walk. Allocated blocks are skipped and do not reload the ring word.
static_assert(sizeof(unsigned) == 4, "32-bit heap words");

extern "C" {
#if defined(NORRATH_SCRAMBLE_FREE_HOST_CHECK)
void* norrath_scramble_test_fill(void* destination, int value, __SIZE_TYPE__ count);
#define memset norrath_scramble_test_fill
#else
void* memset(void* destination, int value, __SIZE_TYPE__ count);
#endif
extern unsigned char norrath_heap_ring_word_view[4];
#if defined(NORRATH_SCRAMBLE_FREE_HOST_CHECK)
extern unsigned long long norrath_scramble_free_host_address_base;
#endif
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned read_half(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
unsigned char* from_low(unsigned address) {
#if defined(NORRATH_SCRAMBLE_FREE_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_scramble_free_host_address_base + address);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
}

extern "C" void memScrambleFree__Fv() {
    unsigned cursor = read_word(norrath_heap_ring_word_view);
    unsigned start = cursor;
    for (;;) {
        unsigned char* bytes = from_low(cursor);
        if (read_half(bytes + 2) == 0u) {
            const unsigned size = read_word(bytes + 4);
            memset(from_low(cursor + 28u), 0xa5, size);
            start = read_word(norrath_heap_ring_word_view);
        }
        cursor = read_word(from_low(cursor) + 12);
        if (cursor == start) return;
    }
}
