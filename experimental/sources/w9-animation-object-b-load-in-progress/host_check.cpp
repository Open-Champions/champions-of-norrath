// Host check for loadInProgress storage. Not an EE plan input.
#include <cstdio>
#include "load_in_progress.cpp"
int main() {
    int failures = 0;
    if (sizeof(loadInProgress) != 1) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    if (loadInProgress != 0) { std::fprintf(stderr, "fail: initial value\n"); ++failures; }
    if (failures) return 1;
    std::printf("loadInProgress storage check passed\n");
    return 0;
}
