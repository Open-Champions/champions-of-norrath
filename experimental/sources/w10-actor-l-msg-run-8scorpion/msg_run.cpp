#include "../w10-actor-l-common/access.hpp"
// Observed msg_run__8Scorpion at 0x001f2418, 1456 bytes. Per-frame step.
//  1. newDamageHandler(object, model) with model = word +12 of the group at the word at +36;
//     everyFrame(object, model) nonzero ends the call; objectActivationCheck(object, 600) zero
//     ends the call; advance = creatureAdvanceAnimation(object, 1.0); slideBack(object, 0.5); the
//     table at the word at +72 (entries of {signed halfword adjustment, function word at +4}) has
//     an entry at table offset 168 called with the adjusted object only.
//  2. The signed halfword at +572 selects a case (unsigned below 16, anything else is nothing).
//     Call184(a) means the table entry at offset 184 called with the adjusted object and a.
//     0: enemyInZone(object, 400.0, 32767) nonzero or enemyInZone(object, 500.0, 16000)
//        nonzero -> Call184(3); else Call184(1) when word +256 < word +808 (signed).
//     1: enemyInZone(400.0, 32767) nonzero or enemyInZone(500.0, 16000) nonzero -> Call184(0);
//        else smartWander(object).
//     3: when word +576 is zero Call184(0); else with x = float at +28 plus 20.0: enemyInZone(x,
//        6000) nonzero -> Call184(6); else enemyInZone(x, 32767) nonzero -> turnToFaceEnemy, else
//        steerAndRouteToEnemy.
//     5: enemyInZone(x, 6000) nonzero -> Call184(6); else bExitStatePersueIdle nonzero ->
//        Call184(3).
//     6, 13: melee branch (see the code): when word +576 is nonzero and bit 0 of `advance` is
//        set, produceMeleeDamage(object, 36.0, halfwords +200, +202, +204, mode, description).
//        State 13 passes mode 0x10004 and a locally built item description, after dividing
//        the halfwords at +200 and +202 by 4 (gameDifficulty 2) or 8 (gameDifficulty 3);
//        other states pass mode 8 and no description. Then turnToFaceEnemy when bit 3 of the
//        halfword at +294 is set (and word +576 was nonzero); then, unless that bit is set, word
//        +256 is decremented; when it is then <= 0, a draw r = eRand(object): bit 0 ->
//        Call184(14), else bit 1 -> Call184(15), else Call184(3).
//     8: Call184(3) unless bit 4 of the halfword at +294 is set.
//     9: Call184(0) when word +804 < word +808.
//    10: objectRemoveFromRunList when bit 7 of the halfword at +294 is set; then when bit 0 of
//        `advance` is set and bit 1 of the word at +12 is clear, floorAdd at the Point3
//        (x + icos(halfword +56), y + isin(halfword +56), z) with the (short) of a first eRand draw,
//        the float (second draw % 10 + 25), 6 and 1.
//    14, 15: move2(object, zero vector); then Call184(3) unless bit 3 of the halfword at +294 is set.
//     2, 4, 7, 11, 12 and 16 or more: nothing.
// Unknown: state names and the meaning of the fields; the layout of the item description beyond
// the initialised bytes. Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float step);
void slideBack__8Creaturef(void* self, float amount);
void* enemyInZone__8Creaturefs(void* self, float range, short value);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* f);
void move2__8CreatureG6Point3(void* self, const void* position);
void objectRemoveFromRunList__FP10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
unsigned eRand__10GameObject(void* self);
void floorAdd__FG6Point3sfii(const void* position, short a, float b, int c, int d);
extern int gameDifficulty;

