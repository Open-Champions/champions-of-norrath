#include "../w10-actor-b-common/fields.hpp"
// Observed msg_run__8AntQueen at 0x206ea8, 2540 bytes. Common start, in order:
// newDamageHandler(object, model) and everyFrame(object, model) where model is the word at +12
// of the record whose address is the word at +36 (re-read before each call); a nonzero everyFrame
// result returns. objectActivationCheck(object, 600); the result of creatureAdvanceAnimation(1.0)
// is kept ("done", tested for bit 0 below); slideBack(0.5); the virtual entry at byte offset 168
// (adjusted this, no other argument); then the signed halfword at +572 is the state and an
// unsigned compare against 20 picks a block from the table at 0x46fc60. Entering a state is
// the virtual entry at byte offset 184 with the adjusted this and the state number ("goto N").
// Words: +256, +716, +804, +808, +964, +972 are timers/counters (signed compares), +576 is a
// pointer word (the target), +292 is a signed byte, +294 a halfword of flag bits.
//  0: subTurnAnimation; an enemy within 400 (any angle 32767) with the byte +292 below 4, or within
//     500 (angle 16000) with it below 4, goes to 3; otherwise, when word +256 is below +808, goes to 1.
//  1: addTurnAnimation; an enemy within 400/32767 or 500/16000 goes to 0; otherwise smartWander.
//  3: addTurnAnimation; no target (+576 zero) goes to 0. With eRand & 3 nonzero: an enemy within
//     single(+28) + 20 (angle 6000) goes to 6; with zero: alternateEnemyInZone(0, single(+28) + 40,
//     halfword(+56) - 32767, 6000) goes to 19. Otherwise steerAndRouteToEnemy; then, when word +972
//     is below +808 and the byte +292 is below 4, goes to 14.
//  5: the same eRand & 3 split with the target pointer required first (nonzero: go to 6 on the zone
//     test; zero: go to 19 on the alternate test); otherwise bExitStatePersueIdle nonzero goes to 3.
//  6: subTurnAnimation; with a target: when done has bit 0, produceMeleeDamage(36, three signed
//     halfwords +200/+202/+204, 0x40008, none) and word +964 = 1; with +964 zero turnToFaceEnemy;
//     then, when bit 8 of +294 is clear, goes to 3.
//  8: subTurnAnimation; when bit 0x10 of +294 is clear and the byte +292 is below 4, goes to 3.
//  9: when word +804 is below +808, goes to 3.
//  10: subTurnAnimation; bit 0x80 of +294 set: soundFadeStream(0), then leaves the run list. Then,
//     when done has bit 0 and bit 2 of the flag word at +12 is clear, a floor decal: floorAdd(position
//     moved by icos/isin of the angle, (short)eRand, 15 + (eRand & 3), 6, 1).
//  13: addTurnAnimation; no target goes to 0; when word +972 is below +808: turnToFaceEnemy and
//     an enemy within 500/3000 goes to 17; otherwise (unless bit 8 of +294) the point at +272 is the
//     steering target: arrival or failure of steerAndRouteToPosition clears +972; so does word
//     +716 of 31 or more.
//  14: addTurnAnimation; no target goes to 0; word +972 below +808 goes to 13; else (bit 8 of +294
//     clear) an enemy within single(+28) + 20/6000 goes to 15 and then, one time in three, clears
//     +972; otherwise an enemy within single(+28) + 20 (angle 32767) turns to face it, else
//     steerAndRouteToEnemy.
//  15, 16: subTurnAnimation; with a target and done bit 0, builds an item description with one
//     effect (type 23, magnitude from a skill-object virtual call) and calls produceMeleeDamage(40,
//     halfwords +200/+202/+204, 0x10004, description); turnToFaceEnemy; unless bit 8 of +294, goes to
//     13 from state 16 and to 14 from 15 (also when there is no target).
//  17: subTurnAnimation; bit 8 of +294 clear and the byte +292 below 4 goes to 3.
//  18: subTurnAnimation; with a target and word +808 equal to +256 minus 120: an AntWave (976 bytes)
//     is created at the position moved ten past the radius (single +28 + 10) along the facing,
//     owned by the object stored in the node of +576, angle halfword +56, argument 600; then,
//     when word +256 is below +808 and the byte +292 is below 4, goes to 3.
//  19: subTurnAnimation; when done has bit 0, an item description with one effect (as above)
//     and produceGeneralDamage(0, 40, halfword(+56) + 32767, 3000, halfwords +200/+202/+204,
//     0x40008, description); then, with bit 8 of +294 clear, goes to 3.
//  2, 4, 7, 11, 12 and states of 20 or more: nothing.
// The item description's initialised fields are those the original writes: halfwords of -1 at
// +36/+44/+52/+60, bytes 0xff at +32/+33, zero halfwords +16..+22 and +24..+30; the effect slot
// is searched at +36 + 8k for an empty (-1) or type-23 slot among four, then its type, two copies
// of the magnitude and a zero are stored. Unknown: what the states are named, and the roles of
// +256, +716, +804, +808, +964 and +972.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void subTurnAnimation__8AntQueen(void* self);
void addTurnAnimation__8AntQueen(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void smartWander__8Creature(void* self);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short range);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const float* point);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int flags,
                                                           void* description);
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short angle,
                                                                short range, int c, int d, int e, int flags,
                                                                void* description);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int bytes);
