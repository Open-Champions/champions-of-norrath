// Observed fxFlyingFire__FP6Point3T0ffb at 0x001dfe98, 2516 bytes. Arguments: a position P, a second vector Q (both by
// pointer), floats A and B, and a flag F. All random numbers come from the global eRandState, advanced inline by
// state * 0x19660d + 0x3c6ef35f with the result being the new state shifted right by 16 (the same step as
// eRand__5Spell and eRand__10GameObject, but on the global); a "ball" draw below takes three such results as
// x, y, z, each float(roll) * 2^-16 doubled minus 1, and repeats while 1.0 < x*x + y*y + z*z (summed in that order).
// Steps in order:
//  1. Nothing happens unless gameCloseToCamera__FP6Point3(P) is non-zero.
//  2. S is 1 when the word at gameCam + 84 is zero and the horizontal distance squared from P to worldCameraPos
//     (the first two floats) is below the pool float 0x484cd900 (209764.0), else 0. S picks the particle record in step 3.
//  3. n1 = round(B * 3) (half away from zero, as (int)(x + 0.5) for x >= 0 and (int)(x - 0.5) otherwise). The start point
//     T is P, or P - 2Q (componentwise) when F is true. n1 times: velocity = Q * 0.75 + ball1 * 4; point = T + ball2 * A;
//     P_AddParticle__FP11ParticleDefP6Point3T1f(smokeParticleSmall if S else smokeParticle (0x007d6920 / 0x007d68d0),
//     point, velocity, 0.0). Q is read again every pass.
//  4. n2 = round(B * 10). n2 times: velocity = Q * 0.5; point = P + ball * A; record flameParticle (0x007d6880).
//  5. Twice: velocity = Q * 0.5 + ball1 * 4; point = P + ball2 * (A + 2.0); record sparkParticle (0x007d6830).
// Inferred: this is a flying-fire trail effect. Unknown: what the arguments mean beyond their use (A looks like a spread,
// B a density, F a "lead" flag) and the pool constant. The four particle records are file-local objects of the original
// unit (imported here under address names). Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned eRandState;
extern void* gameCam;
extern float worldCameraPos[];
extern unsigned char fxflyingfire_particle_7d6920[];
extern unsigned char fxflyingfire_particle_7d68d0[];
extern unsigned char fxflyingfire_particle_7d6880[];
extern unsigned char fxflyingfire_particle_7d6830[];
int gameCloseToCamera__FP6Point3(Point3* position);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);

namespace {
using namespace w12c;

inline float roll_unit() {
    eRandState = eRandState * 0x19660du + 0x3c6ef35fu;
    const float r = static_cast<float>(static_cast<int>(eRandState >> 16)) * bits(0x37800000u);
    return r + r - 1.0f;
}

inline void ball(Point3& d) {
    do {
        d.x = roll_unit();
        d.y = roll_unit();
        d.z = roll_unit();
    } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
}

inline int rounded(float x) { return 0.0f <= x ? static_cast<int>(x + 0.5f) : static_cast<int>(x - 0.5f); }
}  // namespace

void fxFlyingFire__FP6Point3T0ffb(Point3* position, Point3* q, float a, float b, bool lead) {
    if (gameCloseToCamera__FP6Point3(position) == 0) return;
    bool small = false;
    if (word(static_cast<unsigned char*>(gameCam), 84) == 0u) {
        const float dy = position->y - worldCameraPos[1];
        const float dx = position->x - worldCameraPos[0];
        const float d = dx * dx + dy * dy;
        if (d < bits(0x484cd900u)) small = true;
    }
    const int n1 = rounded(b * 3.0f);
    Point3 start;
    if (lead) {
        const float x2 = q->x + q->x;
        const float y2 = q->y + q->y;
        const float z2 = q->z + q->z;
        start.y = position->y - y2;
        start.z = position->z - z2;
        start.x = position->x - x2;
    } else {
        start = *position;
    }
    for (int i = 0; i < n1; ++i) {
        Point3 base;
        base.x = q->x * 0.75f;
        base.y = q->y * 0.75f;
        base.z = q->z * 0.75f;
        Point3 d1;
        ball(d1);
        const float sx = d1.x * 4.0f;
        const float sy = d1.y * 4.0f;
        const float sz = d1.z * 4.0f;
        Point3 velocity;
        velocity.x = base.x + sx;
        velocity.y = base.y + sy;
        velocity.z = base.z + sz;
        Point3 d2;
        ball(d2);
        Point3 e;
        e.x = d2.x * a;
        e.y = d2.y * a;
        e.z = d2.z * a;
        Point3 point;
        point.x = start.x + e.x;
        point.y = start.y + e.y;
        point.z = start.z + e.z;
        P_AddParticle__FP11ParticleDefP6Point3T1f(small ? fxflyingfire_particle_7d6920 : fxflyingfire_particle_7d68d0, &point, &velocity, 0.0f);
    }
    const int n2 = rounded(b * 10.0f);
    for (int i = 0; i < n2; ++i) {
        Point3 velocity;
        velocity.x = q->x * 0.5f;
        velocity.y = q->y * 0.5f;
        velocity.z = q->z * 0.5f;
        Point3 d;
        ball(d);
        Point3 e;
        e.x = d.x * a;
        e.y = d.y * a;
        e.z = d.z * a;
        Point3 point;
        point.y = position->y + e.y;
        point.z = position->z + e.z;
        point.x = position->x + e.x;
        P_AddParticle__FP11ParticleDefP6Point3T1f(fxflyingfire_particle_7d6880, &point, &velocity, 0.0f);
    }
    for (int i = 0; i < 2; ++i) {
        Point3 base;
        base.x = q->x * 0.5f;
        base.y = q->y * 0.5f;
        base.z = q->z * 0.5f;
        Point3 d1;
        ball(d1);
        const float sx = d1.x * 4.0f;
        const float sy = d1.y * 4.0f;
        const float sz = d1.z * 4.0f;
        Point3 velocity;
        velocity.x = base.x + sx;
        velocity.y = base.y + sy;
        velocity.z = base.z + sz;
        Point3 d2;
        ball(d2);
        const float spread = a + 2.0f;
        Point3 e;
        e.y = d2.y * spread;
        e.z = d2.z * spread;
        e.x = d2.x * spread;
        Point3 point;
        point.y = position->y + e.y;
        point.z = position->z + e.z;
        point.x = position->x + e.x;
        P_AddParticle__FP11ParticleDefP6Point3T1f(fxflyingfire_particle_7d6830, &point, &velocity, 0.0f);
    }
}
}
