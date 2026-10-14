// Observed msg_run__8HolyBolt at 0x002bacd0, 1568 bytes. O is the object, P the point (three singles) at O+16, frameNm the global frame
// counter and world the global world pointer. Pool constants with no symbols (bits): m = 0x3f410624 (about 0.754) and s = 0x42666666
// (57.6). A "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in that
// order, each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The particle definition is an 80-byte bss object of the image
// (file-local symbol skullHitParticle 0x008d8000), imported through an address-named view. Steps in order:
//  1. When the word at O+100 (the target T) is nonzero: the singles O+104, O+108 and O+112 become T.x, T.y and T.z + 36.0 (T's singles
//     at +16, +20, +24); a = iatan2__Fff(T.y - O.y, T.x - O.x); with h the unsigned halfword at O+56 and d = (short)(a - h), the halfword
//     at O+56 becomes (short)(d + h) when |d| < 1092, (h - 1092) when d <= 0 otherwise, and (h + 1092) for the remaining case; then, when
//     the word at (T's word at O+100) + 12 has bit 3 (8) set, the word at O+100 becomes 0.
//  2. With g the signed halfword at O+56: the single at O+88 becomes icos__Fi(g) * 240.0 and then the single at O+92 becomes
//     isin__Fi(g) * 240.0. With z = (short)(int)(the single at O+112 - the single at O+24): the word at O+96 becomes 0 when
//     -9 <= z <= 9, otherwise the single 50.0 for z > 0 and -50.0 for z <= 0.
//  3. r = move__10ProjectileG6Point3(O, a copy of the point at O+88). A = (O+92, -(O+88), 0.0) is normalised (squared length x*x + y*y,
//     then + z*z; components multiplied by rsqrt.s of it) and multiplied by icos__Fi(frameNm * 6880) * m; B = (0, 0, 1) is multiplied by
//     isin__Fi(frameNm * 6880) * m (the sine call after the cosine one). The position Q is ((A + B) + P). With c = icos__Fi(g) * 10.0 and
//     then e = isin__Fi(g) * 10.0 (g re-read from O+56), P_ExtendTrail__FiG6Point3(the word at O+180, (Q.x + c, Q.y + e, Q.z)) is called.
//  4. When r is exactly 1: a roll R with the halfword at O+56 becomes (unsigned short)(h + R % 32767 - 16384) (unsigned remainder); ten times,
//     a roll gives g2 = (short)(the halfword at O+56 + roll % 32767 - 16384), U = (P.x + icos__Fi(g2) * 24.0, P.y + isin__Fi(g2) * 24.0,
//     P.z) is formed and worldHitscan__FP12_worldHeaderG6Point3T1fi(world, copy of P, U - P, 2.0, 0) is asked; when the result is not above
//     0.0 the halfword at O+56 becomes g2. Then lumpFindResource__FPcT0("fx.lmp", "spel_he_dismiss_bnce.anm") is played with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, O, 1.0), and 25 particles are added: a unit-ball vector scaled by s,
//     P_AddParticle__FP11ParticleDefP6Point3T1f(definition, the address O+16, the scaled vector, 0.0).
//  5. soundPan2D__FiP6Point3f(the word at O+176, the address O+16, 1.0) is called; P is copied to O+164 (twelve bytes);
//     lightUpdatePosition__FP5Light is called on O+116. When frameNm is not below the signed word at O+84 and O is not null, the entry at
//     table offset 8 of the table whose address is the word at O+72 is called on O (adjusted by its signed halfword) with 3.
// Inferred: a homing holy bolt that steers toward a target, leaves a trail and, when it stops on something, scatters particles and makes
// a sound. Unknown: what the words at O+84, +100, +176, +180 and the singles at O+104..O+112 mean beyond their use here. Nothing here ran
// on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w16_spellmsg_a_view_8d8000[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void P_ExtendTrail__FiG6Point3(int trail, Point3 p);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, Point3 from, Point3 direction, float radius, int mode);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void soundPan2D__FiP6Point3f(int sound, Point3* position, float volume);
void lightUpdatePosition__FP5Light(void* light);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

