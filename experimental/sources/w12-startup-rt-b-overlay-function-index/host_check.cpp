// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include <cstdint>
#include "overlay_function_index.cpp"
int main() {
    if (sizeof norrath_overlay_sFunctionIndex_11edf00 != 4) { std::printf("the slot's first word must be an int\n"); return 1; }
    if (reinterpret_cast<std::uintptr_t>(&norrath_overlay_sFunctionIndex_11edf00) % 8 != 0) { std::printf("must be eight-byte aligned\n"); return 2; }
    if (norrath_overlay_sFunctionIndex_11edf00 != 0) { std::printf("must start at zero\n"); return 3; }
    std::printf("norrath_overlay_sFunctionIndex_11edf00 cases passed\n");
    return 0;
}
