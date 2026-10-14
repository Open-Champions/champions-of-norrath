// Observed run__16SpellConeOfFrostG6Point3s at 0x002af508, 1616 bytes. Arguments: the spell S, a point ORG (three singles, by address, copied
// to a local) and a signed halfword angle A. frameNm is the global frame counter. T is the object that is the word at S+8 and N the signed byte at
// S+0; "slot(i)" is the entry at offset 88 of the table whose address is the word at T+96, called on T (adjusted by the entry's signed halfword)
// with (i, N) and returning an int. Pool constants with no symbols (bits): k1 = 0x43360b60 (about 182.04), k2 = 0x3c23d70a (0.01),
// k3 = 0x3f7ffe7e (0.99998), k4 = 0x3e4ccccc (0.2), k5 = 0x3f666666 (0.9), k6 = 0x3fdfded6 (about 1.749), k7 = 0x3e088888 (about 0.1333),
// plus the literals below. The three particle definitions are 80-byte bss objects of the image (file-local symbols primary 0x008d77c0, puff
// 0x008d7810 and ice 0x008d7860), imported through address-named views. A "roll" is (float)eRand__5Spell(S) * 2^-16. Steps in order:
//  1. w = (int)((float)slot(2) * k1); u = (float)slot(1) * k2 + 1.0; W = (float)w.
//  2. Fifteen times (the particle burst "primary"): R1 gives the angle a = (short)(A + (int)((roll1 - 0.5) * W)); R2 gives the radius
//     r = (roll2 * 74.0) * u; the velocity is (icos__Fi(a) * r, isin__Fi(a) * r, ((roll3 - 0.5) * 6.0) * u); a fourth roll is drawn and
//     only feeds values the image never uses; P_AddParticle__FP11ParticleDefP6Point3T1f(primary, the address of the local ORG copy, velocity, 0.0).
//  3. W2 = (float)(int)(W * k3). Six times ("puff"): a = (short)(A + (int)((roll1 - 0.5) * W2)); r = ((roll2 * k4 + k5) * 60.0) * u; velocity
//     (icos(a) * r, isin(a) * r, ((roll3 - 0.5) * 4.0) * u); P_AddParticle(puff, ORG, velocity, 0.0).
//  4. W3 = (float)(int)(W * k6). Only when eRand__5Spell(S) % 5 == 0 (unsigned): 24 times ("ice"): a = (short)(A + (int)((roll1 - 0.5) * W3));
//     a second roll is drawn and not used; velocity (icos(a) * 42.0, isin(a) * 42.0, ((roll3 - 0.5) * 17.0) * u); P_AddParticle(ice, ORG, velocity, 0.0).
//  5. Only when frameNm & 7 is zero: a = (short)(A + (int)((roll - 0.5) * W)); velocity ((icos(a) * 74.0) * 2 * u, (isin(a) * 74.0) * 2 * u, 0.0);
//     m = slot(0); 116 bytes come from blockAlloc__Fi and, when non-null, the DummyDamager constructor
//     __12DummyDamagerG6Point3T1iiifiP8Creatures runs on them with (ORG, velocity, 45, 10, 18, (float)m * k7, 0x0080c040, the word at S+12, 1).
//  6. The light embedded at S+24 gets its singles at +28 and +36 (offsets from S+24) set to q = (float)((int)(isin__Fi(frameNm * 1000) * 6.0)
//     + 108) and those at +32 and +40 set to 1.0 / (q * q); lightUpdatePosition__FP5Light(S+24) is called last.
// Inferred: a cone of frost in front of a caster: a spray of cold particles, an occasional heavier burst, and a damager object every eighth frame.
// Unknown: what the byte at S+0, the slots 0..2, and the constants mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w16_spellmsg_a_view_8d77c0[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d7810[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d7860[80];
int eRand__5Spell(void* spell);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void dummy_damager_ctor(void* self, Point3 position, Point3 velocity, int a, int b, int c, float f, int flags, void* creature, short extra)
    __asm__("__12DummyDamagerG6Point3T1iiifiP8Creatures");