extern "C" void msg_run__8HolyBolt(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    unsigned char* const target = from_word(word(o, 100));
    if (target != nullptr) {
        real(o, 104) = real(target, 16);
        real(o, 108) = real(target, 20);
        real(o, 112) = real(target, 24) + 36.0f;
        const int a = iatan2__Fff(real(target, 20) - real(o, 20), real(target, 16) - real(o, 16));
        const int h = *reinterpret_cast<unsigned short*>(o + 56);
        const int d = static_cast<short>(a - h);
        const int ad = d < 0 ? -d : d;
        int next;
        if (ad < 1092) next = d + h;
        else if (d <= 0) next = h - 1092;
        else next = h + 1092;
        half(o, 56) = static_cast<short>(next);
        if ((word(from_word(word(o, 100)), 12) & 8u) != 0u) word(o, 100) = 0;
    }
    real(o, 88) = icos__Fi(half(o, 56)) * 240.0f;
    real(o, 92) = isin__Fi(half(o, 56)) * 240.0f;
    const short dz = static_cast<short>(static_cast<int>(real(o, 112) - real(o, 24)));
    if (static_cast<unsigned short>(dz + 9) < 19u) word(o, 96) = 0;
    else real(o, 96) = dz > 0 ? 50.0f : -50.0f;
    const Point3 delta = *reinterpret_cast<Point3*>(o + 88);
    const int r = move__10ProjectileG6Point3(o, delta);
    Point3 a = {real(o, 92), -real(o, 88), zero};
    Point3 b = {zero, zero, 1.0f};
    float inv;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(inv) : "f"(one), "f"(a.x * a.x + a.y * a.y + a.z * a.z));
    a.x = a.x * inv;
    a.y = a.y * inv;
    a.z = a.z * inv;
    const float k1 = icos__Fi(frameNm * 6880) * bits(0x3f410624u);
    a.x = a.x * k1;
    a.y = a.y * k1;
    a.z = a.z * k1;
    const float k2 = isin__Fi(frameNm * 6880) * bits(0x3f410624u);
    b.x = b.x * k2;
    b.y = b.y * k2;
    b.z = b.z * k2;
    Point3 q = {a.x + b.x, a.y + b.y, a.z + b.z};
    q.x = q.x + real(o, 16);
    q.y = q.y + real(o, 20);
    q.z = q.z + real(o, 24);
    const float c = icos__Fi(half(o, 56)) * 10.0f;
    const float e = isin__Fi(half(o, 56)) * 10.0f;
    const Point3 trail = {q.x + c, q.y + e, q.z};
    P_ExtendTrail__FiG6Point3(static_cast<int>(word(o, 180)), trail);
    if (r == 1) {
        half(o, 56) = static_cast<short>(*reinterpret_cast<unsigned short*>(o + 56) + static_cast<unsigned>(eRand__10GameObject(o)) % 32767u - 16384u);
        for (int i = 9; i >= 0; --i) {
            const short g2 = static_cast<short>(*reinterpret_cast<unsigned short*>(o + 56) + static_cast<unsigned>(eRand__10GameObject(o)) % 32767u - 16384u);
            const float ux = real(o, 16) + icos__Fi(g2) * 24.0f;
            const float uy = real(o, 20) + isin__Fi(g2) * 24.0f;
            const float uz = real(o, 24);
            const Point3 direction = {ux - real(o, 16), uy - real(o, 20), uz - real(o, 24)};
            const Point3 from = *reinterpret_cast<Point3*>(o + 16);
            if (!(0.0f < worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, 2.0f, 0))) half(o, 56) = g2;
        }
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_dismiss_bnce.anm")), o, 1.0f);
        for (int i = 0; i < 25; ++i) {
            Point3 u;
            unit_ball(o, u, roll);
            Point3 velocity = {u.x * bits(0x42666666u), u.y * bits(0x42666666u), u.z * bits(0x42666666u)};
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d8000, reinterpret_cast<Point3*>(o + 16), &velocity, zero);
        }
    }
    soundPan2D__FiP6Point3f(static_cast<int>(word(o, 176)), reinterpret_cast<Point3*>(o + 16), 1.0f);
    *reinterpret_cast<Point3*>(o + 164) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 116);
    const int end = sword(o, 84);
    __asm__("" : "+r"(o));
    if (!(frameNm < end) && o != nullptr) {
        unsigned char* const entry = from_word(word(o, 72)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
