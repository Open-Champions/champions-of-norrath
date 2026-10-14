// Observed msg_collision__8HolyBoltP10GameObject at 0x002bb4b0, 980 bytes. Arguments: the bolt O and the object T it touched (T is read
// without a null test). "flags" of an object is its word at +12. P is the point (three singles) at O+16. Pool constants with no symbols
// (bits): K = 0x3e4ccccc (about 0.2, two pool entries with the same bits), s = 0x42666666 (57.6). A "unit-ball vector" is drawn by
// rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in that order, each call separate), repeated
// while 1.0 < (x*x + y*y) + z*z. The particle definition is an 80-byte bss object of the image (file-local symbol skullHitParticleLong
// 0x008d8050), imported through an address-named view. Steps in order:
//  1. Nothing happens when T's flags lack 0x20, or when the word at O+84 (Q) is zero. Otherwise the word at O+84 becomes 0.
//  2. 188 bytes come from blockAlloc__Fi and, when non-null, the LightEffect constructor
//     __11LightEffectG6Point3ffffffffffffP10GameObjecti runs on them with (a copy of P, the twelve singles 172.0, 252.0, 500.0, -32.0,
//     0.0, 0.0, 0.0, 0.0, 180.0, 120.0, 1.0, 1.0, null object, 0).
//  3. Fifty times a unit-ball vector scaled by s is the velocity and P_AddParticle__FP11ParticleDefP6Point3T1f(definition, the address
//     O+16, velocity, 0.0) runs. Then lumpFindResource__FPcT0("fx.lmp", "spel_hit_mm.anm") is played with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, O, 1.0).
//  4. X is the object that is the word at O+184. Its table is the word at X+96; the entry at offset 88 is called on X with (a, the word
//     at O+188), a = 0 when the signed byte at T+172 equals 16 and 1 otherwise; it gives n. With R = eRand__10GameObject(O), the amount
//     is n + (int)(((float)R * 2^-16) * K * (float)n) (the multiplication order is exactly this).
//  5. gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, (the word at O+76 + 12 when that word is nonzero, else 0),
//     (float)n, (float)amount, 10000000, 0, 0, 0x0200c040, null description, false) runs.
//  6. When the single at T+560 is not above 0.0: 176 bytes come from blockAlloc__Fi and, when non-null, the LostSoul constructor
//     __8LostSoulG6Point3T1iP10_vagHeaderii runs on them with (a copy of P, a copy of the point at O+88, the signed halfword at O+56, null,
//     0, 1).
// Inferred: a holy bolt that, on hitting a living thing, bursts with light, particles and a sound, deals damage and, if the target is
// then dead (health at T+560), raises a lost soul at the spot. Unknown: what the words at O+76, +84, +184, +188, the byte at T+172 and
// the entry at 88 mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned char norrath_w16_spellmsg_a_view_8d8050[80];
int eRand__10GameObject(void* object);
void* blockAlloc__Fi(int size);
void light_effect_ctor(void* self, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11,
                       float f12, void* object, int mode) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void lost_soul_ctor(void* self, Point3 position, Point3 velocity, int angle, void* vag, int a, int b) __asm__("__8LostSoulG6Point3T1iP10_vagHeaderii");
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

extern "C" void msg_collision__8HolyBoltP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    if ((word(t, 12) & 0x20u) == 0u) return;
    if (word(o, 84) == 0u) return;
    word(o, 84) = 0;
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    void* const block = blockAlloc__Fi(188);
    if (block != nullptr) {
        const Point3 where = *reinterpret_cast<Point3*>(o + 16);
        light_effect_ctor(block, where, 172.0f, 252.0f, 500.0f, -32.0f, zero, zero, zero, zero, 180.0f, 120.0f, 1.0f, 1.0f, nullptr, 0);
    }
    for (int i = 0; i < 50; ++i) {
        Point3 u;
        unit_ball(o, u, roll);
        Point3 velocity = {u.x * bits(0x42666666u), u.y * bits(0x42666666u), u.z * bits(0x42666666u)};
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d8050, reinterpret_cast<Point3*>(o + 16), &velocity, zero);
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_hit_mm.anm")), o, 1.0f);
    unsigned char* const x = from_word(word(o, 184));
    unsigned char* const entry = from_word(word(x, 96)) + 88;
    const int a = static_cast<signed char>(t[172]) == 16 ? 0 : 1;
    const int n = reinterpret_cast<int (*)(void*, int, unsigned)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        x + *reinterpret_cast<short*>(entry), a, word(o, 188));
    const int amount = n + static_cast<int>(((static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u)) * bits(0x3e4ccccc)) * static_cast<float>(n));
    const unsigned q = word(o, 76);
    unsigned char* const owner = q != 0u ? from_word(word(from_word(q), 12)) : nullptr;
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(t, owner, static_cast<float>(n), static_cast<float>(amount), 10000000, 0, 0, 0x0200c040, nullptr, false);
    if (real(t, 560) <= zero) {
        void* const soul = blockAlloc__Fi(176);
        if (soul != nullptr) {
            const Point3 position = *reinterpret_cast<Point3*>(o + 16);
            const Point3 velocity = *reinterpret_cast<Point3*>(o + 88);
            lost_soul_ctor(soul, position, velocity, half(o, 56), nullptr, 0, 1);
        }
    }
}
