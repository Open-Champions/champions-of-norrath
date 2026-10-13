// Host check for loadName storage. Not an EE plan input.
#include <cstdio>
#include "load_name.cpp"
int main() {
    int failures = 0;
    if (sizeof(loadName) != 32) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (char c : loadName) if (c != 0) { std::fprintf(stderr, "fail: not zero\n"); ++failures; break; }
    if (failures) return 1;
    std::printf("loadName storage check passed\n");
    return 0;
}
