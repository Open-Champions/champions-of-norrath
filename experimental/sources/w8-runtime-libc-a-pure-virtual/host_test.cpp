// Host comparison only. Not an EE build input.
#include <cstdio>

static int terminate_calls = 0;
extern "C" void __terminate() { ++terminate_calls; }
#include "pure_virtual.cpp"

int main() {
    __pure_virtual();
    __pure_virtual();
    if (terminate_calls != 2) { std::printf("expected two __terminate calls, got %d\n", terminate_calls); return 1; }
    std::printf("__pure_virtual cases passed\n");
    return 0;
}
