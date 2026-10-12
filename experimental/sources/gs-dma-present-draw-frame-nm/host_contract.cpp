#include "draw_frame_nm.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char bytes[4];
    std::memcpy(bytes, &drawFrameNm, 4);
    if (bytes[0] != 0 || bytes[1] != 4 || bytes[2] != 0 || bytes[3] != 0) return 1;
    bytes[0] = 1;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == drawFrameNm) return 2;
    std::puts("draw frame nm contract passed");
    return 0;
}
