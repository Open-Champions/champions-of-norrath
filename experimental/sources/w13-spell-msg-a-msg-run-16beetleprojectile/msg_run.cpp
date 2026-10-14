// Observed msg_run__16beetleProjectile at 0x00211f70, 1204 bytes. O is the object, P the point (three floats) at O+16 and
// frameNm the global frame counter. A unit-ball vector is drawn by rejection as in the other spell sources: each component is
// eRand__10GameObject(O) * 2^-16 doubled minus 1.0, x then y then z, repeated while 1.0 < (x*x + y*y) + z*z. Pool constants
// with no symbols: K1 = bits 0x3fa3d70a (1.28), K2 = bits 0x41c5c28f (24.72), K3 = bits 0x410fffff (9.0 minus one ulp).
// The two particle definitions are 80-byte objects in the image's bss (0x007d72b0 and 0x007d7300); they are imported here by
// address-named views.
//  1. Twice: draw U; the vector V is (U.x * 5, U.y * 5, |U.z| * 25) -- the image multiplies the z part by 5 twice -- and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(fuse definition, P, &V, 0.0) runs.
//  2. Draw U; V is (U.x * K1, U.y * K1, (|U.z| * K1) * K2); P_AddParticle(smoke definition, P, &V, 0.0) runs.
//  3. The single at O+436 drops by K3 and move__10ProjectileG6Point3(O, copy of the point at O+428) runs; when it returns
//     non-zero the word at O+236 becomes 0.
//  4. When frameNm is below the signed word at O+236 the function ends. Otherwise soundPlaySequence__FP15AnimationHeaderP10Game-
//     Objectf(the word at O+456, O, 1.0) runs and then, five times: draw U; A = U * 20 + P; h = worldFindHeight(world, copy of
//     P, 6.0); A.z = h + 1.0; when the word at O+452 (X) is non-zero, 84 bytes are allocated with blockAlloc__Fi and a non-null
//     block gets the LooseFire constructor __9LooseFireG6Point3P10GameObject(block, copy of A, the word at X+12); then
//     floorAdd__FG6Point3sfii(C, (short)eRand__10GameObject(O), 24.0, 9, 0) with C = (A.x, A.y, A.z + 1.0). After the five
//     passes the entry at table offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a beetle spit projectile that trails fuse and smoke particles, and when it lands (move returns non-zero and the
// delay at O+236 has passed) throws five ground fires. Unknown: what the words at O+236, +452, +456, the single at O+436
// (a countdown by K3 per frame) and the constants K1..K3 mean. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w13_spell_msg_a_view_7d72b0[80];
extern unsigned char norrath_w13_spell_msg_a_view_7d7300[80];
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 p, float radius);
void* blockAlloc__Fi(int size);
void* __9LooseFireG6Point3P10GameObject(void* self, Point3 position, void* owner);
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

extern "C" void msg_run__16beetleProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    for (int pass = 0; pass < 2; ++pass) {
        Point3 v;
        unit_ball(o, v, roll);
        float z = v.z;
        if (z < 0.0f) {
            z = -z;
            v.z = z;
        }
        z = z * 5.0f;
        v.x = v.x * 5.0f;
        v.y = v.y * 5.0f;
        v.z = z * 5.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d72b0, position, &v, 0.0f);
    }
    {
        Point3 v;
        unit_ball(o, v, roll);
        float z = v.z;
        if (z < 0.0f) {
            z = -z;
            v.z = z;
        }
        z = z * bits(0x3fa3d70au);
        v.x = v.x * bits(0x3fa3d70au);
        v.y = v.y * bits(0x3fa3d70au);
        v.z = z * bits(0x41c5c28fu);
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d7300, position, &v, 0.0f);
    }
    real(o, 436) = real(o, 436) - bits(0x410fffffu);
    Point3 delta = *reinterpret_cast<Point3*>(o + 428);
    if (move__10ProjectileG6Point3(o, delta) != 0) word(o, 236) = 0;
    if (frameNm < sword(o, 236)) return;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(o, 456)), o, 1.0f);
    for (int i = 0; i < 5; ++i) {
        Point3 a;
        unit_ball(o, a, roll);
        a.x = a.x * 20.0f;
        a.y = a.y * 20.0f;
        a.z = a.z * 20.0f;
        a.y = a.y + position->y;
        a.x = a.x + position->x;
        a.z = a.z + position->z;
        Point3 probe = *position;
        a.z = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 6.0f) + 1.0f;
        if (word(o, 452) != 0) {
            void* const block = blockAlloc__Fi(84);
            if (block != nullptr) __9LooseFireG6Point3P10GameObject(block, a, from_word(word(from_word(word(o, 452)), 12)));
        }
        Point3 c;
        c.x = a.x;
        c.y = a.y;
        c.z = a.z + 1.0f;
        floorAdd__FG6Point3sfii(c, static_cast<short>(eRand__10GameObject(o)), 24.0f, 9, 0);
    }
    call_entry(o, 8, 3);
}
