// Host comparison only. Not an EE build input.
#include <cstdio>

static int calls = 0;
static void* last = nullptr;
extern "C" void __wrap_free(void* allocation) { ++calls; last = allocation; }
#include "builtin_delete.cpp"

int main() {
    __builtin_delete(nullptr);
    if (calls != 0) { std::printf("null must not reach __wrap_free\n"); return 1; }
    int object = 0;
    __builtin_delete(&object);
    if (calls != 1 || last != &object) { std::printf("pointer must be forwarded once\n"); return 2; }
    __builtin_delete(&object);
    if (calls != 2) { std::printf("every non-null call forwards\n"); return 3; }
    std::printf("__builtin_delete cases passed\n");
    return 0;
}
