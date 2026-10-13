// Observed msg_run__12UndeadKnight at 0x0023c798, 2448 bytes. R is the block at the word object+36,
// s the signed halfword at +572 (the state), b the unsigned halfword at +294, e the word at +576,
// wait the word at +256, base the word at +808, a the value of creatureAdvanceAnimation. "setState(n)"
// is the table entry at (word +72) + 184 called with n; "enemyInZone(d, k)" is enemyInZone(this, d, k)
// as a boolean; "melee" is produceMeleeDamage(this, 36.0, h200, h202, h204, 0x40008, null) with the
// signed halfwords at +200, +202, +204. Steps, in order:
//  1. With the word at +968 non-zero: fxRunWeapon(the description at +1040, R[180], the weapon state at
//     +972, a copy of the point at +16, the halfword at +56, R[12], the animation state at +288, 0.5,
//     1 when s is 6 or 13 (else 0), 0, 1.0, null matrix). With the word at +960 non-zero:
//     objectUpdateInGrid(this).
//  2. newDamageHandler(this, R[12]); everyFrame(this, R[12]) non-zero ends the call;
//     objectActivationCheck(this, 1200) (its result is not used).
//  3. a = creatureAdvanceAnimation(this, 1.0); slideBack(this, 0.5); the table entry at +168 is called;
//     with the word at +960 zero: move2(this, {0, 0, 0}).
//  4. A state outside 0..20 (unsigned compare) calls setState(3) and ends. By state:
//   0: enemyInZone(400, 32767) or enemyInZone(500, 16000): setState(3); otherwise setState(1) when
//      wait < base (signed).
//   1: enemyInZone(400, 32767) or enemyInZone(500, 16000): setState(0); otherwise smartWander.
//   3: e zero: setState(0). Otherwise with f the float at +28 and r = eRand % 100:
//      enemyInZone(f + 60, 23300): setState(16) when r < 65 else 14; enemyInZone(f + 80, 11000):
//      setState(14) when r < 65 else 15; enemyInZone(f + 80, 6000): setState(13) when r < 65 else
//      15; enemyInZone(f + 120, 3000): steerAndRouteToEnemy when frameNm < wait (signed), else
//      setState(17); otherwise, when the word at +964 is non-zero steerAndRouteToEnemy, else
//      setState(19). (eRand is called once, only on the branches that use r.)
//   5: enemyInZone(f + 40, 6000): setState(6); otherwise bExitStatePersueIdle: when non-zero
//      setState(3).
//   8: b bit 4 (0x10) clear: setState(3).   9: when the signed word at +804 is below base: setState(3).
//   10: with the word at +964 non-zero, bit 1 of the word at +12 of the object at (that word) + 12 is
//      cleared; when b bit 7 (0x80) is set: soundFadeStream(0) and objectRemoveFromRunList(this).
//   13: e non-zero: melee when a & 7 is non-zero, then turnToFaceEnemy; then setState(3) when b bit 3
//      is clear.
//   14..17: e non-zero: melee when a & 7 is non-zero, then turnToFaceEnemy. Then, when b bit 3 is clear,
//      wait is decremented; when wait is not positive, setState(3).
//   18, 20: when b bit 3 is clear wait is decremented; when wait is not positive, setState(3).
//   19: the halfwords at +148, +150, +152 become -150 and the one at +154 becomes 0. With the word at
//      +964 non-zero (call it p, and t the object at the word p + 12): for 75 rounds, with na and nb the
//      shadow-point counts modelGetNmShadowPoints gives for the models of this object and of t (the
//      table entry at (word +72) + 128 is called on each object to get its model): A = the shadow
//      position of this object at index eRand % na, B = that of t at index eRand % nb (modelGetShadowPos
//      with the point at +16 of the respective object, its halfword at +56, its float at +224 and
//      its animation state at +288, null extra outputs); P_AddParticle(the particle at 0x7d7e30, &A,
//      &(B - A scaled by 0.2989970f), 0.0). Then, for a & 1 set and the word at +964 non-zero, bit 1 of
//      the word at +12 of t is cleared. Then setState(3) when b bit 3 is clear.
//   2, 4, 6, 7, 11, 12: setState(3).
// The meaning of the states and fields is unknown. The particle definition is a file-local object of
// the original unit, named here by its address. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h,
    float i, void* matrix);
void objectUpdateInGrid__FP10GameObject(void* object);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int value);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int move2__8CreatureG6Point3(void* self, Point3 push);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void smartWander__8Creature(void* self);
unsigned eRand__10GameObject(void* object);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* out, float* extra, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void soundFadeStream__Fi(int stream);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern int frameNm;
extern unsigned char undead_knight_msg_run_particle_7d7e30[];
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
}  // namespace

