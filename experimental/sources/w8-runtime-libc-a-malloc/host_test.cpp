// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>

static unsigned char first_reent[4], second_reent[4], third_reent[4], fourth_reent[4];
extern "C" unsigned char* norrath_factory_impure_pointer_word = first_reent;

struct Event { int kind; unsigned char* reent; unsigned size; };
static std::vector<Event> events;
static unsigned char block[8];

extern "C" void norrath_player_malloc_lock(unsigned char* reent) {
    events.push_back({1, reent, 0});
    norrath_factory_impure_pointer_word = second_reent;   // lock service replaces the pointer
}
extern "C" unsigned char* norrath_player_malloc_reentrant(unsigned char* reent, unsigned size) {
    events.push_back({2, reent, size});
    norrath_factory_impure_pointer_word = third_reent;
    return size == 0xffffffffu ? nullptr : block;
}
extern "C" void norrath_player_malloc_unlock(unsigned char* reent) {
    events.push_back({3, reent, 0});
    norrath_factory_impure_pointer_word = fourth_reent;
}
#define malloc w8_malloc_under_test
#include "malloc.cpp"
#undef malloc

int main() {
    int failures = 0;
    void* got = w8_malloc_under_test(48u);
    if (got != block) { std::printf("result must be the reentrant allocation\n"); ++failures; }
    if (events.size() != 3 || events[0].kind != 1 || events[1].kind != 2 || events[2].kind != 3) { std::printf("call order wrong\n"); return 1; }
    if (events[0].reent != first_reent || events[1].reent != second_reent || events[2].reent != third_reent) { std::printf("impure pointer must be re-read for each call\n"); ++failures; }
    if (events[1].size != 48u) { std::printf("size wrong\n"); ++failures; }
    events.clear();
    norrath_factory_impure_pointer_word = first_reent;
    if (w8_malloc_under_test(0xffffffffu) != nullptr || events.size() != 3 || events[1].size != 0xffffffffu) { std::printf("failed allocation must still unlock and return null\n"); ++failures; }
    if (failures) return 2;
    std::printf("malloc cases passed\n");
    return 0;
}
