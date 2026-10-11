#include "last_frame_vblank.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char bytes[4];
    std::memcpy(bytes, &lastFrameVBlankCount, 4);
    if (bytes[0] != 1 || bytes[1] != 0 || bytes[2] != 0 || bytes[3] != 0) return 1;
    bytes[0] = 2;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == lastFrameVBlankCount) return 2;
    std::puts("last frame vblank count contract passed");
    return 0;
}
