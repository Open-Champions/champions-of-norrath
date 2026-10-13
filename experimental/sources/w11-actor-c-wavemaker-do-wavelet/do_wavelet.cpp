#include "../w11-actor-c-common/c.hpp"
// Observed doWavelet__9WaveMakerP6Point3P8Matrix33i at 0x001521e8, 1872 bytes. Arguments: the wave maker s, a
// point p, a 3x3 matrix m (nine floats, modified in place) and an int n. The 32-bit generator at s+68 steps as
// state * 1664525 + 1013904223 and a draw is the upper 16 bits; u(d) means the draw scaled by 2^-16. f is the global
// frameNm. Constants named g1.. are the single-precision words the original reads from the global
// pointer area: g1 0x3ebef976, g2 0x3f5041a2, g3 -1.7572854 (0xbfe0eeba), g4 34.399506 (0x42099918),
// g5 0.0005 (0x3a03126e), g6 0.333 (0x3eaa7ef9), g7 435.0.
//  1. One draw gives w = (u * g1 + g2) * -15.0; V = (0, w, 0). W = m * (0, g3, 0) (matTransform).
//     matRot(m, 0, signed halfword at s+1876).
//  2. When (f + n) & 15 == 0: objectFindInBox over the box low = (p.x-12, p.y-12, p.z-60) and high =
//     (p.x-12, p.y+12, p.z+10) (observed: the high x equals the low x) fills a null-terminated list of up to 64
//     objects. For each object o whose flags word (o+12) has bit 0x20 set and bit 0x01 clear and with
//     p.z - o.z (float at o+24) above 12.0: the float at o+756 becomes 82.0 and the halfword at o+748 becomes
//     iatan2(W.y + (u - 0.5) * 0.25, W.x + (u' - 0.5) * 0.25) from two draws.
//  3. Six passes (counter t from 0 in steps of 11186 while t <= 0xffff), each:
//       Q = (m * V) + p;
//       if Q.z < the float at s+1888 (water level): four draws; P_AddParticle(waveTrail,
//         (Q.x - W.x * 34 + (u3 - 0.5) * 36, Q.y - W.y * 34 + (u4 - 0.5) * 36, water level),
//         ((u1 - 0.5) * 5, (u2 - 0.5) * 5, 0), 0.0);
//       else, when one draw has its low bit set: Z = (0, 0, -(signed halfword at s+1878) * 15.0 * g5);
//         R = (m * Z) + W + ((u - 0.5) * g4, (u' - 0.5) * g4, (u'' - 0.5) * g4 * g6) from three draws and z raised to
//         at least -2.0 (max.s); a fourth draw's low byte decides P_AddParticle(waveMainParticle, Q, R, 0) (non-zero)
//         or waveMainAlternate (zero); then, when a further draw has its low nine bits zero, one more draw gives
//         P_AddParticle(waveRandomFoam, Q, (W.x, W.z, u * 10 + 2), 0);
//       matRot(m, 0, 11186) and t += 11186.
//  4. When (f + n) & 31 == 0: worldPerturbWater(world, p, g7, 24.0).
// The original also stores p - W to a stack temporary that is never read; that is omitted. Inferred: this
// builds the foam/droplet reaction of one travelling wavelet (an arc of six steps around the matrix axis) and
// turns nearby actors toward it; the water level is the float at +1888 as in msg_run__9WaveMaker. Unknown: what
// the objects' fields at +748 and +756 are, what n is (it is also the splash index of doWaveletCollision), and the
// meaning of the constants. The particle definitions and world are data owned by the WaveMaker translation
// unit. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
struct Matrix33 { float m[9]; };

extern "C" {
extern int frameNm;
extern unsigned char* world;
extern unsigned char waveTrail[];
extern unsigned char waveMainParticle[];
extern unsigned char waveMainAlternate[];
extern unsigned char waveRandomFoam[];
void matTransform__FP6Point3P8Matrix33T0(Point3* out, Matrix33* matrix, Point3* in);
void matRot__FP8Matrix33is(Matrix33* matrix, int axis, short angle);
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int iatan2__Fff(float a, float b);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float arg);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);

namespace {
inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

// One step of the generator stored at self+68; returns the upper 16 bits.
inline unsigned draw(void* self) {
    unsigned& state = w11c::word(self, 68);
    state = state * 1664525u + 1013904223u;
    return state >> 16;
}
inline float unit_draw(void* self) { return static_cast<float>(static_cast<int>(draw(self))) * bits(0x37800000u); }

inline float float_max(float a, float b) {
#if defined(__mips__)
    float result;
    __asm__("max.s %0, %1, %2" : "=f"(result) : "f"(a), "f"(b));
    return result;
#else
    return a > b ? a : b;
#endif
}
}  // namespace

