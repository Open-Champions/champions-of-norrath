// Observed: msg_run__14FireStormFlare at 0x0029b238 is 2824 bytes in the pinned SLUS-20565 image. Argument: the object o. Nothing
// is returned. Names: pos = floats at o+16, +20, +24; vel = floats at o+96, +100, +104; S = the float at o+80 (a size); R = the
// signed halfword at +92 (read as a float); W = the word at +88 (a flag); E = the signed word at +108; frameNm and world are image
// globals. "ball" is the rejection-sampled unit-ball draw: three components v = ((float)eRand__10GameObject(o) * 2^-16) twice minus
// one, repeated while 1.0 < x*x + y*y + z*z. Float constants read through the global pointer, none with a symbol, are named by
// their bits: g = 0xbecccccc (-0.4), A1 = 0x4126b786, A2 = 0x410cccc6, A3 = 0x412430c7, B1 = 0x40a1fba8, B2 = 0x410cccc6,
// B3 = 0x40e58075, C2 = 0x410cccc6, D = 0x40d851eb. The rest are immediates.
//  Live phase (W is zero and frameNm < E):
//   1. vz = vz + g is stored (the word at +104), then pos += vel.
//   2. Three particle loops, each running i = 0, 1, ... while (float)i < S * N with N = 30, 18 and 4. An iteration takes a ball p, then a
//      ball q, and calls P_AddParticle(def, &spot, &q, 0.0) with spot = p * (S * f) + pos, where f is A2, B2 and C2 (the same value).
//      Loop 1 (def firestormParticle1, 0x008d6d70) first scales q by A1 in x, y and z and then z again by A3. Loop 2
//      (firestormParticle2, 0x008d6dc0) scales q by B1 and then z again by B3. Loop 3 (firestormParticle3, 0x008d6e10) leaves q alone.
//   3. h = worldFindHeight__FP12_worldHeaderG6Point3f(world, copy of pos, the float at o+28). When pos.z - h < 16.0, W is set to 1,
//      lumpFindResource("fx.lmp", "spel_ew_flares_hit.anm") is looked up and played with soundPlaySequence(resource, o, 1.0).
//  Otherwise (W nonzero or frameNm >= E):
//   4. When (o[28] * 3.0) * S is above (float)R, R is raised by 7 (16-bit unsigned add) and 85 times: with r = eRand(o), the point
//      (icos(r) * R, isin(r) * R, 0) (R re-read each time) has a ball scaled by D added, then pos; a second ball is the motion;
//      P_AddParticle(firestormParticle1, &point, &motion, 0.0) and P_AddParticle(firestormParticle2, &point, &motion, 0.0) follow.
//   5. Otherwise, when eRand(o) % 100 (unsigned) is below 33, pos.z is lowered by 6.0 and floorAdd(pos, (short)eRand(o), S, 9, 0)
//      runs. Then the entry at offset 8 of the table whose address is the word at +72 is called on o (adjusted by the entry's
//      signed halfword) with 3.
// Inferred: the per-frame handler of the flares of a fire storm, from its symbol and strings. Unknown: the meaning of W, E, R and the
// numeric constants. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char firestormParticle1[], firestormParticle2[], firestormParticle3[];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float f);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float volume);
void floorAdd__FG6Point3sfii(Point3 position, short a, float scale, int b, int c);

