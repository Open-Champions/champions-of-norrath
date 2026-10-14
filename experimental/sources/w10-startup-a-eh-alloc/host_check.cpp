// Host comparison only. Not an EE build input. Scripted callees; the emergency pool behaviour is
// checked against hand-derived sequences (n = 2 units gives positions 10, 8, 6, 4, 2 and then
// the terminate path; n = 3 starts at 9 and so on).
#include <csetjmp>
#include <cstdio>
#include <cstring>
#define abort norrath_host_abort
#include "eh_alloc.cpp"
#undef abort
static std::jmp_buf stop;
static int aborted, terminated, malloc_calls;
static void* malloc_result;
static unsigned malloc_size;
alignas(16) static unsigned char context[400];
extern "C" void norrath_host_abort() { ++aborted; std::longjmp(stop, 1); }
extern "C" void* __wrap_malloc(unsigned size) { ++malloc_calls; malloc_size = size; return malloc_result; }
extern "C" void* __get_eh_context() { return context; }
extern "C" void __terminate() { ++terminated; std::longjmp(stop, 2); }
static unsigned& mask() { return *reinterpret_cast<unsigned*>(context + 208); }
static void* call(unsigned size, int* how) {
    aborted = terminated = 0;
    int r = setjmp(stop);
    if (r == 0) { void* p = __eh_alloc(size); *how = 0; return p; }
    *how = r;
    return nullptr;
}
int main() {
    int failures = 0, how;
    char marker;
    malloc_result = &marker; malloc_calls = 0;
    if (call(24, &how) != &marker || how != 0 || malloc_size != 24 || malloc_calls != 1) { std::printf("malloc success path\n"); ++failures; }
    call(0, &how);
    if (how != 1 || aborted != 1 || malloc_calls != 1) { std::printf("zero size must abort before malloc\n"); ++failures; }
    malloc_result = nullptr; mask() = 0;
    const int expect[] = {10, 8, 6, 4, 2};
    for (int i = 0; i < 5; ++i) {
        void* p = call(16, &how);
        if (how != 0 || p != context + 16 + expect[i] * 16) { std::printf("alloc %d wrong\n", i); ++failures; }
    }
    if (mask() != ((1u << 10) | (1u << 8) | (1u << 6) | (1u << 4) | (1u << 2))) { std::printf("mask %x\n", mask()); ++failures; }
    call(16, &how);
    if (how != 2 || terminated != 1) { std::printf("exhausted pool must terminate\n"); ++failures; }
    mask() = 0;
    void* p = call(33, &how);                       // 3 units, bits 7, start at 12 - 3 = 9
    if (how != 0 || p != context + 16 + 9 * 16 || mask() != (3u << 9)) { std::printf("3 units wrong: %x\n", mask()); ++failures; }
    mask() = 0;
    p = call(192, &how);                            // 12 units, position 0: terminate
    if (how != 2) { std::printf("12 units must terminate\n"); ++failures; }
    call(208, &how);                                // 13 units: terminate
    if (how != 2) { std::printf("13 units must terminate\n"); ++failures; }
    mask() = 0;
    p = call(176, &how);                            // 11 units: position 1
    if (how != 0 || p != context + 16 + 16 || mask() != (0x3ffu << 1)) { std::printf("11 units wrong: %x\n", mask()); ++failures; }
    if (failures) return 2;
    std::printf("eh_alloc cases passed\n");
    return 0;
}
