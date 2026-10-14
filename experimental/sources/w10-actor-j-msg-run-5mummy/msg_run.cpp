// Observed msg_run__5Mummy at 0x00237788, 6268 bytes: the per-frame run routine of Mummy. Offsets are bytes of
// the object; R is the block at the word +36 (R[n] is its word at byte n); the state is the halfword +572;
// t = word +808; "setState(n)" is the table entry at (word +72)+184 called on the adjusted object with n; k is
// the word +960; X is the result of creatureAdvanceAnimation(this, 1.0), taken only when the word +1104 is
// zero (otherwise X is 0); T is a point kept on the stack. Order of events:
//  1. newDamageHandler(this, R[12]); a non-zero everyFrame(this, R[12]) ends the call; a zero
//     objectActivationCheck(this, 600) ends the call; X as above; slideBack(this, 0.5); the table entry at
//     (word +72)+168 on the adjusted object. States of 18 or more end the call.
//  2. The cases (Z(r, a) = enemyInZone(this, r, a); B = (float +28)):
//   0: Z(400, 32767) or Z(500, 16000) -> setState(3); for k = 1, 3, 5 also enemyInZone2(this, 0, 500, +56,
//      16000, 0, 1) -> setState(3); then w256 < t -> setState(1).
//   1: Z(400, 32767) or Z(500, 16000) -> setState(0); else smartWander.
//   3: w576 == 0 -> setState(0). k = 1, 3, 5: w1112 < t and Z(72, 32767) -> setState(15); enemyInZone2(this, 0,
//      400, +56, 6000, 0, 1) -> setState(13); else steerAndRouteToEnemy. Other k: Z(B + 40, 6000) ->
//      setState(6); k == 4 and Z(100, 4000) -> setState(14); objectIsCrowded(this, target, 500, 3*B, 0) ->
//      w256 = t + eRand%240 + 240, setState(5); Z(B + 40, 32767) -> turnToFaceEnemy; one in thirty rolls with
//      numInRegion(target point, 1.75 * target float +28) < 3 and the target facing within a half circle of the
//      bearing -> setState(17); else steerAndRouteToEnemy.
//   5: k = 1, 3, 5: enemyInZone2 as in 3 -> setState(13); k = 0, 2, 4: Z(B + 40, 6000) -> setState(6);
//      bExitStatePersueIdle zero ends the call; w256 < t -> setState(3); else turn to face when w576 is
//      set (k = 1, 3, 5), or objectIsCrowded(this, target, 500, 3*B, 1) zero -> setState(3).
//   6, 14: move2(zero); with w576 and X bit 0: for k == 4 and w208 == 3 half of the rolls call
//      creatureSlamEffect(this, target, 1) and double the damage numbers; produceMeleeDamage(this, 36, h200,
//      h202, h204, 8, description at +1032); turnToFaceEnemy; setState(3) unless +294 bit 3 is set.
//   8: move2(zero); setState(3) unless +294 bit 4 is set.   9: w804 < t -> setState(3).
//   10: move2(zero); +294 bit 7 -> objectRemoveFromRunList.
//   15: X bit 0 builds a DustDevil at the position; X bit 1 sets flag bit 41 and 0x800 in the 64-bit word,
//      sets the word +12 bits 0..1, plays R[444], then returns to the home point (+776) when the distance
//      to it exceeds float +788, or else calls RandomTeleportSpell(this, target position or null, 400),
//      and builds a second DustDevil; without +294 bit 3 the bits are cleared again and setState(3) runs.
//   17: w576 == 0 -> setState(0). Z(B + 40, 6000) -> setState(6), k == 4 and Z(100, 4000) -> setState(14),
//      otherwise one roll in thirty with the target within 1.33 units of the remembered point at +1116 ->
//      setState(3), else steerAndRouteToEnemy; the target position is then stored at +1116.
//   13: move2(zero); with w576: halfwords 1048..1062 are set when X bit 0 or w1104 is non-zero; a point 300
//      ahead along the heading is moved by findEnemyLeadPosition(this, 400); with X bit 0,
//      ISeeDeadPeople(this, 300) may (on a roll under one half) call the other object's setState(16) and
//      play "spel_he_heal.anm" of "fx.lmp"; otherwise k picks a spell and a roll chooses which: k = 1
//      (sparks, fire storm or cone of fire), k = 3 (disease bolt, poison rain or disease shield), k = 5
//      (shock of frost, frost storm, sparks or fire storm). The table entry at (word +72)+176 then runs
//      with (0, T). Without w576 only the tail runs.
//  3. Tail of state 13: unless t < w1108, w1104 is cleared and the SpellConeOfFire spell is terminated; then
//     w256 > 0 counts down (and with w1112 < t and Z(72, 32767) ends the cone spell and calls setState(15)),
//     otherwise the cone spell is ended and setState(3) runs.
// The meaning of the states and fields is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern float worldDifficulty;
extern unsigned skills[];
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
int numInRegion__8creatureG6Point3f(Point3 position, float radius);
int iatan2__Fff(float y, float x);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, Point3 delta);
void creatureSlamEffect__8CreatureP8Creaturei(void* self, void* other, int value);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void dust_devil_construct(void* self, Point3 position) __asm__("__9DustDevilG6Point3");
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
void RandomTeleportSpell__FP10GameObjectP6Point3f(void* object, Point3* where, float range);
void* __tf15SpellConeOfFire();
void terminateSpell__8CreatureRC9type_info(void* creature, const void* typeinfo);
void* ISeeDeadPeople__8Creaturef(void* self, float range);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* target, float speed);
void addSpell__8CreatureP5Spell(void* self, void* spell);
void* lumpFindResource__FPcT0(char* lump, char* name);
void sparks_construct(void* self, Point3 position, Point3 velocity, void* creature, void* vag1, void* vag2, int a, int b, int c, int d)
    __asm__("__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii");
