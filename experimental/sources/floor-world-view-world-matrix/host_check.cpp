#include <cstdio>
#include <cstring>

extern "C" unsigned char worldWorldViewMatrix[64];

int main() {
    if (sizeof worldWorldViewMatrix != 64u) return 1;
    unsigned char zeros[64] = {};
    if (std::memcmp(worldWorldViewMatrix, zeros, 64) != 0) return 2;
    worldWorldViewMatrix[0] = 0x00;
    worldWorldViewMatrix[1] = 0x00;
    worldWorldViewMatrix[2] = 0x80;
    worldWorldViewMatrix[3] = 0x3f;
    float first = 0.0f;
    std::memcpy(&first, worldWorldViewMatrix, 4);
    if (first != 1.0f) return 3;
    if (worldWorldViewMatrix[63] != 0) return 4;
    return 0;
}
