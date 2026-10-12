// Host contract for P_AddParticle__FP11ParticleDefP6Point3T1f. Not an EE plan input.
// Pointer words are 32-bit, so all words that hold addresses point into one static image.
#include <cstdio>
#include <cstring>

struct Point3 { float x, y, z; };
extern "C" {
unsigned char currentPS[4] = {0};
unsigned char freeBlocks[4] = {0};
unsigned char blockList[512] = {0};
unsigned char block[128 * 4096] = {0};
unsigned short P_FrameNum = 0;
void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* definition, Point3* position, Point3* velocity, float age);
}

static unsigned char arena[4096];   // system at 0, definition at 512
static int failures = 0;
static int can_alloc = 1, can_alloc_calls = 0, alloc_calls = 0;
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
static int half(const void* base, unsigned offset) {
    short v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 2);
    return v;
}
extern "C" int checkCanAllocDescriptor__Fv() { ++can_alloc_calls; return can_alloc; }
extern "C" int allocDescriptor__FP11ParticleDef(unsigned char* definition) {
    ++alloc_calls;
    set_word(definition, 4, 7u);
    return 1;
}

static unsigned char* const system_base = arena;
static unsigned char* const def = arena + 512;

static void reset(unsigned type) {
    std::memset(arena, 0, sizeof arena);
    std::memset(blockList, 0, sizeof blockList);
    set_word(currentPS, 0, low(system_base));
    set_word(def, 0, type);
    set_word(def, 4, 5u);
    set_word(def, 8, 3u);   // descriptor already counted
    for (unsigned entry = 0; entry < 16; ++entry) set_word(system_base, entry * 16 + 12, 257u);
    // free list: block 3 -> block 5 -> end
    set_word(blockList, 3 * 4, low(blockList + 5 * 4));
    set_word(blockList, 5 * 4, 0u);
    set_word(freeBlocks, 0, low(blockList + 3 * 4));
    P_FrameNum = 1000;
    can_alloc = 1;
    can_alloc_calls = alloc_calls = 0;
}
static unsigned char* record(unsigned block_index, unsigned index) { return block + block_index * 4096 + index * 16; }

int main() {
    Point3 pos = {-10.5f, 20.75f, 30.9f}, vel = {1.5f, -2.0f, 0.25f};
    reset(2);
    unsigned char* entry = system_base + 2 * 16;
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(word(entry, 0) == low(blockList + 3 * 4) && word(entry, 4) == low(blockList + 3 * 4), "first and last take the new block");
    expect(word(freeBlocks) == low(blockList + 5 * 4), "free list advances");
    expect(word(entry, 12) == 2u, "index starts at 1 and advances to 2");
    expect(word(def, 8) == 4u, "use count increments");
    expect(can_alloc_calls == 0 && alloc_calls == 0, "descriptor calls skipped when the use count is nonzero");
    unsigned char* r = record(3, 1);
    expect(half(r, 0) == -10 && half(r, 2) == 20 && half(r, 4) == 30, "plain path stores truncated position");
    expect(half(r, 6) == 1000, "frame number");
    expect(half(r, 8) == 384 && half(r, 10) == -512 && half(r, 12) == 64, "velocity times 256");
    expect(half(r, 14) == 20, "descriptor slot shifted left by two");
    expect(word(system_base, 0) == 0u, "other entries untouched");

    // Index 30 jumps to 33 (the footer and next header are skipped).
    set_word(entry, 12, 30u);
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(word(entry, 12) == 33u, "index 30 advances to 33");
    expect(half(record(3, 30), 6) == 1000, "record 30 written");
    // Index 254 jumps to 257 (full).
    set_word(entry, 12, 254u);
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(word(entry, 12) == 257u, "index 254 advances to 257");
    // The next particle takes the second block and links the first to it.
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(word(blockList, 3 * 4) == low(blockList + 5 * 4), "old last block links to the new block");
    expect(word(entry, 4) == low(blockList + 5 * 4) && word(entry, 0) == low(blockList + 3 * 4), "last changes, first stays");
    expect(word(freeBlocks) == 0u && word(entry, 12) == 2u, "free list ends and index restarts");
    expect(half(record(5, 1), 6) == 1000, "record lands in the second block");

    // A full entry with nothing free stops without changes.
    set_word(entry, 12, 257u);
    const unsigned uses = word(def, 8);
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(word(entry, 12) == 257u && word(def, 8) == uses, "no free block means no change");

    // A zero use count asks permission first, and allocates a descriptor afterwards.
    reset(2);
    set_word(def, 8, 0u);
    can_alloc = 0;
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(can_alloc_calls == 1 && alloc_calls == 0 && word(entry, 12) == 257u && word(freeBlocks) != 0u, "refused allocation changes nothing");
    can_alloc = 1;
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos, &vel, 0.0f);
    expect(alloc_calls == 1 && word(def, 8) == 1u, "descriptor allocated, use count one");
    expect(half(record(3, 1), 14) == 28, "the new descriptor slot (7) is the one stored");

    // Age path.
    reset(0);
    Point3 pos2 = {100.0f, 200.0f, 300.0f}, vel2 = {2.0f, 4.0f, 6.0f};
    const float gravity = 0.0625f;
    std::memcpy(def + 24, &gravity, 4);
    P_AddParticle__FP11ParticleDefP6Point3T1f(def, &pos2, &vel2, 0.5f);
    r = record(3, 1);
    expect(half(r, 0) == 96 && half(r, 2) == 192 && half(r, 4) == 345, "back-stepped position");
    expect(half(r, 6) == 970, "frame number minus the step count");
    expect(half(r, 8) == 512 && half(r, 10) == 1024, "plain velocity");
    expect(half(r, 12) == -13824, "fall velocity times 256");
    if (failures != 0) return 1;
    std::printf("add-particle contract passed\n");
    return 0;
}
