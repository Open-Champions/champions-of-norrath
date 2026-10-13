// Observed run__19SpellHornOmenEffectG6Point3s at 0x002c9a38, 2052 bytes. Arguments: the effect S, a position and a
// short that the function copies but never reads again. S+12 is the owner object O (read each time), S+24 an end
// frame, S+28 a particle-system id, S+20 the word that holds the effect's own slot table. Steps, in order:
//  1. When frameNm is not below the end frame (signed word at +24), the effect ends: when S is not null, slot byte 8
//     of the table at word S+20 is called on S with the argument 3 (nothing else happens).
//  2. Otherwise e = frameNm - end + 600 (a number below 600). Eight halfwords of O are set (offsets +140.. +154, all
//     step 2): with e below 140 the scale is s = float(e) * k0 (pool bits 0x3bea0ea0, 1/140 rounded) and
//     +140 = +148 = (int)(s * 225), +142 = +150 = (int)(s * 20), +146 = +154 = (int)(s * 100); +144 and +152 are zero;
//     otherwise +140 = +148 = 225, +142 = +150 = 20, +146 = +154 = 100 and the same two zeros.
//  3. matMakeTransYaw__FP8Matrix34G6Point3s(matrix, O's position copied from +16, the signed halfword at O+56) builds a
//     matrix; P_SetParticleSystemMatrix__FiP8Matrix34(word at +28, matrix) and P_OpenParticleSystem__Fi(word at +28)
//     follow.
//  4. N = modelGetNmShadowPoints__FP12_modelHeader(O's slot byte 128 result). When N is zero, step 5. Particles are
//     emitted in a do-while: with e below 140 while float(i + 1) < s * 150 (and only when 0.0 < s * 150) for i = 0, 1, ...;
//     otherwise 150 times. Each: model = O's slot byte 128 (called each time on O); r = eRand__5Spell(S);
//     modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, O + 288, the zero
//     position, 0, r % N, &point, null, null, 1.0) gives a point; a direction D1 is drawn inside the unit ball by
//     rejection (each component float(eRand) * 2^-16 doubled minus 1, x, y, z in that order, redrawn while 1.0 < x*x +
//     y*y + z*z) and the point is moved by D1 * kA (kA is the pool float 0x40400ce4, about 3.0008); a second direction
//     D2 is drawn the same way and the velocity is D2 * kB (0x41f00099, about 30.0003); then
//     P_AddParticle__FP11ParticleDefP6Point3T1f(record 0x008d8650 (hornomenParticle1, file-local), &point, &velocity, 0.0).
//  5. P_CloseParticleSystem__Fi(word at +28). (The image holds two copies of the emission code, one per stage; they differ only
//     in the loop count and the colour values.)
// Inferred: this plays a shadow-point particle spray on the owner for the remaining time. Unknown: the meaning of the
// halfwords at O+140..154 (they look like paired colour values), and the effect fields. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
extern int frameNm;
extern unsigned char hornomen_run_particle_8d8650[];
int eRand__5Spell(void* self);
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 position, short yaw);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, Matrix34* m);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* point, float* extra, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);

namespace {
using namespace w12c;

static void draw_direction(unsigned char* s, Point3& d) {
    do {
        float t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.x = t + t - 1.0f;
        t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.y = t + t - 1.0f;
        t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.z = t + t - 1.0f;
    } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
}

static void emit_one(unsigned char* s, int count, const Point3& zero) {
    unsigned char* owner = from_word(word(s, 12));
    void* const model = reinterpret_cast<void*>(call_entry(owner, 128));
    const int r = eRand__5Spell(s);
    Point3 point;
    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, owner + 288, zero, 0, r % count, &point, nullptr, nullptr, 1.0f);
    Point3 d1;
    draw_direction(s, d1);
    Point3 offset;
    offset.x = d1.x * bits(0x40400ce4u);
    offset.y = d1.y * bits(0x40400ce4u);
    offset.z = d1.z * bits(0x40400ce4u);
    point.x = point.x + offset.x;
    point.y = point.y + offset.y;
    point.z = point.z + offset.z;
    Point3 d2;
    draw_direction(s, d2);
    Point3 velocity;
    velocity.x = d2.x * bits(0x41f00099u);
    velocity.y = d2.y * bits(0x41f00099u);
    velocity.z = d2.z * bits(0x41f00099u);
    P_AddParticle__FP11ParticleDefP6Point3T1f(hornomen_run_particle_8d8650, &point, &velocity, 0.0f);
}
}  // namespace

void run__19SpellHornOmenEffectG6Point3s(void* self, Point3 position, short unused) {
    (void)position;
    (void)unused;
    unsigned char* const s = static_cast<unsigned char*>(self);
    if (!(frameNm < sword(s, 24))) {
        if (s != nullptr) call_entry_from(s, 20, 8, 3, 0);
        return;
    }
    const int elapsed = frameNm - sword(s, 24) + 600;
    const float e = static_cast<float>(elapsed);
    unsigned char* owner = from_word(word(s, 12));
    float scale = 1.0f;
    const bool growing = e < 140.0f;
    if (growing) scale = e * bits(0x3bea0ea0u);
    if (growing) {
        half(owner, 144) = 0;
        half(owner, 152) = 0;
        const float a = scale * 225.0f;
        const float b = scale * 20.0f;
        const float c = scale * 100.0f;
        const int ia = static_cast<int>(a);
        const int ib = static_cast<int>(b);
        const int ic = static_cast<int>(c);
        half(owner, 148) = static_cast<short>(ia);
        half(owner, 150) = static_cast<short>(ib);
        half(owner, 154) = static_cast<short>(ic);
        half(owner, 146) = static_cast<short>(ic);
        half(owner, 140) = static_cast<short>(ia);
        half(owner, 142) = static_cast<short>(ib);
    } else {
        half(owner, 154) = 100;
        half(owner, 146) = 100;
        half(owner, 148) = 225;
        half(owner, 150) = 20;
        half(owner, 140) = 225;
        half(owner, 142) = 20;
        half(owner, 144) = 0;
        half(owner, 152) = 0;
    }
    Matrix34 m;
    matMakeTransYaw__FP8Matrix34G6Point3s(&m, *reinterpret_cast<Point3*>(owner + 16), half(owner, 56));
    P_SetParticleSystemMatrix__FiP8Matrix34(sword(s, 28), &m);
    P_OpenParticleSystem__Fi(sword(s, 28));
    owner = from_word(word(s, 12));
    const int count = modelGetNmShadowPoints__FP12_modelHeader(reinterpret_cast<void*>(call_entry(owner, 128)));
    if (count != 0) {
        Point3 zero;
        zero.x = 0.0f;
        zero.y = 0.0f;
        zero.z = 0.0f;
        if (growing) {
            const float total = scale * bits(0x43160000u);
            if (0.0f < total) {
                int i = 0;
                do {
                    emit_one(s, count, zero);
                    ++i;
                } while (static_cast<float>(i) < total);
            }
        } else {
            int i = 0;
            do {
                emit_one(s, count, zero);
                ++i;
            } while (i < 150);
        }
    }
    P_CloseParticleSystem__Fi(sword(s, 28));
}
}
