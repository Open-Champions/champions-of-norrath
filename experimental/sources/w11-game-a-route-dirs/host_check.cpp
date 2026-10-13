// Host contract for route_dirs storage; not an EE input.
#include "route_dirs.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof route_dirs == 0x60, "extent to the next symbol");
    for (auto& d : route_dirs) for (float f : d) if (f != 0.0f) { std::puts("fail: initial values"); return 1; }
    std::puts("route_dirs ok");
    return 0;
}
