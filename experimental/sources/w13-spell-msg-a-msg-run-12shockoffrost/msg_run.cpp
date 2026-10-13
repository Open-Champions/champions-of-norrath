// Observed msg_run__12ShockOfFrost at 0x002bca20, 3548 bytes. O is the object, P the point (three floats) at O+16, frameNm the global frame
// counter. Pool constants with no symbols (bits): K1 = 0x41109750 (9.0369...), K2 = 0x41f995ed (31.198...), K3 = 0x40dfff27 (6.9999...),
// K4 = 0x41800003 (16.000006), K5 = 0x403fffef (2.99999...), K6 = 0x3e999999 (0.3), K7 = 0x3fdba5e3 (1.716), K8 = 0x3f199999 (0.6).
// A "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in that order,
// each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The three particle definitions are 80-byte bss objects of the image
// (file-local symbols shockoffrostParticle1 0x008d80a0, shockoffrostParticle2 0x008d80f0, shockoffrostParticle3 0x008d8140), imported
// through address-named views. A burst "(D, S, base, orbit, W)" means: a unit-ball vector scaled by S, plus (when orbit) the offset (icos(h) * 12.0,
// isin(h) * 12.0) with h the signed halfword at O+56 (taken after that vector is drawn), plus the point base, is the position; a second unit-ball vector scaled by W is the velocity; P_AddParticle__FP11ParticleDefP6Point3T1f(D,
// position, velocity, 0.0) runs. Steps in order:
//  1. lightUpdatePosition__FP5Light(O+88) after the point P is copied to O+136. A 48-byte matrix is built with matMakeTransYaw__FP8Matrix34
//     G6Point3s(matrix, copy of P, (short)(the unsigned halfword at O+56 - 32767)). With a = frameNm * 4000: the points (0, icos(a) * 6.0,
//     isin(a) * 6.0) at O+168 and (0, -icos(a) * 6.0, -isin(a) * 6.0) at O+180 are each replaced by matTransform__FP6Point3P8Matrix34T0(
//     point, matrix, point).
//  2. Thirty bursts with the first definition, scale K1 and the offset (icos(h) * 12.0, isin(h) * 12.0) where h is the signed halfword at O+56,
//     base P, velocity scale K2; the offset is added to the scaled vector before P. Twenty bursts with the second definition, scale K3, the same
//     offset, base P, velocity scale K4. Seven times, two bursts with the third definition, scale K5, no offset, velocity scale 5.0, base
//     the point at O+168 first and the point at O+180 second.
//  3. The word at O+152 drops by 2000. move__10ProjectileG6Point3(O, the point at O+156) gives r. When r has bit 2 and the word at O+196 is
//     non-zero the word at O+192 becomes 0; when r has bit 1 it becomes 0. The function ends when frameNm is below the word at O+192.
//  4. Otherwise: soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_ew_shockfrost_hit.anm"),
//     O, 1.0) and addEffect__FiG6Point3(10, copy of P). Then c = (int)((float)eRand__10GameObject(O) * 2^-16 * 4.0) is evaluated and, while
//     the loop index i (from 0) is below c (c is drawn again after each step): 584 bytes come from blockAlloc__Fi and, when non-null,
//     the PhProp constructor __6PhPropG6Point3PcN22liPQ24item11descriptionf runs on it with (copy of P, "fx.lmp", "iceshard.vif" and
//     "iceshard.tex", kind 1 << i, flags 274, null description, 1.0). The single at +252 becomes (old * K6) + (the single at O+156 * K6 *
//     K7), the single at +256 becomes (old * K6) + (the single at O+160 * K6 * K7), the single at +48 becomes old * K8, and
//     initStuff__6PhPropi(block, 274) runs.
//  5. With C the object that is the word at O+200: n is the entry at table offset 112 of the table that is the word at +96 of C, called on C
//     (adjusted by the entry's signed halfword) with (1, the signed halfword at O+148, the address O+84). While n > 0 it repeats n times:
//     y = (short)((unsigned halfword at O+56) + (eRand % 32767) - 16384); ten times s = (short)eRand: the point U = (O.x + icos(s) * 24.0,
//     O.y + isin(s) * 24.0, O.z) is formed and worldHitscan__FP12_worldHeaderG6Point3T1fi(world, copy of P, U - P, 2.0, 0) is asked; when the
//     result is not above 0.0, y becomes s. Then 124 bytes come from blockAlloc__Fi; when non-null a FrostShard-like object is built on it:
//     __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(block, copy of P, the object that is the word at (the word at O+84) +12,
//     null), the word at +72 = _vt$10FrostShard, the word at +84 = 0 and that owner linked with listOnewayLink__FP8ListHeadT0Pv(block+84, owner,
//     owner) when non-null, halfword +88 = the halfword at O+148, halfword +56 = y, the point (icos(y) * 351.0, isin(y) * 351.0, 0) at +96, the
//     word +108 = frameNm + 60, +112 = 0, +116 = the word at O+200, +120 = the word at O+204, then objectAddToRunList__FP10GameObject(block).
//  6. Finally the entry at table offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a spell that sprays frost particles around its path, and on contact spawns ice chunks and frost shards. Unknown: what the words
// at O+148..+204 mean, the constants, and what the flag word 274 and the kind 1 << i select. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w13_spell_msg_a_view_8d80a0[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d80f0[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d8140[80];
extern unsigned char vt_FrostShard[] __asm__("_vt$10FrostShard");
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void lightUpdatePosition__FP5Light(void* light);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, Point3 p, short yaw);
void matTransform__FP6Point3P8Matrix34T0(Point3* out, void* matrix, Point3* in);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void addEffect__FiG6Point3(int kind, Point3 p);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long kind, int flags, void* description, float scale);
void initStuff__6PhPropi(void* self, int flags);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, Point3 from, Point3 direction, float limit, int mode);
void __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(void* self, Point3 position, void* owner, void* map);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void objectAddToRunList__FP10GameObject(void* object);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static void burst(unsigned char* o, void* definition, float positionScale, bool orbit, const float* base, float velocityScale) {
    using namespace w12c;
    Point3 v;
    unit_ball(o, v, roll);
    Point3 x;
    x.x = v.x * positionScale;
    x.y = v.y * positionScale;
    x.z = v.z * positionScale;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    if (orbit) {
        offsetX = icos__Fi(half(o, 56)) * 12.0f;
        offsetY = isin__Fi(half(o, 56)) * 12.0f;
    }
    x.x = (x.x + offsetX) + base[0];
    x.y = (x.y + offsetY) + base[1];
    x.z = x.z + base[2];
    Point3 y;
    unit_ball(o, y, roll);
    Point3 velocity;
    velocity.x = y.x * velocityScale;
    velocity.y = y.y * velocityScale;
    velocity.z = y.z * velocityScale;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &x, &velocity, 0.0f);
}

