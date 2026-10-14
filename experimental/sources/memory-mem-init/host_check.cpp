#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_heap_next_fit_view[4];
unsigned char norrath_heap_ring_word_view[4];
unsigned char norrath_heap_free_bytes_view[4];
unsigned char norrath_heap_total_bytes_view[4];
unsigned char norrath_heap_fragment_threshold_view[4];
unsigned char norrath_heap_dump_count_view[4];
unsigned char norrath_heap_allocation_tag_view[4];
void memInit__FPvi(void* block, int size);
}

namespace {
unsigned char block[64];
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned read_half(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
unsigned low_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int fail(int code) {
    std::fprintf(stderr, "memInit check failed: %d\n", code);
    return code;
}
}

int main() {
    const unsigned address = low_of(block);
    std::memset(block, 0x11, sizeof block);
    write_word(norrath_heap_allocation_tag_view, 0x11111111u);
    memInit__FPvi(block, 40);
    for (int i = 28; i < 40; ++i) {
        if (block[i] != 0xa5) return fail(1);
    }
    if (block[40] != 0x11) return fail(2);
    if (read_half(block) != 0xabcdu || read_half(block + 2) != 0) return fail(3);
    if (read_word(block + 4) != 12u || read_word(block + 8) != 0u) return fail(4);
    if (read_word(block + 12) != address || read_word(block + 16) != address) return fail(5);
    if (read_word(norrath_heap_next_fit_view) != address || read_word(norrath_heap_ring_word_view) != address)
        return fail(6);
    if (read_word(norrath_heap_free_bytes_view) != 12u || read_word(norrath_heap_total_bytes_view) != 40u)
        return fail(7);
    if (read_word(norrath_heap_fragment_threshold_view) != 36u || read_word(norrath_heap_dump_count_view) != 0u)
        return fail(8);
    if (read_word(norrath_heap_allocation_tag_view) != 0x11111111u) return fail(9);

    std::memset(block, 0x11, sizeof block);
    memInit__FPvi(block, 0);
    if (block[0] == 0x11 || block[28] != 0x11) return fail(10);
    if (read_word(block + 4) != 0xffffffe4u || read_word(norrath_heap_total_bytes_view) != 0u) return fail(11);

    std::memset(block, 0x11, sizeof block);
    memInit__FPvi(block, -5);
    if (block[28] != 0x11) return fail(12);
    if (read_word(block + 4) != static_cast<unsigned>(-5) - 28u) return fail(13);
    if (read_word(norrath_heap_total_bytes_view) != static_cast<unsigned>(-5)) return fail(14);
    std::printf("memInit checks passed\n");
    return 0;
}
