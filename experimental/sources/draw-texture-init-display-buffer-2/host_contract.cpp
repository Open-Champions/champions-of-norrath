#include <cstdio>
extern "C" unsigned displayBuffer2StartAddress;
int main() {
    if (displayBuffer2StartAddress != 0u) return 1;
    std::puts("displayBuffer2 contract passed");
    return 0;
}