void lightUpdatePosition__FP5Light(void* light);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
inline float roll(void* s) { return static_cast<float>(eRand__5Spell(s)) * w12c::bits(0x37800000u); }
int slot(unsigned char* t, int index, int n) {
    using namespace w12c;
    unsigned char* const entry = from_word(word(t, 96)) + 88;
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        t + *reinterpret_cast<short*>(entry), index, n);
}
}

extern "C" void run__16SpellConeOfFrostG6Point3s(void* self, Point3* originPointer, short angle) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    Point3 origin = *originPointer;
    unsigned char* const t = from_word(word(s, 8));
    const int n = static_cast<signed char>(s[0]);
    const int width = static_cast<int>(static_cast<float>(slot(t, 2, n)) * bits(0x43360b60u));
    const float u = static_cast<float>(slot(t, 1, n)) * bits(0x3c23d70au) + 1.0f;
    const float w = static_cast<float>(width);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    Point3 velocity;
    for (int i = 0; i < 15; ++i) {
        const int a = static_cast<short>(angle + static_cast<int>((roll(s) - 0.5f) * w));
        const float r = (roll(s) * 74.0f) * u;
        velocity.x = icos__Fi(a) * r;
        velocity.y = isin__Fi(a) * r;
        velocity.z = ((roll(s) - 0.5f) * 6.0f) * u;
        (void)eRand__5Spell(s);
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d77c0, &origin, &velocity, zero);
    }
    const int width2 = static_cast<int>(w * bits(0x3f7ffe7eu));
    const float w2 = static_cast<float>(width2);
    for (int i = 0; i < 6; ++i) {
        const int a = static_cast<short>(angle + static_cast<int>((roll(s) - 0.5f) * w2));
        const float r = ((roll(s) * bits(0x3e4ccccc) + bits(0x3f666666u)) * 60.0f) * u;
        velocity.x = icos__Fi(a) * r;
        velocity.y = isin__Fi(a) * r;
        velocity.z = ((roll(s) - 0.5f) * 4.0f) * u;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d7810, &origin, &velocity, zero);
    }
    const int width3 = static_cast<int>(w * bits(0x3fdfded6u));
    if (static_cast<unsigned>(eRand__5Spell(s)) % 5u == 0u) {
        const float w3 = static_cast<float>(width3);
        for (int i = 0; i < 24; ++i) {
            const int a = static_cast<short>(angle + static_cast<int>((roll(s) - 0.5f) * w3));
            (void)eRand__5Spell(s);
            velocity.x = icos__Fi(a) * 42.0f;
            velocity.y = isin__Fi(a) * 42.0f;
            velocity.z = ((roll(s) - 0.5f) * 17.0f) * u;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d7860, &origin, &velocity, zero);
        }
    }
    if ((frameNm & 7) == 0) {
        const int a = static_cast<short>(angle + static_cast<int>((roll(s) - 0.5f) * w));
        float x = icos__Fi(a) * 74.0f;
        x = x + x;
        velocity.x = x * u;
        float y = isin__Fi(a) * 74.0f;
        velocity.z = 0.0f;
        const int m = slot(t, 0, n);
        y = y + y;
        velocity.y = y * u;
        const float damage = static_cast<float>(m) * bits(0x3e088888u);
        void* const block = blockAlloc__Fi(116);
        if (block != nullptr) dummy_damager_ctor(block, origin, velocity, 45, 10, 18, damage, 0x0080c040, from_word(word(s, 12)), 1);
    }
    unsigned char* const light = s + 24;
    const float q = static_cast<float>(static_cast<int>(isin__Fi(frameNm * 1000) * 6.0f) + 108);
    real(light, 28) = q;
    real(light, 36) = q;
    const float inverse = 1.0f / (q * q);
    real(light, 40) = inverse;
    real(light, 32) = inverse;
    lightUpdatePosition__FP5Light(light);
}
