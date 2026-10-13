#include <cmath>
#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
    void* decompression, unsigned char* reader, unsigned char* state, unsigned char* output, int count, int skip);
}
int main() {
    unsigned char* reader = arena_at(0x100);
    unsigned char* units = arena_at(0x200);
    unsigned char* state = arena_at(0x300);
    unsigned char* out = arena_at(0x400);
    // Hand-built stream: 5-bit zero run of 1 (00001), one data bit 1, sign 1, run codes 001 and 001.
    // Scratch bytes become {0, 3, 1}: sample 1 lowers the level by its step, sample 2 raises it.
    units[0] = 0x48; units[1] = 0x0e; units[2] = 0; units[3] = 0;
    sw_(reader, norrath_host_address(units)); sw_(reader + 4, 0);
    sw_(state, 0); sw_(state + 4, 128); sf_(state + 8, 0.0f); sf_(state + 12, 0.0f);
    std::memset(out, 0xaa, 8);
    analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(nullptr, reader, state, out, 3, 0);
    // Independent model of the low-pass stage in double precision.
    const int levels[3] = {128, 124, 132};
    const unsigned nextIndex[3] = {0, 2, 4};
    double pos = 0, vel = 0;
    const double k = 1.0 - std::sqrt(0.2), c1 = 0.05;
    for (int s = 0; s < 3; ++s) {
        for (int p = 0; p < 10; ++p) {
            pos += vel;
            vel = (vel + (levels[s] - pos) * c1) * k;
        }
        int v = (int)pos; if (v < 0) v = 0; if (v > 255) v = 255;
        if (out[s] != v) { std::printf("sample %d got %d want %d\n", s, out[s], v); return 1 + s; }
    }
    if (out[3] != 0xaa) return 4;
    if (hw(reader + 4) != 5 + 1 + 1 + 3 + 3) return 5;
    if (hw(state) != nextIndex[2] || hw(state + 4) != 132) return 6;
    // skip: the first sample is decoded into the scratch but nothing is stored or advanced for it.
    sw_(reader + 4, 0); sw_(state, 0); sw_(state + 4, 128); sf_(state + 8, 0.0f); sf_(state + 12, 0.0f);
    std::memset(out, 0xaa, 8);
    analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(nullptr, reader, state, out, 2, 1);
    if (hw(state + 4) != 132 || hw(state) != 4 || out[2] != 0xaa) return 7;
    // count <= 0: bits are consumed, nothing stored.
    sw_(reader + 4, 0); sw_(state, 3); sw_(state + 4, 9);
    analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(nullptr, reader, state, out, 0, 3);
    if (hw(state) != 3 || hw(state + 4) != 9) return 8;
    std::puts("ok");
    return 0;
}
