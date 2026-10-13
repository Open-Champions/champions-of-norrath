// Host contract for doWaveletReflection__9WaveMakerP6Point3; not an EE input. The particle call is recorded.
#include "do_wavelet_reflection.cpp"
#include <cstdio>
#include <cstring>
unsigned char waveTrail[80];
static const void* gdef; static Point3 gpos, gvel; static float garg; static int calls;
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) {
    gdef = d; gpos = *p; gvel = *v; garg = a; ++calls;
}
int main() {
    int failures = 0;
    unsigned char obj[128];
    const unsigned states[] = {0u, 1u, 12345u, 0xffffffffu, 0x80000000u, 0xdeadbeefu};
    const Point3 pts[] = {{0, 0, 0}, {100.5f, -20.25f, 7.0f}, {-1e6f, 3.5f, -9.0f}};
    for (unsigned st : states) for (const Point3& p : pts) {
        std::memset(obj, 0, sizeof obj);
        std::memcpy(obj + 68, &st, 4);
        calls = 0;
        Point3 in = p;
        doWaveletReflection__9WaveMakerP6Point3(obj, &in);
        // model: the generator as a 64-bit computation reduced to 32 bits
        unsigned long long s = st; double u[4];
        for (int i = 0; i < 4; ++i) { s = (s * 1664525ull + 1013904223ull) & 0xffffffffull; u[i] = static_cast<double>(s >> 16) / 65536.0; }
        unsigned endState; std::memcpy(&endState, obj + 68, 4);
        const Point3 wantPos = {p.x + static_cast<float>((u[2] - 0.5) * 36.0), p.y + static_cast<float>((u[3] - 0.5) * 36.0), p.z};
        const Point3 wantVel = {static_cast<float>((u[0] - 0.5) * 5.0), static_cast<float>((u[1] - 0.5) * 5.0), 0.0f};
        if (calls != 1 || gdef != waveTrail || garg != 0.0f || endState != static_cast<unsigned>(s) ||
            std::memcmp(&gpos, &wantPos, 12) != 0 || std::memcmp(&gvel, &wantVel, 12) != 0 || std::memcmp(&in, &p, 12) != 0) {
            std::fprintf(stderr, "state %u point (%g,%g,%g) differs\n", st, p.x, p.y, p.z); ++failures;
        }
    }
    if (!failures) std::puts("doWaveletReflection ok");
    return failures != 0;
}
