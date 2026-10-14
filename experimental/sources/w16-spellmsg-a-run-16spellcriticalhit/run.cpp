// Observed run__16SpellCriticalHitG6Point3s at 0x002ad528, 1408 bytes, 432-byte frame. O is the object, the first argument. The
// second argument (a point, copied to the callee's frame and never used) and the third are not read. k = the pool constant with
// bits 0x3c888888 (about 1/60, no symbol names it). T is the object that is the word at O+8 and N0 the signed byte at O+0;
// "slot(i)" is the entry at offset 88 of the table whose address is the word at T+96, called on T (adjusted by the entry's
// signed halfword) with (i, N0) and returning an int.
//  1. The single at O+24 is lowered by k and stored. When the result is below zero the run ends: if O is not null, the entry at
//     offset 8 of the table whose address is the word at O+20 is called on O (adjusted by its signed halfword) with the
//     argument 3, and nothing else happens.
//  2. Otherwise runEffect__16SpellCriticalHitP8Creature(O, the word at O+12) is called; then the point (Q.x + 0.0, Q.y, Q.z + 60.0),
//     with Q the point at (the word at O+12) + 16, is stored at O+76 (three singles) and lightUpdatePosition__FP5Light is called
//     on O+28.
//  3. With R = slot(1) * 12 and C = netGetLocalObject__FP8Creature(the word at O+12): P_OpenParticleSystem__Fi is called on the
//     word at O+88; a matrix is built by matMakeTransYaw__FP8Matrix34G6Point3s from (C.x + 0.0, C.y, C.z + 41.0) where C.x, C.y,
//     C.z are the singles at C+16, +20, +24 (read after the open call), and the yaw (short)(frameNm * 118), and given to
//     P_SetParticleSystemMatrix__FiP8Matrix34 with the word at O+88. Then N0 + 58 times (none when that is not positive): a
//     point is drawn in the unit ball with eRand__5Spell(O) (three rolls each 2 * (roll * 2^-16) - 1, repeated while
//     1.0 < x*x + y*y + z*z), normalised with rsqrt.s, its z made non-negative and scaled by (float)R; a second unit-ball point
//     V is drawn likewise; then P_AddParticle__FP11ParticleDefP6Point3T1f is called with the particle definition record at
//     0x008d76d0 (80 bytes, a local of the image), the scaled point, V and 0.0.
//  4. P_CloseParticleSystem__Fi is called on the word at O+88. Then for each entry E of playersAlive[0 .. nmPlayersAlive-1] (the
//     count is read again after every runEffect call): with Y = the word at player + 23984 * E + 21872, when Y differs from the word
//     at O+12 and the distance from the point at (the word at O+12) + 16 to the point at Y+16 (differences squared and added
//     x, y, then z, and rooted) is below (float)R, runEffect__16SpellCriticalHitP8Creature(O, Y) is called.
// Inferred: a critical-hit aura that applies its effect to the caster and to every other player within a radius, with a particle
// ring and a light moved above the caster. Unknown: what runEffect does and the meaning of the words at O+76 and O+88 beyond
// their use here. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern int nmPlayersAlive;
extern int playersAlive[];
extern unsigned char player[];
extern unsigned char norrath_w16_spellmsg_a_view_8d76d0[80];
int eRand__5Spell(void* spell);
void* netGetLocalObject__FP8Creature(void* creature);
void runEffect__16SpellCriticalHitP8Creature(void* self, void* creature);
void lightUpdatePosition__FP5Light(void* light);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const Point3* point, short yaw);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, void* matrix);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
inline float reciprocal_length(float squared) {
    float out;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(one), "f"(squared));
    return out;
}
inline float roll_component(void* spell) {
    float v = static_cast<float>(eRand__5Spell(spell)) * w12c::bits(0x37800000u);
    return v + v - 1.0f;
}
}

extern "C" void run__16SpellCriticalHitG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    const float remaining = real(o, 24) - bits(0x3c888888u);
    real(o, 24) = remaining;
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    if (remaining < zero) {
        __asm__("" : "+r"(o));
        if (o != nullptr) {
            unsigned char* const entry = from_word(word(o, 20)) + 8;
            reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
        }
        return;
    }
    runEffect__16SpellCriticalHitP8Creature(o, from_word(word(o, 12)));
    {
        const unsigned char* const q = from_word(word(o, 12)) + 16;
        const Point3 above = {*reinterpret_cast<const float*>(q) + zero, *reinterpret_cast<const float*>(q + 4), *reinterpret_cast<const float*>(q + 8) + 60.0f};
        *reinterpret_cast<Point3*>(o + 76) = above;
    }
    lightUpdatePosition__FP5Light(o + 28);
    const int n0 = static_cast<signed char>(o[0]);
    unsigned char* const t = from_word(word(o, 8));
    int radius;
    {
        unsigned char* const entry = from_word(word(t, 96)) + 88;
        radius = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
                     t + *reinterpret_cast<short*>(entry), 1, n0) * 12;
    }
    unsigned char* const local = static_cast<unsigned char*>(netGetLocalObject__FP8Creature(from_word(word(o, 12))));
    P_OpenParticleSystem__Fi(static_cast<int>(word(o, 88)));
    const Point3 origin = {real(local, 16) + zero, real(local, 20), real(local, 24) + 41.0f};
    alignas(16) unsigned char matrix[64];
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &origin, static_cast<short>(frameNm * 118));
    P_SetParticleSystemMatrix__FiP8Matrix34(static_cast<int>(word(o, 88)), matrix);
    const int total = n0 + 58;
    const float scale = static_cast<float>(radius);
    for (int i = 0; i < total; ++i) {
        Point3 u;
        do {
            u.x = roll_component(o);
            u.y = roll_component(o);
            u.z = roll_component(o);
        } while (1.0f < u.x * u.x + u.y * u.y + u.z * u.z);
        const float inv = reciprocal_length(u.x * u.x + u.y * u.y + u.z * u.z);
        float z = u.z * inv;
        const float x = u.x * inv;
        const float y = u.y * inv;
        if (z < zero) z = -z;
        Point3 position = {x * scale, y * scale, z * scale};
        Point3 velocity;
        do {
            velocity.x = roll_component(o);
            velocity.y = roll_component(o);
            velocity.z = roll_component(o);
        } while (1.0f < velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d76d0, &position, &velocity, zero);
    }
    P_CloseParticleSystem__Fi(static_cast<int>(word(o, 88)));
    for (int i = 0; i < nmPlayersAlive; ++i) {
        unsigned char* const caster = from_word(word(o, 12));
        unsigned char* const other = from_word(word(player + playersAlive[i] * 23984, 21872));
        if (other == caster) continue;
        const float dx = *reinterpret_cast<const float*>(caster + 16) - *reinterpret_cast<const float*>(other + 16);
        const float dy = *reinterpret_cast<const float*>(caster + 20) - *reinterpret_cast<const float*>(other + 20);
        const float dz = *reinterpret_cast<const float*>(caster + 24) - *reinterpret_cast<const float*>(other + 24);
        if (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) < static_cast<float>(radius)) runEffect__16SpellCriticalHitP8Creature(o, other);
    }
}
