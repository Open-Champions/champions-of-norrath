// Observed msg_run__5Torch at 0x0022f700, 2024 bytes. m is the signed byte at object+138 (read again
// wherever the image reads it). A random component is (float)eRand(object) * 2^-16 doubled minus 1;
// a "ball point" is three components in x, y, z order, redrawn while 1.0 < x*x + y*y + z*z.
// Part 1, repeated n times with n = 4 when m is 4, 3 when m is 2 or 5, and 1 otherwise:
//  - v = ball point; v = {2x, 2y, 2z + 4.8f}.
//  - p = ball point scaled by m: m 2 or 5: {10x, 10y, z + 5}; m 3: {5x, 5y, z + 10}; m 4:
//    {15x, 15y, z + 10}; m 1: {10x, 10y, z}; any other m: {4x, 4y, 4z}. Then p.z = -p.z when p.z < 0.
//  - p = {p.x + object.x, p.y + object.y, (p.z + object.z) - 12.0f}, with the object position at
//    +16, +20, +24. P_AddParticle(torchParticle, &p, &v, 0.25f) when m is 3, else with 0.0f.
//  - When frameNm & 7 is zero and the unsigned byte at +138 is not 4 or 5: v.z += (float)eRand * 2^-16
//    * 20.0f; q = ball point; p = {10 q.x, 10 q.y, 10 q.z}; p.z = -p.z when negative; p += object
//    position (no -12); P_AddParticle(heatParticle, &p, &v, 0.0f).
// Part 2, once: with K = (int)object * 0x19660d (wrapping) and f = frameNm:
//  - noise1 = ef1Noise(0x7efe41 + K + 16 f); g = (float)((int)(noise1 * 22.0f) + 160); the words at
//    +104 and +112 get g, and the words at +108 and +116 get 1.0f / (g * g).
//  - the word at +124 gets object.x + 8 * ef1Noise(K + 2 f); +128 gets object.y + 8 * ef1Noise(K + 2 f
//    + 0x19660d); +132 gets object.z + 8 * ef1Noise(K + 2 f + 0x32cc1a).
//  - When m is neither 0 nor 4 and eRand(object) < 794 (unsigned): w = ball point; the velocity is
//    {30.36f w.x, 30.36f w.y, 30.36f w.z + 12.65f}; P_AddParticle(fireSparkParticle, object+16, &that,
//    0.0f).
//  - When the signed byte at +137 is at least 31: objectRemoveFromRunList(object) and the byte at +136
//    becomes 0. Then the byte at +137 is incremented.
// The three particle definitions are file-local objects of the original unit at 0x7d7ba0, 0x7d7c40 and
// 0x7d7c90, named here by address. The +104..+132 words sit in the Light member that starts at +76
// (clone and destructor); that is inferred. The constants 4.8f, 30.36f and 12.65f come from three
// consecutive .sdata words. Unknown: the meaning of the mode values. Nothing here ran on the EE.
extern "C" {
struct Point3 { float x, y, z; };
unsigned eRand__10GameObject(void* object);
float ef1Noise__Fi(int seed);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern int frameNm;
extern unsigned char torch_msg_run_particle_7d7ba0[];
extern unsigned char torch_msg_run_particle_7d7c40[];
extern unsigned char torch_msg_run_particle_7d7c90[];
}

typedef float __attribute__((may_alias)) Single;

namespace {
inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

inline float component(void* object) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * bits(0x37800000u);
    return v + v - 1.0f;
}

inline void ball(void* object, Point3& point) {
    do {
        point.x = component(object);
        point.y = component(object);
        point.z = component(object);
    } while (1.0f < point.x * point.x + point.y * point.y + point.z * point.z);
}
}  // namespace