void fire_storm_construct(void* self, int difficulty, void* creature, Point3 position, void* skill) __asm__("__9FireStormiP10GameObjectG6Point3P5Skill");
void cone_of_fire_construct(void* self, int difficulty, void* creature, void* skill) __asm__("__15SpellConeOfFireiP8CreatureP5Skill");
// The heading argument is passed as the full integer; the constructors narrow it themselves.
void disease_bolt_construct(void* self, int heading, int difficulty, Point3 position, void* creature, void* skill)
    __asm__("__11DiseaseBoltsiG6Point3P10GameObjectP5Skill");
void poison_rain_construct(void* self, int difficulty, void* creature, Point3 position, void* skill, void* header)
    __asm__("__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader");
void disease_shield_construct(void* self, int difficulty, void* creature, void* skill) __asm__("__18SpellDiseaseShieldiP8CreatureP5Skill");
void shock_of_frost_construct(void* self, Point3 position, int heading, int difficulty, void* creature, void* skill)
    __asm__("__12ShockOfFrostG6Point3siP10GameObjectP5Skill");
void frost_storm_construct(void* self, Point3 position, int a, float b, int c, void* creature, Point3 velocity)
    __asm__("__15FrostStormSpellG6Point3ifiP10GameObjectT1");
}

namespace {
using namespace w10j;

__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

// Calls the skill object's entry at (word +96)+104 with (index, (int)worldDifficulty, creature).
int skill_value(unsigned char* creature, unsigned skill_index, int index) {
    unsigned char* skill = from_word(skills[skill_index]);
    unsigned char* e = from_word(word(skill, 96)) + 104;
    const int difficulty = static_cast<int>(worldDifficulty);
#if defined(W10_ACTOR_J_HOST)
    return static_cast<int>(w10_actor_j_host_call(*reinterpret_cast<unsigned*>(e + 4), skill + *reinterpret_cast<short*>(e), 3, static_cast<unsigned long>(index),
                                                  static_cast<unsigned long>(difficulty)));
#else
    return reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(
        skill + *reinterpret_cast<short*>(e), index, difficulty, creature);
#endif
}

void set_state(unsigned char* o, int state) { call_entry(o, 184, static_cast<unsigned long>(state)); }

void halves(unsigned char* o, short h1048, short h1050, short h1052, short h1054, short h1056, short h1058, short h1060, short h1062) {
    half(o, 1048) = h1048;
    half(o, 1050) = h1050;
    half(o, 1052) = h1052;
    half(o, 1054) = h1054;
    half(o, 1056) = h1056;
    half(o, 1058) = h1058;
    half(o, 1060) = h1060;
    half(o, 1062) = h1062;
}

// A point 6 above the aimed point T, used by the fire storm.
void cast_fire_storm(unsigned char* o, const Point3& aim) {
    void* const spell = blockAlloc__Fi(168);
    if (spell != nullptr) {
        const Point3 where = {aim.x + 0.0f, aim.y, aim.z + 6.0f};
        fire_storm_construct(spell, static_cast<int>(worldDifficulty), o, where, from_word(skills[7]));
    }
}

// The sparks spell shared by two branches; `boost` scales the first skill value by a random fraction.
void cast_sparks(unsigned char* o) {
    const int n0 = skill_value(o, 57, 0);
    const float scaled = static_cast<float>(n0) * bits(0x3e4ccccc);
    const float unit = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * bits(0x37800000u);
    const int boosted = n0 + static_cast<int>(scaled * unit);
    const int n1 = skill_value(o, 57, 1);
    const int n2 = skill_value(o, 57, 2);
    const int n3 = skill_value(o, 57, 3);
    const float speed = static_cast<float>(n3);
    unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
    const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(o, target, speed);
    Point3 velocity = {icos__Fi(yaw) * speed, isin__Fi(yaw) * speed, 0.0f};
    void* const spell = blockAlloc__Fi(184);
    if (spell != nullptr) {
        const float lift = real(o, 32) * 0.5f;
        const Point3 where = {real(o, 16) + 0.0f, real(o, 20), real(o, 24) + lift};
        sparks_construct(spell, where, velocity, o, nullptr, nullptr, n0, boosted, n1, n2 * 12);
    }
    unsigned char* const block = from_word(word(o, 36));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(block, 428)), o, 1.0f);
}
}  // namespace

