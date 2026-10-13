#include "../w10-actor-b-common/fields.hpp"
// Observed msg_run__10Arenabeast at 0x2357f0, 3288 bytes. Common start, in order:
// newDamageHandler(object, model) and everyFrame(object, model) with model the word at +12 of the
// record whose address is the word at +36 (re-read for each); a nonzero everyFrame result returns.
// objectActivationCheck(object, 600); the result of creatureAdvanceAnimation(1.0) is kept ("done");
// slideBack(0.0); the virtual entry at byte offset 168 (adjusted this only); move2(object, zero
// vector); then the signed halfword at +572 is the state, an unsigned compare against 24 selects
// a block from the table at 0x472710. "goto N" is the virtual entry at byte offset 184 with the
// adjusted this and the state number. Words: +256, +804, +808 are timers (signed compares), +576
// is the target pointer word, +960 the current attack kind, +964/+968 counters, +294 a halfword of
// flag bits, +28 a single (radius), +56 the facing halfword, +200/+202/+204 signed halfwords.
// "zone2(a, b, angle, range)" is enemyInZone2(object, a, b, angle, range, 0.6, 0) and
// "alt(a, b, angle, range)" is alternateEnemyInZone(object, a, b, angle, range); angles are the
// facing plus the offset shown (kept as signed halfwords), r is the single at +28.
//  0: an enemy within 400 (angle 32767) goes to 3; otherwise +256 below +808 goes to 1.
//  1: an enemy within 400 (angle 32767) goes to 3; otherwise +256 below +808 goes to 0 else smartWander.
//  2, 4, 7, 11, 12 and states of 24 or more: nothing.
//  3: no target goes to 0. Otherwise, in order, the first success enters the state shown:
//     zone2(0, r+20, +0, 4000) 21 (skipped when the kind +960 is already 21); zone2(0, r+20, +16384, 8192)
//     22 (skipped when 22); zone2(0, r+20, -16384, 8192) 23 (skipped when 23); zone2(0, r+50, +0, 4000) 6;
//     zone2(0, 150, +0, 2000) 18; zone2(0, 200, +0, 1000) 13; zone2(150, 200, +8192, 2000) 16 (skipped when
//     16); zone2(150, 200, -8192, 2000) 17 (skipped when 17); alt(0, r+20, +0, 4000) 21 (skipped when 21);
//     alt(0, r+20, +16384, 8192) 22; alt(0, r+20, -16384, 8192) 23 (the last two skipped likewise);
//     alt(r+50, 150, +0, 2000) 18; alt(150, 200, +0, 1000) 13; alt(0, 150, +24767, 2000) 14 (skipped when
//     14); alt(0, 150, -24769, 2000) 15 (skipped when 15); alt(150, 200, +8192, 4000) 16 and
//     alt(150, 200, -8192, 4000) 17 (skipped when equal). Then an enemy within 300 (angle 32767)
//     is faced, else steerAndRouteToEnemy.
//  5: without a target, bExitStatePersueIdle nonzero goes to 3. With one: zone2(0, r+50, +0, 4000) 6;
//     zone2(0, 150, +0, 4000) 18; zone2(0, 200, +0, 1000) 13; zone2(150, 200, +8192, 4000) 16;
//     zone2(150, 200, -8192, 4000) 17; alt(r+50, 150, +0, 4000) 18; alt(150, 200, +0, 1000) 13;
//     alt(0, 150, +24767, 4000) 14 and alt(0, 150, -24769, 4000) 15 (skipped when the kind equals the
//     state); alt(150, 200, +8192, 4000) 16; alt(150, 200, -8192, 4000) 17; then the exit test as above.
//  6, 18, 13: when done has bit 0, produceMeleeDamage(36 / 150 / 200, halfwords +200/+202/+204, 8, none).
//  14, 15, 16, 17: when done has bit 0, produceGeneralDamage(0 / 0 / 150 / 150, 150 / 150 / 200 / 200,
//     facing +24767 / -24769 / +8192 / -8192, 4000, the three halfwords, 8, none). After any of these
//     seven, bit 8 of +294 clear goes to 3.
//  8: bit 0x10 of +294 clear goes to 0.   9: +804 below +808 goes to 3.
//  10: bit 0x80 of +294 set: soundFadeStream(0) and leaves the run list. Then, when done has bit 4
//      (0x10) and bit 2 of the flag word at +12 is clear, a decal: floorAdd(position, (short)eRand,
//      40 + eRand % 10, 0, 1).
//  19: bit 0x10 of +294 clear goes to 3.
//  20: bit 0x10 of +294 clear goes to 3, then +964 = word +808 + eRand % 300 + 600.
//  21, 22, 23: when done has bit 0, produceMeleeDamage(150, three halfwords, 8, none); done bit 4
//      (0x10) sets +968 = 1; bit 8 of +294 clear goes to 3.
// Inferred: the 0.6 single is 0x3f199999 (one ulp under the nearest value). Unknown: the state and
// kind names and the meaning of +960, +964, +968.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, const float* point);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short angle, short range, float c, int d);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short range);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int flags,
                                                           void* description);
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short angle,
                                                                short range, int c, int d, int e, int flags,
                                                                void* description);
