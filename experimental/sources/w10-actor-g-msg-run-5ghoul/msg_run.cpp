// Observed msg_run__5Ghoul at 0x001f6640, 2012 bytes, with an 18-entry jump table at 0x46e710.
// t is the table at word +36 (t[12] the model header), S the signed halfword at +572, a the member
// at +288, setState(v) the table entry at (word +72) + 184 called with the adjusted object and v.
// Common start: newDamageHandler(self, t[12]); everyFrame(self, t[12]) and return when nonzero;
// objectActivationCheck(self, 600) and return when zero; F = creatureAdvanceAnimation(self, 1.0)
// (the image copies it in the delay slot of the next call, so slideBack's result is not the one
// kept); slideBack(self, 0.5); the table entry at +168 called with the adjusted object. S of 18 or
// more (unsigned) stops there. Blend B1 below is the single with bits 0x3dcccccc.
//  S 0: enemyInZone(400.0, 32767) -> setState(3); enemyInZone(500.0, 18000) -> setState(3); else
//       when word +256 is below word +808 (signed) setState(1).
//  S 1 and 13: the same two zone tests -> setState(3); otherwise smartWander(self).
//  S 3: word +576 (L) zero -> setState(0). Else enemyInZone(float(+28) + 20.0, 5000) -> setState(6);
//       else enemyInZone(90.0, 3000): when true, eRand R: setState(16) when R & 1, otherwise
//       setState(17); when false, objectIsCrowded(self, object at L+12, float(+28) * 6.0,
//       float(+28) * 3.0, 0) true -> setState(14); else enemyInZone(float(+28) + 20.0, 32767) true
//       -> turnToFaceEnemy, false -> steerAndRouteToEnemy.
//  S 5: enemyInZone(float(+28) + 20.0, 5000) -> setState(6); else bExitStatePersueIdle nonzero ->
//       setState(3).
//  S 6: when L is nonzero: F & 2 -> produceMeleeDamage(self, 36.0, h200, h202, h204, 1, null); halfword
//       +294 bit 3 (0x8) set -> turnToFaceEnemy. Then, when that bit is clear, setState(16).
//  S 8: halfword +294 bit 4 (0x10) clear -> setState(3).
//  S 9: word +804 below word +808 (signed) -> setState(0).
//  S 10: when the 64-bit word at +288 has both bits 0x0088000000000000 set: bit 0 of word +12 is set,
//       word +960 = word(+808) + eRand % 120, transition t[220] (B1, 19). Otherwise with H = halfword
//       +294: H & 0x10 and word +960 below word +808 -> word +960 = word(+808) + eRand % 120,
//       transition t[212] (B1, 35); else H & 0x20 and word +960 below word +808 -> word +960 =
//       (int)(float(+808) + 90.0), transition t[68 + 8 * (eRand % 7)] (1.0, 3), bit 0 of word +12 set,
//       the position (+16) moved back by 20.0 * (icos, isin) of the halfword +56, then with R1 and R2
//       two more eRand results floorAdd(position, (short)R1, float(25 + R2 % 10), 0, 1); else when
//       H & 0x38 is zero and word +960 is below word +808, objectRemoveFromRunList(self).
//  S 14: L zero -> setState(0). Else when word +256 is below word +808 -> setState(3) and then
//       turnToFaceEnemy; otherwise objectIsCrowded(self, object at L+12, float(+28) * 6.0,
//       float(+28) * 3.0, 1) nonzero -> turnToFaceEnemy only, zero -> setState(3), turnToFaceEnemy.
//  S 15: when word +960 is below word +808 (signed): one-shot t[140] (B1, 2.0, 0) and setState(3).
//  S 16: F & 1 sets word +968 to 1; F & 2 sets it to 0; F & 4 -> setState(3). When word +968 is
//       nonzero: speed = float(+176), multiplied by float(60 - (signed char)b) * (1/60 as the image
//       stores it, bits 0x3c888888) when the byte b at +800 is 1..60; the position delta is
//       (icos(h964) * speed, isin(h964) * speed, 0) passed to move2(self, delta). Then, when bit 3 of
//       halfword +294 is clear, setState(3).
//  S 17: as S 6 but the melee float is 80.0, and bit 3 clear decrements word +256 (signed) and
//       setState(3) follows when that word is not above zero.
//  S 2, 4, 7, 11, 12: nothing.
// Inferred: icos/isin are cosine and sine of an integer angle. Unknown: what the states and most
// fields mean. Nothing here ran on the EE.
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
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* description);
void move2__8CreatureG6Point3(void* self, Point3 delta);
float icos__Fi(int angle);
float isin__Fi(int angle);
unsigned eRand__10GameObject(void* self);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void floorAdd__FG6Point3sfii(Point3 position, short heading, float radius, int a, int b);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;
const float kB1 = 0x1.999998p-4f;

void set_state(void* self, int value) {
    w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), value);
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int& sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
unsigned short half(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
float& flt(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
void* header(void* object, unsigned offset) {
    unsigned char* table = w10g::from_word(w10g::word_at(object, 36));
    return w10g::from_word(*reinterpret_cast<unsigned*>(table + offset));
}
void* linked_object(unsigned char* o) {
    return w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 576)) + 12));
}
}  // namespace