extern "C" void msg_run__5Mummy(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    unsigned advance = 0;
    if (word(o, 1104) == 0u) advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    call_entry(o, 168);
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 18u) return;
    unsigned char* const block = from_word(word(o, 36));
    const unsigned k = word(o, 960);
    const bool group135 = (k == 1u || k == 3u || k == 5u);
    switch (state) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0) {
                set_state(o, 3);
                return;
            }
            if (group135 && enemyInZone2__8Creatureffssfi(self, 0.0f, 500.0f, half(o, 56), 16000, 0.0f, 1) != 0) {
                set_state(o, 3);
                return;
            }
            if (sword(o, 256) < sword(o, 808)) set_state(o, 1);
            return;
        case 1:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0) {
                set_state(o, 0);
                return;
            }
            smartWander__8Creature(self);
            return;
        case 3: {
            if (word(o, 576) == 0u) {
                set_state(o, 0);
                return;
            }
            if (group135) {
                if (sword(o, 1112) < sword(o, 808) && enemyInZone__8Creaturefs(self, 72.0f, 32767) != 0) {
                    set_state(o, 15);
                    return;
                }
                if (enemyInZone2__8Creatureffssfi(self, 0.0f, 400.0f, half(o, 56), 6000, 0.0f, 1) != 0) {
                    set_state(o, 13);
                    return;
                }
                steerAndRouteToEnemy__8Creature(self);
                return;
            }
            const float ahead = real(o, 28);
            if (enemyInZone__8Creaturefs(self, ahead + 40.0f, 6000) != 0) {
                set_state(o, 6);
                return;
            }
            if (k == 4u && enemyInZone__8Creaturefs(self, 100.0f, 4000) != 0) {
                set_state(o, 14);
                return;
            }
            unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
            if (objectIsCrowded__8CreatureP10GameObjectffi(self, target, 500.0f, ahead * 3.0f, 0) != 0) {
                const unsigned e = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + e % 240u + 240u;
                set_state(o, 5);
                return;
            }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 40.0f, 32767) != 0) {
                turnToFaceEnemy__8Creature(self);
                return;
            }
            if (eRand__10GameObject(self) % 30u == 0u) {
                unsigned char* const object = from_word(word(from_word(word(o, 576)), 12));
                const Point3 where = *reinterpret_cast<Point3*>(object + 16);
                const int crowd = numInRegion__8creatureG6Point3f(where, real(from_word(word(from_word(word(o, 576)), 12)), 28) * bits(0x3fe00000u));
                if (crowd < 3) {
                    unsigned char* const other = from_word(word(from_word(word(o, 576)), 12));
                    const int bearing = iatan2__Fff(real(other, 20) - real(o, 20), real(other, 16) - real(o, 16));
                    if (static_cast<unsigned>(bearing - half(from_word(word(from_word(word(o, 576)), 12)), 56) + 16383) < 32767u) {
                        set_state(o, 17);
                        return;
                    }
                }
            }
            steerAndRouteToEnemy__8Creature(self);
            return;
        }
        case 5: {
            if (group135 && enemyInZone2__8Creatureffssfi(self, 0.0f, 400.0f, half(o, 56), 6000, 0.0f, 1) != 0) {
                set_state(o, 13);
                return;
            }
            if ((k == 0u || k == 2u || k == 4u) && enemyInZone__8Creaturefs(self, real(o, 28) + 40.0f, 6000) != 0) {
                set_state(o, 6);
                return;
            }
            if (bExitStatePersueIdle__8Creature(self) == 0) return;
            if (sword(o, 256) < sword(o, 808)) {
                set_state(o, 3);
                return;
            }
            if (word(o, 576) == 0u) return;
            if (group135) {
                turnToFaceEnemy__8Creature(self);
                return;
            }
            const float ahead = real(o, 28);
            unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
            if (objectIsCrowded__8CreatureP10GameObjectffi(self, target, 500.0f, ahead * 3.0f, 1) != 0) return;
            set_state(o, 3);
            return;
        }
        case 6:
        case 14: {
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if (word(o, 576) != 0u) {
                if ((advance & 1u) != 0u) {
                    int a = half(o, 200);
                    int b = half(o, 202);
                    if (word(o, 960) == 4u && word(o, 208) == 3u) {
                        const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
                        if (roll < 0.5f) {
                            creatureSlamEffect__8CreatureP8Creaturei(self, from_word(word(from_word(word(o, 576)), 12)), 1);
                            a = a << 1;
                            b = b << 1;
                        }
                    }
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, a, b, half(o, 204), 8, o + 1032);
                }
                turnToFaceEnemy__8Creature(self);
            }
            if ((half(o, 294) & 8) == 0) set_state(o, 3);
            return;
        }
        case 8:
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if ((half(o, 294) & 0x10) == 0) set_state(o, 3);
            return;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 3);
            return;
        case 10:
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if ((half(o, 294) & 0x80) != 0) objectRemoveFromRunList__FP10GameObject(self);
            return;
        case 15: {
            if ((advance & 1u) != 0u) {
                void* const devil = blockAlloc__Fi(88);
                if (devil != nullptr) dust_devil_construct(devil, *reinterpret_cast<Point3*>(o + 16));
            }
            if ((advance & 2u) != 0u) {
                dword(o, 160) |= 0x20000000000ull | 0x800ull;
                word(o, 12) |= 3u;
                unsigned char* const here = o + 16;
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(block, 444)), self, 1.0f);
                const float dx = real(o, 16) - real(o, 776);
                const float dy = real(o, 20) - real(o, 780);
                const float dz = real(o, 24) - real(o, 784);
                const float radius = real(o, 788);
                if (radius * radius < dx * dx + dy * dy + dz * dz) {
                    *reinterpret_cast<Point3*>(here) = *reinterpret_cast<Point3*>(o + 776);
                    objectUpdateInGrid__FP10GameObject(self);
                } else {
                    const unsigned target = word(o, 576);
                    if (target != 0u) {
                        RandomTeleportSpell__FP10GameObjectP6Point3f(self, reinterpret_cast<Point3*>(from_word(word(from_word(target), 12)) + 16), 400.0f);
                    } else {
                        RandomTeleportSpell__FP10GameObjectP6Point3f(self, nullptr, 400.0f);
                    }
                }
                void* const devil = blockAlloc__Fi(88);
                if (devil != nullptr) dust_devil_construct(devil, *reinterpret_cast<Point3*>(here));
            }
            if ((half(o, 294) & 8) == 0) {
                word(o, 12) &= ~3u;
                dword(o, 160) &= ~(0x20000000000ull | 0x800ull);
                set_state(o, 3);
            }
            return;
        }
        case 17: {
            if (word(o, 576) == 0u) {
                set_state(o, 0);
                return;
            }
            unsigned char* const remembered = o + 1116;
            const float ahead = real(o, 28);
            bool changed = false;
            if (enemyInZone__8Creaturefs(self, ahead + 40.0f, 6000) != 0) {
                set_state(o, 6);
                changed = true;
            } else if (k == 4u && enemyInZone__8Creaturefs(self, 100.0f, 4000) != 0) {
                set_state(o, 14);
                changed = true;
            }
            if (!changed) {
                if (eRand__10GameObject(self) % 30u == 0u) {
                    unsigned char* const spot = from_word(word(from_word(word(o, 576)), 12)) + 16;
                    const float dx = real(spot, 0) - real(remembered, 0);
                    const float dy = real(spot, 4) - real(remembered, 4);
                    if (square_root(dx * dx + dy * dy) < bits(0x3faaaaaa)) {
                        set_state(o, 3);
                    } else {
                        steerAndRouteToEnemy__8Creature(self);
                    }
                } else {
                    steerAndRouteToEnemy__8Creature(self);
                }
            }
            *reinterpret_cast<Point3*>(remembered) = *reinterpret_cast<Point3*>(from_word(word(from_word(word(o, 576)), 12)) + 16);
            return;
        }
        case 13:
            break;
        default:
            return;
    }
    // State 13 from here.
    move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
    if (word(o, 576) != 0u) {
        const bool fire = (advance & 1u) != 0u;
        if (fire || word(o, 1104) != 0u) halves(o, 255, 128, 0, 100, 255, 128, 0, 100);
        const float cosine = icos__Fi(half(o, 56));
        const float sine = isin__Fi(half(o, 56));
        Point3 aim = {real(o, 16) + cosine * 300.0f, real(o, 20) + sine * 300.0f, real(o, 24)};
        findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &aim);
        if (fire) {
            void* const dead = ISeeDeadPeople__8Creaturef(self, 300.0f);
            bool handled = false;
            if (dead != nullptr) {
                const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
                if (roll < 0.5f) {
                    half(o, 146) = 50;
                    half(o, 150) = 128;
                    half(o, 152) = 255;
                    half(o, 142) = 128;
                    half(o, 144) = 255;
                    half(o, 154) = 150;
                    half(o, 140) = 0;
                    half(o, 148) = 0;
                    call_entry(static_cast<unsigned char*>(dead), 184, 16);
                    void* const heal = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_heal.anm"));
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(heal, dead, 1.0f);
                    handled = true;
                }
            }
            if (!handled) {
                if (k == 1u) {
                    int v;
                    if (enemyInZone__8Creaturefs(self, 144.0f, 32767) != 0 && enemyInZone__8Creaturefs(self, 48.0f, 32767) == 0) {
                        v = static_cast<int>(eRand__10GameObject(self) % 3u);
                    } else {
                        v = static_cast<int>(eRand__10GameObject(self) & 1u);
                    }
                    if (v == 0) {
                        halves(o, 255, 128, 0, 50, 255, 128, 0, 100);
                        cast_sparks(o);
                    } else if (v == 1) {
                        halves(o, 255, 128, 0, 50, 255, 128, 0, 100);
                        cast_fire_storm(o, aim);
                    } else if (v == 2) {
                        void* const spell = blockAlloc__Fi(88);
                        if (spell != nullptr) cone_of_fire_construct(spell, static_cast<int>(worldDifficulty), o, from_word(skills[18]));
                        addSpell__8CreatureP5Spell(self, spell);
                        word(o, 1104) = 1u;
                        word(o, 1108) = word(o, 808) + 120u;
                    }
                } else if (k == 3u) {
                    const int n1 = skill_value(o, 5, 1);
                    int v;
                    if (enemyInZone__8Creaturefs(self, static_cast<float>(n1 * 12), 32767) != 0) {
                        v = static_cast<int>(eRand__10GameObject(self) % 3u);
                    } else {
                        v = static_cast<int>(eRand__10GameObject(self) & 1u);
                    }
                    if (v == 0) {
                        unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
                        halves(o, 255, 255, 0, 50, 255, 255, 0, 100);
                        const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, target, 350.0f);
                        void* const spell = blockAlloc__Fi(124);
                        if (spell != nullptr) {
                            const Point3 where = {real(o, 16) + 0.0f, real(o, 20), real(o, 24) + real(o, 32) * 0.5f};
                            disease_bolt_construct(spell, yaw, static_cast<int>(worldDifficulty), where, o, from_word(skills[4]));
                        }
                    } else if (v == 1) {
                        halves(o, 0, 255, 0, 50, 0, 255, 0, 100);
                        void* const spell = blockAlloc__Fi(108);
                        if (spell != nullptr) poison_rain_construct(spell, static_cast<int>(worldDifficulty), o, aim, from_word(skills[24]), from_word(word(block, 452)));
                    } else if (v == 2) {
                        halves(o, 255, 255, 0, 50, 255, 255, 0, 100);
                        void* const spell = blockAlloc__Fi(432);
                        if (spell != nullptr) disease_shield_construct(spell, static_cast<int>(worldDifficulty), o, from_word(skills[5]));
                        addSpell__8CreatureP5Spell(self, spell);
                    }
                } else if (k == 5u) {
                    const int v = static_cast<int>(eRand__10GameObject(self) & 3u);
                    if (v == 0) {
                        unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
                        halves(o, 0, 128, 255, 50, 0, 128, 255, 150);
                        const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, target, 350.0f);
                        void* const spell = blockAlloc__Fi(208);
                        if (spell != nullptr) {
                            const Point3 where = {real(o, 16) + 0.0f, real(o, 20), real(o, 24) + real(o, 32) * 0.5f};
                            shock_of_frost_construct(spell, where, yaw, static_cast<int>(worldDifficulty), o, from_word(skills[23]));
                        }
                    } else if (v == 1) {
                        halves(o, 0, 128, 255, 50, 0, 128, 255, 150);
                        const int n0 = skill_value(o, 58, 0);
                        const int n1 = skill_value(o, 58, 1);
                        const int n2 = skill_value(o, 58, 2);
                        const int n3 = skill_value(o, 58, 3);
                        const float speed = static_cast<float>(n3);
                        const Point3 velocity = {icos__Fi(half(o, 56)) * speed, isin__Fi(half(o, 56)) * speed, 0.0f};
                        void* const spell = blockAlloc__Fi(172);
                        if (spell != nullptr) frost_storm_construct(spell, aim, n2 * 60, static_cast<float>(n1 * 12), n0, o, velocity);
                    } else if (v == 2) {
                        halves(o, 255, 128, 0, 50, 255, 128, 0, 100);
                        cast_sparks(o);
                    } else {
                        halves(o, 255, 128, 0, 50, 255, 128, 0, 100);
                        cast_fire_storm(o, aim);
                    }
                }
            }
        }
        call_entry(o, 176, 0, to_word(&aim));
    }
    // Shared tail: the spell timers.
    if (!(sword(o, 808) < sword(o, 1108))) {
        word(o, 1104) = 0u;
        terminateSpell__8CreatureRC9type_info(self, __tf15SpellConeOfFire());
    }
    const int timer = sword(o, 256);
    if (timer > 0) {
        if ((half(o, 294) & 8) != 0) return;
        sword(o, 256) = timer - 1;
        if (sword(o, 1112) < sword(o, 808)) {
            if (enemyInZone__8Creaturefs(self, 72.0f, 32767) != 0) {
                word(o, 1104) = 0u;
                terminateSpell__8CreatureRC9type_info(self, __tf15SpellConeOfFire());
                set_state(o, 15);
            }
        }
        return;
    }
    word(o, 1104) = 0u;
    terminateSpell__8CreatureRC9type_info(self, __tf15SpellConeOfFire());
    set_state(o, 3);
}
