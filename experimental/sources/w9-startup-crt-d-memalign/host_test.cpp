// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>

static unsigned char r1[4], r2[4], r3[4], r4[4];
extern "C" unsigned char* norrath_factory_impure_pointer_word = r1;

struct Event { int kind; unsigned char* reent; unsigned a, s; };
static std::vector<Event> events;
static unsigned char block[8];

extern "C" void norrath_player_malloc_lock(unsigned char* reent) {
    events.push_back({1, reent, 0, 0});
    norrath_factory_impure_pointer_word = r2;
}
extern "C" void* _memalign_r(unsigned char* reent, unsigned a, unsigned s) {
    events.push_back({2, reent, a, s});
    norrath_factory_impure_pointer_word = r3;
    return s == 0xffffffffu ? nullptr : block;
}
extern "C" void norrath_player_malloc_unlock(unsigned char* reent) {
    events.push_back({3, reent, 0, 0});
    norrath_factory_impure_pointer_word = r4;
}
#define memalign w9_memalign_under_test
#include "memalign.cpp"
#undef memalign

int main() {
    int failures = 0;
    if (w9_memalign_under_test(64u, 2048u) != block) { std::printf("result must be the _memalign_r allocation\n"); ++failures; }
    if (events.size() != 3 || events[0].kind != 1 || events[1].kind != 2 || events[2].kind != 3) { std::printf("call order wrong\n"); return 1; }
    if (events[0].reent != r1 || events[1].reent != r2 || events[2].reent != r3) { std::printf("impure pointer must be re-read for each call\n"); ++failures; }
    if (events[1].a != 64u || events[1].s != 2048u) { std::printf("alignment/size order wrong\n"); ++failures; }
    events.clear();
    norrath_factory_impure_pointer_word = r1;
    if (w9_memalign_under_test(16u, 0xffffffffu) != nullptr || events.size() != 3) { std::printf("failed allocation must still unlock and return null\n"); ++failures; }
    if (failures) return 2;
    std::printf("memalign cases passed\n");
    return 0;
}