extern "C" void msg_run__5Torch(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const signed char first_mode = static_cast<signed char>(object[138]);
    int count = 1;
    if (first_mode == 2) count = 3;
    else if (first_mode == 3) count = 1;
    else if (first_mode == 5) count = 3;
    if (first_mode == 4) count = 4;
    for (int i = 0; i < count; ++i) {
        Point3 v;
        Point3 p;
        ball(self, v);
        v.x = v.x + v.x;
        v.y = v.y + v.y;
        v.z = (v.z + v.z) + bits(0x40999999u);
        ball(self, p);
        const int m = static_cast<signed char>(object[138]);
        if (m == 2 || m == 5) {
            p.x = p.x * 10.0f; p.y = p.y * 10.0f; p.z = p.z + 5.0f;
        } else if (m == 3) {
            p.x = p.x * 5.0f; p.y = p.y * 5.0f; p.z = p.z + 10.0f;
        } else if (m == 4) {
            p.x = p.x * 15.0f; p.y = p.y * 15.0f; p.z = p.z + 10.0f;
        } else if (m == 1) {
            p.x = p.x * 10.0f; p.y = p.y * 10.0f;
        } else {
            p.x = p.x * 4.0f; p.y = p.y * 4.0f; p.z = p.z * 4.0f;
        }
        if (p.z < 0.0f) p.z = -p.z;
        p.x = p.x + *reinterpret_cast<Single*>(object + 16);
        p.y = p.y + *reinterpret_cast<Single*>(object + 20);
        p.z = (p.z + *reinterpret_cast<Single*>(object + 24)) - 12.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(torch_msg_run_particle_7d7ba0, &p, &v, m == 3 ? 0.25f : 0.0f);
        if ((frameNm & 7) == 0 && static_cast<unsigned>(object[138]) - 4u >= 2u) {
            v.z = v.z + static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u) * 20.0f;
            Point3 q;
            ball(self, q);
            p.x = q.x * 10.0f; p.y = q.y * 10.0f; p.z = q.z * 10.0f;
            if (p.z < 0.0f) p.z = -p.z;
            p.x = p.x + *reinterpret_cast<Single*>(object + 16);
            p.y = p.y + *reinterpret_cast<Single*>(object + 20);
            p.z = p.z + *reinterpret_cast<Single*>(object + 24);
            P_AddParticle__FP11ParticleDefP6Point3T1f(torch_msg_run_particle_7d7c40, &p, &v, 0.0f);
        }
    }
    const int K = static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self)) * 0x19660du);
    const int f = frameNm;
    const float noise1 = ef1Noise__Fi(0x7efe41 + K + f * 16);
    const float g = static_cast<float>(static_cast<int>(noise1 * 22.0f) + 160);
    const float inverse = 1.0f / (g * g);
    *reinterpret_cast<Single*>(object + 104) = g;
    *reinterpret_cast<Single*>(object + 112) = g;
    *reinterpret_cast<Single*>(object + 116) = inverse;
    *reinterpret_cast<Single*>(object + 108) = inverse;
    *reinterpret_cast<Single*>(object + 124) = *reinterpret_cast<Single*>(object + 16) + ef1Noise__Fi(K + f * 2) * 8.0f;
    *reinterpret_cast<Single*>(object + 128) = *reinterpret_cast<Single*>(object + 20) + ef1Noise__Fi(K + f * 2 + 0x19660d) * 8.0f;
    *reinterpret_cast<Single*>(object + 132) = *reinterpret_cast<Single*>(object + 24) + ef1Noise__Fi(K + f * 2 + 0x32cc1a) * 8.0f;
    const int mode = static_cast<signed char>(object[138]);
    if (mode != 0 && mode != 4 && eRand__10GameObject(self) < 794u) {
        Point3 w;
        ball(self, w);
        Point3 out;
        out.x = w.x * bits(0x41f2e147u);
        out.y = w.y * bits(0x41f2e147u);
        out.z = w.z * bits(0x41f2e147u) + bits(0x414a6666u);
        P_AddParticle__FP11ParticleDefP6Point3T1f(torch_msg_run_particle_7d7c90, reinterpret_cast<Point3*>(object + 16), &out, 0.0f);
    }
    if (static_cast<signed char>(object[137]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(self);
        object[136] = 0;
    }
    object[137] = static_cast<unsigned char>(object[137] + 1);
}
