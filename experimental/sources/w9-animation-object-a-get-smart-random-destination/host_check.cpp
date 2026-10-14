// Host contract for getSmartRandomDestination__8Creaturei. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
extern "C" void* world = reinterpret_cast<void*>(0x1234);
static std::vector<unsigned> rand_values;
static std::vector<float> floor_values;
static std::vector<std::vector<float>> find_points;
static std::vector<float> find_extents;
static void* find_world;
static unsigned rand_index, floor_index, rand_calls;
static void* rand_self;
extern "C" unsigned eRand__10GameObject(void* self) { ++rand_calls; rand_self = self; return rand_values.at(rand_index++); }
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* w, const void* p, float extent) {
    const float* f = static_cast<const float*>(p);
    find_points.push_back({f[0], f[1], f[2]});
    find_extents.push_back(extent);
    find_world = w;
    return floor_values.at(floor_index++);
}
#include "get_smart_random_destination.cpp"
static unsigned char* self = arena_at(0x100);
static unsigned char* out = arena_at(0x900);
static void reset(float px, float py, float pz, float hx, float hy, float hz, float radius, unsigned flags, float extent) {
    std::memset(self, 0x11, 1024);
    sf_(self + 16, px); sf_(self + 20, py); sf_(self + 24, pz);
    sf_(self + 776, hx); sf_(self + 780, hy); sf_(self + 784, hz);
    sf_(self + 788, radius); sw_(self + 160, flags); sf_(self + 28, extent);
    find_points.clear(); find_extents.clear(); rand_index = floor_index = rand_calls = 0;
    std::memset(out, 0xee, 32);
}
int main() {
    int failures = 0;
    auto check = [&](bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } };
    auto result_is = [&](float x, float y, float z) { return lf_(out) == x && lf_(out + 4) == y && lf_(out + 8) == z; };
    unsigned char before[1024];

    // Near home, accepted on the first try. reach = (int)(100 * 0.707) = 70, mask = 140.
    reset(0, 0, 5, 10, 20, 30, 1000, 0, 2.5f);
    rand_values = {200, 77}; floor_values = {5.0f};
    std::memcpy(before, self, 1024);
    unsigned char* r = getSmartRandomDestination__8Creaturei(out, self, 100);
    check(r == out, "returns the result pointer");
    check(result_is(10.0f + (float)((200 & 140) - 70), 20.0f + (float)((77 & 140) - 70), 30.0f), "first candidate");
    check(rand_calls == 2 && rand_self == self, "two rand calls on the creature");
    check(find_points.size() == 1 && find_points[0][0] == lf_(out) && find_points[0][1] == lf_(out + 4) && find_points[0][2] == 30.0f, "stand height point");
    check(find_extents[0] == 2.5f && find_world == world, "stand height extent and world");
    check(same("creature untouched", self, before, 1024) == 0, "creature untouched");
    check(same("out beyond 12 bytes", out + 12, std::vector<unsigned char>(20, 0xee).data(), 20) == 0, "result is 12 bytes");

    // Floor outside the height window rejects the attempt; strict comparisons at +-12.
    reset(0, 0, 5, 0, 0, 0, 1000, 0, 1);
    rand_values = {1, 1, 2, 2, 3, 3, 4, 4};
    floor_values = {17.0f, -7.0f, 16.99f, 5.0f};   // 5+12 rejected, 5-12 rejected, inside accepted
    getSmartRandomDestination__8Creaturei(out, self, 100);
    check(find_points.size() == 3 && rand_calls == 6, "third attempt accepted");
    check(result_is((float)((3 & 140) - 70), (float)((3 & 140) - 70), 0.0f), "third candidate returned");
    // Ten rejected attempts: the tenth candidate is returned.
    reset(0, 0, 5, 0, 0, 0, 1000, 0, 1);
    rand_values.clear(); floor_values.clear();
    for (unsigned i = 0; i < 40; ++i) rand_values.push_back(i * 7 + 1);
    for (unsigned i = 0; i < 12; ++i) floor_values.push_back(100.0f);
    getSmartRandomDestination__8Creaturei(out, self, 100);
    check(find_points.size() == 10 && rand_calls == 20, "gives up after ten");
    check(result_is((float)((int)(rand_values[18] & 140) - 70), (float)((int)(rand_values[19] & 140) - 70), 0.0f), "tenth candidate returned");

    // Radius smaller than range replaces it: radius 30.9 -> limit 30 -> reach 21 (30*0.707 = 21.2), mask 42.
    reset(0, 0, 0, 0, 0, 0, 30.9f, 0, 1);
    rand_values = {255, 255}; floor_values = {0.0f};
    getSmartRandomDestination__8Creaturei(out, self, 100);
    check(result_is((float)((255 & 42) - 21), (float)((255 & 42) - 21), 0.0f), "radius clamps the range");
    // Range smaller than radius is kept: range 10 -> reach 7 (7.07), mask 14.
    reset(0, 0, 0, 0, 0, 0, 500.0f, 0, 1);
    rand_values = {255, 255}; floor_values = {0.0f};
    getSmartRandomDestination__8Creaturei(out, self, 10);
    check(result_is((float)((255 & 14) - 7), (float)((255 & 14) - 7), 0.0f), "range kept");

    // Far from home: reach*reach <= d2 returns H without any call.
    reset(100, 0, 0, 0, 0, 0, 1000, 0, 1);
    getSmartRandomDestination__8Creaturei(out, self, 100);   // reach 70, 4900 <= 10000
    check(rand_calls == 0 && find_points.empty() && result_is(0, 0, 0), "far returns home");
    // Exactly at the limit counts as far (<=): P-H = (70,0,0), reach 70.
    reset(70, 0, 0, 5, 6, 7, 1000, 0, 1);
    sf_(self + 16, 75);
    getSmartRandomDestination__8Creaturei(out, self, 100);
    check(rand_calls == 0 && result_is(5, 6, 7), "equal distance returns home");
    // One unit inside tries to wander.
    reset(5 + 69.9f, 6, 7, 5, 6, 7, 1000, 0, 1);
    rand_values = {0, 0}; floor_values = {7.0f};
    getSmartRandomDestination__8Creaturei(out, self, 100);
    check(rand_calls == 2, "inside tries");
    // Flag 0x400000: far beyond 10000 squared units returns home even when well within reach.
    reset(150, 0, 0, 0, 0, 0, 1000, 0x400000, 1);
    getSmartRandomDestination__8Creaturei(out, self, 1000);   // reach 707, 499849 > 22500
    check(rand_calls == 0 && result_is(0, 0, 0), "flag, d2 above 10000");
    reset(150, 0, 0, 0, 0, 0, 1000, 0x400000, 1);
    sf_(self + 16, 100);   // d2 == 10000 is not above 10000
    rand_values = {0, 0}; floor_values = {0.0f};
    getSmartRandomDestination__8Creaturei(out, self, 1000);
    check(rand_calls == 2, "flag, d2 == 10000 wanders");
    reset(150, 0, 0, 0, 0, 0, 1000, 0x3bffff, 1);
    rand_values = {0, 0}; floor_values = {0.0f};
    getSmartRandomDestination__8Creaturei(out, self, 1000);
    check(rand_calls == 2, "other flag bits do not count");
    // Zero range: reach 0, 0 <= d2 always, so home.
    reset(1, 2, 3, 1, 2, 3, 1000, 0, 1);
    getSmartRandomDestination__8Creaturei(out, self, 0);
    check(rand_calls == 0 && result_is(1, 2, 3), "zero range is home");
    if (failures) return 1;
    std::printf("getSmartRandomDestination check passed\n");
    return 0;
}
