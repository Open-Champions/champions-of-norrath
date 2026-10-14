#include <cstdio>
#include <cstring>

#include "gstart.cpp"

int main() {
    unsigned char bytes[4];
    std::memcpy(bytes, &gstart, 4);
    if (bytes[0] != 0 || bytes[1] != 0 || bytes[2] != 0 || bytes[3] != 0) {
        std::fprintf(stderr, "fail: gstart bytes\n");
        return 1;
    }
    bytes[0] = 1;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == gstart) return 1;
    std::printf("gstart checks passed\n");
    return 0;
}
