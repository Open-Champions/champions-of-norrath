// Observed msg_run__10HolyStrike at 0x002b1658, 3780 bytes. O is the object, P the point (three singles) at O+16, frameNm the global frame counter.
// Pool constants with no symbols (bits): p1 = 0x40efffdc (about 7.5), v2 = 0x411fff59 (about 10), v3 = 0x4147ff03 (about 12.5), p4 = 0x411fffca
// (about 10), v4 = 0x419fffcf (about 20), p5 = 0x40efffdc (about 7.5, a second entry with the same bits), p6 = 0x411fff59 (about 10), v6 =
// 0x4295ff42 (about 75), p7 = 0x411fffca (about 10), v7 = 0x42efffb6 (about 120). A "unit-ball vector" is drawn by rejection: each component is
// eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in that order, each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The three
// particle definitions are 80-byte bss objects of the image (file-local symbols holystrikeParticle1 0x008d7950, holystrikeParticle2 0x008d79a0 and
// holystrikeParticle3 0x008d79f0), imported through address-named views. A burst "(D, S, W)" means: a unit-ball vector scaled by S plus P is the
// position; a second unit-ball vector scaled by W is the velocity; P_AddParticle__FP11ParticleDefP6Point3T1f(D, position, velocity, 0.0) runs. Steps:
//  1. Bursts: 5 times (definition 1, p1, 16.0), then 10 times (definition 2, v2 as scale 9.9998 and v3 as velocity scale), then 9 times (definition 3,
//     p4, v4). (The scale constants are the pool entries in the order listed above.)
//  2. P_ExtendTrail__FiG6Point3(the word at O+124, a copy of P) runs. (The image also computes a unit direction from O+108 and O+112 and a position
//     from it that it never uses.)
//  3. When the word at O+88 (the target T) is nonzero and the signed word at O+100 is below frameNm: with X the object that is the word at O+104,
//     s = (entry at table offset 88 of the table at X+96, called on X adjusted by its signed halfword, with (2, the signed halfword at O+92)) * 182;
//     C is the object that is the word at T+12; a = iatan2__Fff(C.y - O.y, C.x - P.x) (C's singles at +16 and +20, O.y the single at O+20 and P.x the
//     single at O+16); d = a - (the signed halfword at O+56); with h the unsigned halfword at O+56 the halfword at O+56 becomes h + d when |d| < s,
//     h - s when d <= 0 otherwise, and h + s for the rest; then the single at O+108 becomes icos__Fi(the halfword) * 450.0, the word at O+116 becomes 0
//     and the single at O+112 becomes isin__Fi(the halfword) * 450.0.
//  4. r = move__10ProjectileG6Point3(O, a copy of the point at O+108). When r has bit 2 and the word at O+120 is nonzero the word at O+96 becomes 0;
//     when r has bit 1 it becomes 0. The function ends when frameNm is below the signed word at O+96.
//  5. Otherwise: lumpFindResource__FPcT0("fx.lmp", "spel_he_hstrike_hit.anm") is played with soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, O,
//     1.0); 188 bytes come from blockAlloc__Fi and, when non-null, the LightEffect constructor __11LightEffectG6Point3ffffffffffffP10GameObjecti runs on
//     them with (a copy of P, the twelve singles 255.0, 200.0, 135.0, -32.0, 0.0, 0.0, 0.0, 0.0, 240.0, 120.0, 0.5, 1.0, null object, 0). Then 25 times
//     three bursts: (definition 1, p5, 96.0), (definition 2, p6, v6), (definition 3, p7, v7). Finally, when O is not null, the entry at table offset 8
//     of the table whose address is the word at O+72 is called on O (adjusted by its signed halfword) with 3.
// Inferred: a homing holy strike that bursts into particles each frame and, when it stops, plays a sound, makes a light and a larger burst. Unknown:
// what the words at O+88..O+124 mean beyond their use here. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w16_spellmsg_a_view_8d7950[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d79a0[80];
extern unsigned char norrath_w16_spellmsg_a_view_8d79f0[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void P_ExtendTrail__FiG6Point3(int trail, Point3 p);
void* blockAlloc__Fi(int size);
void light_effect_ctor(void* self, Point3 point, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11,
                       float f12, void* object, int mode) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static void burst(unsigned char* o, void* definition, float positionScale, float velocityScale) {
    using namespace w12c;
    Point3 v;
    unit_ball(o, v, roll);
    Point3 x;
    x.x = v.x * positionScale;
    x.y = v.y * positionScale;
    x.z = v.z * positionScale;
    x.x = x.x + real(o, 16);
    x.y = x.y + real(o, 20);
    x.z = x.z + real(o, 24);
    Point3 y;
    unit_ball(o, y, roll);
    Point3 velocity;
    velocity.x = y.x * velocityScale;
    velocity.y = y.y * velocityScale;
    velocity.z = y.z * velocityScale;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &x, &velocity, 0.0f);
}

