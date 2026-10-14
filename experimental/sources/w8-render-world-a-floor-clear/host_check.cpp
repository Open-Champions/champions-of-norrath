#include "floor_clear.cpp"
#include <cstdio>
static int calls;
extern "C" void norrath_floor_clear() { ++calls; }
int main() {
    floorClear__Fv();
    if (calls != 1) { std::fprintf(stderr, "fail: one call expected, got %d\n", calls); return 1; }
    return 0;
}
