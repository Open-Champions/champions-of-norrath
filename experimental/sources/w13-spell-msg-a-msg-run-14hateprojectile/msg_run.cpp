// Observed msg_run__14HateProjectile at 0x001fd9a8, 3268 bytes. O is the object, P the point (three floats) at O+16, frameNm the global
// frame counter. "flags" of an object is its word at +12. State is the signed byte at O+385. Pool constants with no symbols:
// K1 = bits 0x3c888888 (1/60) and K2 = bits 0x3e4ccccc (0.2). Three particle definitions are 80-byte bss objects of the image (file-local
// symbols hateballParticle 0x007d6fc0 and hateballSmokeParticle 0x007d7010) and are imported through address-named views. A
// "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0, x then y then z, repeated
// while 1.0 < (x*x + y*y) + z*z.
//  A. Always first: t = (the signed word at O+88) - frameNm + 180. soundChangeSequenceVolume__Fif(the word at O+96, 1.0); h = the
//     signed halfword at O+56, w = the word at O+92 as an integer: P.x += icos__Fi(h) * (float)w * K1; P.y += isin__Fi(h) * (float)w *
//     K1; when w is non-zero, P.z -= 1.0. objectUpdateInGrid__FP10GameObject(O) runs.
//  B. State 0 (flying):
//     1. When w is non-zero: ground = worldFindHeight(world, copy of P, 12.0). When P.z < ground: w becomes 0, State becomes 2, the word
//        at O+88 becomes frameNm - 150, soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp",
//        "spel_ew_shock_hit.anm"), O, 1.0) plays, and step 2 is skipped.
//     2. When (frameNm & 7) == 0: the box from P - 20 to P + 20 is searched with objectFindInBox__FG6Point3T0PP10GameObjecti(low, high,
//        list, 512). For each entry E in order, with C the word at O+392 (the caster's list node): when C is non-zero: E == O skips it;
//        flags bit 0x01 skips it; flags without bit 0x20 is a wall hit (same stores and sound as in step 1, then the scan ends with no
//        target); otherwise the entry at table offset 160 of the object that is the word at C+12 is called on it with E and a zero result
//        skips E. When C is zero: E == O skips; bit 0x01 skips; no bit 0x20 is a wall hit; any of bits 0x0d skips. A surviving E is the
//        target when the plane distance sqrt((P.x-E.x)^2 + (P.y-E.y)^2) is below E's single at +28 plus 20.0.
//     3. With a target T: the box from T's point -/+ 600.0 is searched with the same call. With C non-zero, each entry E (no other filter)
//        for which the table-160 call on the object at C+12 with E returns non-zero is linked with listOnewayLink__FP8ListHeadT0Pv(O+80,
//        E, E); with C zero, each entry that has flags bit 0x20, none of 0x0d, and is neither O nor T is linked the same way. Then T is
//        linked. The byte at O+387 becomes 0 and the single at O+388 becomes 3600.0. buildShockChainList__14HateProjectileP6Point3(O, the
//        point at T+16) runs. The attack flags are 0x80040, or 0x40040 when (float)eRand__10GameObject(O) * 2^-16 is not below 0.5.
//        The amount n is the entry at offset 112 of the table that is the word at +96 of the object that is the word at O+76, called on that
//        object with (0, the signed byte at O+384, the address O+392). Then n + (int)((float)n * K2 * ((float)eRand__10GameObject(O) *
//        2^-16)) is m. For each node N of the chain that starts at the word at O+84 (a node's next is the word at N+0) whose word at +12 is
//        non-zero: when the word at O+396 is non-zero, soundPlaySequence(that word, O, 1.0) plays; gameAttackResolver__FP10GameObjectT0ff
//        iiiiPQ24item11descriptionb(the object at N+12, null, (float)n, (float)m, 100000, 0, 0, attack flags, an item description with no
//        entries set (see spell.hpp), false) runs; checkIfSpellIsRunning__8CreatureRC9type_info(the object, __tf15SpellHateEffect()) is
//        asked and, when it returns zero, 48 bytes are allocated and a non-null block gets the SpellHateEffect constructor __15SpellHate
//        EffectiiP8CreatureP10GameObjectP5Skill(block, 3, (int)worldDifficulty, the object, null, the word at skills+120); then
//        addSpell__8CreatureP5Spell(the object, block) runs (also with a null block). State becomes 1, w becomes 0, O+88 becomes frameNm -
//        150 and, for each index i below the signed byte at O+387, the word at O+292+4*i becomes -1.
//     4. A light embedded at O+324 is updated: s = 180 - 6*t when (float)t <= 30.0, else 0; the point P is copied to O+372; q = (int)(isin
//        (frameNm * 6000) * 6.0) - s + 120; the singles at O+352 and O+360 become (float)q and those at O+356 and O+364 become 1.0 /
//        ((float)q * (float)q); lightUpdatePosition__FP5Light(O+324) runs.
//  C. State 1: for each pair of consecutive nodes (cur, next) of the chain at O+84 (with k counting pairs from 0) whose objects (the words at
//     +12) are both non-zero: A = point of cur's object + (0, 0, 36.0), B = the same for next's object; with h the word at O+292+4*k: when h == -1,
//     h = P_StartLightning__FG6Point3T0iiffUi(A, B, 260, 4, 16.0, 8.5, 0xff000fdc) is stored at O+292+4*k; otherwise
//     P_MoveLightning__FiG6Point3T1ii(h, A, B, 128, -1) runs.
//  D. Always last (every state): six times, a unit-ball vector is drawn, normalised with rsqrt.s and scaled by 12.0 (V); then 45 times:
//     X = P + V; a second unit-ball vector Y is drawn and scaled by 10.0; P_AddParticle__FP11ParticleDefP6Point3T1f(hateball definition, X,
//     Y, 0.0) runs, then eRand__10GameObject(O) is drawn and, when its low five bits are zero, P_AddParticle(hateball smoke definition, X, Y,
//     0.0) runs too. When t <= 0 the entry at table offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a "hate" fireball that explodes into a particle ball every frame, finds creatures on contact, chains lightning between the hit
// creatures and applies a hate spell to each. Unknown: the meaning of the words and bytes at O+76..+96, +384, +385..+396 and of the constants.
// Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char skills[];
extern float worldDifficulty;
extern unsigned char norrath_w13_spell_msg_a_view_7d6fc0[80];
extern unsigned char norrath_w13_spell_msg_a_view_7d7010[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void soundChangeSequenceVolume__Fif(int handle, float volume);
void objectUpdateInGrid__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 p, float radius);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void buildShockChainList__14HateProjectileP6Point3(void* self, Point3* point);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void* __tf15SpellHateEffect();
int checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, void* type);
void* blockAlloc__Fi(int size);
void __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill(void* self, int a, int b, void* creature, void* object, void* skill);
void addSpell__8CreatureP5Spell(void* creature, void* spell);
void lightUpdatePosition__FP5Light(void* light);
int P_StartLightning__FG6Point3T0iiffUi(Point3 a, Point3 b, int c, int d, float e, float f, unsigned g);
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3 a, Point3 b, int c, int d);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static inline float rsqrt_s(float numerator, float value) {
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
}