extern "C" void msg_run__10HolyStrike(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    for (int i = 0; i < 5; ++i) burst(o, norrath_w16_spellmsg_a_view_8d7950, bits(0x40efffdcu), 16.0f);
    for (int i = 0; i < 10; ++i) burst(o, norrath_w16_spellmsg_a_view_8d79a0, bits(0x411fff59u), bits(0x4147ff03u));
    for (int i = 0; i < 9; ++i) burst(o, norrath_w16_spellmsg_a_view_8d79f0, bits(0x411fffcau), bits(0x419fffcfu));
    {
        const Point3 at = *reinterpret_cast<Point3*>(o + 16);
        P_ExtendTrail__FiG6Point3(static_cast<int>(word(o, 124)), at);
    }
    unsigned char* const target = from_word(word(o, 88));
    if (target != nullptr) {
        if (sword(o, 100) < frameNm) {
            unsigned char* const x = from_word(word(o, 104));
            unsigned char* const entry = from_word(word(x, 96)) + 88;
            const int steer = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
                                  x + *reinterpret_cast<short*>(entry), 2, half(o, 92)) * 182;
            unsigned char* const c = from_word(word(from_word(word(o, 88)), 12));
            const float py = real(o, 20);
            const float px = real(o, 16);
            const int a = iatan2__Fff(real(c, 20) - py, real(c, 16) - px);
            const int d = a - half(o, 56);
            const int ad = d < 0 ? -d : d;
            const int h = *reinterpret_cast<unsigned short*>(o + 56);
            int next;
            if (ad < steer) next = h + d;
            else if (d <= 0) next = h - steer;
            else next = h + steer;
            half(o, 56) = static_cast<short>(next);
            real(o, 108) = icos__Fi(half(o, 56)) * 450.0f;
            word(o, 116) = 0;
            real(o, 112) = isin__Fi(half(o, 56)) * 450.0f;
        }
    }
    {
        const Point3 delta = *reinterpret_cast<Point3*>(o + 108);
        const int r = move__10ProjectileG6Point3(o, delta);
        if (r & 2) {
            if (word(o, 120) != 0u) word(o, 96) = 0;
        }
        if (r & 1) word(o, 96) = 0;
    }
    if (frameNm < sword(o, 96)) return;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_hstrike_hit.anm")), o, 1.0f);
    void* const block = blockAlloc__Fi(188);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    if (block != nullptr) {
        const Point3 where = *reinterpret_cast<Point3*>(o + 16);
        light_effect_ctor(block, where, 255.0f, 200.0f, 135.0f, -32.0f, zero, zero, zero, zero, 240.0f, 120.0f, 0.5f, 1.0f, nullptr, 0);
    }
    for (int i = 0; i < 25; ++i) {
        burst(o, norrath_w16_spellmsg_a_view_8d7950, bits(0x40efffdcu), 96.0f);
        burst(o, norrath_w16_spellmsg_a_view_8d79a0, bits(0x411fff59u), bits(0x4295ff42u));
        burst(o, norrath_w16_spellmsg_a_view_8d79f0, bits(0x411fffcau), bits(0x42efffb6u));
    }
    __asm__("" : "+r"(o));
    if (o != nullptr) {
        unsigned char* const entry = from_word(word(o, 72)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
