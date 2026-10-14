#include "ambient_state.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof(ambientGroup) == 10880, "sixteen groups");
    if (nmAmbientGroups != 0) return 1;
    for (unsigned char b : ambientGroup) if (b) return 2;
    std::puts("ambient state passed");
    return 0;
}
