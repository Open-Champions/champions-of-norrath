// Host contract for isEnemy__6WraithP10GameObject. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA -ffp-contract=off host_check.cpp
// Record words are arena addresses; the list array holds host pointers.
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <string>
#include "../w10-world-b-common/host_arena.hpp"
#include "../w10-world-b-common/host_check.hpp"
unsigned char* _6Wraith$s_rWraithList[60];
int _6Wraith$s_wraithCount;
static std::string events; static int base_answer = 77;
extern "C" void listDestroy__FP8ListHead(unsigned char* head) { events += "D" + std::to_string(head - arena_at(0)) + ";"; }
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char*, unsigned char*) { events += "B;"; return base_answer; }
#include "wraith_is_enemy.cpp"
static unsigned char* self = arena_at(0x1000);
static unsigned char* other = arena_at(0x1400);
static unsigned char* w[4] = {arena_at(0x2000), arena_at(0x2800), arena_at(0x3000), arena_at(0x3800)};
static unsigned char* slot[4] = {arena_at(0x4000), arena_at(0x4100), arena_at(0x4200), arena_at(0x4300)};
static unsigned char* claim[4] = {arena_at(0x5000), arena_at(0x5100), arena_at(0x5200), arena_at(0x5300)};
static void pos(unsigned char* o, float x, float y) { sf_(o + 16, x); sf_(o + 20, y); sf_(o + 24, 555.f); }
static void reset() {
    std::memset(arena_at(0), 0, 0x6000);
    events.clear();
    pos(self, 10, 0); pos(other, 0, 0);
    for (int i = 0; i != 4; ++i) {
        sw_(slot[i] + 12, norrath_host_address(w[i]));
        sw_(w[i] + 576, norrath_host_address(claim[i]));
        sw_(claim[i] + 12, norrath_host_address(other));
        pos(w[i], 100, 0);
        _6Wraith$s_rWraithList[i] = slot[i];
    }
    _6Wraith$s_wraithCount = 4;
    base_answer = 77;
}
int main() {
    // No other object or no wraiths: only the base answer.
    reset(); if (isEnemy__6WraithP10GameObject(self, nullptr) != 77 || events != "B;") return 1;
    reset(); _6Wraith$s_wraithCount = 0; if (isEnemy__6WraithP10GameObject(self, other) != 77 || events != "B;") return 2;
    reset(); _6Wraith$s_wraithCount = -3; if (isEnemy__6WraithP10GameObject(self, other) != 77 || events != "B;") return 3;
    // Self is far from the others? w are at 100, self at 10: near*3 = 30 < 100 for every w,
    // so every rival list is destroyed, in order, and its word cleared; then the base answer.
    reset();
    if (isEnemy__6WraithP10GameObject(self, other) != 77) return 4;
    std::string want;
    for (int i = 0; i != 4; ++i) want += "D" + std::to_string(w[i] + 576 - arena_at(0)) + ";";
    if (events != want + "B;") { std::printf("%s\n", events.c_str()); return 5; }
    for (int i = 0; i != 4; ++i) if (hw(w[i] + 576) != 0) return 6;
    // Only the first count slots; null slots skipped.
    reset(); _6Wraith$s_wraithCount = 2; _6Wraith$s_rWraithList[0] = nullptr;
    isEnemy__6WraithP10GameObject(self, other);
    if (events != "D" + std::to_string(w[1] + 576 - arena_at(0)) + ";B;") return 7;
    // A slot whose object has no claim, or whose claim names a different target, is left alone.
    reset(); sw_(w[0] + 576, 0); sw_(claim[1] + 12, norrath_host_address(self));
    isEnemy__6WraithP10GameObject(self, other);
    if (events != "D" + std::to_string(w[2] + 576 - arena_at(0)) + ";D" + std::to_string(w[3] + 576 - arena_at(0)) + ";B;") return 8;
    // Not much closer (near*3 >= far) and not found yet: the call returns 0, nothing destroyed after.
    reset(); pos(w[0], 20, 0);                      // near 10, far 20: 30 < 20 false; near < far true; found 0
    if (isEnemy__6WraithP10GameObject(self, other) != 0 || !events.empty()) return 9;
    // Found earlier in the walk (self is slot 0): the nearer-than test now destroys.
    reset(); sw_(slot[0] + 12, norrath_host_address(self)); pos(w[1], 20, 0);
    if (isEnemy__6WraithP10GameObject(self, other) != 77) return 10;
    if (events != "D" + std::to_string(w[1] + 576 - arena_at(0)) + ";D" + std::to_string(w[2] + 576 - arena_at(0)) + ";D" + std::to_string(w[3] + 576 - arena_at(0)) + ";B;") return 11;
    // Farther than the rival: 0 whether found or not.
    reset(); sw_(slot[0] + 12, norrath_host_address(self)); pos(w[1], 5, 0);
    if (isEnemy__6WraithP10GameObject(self, other) != 0 || !events.empty()) return 12;
    reset(); pos(w[0], 10, 0);                      // equal distance: not nearer
    if (isEnemy__6WraithP10GameObject(self, other) != 0) return 13;
    // Only x and y count: a large z difference changes nothing.
    reset(); sf_(self + 24, 1e6f); sf_(w[0] + 24, -1e6f);
    if (isEnemy__6WraithP10GameObject(self, other) != 77) return 14;
    // Exactly three times as near is not enough (30 < 30 false) without the found flag.
    reset(); pos(self, 10, 0); pos(w[0], 30, 0);
    if (isEnemy__6WraithP10GameObject(self, other) != 0) return 15;
    // The y axis counts too.
    reset(); pos(self, 0, 10); pos(w[0], 0, 100);
    if (isEnemy__6WraithP10GameObject(self, other) != 77) return 16;
    std::puts("Wraith isEnemy check passed");
    return 0;
}
