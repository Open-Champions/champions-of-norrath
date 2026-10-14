// Observed generateDust__7CyclopsG6Point3 at 0x002038f0, 604 bytes. Arguments: the object and a Point3 passed
// by value. Nothing is returned. A float scale K is read from the image at 0x004811c8 (0x405ccccc, about
// 3.45; no symbol names it and no store to it was found in the image). Fifty times:
//  1. A direction (dx, dy, dz) is drawn inside the unit ball by rejection: three components, each twice
//     (eRand * 2^-16, eRand taken as a signed int) minus one, in the order x, y, z, repeated until
//     (x*x + y*y) + z*z is not above 1.0. Then each component is multiplied by K.
//  2. The next eRand, truncated to a short, is the angle a. The next eRand modulo 50 (unsigned), as a float,
//     times icos(a) is the particle's x offset; the next eRand modulo 50 times isin(a) is its y offset;
//     the next eRand modulo 20 minus 10 is its z offset.
//  3. The offsets are added to the argument point and P_AddParticle runs with the particle definition
//     hitParticleDust2, the resulting point, the direction and 0.0.
// The eRand calls run in the order given. hitParticleDust2 is a file-local object in the image at 0x007d70c0
// (a different file-local object of the same name is at 0x007d69c0); only the first is used here. Unknown: what
// the object's fields are beyond the Cyclops being its owner by the symbol name. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned char hitParticleDust2[];
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void generateDust__7CyclopsG6Point3(void* self, Point3 position) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float scale = bits(0x405ccccc);
    auto rand = [](unsigned char* object) { return eRand__10GameObject(object); };
    for (int i = 0; i < 50; ++i) {
        Point3 direction;
        unit_ball(o, direction, rand);
        direction.x *= scale;
        direction.y *= scale;
        direction.z *= scale;
        const short angle = static_cast<short>(eRand__10GameObject(o));
        Point3 point;
        point.x = static_cast<float>(static_cast<int>(eRand__10GameObject(o) % 50u)) * icos__Fi(angle);
        point.y = static_cast<float>(static_cast<int>(eRand__10GameObject(o) % 50u)) * isin__Fi(angle);
        point.z = static_cast<float>(static_cast<int>(eRand__10GameObject(o) % 20u) - 10);
        point.x += position.x;
        point.y += position.y;
        point.z += position.z;
        P_AddParticle__FP11ParticleDefP6Point3T1f(hitParticleDust2, &point, &direction, 0.0f);
    }
}
}
