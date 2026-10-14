// Host check for statsFrame__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "stats_frame.cpp"

unsigned char statsScreen[232];
unsigned char player[23984 * 4];
int statsDrawDelay;
unsigned marble, stage;
unsigned char animState[400];

static std::vector<std::string> events;
static void* lumpResult;
extern "C" {
void* lumpQuery__FPc(const char* name) { events.push_back(std::string("lump:") + name); return lumpResult; }
void inventoryLoad__Fv() { events.push_back("load"); }
void lightSetSceneLights__FG6Point3iiiiiiiii(const void* d, int a, int b, int c, int dd, int e, int f, int g, int h, int i) {
    const float* v = static_cast<const float*>(d);
    events.push_back("light:" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," + std::to_string(v[2]) + ":" + std::to_string(a) + "," +
                     std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(dd) + "," + std::to_string(e) + "," + std::to_string(f) +
                     "," + std::to_string(g) + "," + std::to_string(h) + "," + std::to_string(i));
}
void matIdent__FP8Matrix34(void*) { events.push_back("ident"); }
void matRot__FP8Matrix34is(void*, int axis, short angle) { events.push_back("rot:" + std::to_string(axis) + "," + std::to_string(angle)); }
void worldSetViewMatricies__FP6Point3P8Matrix34(const void* p, void*) {
    const float* v = static_cast<const float*>(p);
    events.push_back("view:" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," + std::to_string(v[2]));
}
void inventoryDrawPlayer__FiiP14AnimationStatei(int slot, int a, void* anim, int b) {
    events.push_back("player:" + std::to_string(slot) + "," + std::to_string(a) + "," +
                     std::to_string(static_cast<unsigned char*>(anim) - animState) + "," + std::to_string(b));
}
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned color) {
    events.push_back("tex:" + std::to_string(norrath_host_address(static_cast<unsigned char*>(t))) + "," + std::to_string(x) + "," + std::to_string(y) +
                     "," + std::to_string(z) + "," + std::to_string(color));
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    using namespace norrath_aoc;
    unsigned char* tex = arena_at(0x100);
    store_half(tex + 2, 64);
    marble = norrath_host_address(tex); stage = norrath_host_address(arena_at(0x400));
    lumpResult = nullptr;
    statsFrame__Fv();
    CHECK((events == std::vector<std::string>{"lump:inventry.lmp", "load"}));
    events.clear(); lumpResult = tex; statsDrawDelay = 2;
    statsFrame__Fv();
    CHECK(statsDrawDelay == 1 && events.size() == 1);
    events.clear(); statsDrawDelay = 0;
    store_word(statsScreen + 52, 2);
    player[23984 * 2 + 241] = 0xff;
    statsFrame__Fv();
    CHECK(events.size() == 2 && events[1].rfind("light:", 0) == 0 && load_word(statsScreen + 52) == 2);   // no slot: stops after the light call
    events.clear();
    player[23984 * 2 + 241] = 3;
    statsFrame__Fv();
    const unsigned t = norrath_host_address(tex), s = norrath_host_address(arena_at(0x400));
    std::vector<std::string> want = {"lump:inventry.lmp", events[1], "ident", "rot:0,16384", "rot:2,0", "view:0.000000,-108.000000,0.000000", "player:3,1,120,0",
        "tex:" + std::to_string(t) + ",0,0,0,2155905152", "tex:" + std::to_string(t) + ",0,64,0,2155905152", "tex:" + std::to_string(t) + ",0,128,0,2155905152",
        "tex:" + std::to_string(s) + ",320,0,0,2155905152"};
    CHECK(events == want);
    CHECK(events[1] == "light:-0.500000,-0.500000,0.500000:101,107,78,0,38,40,48,0,128");
    CHECK(load_word(statsScreen + 52) == 3);
    std::puts("ok");
    return 0;
}
