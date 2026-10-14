#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char enableMallocWrapper;
}
extern "C" void* __wrap_malloc(unsigned size);

namespace {
int malloc_calls = 0;
int alloc_calls = 0;
unsigned seen_size = 0;
const char* seen_name = nullptr;
void* malloc_result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x1111));
void* alloc_result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x2222));
int fail(int code) {
    std::fprintf(stderr, "wrap malloc check failed: %d\n", code);
    return code;
}
}

extern "C" void* malloc(unsigned size) {
    ++malloc_calls;
    seen_size = size;
    return malloc_result;
}
extern "C" void* memAlloc__FiPc(int bytes, char* name) {
    ++alloc_calls;
    seen_size = static_cast<unsigned>(bytes);
    seen_name = name;
    return alloc_result;
}

int main() {
    enableMallocWrapper = 0;
    if (__wrap_malloc(1000) != malloc_result || malloc_calls != 1 || alloc_calls != 0 || seen_size != 1000u)
        return fail(1);
    enableMallocWrapper = 1;
    if (__wrap_malloc(64) != alloc_result || alloc_calls != 1 || malloc_calls != 1 || seen_size != 64u)
        return fail(2);
    if (seen_name == nullptr || std::strcmp(seen_name, "fake malloc") != 0) return fail(3);
    enableMallocWrapper = 0x80;
    if (__wrap_malloc(8) != alloc_result || alloc_calls != 2 || malloc_calls != 1) return fail(4);
    std::printf("wrap malloc checks passed\n");
    return 0;
}
