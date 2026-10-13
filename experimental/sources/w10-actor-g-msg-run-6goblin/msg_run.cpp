// Observed msg_run__6Goblin at 0x0022c998, 5264 bytes, with a 16-entry jump table at 0x471f60.
// t is the table at word +36 (t[12] the model header); S the signed halfword at +572; P the word at
// +972; Q the word at +992; L the word at +576 (a link whose word +12 is the target object, so "the
// target" below is the object at L+12); F the result of creatureAdvanceAnimation; setState(v) the
// table entry at (word +72) + 184 called with the adjusted object and v; "call176" the entry at +176
// called with the adjusted object, 0 and a position.
// Common start: newDamageHandler(self, t[12]); everyFrame(self, t[12]) and return when nonzero;
// objectActivationCheck(self, 600) and return when zero; F = creatureAdvanceAnimation(self, 1.0);
// slideBack(self, 0.5) unless S is 8 (S is read before that call and read again after the next
// one); the table entry at +168 called with the adjusted object. S of 16 or more (unsigned) stops
// there. Z(a, b, s) below is enemyInZone and Z2 is enemyInZone2(self, 0.0, a, h56, s, 0.0, 1) where
// h56 is the halfword at +56.
//  S 0: when P is 4: Z2(300, 32767) -> setState(3); when P is still 4: Z2(400, 16000) -> setState(3).
//       Then Z(300, 32767) -> setState(3); Z(400, 16000) -> setState(3); else word +256 below word
//       +808 (signed) -> setState(1).
//  S 1: the same tests with setState(0); otherwise smartWander(self).
//  S 2: the same tests with setState(0); otherwise setState(0) when smartWander(self) returns zero.
//  S 14: when word +808 is below word +256 (signed) and L is nonzero, flee2(self, target); otherwise
//       setState(0).
//  S 3: L zero -> setState(0). P of 4 or 5: Z(90, 32767) -> setState(14); else Z2(400, 6000) ->
//       setState(6); else steerAndRouteToEnemy. Otherwise (P not 4 or 5), the steps below end in
//       copying the 12 bytes at target+16 to +960..971: Z(float(+28) + 30, 6000) -> setState(6);
//       Z(float(+28) + 30, 32767) -> turnToFaceEnemy; P above zero (signed) and objectIsCrowded(self,
//       target, 400.0, float(+28) * 3, 0) -> setState(13); then eRand % 30 != 0 -> steerAndRoute;
//       else with N = numInRegion(position of target, float(target+28) * 1.75): N of 3 or more ->
//       steerAndRoute; else with A = iatan2(target.y - y, target.x - x) (fields +20 and +16) minus
//       the signed halfword at target+56: (A + 16383) below 32767 (unsigned) -> setState(4), else
//       steerAndRoute.
//  S 4: L zero -> setState(0). Z(float(+28) + 30, 6000) -> setState(6). Else eRand % 30 != 0 ->
//       steerAndRoute; else when the distance of target (+16, +20) from the point stored at +960 is
//       below 0.99999994 -> setState(3), otherwise steerAndRoute. Always ends by copying the 12
//       bytes at target+16 to +960..971.
//  S 13: L zero -> setState(0). P of 5: Z(90, 32767) -> setState(14); else word +976 below word
//       +808 (signed) -> setState(3). Other P: word +256 below word +808 -> setState(3); else not
//       objectIsCrowded(self, target, 400.0, float(+28) * 3, 1) -> setState(3). Ends with
//       turnToFaceEnemy.
//  S 15: word +256 below word +808 (signed) -> setState(3).
//  S 5: L zero -> setState(3). P of 4 or 5: Z2(400, 6000) -> setState(6). If P is not 4 or 5 (read
//       again): Z(float(+28) + 30, 6000) -> setState(6) and stop. Then bExitStatePersueIdle nonzero
//       -> setState(3).
//  S 6: see the code; it is the ranged attack: with L nonzero and P of 4 (arrows), 5 (spells) or
//       otherwise (melee), then the shared timer step: when bit 3 (0x8) of halfword +294 is clear
//       word +256 is decremented; when word +256 is above zero (signed) the function ends; else with
//       P of 5 word +976 = word(+808) + eRand % 180 + 180 and setState(13), otherwise setState(3).
//  S 8: the 64-bit word at +288 having none of the bits 0x0030000000000000 -> setState(3).
//  S 9: word +804 below word +808 (signed) -> setState(3).
//  S 10: with bit 7 (0x80) of halfword +294 set, objectAddToSlowRunList. With bit 0 of F set and bit
//       1 of word +12 clear, floorAdd(position, (short)eRand, float(eRand % 10 + 20), 0, 1). With bit
//       1 of F set and bit 1 of word +12 clear, bit 0 of word +12 is set. When word +1136 is nonzero
//       and bit 7 of halfword +294 is set: float +1140 += 0.35 (bits 0x3eb33333), float +60 = float
//       (+24) - float(+1140), and when 24.0 < float(+1140) bits 0x4002 of word +12 are set and
//       objectAddToDeleteList(self).
//  S 7, 11, 12: nothing.
// Inferred: S 6 is an attack with arrows, spell objects or a melee blow. Unknown: what most states and
// fields mean. worldDifficulty, skills, _4item$globalItem and every callee are unresolved imports.
// Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short flags);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
int smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void flee2__8CreatureP10GameObject(void* self, void* target);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
int numInRegion__8creatureG6Point3f(Point3 position, float radius);
int iatan2__Fff(float y, float x);
float icos__Fi(int angle);
float isin__Fi(int angle);
unsigned eRand__10GameObject(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* description);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* target, float range);
void* ISeeDeadPeople__8Creaturef(void* self, float range);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short angle, void* animation, int a, Point3* out, Point3* out2, float f);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void ColdArrowProjectile_construct(void* self, Point3 position, short yaw, void* owner, int a, void* skill)
    __asm__("__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill");
