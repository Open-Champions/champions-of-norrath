// Observed msg_run__10FrostShard at 0x002bbe98, 1676 bytes. O is the object, P the point (three singles) at O+16, frameNm the global
// frame counter. Pool constants with no symbols (bits): K1 = 0x41109750 (about 9.037), K2 = 0x41f995ed (about 31.198), K3 = 0x40dfff27
// (about 6.9999), K4 = 0x41800003 (about 16.000006), K6 = 0x3e999999 (0.3), K7 = 0x3fdba5e3 (1.716), K8 = 0x3f199999 (0.6). A
// "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in that order,
// each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The two particle definitions are 80-byte bss objects of the image
// (file-local symbols shockoffrostParticle1 0x008d80a0 and shockoffrostParticle2 0x008d80f0, shared with the shock-of-frost spell),
// imported through address-named views. A burst "(D, S, W)" means: a unit-ball vector scaled by S plus P is the position; a second
// unit-ball vector scaled by W is the velocity; P_AddParticle__FP11ParticleDefP6Point3T1f(D, position, velocity, 0.0) runs. Steps:
//  1. Thirty bursts (first definition, K1, K2), then twenty bursts (second definition, K3, K4).
//  2. The signed word at O+92 drops by 2000. r = move__10ProjectileG6Point3(O, the point at O+96). When r has bit 2 and the word at
//     O+112 is nonzero the word at O+108 becomes 0; when r has bit 1 it becomes 0. The function ends when frameNm is below the signed
//     word at O+108.
//  3. Otherwise: with k = (eRand__10GameObject(O) % 3, unsigned) + 1, the name sprintf("expl_ice%d.anm", k) is looked up with
//     lumpFindResource__FPcT0("fx.lmp", name) and played with soundPlaySequence__FP15AnimationHeaderG6Point3f(header, copy of P,
//     1.0); then addEffect__FiG6Point3(10, copy of P).
//  4. Then, for i from 0 while i < (int)(((float)eRand__10GameObject(O) * 2^-16) * 4.0) (the roll is drawn again before every test): 584
//     bytes come from blockAlloc__Fi and, when non-null, the PhProp constructor __6PhPropG6Point3PcN22liPQ24item11descriptionf runs
//     on it with (copy of P, "fx.lmp", "iceshard.vif", "iceshard.tex", kind 1 << i, flags 274, null description, 1.0); the single at
//     +252 becomes (old * K6) + ((the single at O+96 * K6) * K7), the single at +256 becomes (old * K6) + ((the single at O+100 * K6) * K7),
//     the single at +48 becomes old * K8, and initStuff__6PhPropi(block, 274) runs.
//  5. Finally, when O is not null, the entry at table offset 8 of the table whose address is the word at O+72 is called on O (adjusted
//     by the entry's signed halfword) with 3.
// Inferred: a flying frost shard that sprays frost particles, and at the end of its life plays a sound and spawns ice chunks. Unknown:
// what the words at O+92..O+112 mean and what the flag word 274 and the kind 1 << i select. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w13_spell_msg_a_view_8d80a0[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d80f0[80];
int eRand__10GameObject(void* object);
int image_sprintf(char* destination, const char* format, ...) __asm__("sprintf");
int move__10ProjectileG6Point3(void* self, Point3 delta);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);
void addEffect__FiG6Point3(int kind, Point3 p);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long kind, int flags, void* description, float scale);
void initStuff__6PhPropi(void* self, int flags);
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

extern "C" void msg_run__10FrostShard(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    for (int i = 0; i < 30; ++i) burst(o, norrath_w13_spell_msg_a_view_8d80a0, bits(0x41109750u), bits(0x41f995edu));
    for (int i = 0; i < 20; ++i) burst(o, norrath_w13_spell_msg_a_view_8d80f0, bits(0x40dfff27u), bits(0x41800003u));
    sword(o, 92) = sword(o, 92) - 2000;
    const Point3 delta = *reinterpret_cast<Point3*>(o + 96);
    const int r = move__10ProjectileG6Point3(o, delta);
    if (r & 2) {
        if (word(o, 112) != 0u) word(o, 108) = 0;
    }
    if (r & 1) word(o, 108) = 0;
    if (frameNm < sword(o, 108)) return;
    char name[32];
    image_sprintf(name, "expl_ice%d.anm", static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 3u) + 1);
    void* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name);
    const Point3 at = *reinterpret_cast<Point3*>(o + 16);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(header, at, 1.0f);
    const Point3 effectAt = *reinterpret_cast<Point3*>(o + 16);
    addEffect__FiG6Point3(10, effectAt);
    for (int i = 0; i < static_cast<int>((static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u)) * 4.0f); ++i) {
        unsigned char* const block = static_cast<unsigned char*>(blockAlloc__Fi(584));
        if (block != nullptr) {
            Point3 where = *reinterpret_cast<Point3*>(o + 16);
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(block, where, const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.vif"),
                                                           const_cast<char*>("iceshard.tex"), 1L << i, 274, nullptr, 1.0f);
            real(block, 252) = real(block, 252) * bits(0x3e999999u) + (real(o, 96) * bits(0x3e999999u)) * bits(0x3fdba5e3u);
            real(block, 48) = real(block, 48) * bits(0x3f199999u);
            real(block, 256) = real(block, 256) * bits(0x3e999999u) + (real(o, 100) * bits(0x3e999999u)) * bits(0x3fdba5e3u);
            initStuff__6PhPropi(block, 274);
        }
    }
    __asm__("" : "+r"(o));
    if (o != nullptr) {
        unsigned char* const entry = from_word(word(o, 72)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
