#include "../w11-actor-c-common/c.hpp"
// Observed msg_run__6Sparks at 0x00216970, 1716 bytes. The object s is a Projectile with a ring of particle
// effects. Notation: f is the global frameNm, e the word at s+84 (an end frame; zero means ended), p the
// three floats at s+16, v the three floats at s+88, and a unit-ball draw is three eRand(s) rolls (each
// (float)(int)roll * 2^-16, doubled, minus 1.0) repeated while 1.0 < x*x + y*y + z*z.
//  A. When f < e (signed):
//     1. Projectile::move(s, v) runs; when it returns 1, e is cleared.
//     2. soundPan2D(word at s+164, p, 1.0).
//     3. Three times: a unit-ball draw, each component times 9.6, then P_AddParticle(trail, p, that vector, 0.0).
//     4. Five times: a unit-ball draw whose scaled value the original never uses, then a second unit-ball draw,
//        each component times 9.6, then P_AddParticle(smoke, p, that vector, 0.0).
//     5. One roll; when its low bit is set: D = v * (1/60) * 5.0, E = p - D, a unit-ball draw times 10.0, and
//        P_AddParticle(fireSmoke, E, that vector, 0.0).
//     6. p is copied to the floats at s+148 and lightUpdatePosition runs on the member at s+100.
//  B. Otherwise (f >= e): fifty times a unit-ball draw times 57.6 and P_AddParticle(hit, p, vector, 0.0).
//     When the signed word at s+176 is above zero: with t the word at (word at s+76) + 12 (or s itself when the word
//     at s+76 is zero), makeExplosion(p, 2.0) and objectRadialDamage(t, p, w176, 40.0, (float)w180, 0x40820,
//     w176, 0). Then s is destroyed through the virtual slot at byte 8 of its table with the int 3.
// The four particle definitions (trailParticle, fireSmokeParticle, fireSmoke, fireHitParticle in the image's
// symbol names; 80-byte records at 0x007d7540, 0x007d7590, 0x007d7680 and 0x007d7630) are file-local data of
// the original unit and are unresolved data imports here. Inferred: step B is the sparks' death burst with
// radial damage; the words at +176 and +180 look like a damage amount and radius. Unknown: the exact meaning
// of the words at +84, +164, +176 and +180 and which constants are tuning values. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern int frameNm;
extern unsigned char norrath_w11_actor_c_view_7d7540[];   // trailParticle
extern unsigned char norrath_w11_actor_c_view_7d7590[];   // fireSmokeParticle
extern unsigned char norrath_w11_actor_c_view_7d7630[];   // fireHitParticle
extern unsigned char norrath_w11_actor_c_view_7d7680[];   // fireSmoke
unsigned eRand__10GameObject(void* self);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void soundPan2D__FiP6Point3f(int sound, Point3* position, float gain);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float extra);
void lightUpdatePosition__FP5Light(void* light);
void makeExplosion__FG6Point3f(Point3 position, float size);
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* object, Point3 position, int a, float b, float c, int d, int e, int f);

namespace {
inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

inline float roll_component(void* self) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
    return v + v - 1.0f;
}

inline Point3 unit_ball(void* self) {
    Point3 p;
    do {
        p.x = roll_component(self);
        p.y = roll_component(self);
        p.z = roll_component(self);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
    return p;
}

inline Point3 scaled_ball(void* self, float scale) {
    Point3 p = unit_ball(self);
    p.x *= scale;
    p.y *= scale;
    p.z *= scale;
    return p;
}
}  // namespace

void msg_run__6Sparks(void* self) {
    using namespace w11c;
    Point3* const position = reinterpret_cast<Point3*>(bytes(self) + 16);
    if (frameNm < static_cast<int>(word(self, 84))) {
        const Point3 velocity = *reinterpret_cast<const Point3*>(bytes(self) + 88);
        if (move__10ProjectileG6Point3(self, velocity) == 1) word(self, 84) = 0u;
        soundPan2D__FiP6Point3f(static_cast<int>(word(self, 164)), position, 1.0f);
        const float spread = bits(0x41199999u);
        for (int i = 0; i < 3; ++i) {
            Point3 v = scaled_ball(self, spread);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w11_actor_c_view_7d7540, position, &v, 0.0f);
        }
        for (int i = 0; i < 5; ++i) {
            (void)unit_ball(self);
            Point3 v = scaled_ball(self, spread);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w11_actor_c_view_7d7590, position, &v, 0.0f);
        }
        if ((eRand__10GameObject(self) & 1u & 0xffffu) != 0u) {
            const float step = bits(0x3c888888u);
            const float cx = velocity.x * step, cy = velocity.y * step, cz = velocity.z * step;
            const float five = 5.0f;
            Point3 d;
            d.x = cx * five;
            d.y = cy * five;
            d.z = cz * five;
            Point3 origin;
            origin.x = position->x - d.x;
            origin.y = position->y - d.y;
            origin.z = position->z - d.z;
            Point3 v = scaled_ball(self, 10.0f);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w11_actor_c_view_7d7680, &origin, &v, 0.0f);
        }
        *reinterpret_cast<Point3*>(bytes(self) + 148) = *position;
        lightUpdatePosition__FP5Light(bytes(self) + 100);
        return;
    }
    const float burst = bits(0x42666666u);
    for (int i = 0; i < 50; ++i) {
        Point3 v = scaled_ball(self, burst);
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w11_actor_c_view_7d7630, position, &v, 0.0f);
    }
    if (static_cast<int>(word(self, 176)) > 0) {
        void* target = self;
        if (word(self, 76) != 0u) target = link(link(self, 76), 12);
        makeExplosion__FG6Point3f(*position, 2.0f);
        objectRadialDamage__FP10GameObjectG6Point3iffiii(target, *position, static_cast<int>(word(self, 176)), 40.0f,
                                                       static_cast<float>(static_cast<int>(word(self, 180))), 0x40820, static_cast<int>(word(self, 176)), 0);
    }
    call_slot_int(self, 8, 3);
}
}