static void wall_hit(unsigned char* o) {
    using namespace w12c;
    word(o, 92) = 0;
    *reinterpret_cast<signed char*>(o + 385) = 2;
    word(o, 88) = static_cast<unsigned>(frameNm - 150);
    void* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shock_hit.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, o, 1.0f);
}

extern "C" void msg_run__14HateProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* hit = nullptr;
    const int t = sword(o, 88) - frameNm + 180;
    const float tf = static_cast<float>(t);
    soundChangeSequenceVolume__Fif(sword(o, 96), 1.0f);
    const float cosine = icos__Fi(half(o, 56));
    real(o, 16) = real(o, 16) + (cosine * static_cast<float>(sword(o, 92))) * bits(0x3c888888u);
    const float sine = isin__Fi(half(o, 56));
    real(o, 20) = real(o, 20) + (sine * static_cast<float>(sword(o, 92))) * bits(0x3c888888u);
    if (sword(o, 92) != 0) real(o, 24) = real(o, 24) - 1.0f;
    objectUpdateInGrid__FP10GameObject(o);
    if (*reinterpret_cast<signed char*>(o + 385) == 0) {
        bool ground = false;
        if (sword(o, 92) != 0) {
            Point3 probe = *reinterpret_cast<Point3*>(o + 16);
            const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 12.0f);
            if (real(o, 24) < h) {
                ground = true;
                wall_hit(o);
            }
        }
        if (!ground && (frameNm & 7) == 0) {
            const Point3 p = *reinterpret_cast<Point3*>(o + 16);
            Point3 low;
            Point3 high;
            high.x = p.x + 20.0f;
            high.y = p.y + 20.0f;
            high.z = p.z + 20.0f;
            low.x = p.x - 20.0f;
            low.y = p.y - 20.0f;
            low.z = p.z - 20.0f;
            void* list[512];
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 512);
            bool wall = false;
            for (int i = 0; list[i] != nullptr && !wall; ++i) {
                unsigned char* const e = static_cast<unsigned char*>(list[i]);
                if (word(o, 392) != 0) {
                    if (e == o) continue;
                    const unsigned flags = word(e, 12);
                    if (flags & 1u) continue;
                    if (!(flags & 0x20u)) {
                        wall = true;
                        wall_hit(o);
                        break;
                    }
                    if (call_entry(from_word(word(from_word(word(o, 392)), 12)), 160, to_word(e)) == 0) continue;
                } else {
                    if (e == o) continue;
                    const unsigned flags = word(e, 12);
                    if (flags & 1u) continue;
                    if (!(flags & 0x20u)) {
                        wall = true;
                        wall_hit(o);
                        break;
                    }
                    if (flags & 0xdu) continue;
                }
                const float dx = real(o, 16) - real(e, 16);
                const float dy = real(o, 20) - real(e, 20);
                const float distance = __builtin_sqrtf(dx * dx + dy * dy);
                if (distance < real(e, 28) + 20.0f) {
                    hit = e;
                    break;
                }
            }
        }
        if (hit != nullptr) {
            Point3 center = *reinterpret_cast<Point3*>(hit + 16);
            Point3 high;
            high.x = center.x + bits(0x44160000u);
            high.y = center.y + bits(0x44160000u);
            high.z = center.z + bits(0x44160000u);
            Point3 low;
            low.x = center.x - bits(0x44160000u);
            low.y = center.y - bits(0x44160000u);
            low.z = center.z - bits(0x44160000u);
            void* list[512];
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 512);
            unsigned char* const chain = o + 80;
            for (int i = 0; list[i] != nullptr; ++i) {
                unsigned char* const e = static_cast<unsigned char*>(list[i]);
                if (word(o, 392) != 0) {
                    if (call_entry(from_word(word(from_word(word(o, 392)), 12)), 160, to_word(e)) != 0) listOnewayLink__FP8ListHeadT0Pv(chain, e, e);
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
            *reinterpret_cast<signed char*>(o + 387) = 0;
            real(o, 388) = bits(0x45610000u);
            buildShockChainList__14HateProjectileP6Point3(o, reinterpret_cast<Point3*>(hit + 16));
            unsigned attack = 0x80040u;
            if (!(static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u) < 0.5f)) attack = 0x40040u;
            unsigned char description[80];
            w13a::init_description(description);
            const int n = w13a::call_entry_112(from_word(word(o, 76)), *reinterpret_cast<signed char*>(o + 384), o + 392);
            const float k2 = bits(0x3e4ccccc);
            const int roll2 = eRand__10GameObject(o);
            const float amountF = static_cast<float>(n);
            float spread = amountF * k2;
            spread = spread * (static_cast<float>(roll2) * bits(0x37800000u));
            const int m = n + static_cast<int>(spread);
            for (unsigned char* node = from_word(word(o, 84)); node != nullptr; node = from_word(word(node, 0))) {
                if (word(node, 12) == 0) continue;
                if (word(o, 396) != 0) soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(o, 396)), o, 1.0f);
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(from_word(word(node, 12)), nullptr, amountF, static_cast<float>(m), 100000, 0,
                                                                                  0, static_cast<int>(attack), description, false);
                void* const type = __tf15SpellHateEffect();
                if (checkIfSpellIsRunning__8CreatureRC9type_info(from_word(word(node, 12)), type) == 0) {
                    void* const block = blockAlloc__Fi(48);
                    if (block != nullptr)
                        __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill(block, 3, static_cast<int>(worldDifficulty), from_word(word(node, 12)), nullptr,
                                                                            from_word(word(skills, 120)));
                    addSpell__8CreatureP5Spell(from_word(word(node, 12)), block);
                }
            }
            const signed char count = *reinterpret_cast<signed char*>(o + 387);
            *reinterpret_cast<signed char*>(o + 385) = 1;
            word(o, 92) = 0;
            word(o, 88) = static_cast<unsigned>(frameNm - 150);
            if (count > 0) {
                int i = 0;
                do {
                    word(o, 292 + 4 * i) = 0xffffffffu;
                    ++i;
                } while (i < *reinterpret_cast<signed char*>(o + 387));
            }
        }
        int s = 0;
        if (tf <= 30.0f) s = 180 - 6 * t;
        unsigned char* const light = o + 324;
        *reinterpret_cast<Point3*>(o + 372) = *reinterpret_cast<Point3*>(o + 16);
        const float swing = isin__Fi(frameNm * 6000) * 6.0f;
        const float q = static_cast<float>(static_cast<int>(swing) - s + 120);
        const float square = q * q;
        real(light, 28) = q;
        real(light, 36) = q;
        const float inverse = 1.0f / square;
        real(light, 40) = inverse;
        real(light, 32) = inverse;
        lightUpdatePosition__FP5Light(light);
    }
    if (*reinterpret_cast<signed char*>(o + 385) == 1 && word(o, 84) != 0 && word(from_word(word(o, 84)), 0) != 0) {
        unsigned char* cur = from_word(word(o, 84));
        unsigned char* next = from_word(word(cur, 0));
        for (int k = 0; next != nullptr; ++k) {
            if (word(cur, 12) != 0 && word(next, 12) != 0) {
                Point3 a = *reinterpret_cast<Point3*>(from_word(word(cur, 12)) + 16);
                const Point3 offsetA = {0.0f, 0.0f, 36.0f};
                a.x = a.x + offsetA.x;
                a.z = a.z + 36.0f;
                Point3 b = *reinterpret_cast<Point3*>(from_word(word(next, 12)) + 16);
                b.x = b.x + offsetA.x;
                b.z = b.z + 36.0f;
                const int handle = sword(o, 292 + 4 * k);
                if (handle == -1) {
                    word(o, 292 + 4 * k) = static_cast<unsigned>(P_StartLightning__FG6Point3T0iiffUi(a, b, 260, 4, 16.0f, 8.5f, 0xff000fdcu));
                } else {
                    P_MoveLightning__FiG6Point3T1ii(handle, a, b, 128, -1);
                }
            }
            cur = next;
            next = from_word(word(cur, 0));
        }
    }
    for (int j = 0; j < 6; ++j) {
        Point3 u;
        unit_ball(o, u, roll);
        Point3 v = u;
        const float inverse = rsqrt_s(1.0f, (v.x * v.x + v.y * v.y) + v.z * v.z);
        v.y = v.y * inverse;
        v.z = v.z * inverse;
        v.x = v.x * inverse;
        v.x = v.x * 12.0f;
        v.y = v.y * 12.0f;
        v.z = v.z * 12.0f;
        for (int i = 44; i >= 0; --i) {
            Point3 x;
            x.x = real(o, 16) + v.x;
            x.y = real(o, 20) + v.y;
            x.z = real(o, 24) + v.z;
            Point3 y;
            unit_ball(o, y, roll);
            Point3 velocity;
            velocity.x = y.x * 10.0f;
            velocity.y = y.y * 10.0f;
            velocity.z = y.z * 10.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6fc0, &x, &velocity, 0.0f);
            if ((eRand__10GameObject(o) & 0x1f) == 0) P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d7010, &x, &velocity, 0.0f);
        }
    }
    if (t <= 0) call_entry(o, 8, 3);
}
