#include <cstdio>

extern "C" {
unsigned char norrath_heap_allocation_tag_view[4];
}
extern "C" void memClearMark__Fi(int bit);

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
int fail(int code) {
    std::fprintf(stderr, "memClearMark check failed: %d\n", code);
    return code;
}
}

int main() {
    write_word(norrath_heap_allocation_tag_view, 0x80000009u);
    memClearMark__Fi(0);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000008u) return fail(1);
    memClearMark__Fi(3);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000000u) return fail(2);
    memClearMark__Fi(1);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000000u) return fail(3);
    memClearMark__Fi(-1);
    if (read_word(norrath_heap_allocation_tag_view) != 0u) return fail(4);
    memClearMark__Fi(32);
    if (read_word(norrath_heap_allocation_tag_view) != 0u) return fail(5);
    std::printf("memClearMark checks passed\n");
    return 0;
}