void ExplodeArrowProjectile_construct(void* self, Point3 position, short yaw, void* owner, int a, void* skill)
    __asm__("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
void FireArrowProjectile_construct(void* self, Point3 position, short yaw, void* owner, int a, void* skill)
    __asm__("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
void MissileWeapon_construct(void* self, int a, void* model, void* texture, Point3 position, Point3 velocity, int yaw, void* owner,
                             void* sound1, void* sound2, int b, int c, int d, int e, int f, int g, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
void FrostStormSpell_construct(void* self, Point3 position, int a, float b, int c, void* owner, Point3 velocity)
    __asm__("__15FrostStormSpellG6Point3ifiP10GameObjectT1");
void ShockOfFrost_construct(void* self, Point3 position, short yaw, int a, void* owner, void* skill)
    __asm__("__12ShockOfFrostG6Point3siP10GameObjectP5Skill");
void ShockProjectile_construct(void* self, Point3 position, short yaw, void* owner, void* skill, int a)
    __asm__("__15ShockProjectileG6Point3sP10GameObjectP5Skilli");
void floorAdd__FG6Point3sfii(Point3 position, short heading, float radius, int a, int b);
void objectAddToSlowRunList__FP10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
extern float worldDifficulty;
extern unsigned skills[];
extern unsigned char globalItem[] __asm__("_4item$globalItem");
}

namespace {
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int& sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
float& flt(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
unsigned short half(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
short shalf(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;

void set_state(void* self, int value) {
    w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), value);
}
unsigned char* target_of(unsigned char* o) {
    return w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 576)) + 12));
}
void remember_target(unsigned char* o) {
    unsigned char* target = target_of(o);
    *reinterpret_cast<Point3*>(o + 960) = *reinterpret_cast<Point3*>(target + 16);
}
__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
int zone(void* self, float range, short flags) { return enemyInZone__8Creaturefs(self, range, flags); }
int zone2(unsigned char* o, float b, short d) {
    return enemyInZone2__8Creatureffssfi(o, 0.0f, b, shalf(o, 56), d, 0.0f, 1);
}
// The shared opening of states 0, 1 and 2: two archer tests and two plain tests, each leading to
// setState(value). True when one of them fired.
bool wander_tests(unsigned char* o, int value) {
    const unsigned p = word(o, 972);
    if (p == 4u) {
        if (zone2(o, 300.0f, 32767)) { set_state(o, value); return true; }
        if (word(o, 972) == p) {
            if (zone2(o, 400.0f, 16000)) { set_state(o, value); return true; }
        }
    }
    if (zone(o, 300.0f, 32767)) { set_state(o, value); return true; }
    if (zone(o, 400.0f, 16000)) { set_state(o, value); return true; }
    return false;
}
void call176(unsigned char* o, Point3 position) {
    w10g::fn_from_word<void (*)(void*, int, Point3)>(w10g::entry_function(o, 176))(w10g::adjusted(o, 176), 0, position);
}
int skill_roll(unsigned char* o, int which) {
    unsigned char* skill = w10g::from_word(skills[58]);
    unsigned char* e = w10g::from_word(*reinterpret_cast<unsigned*>(skill + 96)) + 104;
    return w10g::fn_from_word<int (*)(void*, int, int, void*)>(*reinterpret_cast<unsigned*>(e + 4))(
        skill + *reinterpret_cast<short*>(e), which, static_cast<int>(worldDifficulty), o);
}
void* skill_word(unsigned index) { return w10g::from_word(skills[index]); }

