#include <cstdio>
#include <cstring>

extern "C" {
float farClip;
void worldSetFarClip__Ff(float distance);
}

namespace {
unsigned bits_of(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}
int fail(int code) {
    std::fprintf(stderr, "set far clip failed %d bits %08x\n", code, bits_of(farClip));
    return code;
}
}

int main() {
    const unsigned limit = 0x44e10000u;
    worldSetFarClip__Ff(1000.0f);
    if (bits_of(farClip) != bits_of(1000.0f)) return fail(1);
    worldSetFarClip__Ff(1800.0f);
    if (bits_of(farClip) != limit) return fail(2);
    worldSetFarClip__Ff(1800.125f);
    if (bits_of(farClip) != limit) return fail(3);
    worldSetFarClip__Ff(-40.0f);
    if (bits_of(farClip) != bits_of(-40.0f)) return fail(4);
    float unordered;
    unsigned nan_bits = 0x7fc00000u;
    std::memcpy(&unordered, &nan_bits, 4);
    worldSetFarClip__Ff(unordered);
    if (bits_of(farClip) != nan_bits) return fail(5);
    return 0;
}
