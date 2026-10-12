// Host comparison only. Not an EE build input.
#include <cstdio>
#include <initializer_list>
#include <vector>

static std::vector<int> events;
extern "C" int raise(int signal_number) { events.push_back(signal_number); return 0; }
extern "C" void _exit(int status) { events.push_back(1000 + status); }
#define abort w8_abort_under_test
#include "abort.cpp"
#undef abort

int main() {
    w8_abort_under_test();
    if (events != std::vector<int>{6, 1001}) { std::printf("abort order wrong\n"); return 1; }
    // A returning raise (handler returned) must still reach _exit exactly once.
    events.clear();
    w8_abort_under_test();
    if (events != std::vector<int>{6, 1001}) { std::printf("abort repeat wrong\n"); return 2; }
    std::printf("abort cases passed\n");
    return 0;
}
