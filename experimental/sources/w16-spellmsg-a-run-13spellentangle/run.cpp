// Observed run__13SpellEntangleG6Point3s at 0x002c6630, 1612 bytes, 400-byte frame. O is the object, the first argument. The
// second argument (a point, copied to the callee's frame and never used) and the third are not read. Constants named by their
// float bits (no symbol names them): k = 0x3c888888 (about 1/60), s = 0x3f9eb851 (about 1.24), h = 0x3f028f5c (about 0.51). The
// single at O+24 gains k and is stored; the word at O+28 is the state S and C the object that is the word at O+12.
//  S == 0: a matrix is built by matMakeTransYaw__FP8Matrix34G6Point3s from a copy of the point at C+16 and the signed halfword at
//    C+56; P_OpenParticleSystem__Fi(the word at O+36); P_SetParticleSystemMatrix__FiP8Matrix34(the word at O+36, matrix). Then 7
//    times: two rolls R1 and R2 from eRand__5Spell(O) give the point ((R1 * 2^-16 + -0.5) * C.f28 * s, (R2 * 2^-16 + -0.5) *
//    C.f28 * s, C.f32 + 12.0), where C.f28 is the single at C+28 and C.f32 the single at C+32 (C is read again after each roll), and
//    P_AddParticle__FP11ParticleDefP6Point3T1f is called with the particle definition record at 0x008d83d0 (80 bytes, a local of
//    the image), that point, the point (0, 0, -72) and 0.0. Then P_CloseParticleSystem__Fi(the word at O+36). When h is below the
//    single at O+24: syncRandState is advanced (state * 0x19660d + 0x3c6ef35f), W = (state >> 16) % 100 (unsigned), and the entry
//    at offset 88 of the table whose address is the word at (the word at O+8) + 96 is called on that object (T) with (2, the
//    signed byte at O) giving V. If W + 1 < V (signed): C's halfword +794 becomes (short)(60 * the word at O+32), +140 = 4, +142 = 92,
//    +154 = 81, +148 = 4, +150 = 92, +152 = 0, +144 = 0, +146 = 81 and the word at O+28 is incremented. Otherwise, 100 times: a
//    point is drawn in the unit ball (three rolls, each 2 * (roll * 2^-16) - 1, repeated while 1.0 < x*x + y*y + z*z) and
//    P_AddParticle is called with the record at 0x008d8420 (local), the point (C.x, C.y, C.z + C.f32) (C+16, +20, +24 and +32, read
//    once before the loop), the ball point times 21.0 and 0.0; after the 100 particles the entry at offset 8 of the table whose
//    address is the word at O+20 is called on O (adjusted by its signed halfword) with the argument 3 and the function returns.
//  S == 1: the entry at offset 128 of the table whose address is the word at C+72 is called on C (adjusted by its halfword) and its
//    result passed to modelGetNmShadowPoints__FP12_modelHeader gives n. Then 22 times: C is read again, that entry is called again
//    (model), R = eRand__5Spell(O), and modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f is called
//    with (model, C+288, a copy of the point at C+16, the signed halfword at C+56, R % n (signed), &p, &extra, null matrix, 1.0); a
//    unit-ball point is then drawn (three rolls) and discarded, and P_AddParticle is called with the record at 0x008d8380 (local), p,
//    the point (0, 0, 0) and 0.0.
//  Any other S does nothing in the switch. In every case that reaches the end (not the S == 0 particle-burst return): when the
//    word at O+32 converted to a single is below (the single at O+24, reloaded) - h, and O is not null, the entry at offset 8 of the
//    table whose address is the word at O+20 is called on O with the argument 3.
// Inferred: a spell that, in three phases, sprays a fan of particles at the target, shows a number of rings on the target's shadow
// points and, with a chance depending on the skill, ends in a burst. Unknown: what the state values and the table entries mean.
// Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned syncRandState;
extern unsigned char norrath_w16_spellmsg_a_view_8d83d0[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d8420[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d8380[80];
int eRand__5Spell(void* spell);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* animation, const Point3* position, short yaw, int index,
                                                                                         Point3* out_point, float* out_extra, void* matrix33, float scale);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const Point3* point, short yaw);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, void* matrix);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
inline float roll_component(void* spell) {
    float v = static_cast<float>(eRand__5Spell(spell)) * w12c::bits(0x37800000u);
    return v + v - 1.0f;
}
void end_call(unsigned char* o) {
    using namespace w12c;
    unsigned char* const entry = from_word(word(o, 20)) + 8;
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
}
void* model_of(unsigned char* c) {
    using namespace w12c;
    unsigned char* const entry = from_word(word(c, 72)) + 128;
    return reinterpret_cast<void* (*)(void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(c + *reinterpret_cast<short*>(entry));
}
}

