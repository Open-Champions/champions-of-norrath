// Observed msg_run__15ShockProjectile at 0x0028c910, 3004 bytes. O is the object, P the point (three floats) at O+16, frameNm the global
// frame counter. "flags" of an object is its word at +12. State is the signed byte at O+393. Pool constants with no symbols: K1 = bits
// 0x472de900 (44521.0) and K2 = bits 0x3e4ccccc (0.2). Two particle definitions are 80-byte bss objects of the image (file-local symbols
// ballParticle 0x008d6320 and ballSmokeParticle 0x008d6370) and are imported through address-named views. A "unit-ball vector" is drawn by
// rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0, x then y then z, repeated while 1.0 < (x*x + y*y) + z*z.
// This object has the same layout idea as the hate projectile of this package, shifted: the caster's list node is at O+404, the light at
// O+332, the chain head at O+84, the link head at O+80, the lightning handles at O+300+4*k.
//  A. Always first: t = (the signed word at O+88) - frameNm + 300. soundChangeSequenceVolume__Fif(the word at O+92, 1.0); P.x += the single
//     at O+288 and P.y += the single at O+292; objectUpdateInGrid__FP10GameObject(O) runs.
//  B. State 0 (flying):
//     1. When the single at O+400 is not 0.0: ground = worldFindHeight(world, copy of P, 6.0). When P.z < ground: the singles at O+288, +292
//        and +400 become 0.0, State becomes 2, the word at O+88 becomes frameNm - 270, soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
//        lumpFindResource__FPcT0("fx.lmp", "spel_ew_shock_hit.anm"), O, 1.0) plays, and step 2 and 3 are skipped (step 4 still runs).
//     2. When (frameNm & 7) == 0: the box from P - 18.0 to P + 18.0 is searched with objectFindInBox__FG6Point3T0PP10GameObjecti(low, high,
//        list, 512). For each entry E in order, with C the word at O+404 (the caster's list node): when C is non-zero the entry at table
//        offset 160 of the object that is the word at C+12 is called on it with E, and a zero result skips E (there is no other filter);
//        when C is zero E is skipped when its flags lack 0x20, have any of 0x0d, or when E == O. A surviving E is the target when the plane
//        distance sqrt((P.x-E.x)^2 + (P.y-E.y)^2) is below E's single at +28 plus 18.0.
//     3. With a target T: the box from T's point -/+ 1200.0 is searched with the same call. With C non-zero each entry E for which the
//        table-160 call on the object at C+12 with E returns non-zero is linked with listOnewayLink__FP8ListHeadT0Pv(O+80, E, E); with C
//        zero each entry with flags bit 0x20, none of 0x0d, and neither O nor T is linked the same way. Then T is linked. The byte at O+395
//        becomes 0 and the single at O+396 becomes K1. buildShockChainList__15ShockProjectileP6Point3(O, the point at T+16) runs. An item
//        description with no entries set (see spell.hpp) is built; n is the entry at offset 112 of the table that is the word at +96 of the
//        object that is the word at O+76, called on that object with (0, the signed byte at O+392, the address O+404); m = n + (int)((float)n
//        * K2 * ((float)(r >> 16) * 2^-16)) where r is syncRandState after one step (state * 0x19660d + 0x3c6ef35f); the first description
//        entry that is free or has kind 24 gets (24, n, m, 0). For each node N of the chain from the word at O+84 (a node's next is the word
//        at N+0) whose word at +12 is non-zero: gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(the object at N+12, null,
//        0.0, 0.0, 100000, 0, 0, 64, description, false) runs. State becomes 1, the singles at O+288, O+292 and O+400 become 0.0, the word at
//        O+88 becomes frameNm - 270 and, for each index i below the signed byte at O+395 (read after the loop), the word at O+300+4*i
//        becomes -1.
//     4. A light embedded at O+332 is updated: s = 180 - 6*t when (float)t <= 30.0, else 0; the point P is copied to O+380; q = (int)(isin(
//        frameNm * 6000) * 10.0) - s + 96; the singles at O+360 and O+368 become (float)q and those at O+364 and O+372 become 1.0 / ((float)q *
//        (float)q); lightUpdatePosition__FP5Light(O+332) runs.
//  C. State 1: for each pair of consecutive nodes (cur, next) of the chain at O+84 (k counts pairs from 0) whose objects (the words at +12) are
//     both non-zero: A = point of cur's object + (0, 0, 36.0), B = the same for next's object; with h the word at O+300+4*k: when h == -1,
//     h = P_StartLightning__FG6Point3T0iiffUi(A, B, 260, 4, 16.0, 8.5, 0x5e808080) is stored there; otherwise
//     P_MoveLightning__FiG6Point3T1ii(h, A, B, 128, -1) runs.
//  D. Always last (every state): 42 times, a unit-ball vector is drawn, normalised with rsqrt.s and scaled by 17.0 (V); X = P + V; a second
//     unit-ball vector Y is drawn (not scaled); P_AddParticle__FP11ParticleDefP6Point3T1f(ball definition, X, Y, 0.0) runs, then
//     eRand__10GameObject(O) is drawn and, when its low five bits are zero, P_AddParticle(ball smoke definition, X, Y, 0.0) runs too. When
//     t <= 0 the entry at table offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a shock bolt that explodes into a particle ball every frame, finds creatures on contact, chains lightning between them and
// damages each through the attack resolver. Unknown: the meaning of the words and bytes at O+76..+92, +392..+404 and of the constants.
// Nothing here ran on the EE.
#include "../w14-projectile-b-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned syncRandState;
extern unsigned char norrath_w13_spell_msg_a_view_8d6320[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d6370[80];
int eRand__10GameObject(void* object);
float isin__Fi(int angle);
void soundChangeSequenceVolume__Fif(int handle, float volume);
void objectUpdateInGrid__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 p, float radius);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void buildShockChainList__15ShockProjectileP6Point3(void* self, Point3* point);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void lightUpdatePosition__FP5Light(void* light);
int P_StartLightning__FG6Point3T0iiffUi(Point3 a, Point3 b, int c, int d, float e, float f, unsigned g);
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3 a, Point3 b, int c, int d);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

