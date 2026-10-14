#include <cstdio>
#include <cstring>

#include "frame_nm.cpp"

int main() {
    unsigned char bytes[4];
    std::memcpy(bytes, &frameNm, 4);
    if (bytes[0] != 0 || bytes[1] != 0 || bytes[2] != 0 || bytes[3] != 0) {
        std::fprintf(stderr, "fail: frameNm bytes\n");
        return 1;
    }
    bytes[0] = 1;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == frameNm) {
        std::fprintf(stderr, "fail: frameNm check cannot see a low byte\n");
        return 1;
    }
    std::printf("frameNm checks passed\n");
    return 0;
}
