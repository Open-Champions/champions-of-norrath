// Observed msg_run__10WaterSpout at 0x001d4dd8, 2136 bytes. Offsets are used for fields: P is the
// point at +16, X and Y the floats at +76 and +80 (the constructor stores 9.2 times the cosine and
// the sine of the angle there; they are used here as a horizontal drift), and the signed bytes at
// +88 (run-list flag, see msg_draw) and +89 (a counter).
//  1. Three particle definitions are built on first use, each behind its own flag word, by the
//     ParticleDef constructor __11ParticleDefiiiiiiiifffiiiPFPv_v, in the order splash, water, foam.
//     Observed arguments (eight ints, three floats, three ints, and a null function pointer):
//       splash: 100,110,100,113,30,40,30,20, 82.538 / 156.578 / 2.0, 2,0,2
//       water:  70,83,74,59,31,43,31,55,     21.0 / 81.0 / -5.8,      2,0,2
//       foam:   90,102,93,59,31,40,30,55,    -10.96 / 60.497 / -5.88, 2,1,1
//     The three objects are 80 bytes each and are 16-byte aligned in the original; they are local
//     statics there and are private data here.
//  2. Every call: two splash particles, each from P_AddParticle__FP11ParticleDefP6Point3T1f(def,
//     position, velocity, 0.0), where a random point in the unit ball u gives position
//     (u.x * 8.2 + P.x + 4 * X, u.y * 8.2 + P.y + 4 * Y, P.z - 56) and a second ball point v gives
//     velocity v * 1.48. Then two water particles with position w * 8.2 + P (w a ball point) and
//     velocity z * 1.48 + (X, Y, 0). Then, when frameNm is even, one foam particle with position
//     u * 8.25 + P and velocity g * 1.4 + (X, Y, 0).
//     Ball points use eRand__10GameObject: each coordinate is 2 * (draw * 2^-16) - 1, redrawn as a
//     whole while the squared length exceeds 1.
//  3. When the low five bits of frameNm are zero: worldPerturbWater__FP12_worldHeaderG6Point3ff(world,
//     (P.x + 4 X, P.y + 4 Y, P.z - 56), 1043.0, 24.0).
//  4. When the byte at +89 is 31 or more: objectRemoveFromRunList__FP10GameObject(this) and the byte
//     at +88 is cleared. The byte at +89 is then incremented.
// Unknown: the particle definition layout and its constructor's parameter meanings; the origin of the
// float constants (all read as literals). Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
extern unsigned char* world;
extern int frameNm;
unsigned eRand__10GameObject(void* object);
void particledef_construct(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k,
                           void (*callback)(void*)) __asm__("__11ParticleDefiiiiiiiifffiiiPFPv_v");
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float delay);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
alignas(16) unsigned char splashParticle[80];
alignas(16) unsigned char waterParticle[80];
alignas(16) unsigned char foamParticle[80];
int splashBuilt, waterBuilt, foamBuilt;

unsigned draw(unsigned char* object) { return eRand__10GameObject(object); }
}  // namespace

extern "C" void msg_run__10WaterSpout(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (splashBuilt == 0) {
        particledef_construct(splashParticle, 100, 110, 100, 113, 30, 40, 30, 20, bits(0x42a51374u), bits(0x431c93f7u), 2.0f, 2, 0, 2, nullptr);
        splashBuilt = 1;
    }
    if (waterBuilt == 0) {
        particledef_construct(waterParticle, 70, 83, 74, 59, 31, 43, 31, 55, 21.0f, 81.0f, bits(0xc0b99999u), 2, 0, 2, nullptr);
        waterBuilt = 1;
    }
    if (foamBuilt == 0) {
        particledef_construct(foamParticle, 90, 102, 93, 59, 31, 40, 30, 55, bits(0xc12f5c28u), bits(0x4271fcedu), bits(0xc0bc28f5u), 2, 1, 1, nullptr);
        foamBuilt = 1;
    }
    const Point3& p = *reinterpret_cast<const Point3*>(o + 16);
    Point3 position;
    Point3 velocity;
    for (int i = 0; i < 2; ++i) {
        unit_ball(o, position, draw);
        position.x = position.x * bits(0x41033333u);
        position.y = position.y * bits(0x41033333u);
        position.z = p.z - 56.0f;
        position.x = position.x + p.x;
        position.y = position.y + p.y;
        position.x = position.x + real(o, 76) * 4.0f;
        position.y = position.y + real(o, 80) * 4.0f;
        unit_ball(o, velocity, draw);
        velocity.x = velocity.x * bits(0x3fbd70a3u);
        velocity.y = velocity.y * bits(0x3fbd70a3u);
        velocity.z = velocity.z * bits(0x3fbd70a3u);
        P_AddParticle__FP11ParticleDefP6Point3T1f(splashParticle, &position, &velocity, 0.0f);
    }
    for (int i = 0; i < 2; ++i) {
        unit_ball(o, position, draw);
        position.x = position.x * bits(0x41033333u) + p.x;
        position.y = position.y * bits(0x41033333u) + p.y;
        position.z = position.z * bits(0x41033333u) + p.z;
        unit_ball(o, velocity, draw);
        velocity.x = velocity.x * bits(0x3fbd70a3u) + real(o, 76);
        velocity.y = velocity.y * bits(0x3fbd70a3u) + real(o, 80);
        velocity.z = velocity.z * bits(0x3fbd70a3u);
        P_AddParticle__FP11ParticleDefP6Point3T1f(waterParticle, &position, &velocity, 0.0f);
    }
    if (((frameNm ^ 1) & 1) != 0) {
        unit_ball(o, position, draw);
        position.x = position.x * 8.25f + p.x;
        position.y = position.y * 8.25f + p.y;
        position.z = position.z * 8.25f + p.z;
        unit_ball(o, velocity, draw);
        velocity.x = velocity.x * bits(0x3fb33333u) + real(o, 76);
        velocity.y = velocity.y * bits(0x3fb33333u) + real(o, 80);
        velocity.z = velocity.z * bits(0x3fb33333u);
        P_AddParticle__FP11ParticleDefP6Point3T1f(foamParticle, &position, &velocity, 0.0f);
    }
    if ((frameNm & 0x1f) == 0) {
        const Point3 spot = {p.x + real(o, 76) * 4.0f, p.y + real(o, 80) * 4.0f, p.z - 56.0f};
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, spot, bits(0x44826000u), 24.0f);
    }
    if (static_cast<signed char>(o[89]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(self);
        o[88] = 0;
    }
    o[89] = static_cast<unsigned char>(o[89] + 1);
}
