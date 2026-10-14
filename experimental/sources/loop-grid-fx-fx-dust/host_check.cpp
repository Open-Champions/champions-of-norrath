// Host contract for fxDust. Not an EE plan input.
#include <cstdio>

extern "C" unsigned char fxDust[80];

int main() {
    for (int n = 0; n < 80; ++n)
        if (fxDust[n] != 0) {
            std::fprintf(stderr, "fail: fxDust starts zero\n");
            return 1;
        }
    std::printf("fx-dust contract passed\n");
    return 0;
}
