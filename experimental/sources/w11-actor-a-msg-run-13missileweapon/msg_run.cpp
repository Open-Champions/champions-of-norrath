// Observed msg_run__13MissileWeapon at 0x00145720, 4452 bytes. Argument: the object o. Nothing is returned. Names:
// pos = floats at o+16..o+24; vel = the three floats at +428; kind = the signed word at +208; S = the signed byte
// at +204 (a state); L = the Light member at +368; the object's own random state is the word at +68, stepped as
// s = s * 0x19660d + 0x3c6ef35f and drawn as (s >> 16) (inline, not through eRand); "unit(s)" below means the
// rejection sampler of three such draws, each twice ((float)draw * 2^-16) minus one, repeated until the squared
// length is not above 1.0 (the same shared unit_ball). "delete" means the entry at offset 8 of o's table called
// on o with 3 (the image skips it when o is null). Float constants read through the global pointer, none with a
// symbol: K1 = 21.6 (0x41accccc), K2 = 1.633 (0x3fd10624), K3 = 0.06 (0x3d75c28f), K5 = 0.01 (0x3c23d70a), K4 = 0.1
// (0x3dcccccc), K7 = 0.06 (0x3d75c28f), K8 = 0.08 (0x3da3d70a), K9 = 12.129 (0x41421062), K10 = 0.08 (0x3da3d70a).
//  1. When the word at +292 (X) is nonzero: the byte at +205 becomes objectActiveProcessing(o, signed byte +205, 480);
//     with T = the word at X+12, when T's flags (word +12) have 0x20 and the 64-bit word at T+160 has bit 41, the
//     object is queued with objectAddToDeleteList. Return.
//  2. When S is nonzero: delete; return.
//  3. When the halfword at +250 is nonzero, the halfword at +252 gets the unsigned sum of both. result =
//     move(o, vel) & ~2, with 2 added again when S is nonzero.
//  4. When result is nonzero (a hit): the sound lumpFindResource("fx.lmp", n) is played at volume 1.0 with
//     n = "spel_hit_explarrow.anm" for kinds 1 and 2, "spel_cold_afteraffct.anm" for kind 5 and
//     "spel_elctric_afteraffct.anm" for kind 6. The word at +236 (an end frame) is cleared. Then:
//     result 2 (S nonzero, kind 0 or 3 only): S == 2 adds effect 17 (levelFlags bit 0x100) or 2 at pos; S == 3
//     sets +236 = frameNm; any other S sets +236 = frameNm + 1.
//     result 1: kind 1 adds effect 5 at pos; kinds 0 and 3 draw s: levelFlags bit 0x40000 picks the text
//     "impct_mtl_ice%d.anm" else "impct_mtl_stone%d.anm" with %d = 1 + ((draw) & 3), add effect 8 (bit
//     0x40000) or 17 (bit 0x100) or 2 at pos and play the named sound; every kind but 1, 0 and 3 plays no sound.
//     Then for results 1 (kinds 1, 0, 3, and every other kind) and 2, when the word at +440 is nonzero,
//     soundPlaySound(that vag, pos, 1.0, 1.0, 0.0).
//     Then by kind: 1: 3 + (draw & 3) ParticleProp objects (328 bytes, constructor with pos and 8), then a 188-byte
//     LightEffect (255, 155, 0, -32, 0, 0, 0, 0, 216, 120, 1, 1); 5: effect 10 at pos and the same light
//     with (0, 155, 255, ...); 6: 100 ballSpark particles (velocity unit * K1, z also + K2) and the blue light;
//     2: makeExplosion(pos, 2.0), the orange light and objectRadialDamage(o, pos, word +216, 0, 24.0, 72.0, 0, 0).
//  5. Whether or not it hit, by kind: 1: 24 times, a fire trail particle and a smoke trail particle around
//     pos + vel * K3 (jittered by three draws mod 40 minus 20 times K4 plus vel * K5 * unit) with velocity unit * 4.0,
//     the smoke point being moved back by vel * K7; then pos is saved at +416. 2: pos is saved at +416. 5: 32
//     snow particles at (unit * 10 + pos + vel * K8) with velocity unit * K9, where the second unit draw starts
//     from an uninitialised stack word (the image reads it; it is as indeterminate here); pos is saved. 6: when the
//     owner's (word +76, its word +12) feat 4096 rank is 2 or more, fxRunBallLightning(o+84, o, owner, pos +
//     vel * K10, 1.0, 0, 3); pos is saved. 0: with levelFlags bit 0x100, when a draw mod 3 is 0 effect 17 is added
//     at pos with z + 18 * a second draw / 65536 and nothing else of this step runs; otherwise, and for every other
//     kind, P_ExtendTrail(word +364, pos) when that word is nonzero. For kinds 1, 2, 5 and 6, lightUpdatePosition(L)
//     runs.
//  6. When frameNm >= the word at +236, delete.
// Inferred: this is the per-frame handler of a thrown spell projectile, from its symbol; kinds select the spell
// element by the sound names. Unknown: the meaning of the states, +205, +236, +440 and the numeric effect ids.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern int levelFlags;
extern int frameNm;
extern unsigned char ballSparkParticle[], trailParticleFire[], trailParticleSmoke[], snowParticle1[];
int objectActiveProcessing__FP10GameObjectii(void* object, int a, int b);
void objectAddToDeleteList__FP10GameObject(void* object);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void* lumpFindResource__FPcT0(char* lump, char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
void addEffect__FiG6Point3(int effect, Point3 position);
int libc_sprintf(char* buffer, const char* format, ...) __asm__("sprintf");
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, Point3* position, float a, float b, float c);
void __12ParticlePropG6Point3i(void* self, Point3 position, int a);
void* blockAlloc__Fi(int size);
void LightEffect_construct(void* self, Point3 position, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l,
                           void* object, int n) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void makeExplosion__FG6Point3f(Point3 position, float scale);
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* object, Point3 position, int a, int b, float c, float d, int e, int f);
int playerGetFeatRank__Fix(int player, long long feat);
void fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(void* state, void* object, void* owner, Point3 position, float a, int b, int c);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void lightUpdatePosition__FP5Light(void* light);
}

