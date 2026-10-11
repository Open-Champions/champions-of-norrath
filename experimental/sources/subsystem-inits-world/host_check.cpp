// Host contract for worldInit__Fv. Not an EE plan input.
#include <cstdio>

extern "C" void worldInit__Fv();
extern "C" float farClip = 0;

namespace {
int calls = 0;
unsigned args[4];
unsigned far_at_call = 0;

unsigned bits_of(float value) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.value = value;
    return converted.bits;
}
}

extern "C" void worldSetFrustum__Fffff(float first, float second, float third, float fourth) {
    far_at_call = bits_of(farClip);
    args[0] = bits_of(first);
    args[1] = bits_of(second);
    args[2] = bits_of(third);
    args[3] = bits_of(fourth);
    ++calls;
}

int main() {
    farClip = 0.0f;
    worldInit__Fv();
    if (calls != 1 || far_at_call != 0x44e10000u || args[0] != 0x42480000u ||
        args[1] != 0x3faa3d70u || args[2] != 0x42a00000u || args[3] != 0x3f800000u ||
        bits_of(farClip) != 0x44e10000u) {
        std::fprintf(stderr, "fail: frustum %d %08x %08x %08x %08x %08x\n",
                     calls, far_at_call, args[0], args[1], args[2], args[3]);
        return 1;
    }
    return 0;
}
