#include <cstdio>
extern "C" unsigned engineDrawBufferStartAddress;
int main() {
    if (engineDrawBufferStartAddress != 0u) return 1;
    std::puts("engineDrawBuffer contract passed");
    return 0;
}