void __7AntWaveP10GameObjectG6Point3si(void* block, void* owner, const float* point, short angle, int argument);
void soundFadeStream__Fi(int stream);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const float* point, short angle, float extent, int kind, int extra);
unsigned eRand__10GameObject(void* self);
extern unsigned skills[];
}

namespace {
using namespace actor_b;

__attribute__((always_inline)) inline void go(void* self, int state) {
    void* adjusted;
    rtti_c::slot_function<void (*)(void*, int)>(self, 184, &adjusted)(adjusted, state);
}

// Fills the stack description the way the original does, then adds the type-23 effect.
__attribute__((always_inline)) inline void build_description(void* self, unsigned char* description) {
    for (unsigned k = 0; k < 4; ++k) field<Half>(description, 60 - 8 * k) = 0xffffu;
    description[33] = 0xff;
    description[32] = 0xff;
    for (unsigned k = 0; k < 4; ++k) {
        field<Half>(description, 16 + 2 * k) = 0;
        field<Half>(description, 24 + 2 * k) = 0;
    }
    void* skill = rtti_c::pointer(skills[26]);
    void* table = rtti_c::pointer(field<Word>(skill, 96));
    void* entry = static_cast<unsigned char*>(table) + 96;
    typedef int (*SkillFunction)(void*, int, int);
    const int amount = reinterpret_cast<SkillFunction>(rtti_c::pointer(field<Word>(entry, 4)))(
        static_cast<unsigned char*>(skill) + field<SHalf>(entry, 0), 0, 1);
    (void)self;
    for (int index = 0; index < 4; ++index) {
        const unsigned base = 36 + 8 * static_cast<unsigned>(index);
        const int type = field<SHalf>(description, base);
        if (type == -1 || type == 23) {
            field<Half>(description, base) = 23;
            field<Half>(description, base + 6) = 0;
            field<Half>(description, base + 4) = static_cast<unsigned short>(amount);
            field<Half>(description, base + 2) = static_cast<unsigned short>(amount);
            break;
        }
    }
}

__attribute__((always_inline)) inline float reach(void* self) { return field<Single>(self, 28) + 20.0f; }
__attribute__((always_inline)) inline short turned(void* self) {
    return static_cast<short>(field<Half>(self, 56) - 32767u);
}
__attribute__((always_inline)) inline bool low_threat(void* self) { return field<SByte>(self, 292) < 4; }
}

