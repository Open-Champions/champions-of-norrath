#include "deallocate_channels.cpp"

int main() {
    unsigned long long d[2] = {0xffffffffffffffffull, 0x1234ull};
    DeallocateChannels__13VISoundDeviceUl(d, 0x8000000000000005ull);
    if (d[0] != 0x7ffffffffffffffaull) return 1;
    if (d[1] != 0x1234ull) return 2;
    DeallocateChannels__13VISoundDeviceUl(d, 0);
    if (d[0] != 0x7ffffffffffffffaull) return 3;
    return 0;
}
