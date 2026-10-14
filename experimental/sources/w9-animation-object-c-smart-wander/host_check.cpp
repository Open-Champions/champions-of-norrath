// Host contract for smartWander__8Creature. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "smart_wander.cpp"
static std::vector<std::string> events;
static int steer_result;
static unsigned char* creature;
extern "C" int steerAndRouteToPosition__8CreatureG6Point3(unsigned char* self, unsigned char* point) {
    events.push_back("steer:" + std::to_string(hw(point)) + "," + std::to_string(hw(point + 4)) + "," + std::to_string(hw(point + 8)) +
                     (self == creature ? "" : "!bad-self"));
    sw_(point, 0xdead);   // the callee owns its copy; the creature's point must stay intact
    return steer_result;
}
extern "C" void resetRouting__8Creature(unsigned char* self) {
    events.push_back("reset" + std::string(self == creature ? "" : "!bad-self"));
    sw_(self + 716, 0);
}
extern "C" void getSmartRandomDestination__8Creaturei(unsigned char* result, unsigned char* self, int range) {
    events.push_back("dest:" + std::to_string(range) + (self == creature ? "" : "!bad-self"));
    sw_(result, 11); sw_(result + 4, 22); sw_(result + 8, 33);
}
extern "C" void norrath_host_call_pointer_int(unsigned function, unsigned char* first, int second) {
    events.push_back("virt:" + std::to_string(function) + ":" + std::to_string(first - creature) + ":" + std::to_string(second));
}
int main() {
    creature = arena_at(0x100);
    unsigned char* vtable = arena_at(0x800);
    int failures = 0;
    auto run = [&](int steer, int attempts, const std::vector<std::string>& want, int want_ret, bool goal_changes, const char* what) {
        std::memset(arena_at(0), 0x44, 0x1000);
        sw_(creature + 72, norrath_host_address(vtable));
        sh_(vtable + 184, 0xfff0u);       // signed halfword -16
        sw_(vtable + 188, 0x1234);
        sw_(creature + 272, 1); sw_(creature + 276, 2); sw_(creature + 280, 3);
        sw_(creature + 716, static_cast<unsigned>(attempts));
        events.clear(); steer_result = steer;
        unsigned char before[0x1000]; std::memcpy(before, arena_at(0), 0x1000);
        const int r = smartWander__8Creature(creature);
        unsigned char want_mem[0x1000]; std::memcpy(want_mem, before, 0x1000);
        if (goal_changes) { sw_(want_mem + 0x100 + 272, 11); sw_(want_mem + 0x100 + 276, 22); sw_(want_mem + 0x100 + 280, 33); sw_(want_mem + 0x100 + 716, 0); }
        if (r != want_ret || events != want || same(what, arena_at(0), want_mem, 0x1000)) {
            std::printf("%s: ret %d, %zu events\n", what, r, events.size());
            for (auto& e : events) std::printf("  %s\n", e.c_str());
            ++failures;
        }
    };
    run(0, 3, {"steer:1,2,3"}, 1, false, "no steer result, few attempts");
    run(5, 3, {"steer:1,2,3", "virt:4660:-16:0"}, 1, false, "steer result, few attempts");
    run(0, 4, {"steer:1,2,3", "reset", "dest:300"}, 0, true, "four attempts");
    run(-1, 100, {"steer:1,2,3", "virt:4660:-16:0", "reset", "dest:300"}, 0, true, "steer result, many attempts");
    run(0, -5, {"steer:1,2,3"}, 1, false, "negative attempts count as below 4");
    if (failures) return 1;
    std::printf("smartWander check passed\n");
    return 0;
}
