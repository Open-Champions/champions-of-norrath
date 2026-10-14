// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>

static unsigned char r1[4], r2[4], r3[4], r4[4];
extern "C" unsigned char* norrath_factory_impure_pointer_word = r1;

struct Event { int kind; unsigned char* reent; unsigned char* ptr; };
static std::vector<Event> events;
static unsigned char block[8];

extern "C" void norrath_player_malloc_lock(unsigned char* reent) {
    events.push_back({1, reent, nullptr});
    norrath_factory_impure_pointer_word = r2;
}
extern "C" void norrath_player_free_reentrant(unsigned char* reent, unsigned char* p) {
    events.push_back({2, reent, p});
    norrath_factory_impure_pointer_word = r3;
}
extern "C" void norrath_player_malloc_unlock(unsigned char* reent) {
    events.push_back({3, reent, nullptr});
    norrath_factory_impure_pointer_word = r4;
}
#define free w9_free_under_test
#include "free.cpp"
#undef free

int main() {
    int failures = 0;
    w9_free_under_test(block);
    if (events.size() != 3 || events[0].kind != 1 || events[1].kind != 2 || events[2].kind != 3) { std::printf("call order wrong\n"); return 1; }
    if (events[0].reent != r1 || events[1].reent != r2 || events[2].reent != r3) { std::printf("impure pointer must be re-read for each call\n"); ++failures; }
    if (events[1].ptr != block) { std::printf("pointer not forwarded\n"); ++failures; }
    // A null pointer still takes the lock and unlock; the null check lives in _free_r.
    events.clear();
    norrath_factory_impure_pointer_word = r1;
    w9_free_under_test(nullptr);
    if (events.size() != 3 || events[1].ptr != nullptr) { std::printf("null must reach _free_r and still unlock\n"); ++failures; }
    if (failures) return 2;
    std::printf("free cases passed\n");
    return 0;
}