extern "C" void msg_run__12UndeadKnight(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 968) != 0) {
        unsigned char* const group = from_word(word(o, 36));
        const int state = half(o, 572);
        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1040, from_word(word(group, 180)), o + 972, position, half(o, 56), from_word(word(group, 12)), o + 288, 0.5f,
            (state == 6 || state == 13) ? 1 : 0, 0, 1.0f, nullptr);
    }
    if (word(o, 960) != 0) objectUpdateInGrid__FP10GameObject(self);
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 1200);
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    call_entry(o, 168);
    if (word(o, 960) == 0) {
        const Point3 zero = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero);
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
    switch (static_cast<unsigned>(state)) {
        case 0:
            if (zone(400.0f, 32767) || zone(500.0f, 16000)) { set_state(3); return; }
            if (sword(o, 256) < sword(o, 808)) set_state(1);
            return;
        case 1:
            if (zone(400.0f, 32767) || zone(500.0f, 16000)) { set_state(0); return; }
            smartWander__8Creature(self);
            return;
        case 3: {
            if (word(o, 576) == 0) { set_state(0); return; }
            if (zone(real(o, 28) + 60.0f, 23300)) {
                set_state(eRand__10GameObject(self) % 100u < 65u ? 16 : 14);
                return;
            }
            if (zone(real(o, 28) + 80.0f, 11000)) {
                set_state(eRand__10GameObject(self) % 100u < 65u ? 14 : 15);
                return;
            }
            if (zone(real(o, 28) + 80.0f, 6000)) {
                set_state(eRand__10GameObject(self) % 100u < 65u ? 13 : 15);
                return;
            }
            if (zone(real(o, 28) + 120.0f, 3000)) {
                if (frameNm < sword(o, 256)) steerAndRouteToEnemy__8Creature(self);
                else set_state(17);
            } else if (word(o, 964) != 0) {
                steerAndRouteToEnemy__8Creature(self);
            } else {
                set_state(19);
            }
            return;
        }
        case 5:
            if (zone(real(o, 28) + 40.0f, 6000)) { set_state(6); return; }
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(3);
            return;
        case 8:
            if ((bits294() & 0x10u) == 0) set_state(3);
            return;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(3);
            return;
        case 10:
            if (word(o, 964) != 0) {
                unsigned char* const target = from_word(word(from_word(word(o, 964)), 12));
                word(target, 12) = word(target, 12) & ~2u;
            }
            if ((bits294() & 0x80u) != 0) {
                soundFadeStream__Fi(0);
                objectRemoveFromRunList__FP10GameObject(self);
            }
            return;
        case 13:
            if (word(o, 576) != 0) {
                if ((advance & 7) != 0) melee();
                turnToFaceEnemy__8Creature(self);
            }
            if ((bits294() & 8u) != 0) return;
            set_state(3);
            return;
        case 14:
        case 15:
        case 16:
        case 17:
            if (word(o, 576) != 0) {
                if ((advance & 7) != 0) melee();
                turnToFaceEnemy__8Creature(self);
            }
            count_down();
            return;
        case 18:
        case 20:
            count_down();
            return;
        case 19: {
            half(o, 152) = -150;
            half(o, 148) = -150;
            half(o, 150) = -150;
            half(o, 154) = 0;
            if (word(o, 964) != 0) {
                unsigned char* const target = from_word(word(from_word(word(o, 964)), 12));
                void* const my_model = from_word(call_entry(o, 128));
                const int my_points = modelGetNmShadowPoints__FP12_modelHeader(my_model);
                void* const their_model = from_word(call_entry(target, 128));
                const int their_points = modelGetNmShadowPoints__FP12_modelHeader(their_model);
                for (int round = 0; round < 75; ++round) {
                    void* const model = from_word(call_entry(o, 128));
                    const int mine = signed_remainder(static_cast<int>(eRand__10GameObject(self)), my_points);
                    Point3 first;
                    const Point3 my_position = *reinterpret_cast<Point3*>(o + 16);
                    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                        model, o + 288, my_position, half(o, 56), mine, &first, nullptr, nullptr, real(o, 224));
                    void* const other_model = from_word(call_entry(target, 128));
                    const int theirs = signed_remainder(static_cast<int>(eRand__10GameObject(self)), their_points);
                    Point3 second;
                    const Point3 their_position = *reinterpret_cast<Point3*>(target + 16);
                    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                        other_model, target + 288, their_position, half(target, 56), theirs, &second, nullptr, nullptr, real(target, 224));
                    Point3 velocity;
                    velocity.x = second.x - first.x;
                    velocity.y = second.y - first.y;
                    velocity.z = second.z - first.z;
                    velocity.x = velocity.x * bits(0x3e991622u);
                    velocity.y = velocity.y * bits(0x3e991622u);
                    velocity.z = velocity.z * bits(0x3e991622u);
                    P_AddParticle__FP11ParticleDefP6Point3T1f(undead_knight_msg_run_particle_7d7e30, &first, &velocity, 0.0f);
                }
            }
            if ((advance & 1) != 0 && word(o, 964) != 0) {
                unsigned char* const target = from_word(word(from_word(word(o, 964)), 12));
                word(target, 12) = word(target, 12) & ~2u;
            }
            if ((bits294() & 8u) != 0) return;
            set_state(3);
            return;
        }
        default:
            set_state(3);
            return;
    }
}
