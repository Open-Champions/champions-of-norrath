// Observed msg_run__10PoisonRain at 0x002be6d0, 1708 bytes. O is the object, P the point (three floats) at O+16, frameNm the
// global frame counter and R the single at O+84. Pool constants with no symbols (the image keeps a separate copy for every use;
// the values of each copy are equal): A1 = bits 0x40800389 (4.00043), B = bits 0x40490fcf (3.14159), C = bits 0x3be38e38 (1/144),
// A2 = bits 0x3ffff4ec (1.99966), K1 = bits 0x3e4ccccc (0.2), K2 = bits 0x3f266666 (0.65). The two particle definitions are
// 80-byte objects in the image's bss (0x008d8190 and 0x008d81e0; file-local symbols poisonrainParticle1 and 2), imported here
// through address-named views.
// Nothing happens unless the signed word at O+80 is below frameNm.
//  1. When frameNm < the signed word at O+76 and the word at O+100 is non-zero:
//     a. Cloud drops: n1 = (R*A1)*(R*B)*C. While (float)i < n1 (i counts up from 0, the test is made before the first pass and
//        after every pass with R read again): i grows by one; draw r1, r2 and r3 with eRand__10GameObject(O) in that order;
//        a = (short)r1, t = (float)r2 * 2^-16; W = (icos__Fi(a) * (t*R), isin__Fi(a) * (t*R), 200.0) with t*R computed as t
//        times R; W.x += P.x, W.y += P.y, W.z = ((200.0 + (float)(r3 % 200 unsigned)) + -100.0) + P.z; the velocity is
//        (0.0, 0.0, -80.0); P_AddParticle__FP11ParticleDefP6Point3T1f(definition 1, &W, &velocity, 0.0).
//     b. Splashes: n2 = (R*A2)*(R*B)*C, the same loop shape: draw r1 and r2 as above and build X = (icos(a) * (t*R), isin(a) *
//        (t*R), 0.0); draw ra and rb; the velocity is (float)(ra & 1) + -1.0, (float)(rb & 1) + -1.0, 0.0); X += P; the ground
//        height h = worldFindPointHeight__FP12_worldHeaderG6Point3(world, &X) gives X.z = h + 5.0; P_AddParticle(definition 2,
//        &X, &velocity, 0.0).
//     c. When frameNm % 30 == 0: m = (int)R; the box from P - (float)m to P + (float)m is searched with
//        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 64). Let Z be the object that is the word at (O+100)+12
//        (read once, even when the list is empty). For each non-null entry E: the entry at table offset 160 of Z's table is
//        called on Z with E; when it returns non-zero, amount = the entry at offset 112 of the table that is the word at +96 of
//        the object that is the word at O+96, called on that object with (0, the word at O+92, the address O+100); then an item
//        description is built (see spell.hpp), its first entry whose first halfword is -1 or 23 gets 23, amount, amount +
//        (int)(amount * ((float)eRand__10GameObject(O) * 2^-16) * K1) and 0, and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24
//        item11descriptionb(E, Z, 0.0, 0.0, 100000, 0, 0, 64, description, false) runs.
//  2. Otherwise: soundStopSound__Fi(the word at O+104); 188 bytes are allocated with blockAlloc__Fi and a non-null block gets the
//     LightEffect constructor with Q = (P.x + 0.0, P.y, P.z + 12.0) and the floats 0.0, 255.0, 0.0, 100.0, 0.0, 255.0, 0.0, 100.0,
//     R + R, 0.0, K2, 0.5, no object and flags 1; then the entry at table offset 8 of O's table is called on O (adjusted by the
//     entry's signed halfword) with 3.
// Inferred: a poison rain effect that drops and splashes particles while it lives, hurts what the owner treats as an enemy every 30
// frames (slot 160 is called isEnemy in the combat-reactions record) and ends with a light flash. Unknown: the meaning of the words at
// O+76, +80, +92, +96, +100, +104 and of the constants. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w13_spell_msg_a_view_8d8190[80];
extern unsigned char norrath_w13_spell_msg_a_view_8d81e0[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
float worldFindPointHeight__FP12_worldHeaderG6Point3(void* world, Point3* p);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void soundStopSound__Fi(int channel);
void* blockAlloc__Fi(int size);
void lightEffect_construct(void* self, const Point3* position, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7,
                           float f8, float f9, float f10, float f11, void* object, int flags)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
}

static float rate(float r, unsigned a) {
    const float x = r * w12c::bits(a);
    const float y = r * w12c::bits(0x40490fcfu);
    return (x * y) * w12c::bits(0x3be38e38u);
}

