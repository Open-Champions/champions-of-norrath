// Host contract for updatePS__FP14ParticleSystem. Not an EE plan input.
// Build with -DNORRATH_HOST_CONTRACT. Pointer words are 32-bit, so every object
// lives in one static image and the hooks map the low 32 bits back.
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
unsigned char blockList[512] = {0};
unsigned char block[524288] = {0};
unsigned char descList[2048] = {0};
unsigned char descListMark[128] = {0};
unsigned char freeBlocks[4] = {0};
unsigned short P_FrameNum = 100;
unsigned lifeTimes[4] = {10, 20, 30, 40};
int updatePS__FP14ParticleSystem(void* system);
}
static unsigned char arena[4096];
static std::vector<unsigned> calls;
static unsigned callback_address = 0;
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
extern "C" void norrath_host_call(unsigned function, unsigned argument) {
    if (function == callback_address) calls.push_back(argument);
    else std::fprintf(stderr, "fail: unexpected callee %x\n", function);
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset = 0) {
    unsigned v; std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4); return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}
static void set_half(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 2);
}
static unsigned half(const void* base, unsigned offset) { unsigned v = 0; std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 2); return v; }
static unsigned char* node_word(int n) { return blockList + 4 * n; }
static unsigned char* particle(int node, int slot) { return block + 4096 * node + 16 * slot; }
// A particle stamped so that stamp + life[entry] is below (expired) or at/above the frame.
static void make(int node, int slot, bool expired, unsigned entry, int descriptor_kind) {
    unsigned char* p = particle(node, slot);
    std::memset(p, 0, 16);
    const int stamp = expired ? 100 - static_cast<int>(lifeTimes[entry]) - 1 : 100 - static_cast<int>(lifeTimes[entry]);
    set_half(p, 6, static_cast<unsigned>(stamp));
    set_half(p, 14, static_cast<unsigned>(descriptor_kind << 2) | 3u);
}

int main() {
    alignas(16) static unsigned char system[256];
    alignas(16) static unsigned char desc[2][64];
    callback_address = 0x1234u;
    std::memset(system, 0, sizeof system);
    set_word(desc[0], 4, 0); set_word(desc[0], 8, 2); set_word(desc[0], 12, callback_address);
    set_word(desc[1], 4, 1); set_word(desc[1], 8, 1); set_word(desc[1], 12, 0);
    set_word(descList, 24, low(desc[0]));
    set_word(descList, 64 + 24, low(desc[1]));
    set_word(descListMark, 0, 1); set_word(descListMark, 4, 1);
    set_word(freeBlocks, 0, 0x5000u);

    // Entry 0 (group 0): node 2, slots 1,2 expired, 3 not. Kind 0 has count 2 and a callback.
    set_word(node_word(2), 0, 0);
    make(2, 1, true, 0, 0); make(2, 2, true, 0, 0); make(2, 3, false, 0, 0);
    set_word(system, 0, low(node_word(2))); set_word(system, 4, low(node_word(2)));
    set_word(system, 8, 1); set_word(system, 12, 5);
    // Entry 1: head == tail and slot == end on entry: retired at once.
    set_word(node_word(5), 0, 0);
    set_word(system, 16, low(node_word(5))); set_word(system, 20, low(node_word(5)));
    set_word(system, 24, 7); set_word(system, 28, 7);
    // Entry 2: two nodes 8 -> 9, slots 253 and 254 expired, then node 9 slot 1 is live.
    // Kind 1 has no callback and count 1.
    set_word(node_word(8), 0, low(node_word(9))); set_word(node_word(9), 0, 0);
    make(8, 253, true, 2, 1); make(8, 254, true, 2, 1); make(9, 1, false, 2, 1);
    set_word(system, 32, low(node_word(8))); set_word(system, 36, low(node_word(9)));
    set_word(system, 40, 253); set_word(system, 44, 257);
    // Entry 3: slot 30 expired, slots 31 and 32 would be expired but are skipped, 33 is live.
    set_word(node_word(11), 0, 0);
    make(11, 30, true, 3, 1); make(11, 31, true, 3, 1); make(11, 32, true, 3, 1); make(11, 33, false, 3, 1);
    set_word(system, 48, low(node_word(11))); set_word(system, 52, low(node_word(11)));
    set_word(system, 56, 30); set_word(system, 60, 40);

    const int result = updatePS__FP14ParticleSystem(system);
    expect(result == 1, "a tested particle gives result 1");
    expect(calls.size() == 2 && calls[0] == low(particle(2, 1)) && calls[1] == low(particle(2, 2)), "callback per expired particle, in order");
    expect(half(particle(2, 1), 14) == 0 && half(particle(2, 2), 14) == 0 && half(particle(2, 3), 14) != 0, "expired halfword cleared");
    expect(word(desc[0], 8) == 0u && word(desc[0], 4) == 0xffffffffu && word(descListMark, 0) == 0xffffffffu,
           "count reaching zero resets the mark and slot");
    expect(word(system, 0) == low(node_word(2)) && word(system, 8) == 3u, "entry 0 resumes at slot 3");
    expect(word(system, 16) == 0u && word(system, 20) == 0u && word(system, 24) == 1u && word(system, 28) == 257u, "entry 1 retired");
    expect(word(node_word(5), 0) == 0x5000u, "retired node links to the old free list");
    // Entry 2 pushed node 8 on the free list at slot 255, then retired nothing.
    expect(word(node_word(8), 0) == low(node_word(5)), "advanced node is pushed on the free list that entry 1 left");
    expect(word(freeBlocks) == low(node_word(8)), "free list head is the advanced node");
    expect(word(system, 32) == low(node_word(9)) && word(system, 40) == 1u, "entry 2 continues at node 9 slot 1");
    expect(word(system, 56) == 33u, "entry 3 skips slots 31 and 32");
    expect(half(particle(11, 31), 14) != 0 && half(particle(11, 32), 14) != 0, "skipped slots are not processed");
    // Group 1 and later are empty; a second call over an empty system returns 0.
    unsigned char empty[256];
    std::memset(empty, 0, sizeof empty);
    calls.clear();
    expect(updatePS__FP14ParticleSystem(empty) == 0 && calls.empty(), "empty system returns 0");
    if (failures) return 1;
    std::printf("update-ps contract passed\n");
    return 0;
}