void set_lights(unsigned char* o, short a140, short a142, short a144, short a146, short a148, short a150, short a152, short a154) {
    *reinterpret_cast<short*>(o + 140) = a140;
    *reinterpret_cast<short*>(o + 142) = a142;
    *reinterpret_cast<short*>(o + 144) = a144;
    *reinterpret_cast<short*>(o + 146) = a146;
    *reinterpret_cast<short*>(o + 148) = a148;
    *reinterpret_cast<short*>(o + 150) = a150;
    *reinterpret_cast<short*>(o + 152) = a152;
    *reinterpret_cast<short*>(o + 154) = a154;
}

// S 6 with P of 4: arrows.
void archer(unsigned char* o, int frameFlags) {
    const float cosine = icos__Fi(shalf(o, 56));
    const float sine = isin__Fi(shalf(o, 56));
    Point3 target;
    target.x = flt(o, 16) + cosine * 300.0f;
    target.y = flt(o, 20) + sine * 300.0f;
    target.z = flt(o, 24);
    findEnemyLeadPosition__8CreaturefP6Point3(o, 400.0f, &target);
    if ((frameFlags & 1) != 0) {
        Point3 attach;
        attach.x = flt(o, 16);
        attach.y = flt(o, 20);
        attach.z = (flt(o, 24) + flt(o, 32) * 0.5f) + 12.0f;
        word(o, 996) = 0;
        Point3 scratch;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12)), *reinterpret_cast<Point3*>(o + 16),
            shalf(o, 56), o + 288, 0, &attach, &scratch, flt(o, 224));
        const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(o, target_of(o), 400.0f);
        Point3 velocity;
        velocity.x = icos__Fi(yaw) * 400.0f;
        velocity.y = isin__Fi(yaw) * 400.0f;
        const unsigned first = eRand__10GameObject(o);
        velocity.z = static_cast<float>(static_cast<int>(first % 60u) - 30);
        const unsigned second = eRand__10GameObject(o);
        char name[32];
        sprintf(name, "swsh_arrow%d.anm", static_cast<int>((second & 3u) + 1u));
        char lump[] = "fx.lmp";
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), o, 1.0f);
        const int kind = sword(o, 208);
        if (kind == 2) {
            void* block = blockAlloc__Fi(116);
            if (block != nullptr) ExplodeArrowProjectile_construct(block, attach, static_cast<short>(yaw), o, static_cast<int>(worldDifficulty), skill_word(47));
        } else if (kind == 1) {
            void* block = blockAlloc__Fi(176);
            if (block != nullptr) ColdArrowProjectile_construct(block, attach, static_cast<short>(yaw), o, static_cast<int>(worldDifficulty), skill_word(44));
        } else if (kind == 3) {
            void* block = blockAlloc__Fi(176);
            if (block != nullptr) FireArrowProjectile_construct(block, attach, static_cast<short>(yaw), o, static_cast<int>(worldDifficulty), skill_word(43));
        } else {
            void* block = blockAlloc__Fi(452);
            if (block != nullptr) {
                MissileWeapon_construct(block, 0, w10g::from_word(*reinterpret_cast<unsigned*>(globalItem + 8)),
                                        w10g::from_word(*reinterpret_cast<unsigned*>(globalItem + 12)), attach, velocity, yaw - 16384, o,
                                        nullptr, nullptr, shalf(o, 200), shalf(o, 202), 0, 0, shalf(o, 204), 4, nullptr);
            }
        }
    }
    if ((frameFlags & 2) != 0) word(o, 996) = 1;
    if ((half(o, 294) & 0x8u) != 0) call176(o, target);
}

