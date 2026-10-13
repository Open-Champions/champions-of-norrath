// Host comparison only. Not an EE build input. abort is replaced by a counter that long-jumps.
#include <csetjmp>
#include <cstdio>
#define abort norrath_host_abort
#include "default_terminate.cpp"
#undef abort
static std::jmp_buf stop;
static int calls;
extern "C" void norrath_host_abort() { ++calls; std::longjmp(stop, 1); }
int main() {
    if (setjmp(stop) == 0) __default_terminate();
    if (calls != 1) { std::printf("abort not called exactly once\n"); return 1; }
    std::printf("default_terminate cases passed\n");
    return 0;
}
