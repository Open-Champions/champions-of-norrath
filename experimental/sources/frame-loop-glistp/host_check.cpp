#include <cstdio>
#include <cstring>

#include "glistp.cpp"

int main() {
    unsigned char bytes[4];
    std::memcpy(bytes, &glistp, 4);
    if (bytes[0] != 0 || bytes[1] != 0 || bytes[2] != 0 || bytes[3] != 0) {
        std::fprintf(stderr, "fail: glistp bytes\n");
        return 1;
    }
    bytes[0] = 1;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == glistp) return 1;
    std::printf("glistp checks passed\n");
    return 0;
}
