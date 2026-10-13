// Observed msg_run__7Ulthork at 0x001f5468, 2088 bytes. R is the block at the word object+36, s the
// signed halfword at +572 (the state), b the unsigned halfword at +294, e the word at +576, wait the
// word at +256, base the word at +808, a the value of creatureAdvanceAnimation, f the float at +28.
// "setState(n)" is the table entry at (word +72) + 184 called with n; "zone(d, k)" is enemyInZone(this,
// d, k) as a boolean. Steps, in order:
//  1. newDamageHandler(this, R[12]); everyFrame(this, R[12]) non-zero ends the call;
//     objectActivationCheck(this, 600) returning zero ends the call.
//  2. a = creatureAdvanceAnimation(this, 1.0); slideBack(this, 0.3); the table entry at +168 is called.
//  3. A state outside 0..13 (unsigned compare) and the states 2, 4, 7, 11, 12 end the call. Otherwise:
//   0: zone(400, 32767): setState(3); otherwise setState(1) when wait < base (signed).
//   1: zone(400, 32767): setState(3); otherwise setState(0) when wait < base; otherwise smartWander.
//   3: e zero: setState(0). Otherwise zone(f + 20, 5000): setState(6); zone(f + 20, 32767):
//      turnToFaceEnemy and end; otherwise, when the signed byte at +1104 is 2 and enemyInZone2(this, 0.0,
//      400.0, h56, 5000, 0.0, 1) is non-zero, setState(13); otherwise steerAndRouteToEnemy.
//   5: zone(f + 20, 5000): setState(6). Otherwise, when the byte at +1104 is 2 and zone(400, 5000):
//      setState(13); otherwise bExitStatePersueIdle: when non-zero setState(3).
//   6: e non-zero: with a bit 0 set, melee = produceMeleeDamage(this, 36.0, h200, h202, h204, k, null)
//      with k = 0x80008 when eRand & 7 is zero, else 0x40008 when a second eRand & 3 is zero, else 8;
//      with a bit 1 set, setState(3) when eRand % 5 is zero. Then, when b bit 3 is clear, setState(3)
//      and end; otherwise turnToFaceEnemy. After the block (or with e zero): when b bit 3 is clear,
//      setState(3).
//   8: b bit 4 (0x10) clear: setState(0).   9: signed word at +804 below base: setState(3).
//   10: b bit 7 (0x80) set: objectRemoveFromRunList(this).
//   13: e non-zero: c = icos(h56), t = {x + 300 c, y + 300 isin(h56), z} from the point at +16, and
//      findEnemyLeadPosition(this, 400.0, &t); with a bit 0 set the description halfwords at +1048..
//      +1062 are set to 0, 128, 255, 50, 0, 128, 255, 150, and with k = ISeeDeadPeople(this, 300.0)
//      (an object or null) and r = eRand: kind 2 when k is non-null, else r & 1. Kind 0: yaw =
//      creatureFindLeadYaw(this, the word at e + 12, 350.0); a 208-byte block is allocated and, when
//      non-null, ShockOfFrost is constructed in it with (the point {x, y, z + 0.5 * the float at +32}
//      from +16, yaw, (int)worldDifficulty, this, skills[23]). Kind 1: with K the object skills[58]
//      the entry at (word K + 96) + 104 is called four times with (index 0..3, (int)worldDifficulty,
//      this) giving r0..r3; a 172-byte block is allocated and, when non-null, FrostStormSpell is
//      constructed in it with (t, 60 * r2, (float)(12 * r1), r0, this, {300 c3 ...}) where the last
//      point is {cos(h56) * (float)r3, sin(h56) * (float)r3, 0}. Kind 2: setState(14) is called on k,
//      lumpFindResource("fx.lmp", "spel_he_heal.anm") is played on k as a sequence at 1.0. Then
//      turnToFaceEnemy. Finally (also with e zero): setState(3) when wait < base, else setState(6)
//      when zone(f + 20, 5000).
// Meanings of the states, fields and the description halfwords are unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

