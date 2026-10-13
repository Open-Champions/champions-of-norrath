// Observed msg_run__10MummySnake at 0x0023b6c0, 1448 bytes. O is the object, P the point (three floats) at O+16, H the
// halfword at O+84 (a heading), A the halfword at O+92, frameNm the global frame counter, T the word at O+80 (a list head whose
// first node is the target object W = the word at T+12; "flags" of an object is its word at +12). One pool constant with no
// symbol: K = bits 0x400aaaaa (2.1667).
//  1. A new target is looked up when T is zero or W's flags have bit 8: getClosestLivingPlayer__FG6Point3f(copy of P, 1000.0)
//     gives a player index n; unless it is -1, the word at (player + n*23984 + 21872) is read, and when non-zero
//     listOnewayLink__FP8ListHeadT0Pv(O+80, w, w) links it. T is read again afterwards.
//  2. When T is zero, H grows by 150 (16 bits). Otherwise iatan2__Fff(W.y - P.y, W.x - P.x) gives an angle a; H changes by -500 when
//     (short)(a - H) <= 0 and by +500 otherwise; then the single at O+24 (P.z) moves by 1.0 towards W.z + W's single at +32 times
//     0.5 (it is lowered when that is below P.z, raised when above, left alone when equal).
//  3. A grows by 3400 (16 bits). D = (icos__Fi(H) * K, isin__Fi(H) * K, 0.0) is added to P.x and P.y (in the object). Q is a copy of
//     the new P and V is (0, 0, 0); u = isin__Fi(A) * 5.0; Q.x += icos__Fi(H + 16384) * u and Q.y += isin__Fi(H + 16384) * u (the
//     trig calls run in the order icos(H), isin(H), icos(H+16384), isin(A), isin(H+16384), isin(A)); then
//     P_AddParticle__FP11ParticleDefP6Point3T1f(SnakeParticle, &Q, &V, 0.0) runs.
//  4. Done is set (s5 below) in these cases, tested in order: T is non-zero and the word Y at O+76 is non-zero and the distance from P
//     to (W.x, W.y, W.z + W[+32]*0.5) is below W's single at +28 plus 5.0: an item description is built (see spell.hpp); the
//     amount is the entry at offset 96 of the table that is the word at +96 of the object that is the word at skills+104, called on
//     it with (0, (int)worldDifficulty); the first description entry whose first halfword is -1 or 23 gets 23, amount, amount and
//     0; gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(W, the object that is the word at Y+12, 50.0, 50.0,
//     10000, 0, 0, 0x10000, description, false) runs; then Done is set, and soundPlaySequence__FP15AnimationHeaderP10GameObjectf
//     (the word at O+96, O, 1.0) runs when that word is non-zero. Done is also set when Y is zero, when Y's object has flags with bit 8
//     (the object that is the word at Y+12), and when the signed word at O+88 is below frameNm (the last two are tested only when
//     Y is non-zero; the bit-8 test comes first).
//  5. When Done: E is a copy of P with E.x += icos(H + 16384) * (isin(A) * 5.0) and E.y += isin(H + 16384) * (isin(A) * 5.0) (calls in the
//     order icos(H+16384), isin(A), isin(H+16384), isin(A)); addEffect__FiG6Point3 runs six times with 14 and once with 3, all with E,
//     and objectAddToDeleteList__FP10GameObject(O) ends the function.
// Inferred: a snake that chases the nearest living player, spits particles, bites for a fixed attack when in reach and dies when its
// owner is gone or its time is up. Unknown: the meaning of the words at O+76, +80, +88, +96, the halfwords and the constants.
// Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern unsigned char player[];
extern unsigned char skills[];
extern unsigned char SnakeParticle[80];
extern float worldDifficulty;
int getClosestLivingPlayer__FG6Point3f(Point3 position, float radius);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
int iatan2__Fff(float y, float x);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void addEffect__FiG6Point3(int kind, Point3 p);
void objectAddToDeleteList__FP10GameObject(void* object);
}

