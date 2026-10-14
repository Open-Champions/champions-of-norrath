// Observed msg_run__18FrostStormIceShard at 0x001e1a40, 1448 bytes. O is the object, P the point (three singles) at O+16, V the point
// at O+84, frameNm the global frame counter and world the global world pointer. Pool constants with no symbols (bits): g = 0xbeced8f4
// (about -0.404), a = 0x3c088888 (about 1/120), b = 0x40825391 (about 4.0727), d = 0x3f5aa012 (about 0.854), e = 0x3f89fc08 (about
// 1.078), h = 0x412c7b0a (about 10.78). A "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16
// doubled minus 1.0 (x, y, z in that order, each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The particle definition is an
// 80-byte bss object of the image (file-local symbol froststormParticle3 0x007d6ab0), imported through an address-named view. A burst
// "(S, W)" means: a unit-ball vector scaled by S plus P is the position, a second unit-ball vector scaled by W is the velocity, and
// P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, 0.0) runs. With n = the single at O+80:
//  - When the word at O+76 is zero and frameNm is below the signed word at O+96: the single at O+92 gains g; V is added to P component
//    by component (so the new O+92 value is used); then bursts (d, e) are made for i = 0, 1, ... while (float)i < (n * a) * (n * b)
//    (tested before the first burst and after every one, with n read again). Then h0 = worldFindHeight__FP12_worldHeaderG6Point3f(world,
//    copy of P, the single at O+28); when (the single at O+24) - h0 is below 16.0 the word at O+76 becomes 1. The function ends there.
//  - Otherwise: bursts (d, h) are made for i = 0, 1, ... while (float)i < (n * n) * b (tested before the first burst and after every
//    one). Then, when O is not null, the entry at table offset 8 of the table whose address is the word at O+72 is called on O (adjusted
//    by the entry's signed halfword) with the argument 3.
// Inferred: an ice shard that flies under gravity, throwing particles, until it is close to the ground; then it bursts and ends. Unknown:
// what the words at O+76, +96, the singles at O+28, +80 and the units of the constants mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w16_spellmsg_a_view_7d6ab0[80];
int eRand__10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

static void burst(unsigned char* o, float positionScale, float velocityScale) {
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
    P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_7d6ab0, &x, &velocity, 0.0f);
}

extern "C" void msg_run__18FrostStormIceShard(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    if (word(o, 76) == 0u && frameNm < sword(o, 96)) {
        real(o, 92) = real(o, 92) + bits(0xbeced8f4u);
        real(o, 16) = real(o, 16) + real(o, 84);
        real(o, 20) = real(o, 20) + real(o, 88);
        real(o, 24) = real(o, 24) + real(o, 92);
        for (int i = 0; static_cast<float>(i) < (real(o, 80) * bits(0x3c088888u)) * (real(o, 80) * bits(0x40825391u)); ++i)
            burst(o, bits(0x3f5aa012u), bits(0x3f89fc08u));
        const Point3 at = *reinterpret_cast<Point3*>(o + 16);
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, at, real(o, 28));
        if (real(o, 24) - height < 16.0f) word(o, 76) = 1;
        return;
    }
    for (int i = 0; static_cast<float>(i) < (real(o, 80) * real(o, 80)) * bits(0x40825391u); ++i)
        burst(o, bits(0x3f5aa012u), bits(0x412c7b0au));
    __asm__("" : "+r"(o));
    if (o != nullptr) {
        unsigned char* const entry = from_word(word(o, 72)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
