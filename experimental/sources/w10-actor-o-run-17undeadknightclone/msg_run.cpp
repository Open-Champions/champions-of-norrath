// Observed msg_run__17UndeadKnightClone at 0x0023e0f0, 4992 bytes. R is the block at the word object+36,
// s the signed halfword at +572 (the state), b the unsigned halfword at +294, e the word at +576, wait
// the word at +256, base the word at +808, a the value of creatureAdvanceAnimation, f the float at +28,
// wind the float at +228, pos the point at +16, anim the animation state at +288. "setState(n)" is the
// table entry at (word +72) + 184 called with n; "zone(d, k)" is enemyInZone(this, d, k) as a boolean;
// "melee" is produceMeleeDamage(this, 36.0, h200, h202, h204, 0x40008, null) with the signed halfwords at
// +200, +202, +204; "model" is the table entry at (word +72) + 128 called on this object. A "ball
// point" is three components (float)eRand(this) * 2^-16, doubled, minus 1.0 in x, y, z order, redrawn
// while 1.0 < x*x + y*y + z*z.
// Part 1: with the word at +2556 non-zero: fxRunWeapon(the description at +2624, R[180], the weapon state
// at +2560, a copy of pos, the halfword at +56, R[12], anim, 0.5, 1 when s is 6 or 13 (else 0), 0, 1.0,
// null matrix). newDamageHandler(this, R[12]); everyFrame(this, R[12]) non-zero ends the call;
// objectActivationCheck(this, 600) (its result is not used).
// Part 2: a = creatureAdvanceAnimation(this, 1.0); slideBack(this, 0.5); the table entry at +168 is
// called; move2(this, {0, 0, 0}); the point {pos.x + 0.0, pos.y, pos.z + 84.0} is copied to +2544 and
// lightUpdatePosition(the Light at +2496) is called.
// Part 3, by state (a state outside 0..17, and 2, 4, 6, 7, 8, 9, 11, 12, setState(3)):
//   0: zone(400, 32767) or zone(500, 16000): setState(3); otherwise setState(1) when wait < base.
//   1: zone(400, 32767) or zone(500, 16000): setState(0); otherwise smartWander.
//   3: e zero: setState(0). Otherwise zone(f + 60, 23300): setState(16) when eRand % 100 < 65, else 14;
//      zone(f + 80, 11000): 14 when eRand % 100 < 65, else 15; zone(f + 80, 6000): 13 when
//      eRand % 100 < 65, else 15; zone(f + 120, 3000): steerAndRouteToEnemy when frameNm < wait (signed),
//      else setState(17); otherwise zone(300, 32767) or a nonzero strcmp of the text at levelName + 81
//      with the level text "battle3": steerAndRouteToEnemy; equal: setState(10).
//   5: zone(f + 40, 6000): setState(6); otherwise bExitStatePersueIdle non-zero: setState(3).
//   13: e non-zero: melee when a & 7 is non-zero, then turnToFaceEnemy; then setState(3) when b bit 3
//      is clear.
//   14..17: e non-zero: melee when a & 7 is non-zero, then turnToFaceEnemy. Then, when b bit 3 is
//      clear, wait is decremented; when wait is not positive, setState(3).
//   10: "scatter" 500 times (particle 0x7d7ed0, speed 24.0, divisor 0 as in the image, whose divide has
//      no zero check and leaves the dividend in the remainder), then "burst" (particle 0x7d7ed0,
//      speed 24.0, constants 1e-5, 8e11, 35.35), then, when frameNm is not below wait (signed): the
//      word at +2556 becomes 0; a matrix is taken (modelGetCharAttachmentMatrix(udkResourceMap[3],
//      copy of pos, h56, anim, 0, &M, wind-less float at +224)) and, when a 584-byte block is non-null,
//      PhProp is constructed (zero point, M, "udk.lmp", "sword.vif", "sword.tex", 1, 16641, null, the
//      float at +224); the word at +2696 becomes 0; the same with index 1 and "shield.vif",
//      "shield.tex"; then objectAddToDeleteList(this) and objectRemoveFromRunList(this).
// Part 4, after every state: "scatter" 100 times with the shadow-point count of this object's model as
// the divisor (particle 0x7d7e80, speed 12.0), then "burst" (particle 0x7d7e80, speed 12.0, constants
// 1e-5, 1.6e12, 35.35).
// Scatter round: model; r = eRand; modelGetShadowPos(model, anim, copy of pos, h56, r % divisor, &P,
// &w, null, the float at +224); P += 10 * ball point; v = 24 or 12 times a second ball point;
// P_AddParticle(particle, &P, &v, 0.0).
// Burst: points = modelGetNmShadowPoints(model); acc = 0; for each index i below points: model;
// modelGetShadowPos(model, anim, copy of pos, h56, i, &P, &w, null, the float at +224); acc += w * w *
// w; while 0 < acc: acc -= cut; scale = (w * k0) * k1; p = (r1 * 2^-16 - 0.5, r2 * 2^-16 - 0.5,
// r3 * 2^-16 - 0.5) * scale (three eRand calls) + P; v = speed * ball point; when wind is non-zero p +=
// {cos(h56) * wind, sin(h56) * wind, 0}; P_AddParticle(particle, &p, &v, 0.0).
// The particle definitions are file-local objects of the original unit, named here by address.
// Meanings of the states and fields are unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h,
    float i, void* matrix);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int value);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int move2__8CreatureG6Point3(void* self, Point3 push);