extern "C" void msg_run__12ShockOfFrost(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float* const p = reinterpret_cast<const float*>(o + 16);
    *reinterpret_cast<Point3*>(o + 136) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 88);
    float matrix[16];
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, *reinterpret_cast<Point3*>(o + 16), static_cast<short>(static_cast<unsigned short>(half(o, 56)) - 32767));
    word(o, 168) = 0;
    real(o, 172) = icos__Fi(frameNm * 4000) * 6.0f;
    real(o, 176) = isin__Fi(frameNm * 4000) * 6.0f;
    word(o, 180) = 0;
    real(o, 184) = -icos__Fi(frameNm * 4000) * 6.0f;
    real(o, 188) = -isin__Fi(frameNm * 4000) * 6.0f;
    matTransform__FP6Point3P8Matrix34T0(reinterpret_cast<Point3*>(o + 168), matrix, reinterpret_cast<Point3*>(o + 168));
    matTransform__FP6Point3P8Matrix34T0(reinterpret_cast<Point3*>(o + 180), matrix, reinterpret_cast<Point3*>(o + 180));
    for (int i = 0; i < 30; ++i) burst(o, norrath_w13_spell_msg_a_view_8d80a0, bits(0x41109750u), true, p, bits(0x41f995edu));
    for (int i = 0; i < 20; ++i) burst(o, norrath_w13_spell_msg_a_view_8d80f0, bits(0x40dfff27u), true, p, bits(0x41800003u));
    for (int i = 0; i < 7; ++i) {
        burst(o, norrath_w13_spell_msg_a_view_8d8140, bits(0x403fffefu), false, reinterpret_cast<const float*>(o + 168), 5.0f);
        burst(o, norrath_w13_spell_msg_a_view_8d8140, bits(0x403fffefu), false, reinterpret_cast<const float*>(o + 180), 5.0f);
    }
    sword(o, 152) = sword(o, 152) - 2000;
    Point3 delta = *reinterpret_cast<Point3*>(o + 156);
    const int r = move__10ProjectileG6Point3(o, delta);
    if (r & 2) {
        if (word(o, 196) != 0) word(o, 192) = 0;
    }
    if (r & 1) word(o, 192) = 0;
    if (frameNm < sword(o, 192)) return;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shockfrost_hit.anm")), o, 1.0f);
    Point3 at = *reinterpret_cast<Point3*>(o + 16);
    addEffect__FiG6Point3(10, at);
    for (int i = 0; i < static_cast<int>((static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u)) * 4.0f); ++i) {
        unsigned char* const block = static_cast<unsigned char*>(blockAlloc__Fi(584));
        if (block != nullptr) {
            Point3 where = *reinterpret_cast<Point3*>(o + 16);
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(block, where, const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.vif"),
                                                           const_cast<char*>("iceshard.tex"), 1L << i, 274, nullptr, 1.0f);
            real(block, 252) = real(block, 252) * bits(0x3e999999u) + (real(o, 156) * bits(0x3e999999u)) * bits(0x3fdba5e3u);
            real(block, 48) = real(block, 48) * bits(0x3f199999u);
            real(block, 256) = real(block, 256) * bits(0x3e999999u) + (real(o, 160) * bits(0x3e999999u)) * bits(0x3fdba5e3u);
            initStuff__6PhPropi(block, 274);
        }
    }
    unsigned char* const caster = from_word(word(o, 200));
    const int n = w13a::call_entry_112_mode(caster, 1, half(o, 148), o + 84);
    for (int count = n; count > 0; --count) {
        short yaw = static_cast<short>(static_cast<unsigned short>(half(o, 56)) + (static_cast<unsigned>(eRand__10GameObject(o)) % 32767u) - 16384);
        for (int k = 9; k >= 0; --k) {
            const short s = static_cast<short>(eRand__10GameObject(o));
            Point3 u;
            u.x = real(o, 16) + icos__Fi(s) * 24.0f;
            u.y = real(o, 20) + isin__Fi(s) * 24.0f;
            u.z = real(o, 24);
            Point3 direction;
            direction.x = u.x - p[0];
            direction.y = u.y - p[1];
            direction.z = u.z - p[2];
            Point3 from = *reinterpret_cast<Point3*>(o + 16);
            const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, 2.0f, 0);
            if (!(0.0f < hit)) yaw = s;
        }
        unsigned char* const shard = static_cast<unsigned char*>(blockAlloc__Fi(124));
        if (shard != nullptr) {
            Point3 where = *reinterpret_cast<Point3*>(o + 16);
            unsigned char* const owner = from_word(word(from_word(word(o, 84)), 12));
            const short kind = half(o, 148);
            const unsigned head = word(o, 200);
            const unsigned extra = word(o, 204);
            __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(shard, where, owner, nullptr);
            word(shard, 72) = to_word(vt_FrostShard);
            word(shard, 84) = 0;
            if (owner != nullptr) listOnewayLink__FP8ListHeadT0Pv(shard + 84, owner, owner);
            half(shard, 88) = kind;
            half(shard, 56) = yaw;
            const float c = icos__Fi(yaw) * bits(0x43af0000u);
            const float sn = isin__Fi(half(shard, 56)) * bits(0x43af0000u);
            Point3 velocity;
            velocity.x = c;
            velocity.y = sn;
            velocity.z = 0.0f;
            *reinterpret_cast<Point3*>(shard + 96) = velocity;
            word(shard, 108) = static_cast<unsigned>(frameNm + 60);
            word(shard, 116) = head;
            word(shard, 120) = extra;
            word(shard, 112) = 0;
            objectAddToRunList__FP10GameObject(shard);
        }
    }
    call_entry(o, 8, 3);
}