// S 6 with P of 5: spells.
void caster(unsigned char* o, int frameFlags) {
    Point3 attach = *reinterpret_cast<Point3*>(o + 16);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12)), *reinterpret_cast<Point3*>(o + 16),
        shalf(o, 56), o + 288, 0, &attach, nullptr, 1.0f);
    const float cosine = icos__Fi(shalf(o, 56));
    const float sine = isin__Fi(shalf(o, 56));
    Point3 lead;
    lead.x = flt(o, 16) + cosine * 300.0f;
    lead.y = flt(o, 20) + sine * 300.0f;
    lead.z = flt(o, 24);
    findEnemyLeadPosition__8CreaturefP6Point3(o, 400.0f, &lead);
    if ((frameFlags & 1) != 0) {
        unsigned pick = eRand__10GameObject(o) % 3u;
        void* dead = ISeeDeadPeople__8Creaturef(o, 300.0f);
        if (dead != nullptr) {
            const unsigned roll = eRand__10GameObject(o);
            if (static_cast<float>(static_cast<int>(roll)) * 0x1.0p-16f < 0.5f) pick = 3;
        }
        if (pick == 0) {
            const int a = skill_roll(o, 0);
            const int b = skill_roll(o, 1);
            const int c = skill_roll(o, 2);
            const int d = skill_roll(o, 3);
            Point3 velocity;
            velocity.x = icos__Fi(shalf(o, 56)) * static_cast<float>(d);
            velocity.y = isin__Fi(shalf(o, 56)) * static_cast<float>(d);
            velocity.z = 0.0f;
            void* block = blockAlloc__Fi(172);
            if (block != nullptr) FrostStormSpell_construct(block, lead, c * 60, static_cast<float>(b * 12), a, o, velocity);
        } else if (pick == 1) {
            set_lights(o, 0, 128, 255, 50, 0, 128, 255, 150);
            const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(o, target_of(o), 350.0f);
            void* block = blockAlloc__Fi(208);
            if (block != nullptr) {
                const float half_height = flt(o, 32) * 0.5f;
                Point3 position;
                position.x = flt(o, 16) + 0.0f;
                position.y = flt(o, 20);
                position.z = flt(o, 24) + half_height;
                ShockOfFrost_construct(block, position, static_cast<short>(yaw), static_cast<int>(worldDifficulty), o, skill_word(23));
            }
        } else if (pick == 2) {
            set_lights(o, 80, 128, 255, 50, 80, 128, 255, 150);
            const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(o, target_of(o), 340.0f);
            void* block = blockAlloc__Fi(408);
            if (block != nullptr) {
                const float half_height = flt(o, 32) * 0.5f;
                Point3 position;
                position.x = flt(o, 16) + 0.0f;
                position.y = flt(o, 20);
                position.z = flt(o, 24) + half_height;
                ShockProjectile_construct(block, position, static_cast<short>(yaw), o, skill_word(37), static_cast<int>(worldDifficulty));
            }
        } else {
            set_lights(o, 0, 128, 255, 50, 0, 128, 255, 150);
            unsigned char* person = static_cast<unsigned char*>(dead);
            w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(person, 184))(w10g::adjusted(person, 184), 16);
            char lump[] = "fx.lmp";
            char name[] = "spel_he_heal.anm";
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), person, 1.0f);
        }
    }
    if ((half(o, 294) & 0x8u) != 0) call176(o, lead);
}

