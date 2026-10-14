// Host contract for P_Clear__Fv. Not an EE plan input.
// Pointer words are 32-bit, so chain nodes live in one static arena.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char pushLevel[4] = {0};
unsigned char freeBlocks[4] = {0};
unsigned char globalSystem[256] = {0};
unsigned char blockList[512] = {0};
unsigned char descListMark[128] = {0};
unsigned char descList[2048] = {0};
unsigned char nmSingle[4] = {0};
unsigned char nmLines[4] = {0};
unsigned char PS[16 * 312] = {0};
void P_Clear__Fv();
}

static unsigned char arena[4096];
static int failures = 0;
static int calls = 0, pop_at = 0, trails_at = 0, systems_at = 0;
static unsigned free_blocks_at_trails = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset = 0) {
    unsigned v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4);
    return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}
extern "C" void P_Pop__Fv() { pop_at = ++calls; }
extern "C" void initTrails__Fv() { trails_at = ++calls; free_blocks_at_trails = word(freeBlocks); }
extern "C" void initParticleSystems__Fv() { systems_at = ++calls; }

static unsigned char* node(int index) { return arena + 64 * index; }
// Chain of `count` nodes starting at index `first`, linked by word 0.
static void chain(unsigned char* entry, int first, int count) {
    for (int i = 0; i < count; ++i) set_word(node(first + i), 0, i + 1 < count ? low(node(first + i + 1)) : 0u);
    set_word(entry, 0, low(node(first)));
    set_word(entry, 4, low(node(first + count - 1)));
}

int main() {
    // Descriptor slots: slot 0 is untouched, slot 3 is in use.
    std::memset(descListMark, 0xff, sizeof descListMark);
    set_word(descListMark, 0, 7u);               // slot 0 mark stays
    set_word(descListMark, 3 * 4, 1u);           // slot 3 in use
    set_word(descList, 3 * 64 + 24, low(node(40)));
    set_word(node(40), 4, 5u);
    set_word(node(40), 8, 6u);
    set_word(nmSingle, 0, 9u);
    set_word(nmLines, 0, 8u);
    set_word(pushLevel, 0, 1u);
    set_word(freeBlocks, 0, 0x7000u);

    // globalSystem entry 2 (group 0): three nodes 1..3. Entry 5 (group 1, entry 1): two nodes 10, 11.
    chain(globalSystem + 2 * 16, 1, 3);
    chain(globalSystem + 64 + 16, 10, 2);
    // PS record 3 is in use with a chain of nodes 20, 21. Record 4 is unused but has a chain.
    set_word(PS + 3 * 312, 304, 1u);
    chain(PS + 3 * 312 + 3 * 64 + 3 * 16, 20, 2);
    chain(PS + 4 * 312, 30, 2);

    P_Clear__Fv();
    expect(pop_at == 1, "P_Pop runs first when pushLevel is nonzero");
    expect(trails_at == 2 && systems_at == 3, "initTrails then initParticleSystems last");
    expect(word(descListMark, 3 * 4) == 0xffffffffu, "used descriptor mark is reset");
    expect(word(descListMark, 0) == 7u, "slot 0 is not part of the walk");
    expect(word(node(40), 4) == 0xffffffffu && word(node(40), 8) == 0u, "descriptor record reset");
    expect(word(nmSingle) == 0u && word(nmLines) == 0u, "counters cleared");

    // Chain nodes are pushed in order onto the old free list, head first.
    expect(word(node(1)) == 0x7000u, "first node points at the old free list");
    expect(word(node(2)) == low(node(1)), "second node points at the first");
    expect(word(node(3)) == low(node(2)), "third node points at the second");
    expect(word(node(10)) == low(node(3)), "the next chain continues the list");
    expect(word(node(11)) == low(node(10)), "second chain order");
    expect(word(node(20)) == low(node(11)), "PS chain continues after globalSystem");
    expect(word(node(21)) == low(node(20)), "PS chain second node");
    expect(word(node(30)) == low(node(31)), "unused PS chain keeps its own links");

    for (unsigned entry = 0; entry < 16; ++entry) {
        const unsigned char* e = globalSystem + entry * 16;
        expect(word(e, 0) == 0u && word(e, 4) == 0u && word(e, 8) == 1u && word(e, 12) == 257u, "globalSystem entry reset");
    }
    const unsigned char* used = PS + 3 * 312;
    for (unsigned entry = 0; entry < 16; ++entry)
        expect(word(used, entry * 16) == 0u && word(used, entry * 16 + 8) == 1u && word(used, entry * 16 + 12) == 257u,
               "used PS record entries reset");
    expect(word(used, 304) == 1u, "word 304 is left alone");
    expect(word(PS + 4 * 312, 0) == low(node(30)), "unused PS record is left alone");

    for (unsigned i = 0; i < 127; ++i) expect(word(blockList, i * 4) == low(blockList + (i + 1) * 4), "block list link");
    expect(word(blockList, 127 * 4) == 0u, "block list ends with zero");
    expect(word(freeBlocks) == low(blockList), "free list is the block list");
    expect(free_blocks_at_trails == low(blockList), "the list is rebuilt before initTrails");

    // pushLevel zero skips P_Pop.
    pop_at = 0;
    calls = 0;
    set_word(pushLevel, 0, 0u);
    P_Clear__Fv();
    expect(pop_at == 0, "no P_Pop when pushLevel is zero");
    if (failures != 0) return 1;
    std::printf("p-clear contract passed\n");
    return 0;
}