void lightUpdatePosition__FP5Light(void* light);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void smartWander__8Creature(void* self);
unsigned eRand__10GameObject(void* object);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* out, float* weight, void* matrix, float scale);
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
    void* model, Point3 point, short yaw, void* state, int index, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(void* self, Point3 position, void* matrix, char* a, char* b, char* c, long d, int e,
                                                               void* description, float scale);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
int strcmp(const char* a, const char* b);
extern int frameNm;
extern char levelName[];
extern unsigned udkResourceMap[];
extern unsigned char undead_knight_clone_msg_run_particle_7d7e80[];
extern unsigned char undead_knight_clone_msg_run_particle_7d7ed0[];
}

namespace {
// The image divides without the zero-divisor trap the compiler would add to a plain `%`.
__attribute__((always_inline)) inline int signed_remainder(int value, int divisor) {
#if defined(__mips__)
    int remainder;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(remainder) : "r"(value), "r"(divisor) : "hi", "lo");
    return remainder;
#else
    return divisor == 0 ? value : value % divisor;
#endif
}

inline float component(void* object) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * w10o::bits(0x37800000u);
    return v + v - 1.0f;
}

inline void ball(void* object, Point3& p) {
    do {
        p.x = component(object);
        p.y = component(object);
        p.z = component(object);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}

void scatter(unsigned char* o, int rounds, void* particle, float speed, bool use_points) {
    using namespace w10o;
    void* const self = o;
    void* const anim = o + 288;
    int divisor = 0;
    if (use_points) divisor = modelGetNmShadowPoints__FP12_modelHeader(from_word(call_entry(o, 128)));
    Point3 out;
    float weight;
    for (int i = 0; i < rounds; ++i) {
        void* const model = from_word(call_entry(o, 128));
        const int index = signed_remainder(static_cast<int>(eRand__10GameObject(self)), divisor);
        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, anim, position, half(o, 56), index, &out, &weight, nullptr, real(o, 224));
        Point3 first;
        ball(self, first);
        const Point3 push = {first.x * 10.0f, first.y * 10.0f, first.z * 10.0f};
        out.x = out.x + push.x;
        out.y = out.y + push.y;
        out.z = out.z + push.z;
        Point3 second;
        ball(self, second);
        Point3 velocity = {second.x * speed, second.y * speed, second.z * speed};
        P_AddParticle__FP11ParticleDefP6Point3T1f(particle, &out, &velocity, 0.0f);
    }
}

