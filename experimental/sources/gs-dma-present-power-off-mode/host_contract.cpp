#include "power_off_mode.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char byte = 0xff;
    std::memcpy(&byte, &powerOffMode, 1);
    if (byte != 0) return 1;
    byte = 1;
    if (static_cast<unsigned char>(powerOffMode) == byte) return 2;
    std::puts("power off mode contract passed");
    return 0;
}
