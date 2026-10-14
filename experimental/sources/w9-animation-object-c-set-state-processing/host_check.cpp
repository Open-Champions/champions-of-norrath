// Host contract for setStateProcessing__8CreaturePi. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "set_state_processing.cpp"
static int exit_result = 0, exit_calls = 0;
static unsigned char* exit_arg = nullptr;
extern "C" int bExitStatePersueIdle__8Creature(unsigned char* self) { ++exit_calls; exit_arg = self; return exit_result; }
int main() {
    unsigned char* c = arena_at(0x100);
    int failures = 0;
    auto run = [&](int flag, int state, int exit_ret, int want_ret, int want_state, int want_calls, const char* what) {
        std::memset(c, 0, 1024);
        c[570] = static_cast<unsigned char>(flag);
        exit_result = exit_ret; exit_calls = 0; exit_arg = nullptr;
        int s = state;
        const int r = setStateProcessing__8CreaturePi(c, &s);
        if (r != want_ret || s != want_state || exit_calls != want_calls || (want_calls && exit_arg != c)) {
            std::printf("%s: ret %d state %d calls %d\n", what, r, s, exit_calls);
            ++failures;
        }
    };
    run(1, 1, 0, 1, 1, 0, "flag set, state 1");
    run(-1, 1, 0, 1, 1, 0, "negative flag, state 1");
    run(0, 1, 0, 0, 1, 0, "flag clear, state 1");
    run(1, 3, 0, 0, 5, 1, "state 3, exit result 0");
    run(0, 3, 0, 0, 5, 1, "flag clear, state 3, exit result 0");
    run(1, 3, 1, 0, 3, 1, "state 3, exit result nonzero");
    run(0, 3, 7, 0, 3, 1, "flag clear, state 3, exit result 7");
    run(1, 2, 0, 0, 2, 0, "state 2");
    run(0, 5, 0, 0, 5, 0, "state 5");
    run(1, 0, 0, 0, 0, 0, "state 0");
    if (failures) return 1;
    std::printf("setStateProcessing check passed\n");
    return 0;
}