void burst(unsigned char* o, void* particle, float speed, unsigned k0, unsigned cut, unsigned k1) {
    using namespace w10o;
    void* const self = o;
    void* const anim = o + 288;
    float accumulated = 0.0f;
    const int points = modelGetNmShadowPoints__FP12_modelHeader(from_word(call_entry(o, 128)));
    for (int i = 0; i < points; ++i) {
        void* const model = from_word(call_entry(o, 128));
        Point3 where;
        float weight;
        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, anim, position, half(o, 56), i, &where, &weight, nullptr, real(o, 224));
        accumulated = accumulated + weight * weight * weight;
        if (!(0.0f < accumulated)) continue;
        do {
            accumulated = accumulated - bits(cut);
            const float scale = (weight * bits(k0)) * bits(k1);
            const float r1 = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float r2 = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float r3 = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            Point3 p;
            p.x = (r1 * bits(0x37800000u) - 0.5f) * scale;
            p.y = (r2 * bits(0x37800000u) - 0.5f) * scale;
            p.z = (r3 * bits(0x37800000u) - 0.5f) * scale;
            p.x = p.x + where.x;
            p.y = p.y + where.y;
            p.z = p.z + where.z;
            Point3 b;
            ball(self, b);
            Point3 velocity = {b.x * speed, b.y * speed, b.z * speed};
            if (real(o, 228) != 0.0f) {
                const float wx = icos__Fi(half(o, 56)) * real(o, 228);
                const float wy = isin__Fi(half(o, 56)) * real(o, 228);
                p.x = p.x + wx;
                p.y = p.y + wy;
                p.z = p.z + 0.0f;
            }
            P_AddParticle__FP11ParticleDefP6Point3T1f(particle, &p, &velocity, 0.0f);
        } while (0.0f < accumulated);
    }
}
}  // namespace