void doWavelet__9WaveMakerP6Point3P8Matrix33i(void* self, const Point3* point, Matrix33* matrix, int n) {
    using namespace w11c;
    const float half_one = 0.5f;
    Point3 direction;
    direction.x = 0.0f;
    direction.y = (unit_draw(self) * bits(0x3ebef976u) + bits(0x3f5041a2u)) * -15.0f;
    direction.z = 0.0f;
    Point3 axis = {0.0f, bits(0xbfe0eeba), 0.0f};
    Point3 wave;
    matTransform__FP6Point3P8Matrix33T0(&wave, matrix, &axis);
    matRot__FP8Matrix33is(matrix, 0, shalf(self, 1876));
    if (((frameNm + n) & 15) == 0) {
        Point3 low = {point->x - 12.0f, point->y - 12.0f, point->z - 60.0f};
        Point3 high = {point->x - 12.0f, point->y + 12.0f, point->z + 10.0f};
        void* found[64];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
        for (int i = 0; found[i] != nullptr; ++i) {
            unsigned char* o = static_cast<unsigned char*>(found[i]);
            const unsigned flags = word(o, 12);
            if ((flags & 0x20u) == 0u) continue;
            if ((flags & 1u) != 0u) continue;
            if (point->z - real(o, 24) <= 12.0f) continue;
            real(o, 756) = 82.0f;
            const float a = (unit_draw(self) - half_one) * 0.25f;
            const float b = (unit_draw(self) - half_one) * 0.25f;
            half(o, 748) = static_cast<unsigned short>(iatan2__Fff(wave.y + a, wave.x + b));
        }
    }
    int counter = 0;
    do {
        Point3 q;
        matTransform__FP6Point3P8Matrix33T0(&q, matrix, &direction);
        q.x = q.x + point->x;
        q.y = q.y + point->y;
        q.z = q.z + point->z;
        const float water = real(self, 1888);
        if (q.z < water) {
            const float u1 = unit_draw(self);
            const float u2 = unit_draw(self);
            const float u3 = unit_draw(self);
            const float u4 = unit_draw(self);
            Point3 position;
            Point3 velocity;
            position.x = q.x - wave.x * 34.0f;
            position.y = q.y - wave.y * 34.0f;
            position.z = water;
            velocity.x = (u1 - half_one) * 5.0f;
            velocity.y = (u2 - half_one) * 5.0f;
            velocity.z = 0.0f;
            position.x += (u3 - half_one) * 36.0f;
            position.y += (u4 - half_one) * 36.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(waveTrail, &position, &velocity, 0.0f);
        } else if ((draw(self) & 1u) != 0u) {
            Point3 lift = {0.0f, 0.0f, static_cast<float>(-shalf(self, 1878)) * 15.0f * bits(0x3a03126eu)};
            Point3 r;
            matTransform__FP6Point3P8Matrix33T0(&r, matrix, &lift);
            r.x = r.x + wave.x;
            r.y = r.y + wave.y;
            r.z = r.z + wave.z;
            const float ua = unit_draw(self);
            const float ub = unit_draw(self);
            const float uc = unit_draw(self);
            const unsigned low_byte = draw(self) & 0xffu;
            const float spread = bits(0x42099918u);
            r.x += (ua - half_one) * spread;
            r.y += (ub - half_one) * spread;
            r.z += (uc - half_one) * spread * bits(0x3eaa7ef9u);
            r.z = float_max(-2.0f, r.z);
            P_AddParticle__FP11ParticleDefP6Point3T1f(low_byte != 0u ? waveMainParticle : waveMainAlternate, &q, &r, 0.0f);
            if ((draw(self) & 0x1ffu) == 0u) {
                Point3 foam;
                foam.x = wave.x;
                foam.y = wave.z;
                foam.z = unit_draw(self) * 10.0f + 2.0f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(waveRandomFoam, &q, &foam, 0.0f);
            }
        }
        matRot__FP8Matrix33is(matrix, 0, 11186);
        counter += 11186;
    } while (!(0xffff < counter));
    if (((frameNm + n) & 31) == 0) {
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, *point, bits(0x43d98000u), 24.0f);
    }
}
}
