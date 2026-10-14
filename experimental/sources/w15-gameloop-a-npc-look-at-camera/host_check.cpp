// Host check for npcLookAtCamera__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "npc_look_at_camera.cpp"

unsigned targetObjs[10];
float worldCameraPos[3];
static int calls;
static float seenA[10], seenB[10];
extern "C" int iatan2__Fff(float a, float b) {
    seenA[calls] = a; seenB[calls] = b;
    return 1000 * (calls++ + 1);
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    using namespace norrath_aoc;
    worldCameraPos[0] = 10.0f; worldCameraPos[1] = -4.0f; worldCameraPos[2] = 99.0f;
    for (int i = 0; i < 10; ++i) {
        unsigned char* object = arena_at(0x1000 + 0x100 * i);
        store_float(object + 16, 10.0f + i); store_float(object + 20, 2.0f * i); store_float(object + 24, 7.0f);
        store_half(object + 56, 0xffff);
        store_pointer(reinterpret_cast<unsigned char*>(&targetObjs[i]), object);
    }
    npcLookAtCamera__Fv();
    CHECK(calls == 10);
    for (int i = 0; i < 10; ++i) {
        unsigned char* object = arena_at(0x1000 + 0x100 * i);
        CHECK(seenA[i] == static_cast<float>(i) && seenB[i] == 2.0f * i + 4.0f);
        CHECK(static_cast<short>(load_half(object + 56)) == static_cast<short>(-16384 - 1000 * (i + 1)));
    }
    std::puts("ok");
    return 0;
}
