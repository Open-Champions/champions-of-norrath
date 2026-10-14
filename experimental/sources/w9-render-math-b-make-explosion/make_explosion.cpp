// Observed makeExplosion__FG6Point3f at 0x001d98b8, extent 1504. Arguments: a
// Point3 by value (arrives as a pointer to a caller copy; copied to the stack
// first) and a float scale. Returns nothing. It adds particles with
// P_AddParticle(definition, position, velocity, float) in three phases.
//
// Count: n = (int)(scale * 0.5 * 32.0 + 0.5) when that product is >= 0 (compare
// 0.0 <= product), otherwise (int)(product - 0.5). The original stores n in a
// stack slot and re-reads it. Its stack array has room for 120 entries and is not
// guarded; this source keeps that array size and does not guard it either.
//
// Random numbers: the generator is eRandState (0x00483e58) advanced inline as
//   s = s * 0x19660d + 0x3c6ef35f, value = (float)(s >> 16) * 2^-16.
// A "draw" is 2 * value - 1.0. A point inside the unit ball is three consecutive
// draws (x, y, z); the draw is repeated, continuing from the latest state, while
// 1.0 < (x*x + y*y) + z*z. The state is re-read from eRandState before every
// ball point of phase 1 and every step of phase 3 (the callee can change it) and
// once before phase 2; it is stored back before each call and after each point
// of phase 2.
//
// Phase 1, 44 times: take a ball point d; then one more advance whose
//   speed = ((float)(s >> 16) * 2^-16) * K3.
// d *= scale*K1; n = d normalised (rsqrt.s with numerator 1.0 on (nx*nx+ny*ny)+nz*nz,
// each component multiplied by it); velocity = n * (scale*K2); position = d +
// the Point3 argument. Store the state and call P_AddParticle(ballParticle,
// &position, &velocity, speed).
// Phase 2, n times (only if n > 0): take a ball point; entry i gets position =
// the argument and velocity = point * (scale*K4).
// Phase 3, for step = 0..39 and for each entry i (if n > 0): position += velocity
// componentwise; velocity.z = (float)((double)velocity.z - G) with the double
// arithmetic done by fptodp, dpsub and dptofp; take a ball point t; t *= scale*K5;
// store the state; P_AddParticle(trailParticle, &position, &t, (float)step * K6).
//
// Constants (read from the executable's float pool): K1 = 0x411ccccc (about 9.8),
// K2 = 0x4003d70a (about 2.06), K3 = 0x3e428f5c (about 0.19), K4 = 0x3ff1eb85
// (about 1.89), K5 = 0x3fc51eb8 (about 1.54), K6 = 0x3ccccccc (about 0.025);
// G is the double 0x3f947ae147ae147a (a hair under 0.02).
// Not reproduced: a register-only countdown loop of four steps between the count
// and phase 1 (119, 89, 59, 29, -1, five nops per step), which reads and writes no
// memory and calls nothing.
// Inferred: an explosion effect, a burst of ball particles then 40 steps of
// trailing particles that fall under gravity; ballParticle (0x007d6010) and
// trailParticle (0x007d6060) are file-local objects of the original, defined in the
// authored fx static initialiser as file-local too, so they stay unresolved imports
// here. Unknown: the particle definition fields and the units of K1..K6.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
extern int eRandState;
extern unsigned char ballParticle[];
extern unsigned char trailParticle[];
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, Point3* position, Point3* velocity, float arg);
unsigned long long fptodp(float value);
unsigned long long dpsub(unsigned long long a, unsigned long long b);
float dptofp(unsigned long long value);
}

namespace {
constexpr float K1 = __builtin_bit_cast(float, 0x411cccccu);
constexpr float K2 = __builtin_bit_cast(float, 0x4003d70au);
constexpr float K3 = __builtin_bit_cast(float, 0x3e428f5cu);
constexpr float K4 = __builtin_bit_cast(float, 0x3ff1eb85u);
constexpr float K5 = __builtin_bit_cast(float, 0x3fc51eb8u);
constexpr float K6 = __builtin_bit_cast(float, 0x3cccccccu);
constexpr unsigned long long G = 0x3f947ae147ae147aull;
constexpr float inv65536 = 1.52587890625e-05f;

struct Entry {
    Point3 position;
    Point3 velocity;
};

unsigned advance(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }

float draw(unsigned& state) {
    state = advance(state);
    float value = static_cast<float>(static_cast<int>(state >> 16)) * inv65536;
    value = value + value;
    return value - 1.0f;
}

void ballPoint(unsigned& state, Point3& out) {
    float x, y, z;
    do {
        x = draw(state);
        y = draw(state);
        z = draw(state);
    } while (1.0f < (x * x + y * y) + z * z);
    out.x = x;
    out.y = y;
    out.z = z;
}

float reciprocalSqrt(float lengthSquared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(lengthSquared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(lengthSquared);
#endif
}
}  // namespace

extern "C" void makeExplosion__FG6Point3f(Point3 center, float scale) {
    Entry entries[120];
    const float product = scale * 0.5f * 32.0f;
    const int count = (0.0f <= product) ? static_cast<int>(product + 0.5f) : static_cast<int>(product - 0.5f);

    for (int i = 0; i < 44; ++i) {
        unsigned state = static_cast<unsigned>(eRandState);
        Point3 d;
        ballPoint(state, d);
        state = advance(state);
        float speed = static_cast<float>(static_cast<int>(state >> 16)) * inv65536;
        speed = speed * K3;
        const float shift = scale * K1;
        const float push = scale * K2;
        d.x = d.x * shift;
        d.y = d.y * shift;
        d.z = d.z * shift;
        Point3 n = d;
        const float r = reciprocalSqrt((n.x * n.x + n.y * n.y) + n.z * n.z);
        n.x = n.x * r;
        n.y = n.y * r;
        n.z = n.z * r;
        Point3 velocity;
        velocity.x = n.x * push;
        velocity.y = n.y * push;
        velocity.z = n.z * push;
        d.x = d.x + center.x;
        d.y = d.y + center.y;
        d.z = d.z + center.z;
        eRandState = static_cast<int>(state);
        P_AddParticle__FP11ParticleDefP6Point3T1f(ballParticle, &d, &velocity, speed);
    }

    if (count > 0) {
        unsigned state = static_cast<unsigned>(eRandState);
        for (int i = 0; i < count; ++i) {
            Point3 p;
            ballPoint(state, p);
            eRandState = static_cast<int>(state);
            const float kick = scale * K4;
            entries[i].velocity.x = p.x * kick;
            entries[i].velocity.y = p.y * kick;
            entries[i].velocity.z = p.z * kick;
            entries[i].position = center;
        }
    }

    for (int step = 0; step < 40; ++step) {
        if (count <= 0) continue;
        const float stepValue = static_cast<float>(step);
        for (int i = 0; i < count; ++i) {
            Entry& e = entries[i];
            e.position.x = e.position.x + e.velocity.x;
            e.position.y = e.position.y + e.velocity.y;
            e.position.z = e.position.z + e.velocity.z;
            e.velocity.z = dptofp(dpsub(fptodp(e.velocity.z), G));
            unsigned state = static_cast<unsigned>(eRandState);
            Point3 t;
            ballPoint(state, t);
            const float spread = scale * K5;
            t.x = t.x * spread;
            t.y = t.y * spread;
            t.z = t.z * spread;
            eRandState = static_cast<int>(state);
            P_AddParticle__FP11ParticleDefP6Point3T1f(trailParticle, &e.position, &t, stepValue * K6);
        }
    }
}