extern "C" void msg_run__17UndeadKnightClone(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 2556) != 0) {
        unsigned char* const group = from_word(word(o, 36));
        const int state = half(o, 572);
        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 2624, from_word(word(group, 180)), o + 2560, position, half(o, 56), from_word(word(group, 12)), o + 288, 0.5f,
            (state == 6 || state == 13) ? 1 : 0, 0, 1.0f, nullptr);
    }
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    call_entry(o, 168);
    {
        const Point3 zero = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero);
    }
    {
        Point3 lifted;
        lifted.x = real(o, 16) + 0.0f;
        lifted.y = real(o, 20);
        lifted.z = real(o, 24) + 84.0f;
        *reinterpret_cast<Point3*>(o + 2544) = lifted;
        lightUpdatePosition__FP5Light(o + 2496);
    }
    auto set_state = [&](int n) { call_entry(o, 184, static_cast<unsigned long>(n)); };
    auto zone = [&](float range, short angle) { return enemyInZone__8Creaturefs(self, range, angle) != 0; };
    auto bits294 = [&]() { return static_cast<unsigned>(*reinterpret_cast<unsigned short*>(o + 294)); };
    auto melee = [&]() {
        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, half(o, 200), half(o, 202), half(o, 204), 0x40008, nullptr);
    };
    auto count_down = [&]() {
        if ((bits294() & 8u) == 0) sword(o, 256) = sword(o, 256) - 1;
        if (sword(o, 256) > 0) return;
        set_state(3);
    };
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) > 17u) {
        set_state(3);
    } else {
        switch (state) {
            case 0:
                if (zone(400.0f, 32767) || zone(500.0f, 16000)) set_state(3);
                else if (sword(o, 256) < sword(o, 808)) set_state(1);
                break;
            case 1:
                if (zone(400.0f, 32767) || zone(500.0f, 16000)) set_state(0);
                else smartWander__8Creature(self);
                break;
            case 3:
                if (word(o, 576) == 0) {
                    set_state(0);
                } else if (zone(real(o, 28) + 60.0f, 23300)) {
                    set_state(eRand__10GameObject(self) % 100u < 65u ? 16 : 14);
                } else if (zone(real(o, 28) + 80.0f, 11000)) {
                    set_state(eRand__10GameObject(self) % 100u < 65u ? 14 : 15);
                } else if (zone(real(o, 28) + 80.0f, 6000)) {
                    set_state(eRand__10GameObject(self) % 100u < 65u ? 13 : 15);
                } else if (zone(real(o, 28) + 120.0f, 3000)) {
                    if (frameNm < sword(o, 256)) steerAndRouteToEnemy__8Creature(self);
                    else set_state(17);
                } else if (zone(300.0f, 32767)) {
                    steerAndRouteToEnemy__8Creature(self);
                } else if (strcmp(levelName + 81, "battle3") == 0) {
                    set_state(10);
                } else {
                    steerAndRouteToEnemy__8Creature(self);
                }
                break;
            case 5:
                if (zone(real(o, 28) + 40.0f, 6000)) set_state(6);
                else if (bExitStatePersueIdle__8Creature(self) != 0) set_state(3);
                break;
            case 13:
                if (word(o, 576) != 0) {
                    if ((advance & 7) != 0) melee();
                    turnToFaceEnemy__8Creature(self);
                }
                if ((bits294() & 8u) == 0) set_state(3);
                break;
            case 14:
            case 15:
            case 16:
            case 17:
                if (word(o, 576) != 0) {
                    if ((advance & 7) != 0) melee();
                    turnToFaceEnemy__8Creature(self);
                }
                count_down();
                break;
            case 10: {
                scatter(o, 500, undead_knight_clone_msg_run_particle_7d7ed0, 24.0f, false);
                burst(o, undead_knight_clone_msg_run_particle_7d7ed0, 24.0f, 0x3727c5acu, 0x533a43b7u, 0x420d6666u);
                if (!(frameNm < sword(o, 256))) {
                    word(o, 2556) = 0;
                    void* const anim = o + 288;
                    {
                        unsigned char matrix[48];
                        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
                        modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
                            from_word(udkResourceMap[3]), position, half(o, 56), anim, 0, matrix, real(o, 224));
                        void* const block = blockAlloc__Fi(584);
                        if (block != nullptr) {
                            const Point3 zero = {0.0f, 0.0f, 0.0f};
                            __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(
                                block, zero, matrix, const_cast<char*>("udk.lmp"), const_cast<char*>("sword.vif"), const_cast<char*>("sword.tex"), 1, 16641,
                                nullptr, real(o, 224));
                        }
                    }
                    word(o, 2696) = 0;
                    {
                        unsigned char matrix[48];
                        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
                        modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
                            from_word(udkResourceMap[3]), position, half(o, 56), anim, 1, matrix, real(o, 224));
                        void* const block = blockAlloc__Fi(584);
                        if (block != nullptr) {
                            const Point3 zero = {0.0f, 0.0f, 0.0f};
                            __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(
                                block, zero, matrix, const_cast<char*>("udk.lmp"), const_cast<char*>("shield.vif"), const_cast<char*>("shield.tex"), 1, 16641,
                                nullptr, real(o, 224));
                        }
                    }
                    objectAddToDeleteList__FP10GameObject(self);
                    objectRemoveFromRunList__FP10GameObject(self);
                }
                break;
            }
            default:
                set_state(3);
                break;
        }
    }
    scatter(o, 100, undead_knight_clone_msg_run_particle_7d7e80, 12.0f, true);
    burst(o, undead_knight_clone_msg_run_particle_7d7e80, 12.0f, 0x3727c5acu, 0x53ba43b7u, 0x420d6666u);
}
