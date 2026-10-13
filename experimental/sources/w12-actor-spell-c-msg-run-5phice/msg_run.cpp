// Observed msg_run__5PhIce at 0x001df588, 632 bytes. O is the object. Steps in order:
//  1. V = (f444 * f400, f448 * f400, f452 * f400) with fN the float at O+N (stored in a local point).
//  2. The word at +584 (a counter, c) is incremented by one. When c was below 90 (signed), eRand__10GameObject(O)
//     is called and, when its low three bits are zero, P_AddParticle__FP11ParticleDefP6Point3T1f is called with the
//     particle record at 0x007d6740 (flyingIcePuffParticle in the symbol table, file-local), the position O+16, V
//     and the float 0.0.
//  3. When the word at +560 is non-zero and the signed word at +588 (b) is below 3, thirty (30) bursts run, each:
//       - a direction D is drawn inside the unit ball by rejection: each component is
//         float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//         1.0 < x*x + y*y + z*z (summed in that order);
//       - m = float(eRand__10GameObject(O)) * 2^-16 * 20.0;
//       - the burst velocity W = (D.x * m, D.y * m, |dptofp(dpmul(fptodp(D.z * m), 0.25))|) (the third
//         component goes through the original's soft-double routines and is made non-negative);
//       - then W.x += V.x and W.y += V.y (V.z is not added);
//       - P_AddParticle(record 0x007d6790 (iceHitParticle, file-local), O+16, W, 0.0).
//     After the thirty bursts, b is incremented.
//  4. msg_run__6PhProp(O) is called (the parent run step, inferred from the name).
// The two particle records are file-local objects of the original unit (80 bytes each, defined by the
// w8-fx-particles-init-fx initializer); here they are imported under address names, like the other
// msg_run particles in this repository. Unknown: what the fields at +400, +444..452, +560, +584 and +588 mean.
// Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned char phice_msg_run_particle_7d6740[];
extern unsigned char phice_msg_run_particle_7d6790[];
int eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
void msg_run__6PhProp(void* self);
unsigned long long fptodp(float value);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);

void msg_run__5PhIce(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 v;
    const float factor = real(o, 400);
    v.x = real(o, 444) * factor;
    v.y = real(o, 448) * factor;
    v.z = real(o, 452) * factor;
    const int count = sword(o, 584);
    sword(o, 584) = count + 1;
    if (count < 90) {
        if ((eRand__10GameObject(o) & 7) == 0)
            P_AddParticle__FP11ParticleDefP6Point3T1f(phice_msg_run_particle_7d6740, reinterpret_cast<Point3*>(o + 16), &v, 0.0f);
    }
    if (word(o, 560) != 0u && sword(o, 588) < 3) {
        const float scale = bits(0x37800000u);
        for (int i = 29; i >= 0; --i) {
            Point3 d;
            do {
                float a = static_cast<float>(eRand__10GameObject(o)) * scale;
                d.x = a + a - 1.0f;
                a = static_cast<float>(eRand__10GameObject(o)) * scale;
                d.y = a + a - 1.0f;
                a = static_cast<float>(eRand__10GameObject(o)) * scale;
                d.z = a + a - 1.0f;
            } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
            const float m = static_cast<float>(eRand__10GameObject(o)) * scale * 20.0f;
            Point3 w;
            w.x = d.x * m;
            w.y = d.y * m;
            w.z = dptofp(dpmul(fptodp(d.z * m), 0x3fd0000000000000ull));
            if (w.z < 0.0f) w.z = -w.z;
            w.x = w.x + v.x;
            w.y = w.y + v.y;
            P_AddParticle__FP11ParticleDefP6Point3T1f(phice_msg_run_particle_7d6790, reinterpret_cast<Point3*>(o + 16), &w, 0.0f);
        }
        sword(o, 588) = sword(o, 588) + 1;
    }
    msg_run__6PhProp(o);
}
}
