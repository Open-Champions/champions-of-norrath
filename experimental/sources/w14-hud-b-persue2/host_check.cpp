// Host check for persue2__8CreatureP10GameObjectfff. Not an EE plan input.
// Build as x86_64 with statics below 4 GiB (the table holds a 32-bit function address):
//   c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
// Recording stubs stand in for the callees; the branches of the routine are driven one by one.
#include "persue2.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
_worldHeader* world;
int frameNm;
}
alignas(16) static unsigned char creature[0x400];
alignas(16) static unsigned char target[0x100];
alignas(16) static unsigned char table[0x300];
static std::string trace;
static float hitValue;
static short moveResult;
static Point3 steerResult;
static Point3 routeResult;
static int routeCalls;

static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", static_cast<double>(v)); return b; }
static std::string pt(const Point3& p) { return "(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + ")"; }
static float& at(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }

extern "C" {
float icos__Fi(int angle) { trace += "cos" + std::to_string(angle) + ";"; return 2.0f; }
float isin__Fi(int angle) { trace += "sin" + std::to_string(angle) + ";"; return 3.0f; }
Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(Point3* out, void* c, float reach, float second, void* ignore) {
    trace += std::string("wall(") + (c == creature ? "C" : "?") + "," + num(reach) + "," + num(second) + "," + (ignore == target ? "T" : "?") + ");";
    *out = steerResult;
    return out;
}
float worldHitscan__FP12_worldHeaderG6Point3T1fi(_worldHeader*, Point3 start, Point3 direction, float range, int mode) {
    trace += "hit(" + pt(start) + "," + pt(direction) + "," + num(range) + "," + std::to_string(mode) + ");";
    return hitValue;
}
void getNextRoutePoint__8CreatureP6Point3G6Point3ffi(void* c, Point3* route, Point3 to, float a, float b, int flag) {
    trace += std::string("route(") + (c == creature ? "C" : "?") + "," + (route == reinterpret_cast<Point3*>(creature + 260) ? "R" : "?") + "," + pt(to) + "," + num(a) + "," + num(b) + "," + std::to_string(flag) + ");";
    ++routeCalls;
    *route = routeResult;
}
void move2__8CreatureG6Point3(void* c, Point3 movement) {
    trace += std::string("move2(") + (c == creature ? "C" : "?") + "," + pt(movement) + ");";
}
}
static int moveEntry(void* self, int zero, Point3 destination) {
    trace += "move(" + std::to_string(static_cast<unsigned char*>(self) - creature) + "," + std::to_string(zero) + "," + pt(destination) + ");";
    return moveResult;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void reset() {
    std::memset(creature, 0, sizeof creature);
    std::memset(target, 0, sizeof target);
    std::memset(table, 0, sizeof table);
    trace.clear(); routeCalls = 0; moveResult = 0; hitValue = -1.0f;
    steerResult = {0, 0, 0}; routeResult = {-1.0f, 0, 0};
    // creature at the origin with span 4; target at (3, 4, 0): distance 5
    at(creature, 28) = 4.0f;
    at(target, 16) = 3.0f; at(target, 20) = 4.0f; at(target, 24) = 0.0f;
    *reinterpret_cast<unsigned*>(creature + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
    *reinterpret_cast<short*>(table + 176) = 8;
    *reinterpret_cast<unsigned*>(table + 180) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(moveEntry));
    at(creature, 176) = 10.0f; at(creature, 180) = 20.0f; at(creature, 184) = 30.0f;
    at(creature, 260) = -1.0f;
    *reinterpret_cast<short*>(creature + 56) = 77;
    creature[569] = 1; frameNm = 100;  // (100 ^ 1) & 7 != 0
}

int main() {
    // no target: nothing at all
    reset();
    persue2__8CreatureP10GameObjectfff(creature, nullptr, 0, 0, 0);
    CHECK(trace.empty());

    // close target (b + 5 >= distance): the clear line is followed, no final move2
    reset();
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, 0.f, 0.f);  // b + 5 == 5, distance 5: not less
    CHECK(trace == "hit((0,0,13),(3,4,0),1.1,3);move(8,0,(3,4,0));");
    CHECK(at(creature, 180) == 20.0f);

    // far target: clear line (hit < 0), then move2 with speed 10
    reset();
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace == "hit((0,0,13),(3,4,0),1.1,3);move(8,0,(3,4,0));cos77;sin77;move2(C,(20,30,0));");
    // hit just above 1 is also clear; exactly 1 and 0 are blocked
    reset(); hitValue = 1.5f;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.find("move(8,0,(3,4,0))") != std::string::npos && routeCalls == 0);
    reset(); hitValue = 1.0f;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.rfind("hit(", 0) == 0 && trace.find("route(C,R,(3,4,0),-1,0,0);move(8,0,(3,4,0));") != std::string::npos);
    CHECK(routeCalls == 1 && *reinterpret_cast<int*>(creature + 712) == 130);

    // steering frame: ((frameNm ^ byte569) & 7) == 0 calls the steering helper with min(dist, span+10)
    reset(); frameNm = 5; creature[569] = 5;
    steerResult = {1.0f, 2.0f, 3.0f};
    at(creature, 16) = 100.f; at(creature, 20) = 200.f; at(creature, 24) = 300.f;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, 0.f, 0.f);
    CHECK(trace.rfind("wall(C,14,8,T);", 0) == 0);  // distance is huge, so reach = span + 10 = 14; second = 2 * span
    CHECK(at(creature, 760) == 1.0f && at(creature, 764) == 2.0f && at(creature, 768) == 3.0f);
    CHECK(trace.find("move(8,0,(101,202,303));") != std::string::npos && trace.find("hit(") == std::string::npos);
    // a steering vector with a zero x or y is ignored; a steering call closer than span+10 uses the distance
    reset(); frameNm = 5; creature[569] = 5; steerResult = {0.0f, 2.0f, 3.0f};
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.rfind("wall(C,5,8,T);", 0) == 0 && trace.find("hit(") != std::string::npos);

    // blocked, route point valid and far: no refresh, move toward the route point
    reset(); hitValue = 0.5f;
    at(creature, 260) = 30.0f; at(creature, 264) = 40.0f; at(creature, 268) = 7.0f;
    *reinterpret_cast<int*>(creature + 712) = 100;  // not older than frameNm
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.find("route(") == std::string::npos && trace.find("move(8,0,(30,40,7));") != std::string::npos);
    // blocked, route expired: refreshed once, then used
    reset(); hitValue = 0.5f;
    at(creature, 260) = 30.0f; at(creature, 264) = 40.0f;
    *reinterpret_cast<int*>(creature + 712) = 99;
    routeResult = {50.0f, 60.0f, 70.0f};
    persue2__8CreatureP10GameObjectfff(creature, target, 1.f, -1.f, 2.f);
    CHECK(trace.find("route(C,R,(3,4,0),-1,2,0);move(8,0,(50,60,70));") != std::string::npos && routeCalls == 1);
    // a route point nearer than span forces refreshes up to 16 rounds, then the point is used
    reset(); hitValue = 0.5f;
    at(creature, 260) = 1.0f; at(creature, 264) = 1.0f;
    *reinterpret_cast<int*>(creature + 712) = 1000;
    routeResult = {1.0f, 1.0f, 0.0f};
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, 100.f, 0.f);
    CHECK(routeCalls == 16 && trace.find("move(8,0,(1,1,0));") != std::string::npos);

    // large move results halve speed and the two floats; the level byte scales the speed
    reset(); moveResult = 16001;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.find("move2(C,(10,15,0));") != std::string::npos && at(creature, 180) == 10.0f && at(creature, 184) == 15.0f && at(creature, 176) == 10.0f);
    reset(); moveResult = -16001;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(at(creature, 180) == 10.0f);
    reset(); moveResult = 16000;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(at(creature, 180) == 20.0f);
    reset(); creature[800] = 30;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    const float scaled = 10.0f * (30.0f * __builtin_bit_cast(float, 0x3c888888u));
    CHECK(trace.find("move2(C,(" + num(2.0f * scaled) + "," + num(3.0f * scaled) + ",0));") != std::string::npos);
    reset(); creature[800] = 61;
    persue2__8CreatureP10GameObjectfff(creature, target, 0.f, -1.f, 0.f);
    CHECK(trace.find("move2(C,(20,30,0));") != std::string::npos);
    std::puts("ok");
    return 0;
}
