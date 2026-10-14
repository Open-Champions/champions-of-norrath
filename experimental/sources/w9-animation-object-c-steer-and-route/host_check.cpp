// Host contract for steerAndRouteToPosition__8CreatureG6Point3. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <array>
#include <cmath>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "steer_and_route.cpp"
unsigned char* world = reinterpret_cast<unsigned char*>(0x4321);
int frameNm;
signed char cutMode;
static std::vector<std::string> ev;
static unsigned char* creature;
static std::string fl(float f) { char b[40]; std::snprintf(b, sizeof b, "%g", f); return b; }
static std::string pt(const float* p) { return fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2]); }
static int atan_result, turn_result, found_result;
static float clearance_result;
static float steer_result[3], current_route[3];
static std::vector<std::array<float, 3>> next_points;
static unsigned next_index;
static float spot_result[3];
extern "C" int iatan2__Fff(float y, float x) { ev.push_back("atan:" + fl(y) + "," + fl(x)); return atan_result; }
extern "C" float worldHitscan__FP12_worldHeaderG6Point3T1fi(unsigned char* w, const float* o, const float* d, float lim, int fl_) {
    ev.push_back("hit:" + pt(o) + ":" + pt(d) + ":" + fl(lim) + ":" + std::to_string(fl_) + (w == world ? "" : "!world"));
    return clearance_result;
}
extern "C" void wallAndObjectSteering__FP8CreatureffP10GameObject(float* out, unsigned char* self, float a, float b, unsigned char* obj) {
    ev.push_back("wall:" + fl(a) + "," + fl(b) + (obj ? ":obj" : ""));
    out[0] = steer_result[0]; out[1] = steer_result[1]; out[2] = steer_result[2];
}
extern "C" void getCurrentRoutePoint__8Creature(float* out, unsigned char*) { ev.push_back("cur"); out[0] = current_route[0]; out[1] = current_route[1]; out[2] = current_route[2]; }
extern "C" void getNextRoutePoint__8CreatureP6Point3G6Point3ffi(unsigned char* self, float* p, const float* t, float a, float b, int c) {
    ev.push_back("next:" + pt(p) + ":" + pt(t) + ":" + fl(a) + "," + fl(b) + "," + std::to_string(c) + ":t712=" + std::to_string(int(hw(self + 712))));
    static const std::array<float, 3> none = {-1.0f, 0.0f, 0.0f};
    const auto& n = next_points.empty() ? none : next_points[next_index < next_points.size() ? next_index : next_points.size() - 1];
    ++next_index;
    p[0] = n[0]; p[1] = n[1]; p[2] = n[2];
}
extern "C" float icos__Fi(int a) { return 0.5f; }
extern "C" float isin__Fi(int a) { return 0.25f; }
extern "C" int move2__8CreatureG6Point3(unsigned char*, const float* p) { ev.push_back("move:" + pt(p)); return 0; }
extern "C" unsigned char* objectFindEmptySpot__FG6Point3iiPc(float* out, const float* c, int spread, int r, char* found) {
    ev.push_back("spot:" + pt(c) + ":" + std::to_string(spread) + "," + std::to_string(r));
    out[0] = spot_result[0]; out[1] = spot_result[1]; out[2] = spot_result[2]; *found = static_cast<char>(found_result);
    return nullptr;
}
extern "C" void objectUpdateInGrid__FP10GameObject(unsigned char* o) { ev.push_back(std::string("grid") + (o == creature ? "" : "!self")); }
extern "C" int norrath_host_call_pointer_int_pointer_ret_int(unsigned fn, unsigned char* self, int second, const float* third) {
    ev.push_back("virt:" + std::to_string(fn) + ":" + std::to_string(second) + ":" + pt(third) + (self == creature - 16 ? "" : "!adjust"));
    return turn_result;
}
int main() {
    creature = arena_at(0x100);
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); for (auto& e : ev) std::printf("   %s\n", e.c_str()); ++failures; } };
    auto reset = [&]() {
        std::memset(arena_at(0), 0, 0x1000);
        sf_(creature + 16, 0); sf_(creature + 20, 0); sf_(creature + 24, 0); sf_(creature + 28, 10.0f);
        sh_(creature + 56, 0); sw_(creature + 72, norrath_host_address(arena_at(0x800)));
        sh_(arena_at(0x800) + 176, 0xfff0u); sw_(arena_at(0x800) + 180, 0x7777);
        sf_(creature + 176, 8.0f); sf_(creature + 180, 6.0f); sf_(creature + 184, -4.0f);
        creature[569] = 0; creature[570] = 0; creature[800] = 0;
        sw_(creature + 808, 1000); sw_(creature + 740, 1000); sw_(creature + 732, 1000); sw_(creature + 736, 0);
        sf_(creature + 720, 0); sf_(creature + 724, 0); sf_(creature + 728, 0);
        for (int i = 0; i != 3; ++i) { steer_result[i] = 0; current_route[i] = -1.0f; spot_result[i] = 50.0f + i; }
        frameNm = 0; cutMode = 0; atan_result = 100; turn_result = 0; clearance_result = 0.75f; found_result = 0;
        next_points.clear(); next_index = 0; ev.clear();
    };
    auto go = [&](float x, float y, float z) {
        unsigned char t[12]; sf_(t, x); sf_(t + 4, y); sf_(t + 8, z);
        return steerAndRouteToPosition__8CreatureG6Point3(creature, t);
    };
    // 1. Inside the radius: returns 1 and calls nothing.
    reset(); expect(go(5, 0, 0) == 1 && ev.empty(), "arrived inside the radius");
    reset(); expect(go(6, 6, 6) == 0 && !ev.empty(), "3D distance 10.39 is outside the radius");
    // 2. Close and behind: only on the 8th-frame phase.
    reset(); atan_result = 20000; expect(go(15, 0, 0) == 1 && ev.size() == 1 && ev[0] == "atan:0,15", "close target outside the cone counts as arrived");
    reset(); atan_result = -16385; expect(go(15, 0, 0) == 1, "magnitude 16385 counts as arrived");
    reset(); atan_result = 16384; expect(go(15, 0, 0) == 0, "magnitude 16384 does not");
    reset(); atan_result = 20000; creature[569] = 1; expect(go(15, 0, 0) == 0 && ev[0].rfind("hit:", 0) == 0, "off-phase frames skip the heading test");
    reset(); atan_result = 20000; sh_(creature + 56, 0xffffu); atan_result = 16383; expect(go(15, 0, 0) == 0, "heading is subtracted as a signed halfword (16383 - -1 = 16384)");
    reset(); atan_result = 20000; expect(go(25, 0, 0) == 0 && ev[0].rfind("hit:", 0) == 0, "beyond 2r the heading test is skipped");
    // 3. Hitscan arguments and the steering-driven route.
    reset(); steer_result[0] = 1; steer_result[1] = 2; steer_result[2] = 7;
    sf_(creature + 16, 3); sf_(creature + 20, 4); sf_(creature + 24, 5);
    expect(go(15, 4, 5) == 0, "far target returns 0");
    expect(ev.size() >= 5 && ev[0] == "atan:0,12" && ev[1] == "hit:3,4,18:12,0,0:1.1:3" && ev[2] == "wall:12,20", "hitscan origin is lifted by 13; steering reach is min(dist, r + 10)");
    expect(ev[3] == "virt:30583:0:4,6,12", "route is steering + position when both x and y are nonzero");
    expect(ev[4] == "move:4,2,0", "push is cos*speed, sin*speed, 0");
    // far target: reach = r + 10
    reset(); steer_result[0] = 1; steer_result[1] = 1; atan_result = 0;
    expect(go(30, 0, 0) == 0 && ev[1].rfind("wall:20,20", 0) == 0, "reach capped at r + 10");
    // Steering is not recomputed when byte570 is set or off-phase; the stored point is used.
    reset(); creature[570] = 1; sf_(creature + 760, 3); sf_(creature + 764, 4); sf_(creature + 768, 5);
    go(30, 0, 0); expect(ev[1] == "virt:30583:0:3,4,5", "stored steering used when +570 is set");
    reset(); creature[569] = 3; sf_(creature + 760, 3); sf_(creature + 764, 4);
    go(30, 0, 0); expect(ev[1] == "virt:30583:0:3,4,0" && ev[0].rfind("hit:", 0) == 0, "stored steering used off-phase");
    // The steering point is stored in the creature when recomputed.
    reset(); steer_result[0] = 2; steer_result[1] = 3; steer_result[2] = 4;
    go(30, 0, 0); expect(lf_(creature + 760) == 2 && lf_(creature + 764) == 3 && lf_(creature + 768) == 4, "steering stored at +760");
    // 4. Steering with a zero x or y falls to the route logic.
    reset(); steer_result[0] = 0; steer_result[1] = 5; current_route[0] = 30; current_route[1] = 40; current_route[2] = 1;
    sw_(creature + 712, 500);
    go(30, 0, 0);
    expect(ev[2] == "cur" && ev[3] == "virt:30583:0:30,40,1", "a valid current route point is used unchanged (planar distance 50 >= r, timer ahead)");
    reset(); current_route[0] = -1.0f; next_points = {{30, 40, 2}}; steer_result[0] = 0;
    go(30, 0, 0);
    expect(ev[2] == "cur" && ev[3] == "next:-1,-1,-1:30,0,0:10,0,0:t712=180" && ev[4] == "virt:30583:0:30,40,2" && int(hw(creature + 712)) == 180,
           "R.x == -1 asks for the next point after storing frame + 180 and passing r, 0.0, 0");
    reset(); current_route[0] = -1.0f; next_points = {{-1, 0, 0}}; steer_result[0] = 0;
    go(30, 0, 0);
    expect(ev[4] == "virt:30583:0:30,0,0", "R.x still -1 afterwards falls back to the target");
    reset(); current_route[0] = 2; current_route[1] = 1; next_points = {{3, 3, 3}}; steer_result[0] = 0;
    go(30, 0, 0);
    expect(ev[3].rfind("next:2,1,-1:", 0) == 0 || ev[3].rfind("next:2,1,", 0) == 0, "a route point inside r is replaced");
    reset(); current_route[0] = 2; current_route[1] = 1; next_points = {{3, 3, 3}}; steer_result[0] = 0;
    {
        go(30, 0, 0);
        int nexts = 0; for (auto& e : ev) if (e.rfind("next:", 0) == 0) ++nexts;
        expect(nexts == 16, "a point that keeps landing inside r is replaced exactly 16 times");
    }
    reset(); current_route[0] = 40; current_route[1] = 1; sw_(creature + 712, 10); frameNm = 100; creature[569] = 4; steer_result[0] = 0; next_points = {{50, 0, 0}};
    sw_(creature + 740, 0);   // keep clearance override off
    go(100, 0, 0);
    { int nexts = 0; for (auto& e : ev) if (e.rfind("next:", 0) == 0) ++nexts; expect(nexts == 1 && hw(creature + 712) == 280, "an expired timer (712 < frame) replaces the point once"); }
    // 5. Clearance gating of the route logic.
    reset(); clearance_result = 1.5f; current_route[0] = 30; steer_result[0] = 0; go(30, 0, 0);
    expect(ev.size() >= 2 && ev[2].rfind("virt:", 0) == 0 && ev[2].find(":30,0,0") != std::string::npos, "clearance above 1 skips the route lookup, R stays the target");
    reset(); clearance_result = -0.25f; steer_result[0] = 0; go(30, 0, 0);
    expect(ev[2].rfind("virt:", 0) == 0, "negative clearance skips the route lookup");
    reset(); clearance_result = 9.0f; sw_(creature + 808, 1000); sw_(creature + 740, 2000); sw_(creature + 736, 1); steer_result[0] = 0; current_route[0] = 30; current_route[1] = 40; go(30, 0, 0);
    expect(ev[2] == "cur", "a pending timer (808 < 740) with +736 set overrides clearance to 0.5");
    reset(); clearance_result = 0.5f; sw_(creature + 808, 1000); sw_(creature + 740, 2000); sw_(creature + 736, 0); steer_result[0] = 0; go(30, 0, 0);
    expect(ev[2].rfind("virt:", 0) == 0, "+736 clear overrides clearance to 1.5, skipping the route lookup");
    // 6. The virtual turn result and the speed.
    reset(); steer_result[0] = 1; steer_result[1] = 1; turn_result = 16001; go(30, 0, 0);
    expect(lf_(creature + 180) == 3.0f && lf_(creature + 184) == -2.0f && ev.back().rfind("move:2,1,0", 0) == 0, "|turn| >= 16001 halves +180, +184 and the speed");
    reset(); steer_result[0] = 1; steer_result[1] = 1; turn_result = -16001; go(30, 0, 0);
    expect(lf_(creature + 180) == 3.0f, "negative turn also halves");
    reset(); steer_result[0] = 1; steer_result[1] = 1; turn_result = 16000; go(30, 0, 0);
    expect(lf_(creature + 180) == 6.0f && ev.back().rfind("move:4,2,0", 0) == 0, "|turn| 16000 does not halve");
    reset(); steer_result[0] = 1; steer_result[1] = 1; turn_result = 0x12345; go(30, 0, 0);
    expect(lf_(creature + 180) == 6.0f, "the turn is only the low 16 bits (0x2345)");
    reset(); steer_result[0] = 1; steer_result[1] = 1; turn_result = 0x18000; go(30, 0, 0);
    expect(lf_(creature + 180) == 3.0f, "low 16 bits 0x8000 is -32768, magnitude 32768, halves");
    const float k = 0x1.111110p-6f;
    reset(); steer_result[0] = 1; steer_result[1] = 1; creature[800] = 30; go(30, 0, 0);
    { std::string want = "move:" + fl(0.5f * (8.0f * (30.0f * k))) + "," + fl(0.25f * (8.0f * (30.0f * k))) + ",0"; expect(ev.back() == want, "byte800 = 30 scales the speed by 30 * 0.016666668"); }
    reset(); steer_result[0] = 1; steer_result[1] = 1; creature[800] = 61; go(30, 0, 0); expect(ev.back() == "move:4,2,0", "byte800 = 61 is out of the 1..60 range");
    reset(); steer_result[0] = 1; steer_result[1] = 1; creature[800] = 0; go(30, 0, 0); expect(ev.back() == "move:4,2,0", "byte800 = 0 is out of range");
    reset(); steer_result[0] = 1; steer_result[1] = 1; creature[800] = 60; go(30, 0, 0); expect(ev.back() == "move:0,0,0", "byte800 = 60 scales to zero");
    reset(); steer_result[0] = 1; steer_result[1] = 1; creature[800] = 200; go(30, 0, 0); expect(ev.back() == "move:4,2,0", "byte800 = 200 is out of range");
    // 7. Stuck detection and the empty spot search.
    reset(); steer_result[0] = 1; steer_result[1] = 1; cutMode = 1; sf_(creature + 16, 9); sf_(creature + 20, 8); sf_(creature + 24, 7); sw_(creature + 808, 77);
    expect(go(30, 8, 7) == 0 && lf_(creature + 720) == 9 && lf_(creature + 724) == 8 && lf_(creature + 728) == 7 && hw(creature + 732) == 77, "cutMode set: Q = P, +732 = +808");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sf_(creature + 720, 3.0f); sw_(creature + 808, 55);
    go(30, 0, 0); expect(lf_(creature + 720) == 0.0f && hw(creature + 732) == 55, "5.0 < |P-Q|^2: Q = P, +732 = +808");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sf_(creature + 720, 2.0f); sf_(creature + 728, 1.0f); sw_(creature + 808, 55);
    go(30, 0, 0); expect(lf_(creature + 720) == 2.0f && hw(creature + 732) == 1000, "|P-Q|^2 = 5.0 exactly is not stuck");
    // a < b: sets the timer and +736
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 740, 900); sw_(creature + 732, 600); clearance_result = 0.5f;
    go(30, 0, 0); expect(hw(creature + 740) == 1240 && hw(creature + 736) == 0, "a < b and +732 < b - 300: +740 = b + 240, +736 = 0 for clearance 0.5");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 740, 900); sw_(creature + 732, 600); clearance_result = 1.25f;
    go(30, 0, 0); expect(hw(creature + 740) == 1240 && hw(creature + 736) == 1, "+736 = 1 for clearance above 1");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 740, 900); sw_(creature + 732, 700); clearance_result = 0.5f;
    go(30, 0, 0); expect(hw(creature + 740) == 900 && hw(creature + 736) == 0, "+732 = b - 300 does not trigger");
    // a == b
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 732, 699); found_result = 1;
    sf_(creature + 16, 0); go(30, 0, 0);
    expect(ev.size() >= 6 && ev[ev.size() - 2] == "spot:0,0,0:72,10" && ev.back() == "grid" && lf_(creature + 16) == 50 && lf_(creature + 20) == 51 && lf_(creature + 24) == 52, "a == b and +732 < a - 300: a found spot moves the creature and updates the grid");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 732, 699); found_result = 0; go(30, 0, 0);
    expect(ev.back() == "spot:0,0,0:72,10" && lf_(creature + 16) == 0, "no spot found: nothing moves");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 732, 700); found_result = 1; go(30, 0, 0);
    expect(ev.back().rfind("move:", 0) == 0, "+732 = a - 300 does not search");
    reset(); steer_result[0] = 1; steer_result[1] = 1; sw_(creature + 740, 1100); sw_(creature + 732, 0); found_result = 1; go(30, 0, 0);
    expect(ev.back().rfind("move:", 0) == 0 && hw(creature + 740) == 1100, "a > b does nothing");
    if (failures) return 1;
    std::printf("steerAndRouteToPosition check passed\n");
    return 0;
}