extern "C" void msg_run__15ShockProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* hit = nullptr;
    const int t = sword(o, 88) - frameNm + 300;
    const float tf = static_cast<float>(t);
    soundChangeSequenceVolume__Fif(sword(o, 92), 1.0f);
    real(o, 16) = real(o, 16) + real(o, 288);
    real(o, 20) = real(o, 20) + real(o, 292);
    objectUpdateInGrid__FP10GameObject(o);
    if (*reinterpret_cast<signed char*>(o + 393) == 0) {
        bool ground = false;
        if (real(o, 400) != 0.0f) {
            Point3 probe = *reinterpret_cast<Point3*>(o + 16);
            const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 6.0f);
            if (real(o, 24) < h) {
                ground = true;
                real(o, 292) = 0.0f;
                *reinterpret_cast<signed char*>(o + 393) = 2;
                real(o, 400) = 0.0f;
                word(o, 88) = static_cast<unsigned>(frameNm - 270);
                real(o, 288) = 0.0f;
                void* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shock_hit.anm"));
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, o, 1.0f);
            }
        }
        if (!ground && (frameNm & 7) == 0) {
            const Point3 p = *reinterpret_cast<Point3*>(o + 16);
            Point3 low;
            Point3 high;
            high.x = p.x + 18.0f;
            high.y = p.y + 18.0f;
            high.z = p.z + 18.0f;
            low.x = p.x - 18.0f;
            low.y = p.y - 18.0f;
            low.z = p.z - 18.0f;
            void* list[512];
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 512);
            for (int i = 0; list[i] != nullptr; ++i) {
                unsigned char* const e = static_cast<unsigned char*>(list[i]);
                if (word(o, 404) != 0) {
                    if (call_entry(from_word(word(from_word(word(o, 404)), 12)), 160, to_word(e)) == 0) continue;
                } else {
                    const unsigned flags = word(e, 12);
                    if (!(flags & 0x20u)) continue;
                    if (flags & 0xdu) continue;
                    if (e == o) continue;
                }
                const float dx = real(o, 16) - real(e, 16);
                const float dy = real(o, 20) - real(e, 20);
                const float distance = __builtin_sqrtf(dy * dy + dx * dx);
                if (distance < real(e, 28) + 18.0f) {
                    hit = e;
                    break;
                }
            }
        }
        if (hit != nullptr) {
            Point3 center = *reinterpret_cast<Point3*>(hit + 16);
            Point3 high;
            high.x = center.x + bits(0x44960000u);
            high.y = center.y + bits(0x44960000u);
            high.z = center.z + bits(0x44960000u);
            Point3 low;
            low.x = center.x - bits(0x44960000u);
            low.y = center.y - bits(0x44960000u);
            low.z = center.z - bits(0x44960000u);
            void* list[512];
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 512);
            unsigned char* const chain = o + 80;
            for (int i = 0; list[i] != nullptr; ++i) {
                unsigned char* const e = static_cast<unsigned char*>(list[i]);
                if (word(o, 404) != 0) {
                    if (call_entry(from_word(word(from_word(word(o, 404)), 12)), 160, to_word(e)) != 0) listOnewayLink__FP8ListHeadT0Pv(chain, e, e);
                } else {
                    const unsigned flags = word(e, 12);
                    if (!(flags & 0x20u)) continue;
                    if (flags & 0xdu) continue;
                    if (e == o) continue;
                    if (e == hit) continue;
                    listOnewayLink__FP8ListHeadT0Pv(chain, e, e);
                }
            }
            listOnewayLink__FP8ListHeadT0Pv(chain, hit, hit);
            *reinterpret_cast<signed char*>(o + 395) = 0;
            real(o, 396) = bits(0x472de900u);
            buildShockChainList__15ShockProjectileP6Point3(o, reinterpret_cast<Point3*>(hit + 16));
            unsigned char description[80];
            w13a::init_description(description);
            const int n = w13a::call_entry_112(from_word(word(o, 76)), *reinterpret_cast<signed char*>(o + 392), o + 404);
            float spread = static_cast<float>(n) * bits(0x3e4ccccc);
            syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
            spread = spread * (static_cast<float>(syncRandState >> 16) * bits(0x37800000u));
            const int m = n + static_cast<int>(spread);
            w13a::fill_description(description, 24, static_cast<short>(n), static_cast<short>(m), 0);
            for (unsigned char* node = from_word(word(o, 84)); node != nullptr; node = from_word(word(node, 0))) {
                if (word(node, 12) == 0) continue;
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(from_word(word(node, 12)), nullptr, 0.0f, 0.0f, 100000, 0, 0, 64,
                                                                                  description, false);
            }
            const signed char count = *reinterpret_cast<signed char*>(o + 395);
            *reinterpret_cast<signed char*>(o + 393) = 1;
            real(o, 400) = 0.0f;
            word(o, 88) = static_cast<unsigned>(frameNm - 270);
            real(o, 288) = 0.0f;
            real(o, 292) = 0.0f;
            if (count > 0) {
                int i = 0;
                do {
                    word(o, 300 + 4 * i) = 0xffffffffu;
                    ++i;
                } while (i < *reinterpret_cast<signed char*>(o + 395));
            }
        }
        int s = 0;
        if (tf <= 30.0f) s = 180 - 6 * t;
        unsigned char* const light = o + 332;
        *reinterpret_cast<Point3*>(o + 380) = *reinterpret_cast<Point3*>(o + 16);
        const float swing = isin__Fi(frameNm * 6000) * 10.0f;
        const float q = static_cast<float>(static_cast<int>(swing) - s + 96);
        const float square = q * q;
        real(light, 28) = q;
        real(light, 36) = q;
        const float inverse = 1.0f / square;
        real(light, 40) = inverse;
        real(light, 32) = inverse;
        lightUpdatePosition__FP5Light(light);
    }
    if (*reinterpret_cast<signed char*>(o + 393) == 1 && word(o, 84) != 0 && word(from_word(word(o, 84)), 0) != 0) {
        unsigned char* cur = from_word(word(o, 84));
        unsigned char* next = from_word(word(cur, 0));
        for (int k = 0; next != nullptr; ++k) {
            if (word(cur, 12) != 0 && word(next, 12) != 0) {
                Point3 a = *reinterpret_cast<Point3*>(from_word(word(cur, 12)) + 16);
                a.z = a.z + 36.0f;
                Point3 b = *reinterpret_cast<Point3*>(from_word(word(next, 12)) + 16);
                b.z = b.z + 36.0f;
                const int handle = sword(o, 300 + 4 * k);
                if (handle == -1) {
                    word(o, 300 + 4 * k) = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(a, b, 260, 4, 16.0f, 8.5f, 0x5e808080u));
                } else {
                    P_MoveLightning__FiG6Point3T1ii(handle, a, b, 128, -1);
                }
            }
            cur = next;
            next = from_word(word(cur, 0));
        }
    }
    for (int j = 0; j < 42; ++j) {
        Point3 u;
        unit_ball(o, u, roll);
        Point3 v = u;
        const float inverse = rsqrt_s(1.0f, (v.x * v.x + v.y * v.y) + v.z * v.z);
        v.y = v.y * inverse;
        v.z = v.z * inverse;
        v.x = v.x * inverse;
        v.x = v.x * 17.0f;
        v.y = v.y * 17.0f;
        v.z = v.z * 17.0f;
        Point3 x;
        x.x = real(o, 16) + v.x;
        x.y = real(o, 20) + v.y;
        x.z = real(o, 24) + v.z;
        Point3 y;
        unit_ball(o, y, roll);
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_8d6320, &x, &y, 0.0f);
        if ((eRand__10GameObject(o) & 0x1f) == 0) P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_8d6370, &x, &y, 0.0f);
    }
    if (t <= 0) call_entry(o, 8, 3);
}
