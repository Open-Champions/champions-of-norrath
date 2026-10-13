// Host contract for makeExplosion__FG6Point3f. Not an EE plan input.
// Build with -ffp-contract=off. The particle call and the double helpers are host
// stand-ins; the particle stand-in records every call and perturbs eRandState so
// that a missed re-read of the generator state shows up.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
int eRandState;
unsigned char ballParticle[80], trailParticle[80];
}
struct Call { const void* def; Point3 pos, vel; float arg; int stateBefore; };
static std::vector<Call> calls;
extern "C" {
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* def, Point3* pos, Point3* vel, float arg) {
    calls.push_back(Call{def, *pos, *vel, arg, eRandState});
    eRandState = static_cast<int>(static_cast<unsigned>(eRandState) * 5u + 11u);
}
unsigned long long fptodp(float v) { double d = v; unsigned long long b; std::memcpy(&b, &d, 8); return b; }
unsigned long long dpsub(unsigned long long a, unsigned long long b) {
    double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double r = x - y; unsigned long long o; std::memcpy(&o, &r, 8); return o;
}
float dptofp(unsigned long long a) { double x; std::memcpy(&x, &a, 8); return static_cast<float>(x); }
void makeExplosion__FG6Point3f(Point3, float);
}

// Independent reference, written as a stream of draws instead of inline loops.
struct Stream {
    unsigned s;
    float draw() {
        s = s * 0x19660du + 0x3c6ef35fu;
        float u = static_cast<float>(static_cast<int>(s >> 16)) * 1.52587890625e-05f;
        return (u + u) - 1.0f;
    }
    Point3 ball() {
        for (;;) {
            const float x = draw(), y = draw(), z = draw();
            if (!(1.0f < (x * x + y * y) + z * z)) return Point3{x, y, z};
        }
    }
};
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
static bool samep(const Point3& a, const Point3& b) { return same(a.x, b.x) && same(a.y, b.y) && same(a.z, b.z); }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float K1 = __builtin_bit_cast(float, 0x411cccccu), K2 = __builtin_bit_cast(float, 0x4003d70au),
                K3 = __builtin_bit_cast(float, 0x3e428f5cu), K4 = __builtin_bit_cast(float, 0x3ff1eb85u),
                K5 = __builtin_bit_cast(float, 0x3fc51eb8u), K6 = __builtin_bit_cast(float, 0x3cccccccu);
    const float scales[] = {0.0f, 0.03f, 0.06f, 0.5f, 1.0f, 1.7f, 7.4f, -0.2f};
    for (float scale : scales) {
        eRandState = 0x13579bdf;
        calls.clear();
        const Point3 center = {100.0f, -250.5f, 30.25f};
        makeExplosion__FG6Point3f(center, scale);

        const float product = scale * 0.5f * 32.0f;
        const int count = (0.0f <= product) ? static_cast<int>(product + 0.5f) : static_cast<int>(product - 0.5f);
        expect(calls.size() == static_cast<size_t>(44 + (count > 0 ? 40 * count : 0)), "call count is 44 + 40 * count");
        // replay with the same perturbation of the generator between calls
        unsigned shared = 0x13579bdfu;
        size_t at = 0;
        bool ok = calls.size() >= 44;
        for (int i = 0; i < 44 && ok; ++i) {
            Stream st{shared};
            Point3 d = st.ball();
            st.s = st.s * 0x19660du + 0x3c6ef35fu;
            float speed = static_cast<float>(static_cast<int>(st.s >> 16)) * 1.52587890625e-05f * K3;
            d = Point3{d.x * (scale * K1), d.y * (scale * K1), d.z * (scale * K1)};
            const float inv = 1.0f / std::sqrt((d.x * d.x + d.y * d.y) + d.z * d.z);
            const Point3 vel = {d.x * inv * (scale * K2), d.y * inv * (scale * K2), d.z * inv * (scale * K2)};
            const Point3 pos = {d.x + center.x, d.y + center.y, d.z + center.z};
            const Call& c = calls[at++];
            // the host reciprocal square root is a division and a square root: allow a few ulps
            auto close = [](float a, float b) { return (std::isnan(a) && std::isnan(b)) || std::fabs(a - b) <= 4e-6f * (std::fabs(b) + 1e-3f); };
            ok = c.def == ballParticle && same(c.pos.x, pos.x) && same(c.pos.y, pos.y) && same(c.pos.z, pos.z) &&
                 close(c.vel.x, vel.x) && close(c.vel.y, vel.y) && close(c.vel.z, vel.z) && same(c.arg, speed) &&
                 static_cast<unsigned>(c.stateBefore) == st.s;
            shared = st.s * 5u + 11u;
        }
        expect(ok, "phase 1: 44 ball particles (position, velocity, speed, state before the call)");
        if (count > 0) {
            Stream st{shared};
            std::vector<Point3> pos(count, center), vel(count);
            for (int i = 0; i < count; ++i) {
                const Point3 p = st.ball();
                vel[i] = Point3{p.x * (scale * K4), p.y * (scale * K4), p.z * (scale * K4)};
            }
            shared = st.s;
            bool trailOk = calls.size() == static_cast<size_t>(44 + 40 * count);
            for (int step = 0; step < 40 && trailOk; ++step) {
                for (int i = 0; i < count && trailOk; ++i) {
                    pos[i] = Point3{pos[i].x + vel[i].x, pos[i].y + vel[i].y, pos[i].z + vel[i].z};
                    vel[i].z = static_cast<float>(static_cast<double>(vel[i].z) - __builtin_bit_cast(double, 0x3f947ae147ae147aull));
                    Stream t{shared};
                    Point3 q = t.ball();
                    q = Point3{q.x * (scale * K5), q.y * (scale * K5), q.z * (scale * K5)};
                    const Call& c = calls[at++];
                    trailOk = c.def == trailParticle && samep(c.pos, pos[i]) && samep(c.vel, q) &&
                              same(c.arg, static_cast<float>(step) * K6) && static_cast<unsigned>(c.stateBefore) == t.s;
                    shared = t.s * 5u + 11u;
                }
            }
            expect(trailOk, "phases 2 and 3: entries, gravity step, trail particles");
        }
        expect(eRandState == static_cast<int>(shared), "generator state after the last call");
    }
    if (failures) return 1;
    std::printf("makeExplosion contract passed\n");
    return 0;
}
