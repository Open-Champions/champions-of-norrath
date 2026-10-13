#include "../w11-actor-c-common/c.hpp"
// Observed getWaveletMat__9WaveMakerifP8Matrix33 at 0x00155e48, 192 bytes. Arguments: the wave maker s, an
// index i, a blend factor t and an output of nine floats. The word at s+1892 points to a table whose word
// at +4 is an entry count n. Entries are 36 bytes (nine floats) starting at s+76. With
// j = (i + 1) mod n (signed remainder; the original has no zero-count check, so n = 0 leaves the remainder
// equal to i + 1) the output floats 0, 1, 3 and 4 become entry i's value plus (entry j's value minus entry i's)
// times t; output floats 2, 5, 6 and 7 are set to 0 and float 8 to 1.0, which makes a 2-by-2 block
// blended between two stored matrices inside a 3-by-3 identity-extended matrix. Inferred: the entries
// are rotation/scale blocks of consecutive wavelets and the blend is a linear interpolation; names unestablished. Nothing
// here ran on the EE.
extern "C" void getWaveletMat__9WaveMakerifP8Matrix33(void* self, int index, float t, float* out) {
    using namespace w11c;
    const int count = static_cast<int>(word(link(self, 1892), 4));
#if defined(__mips__)
    int next;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(next) : "r"(index + 1), "r"(count) : "hi", "lo");
#else
    const int next = count == 0 ? index + 1 : (index + 1) % count;
#endif
    const float* a = reinterpret_cast<const float*>(bytes(self) + 76 + index * 36);
    const float* b = reinterpret_cast<const float*>(bytes(self) + 76 + next * 36);
    out[0] = a[0] + (b[0] - a[0]) * t;
    out[1] = a[1] + (b[1] - a[1]) * t;
    out[3] = a[3] + (b[3] - a[3]) * t;
    out[4] = a[4] + (b[4] - a[4]) * t;
    out[2] = 0.0f;
    out[5] = 0.0f;
    out[6] = 0.0f;
    out[7] = 0.0f;
    out[8] = 1.0f;
}
