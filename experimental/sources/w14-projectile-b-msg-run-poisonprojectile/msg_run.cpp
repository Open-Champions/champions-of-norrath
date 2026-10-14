// Observed msg_run__16PoisonProjectile at 0x0020f390, 908 bytes. O is the object, P the point (three floats) at O+16
// and frameNm the global frame counter. Steps in order:
//  1. animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(O+100, P, null, null).
//  2. Four times: a random direction is drawn by rejection. Each component is eRand__10GameObject(O) * 2^-16 doubled
//     minus 1.0 (x, y, z in that order, each call separate); the draw repeats while 1.0 < (x*x + y*y) + z*z. The
//     direction is scaled by 8.0 on every component, copied, and given to P_AddParticle__FP11ParticleDefP6Point3T1f
//     (norrath_w13_spell_msg_a_view_7d7260, P, &copy, 0.0).
//  3. The single at O+148 drops by 4.95 (the pool constant bits 0x409e6664, no symbol names it). Then
//     move__10ProjectileG6Point3(O, copy of the point at O+140) returns r.
//  4. When r is non-zero:
//     - soundPlaySequence__FP15AnimationHeaderP10GameObjectf(the word at +4 of entry [word O+96] of the table whose
//       address is the word at O+36 (entries are 8 bytes), O, 1.0);
//     - the word at O+156 becomes 0;
//     - when r is exactly 1: the word at O+152 becomes 1; h = worldFindHeight(world, copy of P, 5.0); when the single at
//       O+24 is below h it becomes h; then, when that single is below (the single at O+164) - 24.0, floorAdd is
//       called with (copy of P, (short) a, (float)(12 + b % 6), 4 + (c & 1), 0), where a, b and c are three
//       successive eRand__10GameObject(O) results (b is taken unsigned);
//     - when the word at O+152 is non-zero and the word Q at O+76 is non-zero: d = gameDifficultyScale(); then
//       objectRadialDamage__FP10GameObjectG6Point3iffiii(the word at Q+12, copy of P, (int)(d + d), 12.0, 40.0, 0x10000,
//       5, 0) and addEffect__FiG6Point3(6, copy of P).
//  5. Finally, when frameNm is not below the word at O+156: the entry at table offset 8 of O's table (the word at +72)
//     is called on O (adjusted by the entry's signed halfword) with the argument 3.
// Inferred: this is the per-frame run of a poison projectile (spits particles, moves, lands). Unknown: what the words
// at O+96, +152, +156 and +164, the two radial-damage floats and the entry at offset 8 mean. Nothing here ran on the EE.
// The particle definition is the 80-byte bss object at 0x007d7260 (a file-local symbol named particlePoisonDrop in the image);
// the local name cannot be linked to, so it is imported through the address-named view norrath_w13_spell_msg_a_view_7d7260.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w13_spell_msg_a_view_7d7260[80];
int eRand__10GameObject(void* object);
int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* other, short* extra);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 p, float radius);
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d);
float gameDifficultyScale__Fv();
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* owner, Point3 p, int amount, float radius, float f, int a, int b, int c);
void addEffect__FiG6Point3(int kind, Point3 p);

void msg_run__16PoisonProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(o + 100, position, nullptr, nullptr);
    const float scale = bits(0x37800000u);
    for (int i = 0; i < 4; ++i) {
        Point3 d;
        do {
            d.x = static_cast<float>(eRand__10GameObject(o)) * scale;
            d.x = d.x + d.x - 1.0f;
            d.y = static_cast<float>(eRand__10GameObject(o)) * scale;
            d.y = d.y + d.y - 1.0f;
            d.z = static_cast<float>(eRand__10GameObject(o)) * scale;
            d.z = d.z + d.z - 1.0f;
        } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
        Point3 v = d;
        v.x = v.x * 8.0f;
        v.y = v.y * 8.0f;
        v.z = v.z * 8.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d7260, position, &v, 0.0f);
    }
    real(o, 148) = real(o, 148) - bits(0x409e6664u);
    Point3 delta = *reinterpret_cast<Point3*>(o + 140);
    const int r = move__10ProjectileG6Point3(o, delta);
    if (r != 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            from_word(word(from_word(word(o, 36)) + (sword(o, 96) << 3), 4)), o, 1.0f);
        word(o, 156) = 0;
        if (r == 1) {
            word(o, 152) = 1;
            Point3 probe = *position;
            const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 5.0f);
            if (real(o, 24) < h) real(o, 24) = h;
            if (real(o, 24) < real(o, 164) - 24.0f) {
                const int a = eRand__10GameObject(o);
                const unsigned b = static_cast<unsigned>(eRand__10GameObject(o));
                const int c = eRand__10GameObject(o);
                Point3 spot = *position;
                floorAdd__FG6Point3sfii(spot, static_cast<short>(a), static_cast<float>(static_cast<int>(b % 6u) + 12), (c & 1) + 4, 0);
            }
        }
        if (word(o, 152) != 0 && word(o, 76) != 0) {
            const float d = gameDifficultyScale__Fv();
            Point3 center = *position;
            objectRadialDamage__FP10GameObjectG6Point3iffiii(from_word(word(from_word(word(o, 76)), 12)), center,
                                                              static_cast<int>(d + d), 12.0f, 40.0f, 0x10000, 5, 0);
            Point3 effectAt = *position;
            addEffect__FiG6Point3(6, effectAt);
        }
    }
    if (frameNm >= sword(o, 156)) call_entry(o, 8, 3);
}
}
