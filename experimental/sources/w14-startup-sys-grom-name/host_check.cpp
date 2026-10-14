// Host check for gRomName. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// The object is 15 zero bytes at load (observed in the pinned image).
#include <cstdio>
#include "grom_name.cpp"
int main() {
    int bad = 0;
    if (sizeof gRomName != 15) { std::puts("size"); ++bad; }
    for (char c : gRomName) if (c != 0) { std::puts("not zero"); ++bad; break; }
    return bad;
}
