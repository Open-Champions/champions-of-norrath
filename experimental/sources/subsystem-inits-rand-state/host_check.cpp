// Host contract for eRandState. Not an EE plan input.
#include <cstdio>

extern "C" int eRandState;

int main() {
    if (eRandState != 0) {
        std::fprintf(stderr, "fail: eRandState %d\n", eRandState);
        return 1;
    }
    eRandState = 0x12345678;
    if (eRandState != 0x12345678) {
        std::fprintf(stderr, "fail: eRandState width\n");
        return 1;
    }
    return 0;
}
