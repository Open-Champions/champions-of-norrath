#include "block_alloc_init.cpp"
#include <cstdio>
static int calls;
extern "C" void norrath_block_alloc_init() { ++calls; }
int main() {
    blockAllocInit__Fv();
    if (calls != 1) { std::fprintf(stderr, "fail: one call expected, got %d\n", calls); return 1; }
    return 0;
}
