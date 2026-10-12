// Host contract for P_DeleteParticleSystem__Fi. Not an EE plan input.
// Pointer words are 32-bit, so chain nodes live in one static arena.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char freeBlocks[4] = {0};
unsigned char PS[16 * 312] = {0};
void P_DeleteParticleSystem__Fi(int id);
}

static unsigned char arena[4096];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset = 0) {
    unsigned v; std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4); return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}
static unsigned char* node(int index) { return arena + 64 * index; }
static void chain(unsigned char* entry, int first, int count) {
    for (int i = 0; i < count; ++i) set_word(node(first + i), 0, i + 1 < count ? low(node(first + i + 1)) : 0x5a5a5a5au);
    set_word(entry, 0, low(node(first)));
    set_word(entry, 4, low(node(first + count - 1)));
}
static void set_record(int index, unsigned owner) {
    std::memset(PS + index * 312, 0, 312);
    set_word(PS + index * 312, 304, owner);
}

int main() {
    // Record 5 is owned by id 0x105 (index = low byte). Entries 2 and 7 hold chains.
    set_record(5, 0x105u);
    chain(PS + 5 * 312 + 2 * 16, 1, 3);
    chain(PS + 5 * 312 + 7 * 16, 10, 2);
    set_record(6, 0x106u);
    chain(PS + 6 * 312, 20, 2);
    set_word(freeBlocks, 0, 0x7000u);

    P_DeleteParticleSystem__Fi(0x105);
    expect(word(node(1)) == 0x7000u, "first node points at the old free list");
    expect(word(node(2)) == low(node(1)), "second node points at the first");
    expect(word(node(3)) == low(node(2)), "third node points at the second");
    expect(word(node(10)) == low(node(3)), "the next chain continues the list");
    expect(word(node(11)) == low(node(10)), "second chain order");
    expect(word(freeBlocks) == low(node(11)), "free list head is the last tail");
    for (unsigned e = 0; e < 16; ++e) {
        const unsigned char* s = PS + 5 * 312 + e * 16;
        expect(word(s, 0) == 0u && word(s, 4) == 0u && word(s, 8) == 1u && word(s, 12) == 257u, "entry reset");
    }
    expect(word(PS + 5 * 312, 304) == 0u, "owner word cleared");
    expect(word(PS + 6 * 312, 0) == low(node(20)), "other record untouched");

    // An id whose word does not match is ignored, including a matching low byte.
    set_word(freeBlocks, 0, 0x7000u);
    P_DeleteParticleSystem__Fi(0x206);
    expect(word(PS + 6 * 312, 0) == low(node(20)) && word(freeBlocks) == 0x7000u, "mismatched id changes nothing");
    P_DeleteParticleSystem__Fi(0);
    expect(word(freeBlocks) == 0x7000u, "id zero returns early");

    // The matching id releases the other record; an empty record still rewrites freeBlocks.
    P_DeleteParticleSystem__Fi(0x106);
    expect(word(node(20)) == 0x7000u && word(node(21)) == low(node(20)), "second record chain released");
    expect(word(freeBlocks) == low(node(21)), "tail becomes the list");
    set_record(7, 0x107u);
    set_word(freeBlocks, 0, 0x1234u);
    P_DeleteParticleSystem__Fi(0x107);
    expect(word(freeBlocks) == 0x1234u, "empty record leaves the list value");
    expect(word(PS + 7 * 312, 8) == 1u && word(PS + 7 * 312, 12) == 257u, "empty record still reset");
    if (failures) return 1;
    std::printf("delete-particle-system contract passed\n");
    return 0;
}