extern "C" void msg_run__8AntQueen(void* self) {
    newDamageHandler__8CreatureP12_modelHeader(self, link(link(self, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, link(link(self, 36), 12)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const int done = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    {
        void* adjusted;
        rtti_c::slot_function<void (*)(void*)>(self, 168, &adjusted)(adjusted);
    }
    const unsigned state = static_cast<unsigned>(static_cast<int>(field<SHalf>(self, 572)));
    switch (state) {
    case 0:
        subTurnAnimation__8AntQueen(self);
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 && low_threat(self)) { go(self, 3); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0 && low_threat(self)) { go(self, 3); return; }
        if (!(field<SWord>(self, 256) < field<SWord>(self, 808))) return;
        go(self, 1);
        return;
    case 1:
        addTurnAnimation__8AntQueen(self);
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) { go(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0) { go(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 3:
        addTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) == 0) { go(self, 0); return; }
        if ((eRand__10GameObject(self) & 3u) != 0) {
            if (enemyInZone__8Creaturefs(self, reach(self), 6000) != 0) { go(self, 6); return; }
        } else if (alternateEnemyInZone__8Creatureffss(self, 0.0f, field<Single>(self, 28) + 40.0f, turned(self), 6000) != nullptr) {
            go(self, 19);
            return;
        }
        steerAndRouteToEnemy__8Creature(self);
        if (!(field<SWord>(self, 972) < field<SWord>(self, 808))) return;
        if (!low_threat(self)) return;
        go(self, 14);
        return;
    case 5: {
        const unsigned roll = eRand__10GameObject(self) & 3u;
        if (roll != 0) {
            if (field<Word>(self, 576) != 0 && enemyInZone__8Creaturefs(self, reach(self), 6000) != 0) { go(self, 6); return; }
        } else if (field<Word>(self, 576) != 0) {
            if (alternateEnemyInZone__8Creatureffss(self, 0.0f, field<Single>(self, 28) + 40.0f, turned(self), 6000) != nullptr) { go(self, 19); return; }
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        go(self, 3);
        return;
    }
    case 13: {
        addTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) == 0) { go(self, 0); return; }
        if (field<SWord>(self, 972) < field<SWord>(self, 808)) {
            turnToFaceEnemy__8Creature(self);
            if (enemyInZone__8Creaturefs(self, 500.0f, 3000) != 0) go(self, 17);
            return;
        }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        float target[3] __attribute__((may_alias));
        const Word* source = &field<Word>(self, 272);
        Word* copy = reinterpret_cast<Word*>(target);
        copy[0] = source[0];
        copy[1] = source[1];
        copy[2] = source[2];
        if (steerAndRouteToPosition__8CreatureG6Point3(self, target) != 0) {
            field<Word>(self, 972) = 0;
            return;
        }
        if (field<SWord>(self, 716) < 31) return;
        field<Word>(self, 972) = 0;
        return;
    }
    case 14:
        addTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) == 0) { go(self, 0); return; }
        if (field<SWord>(self, 972) < field<SWord>(self, 808)) { go(self, 13); return; }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        if (enemyInZone__8Creaturefs(self, reach(self), 6000) != 0) {
            go(self, 15);
            if (eRand__10GameObject(self) % 3u != 0) return;
            field<Word>(self, 972) = 0;
            return;
        }
        if (enemyInZone__8Creaturefs(self, reach(self), 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
            return;
        }
        steerAndRouteToEnemy__8Creature(self);
        return;
    case 17:
        subTurnAnimation__8AntQueen(self);
        if ((field<Half>(self, 294) & 8u) != 0) return;
        if (!low_threat(self)) return;
        go(self, 3);
        return;
    case 6:
        subTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) != 0) {
            if ((done & 1) != 0) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, field<SHalf>(self, 200), field<SHalf>(self, 202), field<SHalf>(self, 204), 0x40008, nullptr);
                field<Word>(self, 964) = 1;
            }
            if (field<Word>(self, 964) == 0) turnToFaceEnemy__8Creature(self);
        }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        go(self, 3);
        return;
    case 19: {
        subTurnAnimation__8AntQueen(self);
        if ((done & 1) != 0) {
            unsigned char description[64];
            build_description(self, description);
            produceGeneralDamage__8CreatureffssiiiiPQ24item11description(self, 0.0f, 40.0f, static_cast<short>(field<Half>(self, 56) + 32767u), 3000, field<SHalf>(self, 200), field<SHalf>(self, 202), field<SHalf>(self, 204), 0x40008, description);
        }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        go(self, 3);
        return;
    }
    case 18: {
        subTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) != 0 && field<SWord>(self, 808) == field<SWord>(self, 256) - 120) {
            const float radius = field<Single>(self, 28) + 10.0f;
            float point[3];
            point[0] = field<Single>(self, 16) + icos__Fi(field<SHalf>(self, 56)) * radius;
            const float radius_again = field<Single>(self, 28) + 10.0f;
            point[1] = field<Single>(self, 20) + isin__Fi(field<SHalf>(self, 56)) * radius_again;
            point[2] = field<Single>(self, 24);
            void* block = blockAlloc__Fi(976);
            if (block != nullptr) {
                __7AntWaveP10GameObjectG6Point3si(block, link(link(self, 576), 12), point, field<SHalf>(self, 56), 600);
            }
        }
        if (!(field<SWord>(self, 256) < field<SWord>(self, 808))) return;
        if (!low_threat(self)) return;
        go(self, 3);
        return;
    }
    case 8:
        subTurnAnimation__8AntQueen(self);
        if ((field<Half>(self, 294) & 0x10u) != 0) return;
        if (!low_threat(self)) return;
        go(self, 3);
        return;
    case 9:
        if (!(field<SWord>(self, 804) < field<SWord>(self, 808))) return;
        go(self, 3);
        return;
    case 15:
    case 16:
        subTurnAnimation__8AntQueen(self);
        if (field<Word>(self, 576) != 0) {
            if ((done & 1) != 0) {
                unsigned char description[64];
                build_description(self, description);
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 40.0f, field<SHalf>(self, 200), field<SHalf>(self, 202), field<SHalf>(self, 204), 0x10004, description);
            }
            turnToFaceEnemy__8Creature(self);
        }
        if ((field<Half>(self, 294) & 8u) != 0) return;
        go(self, field<SHalf>(self, 572) == 16 ? 13 : 14);
        return;
    case 10:
        subTurnAnimation__8AntQueen(self);
        if ((field<Half>(self, 294) & 0x80u) != 0) {
            soundFadeStream__Fi(0);
            objectRemoveFromRunList__FP10GameObject(self);
        }
        if ((done & 1) == 0) return;
        if ((field<Word>(self, 12) & 2u) != 0) return;
        {
            float point[3];
            point[0] = field<Single>(self, 16) + icos__Fi(field<SHalf>(self, 56));
            point[1] = field<Single>(self, 20) + isin__Fi(field<SHalf>(self, 56));
            point[2] = field<Single>(self, 24);
            const short angle = static_cast<short>(eRand__10GameObject(self));
            const unsigned extent = eRand__10GameObject(self) & 3u;
            floorAdd__FG6Point3sfii(point, angle, static_cast<float>(static_cast<int>(extent + 15u)), 6, 1);
        }
        return;
    default:
        return;
    }
}
