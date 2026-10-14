#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned long long norrath_delete_marked_host_address_base = 0;
extern "C" {
unsigned char norrath_heap_ring_word_view[4];
}
extern "C" void memDeleteMarked__Fi(int bit);

namespace {
unsigned char arena[3][64];
int frees = 0;
unsigned char* freed = nullptr;

unsigned low_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_half(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
void set_block(int index, int next, unsigned flag, unsigned tag) {
    std::memset(arena[index], 0x11, sizeof arena[index]);
    write_word(arena[index] + 12, low_of(arena[next]));
    arena[index][2] = static_cast<unsigned char>(flag);
    arena[index][3] = static_cast<unsigned char>(flag >> 8);
    write_word(arena[index] + 20, tag);
}
int fail(int code) {
    std::fprintf(stderr, "memDeleteMarked check failed: %d\n", code);
    return code;
}
}

extern "C" void memFree__FPv(void* block) {
    ++frees;
    freed = static_cast<unsigned char*>(block);
    unsigned char* header = freed - 28;
    header[2] = 0;
    header[3] = 0;
    // Drop the following block. A scan that continues through the old link
    // still frees it; a scan that restarts follows this new link.
    write_word(header + 12, low_of(header));
}

int main() {
    norrath_delete_marked_host_address_base =
        reinterpret_cast<std::uintptr_t>(arena) & ~0xffffffffull;
    set_block(0, 1, 1, 0x1u);
    set_block(1, 2, 1, 0x1u);
    set_block(2, 0, 0, 0x1u);
    write_word(norrath_heap_ring_word_view, low_of(arena[0]));
    frees = 0;
    memDeleteMarked__Fi(0);
    if (frees != 1 || freed != arena[0] + 28) return fail(1);
    if (read_half(arena[1] + 2) != 1u) return fail(2);

    set_block(0, 1, 2, 0x80000000u);
    set_block(1, 0, 1, 0u);
    write_word(norrath_heap_ring_word_view, low_of(arena[0]));
    frees = 0;
    memDeleteMarked__Fi(31);
    if (frees != 1 || freed != arena[0] + 28) return fail(3);
    memDeleteMarked__Fi(-1);
    if (frees != 1) return fail(4);

    set_block(0, 0, 1, 0x2u);
    write_word(norrath_heap_ring_word_view, low_of(arena[0]));
    frees = 0;
    memDeleteMarked__Fi(0);
    if (frees != 0) return fail(5);
    std::printf("memDeleteMarked checks passed\n");
    return 0;
}
