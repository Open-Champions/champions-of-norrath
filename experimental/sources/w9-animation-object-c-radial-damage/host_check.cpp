// Host contract for objectRadialDamage__FP10GameObjectG6Point3iffiii. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <cmath>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "radial_damage.cpp"
struct Hit { unsigned char* object; unsigned char* source; int pass; float amount; };
static std::vector<Hit> hits;
static std::vector<unsigned> found;
static float seen_low[3], seen_high[3];
static int box_calls, box_limit;
extern "C" int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned* out, int limit) {
    ++box_calls; box_limit = limit;
    for (int i = 0; i != 3; ++i) { seen_low[i] = low[i]; seen_high[i] = high[i]; }
    unsigned n = 0;
    for (unsigned a : found) out[n++] = a;
    out[n] = 0;
    return int(n);
}
extern "C" void norrath_host_call_pointer_pointer_int_float(unsigned function, unsigned char* first, unsigned char* second, int third, float fourth) {
    hits.push_back({first, second, third, fourth});
    if (function != 0x7777) hits.push_back({nullptr, nullptr, -1, 0});   // wrong function word
}
static unsigned char* put(int slot, float x, float y, float radius, unsigned flags, short adjust = 0) {
    unsigned char* o = arena_at(0x1000 + slot * 0x100);
    std::memset(o, 0, 0x100);
    sf_(o + 16, x); sf_(o + 20, y); sf_(o + 24, 99.0f); sf_(o + 28, radius); sw_(o + 12, flags);
    unsigned char* vtable = arena_at(0x400 + slot * 0x20);
    sw_(o + 72, norrath_host_address(vtable));
    sh_(vtable + 16, static_cast<unsigned>(adjust)); sw_(vtable + 20, 0x7777);
    return o;
}
int main() {
    int failures = 0;
    unsigned char* source = put(0, 0, 0, 0, 0);
    unsigned char* centre = arena_at(0x300);
    sf_(centre, 0); sf_(centre + 4, 0); sf_(centre + 8, 5);
    auto run = [&](std::vector<unsigned char*> objs, int damage, float inner, float outer, int minimum, int mask) {
        hits.clear(); found.clear(); box_calls = 0;
        for (auto* o : objs) found.push_back(norrath_host_address(o));
        objectRadialDamage__FP10GameObjectG6Point3iffiii(source, centre, damage, inner, outer, 0x40800, minimum, mask);
    };
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* near_ = put(1, 30, 0, 0, 0);
    unsigned char* mid = put(2, 80, 0, 0, 0);
    unsigned char* diag = put(3, 50, 50, 0, 0);
    unsigned char* far_ = put(4, 60, 60, 0, 0);
    unsigned char* big = put(5, 80, 0, 10, 0);
    unsigned char* masked = put(6, 10, 0, 0, 0x8000);
    unsigned char* shifted = put(7, 20, 0, 0, 0, -16);
    run({near_, mid, diag, far_, big, masked, source, shifted}, 60, 40.0f, 100.0f, 25, 0x8000);
    expect(box_calls == 1 && box_limit == 128, "one box query with room for 128");
    expect(seen_low[0] == -100 && seen_low[1] == -100 && seen_low[2] == -95 && seen_high[0] == 100 && seen_high[2] == 105, "box is centre +- outer");
    expect(hits.size() == 5, "five victims: near, mid, diag, big and shifted");
    if (hits.size() == 5) {
        expect(hits[0].object == near_ && hits[0].amount == 60 && hits[0].pass == 0x40800 && hits[0].source == source, "near: (100-30)*1.0 capped to 60");
        expect(hits[1].object == mid && hits[1].amount == 25, "mid: 20 raised to the minimum of 25");
        expect(hits[2].object == diag && hits[2].amount == 25, "diag raised to the minimum of 25");
        expect(hits[3].object == big && hits[3].amount == 30, "big: radius 10 shortens the distance to 70");
        expect(hits[4].object == shifted - 16 && hits[4].amount == 60, "slot adjust applied to the victim pointer");
    }
    // Span of 1 or less: scale 100, so the amount is the distance gap times 100, then capped.
    run({mid}, 500, 99.5f, 100.5f, 0, 0);
    expect(hits.size() == 1 && hits[0].amount == 500, "span 1.0 uses scale 100: 20.5*100 capped to 500");
    run({mid}, 5000, 99.5f, 100.5f, 0, 0);
    expect(hits.size() == 1 && hits[0].amount == 2050, "span 1.0 uses scale 100 uncapped: 2050");
    // Truncation of the amount toward zero before the call.
    run({mid}, 100, 0.0f, 100.0f, 0, 0);
    expect(hits.size() == 1 && hits[0].amount == 20, "scale 1: 20");
    run({near_}, 7, 0.0f, 10.0f, 0, 0);
    expect(hits.empty(), "outside the outer distance: nothing");
    run({}, 7, 0.0f, 10.0f, 0, 0);
    expect(hits.empty() && box_calls == 1, "empty box result");
    if (failures) return 1;
    std::printf("objectRadialDamage check passed\n");
    return 0;
}
