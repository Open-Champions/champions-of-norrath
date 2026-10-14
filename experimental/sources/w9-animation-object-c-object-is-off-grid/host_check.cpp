// Host contract for objectIsOffGrid__FP10GameObject. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "object_is_off_grid.cpp"
unsigned char* offTheGrid;
// Iterator stand-in: visits arena nodes in list order and records the call sequence.
static unsigned order[8];
static int count, position, constructed, destroyed, nexts, last_mask, last_flags;
static unsigned char** seen_head;
static void show(unsigned* it) { sw_(reinterpret_cast<unsigned char*>(it) + 4, position < count ? order[position] : 0); }
extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned* it, unsigned char** head, int mask) {
    ++constructed; seen_head = head; last_mask = mask; position = 0; show(it);
}
extern "C" void next__16ListSafeIterator(unsigned* it) { ++nexts; ++position; show(it); }
extern "C" void _$_16ListSafeIterator(unsigned*, int flags) { ++destroyed; last_flags = flags; }
int main() {
    unsigned char* object[3] = {arena_at(0x100), arena_at(0x200), arena_at(0x300)};
    unsigned char* node[3] = {arena_at(0x400), arena_at(0x500), arena_at(0x600)};
    for (int i = 0; i != 3; ++i) { std::memset(node[i], 0, 16); sw_(node[i] + 12, norrath_host_address(object[i])); }
    unsigned char* other = arena_at(0x700);
    int failures = 0;
    auto run = [&](int n, unsigned char* wanted, int want_ret, int want_nexts, const char* what) {
        count = n; for (int i = 0; i != n; ++i) order[i] = norrath_host_address(node[i]);
        constructed = destroyed = nexts = 0;
        const int r = objectIsOffGrid__FP10GameObject(wanted);
        if (r != want_ret || nexts != want_nexts || constructed != 1 || destroyed != 1 || last_mask != 0 ||
            last_flags != 2 || seen_head != &offTheGrid) {
            std::printf("%s: ret %d nexts %d constructed %d destroyed %d\n", what, r, nexts, constructed, destroyed);
            ++failures;
        }
    };
    run(0, object[0], 0, 0, "empty list");
    run(3, object[0], 1, 0, "first node");
    run(3, object[1], 1, 1, "second node");
    run(3, object[2], 1, 2, "third node");
    run(3, other, 0, 3, "absent object");
    run(2, object[2], 0, 2, "object only past the end");
    if (failures) return 1;
    std::printf("objectIsOffGrid check passed\n");
    return 0;
}