void soundFadeStream__Fi(int stream);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const float* point, short angle, float extent, int kind, int extra);
unsigned eRand__10GameObject(void* self);
}

namespace {
using namespace actor_b;
using rtti_c::single_from_bits;

__attribute__((always_inline)) inline void go(void* self, int state) {
    void* adjusted;
    rtti_c::slot_function<void (*)(void*, int)>(self, 184, &adjusted)(adjusted, state);
}
__attribute__((always_inline)) inline short facing(void* self, unsigned offset) {
    return static_cast<short>(field<Half>(self, 56) + offset);
}
__attribute__((always_inline)) inline bool zone2(void* self, float a, float b, short angle, short range) {
    return enemyInZone2__8Creatureffssfi(self, a, b, angle, range, single_from_bits(0x3f199999u), 0) != 0;
}
__attribute__((always_inline)) inline bool alt(void* self, float a, float b, short angle, short range) {
    return alternateEnemyInZone__8Creatureffss(self, a, b, angle, range) != nullptr;
}
__attribute__((always_inline)) inline int kind(void* self) { return static_cast<int>(field<Word>(self, 960)); }
__attribute__((always_inline)) inline void melee(void* self, float amount) {
    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, amount, field<SHalf>(self, 200), field<SHalf>(self, 202), field<SHalf>(self, 204), 8, nullptr);
}
__attribute__((always_inline)) inline void general(void* self, float a, float b, short angle) {
    produceGeneralDamage__8CreatureffssiiiiPQ24item11description(self, a, b, angle, 4000, field<SHalf>(self, 200), field<SHalf>(self, 202), field<SHalf>(self, 204), 8, nullptr);
}
}

