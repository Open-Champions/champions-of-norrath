#include <cstdio>
#include <cstring>

extern "C" float farClip;

int main() {
    unsigned bits = 0xffffffffu;
    std::memcpy(&bits, &farClip, 4);
    if (bits != 0u) return 1;
    farClip = 3.0f;
    std::memcpy(&bits, &farClip, 4);
    if (bits != 0x40400000u) return 2;
    return 0;
}