extern "C" void msg_run__10MummySnake(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const position = o + 16;
    if (word(o, 80) == 0 || (word(from_word(word(from_word(word(o, 80)), 12)), 12) & 8u) != 0) {
        Point3 copy = *reinterpret_cast<Point3*>(position);
        const int n = getClosestLivingPlayer__FG6Point3f(copy, 1000.0f);
        if (n != -1) {
            const unsigned found = word(player + n * 23984, 21872);
            if (found != 0) listOnewayLink__FP8ListHeadT0Pv(o + 80, from_word(found), from_word(found));
        }
    }
    if (word(o, 80) == 0) {
        half(o, 84) = static_cast<short>(static_cast<unsigned short>(half(o, 84)) + 150);
    } else {
        unsigned char* const w = from_word(word(from_word(word(o, 80)), 12));
        const float dy = real(w, 20) - real(o, 20);
        const int angle = iatan2__Fff(dy, real(w, 16) - real(o, 16));
        const unsigned short heading = static_cast<unsigned short>(half(o, 84));
        if (static_cast<short>(angle - heading) <= 0) half(o, 84) = static_cast<short>(heading - 500);
        else half(o, 84) = static_cast<short>(heading + 500);
        unsigned char* const target = from_word(word(from_word(word(o, 80)), 12));
        const float z = real(o, 24);
        const float center = real(target, 24) + real(target, 32) * 0.5f;
        if (center < z) real(o, 24) = z - 1.0f;
        else if (z < center) real(o, 24) = z + 1.0f;
    }
    half(o, 92) = static_cast<short>(static_cast<unsigned short>(half(o, 92)) + 3400);
    const float cosine = icos__Fi(half(o, 84));
    const float scaledCos = cosine * bits(0x400aaaaau);
    const float sine = isin__Fi(half(o, 84));
    const float scaledSin = sine * bits(0x400aaaaau);
    Point3 drift;
    drift.x = scaledCos;
    drift.z = 0.0f;
    drift.y = scaledSin;
    real(o, 16) = real(o, 16) + drift.x;
    real(o, 20) = real(o, 20) + drift.y;
    Point3 q = *reinterpret_cast<Point3*>(position);
    Point3 v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    float side = icos__Fi(half(o, 84) + 16384);
    float spread = isin__Fi(half(o, 92)) * 5.0f;
    q.x = q.x + side * spread;
    side = isin__Fi(half(o, 84) + 16384);
    spread = isin__Fi(half(o, 92)) * 5.0f;
    q.y = q.y + side * spread;
    P_AddParticle__FP11ParticleDefP6Point3T1f(SnakeParticle, &q, &v, 0.0f);
    bool done = false;
    if (word(o, 80) != 0) {
        if (word(o, 76) != 0) {
            unsigned char* const w = from_word(word(from_word(word(o, 80)), 12));
            Point3 u = *reinterpret_cast<Point3*>(w + 16);
            u.z = u.z + real(w, 32) * 0.5f;
            const float dx = u.x - real(o, 16);
            const float dy = u.y - real(o, 20);
            const float dz = u.z - real(o, 24);
            const float distance = __builtin_sqrtf((dx * dx + dy * dy) + dz * dz);
            if (distance < real(w, 28) + 5.0f) {
                unsigned char description[80];
                w13a::init_description(description);
                unsigned char* const holder = from_word(word(skills, 104));
                const int amount = static_cast<int>(call_entry_from(holder, 96, 96, 0, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
                w13a::fill_description(description, 23, static_cast<short>(amount), static_cast<short>(amount), 0);
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                    from_word(word(from_word(word(o, 80)), 12)), from_word(word(from_word(word(o, 76)), 12)), 50.0f, 50.0f, 10000, 0, 0, 0x10000,
                    description, false);
                done = true;
                if (word(o, 96) != 0) soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(o, 96)), o, 1.0f);
            }
        } else {
            done = true;
        }
    }
    if (word(o, 76) == 0) {
        done = true;
    } else if ((word(from_word(word(from_word(word(o, 76)), 12)), 12) & 8u) != 0) {
        done = true;
    } else if (sword(o, 88) < frameNm) {
        done = true;
    }
    if (!done) return;
    Point3 effect = *reinterpret_cast<Point3*>(position);
    float c = icos__Fi(half(o, 84) + 16384);
    float u2 = isin__Fi(half(o, 92)) * 5.0f;
    effect.x = effect.x + c * u2;
    c = isin__Fi(half(o, 84) + 16384);
    u2 = isin__Fi(half(o, 92)) * 5.0f;
    effect.y = effect.y + c * u2;
    for (int i = 0; i < 6; ++i) addEffect__FiG6Point3(14, effect);
    addEffect__FiG6Point3(3, effect);
    objectAddToDeleteList__FP10GameObject(o);
}