namespace {
using namespace w11a;

inline unsigned step(unsigned s) { return s * 0x19660du + 0x3c6ef35fu; }

void delete_self(unsigned char* o) {
    if (o != nullptr) call_entry(o, 8, 3);
}

void light_orange(Point3 position) {
    void* const block = blockAlloc__Fi(188);
    if (block != nullptr)
        LightEffect_construct(block, position, bits(0x437f0000u), bits(0x431b0000u), 0.0f, bits(0xc2000000u), 0.0f, 0.0f, 0.0f, 0.0f, bits(0x43580000u), bits(0x42f00000u), 1.0f,
                              1.0f, nullptr, 0);
}

void light_blue(Point3 position) {
    void* const block = blockAlloc__Fi(188);
    if (block != nullptr)
        LightEffect_construct(block, position, 0.0f, bits(0x431b0000u), bits(0x437f0000u), bits(0xc2000000u), 0.0f, 0.0f, 0.0f, 0.0f, bits(0x43580000u), bits(0x42f00000u), 1.0f,
                              1.0f, nullptr, 0);
}

void play_named(const char* name, unsigned char* o) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), o, 1.0f);
}
}  // namespace

extern "C" void msg_run__13MissileWeapon(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 292) != 0) {
        o[205] = static_cast<unsigned char>(objectActiveProcessing__FP10GameObjectii(o, *reinterpret_cast<signed char*>(o + 205), 480));
        unsigned char* const target = from_word(word(from_word(word(o, 292)), 12));
        if ((word(target, 12) & 0x20u) && (dword(target, 160) & (0x8000ull << 26)) != 0) objectAddToDeleteList__FP10GameObject(o);
        return;
    }
    if (*reinterpret_cast<signed char*>(o + 204) != 0) {
        delete_self(o);
        return;
    }
    if (half(o, 250) != 0) half(o, 252) = static_cast<short>(static_cast<unsigned short>(half(o, 252)) + static_cast<unsigned short>(half(o, 250)));
    int result = move__10ProjectileG6Point3(o, *reinterpret_cast<Point3*>(o + 428)) & ~2;
    if (*reinterpret_cast<signed char*>(o + 204) != 0) result |= 2;
    if (result != 0) {
        int kind = sword(o, 208);
        if (static_cast<unsigned>(kind - 1) < 2u) play_named("spel_hit_explarrow.anm", o);
        else if (kind == 5) play_named("spel_cold_afteraffct.anm", o);
        else if (kind == 6) play_named("spel_elctric_afteraffct.anm", o);
        kind = sword(o, 208);
        word(o, 236) = 0;
        if (result == 2) {
            const int state = *reinterpret_cast<signed char*>(o + 204);
            if (state != 0 && (kind == 0 || kind == 3)) {
                if (state == 2) {
                    addEffect__FiG6Point3((levelFlags & 0x100) ? 17 : 2, *reinterpret_cast<Point3*>(o + 16));
                } else if (state == 3) {
                    word(o, 236) = static_cast<unsigned>(frameNm);
                } else {
                    word(o, 236) = static_cast<unsigned>(frameNm) + 1u;
                }
            }
        } else if (result == 1) {
            if (kind == 1) {
                addEffect__FiG6Point3(5, *reinterpret_cast<Point3*>(o + 16));
            } else if (kind == 0 || kind == 3) {
                char name[64];
                unsigned s = step(word(o, 68));
                word(o, 68) = s;
                if (levelFlags & 0x40000) {
                    libc_sprintf(name, "impct_mtl_ice%d.anm", static_cast<int>((s >> 16) & 3u) + 1);
                    addEffect__FiG6Point3(8, *reinterpret_cast<Point3*>(o + 16));
                } else {
                    libc_sprintf(name, "impct_mtl_stone%d.anm", static_cast<int>((s >> 16) & 3u) + 1);
                    addEffect__FiG6Point3((levelFlags & 0x100) ? 17 : 2, *reinterpret_cast<Point3*>(o + 16));
                }
                play_named(name, o);
            }
            if (word(o, 440) != 0)
                soundPlaySound__FP10_vagHeaderP6Point3fff(from_word(word(o, 440)), reinterpret_cast<Point3*>(o + 16), 1.0f, 1.0f, 0.0f);
            kind = sword(o, 208);
        }
        if (kind == 1) {
            unsigned s = step(word(o, 68));
            const unsigned count = (s >> 16 & 3u) + 3u;
            word(o, 68) = s;
            for (unsigned i = count; i != 0; --i) {
                void* const block = blockAlloc__Fi(328);
                if (block != nullptr) __12ParticlePropG6Point3i(block, *reinterpret_cast<Point3*>(o + 16), 8);
            }
            light_orange(*reinterpret_cast<Point3*>(o + 16));
        } else if (kind == 5) {
            addEffect__FiG6Point3(10, *reinterpret_cast<Point3*>(o + 16));
            light_blue(*reinterpret_cast<Point3*>(o + 16));
        } else if (kind == 6) {
            for (int i = 0; i < 100; ++i) {
                unsigned s = word(o, 68);
                Point3 velocity;
                unit_ball(o, velocity, [&s](unsigned char*) { s = step(s); return s >> 16; });
                word(o, 68) = s;
                velocity.x = velocity.x * bits(0x41accccc);
                velocity.y = velocity.y * bits(0x41accccc);
                velocity.z = velocity.z * bits(0x41accccc) + bits(0x3fd10624);
                P_AddParticle__FP11ParticleDefP6Point3T1f(ballSparkParticle, reinterpret_cast<Point3*>(o + 16), &velocity, 0.0f);
            }
            light_blue(*reinterpret_cast<Point3*>(o + 16));
        } else if (kind == 2) {
            makeExplosion__FG6Point3f(*reinterpret_cast<Point3*>(o + 16), 2.0f);
            light_orange(*reinterpret_cast<Point3*>(o + 16));
            objectRadialDamage__FP10GameObjectG6Point3iffiii(o, *reinterpret_cast<Point3*>(o + 16), sword(o, 216), 0, bits(0x41c00000u), bits(0x42900000u), 0, 0);
        }
    }
    const int kind = sword(o, 208);
    if (kind == 1) {
        Point3 base;
        base.x = real(o, 16) + real(o, 428) * bits(0x3d75c28fu);
        base.y = real(o, 20) + real(o, 432) * bits(0x3d75c28fu);
        base.z = real(o, 24) + real(o, 436) * bits(0x3d75c28fu);
        for (int i = 0; i < 24; ++i) {
            unsigned s = word(o, 68);
            Point3 a;
            unit_ball(o, a, [&s](unsigned char*) { s = step(s); return s >> 16; });
            const unsigned r1 = (s = step(s)) >> 16;
            const unsigned r2 = (s = step(s)) >> 16;
            const unsigned r3 = (s = step(s)) >> 16;
            word(o, 68) = s;
            Point3 point;
            const float dx = real(o, 428) * bits(0x3c23d70au) * a.x;
            const float dy = real(o, 432) * bits(0x3c23d70au) * a.y;
            const float dz = real(o, 436) * bits(0x3c23d70au) * a.z;
            point.x = base.x + static_cast<float>(static_cast<int>(r1 % 40u) - 20) * bits(0x3dcccccc) + dx;
            point.y = base.y + static_cast<float>(static_cast<int>(r2 % 40u) - 20) * bits(0x3dcccccc) + dy;
            point.z = base.z + static_cast<float>(static_cast<int>(r3 % 40u) - 20) * bits(0x3dcccccc) + dz;
            Point3 velocity;
            unit_ball(o, velocity, [&s](unsigned char*) { s = step(s); return s >> 16; });
            word(o, 68) = s;
            velocity.x = velocity.x * bits(0x40800000u);
            velocity.y = velocity.y * bits(0x40800000u);
            velocity.z = velocity.z * bits(0x40800000u);
            P_AddParticle__FP11ParticleDefP6Point3T1f(trailParticleFire, &point, &velocity, 0.0f);
            point.x = point.x - real(o, 428) * bits(0x3d75c28fu);
            point.y = point.y - real(o, 432) * bits(0x3d75c28fu);
            point.z = point.z - real(o, 436) * bits(0x3d75c28fu);
            P_AddParticle__FP11ParticleDefP6Point3T1f(trailParticleSmoke, &point, &velocity, 0.0f);
        }
        *reinterpret_cast<Point3*>(o + 416) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 368);
    } else if (kind == 2) {
        *reinterpret_cast<Point3*>(o + 416) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 368);
    } else if (kind == 5) {
        for (int i = 0; i < 32; ++i) {
            unsigned s = word(o, 68);
            Point3 a;
            unit_ball(o, a, [&s](unsigned char*) { s = step(s); return s >> 16; });
            word(o, 68) = s;
            a.x = a.x * 10.0f;
            a.y = a.y * 10.0f;
            a.z = a.z * 10.0f;
            Point3 d;
            d.x = real(o, 428) * bits(0x3da3d70au);
            d.y = real(o, 432) * bits(0x3da3d70au);
            d.z = real(o, 436) * bits(0x3da3d70au);
            Point3 p = {real(o, 16) + d.x, real(o, 20) + d.y, real(o, 24) + d.z};
            a.x = a.x + p.x;
            a.y = a.y + p.y;
            a.z = a.z + p.z;
            unsigned stale;
            __asm__("" : "=m"(stale));
            unsigned t = stale;
            Point3 velocity;
            unit_ball(o, velocity, [&t](unsigned char*) { t = step(t); return t >> 16; });
            word(o, 68) = t;
            velocity.x = velocity.x * bits(0x41421062u);
            velocity.y = velocity.y * bits(0x41421062u);
            velocity.z = velocity.z * bits(0x41421062u);
            P_AddParticle__FP11ParticleDefP6Point3T1f(snowParticle1, &a, &velocity, 0.0f);
        }
        *reinterpret_cast<Point3*>(o + 416) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 368);
    } else if (kind == 6) {
        int rank = 0;
        const unsigned owner = word(o, 76) != 0 ? word(from_word(word(o, 76)), 12) : 0u;
        if (owner != 0 && (word(from_word(owner), 12) & 0x40u)) rank = playerGetFeatRank__Fix(sword(from_word(owner), 960), 4096);
        if (rank >= 2) {
            Point3 offset = {real(o, 428) * bits(0x3da3d70au), real(o, 432) * bits(0x3da3d70au), real(o, 436) * bits(0x3da3d70au)};
            Point3 point = {real(o, 16) + offset.x, real(o, 20) + offset.y, real(o, 24) + offset.z};
            fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(
                o + 84, o, word(o, 76) != 0 ? from_word(word(from_word(word(o, 76)), 12)) : nullptr, point, 1.0f, 0, 3);
        }
        *reinterpret_cast<Point3*>(o + 416) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 368);
    } else {
        bool done = false;
        if (kind == 0 && (levelFlags & 0x100)) {
            const unsigned first = step(word(o, 68));
            word(o, 68) = first;
            if ((first >> 16) % 3u == 0) {
                const unsigned second = step(first);
                word(o, 68) = second;
                Point3 point = *reinterpret_cast<Point3*>(o + 16);
                point.z = point.z + static_cast<float>(static_cast<int>(second >> 16)) * bits(0x37800000u) * bits(0x41900000u);
                addEffect__FiG6Point3(17, point);
                done = true;
            }
        }
        if (!done && word(o, 364) != 0) P_ExtendTrail__FiG6Point3(sword(o, 364), *reinterpret_cast<Point3*>(o + 16));
    }
    if (frameNm >= sword(o, 236)) delete_self(o);
}
