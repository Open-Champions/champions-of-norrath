// Host contract for the w10-world-c-pm-list-pointers storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "pm_list_pointers.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    bool zero = true;
    for (char* p : pmlistptrs) zero &= p == nullptr;
    expect(zero && pmlistptrsused == 0, "list and count start empty");
    if (failures) return 1;
    std::printf("w10-world-c-pm-list-pointers contract passed\n");
    return 0;
}