// S 6 otherwise: melee.
void melee(unsigned char* o, int frameFlags) {
    if ((frameFlags & 3) != 0) {
        unsigned char* target = target_of(o);
        if ((word(target, 12) & 0x40u) != 0) {
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, 36.0f, shalf(o, 200), shalf(o, 202), shalf(o, 204), 8, o + 1064);
        } else {
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, 36.0f, shalf(o, 200), shalf(o, 202), shalf(o, 204), 0x40008, o + 1064);
        }
        if ((frameFlags & 1) != 0) {
            const unsigned roll = eRand__10GameObject(o);
            if (roll % 5u == 0) set_state(o, 3);
        }
    }
    if ((half(o, 294) & 0x8u) != 0) {
        turnToFaceEnemy__8Creature(o);
    } else if (static_cast<signed char>(o[980]) != 0) {
        *reinterpret_cast<AliasedDouble*>(o + 160) = *reinterpret_cast<AliasedDouble*>(o + 160) | 0x200ull;
    }
    if (word(o, 992) != 0 && (half(o, 294) & 0x8u) == 0) turnToFaceEnemy__8Creature(o);
}
}  // namespace

extern "C" void msg_run__6Goblin(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    model = w10g::from_word(*reinterpret_cast<unsigned*>(w10g::from_word(word(o, 36)) + 12));
    if (everyFrame__8CreatureP12_modelHeader(self, model) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int frameFlags = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    if (shalf(o, 572) != 8) slideBack__8Creaturef(self, 0.5f);
    w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 168))(w10g::adjusted(self, 168));
    const short state = shalf(o, 572);
    if (static_cast<unsigned short>(state) >= 16u) return;
    switch (state) {
    case 0:
        if (wander_tests(o, 3)) return;
        if (sword(o, 256) < sword(o, 808)) set_state(self, 1);
        break;
    case 1:
        if (wander_tests(o, 0)) return;
        smartWander__8Creature(self);
        break;
    case 2:
        if (wander_tests(o, 0)) return;
        if (smartWander__8Creature(self) == 0) set_state(self, 0);
        break;
    case 14:
        if (sword(o, 808) < sword(o, 256) && word(o, 576) != 0) {
            flee2__8CreatureP10GameObject(self, target_of(o));
            return;
        }
        set_state(self, 0);
        break;
    case 3: {
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        const unsigned p = word(o, 972);
        if (p - 4u < 2u) {
            if (zone(self, 90.0f, 32767)) { set_state(self, 14); return; }
            if (zone2(o, 400.0f, 6000)) { set_state(self, 6); return; }
            steerAndRouteToEnemy__8Creature(self);
            return;
        }
        if (zone(self, flt(o, 28) + 30.0f, 6000)) {
            set_state(self, 6);
        } else if (zone(self, flt(o, 28) + 30.0f, 32767)) {
            turnToFaceEnemy__8Creature(self);
        } else if (sword(o, 972) > 0 &&
                   objectIsCrowded__8CreatureP10GameObjectffi(self, target_of(o), 400.0f, flt(o, 28) * 3.0f, 0)) {
            set_state(self, 13);
        } else {
            const unsigned roll = eRand__10GameObject(self);
            if (roll % 30u != 0) {
                steerAndRouteToEnemy__8Creature(self);
            } else {
                unsigned char* target = target_of(o);
                const Point3 position = *reinterpret_cast<Point3*>(target + 16);
                const int count = numInRegion__8creatureG6Point3f(position, flt(target, 28) * 1.75f);
                if (count >= 3) {
                    steerAndRouteToEnemy__8Creature(self);
                } else {
                    unsigned char* other = target_of(o);
                    const float dy = flt(other, 20) - flt(o, 20);
                    const float dx = flt(other, 16) - flt(o, 16);
                    const int angle = iatan2__Fff(dy, dx);
                    unsigned char* again = target_of(o);
                    if (static_cast<unsigned>(angle - shalf(again, 56) + 16383) < 32767u) set_state(self, 4);
                    else steerAndRouteToEnemy__8Creature(self);
                }
            }
        }
        remember_target(o);
        break;
    }
    case 4: {
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        if (zone(self, flt(o, 28) + 30.0f, 6000)) {
            set_state(self, 6);
        } else {
            const unsigned roll = eRand__10GameObject(self);
            if (roll % 30u != 0) {
                steerAndRouteToEnemy__8Creature(self);
            } else {
                unsigned char* target = target_of(o);
                const float dy = flt(target, 20) - flt(o, 964);
                const float dx = flt(target, 16) - flt(o, 960);
                const float distance = square_root(dx * dx + dy * dy);
                if (distance < 0x1.fffffep-1f) set_state(self, 3);
                else steerAndRouteToEnemy__8Creature(self);
            }
        }
        remember_target(o);
        break;
    }
    case 13: {
        if (word(o, 576) == 0) { set_state(self, 0); return; }
        if (word(o, 972) == 5u) {
            if (zone(self, 90.0f, 32767)) set_state(self, 14);
            else if (sword(o, 976) < sword(o, 808)) set_state(self, 3);
        } else if (sword(o, 256) < sword(o, 808)) {
            set_state(self, 3);
        } else if (!objectIsCrowded__8CreatureP10GameObjectffi(self, target_of(o), 400.0f, flt(o, 28) * 3.0f, 1)) {
            set_state(self, 3);
        }
        turnToFaceEnemy__8Creature(self);
        break;
    }
    case 15:
        if (sword(o, 256) < sword(o, 808)) set_state(self, 3);
        break;
    case 5: {
        if (word(o, 576) == 0) { set_state(self, 3); return; }
        if (word(o, 972) - 4u < 2u) {
            if (zone2(o, 400.0f, 6000)) set_state(self, 6);
        }
        if (!(word(o, 972) - 4u < 2u)) {
            if (zone(self, flt(o, 28) + 30.0f, 6000)) { set_state(self, 6); return; }
        }
        if (bExitStatePersueIdle__8Creature(self)) set_state(self, 3);
        break;
    }
    case 6:
        if (word(o, 576) != 0) {
            const unsigned p = word(o, 972);
            if (p == 4u) archer(o, frameFlags);
            else if (p == 5u) caster(o, frameFlags);
            else melee(o, frameFlags);
        }
        if ((half(o, 294) & 0x8u) == 0) sword(o, 256) = sword(o, 256) - 1;
        if (sword(o, 256) > 0) return;
        if (word(o, 972) != 5u) {
            set_state(self, 3);
        } else {
            const unsigned roll = eRand__10GameObject(self);
            word(o, 976) = word(o, 808) + roll % 180u + 180u;
            set_state(self, 13);
        }
        break;
    case 9:
        if (sword(o, 804) < sword(o, 808)) set_state(self, 3);
        break;
    case 8:
        if ((*reinterpret_cast<AliasedDouble*>(o + 288) & 0x0030000000000000ull) == 0) set_state(self, 3);
        break;
    case 10: {
        if ((half(o, 294) & 0x80u) != 0) objectAddToSlowRunList__FP10GameObject(self);
        if ((frameFlags & 1) != 0 && (word(o, 12) & 2u) == 0) {
            const Point3 position = *reinterpret_cast<Point3*>(o + 16);
            const unsigned first = eRand__10GameObject(self);
            const unsigned second = eRand__10GameObject(self);
            floorAdd__FG6Point3sfii(position, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 20u)), 0, 1);
        }
        if ((frameFlags & 2) != 0 && (word(o, 12) & 2u) == 0) word(o, 12) = word(o, 12) | 1u;
        if (word(o, 1136) == 0) return;
        if ((half(o, 294) & 0x80u) == 0) return;
        const float sunk = flt(o, 1140) + 0x1.666666p-2f;
        const float height = flt(o, 24);
        flt(o, 1140) = sunk;
        flt(o, 60) = height - sunk;
        if (24.0f < sunk) {
            word(o, 12) = word(o, 12) | 0x4002u;
            objectAddToDeleteList__FP10GameObject(self);
        }
        break;
    }
    default:
        break;
    }
}
