// Observed msg_run__5Gnome at 0x002015a8, 1016 bytes, with a 14-entry jump table at 0x46f390.
// t is the table at word +36 (t[12] is the model header); S is the state, the signed halfword at
// +572. Common start: newDamageHandler(self, t[12]); everyFrame(self, t[12]) and return when it is
// nonzero; objectActivationCheck(self, 600) and return when it is zero; creatureAdvanceAnimation
// (self, 1.0), whose result is kept as F (the image copies it in the delay slot of the next call, so
// the slideBack result is not the one kept); slideBack(self, 1.0); then the class table entry at
// (word +72) + 168 is called with the adjusted object. A state of 14 or more (unsigned) stops
// there. setState below is the table entry at +184, called with the adjusted object and a value.
//  S 0: when word +964 is zero: enemyInZone(300.0, 32767) -> setState(3) and stop; when word +964 is
//       still zero: enemyInZone(400.0, 16000) -> setState(3) and stop. Then, if word +256 is below
//       word +808 (signed), setState(1).
//  S 1: the same two tests, each leading to setState(0); otherwise smartWander(self).
//  S 3: when word +576 is zero, setState(0). Else enemyInZone(float(+28) + 20.0, 6000) -> setState(6);
//       else enemyInZone(float(+28) + 20.0, 32767) -> turnToFaceEnemy; else steerAndRouteToEnemy.
//  S 5: enemyInZone(float(+28) + 20.0, 6000) -> setState(6); else bExitStatePersueIdle(self)
//       nonzero -> setState(3).
//  S 6: when word +576 is nonzero: if bit 0 of F, produceMeleeDamage(self, 36.0, h200, h202, h204,
//       8, null); and if bit 3 (0x8) of halfword +294 is set, turnToFaceEnemy. Then, when that bit
//       is clear, word +256 is decremented; if word +256 is not above zero (signed), setState(3).
//  S 8: when bit 4 (0x10) of halfword +294 is clear, setState(3).
//  S 9: when word +804 is below word +808 (signed), setState(3).
//  S 10: when bit 7 (0x80) of halfword +294 is set, objectRemoveFromRunList(self).
//  S 13: when word +968 is zero, setState(0). Else the object at link+12 (link = word +968) has the
//        12 bytes at +16 copied to a local; the table entry at +176 is called with the adjusted
//        object, 0 and the address of the copy. Then, when word +972 is below word +808 (signed),
//        word +972 = word +808 + 720 and setState(0).
//  S 2, 4, 7, 11, 12: nothing.
// Inferred: S 13 forwards an alert about a linked object. Unknown: what the states and fields mean;
// the entry at +168 takes only the object. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short flags);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
void set_state(void* self, int value) {
    w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), value);
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int& sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
unsigned short half(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
}  // namespace

extern "C" void msg_run__5Gnome(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    if (everyFrame__8CreatureP12_modelHeader(self, model) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int frameFlags = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 1.0f);
    w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 168))(w10g::adjusted(self, 168));
    const short state = *reinterpret_cast<short*>(o + 572);
    if (static_cast<unsigned short>(state) >= 14u) return;
    switch (state) {
    case 0:
        if (word(o, 964) == 0) {
            if (enemyInZone__8Creaturefs(self, 300.0f, 32767)) { set_state(self, 3); return; }
            if (word(o, 964) == 0) {
                if (enemyInZone__8Creaturefs(self, 400.0f, 16000)) { set_state(self, 3); return; }
            }
        }
        if (sword(o, 256) < sword(o, 808)) set_state(self, 1);
        break;
    case 1:
        if (word(o, 964) == 0) {
            if (enemyInZone__8Creaturefs(self, 300.0f, 32767)) { set_state(self, 0); return; }
            if (word(o, 964) == 0) {
                if (enemyInZone__8Creaturefs(self, 400.0f, 16000)) { set_state(self, 0); return; }
            }
        }
        smartWander__8Creature(self);
        break;
    case 3:
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, *reinterpret_cast<float*>(o + 28) + 20.0f, 6000)) { set_state(self, 6); return; }
        if (enemyInZone__8Creaturefs(self, *reinterpret_cast<float*>(o + 28) + 20.0f, 32767)) {
            turnToFaceEnemy__8Creature(self);
        } else {
            steerAndRouteToEnemy__8Creature(self);
        }
        break;
    case 5:
        if (enemyInZone__8Creaturefs(self, *reinterpret_cast<float*>(o + 28) + 20.0f, 6000)) { set_state(self, 6); return; }
        if (bExitStatePersueIdle__8Creature(self)) set_state(self, 3);
        break;
    case 6:
        if (word(o, 576) != 0) {
            if ((frameFlags & 1) != 0) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                    self, 36.0f, *reinterpret_cast<short*>(o + 200), *reinterpret_cast<short*>(o + 202),
                    *reinterpret_cast<short*>(o + 204), 8, nullptr);
            }
            if ((half(o, 294) & 0x8u) != 0) turnToFaceEnemy__8Creature(self);
        }
        if ((half(o, 294) & 0x8u) == 0) sword(o, 256) = sword(o, 256) - 1;
        if (sword(o, 256) > 0) return;
        set_state(self, 3);
        break;
    case 8:
        if ((half(o, 294) & 0x10u) == 0) set_state(self, 3);
        break;
    case 9:
        if (sword(o, 804) < sword(o, 808)) set_state(self, 3);
        break;
    case 10:
        if ((half(o, 294) & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
        break;
    case 13: {
        if (word(o, 968) == 0) { set_state(self, 0); return; }
        unsigned char* linked = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 968)) + 12));
        Point3 position = *reinterpret_cast<Point3*>(linked + 16);
        w10g::fn_from_word<void (*)(void*, int, Point3)>(w10g::entry_function(self, 176))(w10g::adjusted(self, 176), 0, position);
        if (sword(o, 972) < sword(o, 808)) {
            word(o, 972) = word(o, 808) + 720u;
            set_state(self, 0);
        }
        break;
    }
    default:
        break;
    }
}
