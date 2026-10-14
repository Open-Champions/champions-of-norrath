#include <cstdio>
#include <cstring>

extern "C" unsigned char worldCameraPos[12];

int main() {
    if (sizeof worldCameraPos != 12u) return 1;
    unsigned char zeros[12] = {};
    if (std::memcmp(worldCameraPos, zeros, 12) != 0) return 2;
    worldCameraPos[8] = 0x00;
    worldCameraPos[9] = 0x00;
    worldCameraPos[10] = 0xc8;
    worldCameraPos[11] = 0x42;
    float height = 0.0f;
    std::memcpy(&height, worldCameraPos + 8, 4);
    if (height != 100.0f) return 3;
    return 0;
}
