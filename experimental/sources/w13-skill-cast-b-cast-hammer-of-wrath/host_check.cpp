// Host contract for cast__18SkillHammerOfWrathiP8CreatureG6Point3s; not an EE input.
// The allocations, the three constructors, the link and the shared random state all log.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include "cast.cpp"
static std::string calls;
static unsigned char blocks[4][512];
static void* next_blocks[4];
static int block_index;
static std::string fl(float f) { return std::to_string(f); }
extern "C" {
unsigned syncRandState;
char vt_spell_base[48];
char vt_spell_hammer_of_wrath[48];
void* blockAlloc__Fi(int size) {
    calls += "alloc(" + std::to_string(size) + ");";
    return next_blocks[block_index++];
}
void glowing_shaft_ctor(void* self, Point3 p, int a, int b, int c, int d) {
    calls += "shaft(" + std::to_string(self == blocks[0]) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");";
}
void light_effect_ctor(void* self, Point3 p, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, void* object, int mode) {
    calls += "light(" + std::to_string(self == blocks[1]) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + fl(f1) + "," + fl(f2) + "," + fl(f3) + "," + fl(f4) + "," + fl(f5) + "," + fl(f6) + "," +
             fl(f7) + "," + fl(f8) + "," + fl(f9) + "," + fl(f10) + "," + fl(f11) + "," + fl(f12) + "," + std::to_string(object == nullptr) + "," + std::to_string(mode) + ");";
}
void hammer_ctor(void* self, Point3 p, short angle, void* creature, int rank, void* skill) {
    calls += "hammer(" + std::to_string(self == blocks[2]) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + std::to_string(angle) + "," + std::to_string(word_of(creature)) + "," + std::to_string(rank) + "," +
             std::to_string(word_of(skill)) + ");";
}
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* node, void* data) {
    calls += "link(" + std::to_string(list == blocks[3] + 24) + "," + std::to_string(node == blocks[2]) + "," + std::to_string(data == blocks[2]) + ");";
}
}
int main() {
    alignas(16) unsigned char creature[256];
    char skill;
    const int ranks[] = {-3, 0, 1, 99, 300};
    const unsigned seeds[] = {0u, 1u, 0x12345678u, 0xffffffffu};
    const float xs[] = {1.5f, -0.0f, 0.0f};
    for (int rank : ranks)
        for (unsigned seed : seeds)
            for (int mask = 0; mask < 16; ++mask)
                for (float x : xs) {
                    // mask bit 0..3: whether the allocation of shaft, light, hammer, object succeeds
                    std::memset(creature, 0x5a, sizeof creature);
                    std::memset(blocks, 0xcc, sizeof blocks);
                    for (int i = 0; i < 4; ++i) next_blocks[i] = (mask >> i) & 1 ? blocks[i] : nullptr;
                    block_index = 0;
                    syncRandState = seed;
                    calls.clear();
                    const short angle = static_cast<short>(rank * 37);
                    void* r = cast_hammer_of_wrath(&skill, rank, creature, Point3{x, 2.25f, 30.0f}, angle);
                    short group[8];
                    std::memcpy(group, creature + 140, 16);
                    const short want_group[8] = {-100, -100, -100, 0, -100, -100, -100, 0};
                    if (std::memcmp(group, want_group, 16) != 0) return 1;
                    for (int i = 0; i < 256; ++i) if ((i < 140 || i >= 156) && creature[i] != 0x5a) return 2;
                    std::string expected = "alloc(92);";
                    const float zx = x + 0.0f;
                    if (mask & 1) expected += "shaft(1," + fl(x) + "," + fl(2.25f) + "," + fl(30.0f) + ",90,86,126,250);";
                    expected += "alloc(188);";
                    if (mask & 2) expected += "light(1," + fl(zx) + "," + fl(2.25f) + "," + fl(66.0f) + "," + fl(0.0f) + "," + fl(155.0f) + "," + fl(255.0f) + "," + fl(-96.0f) + "," + fl(0.0f) + "," + fl(0.0f) + "," +
                                              fl(0.0f) + "," + fl(0.0f) + "," + fl(180.0f) + "," + fl(144.0f) + "," + fl(0.0f) + "," + fl(1.0f) + ",1,0);";
                    expected += "alloc(232);";
                    if (mask & 4) expected += "hammer(1," + fl(zx) + "," + fl(2.25f) + "," + fl(90.0f) + "," + std::to_string(angle) + "," + std::to_string(word_of(creature)) + "," + std::to_string(rank) + "," + std::to_string(word_of(&skill)) + ");";
                    unsigned expected_seed = seed;
                    void* want = nullptr;
                    if (mask & 4) {
                        expected += "alloc(28);";
                        if (mask & 8) {
                            expected_seed = seed * 0x19660du + 0x3c6ef35fu;
                            expected += "link(1,1,1);";
                            want = blocks[3];
                        }
                    }
                    if (r != want || calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); return 3; }
                    if (syncRandState != expected_seed) return 4;
                    if (want) {
                        unsigned char* o = blocks[3];
                        unsigned w[7];
                        std::memcpy(w, o, 28);
                        const unsigned head = w[0];
                        if ((head & 0xff) != static_cast<unsigned>(rank & 0xff)) return 5;
                        if (w[1] != 0 || w[2] != word_of(&skill) || w[3] != word_of(creature) || w[4] != (expected_seed >> 16) || w[5] != word_of(vt_spell_hammer_of_wrath) || w[6] != 0) return 6;
                        for (int i = 1; i < 4; ++i) if (o[i] != 0xcc) return 7;
                        if (o[28] != 0xcc) return 8;
                    }
                }
    return 0;
}
