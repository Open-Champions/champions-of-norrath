#include <cstdio>
extern "C" unsigned char packetBuffer[560000];
int main() {
    if (sizeof packetBuffer != 560000u) return 1;
    if (packetBuffer[0] != 0 || packetBuffer[191] != 0 || packetBuffer[559999] != 0) return 2;
    std::puts("packetBuffer contract passed");
    return 0;
}
