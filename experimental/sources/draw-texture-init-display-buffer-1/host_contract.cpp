#include <cstdio>
extern "C" int displayBuffer1StartAddress;
int main() {
    if (displayBuffer1StartAddress != 0) return 1;
    std::puts("displayBuffer1 contract passed");
    return 0;
}
