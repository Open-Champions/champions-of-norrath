// Host comparison only. Not an EE build input.
#include <cstdio>
extern "C" void __default_terminate() {}
#include "terminate_func.cpp"
static void other() {}
int main() {
    if (__terminate_func != __default_terminate) { std::puts("wrong initial handler"); return 1; }
    __terminate_func = other;
    if (__terminate_func != other) { std::puts("not writable"); return 2; }
    __terminate_func = __default_terminate;
    std::puts("terminate_func cases passed");
    return 0;
}
