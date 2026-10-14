// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "passable.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
unsigned char norrath_factory_world_pointer_word[4];
static char log_text[64];
static float heights[4];
static float points[4][3];
static int shifts[4];
static int indices[8];
static void* worlds[8];
static int steps = 0, moves = 0, heights_n = 0;
static int collision_value = 0;
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int value) {
    log_text[steps] = 'c'; worlds[steps] = world; indices[steps++] = index; collision_value = value;
}
extern "C" void worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short shift) {
    log_text[steps] = 'm'; worlds[steps] = world; indices[steps++] = index;
    std::memcpy(points[moves], point, 12); shifts[moves++] = shift;
}
extern "C" void worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height) {
    log_text[steps] = 'h'; worlds[steps] = world; indices[steps++] = index; heights[heights_n++] = height;
}
int main() {
    int bad = 0;
    int world_object;
    rtti_c::host_note(&world_object);
    rtti_c::write_word(rtti_c::address(norrath_factory_world_pointer_word), rtti_c::address_bits(&world_object));
    for (unsigned state = 0; state < 5; ++state) {
        unsigned char object[420];
        std::memset(object, 0, sizeof object);
        rtti_c::write_word(rtti_c::address(object) + 376u, state);
        rtti_c::write_word(rtti_c::address(object) + 396u, 7u);
        rtti_c::write_single(rtti_c::address(object) + 400u, 1000.0f);
        steps = moves = heights_n = 0; std::memset(log_text, 0, sizeof log_text);
        setPassableState__8GothTrapb(object, state & 1u);
        const char* want = (state <= 2) ? "cmh" : "c";
        if (std::strcmp(log_text, want) != 0) { std::printf("state %u order %s\n", state, log_text); ++bad; }
        for (int i = 0; i < steps; ++i) if (worlds[i] != &world_object || indices[i] != 7) { std::printf("world/index\n"); ++bad; }
        if (collision_value != 1) { std::printf("collision value\n"); ++bad; }
        if (state == 0 && (heights[0] != 1000.0f || points[0][0] != 0 || points[0][1] != 0 || points[0][2] != 0 || shifts[0] != 0)) { std::printf("state 0 values\n"); ++bad; }
        if (state == 1 && (heights[0] != 650.0f || points[0][2] != -350.0f || shifts[0] != 0)) { std::printf("state 1 values\n"); ++bad; }
        if (state == 2 && (heights[0] != 1256.0f || points[0][2] != -350.0f || shifts[0] != 0)) { std::printf("state 2 values\n"); ++bad; }
        if (state == 2 && (points[0][0] != 0 || points[0][1] != 0)) { std::printf("state 2 point\n"); ++bad; }
    }
    std::printf(bad ? "failed\n" : "setPassableState__8GothTrapb passed\n");
    return bad != 0;
}