void msg_run__14FireStormFlare(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    auto roll = [](unsigned char* object) { return eRand__10GameObject(object); };
    if (word(o, 88) == 0u && frameNm < sword(o, 108)) {
        real(o, 104) = real(o, 104) + bits(0xbecccccc);
        real(o, 16) = real(o, 16) + real(o, 96);
        real(o, 20) = real(o, 20) + real(o, 100);
        real(o, 24) = real(o, 24) + real(o, 104);
        for (int i = 0; static_cast<float>(i) < real(o, 80) * 30.0f; ++i) {
            Point3 p, q;
            unit_ball(o, p, roll);
            const float f = real(o, 80) * bits(0x410cccc6u);
            p.z = p.z * f; p.x = p.x * f; p.y = p.y * f;
            p.z = p.z + real(o, 24); p.x = p.x + real(o, 16); p.y = p.y + real(o, 20);
            unit_ball(o, q, roll);
            q.z = q.z * bits(0x4126b786u); q.x = q.x * bits(0x4126b786u); q.y = q.y * bits(0x4126b786u);
            q.z = q.z * bits(0x412430c7u);
            P_AddParticle__FP11ParticleDefP6Point3T1f(firestormParticle1, &p, &q, 0.0f);
        }
        for (int i = 0; static_cast<float>(i) < real(o, 80) * 18.0f; ++i) {
            Point3 p, q;
            unit_ball(o, p, roll);
            const float f = real(o, 80) * bits(0x410cccc6u);
            p.z = p.z * f; p.x = p.x * f; p.y = p.y * f;
            p.z = p.z + real(o, 24); p.x = p.x + real(o, 16); p.y = p.y + real(o, 20);
            unit_ball(o, q, roll);
            q.z = q.z * bits(0x40a1fba8u); q.x = q.x * bits(0x40a1fba8u); q.y = q.y * bits(0x40a1fba8u);
            q.z = q.z * bits(0x40e58075u);
            P_AddParticle__FP11ParticleDefP6Point3T1f(firestormParticle2, &p, &q, 0.0f);
        }
        for (int i = 0; static_cast<float>(i) < real(o, 80) * 4.0f; ++i) {
            Point3 p, q;
            unit_ball(o, p, roll);
            const float f = real(o, 80) * bits(0x410cccc6u);
            p.z = p.z * f; p.x = p.x * f; p.y = p.y * f;
            p.z = p.z + real(o, 24); p.x = p.x + real(o, 16); p.y = p.y + real(o, 20);
            unit_ball(o, q, roll);
            P_AddParticle__FP11ParticleDefP6Point3T1f(firestormParticle3, &p, &q, 0.0f);
        }
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, *reinterpret_cast<Point3*>(o + 16), real(o, 28));
        if (real(o, 24) - height < 16.0f) {
            word(o, 88) = 1u;
            void* const resource = lumpFindResource__FPcT0("fx.lmp", "spel_ew_flares_hit.anm");
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(resource, self, 1.0f);
        }
        return;
    }
    if (static_cast<float>(half(o, 92)) < real(o, 28) * 3.0f * real(o, 80)) {
        half(o, 92) = static_cast<short>(static_cast<unsigned short>(half(o, 92)) + 7u);
        for (int n = 0; n < 85; ++n) {
            const int r = eRand__10GameObject(self);
            Point3 point;
            point.x = icos__Fi(r) * static_cast<float>(half(o, 92));
            point.y = isin__Fi(r) * static_cast<float>(half(o, 92));
            point.z = 0.0f;
            Point3 ball;
            unit_ball(o, ball, roll);
            point.x = point.x + ball.x * bits(0x40d851ebu);
            point.y = point.y + ball.y * bits(0x40d851ebu);
            point.z = point.z + ball.z * bits(0x40d851ebu);
            point.x = point.x + real(o, 16);
            point.y = point.y + real(o, 20);
            point.z = point.z + real(o, 24);
            Point3 motion;
            unit_ball(o, motion, roll);
            P_AddParticle__FP11ParticleDefP6Point3T1f(firestormParticle1, &point, &motion, 0.0f);
            P_AddParticle__FP11ParticleDefP6Point3T1f(firestormParticle2, &point, &motion, 0.0f);
        }
        return;
    }
    if (static_cast<unsigned>(eRand__10GameObject(self)) % 100u < 33u) {
        real(o, 24) = real(o, 24) - 6.0f;
        const short angle = static_cast<short>(eRand__10GameObject(self));
        floorAdd__FG6Point3sfii(*reinterpret_cast<Point3*>(o + 16), angle, real(o, 80), 9, 0);
    }
    unsigned char* const entry = from_word(word(o, 72)) + 8;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W12_SPELL_C_HOST)
    w12_spell_c_host_call(function, o + *reinterpret_cast<short*>(entry), 1, 3, 0);
#else
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(o + *reinterpret_cast<short*>(entry), 3);
#endif
}
}
