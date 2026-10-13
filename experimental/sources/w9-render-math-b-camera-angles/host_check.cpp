// Host contract for myPitch, myRoll and myFOV initial values. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" { extern short myPitch; extern short myRoll; extern float myFOV; }
int main() {
    unsigned bits; std::memcpy(&bits, &myFOV, 4);
    if (myPitch != 0x2710 || myRoll != 0 || bits != 0x42480000u) { std::fprintf(stderr, "fail\n"); return 1; }
    std::printf("camera angles contract passed\n");
    return 0;
}