extern "C" void msg_run__10Arenabeast(void* self) {
    newDamageHandler__8CreatureP12_modelHeader(self, link(link(self, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, link(link(self, 36), 12)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const int done = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.0f);
    {
        void* adjusted;
        rtti_c::slot_function<void (*)(void*)>(self, 168, &adjusted)(adjusted);
    }
    const float zero_point[3] = {0.0f, 0.0f, 0.0f};
    move2__8CreatureG6Point3(self, zero_point);
    const unsigned state = static_cast<unsigned>(static_cast<int>(field<SHalf>(self, 572)));
    if (state >= 24) return;
    const float r20 = field<Single>(self, 28) + 20.0f;
    switch (state) {
    case 0:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) { go(self, 3); return; }
        if (!(field<SWord>(self, 256) < field<SWord>(self, 808))) return;
        go(self, 1);
        return;
    case 1:
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) { go(self, 3); return; }
        if (field<SWord>(self, 256) < field<SWord>(self, 808)) { go(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 3:
        if (field<Word>(self, 576) == 0) { go(self, 0); return; }
        if (kind(self) != 21 && zone2(self, 0.0f, r20, facing(self, 0), 4000)) { go(self, 21); return; }
        if (kind(self) != 22 && zone2(self, 0.0f, field<Single>(self, 28) + 20.0f, facing(self, 16384), 8192)) { go(self, 22); return; }
        if (kind(self) != 23 && zone2(self, 0.0f, field<Single>(self, 28) + 20.0f, facing(self, -16384), 8192)) { go(self, 23); return; }
        if (zone2(self, 0.0f, field<Single>(self, 28) + 50.0f, facing(self, 0), 4000)) { go(self, 6); return; }
        if (zone2(self, 0.0f, 150.0f, facing(self, 0), 2000)) { go(self, 18); return; }
        if (zone2(self, 0.0f, 200.0f, facing(self, 0), 1000)) { go(self, 13); return; }
        if (kind(self) != 16 && zone2(self, 150.0f, 200.0f, facing(self, 8192), 2000)) { go(self, 16); return; }
        if (kind(self) != 17 && zone2(self, 150.0f, 200.0f, facing(self, -8192), 2000)) { go(self, 17); return; }
        if (kind(self) != 21 && alt(self, 0.0f, field<Single>(self, 28) + 20.0f, facing(self, 0), 4000)) { go(self, 21); return; }
        if (kind(self) != 22 && alt(self, 0.0f, field<Single>(self, 28) + 20.0f, facing(self, 16384), 8192)) { go(self, 22); return; }
        if (kind(self) != 23 && alt(self, 0.0f, field<Single>(self, 28) + 20.0f, facing(self, -16384), 8192)) { go(self, 23); return; }
        if (alt(self, field<Single>(self, 28) + 50.0f, 150.0f, facing(self, 0), 2000)) { go(self, 18); return; }
        if (alt(self, 150.0f, 200.0f, facing(self, 0), 1000)) { go(self, 13); return; }
        if (kind(self) != 14 && alt(self, 0.0f, 150.0f, facing(self, 24767), 2000)) { go(self, 14); return; }
        if (kind(self) != 15 && alt(self, 0.0f, 150.0f, facing(self, -24769), 2000)) { go(self, 15); return; }
        if (kind(self) != 16 && alt(self, 150.0f, 200.0f, facing(self, 8192), 4000)) { go(self, 16); return; }
        if (kind(self) != 17 && alt(self, 150.0f, 200.0f, facing(self, -8192), 4000)) { go(self, 17); return; }
        if (enemyInZone__8Creaturefs(self, 300.0f, 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
            return;
        }
        steerAndRouteToEnemy__8Creature(self);
        return;
    case 5:
        if (field<Word>(self, 576) != 0) {
            if (zone2(self, 0.0f, field<Single>(self, 28) + 50.0f, facing(self, 0), 4000)) { go(self, 6); return; }
            if (zone2(self, 0.0f, 150.0f, facing(self, 0), 4000)) { go(self, 18); return; }
            if (zone2(self, 0.0f, 200.0f, facing(self, 0), 1000)) { go(self, 13); return; }
            if (zone2(self, 150.0f, 200.0f, facing(self, 8192), 4000)) { go(self, 16); return; }
            if (zone2(self, 150.0f, 200.0f, facing(self, -8192), 4000)) { go(self, 17); return; }
            if (alt(self, field<Single>(self, 28) + 50.0f, 150.0f, facing(self, 0), 4000)) { go(self, 18); return; }
            if (alt(self, 150.0f, 200.0f, facing(self, 0), 1000)) { go(self, 13); return; }
            if (kind(self) != 14 && alt(self, 0.0f, 150.0f, facing(self, 24767), 4000)) { go(self, 14); return; }
            if (kind(self) != 15 && alt(self, 0.0f, 150.0f, facing(self, -24769), 4000)) { go(self, 15); return; }
            if (alt(self, 150.0f, 200.0f, facing(self, 8192), 4000)) { go(self, 16); return; }
            if (alt(self, 150.0f, 200.0f, facing(self, -8192), 4000)) { go(self, 17); return; }
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        go(self, 3);
        return;
    case 6:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        if ((done & 1) != 0) {
            switch (state) {
            case 6: melee(self, 36.0f); break;
            case 18: melee(self, 150.0f); break;
            case 13: melee(self, 200.0f); break;
            case 14: general(self, 0.0f, 150.0f, facing(self, 24767)); break;
            case 15: general(self, 0.0f, 150.0f, facing(self, -24769)); break;
            case 16: general(self, 150.0f, 200.0f, facing(self, 8192)); break;
            default: general(self, 150.0f, 200.0f, facing(self, -8192)); break;
            }
        }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        go(self, 3);
        return;
    case 21:
    case 22:
    case 23:
        if ((done & 1) != 0) melee(self, 150.0f);
        if ((done & 0x10) != 0) field<Word>(self, 968) = 1;
        if ((field<Half>(self, 294) & 8u) != 0) return;
        go(self, 3);
        return;
    case 8:
        if ((field<Half>(self, 294) & 0x10u) != 0) return;
        go(self, 0);
        return;
    case 9:
        if (!(field<SWord>(self, 804) < field<SWord>(self, 808))) return;
        go(self, 3);
        return;
    case 20:
        if ((field<Half>(self, 294) & 0x10u) != 0) return;
        go(self, 3);
        field<Word>(self, 964) = field<Word>(self, 808) + eRand__10GameObject(self) % 300u + 600u;
        return;
    case 19:
        if ((field<Half>(self, 294) & 0x10u) != 0) return;
        go(self, 3);
        return;
    case 10: {
        if ((field<Half>(self, 294) & 0x80u) != 0) {
            soundFadeStream__Fi(0);
            objectRemoveFromRunList__FP10GameObject(self);
        }
        if ((done & 0x10) == 0) return;
        if ((field<Word>(self, 12) & 2u) != 0) return;
        float point[3] __attribute__((may_alias));
        const Word* source = &field<Word>(self, 16);
        Word* copy = reinterpret_cast<Word*>(point);
        copy[0] = source[0];
        copy[1] = source[1];
        copy[2] = source[2];
        const short angle = static_cast<short>(eRand__10GameObject(self));
        const unsigned extent = eRand__10GameObject(self) % 10u;
        floorAdd__FG6Point3sfii(point, angle, static_cast<float>(static_cast<int>(extent + 40u)), 0, 1);
        return;
    }
    default:
        return;
    }
}
