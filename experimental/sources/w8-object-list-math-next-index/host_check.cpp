// Host check for nextIndex. Not an EE plan input.
#include <cstdio>
extern "C" int nextIndex;
int main() {
    if (nextIndex != 0) { std::fprintf(stderr, "fail: nonzero start\n"); return 1; }
    nextIndex = 3;
    if (nextIndex != 3) return 1;
    std::printf("nextIndex check passed\n");
    return 0;
}
