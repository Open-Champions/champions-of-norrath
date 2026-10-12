#include "block_alloc.cpp"
#include <cstdio>
static int seen_size;
static unsigned char block[4];
extern "C" unsigned char* norrath_block_alloc(int requested_size) { seen_size = requested_size; return requested_size < 0 ? nullptr : block; }
int main() {
    int failures = 0;
    if (blockAlloc__Fi(40) != block || seen_size != 40) { std::fprintf(stderr, "fail: pass-through\n"); ++failures; }
    if (blockAlloc__Fi(-2) != nullptr || seen_size != -2) { std::fprintf(stderr, "fail: null result\n"); ++failures; }
    return failures;
}
