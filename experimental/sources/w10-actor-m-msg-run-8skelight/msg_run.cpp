#include "../w10-actor-m-common/access.hpp"
// Observed msg_run__8Skelight at 0x2469d0, 800 bytes. Nothing is returned. T is the table at the word
// at object+36 (its entry at 12 is the model header). "setState(n)" is table entry 184 of the object's table
// (slot 23, setState in the Skelight table view at 0x44fdf8) called on the adjusted object.
//  1. Unless the signed halfword at +572 is 13, newDamageHandler__8Creature(object, T12) runs.
//  2. everyFrame__8Creature(object, T12): a nonzero result ends the function.
//  3. objectActivationCheck__8Creaturei(object, 600); s = creatureAdvanceAnimation__8Creaturef(object, 1.0f)
//     (its result is the value held in a saved register across the next call: the image copies V0 into that
//     register in the delay slot of the call to slideBack, so it is still the advance result);
//     slideBack__8Creaturef(object, 0.7f); then table entry 168 (slot 21) is called on the adjusted object.
//  4. The state is the signed halfword at +572; values 14 and above (or negative) end here, and states 2, 4, 7,
//     9, 10, 11 and 12 do nothing. A jump table at 0x474210 selects (the words at +256 and +808 are compared
//     as signed; the word at +576 is a pointer-like field, tested for zero; h294 is the halfword at +294):
//   0: word576 != 0 -> setState(3); else bFarFromHome__8Creature -> setState(1); else word256 < word808 -> setState(1).
//   1: word256 < word808 -> setState(0); else smartWander__8Creature.
//   3: word576 == 0 -> setState(0). Otherwise enemyInZone__8Creaturefs(object, float at +28 + 50.0f, 6000)
//      nonzero -> setState(6); else enemyInZone(object, float at +28 + 30.0f, 32767) nonzero ->
//      turnToFaceEnemy__8Creature; else steerAndRouteToEnemy__8Creature.
//   5: enemyInZone(object, float at +28 + 50.0f, 6000) nonzero -> setState(6); else when
//      bExitStatePersueIdle__8Creature is nonzero -> setState(3).
//   6: with word576 != 0: when bit 0 of s is set, produceMeleeDamage__8CreaturefiiiiPQ24item11description(object,
//      36.0f, halfwords at +200, +202, +204, 8, the description at object+1032); then if bit 3 of h294 is clear
//      the word at +256 is decremented, otherwise turnToFaceEnemy runs and bit 3 of h294 is read again. With
//      word576 == 0 only the bit 3 test and decrement happen. Finally, if the word at +256 is not above zero
//      (signed), setState(3).
//   8: setState(3).
//   13: when bit 7 (0x80) of h294 is set, setState(0); then addEffect__FiG6Point3(7, copy of the 12 bytes at +16).
// Inferred: Skelight is a creature with a melee state machine; names come from the callees. Unknown: the
// state numbers' meanings, the fields at +256, +576 and +808 (a counter, a pointer-like word and a limit).
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int bFarFromHome__8Creature(void* self);
void smartWander__8Creature(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short mask);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void addEffect__FiG6Point3(int kind, Point3 where);
}

namespace {
inline void set_state(unsigned char* o, int state) { w10m::call_entry(o, 184, static_cast<unsigned long>(state)); }
}

extern "C" void msg_run__8Skelight(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (w10m::half(o, 572) != 13) {
        newDamageHandler__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12)));
    }
    if (everyFrame__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const int advanced = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, w10m::bits(0x3f333333u));
    w10m::call_entry(o, 168);
    switch (static_cast<unsigned>(static_cast<int>(w10m::half(o, 572)))) {
    case 0u:
        if (w10m::word(o, 576) != 0) set_state(o, 3);
        else if (bFarFromHome__8Creature(self) != 0) set_state(o, 1);
        else if (static_cast<int>(w10m::word(o, 256)) < static_cast<int>(w10m::word(o, 808))) set_state(o, 1);
        break;
    case 1u:
        if (static_cast<int>(w10m::word(o, 256)) < static_cast<int>(w10m::word(o, 808))) set_state(o, 0);
        else smartWander__8Creature(self);
        break;
    case 3u:
        if (w10m::word(o, 576) == 0) {
            set_state(o, 0);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x42480000u), 6000) != 0) {
            set_state(o, 6);
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41f00000u), 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
        } else {
            steerAndRouteToEnemy__8Creature(self);
        }
        break;
    case 5u:
        if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x42480000u), 6000) != 0) {
            set_state(o, 6);
        } else if (bExitStatePersueIdle__8Creature(self) != 0) {
            set_state(o, 3);
        }
        break;
    case 6u: {
        bool decremented = false;
        if (w10m::word(o, 576) != 0) {
            if ((advanced & 1) != 0) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, w10m::bits(0x42100000u), w10m::half(o, 200), w10m::half(o, 202),
                                                                       w10m::half(o, 204), 8, o + 1032);
            }
            if ((static_cast<unsigned short>(w10m::half(o, 294)) & 8u) == 0) {
                w10m::word(o, 256) = w10m::word(o, 256) - 1u;
                decremented = true;
            } else {
                turnToFaceEnemy__8Creature(self);
            }
        }
        if (!decremented && (static_cast<unsigned short>(w10m::half(o, 294)) & 8u) == 0) {
            w10m::word(o, 256) = w10m::word(o, 256) - 1u;
        }
        if (static_cast<int>(w10m::word(o, 256)) > 0) return;
        set_state(o, 3);
        break;
    }
    case 8u:
        set_state(o, 3);
        break;
    case 13u:
        if ((static_cast<unsigned short>(w10m::half(o, 294)) & 0x80u) != 0) set_state(o, 0);
        addEffect__FiG6Point3(7, *reinterpret_cast<Point3*>(o + 16));
        break;
    default:
        break;
    }
}
