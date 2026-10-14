// Observed msg_run__11DiseaseBolt at 0x00292358, 3476 bytes. O is the object, P the point (three floats) at O+16 and frameNm the global
// frame counter. A "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled
// minus 1.0 (x, y, z in that order, each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The three particle definitions are 80-byte
// bss objects of the image (file-local symbols diseaseParticle1 0x008d67d0, diseaseParticle2 0x008d6820 and diseaseParticle3 0x008d6870),
// imported through address-named views. A burst "(S, V, W)" means: a unit-ball vector scaled by S on every component and added to P is the
// position, a second unit-ball vector scaled by W is the velocity, and P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position,
// velocity, 0.0) runs.
//  1. Five times burst (10.0, 16.0) with the first definition, five times (15.0, 5.0) with the second, six times (15.0, 5.0) with the third.
//  2. When the word at O+88 (a target object) is non-zero and the word at O+116 is below frameNm (signed compare): n is the entry at
//     table offset 112 of the table that is the word at +96 of the object that is the word at O+92, called on that object (adjusted by the
//     entry's signed halfword) with (1, the signed halfword at O+96, the address O+76); the turn limit is n * 182. a = iatan2__Fff(target
//     y - O y, target x - O x) with the point of the object that is the word at the target node +12 (+16); the difference d = a - (the signed halfword at O+56), compared as a 32-bit
//     value. When |d| < limit the halfword at O+56 becomes (O+56 as unsigned) + d, i.e. a; else it moves by the limit towards a (minus when
//     d <= 0, plus otherwise). The singles at O+100 and O+104 become icos__Fi(halfword) * 351.0 (bits 0x43af0000) and isin__Fi(
//     halfword) * 351.0, and the word at O+108 becomes 0.
//  3. r = move__10ProjectileG6Point3(O, the three words at O+100) . When r has bit 2 and the word at O+120 is non-zero the word at O+112
//     becomes 0; when r has bit 1 the word at O+112 becomes 0 (a bit 2 hit with the word at O+120 zero leaves it).
//  4. When frameNm is below the word at O+112 (signed) the function ends. Otherwise: soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
//     lumpFindResource__FPcT0("fx.lmp", "spel_de_disbolt_hit.anm"), O, 1.0); addEffect__FiG6Point3(12, copy of P); ten times the bursts
//     (10.0, 96.0) with the first definition, (15.0, 30.0) with the second and (15.0, 30.0) with the third; then the entry at table offset 8
//     of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a homing disease bolt that spits particles, steers toward a target, and bursts when it hits. Unknown: the meaning of the words at
// O+88, +92, +96, +112, +116, +120 and of the constants. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w13_spell_msg_a_view_8d67d0[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d6820[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d6870[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
int move__10ProjectileG6Point3(void* self, Point3 delta);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void addEffect__FiG6Point3(int kind, Point3 p);
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

extern "C" void msg_run__11DiseaseBolt(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    for (int i = 0; i < 5; ++i) burst(o, norrath_w13_spell_msg_a_view_8d67d0, 10.0f, 16.0f);
    for (int i = 0; i < 5; ++i) burst(o, norrath_w13_spell_msg_a_view_8d6820, 15.0f, 5.0f);
    for (int i = 0; i < 6; ++i) burst(o, norrath_w13_spell_msg_a_view_8d6870, 15.0f, 5.0f);
    if (word(o, 88) != 0 && sword(o, 116) < frameNm) {
        unsigned char* const entry = from_word(word(from_word(word(o, 92)), 96)) + 112;
        unsigned char* const caster = from_word(word(o, 92));
        const int limit = reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
                              caster + *reinterpret_cast<short*>(entry), 1, half(o, 96), o + 76) * 182;
        unsigned char* const target = from_word(word(o, 88));
        const int a = iatan2__Fff(real(from_word(word(target, 12)), 20) - real(o, 20), real(from_word(word(target, 12)), 16) - real(o, 16));
        const int d = a - half(o, 56);
        const int magnitude = d < 0 ? -d : d;
        const unsigned previous = static_cast<unsigned short>(half(o, 56));
        int turned;
        if (magnitude < limit) turned = previous + d;
        else if (d <= 0) turned = previous - limit;
        else turned = previous + limit;
        half(o, 56) = static_cast<short>(turned);
        real(o, 100) = icos__Fi(half(o, 56)) * bits(0x43af0000u);
        real(o, 104) = isin__Fi(half(o, 56)) * bits(0x43af0000u);
        word(o, 108) = 0;
    }
    Point3 delta = *reinterpret_cast<Point3*>(o + 100);
    const int r = move__10ProjectileG6Point3(o, delta);
    if (r & 2) {
        if (word(o, 120) != 0) word(o, 112) = 0;
    }
    if (r & 1) word(o, 112) = 0;
    if (frameNm < sword(o, 112)) return;
    void* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_de_disbolt_hit.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, o, 1.0f);
    Point3 at = *reinterpret_cast<Point3*>(o + 16);
    addEffect__FiG6Point3(12, at);
    for (int i = 0; i < 10; ++i) {
        burst(o, norrath_w13_spell_msg_a_view_8d67d0, 10.0f, 96.0f);
        burst(o, norrath_w13_spell_msg_a_view_8d6820, 15.0f, 30.0f);
        burst(o, norrath_w13_spell_msg_a_view_8d6870, 15.0f, 30.0f);
    }
    call_entry(o, 8, 3);
}
