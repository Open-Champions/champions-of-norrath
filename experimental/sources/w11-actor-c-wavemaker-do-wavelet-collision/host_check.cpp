// Host contract for doWaveletCollision__9WaveMakerP6Point3i; not an EE input. The particle call is recorded and
// compared with a model that follows the generator in 64-bit arithmetic.
#include "do_wavelet_collision.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
unsigned splashInfoBits[8];
unsigned char waveSplashBig[80], waveSplashSmall[80];
struct Call { const void* def; Point3 pos, vel; float arg; };
static std::vector<Call> calls;
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { calls.push_back({d, *p, *v, a}); }
int main() {
    int failures = 0;
    unsigned char obj[128];
    splashInfoBits[0] = 0x00000005u; splashInfoBits[1] = 0x80000000u; splashInfoBits[3] = 0xffffffffu;
    const struct { int index; bool big; } cases[] = {{0, true}, {1, false}, {2, true}, {3, false}, {63, true}, {62, false}, {96, true}, {127, true}, {5, false}, {120, true}};
    const unsigned states[] = {0u, 1u, 0xdeadbeefu, 0xffffffffu};
    for (auto& c : cases) for (unsigned st : states) {
        std::memset(obj, 0, sizeof obj); std::memcpy(obj + 68, &st, 4);
        calls.clear();
        const Point3 p = {10.5f, -3.25f, 7.0f};
        doWaveletCollision__9WaveMakerP6Point3i(obj, &p, c.index);
        const size_t want = c.big ? 56 : 28;
        if (calls.size() != want) { std::fprintf(stderr, "index %d: %zu particles\n", c.index, calls.size()); ++failures; continue; }
        unsigned long long s = st;
        auto draw = [&]() { s = (s * 1664525ull + 1013904223ull) & 0xffffffffull; return static_cast<double>(s >> 16) / 65536.0; };
        for (size_t i = 0; i < want; ++i) {
            const double u1 = draw(), u2 = draw(), u3 = draw(), u4 = draw(), u5 = draw();
            const Point3 wp = {static_cast<float>(p.x + (u1 - 0.5) * 25.0), static_cast<float>(p.y + (u2 - 0.5) * 25.0), static_cast<float>(p.z + (c.big ? 9.0 : 2.0))};
            const Point3 wv = {static_cast<float>((u3 - 0.5) * 15.0), static_cast<float>((u4 - 0.5) * 15.0), static_cast<float>(u5 * (c.big ? 38.0 : 15.0))};
            auto near = [](float a, float b) { float d = a - b; return d < 1e-3f && d > -1e-3f; };
            if (calls[i].def != (c.big ? static_cast<const void*>(waveSplashBig) : static_cast<const void*>(waveSplashSmall)) || calls[i].arg != 0.0f ||
                !near(calls[i].pos.x, wp.x) || !near(calls[i].pos.y, wp.y) || !near(calls[i].pos.z, wp.z) ||
                !near(calls[i].vel.x, wv.x) || !near(calls[i].vel.y, wv.y) || !near(calls[i].vel.z, wv.z)) {
                std::fprintf(stderr, "index %d state %u particle %zu differs\n", c.index, st, i); ++failures; break;
            }
        }
        unsigned end; std::memcpy(&end, obj + 68, 4);
        if (end != static_cast<unsigned>(s)) { std::fprintf(stderr, "index %d: state not stored back\n", c.index); ++failures; }
    }
    if (!failures) std::puts("doWaveletCollision ok");
    return failures != 0;
}
