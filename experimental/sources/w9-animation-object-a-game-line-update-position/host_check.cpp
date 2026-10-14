// Host contract for gameLineUpdatePosition__FP17gameLinePathState. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <cmath>
#include <cstring>
#include <string>
#include <strings.h>
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
// The line table holds host pointers to line records that live in the arena.
extern "C" int gameLineCount = 0;
extern "C" unsigned char* gameLines[4] = {};
static std::vector<std::string> events;
static std::vector<float> distances;
static unsigned distance_index;
static float next_point[3];
static int init_index;
extern "C" int host_strcasecmp(const char* a, const char* b) { events.push_back(std::string("strcasecmp ") + a + "," + b); return ::strcasecmp(a, b); }
extern "C" void initPosition__FP17gameLinePathState(unsigned char* s) { events.push_back("init"); sw_(s + 32, (unsigned)init_index); s[17] = 1; }
extern "C" float distanceToPlane__FP6Point3N20(const void* point, const void*, const void*) {
    events.push_back("plane " + std::to_string((long)((const unsigned char*)point - arena_at(0))));
    return distances.at(distance_index++);
}
extern "C" void gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(unsigned char* s, int step, float* out) {
    events.push_back("next " + std::to_string(step) + " idx " + std::to_string((int)hw(s + 32)));
    out[0] = next_point[0]; out[1] = next_point[1]; out[2] = next_point[2];
}
static float heading_y, heading_x;
extern "C" int iatan2__Fff(float y, float x) { heading_y = y; heading_x = x; events.push_back("iatan2"); return 0x12345; }
#define strcasecmp host_strcasecmp
#include "game_line_update_position.cpp"
#undef strcasecmp
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
int main() {
    // Target addresses are stored in the records; the pointer table maps them to arena bytes.
    unsigned char* state = arena_at(0x100);
    unsigned char* position = arena_at(0x200);
    unsigned char* text = arena_at(0x300);
    unsigned char* line = arena_at(0x1000);
    unsigned char* points = arena_at(0x2000);
    unsigned char* name = arena_at(0x1800);
    gameLines[0] = line;
    gameLineCount = 1;
    const unsigned N = 64;
    auto reset = [&](int line_index, int point_index, int lap, const char* tag, unsigned b16 = 0) {
        std::memset(state, 0x77, N);
        sw_(state + 0, norrath_host_address(position));
        sw_(state + 20, norrath_host_address(text));
        sw_(state + 28, (unsigned)line_index); sw_(state + 32, (unsigned)point_index); sw_(state + 36, (unsigned)lap);
        state[16] = b16; state[17] = 0x77;
        std::strcpy((char*)text, tag);
        std::memset(line, 0x33, 64);
        std::strcpy((char*)name, "Path");
        sw_(line, norrath_host_address(name)); sw_(line + 4, 4); sf_(line + 16, 2.0f);
        sw_(line + 20, norrath_host_address(points));
        for (int p = 0; p < 4; ++p) { std::memset(points + 64 * p, 0, 64); sf_(points + 64 * p + 48, 0.5f); sf_(points + 64 * p + 52, 0.25f * (p + 1)); }
        sf_(position, 1.0f); sf_(position + 4, 2.0f); sf_(position + 8, 3.0f);
        events.clear(); distances.clear(); distance_index = 0;
        next_point[0] = 4.0f; next_point[1] = 6.0f; next_point[2] = 3.0f;   // from (1,2,3): (3,4,0)/5 -> (0.6,0.8,0)
        init_index = 2;
    };
    // Unknown line name: index -1 is stored and 0 returned; nothing else happens.
    reset(-1, 0, 0, "Missing");
    check(gameLineUpdatePosition__FP17gameLinePathState(state) == 0, "unknown name returns 0");
    check((int)hw(state + 28) == -1 && state[17] == 0 && events.size() == 1, "unknown name stores -1");
    // Case-insensitive lookup, then initPosition (sets +32 and byte 17), then the update proper.
    reset(-1, 0, 0, "pATH");
    distances = {0.0f, 0.0f};
    check(gameLineUpdatePosition__FP17gameLinePathState(state) == 1, "found returns 1");
    check(hw(state + 28) == 0 && events[0] == "strcasecmp Path,pATH" && events[1] == "init", "lookup then init");
    check(hw(state + 32) == 2, "init index kept when the plane test holds still");
    // Count 0: no lines to search.
    reset(-1, 0, 0, "Path");
    gameLineCount = 0;
    check(gameLineUpdatePosition__FP17gameLinePathState(state) == 0 && (int)hw(state + 28) == -1 && events.empty(), "no lines");
    gameLineCount = 1;

    // Advance: threshold 0.5 <= distance 0.5 moves forward and wraps at the point count.
    reset(0, 3, 4, "Path");
    distances = {0.5f, 1.0f};
    check(gameLineUpdatePosition__FP17gameLinePathState(state) == 1, "advance returns 1");
    check(hw(state + 32) == 0 && hw(state + 36) == 5, "advance wraps to point 0, lap count up");
    check(hw(state + 48) == norrath_host_address(points), "current record address");
    check(state[16] == 1 && hw(state + 52) == norrath_host_address(points + 64), "changed: next record is the following point");
    check(lf_(state + 44) == (float)(5 / 4) + (0.25f + 1.0f) / 2.0f, "fraction");
    check(std::fabs(lf_(state + 4) - 0.6f) < 1e-6f, "unit x");
    check(std::fabs(lf_(state + 8) - 0.8f) < 1e-6f && lf_(state + 12) == 0.0f, "unit y and z");
    check(heading_y == lf_(state + 8) && heading_x == lf_(state + 4) && (hw(state + 40) & 0xffff) == 0x2345, "heading from iatan2(y, x), low half stored");
    check(events.back() == "iatan2", "iatan2 is the last call");
    // Retreat: distance below zero with a threshold above it moves back, wrapping from 0 to count-1.
    reset(0, 0, 0, "Path");
    distances = {-1.0f, 0.5f};
    gameLineUpdatePosition__FP17gameLinePathState(state);
    check(hw(state + 32) == 3 && (int)hw(state + 36) == -1, "retreat wraps to the last point");
    check(hw(state + 44) == 0, "negative lap count zeroes +44");
    check(hw(state + 48) == norrath_host_address(points + 3 * 64), "record after retreat");
    // Standing still: no change in index; byte 16 clear; no next-point call; +44 refreshed.
    reset(0, 1, 2, "Path", 0xff);
    distances = {0.25f, 0.5f};
    gameLineUpdatePosition__FP17gameLinePathState(state);
    check(hw(state + 32) == 1 && hw(state + 36) == 2 && state[16] == 0 && state[17] == 0, "no change");
    check(std::string(events.back()).rfind("next", 0) != 0 && events.size() == 2, "no next-point call");
    check(lf_(state + 44) == (float)(2 / 4) + (0.5f + 0.5f) / 2.0f, "fraction when still");
    check(hw(state + 52) == 0x77777777u && lf_(state + 4) == lf_(state + 4) && state[4] == 0x77, "heading fields untouched");
    // Zero distance with threshold above it and not below zero: neither branch.
    reset(0, 2, 0, "Path");
    distances = {0.0f, 0.0f};
    gameLineUpdatePosition__FP17gameLinePathState(state);
    check(hw(state + 32) == 2 && hw(state + 36) == 0, "neither branch");
    // Two-step: lap division is signed integer division.
    reset(0, 1, 9, "Path");
    distances = {0.0f, 0.0f};
    gameLineUpdatePosition__FP17gameLinePathState(state);
    check(lf_(state + 44) == (float)(9 / 4) + (0.5f + 0.0f) / 2.0f, "lap quotient");
    if (failures) return 1;
    std::printf("gameLineUpdatePosition check passed\n");
    return 0;
}
