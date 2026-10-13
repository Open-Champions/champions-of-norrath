// Observed msg_run__14poisonGasSpell at 0x001e0ce0, 1196 bytes. O is the object, P the point (three floats) at O+16,
// frameNm the global frame counter, E the signed word at O+92 (an end frame), n the signed word at O+96 and R the single at
// O+100. A random unit-ball vector is drawn by rejection as in the other spell sources: each component is
// eRand__10GameObject(O) * 2^-16 doubled minus 1.0, x then y then z, repeated while 1.0 < (x*x + y*y) + z*z.
//  1. When frameNm < E:
//     a. R grows by 0.5 when R < (float)n.
//     b. Repeat n/10 times (signed division, re-read from O+96 after each pass; none when n/10 <= 0): draw a unit-ball
//        vector U and set W = U * R + P; draw a second unit-ball vector V and scale it by 5.0 on every component; call
//        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6970, &W, &V, 0.0).
//     c. When frameNm % 30 == 0 and the word at O+76 (Q) is non-zero: m = (int)R; the box from P - (float)m to P + (float)m
//        is searched with objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 64). For each non-null entry X of the
//        list in order: the entry at table offset 160 of the object that is the word at Q+12 is called on that object
//        with X as the argument; when it returns non-zero, an item description is built on the stack (see spell.hpp), its
//        first entry whose first halfword is -1 or 23 gets 23, k, k and 0 where k is the low half of the word at O+108,
//        and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, null, 0.0, 0.0, 100000, 0, 0, 0x01000040,
//        description, false) runs.
//  2. Otherwise the entry at table offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with 3.
// Inferred: a lingering gas cloud that spawns particles for as long as it lives and hurts what the owner treats as an
// enemy once per 30 frames (slot 160 of the combat-reactions record is called isEnemy there). Unknown: what the words at O+92,
// +96, +100, +108 and the flag 0x01000040 mean. Nothing here ran on the EE.
// The particle definition is the 80-byte bss object at 0x007d6970 (a file-local symbol named gasParticle in the image); the
// local name cannot be linked to, so it is imported through the address-named view norrath_w13_spell_msg_a_view_7d6970.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w13_spell_msg_a_view_7d6970[80];
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, void** list, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
}

static int roll(unsigned char* o) { return eRand__10GameObject(o); }

extern "C" void msg_run__14poisonGasSpell(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (frameNm < sword(o, 92)) {
        if (real(o, 100) < static_cast<float>(sword(o, 96))) real(o, 100) = real(o, 100) + 0.5f;
        for (int pass = 0; pass < sword(o, 96) / 10; ++pass) {
            Point3 u;
            unit_ball(o, u, roll);
            const float reach = real(o, 100);
            Point3 w;
            w.x = u.x * reach;
            w.y = u.y * reach;
            w.z = u.z * reach;
            const Point3 p = *reinterpret_cast<Point3*>(o + 16);
            w.z = w.z + p.z;
            w.y = w.y + p.y;
            w.x = w.x + p.x;
            Point3 v;
            unit_ball(o, v, roll);
            v.x = v.x * 5.0f;
            v.y = v.y * 5.0f;
            v.z = v.z * 5.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w13_spell_msg_a_view_7d6970, &w, &v, 0.0f);
        }
        if (frameNm % 30 == 0 && word(o, 76) != 0) {
            const float m = static_cast<float>(static_cast<int>(real(o, 100)));
            const Point3 p = *reinterpret_cast<Point3*>(o + 16);
            Point3 low;
            Point3 high;
            high.z = p.z + m;
            high.x = p.x + m;
            high.y = p.y + m;
            low.x = p.x - m;
            low.y = p.y - m;
            low.z = p.z - m;
            void* list[64];
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 64);
            for (int i = 0; list[i] != nullptr; ++i) {
                unsigned char* const owner = from_word(word(from_word(word(o, 76)), 12));
                if (call_entry(owner, 160, to_word(list[i])) != 0) {
                    unsigned char description[80];
                    w13a::init_description(description);
                    const short k = static_cast<short>(word(o, 108));
                    w13a::fill_description(description, 23, k, k, 0);
                    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(list[i], nullptr, 0.0f, 0.0f, 100000, 0, 0, 0x01000040,
                                                                                      description, false);
                }
            }
        }
    } else {
        call_entry(o, 8, 3);
    }
}
