#include "recall_effect_ctor.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

unsigned char vt_12RecallEffect[16] __asm__("_vt$12RecallEffect");

static std::vector<const char*> order;
static unsigned point_seen[3];
static unsigned flags_at_alloc;
static unsigned char* current;
static void* allocation;
static unsigned cylinder_point[3];
static int cylinder_ints[4];
static void* cylinder_block;

extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    order.push_back("construct");
    std::memcpy(point_seen, point, 12);
    if (resource_map != nullptr || arguments != nullptr) std::fprintf(stderr, "fail: construct args\n");
    // The base constructor leaves a flags word and the cylinder centre words for the body to read.
    unsigned* words = static_cast<unsigned*>(object);
    words[3] = 0x00100000u;
    words[4] = 0x11111111u;
    words[5] = 0x22222222u;
    words[6] = 0x33333333u;
    return 0;
}
extern "C" void* blockAlloc__Fi(int bytes) {
    order.push_back("alloc");
    if (bytes != 96) std::fprintf(stderr, "fail: allocation size\n");
    flags_at_alloc = reinterpret_cast<unsigned*>(current)[3];
    return allocation;
}
extern "C" void __15GlowingCylinderG6Point3iiii(void* cylinder, const void* point, int a, int b, int c, int d) {
    order.push_back("cylinder");
    cylinder_block = cylinder;
    std::memcpy(cylinder_point, point, 12);
    cylinder_ints[0] = a;
    cylinder_ints[1] = b;
    cylinder_ints[2] = c;
    cylinder_ints[3] = d;
}

int main() {
    alignas(16) unsigned char object[128];
    unsigned char block[96];
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    std::memset(object, 0xcc, sizeof object);
    current = object;
    allocation = block;
    const unsigned point[3] = {0x3f800000u, 0x40000000u, 0x40400000u};
    __12RecallEffectG6Point3(object, point);
    const unsigned* words = reinterpret_cast<const unsigned*>(object);
    expect(order.size() == 3 && std::strcmp(order[0], "construct") == 0 && std::strcmp(order[1], "alloc") == 0 &&
               std::strcmp(order[2], "cylinder") == 0, "call order");
    expect(std::memcmp(point_seen, point, 12) == 0, "point passed to the base constructor");
    expect(words[18] == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_12RecallEffect)), "vtable at +72");
    expect(flags_at_alloc == (0x00100000u | 0x4009u) && words[3] == flags_at_alloc, "flags stored before the allocation");
    expect(cylinder_block == block, "cylinder gets the allocated block");
    expect(cylinder_point[0] == 0x11111111u && cylinder_point[1] == 0x22222222u && cylinder_point[2] == 0x33333333u,
           "cylinder centre is the three words at +16");
    expect(cylinder_ints[0] == 70 && cylinder_ints[1] == 86 && cylinder_ints[2] == 126 && cylinder_ints[3] == 250,
           "cylinder integers");
    expect(object[84] == 1 && words[20] == 0u && words[22] == 0u && words[23] == 0x40d0b2e4u, "trailing stores");
    expect(object[85] == 0xcc && object[86] == 0xcc && words[19] != 0u, "byte +84 does not touch its neighbours");

    // An allocation failure skips the cylinder and still finishes the object.
    order.clear();
    allocation = nullptr;
    std::memset(object, 0xcc, sizeof object);
    __12RecallEffectG6Point3(object, point);
    expect(order.size() == 2 && std::strcmp(order[1], "alloc") == 0, "no cylinder without a block");
    expect(object[84] == 1 && words[20] == 0u && words[22] == 0u && words[23] == 0x40d0b2e4u, "stores after failed allocation");
    if (bad) return 1;
    std::puts("recall effect constructor contract passed");
    return 0;
}
