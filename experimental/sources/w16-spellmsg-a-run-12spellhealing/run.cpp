// Observed run__12SpellHealingG6Point3s at 0x002aeb38, 1600 bytes, 416-byte frame. O is the object, the first argument. The
// second argument (a point, copied to the callee's frame and never used) and the third are not read. Constants named by
// their float bits (no symbol names them): k1 = 0x3c23d70a (about 0.01), k2 = 0x3c886600 (about 0.01665), k3 = 0x3a81a000
// (about 0.000989), k4 = 0x3c888888 (about 1/60) and k5 = 0x3f802068 (about 1.000989). T is the object that is the word at
// O+8 and N0 the signed byte at O+0; "slot(i)" is the entry at offset 88 of the table whose address is the word at T+96, called
// on T (adjusted by the entry's signed halfword) with (i, N0) and returning an int.
//  1. R = slot(1) * 12. Every entry E of playersAlive[0 .. nmPlayersAlive-1] whose record P = player + 23984 * E has a creature
//     (the word at P+21872) within distance R of the point at (the word at O+12) + 16 is collected in order (the distance
//     is caster-minus-creature differences squared and added x, y, then z, and rooted; the test is distance <= (float)R).
//  2. For each collected E (P as above): when the single at P+172 plus 1.0 is not above the single at P+176, then if the word
//     at P+21872 equals the word at O+24 the single at P+172 gains the single at O+32; otherwise
//     P[172] = P[172] + (P[176] * k2) * ((float)slot(2) * k1). Whether or not that applied, when the single at O+28 is below
//     k3 the halfwords of the creature at P+21872 are set: +154 = 50, +148 = 53, +140 = 53, +150 = 20, +142 = 20, +152 = 13,
//     +144 = 13, +146 = 50.
//  3. With C = netGetLocalObject__FP8Creature(the word at O+12) and the point Q = C+16 (read after the call that opens the system):
//     P_OpenParticleSystem__Fi is called on the word at O+36; a matrix is built by matMakeTransYaw__FP8Matrix34G6Point3s from
//     the point (Q.x + 0.0, Q.y, Q.z + 19.0) and the yaw (short)(frameNm * 95) and given to P_SetParticleSystemMatrix__FiP8Matrix34
//     with the word at O+36. Then 25 * N0 + 45 times (none when that is not positive): a point is drawn in the unit ball with
//     eRand__5Spell(O) (three rolls each 2 * (roll * 2^-16) - 1, repeated while 1.0 < x*x + y*y + z*z), normalised with rsqrt.s
//     (components x, z, y multiplied by 1 / sqrt(squared length)), its z made non-negative, and scaled by (float)R; a second
//     unit-ball point is drawn (three more rolls, repeated likewise) and discarded; then
//     P_AddParticle__FP11ParticleDefP6Point3T1f is called with the particle definition record at 0x008d7770 (80 bytes, a local
//     of the image), the scaled point, the point (0, 0, 0) and 0.0.
//  4. P_CloseParticleSystem__Fi(the word at O+36) is called. The single at O+28 gains k4; when k5 is below the new value, the
//     entry at offset 8 of the table whose address is the word at O+20 is called on O (adjusted by the entry's signed halfword)
//     with the argument 3 (no null test for O here).
// Inferred: a healing aura that tops up the running amount of nearby players for a few seconds with a ring of particles.
// Unknown: the meaning of P+172/+176 beyond their use here, and of the halfwords. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern int nmPlayersAlive;
extern int playersAlive[];
extern unsigned char player[];
extern unsigned char norrath_w16_spellmsg_a_view_8d7770[80];
int eRand__5Spell(void* spell);
void* netGetLocalObject__FP8Creature(void* creature);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const Point3* point, short yaw);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, void* matrix);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
int slot88(unsigned char* t, int index, int n) {
    using namespace w12c;
    unsigned char* const entry = from_word(word(t, 96)) + 88;
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        t + *reinterpret_cast<short*>(entry), index, n);
}
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

extern "C" void run__12SpellHealingG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = from_word(word(o, 8));
    const int radius = slot88(t, 1, static_cast<signed char>(o[0])) * 12;
    int found[4];
    int count = 0;
    {
        const Point3* const centre = reinterpret_cast<const Point3*>(from_word(word(o, 12)) + 16);
        const float limit = static_cast<float>(radius);
        for (int i = 0; i < nmPlayersAlive; ++i) {
            const int entry = playersAlive[i];
            const Point3* const at = reinterpret_cast<const Point3*>(from_word(word(player + entry * 23984, 21872)) + 16);
            const float dx = centre->x - at->x;
            const float dy = centre->y - at->y;
            const float dz = centre->z - at->z;
            if (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) <= limit) found[count++] = entry;
        }
    }
    for (int i = 0; i < count; ++i) {
        unsigned char* const p = player + found[i] * 23984;
        const float amount = real(p, 172);
        if (amount + 1.0f <= real(p, 176)) {
            if (word(p, 21872) == word(o, 24)) {
                real(p, 172) = amount + real(o, 32);
            } else {
                const float scale = static_cast<float>(slot88(t, 2, static_cast<signed char>(o[0]))) * bits(0x3c23d70au);
                real(p, 172) = real(p, 172) + (real(p, 176) * bits(0x3c886600u)) * scale;
            }
        }
        if (real(o, 28) < bits(0x3a81a000u)) {
            unsigned char* const creature = from_word(word(player + found[i] * 23984, 21872));
            half(creature, 154) = 50;
            half(creature, 148) = 53;
            half(creature, 140) = 53;
            half(creature, 150) = 20;
            half(creature, 142) = 20;
            half(creature, 152) = 13;
            half(creature, 144) = 13;
            half(creature, 146) = 50;
        }
    }
    const int n0 = static_cast<signed char>(o[0]);
    unsigned char* const local = static_cast<unsigned char*>(netGetLocalObject__FP8Creature(from_word(word(o, 12))));
    P_OpenParticleSystem__Fi(static_cast<int>(word(o, 36)));
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    const Point3 origin = {real(local, 16) + zero, real(local, 20), real(local, 24) + 19.0f};
    alignas(16) unsigned char matrix[64];
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &origin, static_cast<short>(frameNm * 95));
    P_SetParticleSystemMatrix__FiP8Matrix34(static_cast<int>(word(o, 36)), matrix);
    const int total = n0 * 25 + 45;
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
        Point3 discard;
        do {
            discard.x = roll_component(o);
            discard.y = roll_component(o);
            discard.z = roll_component(o);
        } while (1.0f < discard.x * discard.x + discard.y * discard.y + discard.z * discard.z);
        Point3 velocity = {zero, zero, zero};
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d7770, &position, &velocity, zero);
    }
    P_CloseParticleSystem__Fi(static_cast<int>(word(o, 36)));
    real(o, 28) = real(o, 28) + bits(0x3c888888u);
    if (bits(0x3f802068u) < real(o, 28)) {
        unsigned char* const entry = from_word(word(o, 20)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
