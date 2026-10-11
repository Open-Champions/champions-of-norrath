#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_heap_allocation_tag_view[4];
}
extern "C" void memSetMark__Fi(int bit);

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int fail(int code) {
    std::fprintf(stderr, "memSetMark check failed: %d\n", code);
    return code;
}
}

int main() {
    std::memset(norrath_heap_allocation_tag_view, 0, 4);
    memSetMark__Fi(0);
    if (read_word(norrath_heap_allocation_tag_view) != 1u) return fail(1);
    memSetMark__Fi(3);
    if (read_word(norrath_heap_allocation_tag_view) != 0x9u) return fail(2);
    memSetMark__Fi(31);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000009u) return fail(3);
    memSetMark__Fi(32);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000009u) return fail(4);
    memSetMark__Fi(-1);
    if (read_word(norrath_heap_allocation_tag_view) != 0x80000009u) return fail(5);
    std::printf("memSetMark checks passed\n");
    return 0;
}
