#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned long long norrath_scramble_free_host_address_base = 0;
extern "C" {
unsigned char norrath_heap_ring_word_view[4];
}
extern "C" void memScrambleFree__Fv();

namespace {
unsigned char arena[3][64];
int fills = 0;
bool move_ring = false;

unsigned low_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void set_block(int index, int next, unsigned flag, unsigned size) {
    std::memset(arena[index], 0x11, sizeof arena[index]);
    write_word(arena[index] + 12, low_of(arena[next]));
    write_word(arena[index] + 4, size);
    arena[index][2] = static_cast<unsigned char>(flag);
    arena[index][3] = 0;
}
bool payload_is(int index, unsigned size, unsigned char value) {
    for (unsigned i = 0; i < size; ++i) {
        if (arena[index][28 + i] != value) return false;
    }
    return arena[index][28 + size] == 0x11;
}
int fail(int code) {
    std::fprintf(stderr, "memScrambleFree check failed: %d\n", code);
    return code;
}
}

extern "C" void* norrath_scramble_test_fill(void* destination, int value, __SIZE_TYPE__ count) {
    ++fills;
    auto* bytes = static_cast<unsigned char*>(destination);
    for (__SIZE_TYPE__ i = 0; i < count; ++i) bytes[i] = static_cast<unsigned char>(value);
    if (move_ring) write_word(norrath_heap_ring_word_view, low_of(arena[2]));
    return nullptr;
}

int main() {
    norrath_scramble_free_host_address_base =
        reinterpret_cast<std::uintptr_t>(arena) & ~0xffffffffull;
    set_block(0, 1, 0, 4);
    set_block(1, 2, 1, 4);
    set_block(2, 0, 0, 2);
    write_word(norrath_heap_ring_word_view, low_of(arena[0]));
    memScrambleFree__Fv();
    if (fills != 2) return fail(1);
    if (!payload_is(0, 4, 0xa5) || !payload_is(2, 2, 0xa5)) return fail(2);
    if (arena[1][28] != 0x11) return fail(3);

    set_block(0, 1, 0, 1);
    set_block(1, 2, 0, 1);
    set_block(2, 0, 0, 1);
    write_word(norrath_heap_ring_word_view, low_of(arena[0]));
    fills = 0;
    move_ring = true;
    memScrambleFree__Fv();
    if (fills != 2) return fail(4);
    if (arena[2][28] != 0x11) return fail(5);
    std::printf("memScrambleFree checks passed\n");
    return 0;
}
