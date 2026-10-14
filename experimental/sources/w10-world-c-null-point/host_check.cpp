// Host contract for the w10-world-c-null-point storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "null_point.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(nullp[0] == 0.0f && nullp[1] == 0.0f && nullp[2] == 0.0f && sizeof(nullp) == 12, "three zero floats");
    if (failures) return 1;
    std::printf("w10-world-c-null-point contract passed\n");
    return 0;
}