void msg_run__8Scorpion(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    void* const model = w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    if (everyFrame__8CreatureP12_modelHeader(self, w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    w10l::call_entry(o, 168);
    const auto call184 = [&](unsigned long argument) { w10l::call_entry(o, 184, argument); };
    switch (static_cast<unsigned>(static_cast<int>(w10l::half(o, 572)))) {
    case 0:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != nullptr) { call184(3); break; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000) != nullptr) { call184(3); break; }
        if (w10l::sword(o, 256) < w10l::sword(o, 808)) call184(1);
        break;
    case 1:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != nullptr) { call184(0); break; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000) != nullptr) { call184(0); break; }
        smartWander__8Creature(self);
        break;
    case 3:
        if (w10l::word(o, 576) == 0) { call184(0); break; }
        if (enemyInZone__8Creaturefs(self, w10l::real(o, 28) + 20.0f, 6000) != nullptr) { call184(6); break; }
        if (enemyInZone__8Creaturefs(self, w10l::real(o, 28) + 20.0f, 32767) != nullptr) turnToFaceEnemy__8Creature(self);
        else steerAndRouteToEnemy__8Creature(self);
        break;
    case 5:
        if (enemyInZone__8Creaturefs(self, w10l::real(o, 28) + 20.0f, 6000) != nullptr) { call184(6); break; }
        if (bExitStatePersueIdle__8Creature(self) != 0) call184(3);
        break;
    case 6:
    case 13: {
        if (w10l::word(o, 576) != 0) {
            if ((advance & 1) != 0) {
                if (static_cast<short>(w10l::half(o, 572)) == 13) {
                    unsigned char description[64];
                    for (unsigned slot = 0; slot < 4; ++slot) *reinterpret_cast<short*>(description + 36 + slot * 8) = -1;
                    description[32] = 0xff;
                    description[33] = 0xff;
                    for (unsigned at = 16; at < 32; at += 2) *reinterpret_cast<short*>(description + at) = 0;
                    if (gameDifficulty == 2) {
                        const int first = w10l::half(o, 200);
                        const int second = w10l::half(o, 202);
                        w10l::half(o, 200) = static_cast<short>(first / 4);
                        w10l::half(o, 202) = static_cast<short>(second / 4);
                    } else if (gameDifficulty == 3) {
                        const int first = w10l::half(o, 200);
                        const int second = w10l::half(o, 202);
                        w10l::half(o, 200) = static_cast<short>(first / 8);
                        w10l::half(o, 202) = static_cast<short>(second / 8);
                    }
                    for (unsigned slot = 0; slot < 4; ++slot) {
                        short* const entry = reinterpret_cast<short*>(description + 36 + slot * 8);
                        if (entry[0] == -1 || entry[0] == 23) {
                            entry[0] = 23;
                            entry[1] = w10l::half(o, 200);
                            entry[2] = w10l::half(o, 202);
                            entry[3] = 0;
                            break;
                        }
                    }
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                        self, 36.0f, w10l::half(o, 200), w10l::half(o, 202), w10l::half(o, 204), 0x10004, description);
                } else {
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                        self, 36.0f, w10l::half(o, 200), w10l::half(o, 202), w10l::half(o, 204), 8, nullptr);
                }
            }
            if ((static_cast<unsigned short>(w10l::half(o, 294)) & 8u) != 0) turnToFaceEnemy__8Creature(self);
        }
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 8u) == 0) w10l::sword(o, 256) = w10l::sword(o, 256) - 1;
        if (w10l::sword(o, 256) > 0) break;
        const unsigned roll = eRand__10GameObject(self);
        if ((roll & 1u) != 0) call184(14);
        else if ((roll & 2u) != 0) call184(15);
        else call184(3);
        break;
    }
    case 8:
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 0x10u) == 0) call184(3);
        break;
    case 9:
        if (w10l::sword(o, 804) < w10l::sword(o, 808)) call184(0);
        break;
    case 10: {
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
        if ((advance & 1) == 0) break;
        if ((w10l::word(o, 12) & 2u) != 0) break;
        const float cosine = icos__Fi(w10l::half(o, 56));
        Point3 position;
        position.x = w10l::real(o, 16) + cosine;
        const float sine = isin__Fi(w10l::half(o, 56));
        position.y = w10l::real(o, 20) + sine;
        position.z = w10l::real(o, 24);
        const unsigned first = eRand__10GameObject(self);
        const unsigned second = eRand__10GameObject(self);
        floorAdd__FG6Point3sfii(&position, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 25u)), 6, 1);
        break;
    }
    case 14:
    case 15: {
        Point3 zero = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, &zero);
        if ((static_cast<unsigned short>(w10l::half(o, 294)) & 8u) == 0) call184(3);
        break;
    }
    default:
        break;
    }
}
}
