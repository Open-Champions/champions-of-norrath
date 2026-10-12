#include "terminate_library.cpp"

#include <cstdio>

static int calls;
extern "C" void InitTLB() { ++calls; }

int main() {
    TerminateLibrary();
    if (calls != 1) return 1;
    TerminateLibrary();
    if (calls != 2) return 2;
    std::puts("terminate library contract passed");
    return 0;
}
