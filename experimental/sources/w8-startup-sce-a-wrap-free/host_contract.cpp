// The library free is renamed so the host's own allocator is not interposed.
#define free norrath_host_library_free
#include "wrap_free.cpp"
#undef free

#include <cstdio>

unsigned char enableMallocWrapper = 0;
static void* mem_seen;
static void* lib_seen;
extern "C" void memFree__FPv(void* block) { mem_seen = block; }
extern "C" void norrath_host_library_free(void* block) { lib_seen = block; }

int main() {
    int token_a, token_b;
    enableMallocWrapper = 0;
    __wrap_free(&token_a);
    if (lib_seen != &token_a || mem_seen != nullptr) return 1;
    lib_seen = nullptr;
    enableMallocWrapper = 1;
    __wrap_free(&token_b);
    if (mem_seen != &token_b || lib_seen != nullptr) return 2;
    enableMallocWrapper = 0x80;
    mem_seen = nullptr;
    __wrap_free(&token_a);
    if (mem_seen != &token_a) return 3;
    std::puts("wrap free contract passed");
    return 0;
}
