// memInit__FPvi at 0x001448e8, 108 bytes.
// A positive size fills that many bytes with 0xA5. Zero and negative sizes
// skip the fill. The header at the block is then one free block: magic
// 0xABCD, flag 0, name 0, size and free-byte words are the size minus 28,
// and next and previous point at the block. The ring and next-fit words
// point at the block, the capacity word receives the size, the fragment
// threshold becomes 36, and the dump count becomes 0.
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4, "32-bit heap words");

extern "C" {
extern unsigned char norrath_heap_next_fit_view[4];
extern unsigned char norrath_heap_ring_word_view[4];
extern unsigned char norrath_heap_free_bytes_view[4];
extern unsigned char norrath_heap_total_bytes_view[4];
extern unsigned char norrath_heap_fragment_threshold_view[4];
extern unsigned char norrath_heap_dump_count_view[4];
void memInit__FPvi(void* block, int size);
}

namespace {
unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
}

extern "C" void memInit__FPvi(void* block, int size) {
    auto* bytes = static_cast<unsigned char*>(block);
    if (size > 0) {
        int index = 0;
        do {
            bytes[index] = 0xa5;
            ++index;
        } while (index < size);
    }
    const unsigned address = low_address(block);
    const unsigned payload = static_cast<unsigned>(size) - 28u;
    write_half(bytes, 0xabcdu);
    write_half(bytes + 2, 0);
    write_word(bytes + 4, payload);
    write_word(bytes + 8, 0);
    write_word(bytes + 12, address);
    write_word(bytes + 16, address);
    write_word(norrath_heap_next_fit_view, address);
    write_word(norrath_heap_ring_word_view, address);
    write_word(norrath_heap_free_bytes_view, payload);
    write_word(norrath_heap_total_bytes_view, static_cast<unsigned>(size));
    write_word(norrath_heap_fragment_threshold_view, 36u);
    write_word(norrath_heap_dump_count_view, 0u);
}
