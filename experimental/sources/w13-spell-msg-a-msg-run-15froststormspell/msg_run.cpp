// Observed msg_run__15FrostStormSpell at 0x001e23f8, 3596 bytes. O is the object, P the point (three floats) at O+16, frameNm the global
// frame counter, K the single at O+84. Pool constants with no symbols (bits): CA = 0x3c088888 (1/120), CB = 0x3e4ccccc (0.2), CC = 0x3f800032,
// CD = 0x4130974d, CE = 0x40d99999 (6.8), CF = 0x3f7fffef, CG = 0x4004bc51, CH = 0x3d50e560 (0.051), CI = 0x3be38e37, CJ = 0x3f800008,
// CK = 0x3e51ec91, CL = 0x407fffc9, CM = 0x3eccccab (0.4), CN = 0x3f733333 (0.95). A "unit-ball vector" is drawn by rejection: each component
// is eRand__10GameObject(O) * 2^-16 doubled minus 1.0, x then y then z, repeated while 1.0 < (x*x + y*y) + z*z. "negabs(x)" is 0.0 - |x|,
// which the image computes through the double-precision helpers fptodp, dpcmp, dpsub and dptofp (exact for single values). Four particle
// definitions are 80-byte bss objects of the image (file-local symbols froststormParticle1 0x007d6a10, froststormParticle2 0x007d6a60 and
// froststormParticle4 0x007d6b00), imported through address-named views. "H" below is the draw of the height term: (float)eRand * 2^-16 *
// 12.0 + 120.0. Steps in order:
//  1. Nothing happens unless the word at O+80 (signed) is below frameNm.
//  2. P gets the three singles at O+96 added. K becomes K * (the single at O+88). The point P is copied to O+160. lightUpdatePosition__FP5
//     Light(O+112) runs; then with q = (the single at O+140) * (the single at O+88) the singles at O+140 and O+148 become q and those at
//     O+144 and O+152 become 1.0 / (q * q).
//  3. Only when the single at O+88 equals 1.0 (the rest of steps 3 to 6 are skipped otherwise):
//     3a. i from 0 while (float)i < (K * CA) * (K * CB) (K is read again each time): a unit-ball vector V is scaled by K * CC on x and y; the
//         third component is H; X = V + P; a second unit-ball vector W is scaled by 7.0 on all three components and then W.z becomes
//         negabs(W.z * CD); P_AddParticle__FP11ParticleDefP6Point3T1f(definition 1, X, W, 0.0) runs.
//     3b. The same loop with the bound (K * CA) * (K * CB), V scaled by K * CF, W scaled by CE and W.z = negabs(W.z * CG), definition 2.
//     3c. When frameNm % 10 == 0 and 0 < ((K * CH) * CI) * K: i from 0 while (float)i < ((K * CH) * CI) * K: V scaled by K * CJ on x and y,
//         third component H, X = V + P; W scaled by CK on x and y and its z multiplied by CL then negabs; 100 bytes come from blockAlloc__Fi;
//         when non-null the game object constructor __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(block, X, null, null) runs, the word
//         at +72 becomes _vt$18FrostStormIceShard, the word at +12 gets the bits 0x421d set, the point at +16 becomes X, the word at +76 is 0,
//         the word at +96 is frameNm + 120, the point at +84 is W, the single at +80 is 1.0 and the single at +28 is 12.0;
//         objectUpdateInGrid__FP10GameObject(block) and objectAddToRunList__FP10GameObject(block) run.
//     3d. i from 0 while (float)i < (K * CA) * (K * CM): a = (short)eRand, r = (float)eRand * 2^-16; X = (cos(a) * (r * K) + P.x,
//         sin(a) * (r * K) + P.y, 0.0 + P.z); then X.z becomes worldFindPointHeight__FP12_worldHeaderG6Point3(world, X) + 5.0; when
//         (P.z - 148.0) <= X.z, P_AddParticle(definition 4, X, (0, 0, 0), 0.0) runs.
//  4. When frameNm % 30 == 0: the box from P - K to P + K is searched with objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 64).
//     For each entry E in order, with C the word at O+108 (a list node): when C is non-zero the entry at table offset 160 of the object that is
//     the word at C+12 is called on it with E, and a zero result skips E. Then an item description with no entries set (see spell.hpp) gets the
//     entry (22, n, n, 0) with n = (int)((float)(the signed word at O+92) * 0.5) and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11
//     descriptionb(E, the object that is the word at C+12 (null when C is zero), (float)w * 0.5 twice, 100000, 0, 0, 0, description, false) runs.
//  5. Finally when the word at O+76 (signed) is below frameNm: the single at O+88 becomes CN, and when K < 12.0 the entry at table offset 8 of
//     O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a falling frost cloud that spawns particles and ice shards, damages objects in its box every 30 frames and shrinks away. Unknown: the
// meaning of the words and singles at O+76..+108, +140..+160 and of the pool constants. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w13_spell_msg_a_view_7d6a10[80];
extern unsigned char norrath_w13_spell_msg_a_view_7d6a60[80];
extern unsigned char norrath_w13_spell_msg_a_view_7d6b00[80];
extern unsigned char vt_FrostStormIceShard[] __asm__("_vt$18FrostStormIceShard");
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void lightUpdatePosition__FP5Light(void* light);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void* blockAlloc__Fi(int size);
void __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* map, void* names);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
float worldFindPointHeight__FP12_worldHeaderG6Point3(void* world, Point3 p);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static float negabs(float x) {
    const float magnitude = x < 0.0f ? 0.0f - x : x;
    return 0.0f - magnitude;
}