extern "C" void run__13SpellEntangleG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    real(o, 24) = real(o, 24) + bits(0x3c888888u);
    const unsigned state = word(o, 28);
    if (state == 0u) {
        unsigned char* c = from_word(word(o, 12));
        alignas(16) unsigned char matrix[64];
        const Point3 origin = {real(c, 16), real(c, 20), real(c, 24)};
        matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &origin, half(c, 56));
        P_OpenParticleSystem__Fi(static_cast<int>(word(o, 36)));
        P_SetParticleSystemMatrix__FiP8Matrix34(static_cast<int>(word(o, 36)), matrix);
        float zero = 0.0f;
        __asm__("" : "+f"(zero));
        for (int i = 0; i < 7; ++i) {
            Point3 position;
            Point3 velocity;
            position.x = (static_cast<float>(eRand__5Spell(o)) * bits(0x37800000u) + bits(0xbf000000u)) * real(from_word(word(o, 12)), 28) * bits(0x3f9eb851u);
            c = from_word(word(o, 12));
            position.y = (static_cast<float>(eRand__5Spell(o)) * bits(0x37800000u) + bits(0xbf000000u)) * real(c, 28) * bits(0x3f9eb851u);
            position.z = real(c, 32) + 12.0f;
            velocity.x = zero;
            velocity.y = zero;
            velocity.z = -72.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d83d0, &position, &velocity, zero);
        }
        P_CloseParticleSystem__Fi(static_cast<int>(word(o, 36)));
        if (bits(0x3f028f5cu) < real(o, 24)) {
            syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
            const unsigned w = (syncRandState >> 16) % 100u;
            unsigned char* const t = from_word(word(o, 8));
            unsigned char* const entry = from_word(word(t, 96)) + 88;
            const int v = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
                t + *reinterpret_cast<short*>(entry), 2, static_cast<signed char>(o[0]));
            if (static_cast<int>(w + 1u) < v) {
                c = from_word(word(o, 12));
                const int stamp = sword(o, 32);
                half(c, 794) = static_cast<short>(stamp * 60);
                half(c, 140) = 4;
                half(c, 142) = 92;
                half(c, 154) = 81;
                word(o, 28) = word(o, 28) + 1u;
                half(c, 148) = 4;
                half(c, 150) = 92;
                half(c, 152) = 0;
                half(c, 144) = 0;
                half(c, 146) = 81;
            } else {
                c = from_word(word(o, 12));
                Point3 position = {real(c, 16), real(c, 20), real(c, 24) + real(c, 32)};
                for (int i = 0; i < 100; ++i) {
                    Point3 u;
                    do {
                        u.x = roll_component(o);
                        u.y = roll_component(o);
                        u.z = roll_component(o);
                    } while (1.0f < u.x * u.x + u.y * u.y + u.z * u.z);
                    Point3 velocity = {u.x * 21.0f, u.y * 21.0f, u.z * 21.0f};
                    P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d8420, &position, &velocity, zero);
                }
                end_call(o);
                return;
            }
        }
    } else if (state == 1u) {
        unsigned char* c = from_word(word(o, 12));
        const int n = modelGetNmShadowPoints__FP12_modelHeader(model_of(c));
        float zero = 0.0f;
        __asm__("" : "+f"(zero));
        for (int i = 0; i < 22; ++i) {
            c = from_word(word(o, 12));
            void* const model = model_of(c);
            const int roll = eRand__5Spell(o);
            const Point3 origin = {real(c, 16), real(c, 20), real(c, 24)};
            Point3 point;
            float extra;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, c + 288, &origin, half(c, 56), roll % n, &point, &extra, nullptr, 1.0f);
            Point3 u;
            do {
                u.x = roll_component(o);
                u.y = roll_component(o);
                u.z = roll_component(o);
            } while (1.0f < u.x * u.x + u.y * u.y + u.z * u.z);
            Point3 velocity = {zero, zero, zero};
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d8380, &point, &velocity, zero);
        }
    }
    const float limit = static_cast<float>(sword(o, 32));
    __asm__("" : "+r"(o));
    if (limit < real(o, 24) - bits(0x3f028f5cu)) {
        if (o != nullptr) end_call(o);
    }
}
