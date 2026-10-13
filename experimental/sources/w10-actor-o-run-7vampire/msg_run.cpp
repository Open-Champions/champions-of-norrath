// Observed msg_run__7Vampire at 0x0023fca0, 1392 bytes. R is the block at the word object+36, s the
// signed halfword at +572 (the state), b the unsigned halfword at +294, e the word at +576, wait the
// word at +256 and base the word at +808. "setState(n)" is the table entry at (word +72) + 184 called
// with n; "enemyInZone(d, a)" is enemyInZone(this, d, a) as a boolean. Steps, in order:
//  1. newDamageHandler(this, R[12]); everyFrame(this, R[12]) returning non-zero ends the call;
//     objectActivationCheck(this, 600) returning zero ends the call.
//  2. a = creatureAdvanceAnimation(this, 1.0); slideBack(this, 0.5); the table entry at +168 is called;
//     move2(this, {0, 0, 0}).
//  3. A state outside 0..16 (compared as an unsigned value) ends the call. By state:
//   0: enemyInZone(400, 32767) or enemyInZone(500, 18000): setState(3); otherwise setState(1) when
//      wait < base (signed).
//   1, 13: enemyInZone(400, 32767) or enemyInZone(500, 18000): setState(3); otherwise smartWander.
//   3: with e zero, setState(0). Otherwise with f the float at +28: enemyInZone(f + 20, 5000):
//      setState(6); enemyInZone(90, 3000): setState(15) when eRand & 3 is zero, else setState(16);
//      otherwise objectIsCrowded(this, the word at e + 12, 6f, 3f, 0): setState(14), else
//      steerAndRouteToEnemy.
//   5: enemyInZone(f + 20, 5000): setState(6); otherwise bExitStatePersueIdle: when non-zero
//      setState(3).
//   6: with e non-zero: bit 0 of a set: produceMeleeDamage(this, 36.0, h200, h202, h204, 8, null)
//      (the signed halfwords at +200, +202, +204); b bit 3 clear: setState(15) and end; else
//      turnToFaceEnemy. Then, when b bit 3 is set, end; else setState(15).
//   8: b bit 4 (0x10) clear: setState(3).
//   10: fxRunCreatureFire(R[12], the animation state at +288, a copy of the point at +16, the
//      halfword at +56, the state block at +824, 1.0, the floats at +224 and +228, the unsigned
//      byte at +40 as a float); when wait + 60 < base: bit 1 of the word at +12 and bit 41 of the
//      doubleword at +160 are set; when (float)wait + 75.0 < (float)base: objectRemoveFromRunList.
//   14: with e zero, setState(0). Otherwise: wait < base: setState(3); else objectIsCrowded(this,
//      the word at e + 12, 6f, 3f, 1) zero: setState(3). Then turnToFaceEnemy.
//   15: b bit 3 clear: setState(3).
//   16: with e non-zero: bit 0 of a: produceMeleeDamage(this, 80.0, ...) as in 6; b bit 3 set:
//      turnToFaceEnemy. Then, with b bit 3 clear, wait is decremented; when wait is not positive,
//      setState(3).
//   2, 4, 7, 9, 11, 12: nothing.
// Meanings of the states, the table entries and the halfwords are unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int value);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int move2__8CreatureG6Point3(void* self, Point3 push);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void smartWander__8Creature(void* self);
unsigned eRand__10GameObject(void* object);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
    void* model, void* animation, Point3 position, short angle, void* state, float a, float b, float c, float d);
void objectRemoveFromRunList__FP10GameObject(void* object);

void msg_run__7Vampire(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    call_entry(o, 168);
    const Point3 zero = {0.0f, 0.0f, 0.0f};
    move2__8CreatureG6Point3(self, zero);
    auto set_state = [&](int n) { call_entry(o, 184, static_cast<unsigned long>(n)); };
    auto zone_near = [&]() {
        return enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 500.0f, 18000) != 0;
    };
    auto crowded = [&](float scale_a, float scale_b, int flag) {
        const float f = real(o, 28);
        return objectIsCrowded__8CreatureP10GameObjectffi(self, from_word(word(from_word(word(o, 576)), 12)), f * scale_a, f * scale_b, flag) != 0;
    };
    auto melee = [&](float amount) {
        if ((advance & 1) != 0)
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, amount, half(o, 200), half(o, 202), half(o, 204), 8, nullptr);
    };
    auto bits294 = [&]() { return static_cast<unsigned>(*reinterpret_cast<unsigned short*>(o + 294)); };
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) > 16u) return;
    switch (state) {
        case 0:
            if (zone_near()) { set_state(3); return; }
            if (static_cast<int>(word(o, 256)) < static_cast<int>(word(o, 808))) set_state(1);
            return;
        case 1:
        case 13:
            if (zone_near()) { set_state(3); return; }
            smartWander__8Creature(self);
            return;
        case 3:
            if (word(o, 576) == 0) { set_state(0); return; }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 20.0f, 5000) != 0) { set_state(6); return; }
            if (enemyInZone__8Creaturefs(self, 90.0f, 3000) != 0) {
                if ((eRand__10GameObject(self) & 3u) == 0) set_state(15);
                else set_state(16);
                return;
            }
            if (crowded(6.0f, 3.0f, 0)) { set_state(14); return; }
            steerAndRouteToEnemy__8Creature(self);
            return;
        case 5:
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 20.0f, 5000) != 0) { set_state(6); return; }
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(3);
            return;
        case 6:
            if (word(o, 576) != 0) {
                melee(36.0f);
                if ((bits294() & 8u) == 0) { set_state(15); return; }
                turnToFaceEnemy__8Creature(self);
            }
            if ((bits294() & 8u) != 0) return;
            set_state(15);
            return;
        case 8:
            if ((bits294() & 0x10u) == 0) set_state(3);
            return;
        case 10:
            fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
                from_word(word(from_word(word(o, 36)), 12)), o + 288, *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 824, 1.0f,
                real(o, 224), real(o, 228), static_cast<float>(o[40]));
            if (static_cast<int>(word(o, 256)) + 60 < static_cast<int>(word(o, 808))) {
                word(o, 12) = word(o, 12) | 2u;
                dword(o, 160) = dword(o, 160) | (0x8000ull << 26);
            }
            if (static_cast<float>(sword(o, 256)) + 75.0f < static_cast<float>(sword(o, 808))) objectRemoveFromRunList__FP10GameObject(self);
            return;
        case 14:
            if (word(o, 576) == 0) { set_state(0); return; }
            if (static_cast<int>(word(o, 256)) < static_cast<int>(word(o, 808))) set_state(3);
            else if (!crowded(6.0f, 3.0f, 1)) set_state(3);
            turnToFaceEnemy__8Creature(self);
            return;
        case 15:
            if ((bits294() & 8u) == 0) set_state(3);
            return;
        case 16:
            if (word(o, 576) != 0) {
                melee(80.0f);
                if ((bits294() & 8u) != 0) turnToFaceEnemy__8Creature(self);
            }
            if ((bits294() & 8u) == 0) sword(o, 256) = sword(o, 256) - 1;
            if (sword(o, 256) > 0) return;
            set_state(3);
            return;
        default:
            return;
    }
}
}