#if defined(W10_ACTOR_O_HOST)
unsigned w10_actor_o_host_call3(unsigned function_word, void* adjusted_object, unsigned a1, int a2, unsigned a3);
#endif

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int value);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
void smartWander__8Creature(void* self);
unsigned eRand__10GameObject(void* object);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
float icos__Fi(int angle);
float isin__Fi(int angle);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
void* ISeeDeadPeople__8Creaturef(void* self, float range);
short creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* object, float range);
void* blockAlloc__Fi(int size);
void __12ShockOfFrostG6Point3siP10GameObjectP5Skill(void* self, Point3 position, short yaw, int level, void* owner, void* skill);
void __15FrostStormSpellG6Point3ifiP10GameObjectT1(void* self, Point3 position, int a, float b, int c, void* owner, Point3 velocity);
char* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern float worldDifficulty;
extern unsigned skills[];
}

namespace {
// The call through the table entry at (word at skill+96) + 104: adjusted object, then three arguments.
inline unsigned call_skill(unsigned char* skill, unsigned index, int level, void* caller) {
    unsigned char* entry = w10o::from_word(w10o::word(skill, 96)) + 104;
    unsigned char* adjusted = skill + *reinterpret_cast<short*>(entry);
    const unsigned function_word = *reinterpret_cast<unsigned*>(entry + 4);
#if defined(W10_ACTOR_O_HOST)
    return w10_actor_o_host_call3(function_word, adjusted, index, level, w10o::to_word(caller));
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned, int, void*)>(static_cast<__UINTPTR_TYPE__>(function_word))(adjusted, index, level, caller);
#endif
}

inline void init_description(unsigned char* o) {
    using namespace w10o;
    half(o, 1058) = 128;
    half(o, 1060) = 255;
    half(o, 1062) = 150;
    half(o, 1050) = 128;
    half(o, 1052) = 255;
    half(o, 1054) = 50;
    half(o, 1048) = 0;
    half(o, 1056) = 0;
}
}  // namespace