static float height_draw(unsigned char* o) {
    using namespace w12c;
    return (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u)) * 12.0f + 120.0f;
}

// The loop bound shared by the first two particle loops.
static float bound_ab(float k) { using namespace w12c; return (k * bits(0x3c088888u)) * (k * bits(0x3e4ccccc)); }

extern "C" void msg_run__15FrostStormSpell(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(sword(o, 80) < frameNm)) return;
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    real(o, 16) = real(o, 16) + real(o, 96);
    real(o, 20) = real(o, 20) + real(o, 100);
    real(o, 24) = real(o, 24) + real(o, 104);
    real(o, 84) = real(o, 84) * real(o, 88);
    *reinterpret_cast<Point3*>(o + 160) = *position;
    unsigned char* const light = o + 112;
    lightUpdatePosition__FP5Light(light);
    const float q = real(light, 28) * real(o, 88);
    real(light, 28) = q;
    real(light, 36) = q;
    const float inverse = 1.0f / (q * q);
    real(light, 40) = inverse;
    real(light, 32) = inverse;
    if (real(o, 88) == 1.0f) {
        for (int i = 0; static_cast<float>(i) < bound_ab(real(o, 84)); ++i) {
            Point3 v;
            unit_ball(o, v, roll);
            const float scale = real(o, 84) * bits(0x3f800032u);
            v.x = v.x * scale;
            v.y = v.y * scale;
            Point3 x;
            x.x = v.x + position->x;
            x.y = v.y + position->y;
            x.z = height_draw(o) + position->z;
            Point3 w;
            unit_ball(o, w, roll);
            w.x = w.x * 7.0f;
            w.y = w.y * 7.0f;
            w.z = negabs((w.z * 7.0f) * bits(0x4130974du));
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6a10, &x, &w, 0.0f);
        }
        for (int i = 0; static_cast<float>(i) < bound_ab(real(o, 84)); ++i) {
            Point3 v;
            unit_ball(o, v, roll);
            const float scale = real(o, 84) * bits(0x3f7fffefu);
            v.x = v.x * scale;
            v.y = v.y * scale;
            Point3 x;
            x.x = v.x + position->x;
            x.y = v.y + position->y;
            x.z = height_draw(o) + position->z;
            Point3 w;
            unit_ball(o, w, roll);
            w.x = w.x * bits(0x40d99999u);
            w.y = w.y * bits(0x40d99999u);
            w.z = negabs((w.z * bits(0x40d99999u)) * bits(0x4004bc51u));
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6a60, &x, &w, 0.0f);
        }
        if (frameNm % 10 == 0 && 0.0f < ((real(o, 84) * bits(0x3d50e560u)) * bits(0x3be38e37u)) * real(o, 84)) {
            for (int i = 0; static_cast<float>(i) < ((real(o, 84) * bits(0x3d50e560u)) * bits(0x3be38e37u)) * real(o, 84); ++i) {
                Point3 v;
                unit_ball(o, v, roll);
                const float scale = real(o, 84) * bits(0x3f800008u);
                v.x = v.x * scale;
                v.y = v.y * scale;
                Point3 x;
                x.x = v.x + position->x;
                x.y = v.y + position->y;
                x.z = height_draw(o) + position->z;
                Point3 w;
                unit_ball(o, w, roll);
                w.x = w.x * bits(0x3e51ec91u);
                w.y = w.y * bits(0x3e51ec91u);
                w.z = negabs(w.z * bits(0x407fffc9u));
                unsigned char* const shard = static_cast<unsigned char*>(blockAlloc__Fi(100));
                if (shard != nullptr) {
                    Point3 at = x;
                    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(shard, at, nullptr, nullptr);
                    word(shard, 72) = to_word(vt_FrostStormIceShard);
                    word(shard, 12) = word(shard, 12) | 0x421du;
                    *reinterpret_cast<Point3*>(shard + 16) = x;
                    word(shard, 76) = 0;
                    word(shard, 96) = static_cast<unsigned>(frameNm + 120);
                    *reinterpret_cast<Point3*>(shard + 84) = w;
                    real(shard, 80) = 1.0f;
                    real(shard, 28) = 12.0f;
                    objectUpdateInGrid__FP10GameObject(shard);
                    objectAddToRunList__FP10GameObject(shard);
                }
            }
        }
        for (int i = 0; static_cast<float>(i) < (real(o, 84) * bits(0x3c088888u)) * (real(o, 84) * bits(0x3eccccabu)); ++i) {
            const short angle = static_cast<short>(eRand__10GameObject(o));
            const float r = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u);
            const float c = icos__Fi(angle);
            const float dx = c * (r * real(o, 84));
            const float s = isin__Fi(angle);
            Point3 x;
            x.x = dx;
            x.y = s * (r * real(o, 84));
            x.z = 0.0f;
            Point3 w = {0.0f, 0.0f, 0.0f};
            x.x = x.x + position->x;
            x.y = x.y + position->y;
            x.z = x.z + position->z;
            const float h = worldFindPointHeight__FP12_worldHeaderG6Point3(world, x);
            x.z = h + 5.0f;
            if (real(o, 24) - 148.0f <= x.z) P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6b00, &x, &w, 0.0f);
        }
    }
    if (frameNm % 30 == 0) {
        const float k = real(o, 84);
        Point3 high;
        Point3 low;
        high.x = position->x + k;
        high.y = position->y + k;
        high.z = position->z + k;
        low.x = position->x - k;
        low.y = position->y - k;
        low.z = position->z - k;
        void* list[64];
        objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 64);
        for (int i = 0; list[i] != nullptr; ++i) {
            unsigned char* const e = static_cast<unsigned char*>(list[i]);
            if (word(o, 108) != 0) {
                if (call_entry(from_word(word(from_word(word(o, 108)), 12)), 160, to_word(e)) == 0) continue;
            }
            unsigned char description[80];
            w13a::init_description(description);
            const int w = sword(o, 92);
            const int n = static_cast<int>(static_cast<float>(w) * 0.5f);
            w13a::fill_description(description, 22, static_cast<short>(n), static_cast<short>(n), 0);
            unsigned char* owner = nullptr;
            if (word(o, 108) != 0) owner = from_word(word(from_word(word(o, 108)), 12));
            const float amount = static_cast<float>(w) * 0.5f;
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(e, owner, amount, amount, 100000, 0, 0, 0, description, false);
        }
    }
    if (sword(o, 76) < frameNm) {
        real(o, 88) = bits(0x3f733333u);
        if (real(o, 84) < 12.0f) call_entry(o, 8, 3);
    }
}