extern "C" void msg_run__10PoisonRain(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(sword(o, 80) < frameNm)) return;
    if (frameNm < sword(o, 76) && word(o, 100) != 0) {
        const Point3 p = *reinterpret_cast<Point3*>(o + 16);
        if (0.0f < rate(real(o, 84), 0x40800389u)) {
            int i = 0;
            do {
                ++i;
                const int r1 = eRand__10GameObject(o);
                const int r2 = eRand__10GameObject(o);
                const int angle = static_cast<short>(r1);
                const float t = static_cast<float>(r2) * bits(0x37800000u);
                const float c = icos__Fi(angle);
                const float x = c * (t * real(o, 84));
                const float s = isin__Fi(angle);
                const float y = s * (t * real(o, 84));
                Point3 w;
                w.x = x;
                w.z = 200.0f;
                w.y = y;
                const unsigned r3 = static_cast<unsigned>(eRand__10GameObject(o));
                w.x = w.x + p.x;
                Point3 velocity;
                velocity.z = -80.0f;
                velocity.x = 0.0f;
                velocity.y = 0.0f;
                w.y = w.y + p.y;
                w.z = ((w.z + static_cast<float>(static_cast<int>(r3 % 200u))) + bits(0xc2c80000u)) + p.z;
                P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_8d8190, &w, &velocity, 0.0f);
            } while (static_cast<float>(i) < rate(real(o, 84), 0x40800389u));
        }
        if (0.0f < rate(real(o, 84), 0x3ffff4ecu)) {
            int i = 0;
            do {
                ++i;
                const int r1 = eRand__10GameObject(o);
                const int r2 = eRand__10GameObject(o);
                const int angle = static_cast<short>(r1);
                const float t = static_cast<float>(r2) * bits(0x37800000u);
                const float c = icos__Fi(angle);
                const float x = c * (t * real(o, 84));
                const float s = isin__Fi(angle);
                const float y = s * (t * real(o, 84));
                Point3 position;
                position.x = x;
                position.z = 0.0f;
                position.y = y;
                Point3 velocity;
                velocity.x = 0.0f;
                velocity.y = 0.0f;
                velocity.z = 0.0f;
                velocity.x = static_cast<float>(eRand__10GameObject(o) & 1) + -1.0f;
                const int rb = eRand__10GameObject(o) & 1;
                position.y = position.y + p.y;
                position.z = position.z + p.z;
                position.x = position.x + p.x;
                velocity.z = 0.0f;
                velocity.y = static_cast<float>(rb) + -1.0f;
                position.z = worldFindPointHeight__FP12_worldHeaderG6Point3(world, &position) + 5.0f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_8d81e0, &position, &velocity, 0.0f);
            } while (static_cast<float>(i) < rate(real(o, 84), 0x3ffff4ecu));
        }
        if (frameNm % 30 != 0) return;
        const float m = static_cast<float>(static_cast<int>(real(o, 84)));
        Point3 high;
        high.z = p.z + m;
        high.x = p.x + m;
        high.y = p.y + m;
        Point3 low;
        low.x = p.x - m;
        low.y = p.y - m;
        low.z = p.z - m;
        void* list[64];
        objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 64);
        unsigned char* const owner = from_word(word(from_word(word(o, 100)), 12));
        for (int e = 0; list[e] != nullptr; ++e) {
            if (call_entry(owner, 160, to_word(list[e])) == 0) continue;
            const int amount = w13a::call_entry_112(from_word(word(o, 96)), sword(o, 92), o + 100);
            unsigned char description[80];
            w13a::init_description(description);
            float spread = static_cast<float>(amount);
            spread = spread * (static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u));
            spread = spread * bits(0x3e4ccccc);
            w13a::fill_description(description, 23, static_cast<short>(amount), static_cast<short>(amount + static_cast<int>(spread)), 0);
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(list[e], owner, 0.0f, 0.0f, 100000, 0, 0, 64, description, false);
        }
        return;
    }
    soundStopSound__Fi(sword(o, 104));
    void* const block = blockAlloc__Fi(188);
    if (block != nullptr) {
        Point3 q;
        q.x = real(o, 16) + 0.0f;
        q.y = real(o, 20);
        q.z = real(o, 24) + 12.0f;
        lightEffect_construct(block, &q, 0.0f, 255.0f, 0.0f, 100.0f, 0.0f, 255.0f, 0.0f, 100.0f, real(o, 84) + real(o, 84), 0.0f,
                              bits(0x3f266666u), 0.5f, nullptr, 1);
    }
    call_entry(o, 8, 3);
}
