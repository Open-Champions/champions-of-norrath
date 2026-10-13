// Host contract for ef1Noise__Fi. Not an EE plan input.
#include <cstdio>
extern "C" { short noiseTable[256]; }
#include "ef1_noise.cpp"
int main() {
    for (int i = 0; i != 256; ++i) noiseTable[i] = static_cast<short>(i * 257 - 16384);
    noiseTable[0] = 32767; noiseTable[255] = -32768;
    // Fraction zero returns the entry itself scaled by 2^-15.
    if (ef1Noise__Fi(0x20) != noiseTable[2] / 32768.0f) return 1;
    // Fraction 8/16 averages neighbours.
    if (ef1Noise__Fi(0x38) != (noiseTable[3] + noiseTable[4]) / 2.0f / 32768.0f) return 2;
    // Index 255 wraps its upper neighbour to entry 0.
    const float want = (noiseTable[255] * 0.75f + noiseTable[0] * 0.25f) / 32768.0f;
    if (ef1Noise__Fi(0xff0 + 4) != want) { std::printf("wrap %g %g\n", ef1Noise__Fi(0xff4), want); return 3; }
    // Index above 255 masks to the low byte; negative arguments shift arithmetically.
    if (ef1Noise__Fi(0x1230) != noiseTable[0x23] / 32768.0f) return 4;
    if (ef1Noise__Fi(-16) != noiseTable[255] / 32768.0f) return 5;
    if (ef1Noise__Fi(-1) != (noiseTable[255] * 0.0625f + noiseTable[0] * 0.9375f) / 32768.0f) return 6;
    std::puts("ef1Noise contract passed");
    return 0;
}
