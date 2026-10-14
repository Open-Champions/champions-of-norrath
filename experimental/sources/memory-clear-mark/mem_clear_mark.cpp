// memClearMark__Fi at 0x00144848, 28 bytes.
// The mark word at GP-16104 loses the bit selected by the low five bits of
// the argument. The following nop is padding in the pinned image.
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4, "32-bit mark word");

extern "C" unsigned char norrath_heap_allocation_tag_view[4];

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void memClearMark__Fi(int bit) {
    const unsigned mask = 1u << (static_cast<unsigned>(bit) & 31u);
    write_word(norrath_heap_allocation_tag_view, read_word(norrath_heap_allocation_tag_view) & ~mask);
}