extern "C" void msg_run__5Ghoul(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    if (everyFrame__8CreatureP12_modelHeader(self, model) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int frameFlags = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 168))(w10g::adjusted(self, 168));
    const short state = *reinterpret_cast<short*>(o + 572);
    if (static_cast<unsigned short>(state) >= 18u) return;
    switch (state) {
    case 0:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767)) { set_state(self, 3); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 18000)) { set_state(self, 3); return; }
        if (sword(o, 256) < sword(o, 808)) set_state(self, 1);
        break;
    case 1:
    case 13:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767)) { set_state(self, 3); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 18000)) { set_state(self, 3); return; }
        smartWander__8Creature(self);
        break;
    case 3: {
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, flt(o, 28) + 20.0f, 5000)) { set_state(self, 6); return; }
        if (enemyInZone__8Creaturefs(self, 90.0f, 3000)) {
            const unsigned roll = eRand__10GameObject(self);
            if ((roll & 1u) != 0) set_state(self, 16);
            else set_state(self, 17);
            return;
        }
        const float size = flt(o, 28);
        if (objectIsCrowded__8CreatureP10GameObjectffi(self, linked_object(o), size * 6.0f, size * 3.0f, 0)) {
            set_state(self, 14);
            return;
        }
        if (enemyInZone__8Creaturefs(self, flt(o, 28) + 20.0f, 32767)) turnToFaceEnemy__8Creature(self);
        else steerAndRouteToEnemy__8Creature(self);
        break;
    }
    case 5:
        if (enemyInZone__8Creaturefs(self, flt(o, 28) + 20.0f, 5000)) { set_state(self, 6); return; }
        if (bExitStatePersueIdle__8Creature(self)) set_state(self, 3);
        break;
    case 16: {
        if ((frameFlags & 1) != 0) word(o, 968) = 1;
        if ((frameFlags & 2) != 0) word(o, 968) = 0;
        if ((frameFlags & 4) != 0) set_state(self, 3);
        if (word(o, 968) != 0) {
            const unsigned char step = o[800];
            float speed = flt(o, 176);
            if (static_cast<unsigned>(step) - 1u < 60u) {
                speed = speed * (static_cast<float>(60 - static_cast<signed char>(step)) * 0x1.111110p-6f);
            }
            const int angle = *reinterpret_cast<short*>(o + 964);
            Point3 delta;
            delta.x = icos__Fi(angle) * speed;
            delta.y = isin__Fi(angle) * speed;
            delta.z = 0.0f;
            move2__8CreatureG6Point3(self, delta);
        }
        if ((half(o, 294) & 0x8u) != 0) return;
        set_state(self, 3);
        break;
    }
    case 14: {
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        if (sword(o, 256) < sword(o, 808)) {
            set_state(self, 3);
        } else {
            const float size = flt(o, 28);
            if (!objectIsCrowded__8CreatureP10GameObjectffi(self, linked_object(o), size * 6.0f, size * 3.0f, 1)) set_state(self, 3);
        }
        turnToFaceEnemy__8Creature(self);
        break;
    }
    case 6:
    case 17:
        if (word(o, 576) != 0) {
            if ((frameFlags & 2) != 0) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                    self, state == 6 ? 36.0f : 80.0f, *reinterpret_cast<short*>(o + 200), *reinterpret_cast<short*>(o + 202),
                    *reinterpret_cast<short*>(o + 204), 1, nullptr);
            }
            if ((half(o, 294) & 0x8u) != 0) turnToFaceEnemy__8Creature(self);
        }
        if (state == 6) {
            if ((half(o, 294) & 0x8u) != 0) return;
            set_state(self, 16);
        } else {
            if ((half(o, 294) & 0x8u) == 0) sword(o, 256) = sword(o, 256) - 1;
            if (sword(o, 256) > 0) return;
            set_state(self, 3);
        }
        break;
    case 9:
        if (sword(o, 804) < sword(o, 808)) set_state(self, 0);
        break;
    case 8:
        if ((half(o, 294) & 0x10u) == 0) set_state(self, 3);
        break;
    case 15:
        if (sword(o, 960) < sword(o, 808)) {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header(self, 140), kB1, 2.0f, 0);
            set_state(self, 3);
        }
        break;
    case 10: {
        void* a = o + 288;
        if ((*reinterpret_cast<AliasedDouble*>(o + 288) & 0x0088000000000000ull) == 0x0088000000000000ull) {
            word(o, 12) = word(o, 12) | 1u;
            const unsigned roll = eRand__10GameObject(self);
            word(o, 960) = word(o, 808) + roll % 120u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 220), kB1, 19);
            return;
        }
        const unsigned bits = half(o, 294);
        if ((bits & 0x10u) != 0 && sword(o, 960) < sword(o, 808)) {
            const unsigned roll = eRand__10GameObject(self);
            word(o, 960) = word(o, 808) + roll % 120u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 212), kB1, 35);
            return;
        }
        if ((bits & 0x20u) != 0 && sword(o, 960) < sword(o, 808)) {
            const int base = static_cast<int>(static_cast<float>(sword(o, 808)) + 90.0f);
            word(o, 960) = static_cast<unsigned>(base);
            const unsigned pick = eRand__10GameObject(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 68 + 8 * (pick % 7u)), 1.0f, 3);
            word(o, 12) = word(o, 12) | 1u;
            Point3 position = *reinterpret_cast<Point3*>(o + 16);
            const int heading = *reinterpret_cast<short*>(o + 56);
            position.x = position.x - icos__Fi(heading) * 20.0f;
            position.y = position.y - isin__Fi(*reinterpret_cast<short*>(o + 56)) * 20.0f;
            const unsigned first = eRand__10GameObject(self);
            const unsigned second = eRand__10GameObject(self);
            floorAdd__FG6Point3sfii(position, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 25u)), 0, 1);
            return;
        }
        if ((bits & 0x38u) != 0) return;
        if (sword(o, 960) < sword(o, 808)) objectRemoveFromRunList__FP10GameObject(self);
        break;
    }
    default:
        break;
    }
}
