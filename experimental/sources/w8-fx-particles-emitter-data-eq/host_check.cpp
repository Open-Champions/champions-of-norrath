// Host contract for __eq__19ParticleEmitterDataRC19ParticleEmitterData. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" int __eq__19ParticleEmitterDataRC19ParticleEmitterData(const void*, const void*);

int main() {
    unsigned char a[24], b[24];
    for (int i = 0; i < 24; ++i) a[i] = static_cast<unsigned char>(i * 7 + 1);
    std::memcpy(b, a, sizeof a);
    if (__eq__19ParticleEmitterDataRC19ParticleEmitterData(a, b) != 1) { std::fprintf(stderr, "fail: equal\n"); return 1; }
    // Every compared byte matters.
    for (int i = 0; i <= 16; ++i) {
        std::memcpy(b, a, sizeof a);
        b[i] ^= 0x40;
        const int got = __eq__19ParticleEmitterDataRC19ParticleEmitterData(a, b);
        const int want = 0;
        if (got != want) { std::fprintf(stderr, "fail: byte %d gives %d\n", i, got); return 1; }
    }
    // Bytes past the record are ignored.
    std::memcpy(b, a, sizeof a);
    b[17] ^= 0xff; b[20] ^= 0xff;
    if (__eq__19ParticleEmitterDataRC19ParticleEmitterData(a, b) != 1) { std::fprintf(stderr, "fail: tail\n"); return 1; }
    // A record compares equal to itself.
    if (__eq__19ParticleEmitterDataRC19ParticleEmitterData(a, a) != 1) { std::fprintf(stderr, "fail: self\n"); return 1; }
    std::printf("emitter-data-eq contract passed\n");
    return 0;
}
