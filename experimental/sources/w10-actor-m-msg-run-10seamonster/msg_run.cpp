#include "../w10-actor-m-common/access.hpp"
// Observed msg_run__10SeaMonster at 0x1f82a0, 1228 bytes. Nothing is returned. T is the table at the word at
// object+36 (entry 12 is the model header). "setState(n)" is table entry 184 of the object's table called on the
// adjusted object with n. h294 is the unsigned halfword at +294; word256 and word808 are compared as signed.
//  1. When the word at +236 is nonzero, the word at +960 is zero and bit 0 of the 64-bit word at +160 is set:
//     soundResetStream__Fi(0); a roll R from eRand__10GameObject(object); sprintf(buffer, a format taking one
//     int, (R & 3) + 1) with the text "mx_boss_generic%d_l.vag"; soundStartMusic__FPci(buffer, 259); the word at
//     +960 becomes 1.
//  2. newDamageHandler__8CreatureP12_modelHeader(object, T12); everyFrame__8CreatureP12_modelHeader(object, T12)
//     nonzero ends the function; objectActivationCheck__8Creaturei(object, 600) returning zero ends it too.
//  3. s = creatureAdvanceAnimation__8Creaturef(object, 1.0f) (its result is still in V0 when the image copies
//     it into a saved register in the delay slot of the slideBack call); slideBack__8Creaturef(object, 0.2f);
//     then table entry 168 (slot 21) is called on the adjusted object.
//  4. The state (signed halfword at +572) must be 0..15; the jump table at 0x46eb10 selects:
//   0: enemyInZone__8Creaturefs(object, 400.0f, 32767) nonzero -> setState(3); else word256 < word808 -> setState(1).
//   1: move2__8CreatureG6Point3(object, zero vector); then as 0 except the last step is word256 < word808 ->
//      setState(0), otherwise smartWander__8Creature.
//   3: word576 == 0 -> setState(0). Otherwise enemyInZone(object, float at +28 + 20.0f, 5000) nonzero ->
//      setState(6); else enemyInZone(object, +28 + 60.0f, 5000) nonzero -> setState(15); else enemyInZone(object,
//      +28 + 60.0f, 32767) nonzero -> turnToFaceEnemy__8Creature, else steerAndRouteToEnemy__8Creature.
//   5: enemyInZone(+28 + 20.0f, 5000) nonzero -> setState(6); else (+28 + 60.0f, 5000) nonzero -> setState(15);
//      else bExitStatePersueIdle__8Creature nonzero -> setState(3).
//   6 and 15: move2(object, zero vector); with word576 != 0 and bit 0 of s set,
//      produceMeleeDamage__8CreaturefiiiiPQ24item11description(object, 36.0f (state 6) or 60.0f (state 15),
//      halfwords at +200, +202, +204, 8, null); when bit 3 of h294 is clear the word at +256 is decremented;
//      then word256 <= 0 (signed) -> setState(3).
//   8: bit 4 (0x10) of h294 clear -> setState(0).
//   9: word804 < word808 (signed) -> setState(3).
//   10: move2(object, zero vector); bit 7 (0x80) of h294 set -> objectRemoveFromRunList__FP10GameObject(object).
//   13 and 14: move2(object, zero vector); bit 3 of h294 clear -> setState(3).
//   2, 4, 7, 11, 12: nothing.
// Inferred: this is the sea-monster boss run loop (music start on first run); names come from the callees.
// Unknown: the state meanings, the fields at +236, +256, +576, +804, +808, and the music-name text beyond its use.
extern "C" {
void soundResetStream__Fi(int stream);
int sprintf(char* buffer, const char* format, ...);
void soundStartMusic__FPci(char* name, int mode);
int eRand__10GameObject(void* object);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short mask);
void move2__8CreatureG6Point3(void* self, Point3 where);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
inline void set_state(unsigned char* o, int state) { w10m::call_entry(o, 184, static_cast<unsigned long>(state)); }
inline void stand_still(void* self) {
    Point3 zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    move2__8CreatureG6Point3(self, zero);
}
inline bool less(unsigned char* o, unsigned left, unsigned right) {
    return static_cast<int>(w10m::word(o, left)) < static_cast<int>(w10m::word(o, right));
}
inline void melee_state(unsigned char* o, int advanced, unsigned amount_bits) {
    stand_still(o);
    if (w10m::word(o, 576) != 0 && (advanced & 1) != 0) {
        produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, w10m::bits(amount_bits), w10m::half(o, 200), w10m::half(o, 202),
                                                               w10m::half(o, 204), 8, nullptr);
    }
    if ((static_cast<unsigned short>(w10m::half(o, 294)) & 8u) == 0) w10m::word(o, 256) = w10m::word(o, 256) - 1u;
    if (static_cast<int>(w10m::word(o, 256)) > 0) return;
    set_state(o, 3);
}
}  // namespace

extern "C" void msg_run__10SeaMonster(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (w10m::word(o, 236) != 0 && w10m::word(o, 960) == 0 && (w10m::dword(o, 160) & 1ull) != 0) {
        char name[32];
        soundResetStream__Fi(0);
        const int roll = eRand__10GameObject(self);
        sprintf(name, "mx_boss_generic%d_l.vag", (roll & 3) + 1);
        soundStartMusic__FPci(name, 259);
        w10m::word(o, 960) = 1;
    }
    newDamageHandler__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advanced = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, w10m::bits(0x3e4ccccc));
    w10m::call_entry(o, 168);
    switch (static_cast<unsigned>(static_cast<int>(w10m::half(o, 572)))) {
    case 0u:
        if (enemyInZone__8Creaturefs(self, w10m::bits(0x43c80000u), 32767) != 0) set_state(o, 3);
        else if (less(o, 256, 808)) set_state(o, 1);
        break;
    case 1u:
        stand_still(self);
        if (enemyInZone__8Creaturefs(self, w10m::bits(0x43c80000u), 32767) != 0) set_state(o, 3);
        else if (less(o, 256, 808)) set_state(o, 0);
        else smartWander__8Creature(self);
        break;
    case 3u:
        if (w10m::word(o, 576) == 0) {
            set_state(o, 0);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41a00000u), 5000) != 0) {
            set_state(o, 6);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x42700000u), 5000) != 0) {
            set_state(o, 15);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x42700000u), 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
        } else {
            steerAndRouteToEnemy__8Creature(self);
        }
        break;
    case 5u:
        if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41a00000u), 5000) != 0) {
            set_state(o, 6);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x42700000u), 5000) != 0) {
            set_state(o, 15);
        } else if (bExitStatePersueIdle__8Creature(self) != 0) {
            set_state(o, 3);
        }
        break;
    case 6u:
        melee_state(o, advanced, 0x42100000u);
        break;
    case 15u:
        melee_state(o, advanced, 0x42700000u);
        break;
    case 8u:
        if ((static_cast<unsigned short>(w10m::half(o, 294)) & 0x10u) == 0) set_state(o, 0);
        break;
    case 9u:
        if (less(o, 804, 808)) set_state(o, 3);
        break;
    case 10u:
        stand_still(self);
        if ((static_cast<unsigned short>(w10m::half(o, 294)) & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
        break;
    case 13u:
    case 14u:
        stand_still(self);
        if ((static_cast<unsigned short>(w10m::half(o, 294)) & 8u) == 0) set_state(o, 3);
        break;
    default:
        break;
    }
}