extern "C" void msg_run__7Ulthork(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, bits(0x3e999999u));
    call_entry(o, 168);
    auto set_state = [&](int n) { call_entry(o, 184, static_cast<unsigned long>(n)); };
    auto zone = [&](float range, short angle) { return enemyInZone__8Creaturefs(self, range, angle) != 0; };
    auto bits294 = [&]() { return static_cast<unsigned>(*reinterpret_cast<unsigned short*>(o + 294)); };
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 14u) return;
    switch (state) {
        case 0:
            if (zone(400.0f, 32767)) { set_state(3); return; }
            if (sword(o, 256) < sword(o, 808)) set_state(1);
            return;
        case 1:
            if (zone(400.0f, 32767)) { set_state(3); return; }
            if (sword(o, 256) < sword(o, 808)) { set_state(0); return; }
            smartWander__8Creature(self);
            return;
        case 3:
            if (word(o, 576) == 0) { set_state(0); return; }
            if (zone(real(o, 28) + 20.0f, 5000)) { set_state(6); return; }
            if (zone(real(o, 28) + 20.0f, 32767)) { turnToFaceEnemy__8Creature(self); return; }
            if (static_cast<signed char>(o[1104]) == 2 &&
                enemyInZone2__8Creatureffssfi(self, 0.0f, 400.0f, half(o, 56), 5000, 0.0f, 1) != 0) {
                set_state(13);
                return;
            }
            steerAndRouteToEnemy__8Creature(self);
            return;
        case 5:
            if (zone(real(o, 28) + 20.0f, 5000)) { set_state(6); return; }
            if (static_cast<signed char>(o[1104]) == 2 && zone(400.0f, 5000)) { set_state(13); return; }
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(3);
            return;
        case 6:
            if (word(o, 576) != 0) {
                if ((advance & 1) != 0) {
                    int flags;
                    if ((eRand__10GameObject(self) & 7u) == 0) flags = 0x80008;
                    else if ((eRand__10GameObject(self) & 3u) == 0) flags = 0x40008;
                    else flags = 8;
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, half(o, 200), half(o, 202), half(o, 204), flags, nullptr);
                }
                if ((advance & 2) != 0) {
                    if (eRand__10GameObject(self) % 5u == 0) set_state(3);
                }
                if ((bits294() & 8u) == 0) { set_state(3); return; }
                turnToFaceEnemy__8Creature(self);
            }
            if ((bits294() & 8u) != 0) return;
            set_state(3);
            return;
        case 8:
            if ((bits294() & 0x10u) == 0) set_state(0);
            return;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(3);
            return;
        case 10:
            if ((bits294() & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
            return;
        case 13: {
            if (word(o, 576) != 0) {
                const float c = icos__Fi(half(o, 56));
                const float s = isin__Fi(half(o, 56));
                Point3 direction = {c, s, 0.0f};
                Point3 offset;
                offset.x = direction.x * 300.0f;
                offset.y = s * 300.0f;
                offset.z = 0.0f;
                Point3 target;
                target.x = real(o, 16) + offset.x;
                target.y = real(o, 20) + offset.y;
                target.z = real(o, 24);
                findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &target);
                if ((advance & 1) != 0) {
                    const unsigned r = eRand__10GameObject(self);
                    void* const dead = ISeeDeadPeople__8Creaturef(self, 300.0f);
                    unsigned kind = r & 1u;
                    if (dead != nullptr) kind = 2;
                    if (kind == 0) {
                        unsigned char* const enemy = from_word(word(o, 576));
                        init_description(o);
                        const short yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, from_word(word(enemy, 12)), 350.0f);
                        void* const block = blockAlloc__Fi(208);
                        if (block != nullptr) {
                            Point3 where;
                            where.x = real(o, 16) + 0.0f;
                            where.y = real(o, 20);
                            where.z = real(o, 24) + real(o, 32) * 0.5f;
                            __12ShockOfFrostG6Point3siP10GameObjectP5Skill(block, where, yaw, static_cast<int>(worldDifficulty), self,
                                                                        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(skills[23])));
                        }
                    } else if (kind == 1) {
                        init_description(o);
                        unsigned char* const skill = from_word(skills[58]);
                        const unsigned r0 = call_skill(skill, 0, static_cast<int>(worldDifficulty), self);
                        const unsigned r1 = call_skill(from_word(skills[58]), 1, static_cast<int>(worldDifficulty), self);
                        const unsigned r2 = call_skill(from_word(skills[58]), 2, static_cast<int>(worldDifficulty), self);
                        const unsigned r3 = call_skill(from_word(skills[58]), 3, static_cast<int>(worldDifficulty), self);
                        const int twelve = static_cast<int>(r1 * 12u);
                        const int sixty = static_cast<int>(r2 * 60u);
                        const float scale = static_cast<float>(static_cast<int>(r3));
                        Point3 velocity;
                        velocity.x = icos__Fi(half(o, 56)) * scale;
                        velocity.y = isin__Fi(half(o, 56)) * scale;
                        velocity.z = 0.0f;
                        void* const block = blockAlloc__Fi(172);
                        if (block != nullptr) {
                            __15FrostStormSpellG6Point3ifiP10GameObjectT1(block, target, sixty, static_cast<float>(twelve), static_cast<int>(r0), self, velocity);
                        }
                    } else {
                        init_description(o);
                        unsigned char* const dead_object = static_cast<unsigned char*>(dead);
                        call_entry(dead_object, 184, 14);
                        char* const header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_heal.anm"));
                        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, dead, 1.0f);
                    }
                }
                turnToFaceEnemy__8Creature(self);
            }
            if (sword(o, 256) < sword(o, 808)) { set_state(3); return; }
            if (zone(real(o, 28) + 20.0f, 5000)) set_state(6);
            return;
        }
        default:
            return;
    }
}
