#include "../w10-actor-m-common/access.hpp"
// Observed msg_run__8Skeleton at 0x244880, 2924 bytes. Nothing is returned. T is the table at the word at
// object+36 (entry 12 is the model header). "setState(n)" is table entry 184 of the object's table (slot 23)
// called on the adjusted object with n; "turnTo(P)" is entry 176 (slot 22) called with (0, &P). h294 is the
// unsigned halfword at +294; word256 and word808 are compared as signed. f28 is the float at +28.
//  1. newDamageHandler__8CreatureP12_modelHeader(object, T12); everyFrame__8CreatureP12_modelHeader(object, T12)
//     nonzero ends the function; objectActivationCheck__8Creaturei(object, 600) returning zero ends it too.
//  2. s = creatureAdvanceAnimation__8Creaturef(object, 1.0f) (still in V0 when the image copies it into a saved
//     register in the delay slot of the slideBack call); slideBack__8Creaturef(object, 0.7f); then table entry 168
//     (slot 21) is called on the adjusted object.
//  3. The state is the signed halfword at +572; 0..15 only. Jump table at 0x4740a0:
//   0: when the signed byte at +972 is nonzero and enemyInZone__8Creaturefs(object, 200.0f, 32767) is nonzero ->
//      setState(3); word576 != 0 -> setState(3); word256 < word808 -> setState(1).
//   1: same two tests; otherwise smartWander__8Creature.
//   3: word576 == 0 -> setState(0). word968 == 4: enemyInZone2__8Creatureffssfi(object, 0.0f, 400.0f, halfword at
//      +56, 2000, 0.0f, 1) nonzero -> setState(14); else the same call with 32767 decides turnToFaceEnemy (nonzero)
//      or steerAndRouteToEnemy. word968 != 4: enemyInZone(f28 + 20.0f, 6000) nonzero -> setState(6); else
//      objectIsCrowded__8CreatureP10GameObjectffi(object, the word at word576+12, 300.0f, f28*3.0f, 0) (f28 read
//      again) nonzero -> word256 = word808 + (roll % 240) + 240 (stored before the call) and setState(5);
//      otherwise enemyInZone(f28 + 20.0f, 32767) decides turnToFaceEnemy or steerAndRouteToEnemy.
//   5: word968 == 4: enemyInZone(400.0f, 2000) nonzero -> setState(14); and if word968 changed during the call
//      the next test is taken; then enemyInZone(f28 + 20.0f, 6000) nonzero -> setState(6). Then
//      bExitStatePersueIdle__8Creature zero ends; word256 < word808 -> setState(3); word576 == 0 ends;
//      objectIsCrowded(object, word576+12 word, 300.0f, f28*3.0f, 1) zero -> setState(3).
//   6: with word576 != 0: bit 0 of s set -> produceMeleeDamage__8CreaturefiiiiPQ24item11description(object, 36.0f,
//      halfwords at +200, +202, +204, 8, object+1048); bit 3 of h294 clear -> the 64-bit word at +160 gains 0x200,
//      otherwise turnToFaceEnemy and h294 is read again. Then, if bit 3 of h294 is clear, word256 is decremented;
//      word256 above zero ends, else setState(3).
//   8: bit 4 (0x10) of h294 clear -> setState(3).    9: word804 < word808 -> setState(0).
//   10: objectRemoveFromRunList__FP10GameObject(object).    15: word256 < word808 -> setState(3).
//   14: word576 == 0 -> setState(0) and the counter step below. Otherwise a shot is prepared: the point P is the
//      object position pushed 300.0f along the facing (icos__Fi / isin__Fi of the halfword at +56) with the
//      position's z; findEnemyLeadPosition__8CreaturefP6Point3(object, 400.0f, P) refines it. When bit 0 of s is
//      set, the muzzle Q = (x, y, z + 0.5*float at +32 + 12.0f), the word at +1124 is cleared, the direction
//      D = P - Q gives an angle iatan2__Fff(D.y, D.x); if that differs from the halfword at +56 by 1001 or
//      more, P is rebuilt from the facing (without the lead) and the angle is taken again from the same D.
//      D is normalised with rsqrt.s of its squared length, scaled by 400.0f, then a random vector from the
//      unit-ball rejection loop scaled by 48.0f is added. A sound "swsh_arrow%d.anm" with (roll & 3) + 1 is
//      looked up in lump "fx.lmp" and played on the object at gain 1.0. By the word at +208:
//        1: a 176-byte ColdArrowProjectile (skills word at 0x34b4f0); 2: a 116-byte ExplodeArrowProjectile (word at
//        0x34b4fc); 3: a 176-byte FireArrowProjectile (word at 0x34b4ec); each constructed with (Q, the angle,
//        object, (int)worldDifficulty, the skill word).
//        4: the virtual method at table 96 of the Skill object skills+184 (arguments 2 and (int)worldDifficulty)
//        returns n; for n > 0, n 120-byte MultiFireProjectiles with angles starting at angle - (int)(n*0.5*500.0f)
//        and rising by 1000 as a short;
//        otherwise a 452-byte MissileWeapon (arguments in the code).
//      Then, with or without the shot: bit 1 of s sets the word at +1124 to 1. If bit 3 of h294 is set,
//      turnTo(P) runs (and h294 is read again). Then the counter step: bit 3 of h294 clear -> word256 is
//      decremented; word256 above zero ends, else setState(3).
// Inferred: archer behaviour; names come from the callees. Unknown: the state meanings, the fields at +208,
// +256, +576, +804, +808, +968, +972, +1124, the arrow counts and the constants' units.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short mask);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short angle, short mask, float c, int flag);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int flag);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
int eRand__10GameObject(void* object);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float gain);
void* blockAlloc__Fi(int size);
// The angle parameter is declared int: the image passes the raw 32-bit result of iatan2 (the receiving
// routines read it as a short).
void ColdArrowProjectile(void* self, Point3* where, int angle, void* owner, int level, void* skill) __asm__("__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill");
void ExplodeArrowProjectile(void* self, Point3* where, int angle, void* owner, int level, void* skill) __asm__("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
void FireArrowProjectile(void* self, Point3* where, int angle, void* owner, int level, void* skill) __asm__("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
void MultiFireProjectile(void* self, Point3* where, int angle, void* owner, int level, void* skill) __asm__("__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill");
void MissileWeapon(void* self, int zero, void* model, void* texture, Point3* where, Point3* velocity, int angle, void* creature, void* sound_a,
                   void* sound_b, int a, int b, int c, int d, int e, int f, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
extern unsigned char skills[];
extern unsigned char _4item$globalItem[];
extern float worldDifficulty;
}

namespace {
inline void set_state(unsigned char* o, int state) { w10m::call_entry(o, 184, static_cast<unsigned long>(state)); }
inline unsigned short h294(unsigned char* o) { return static_cast<unsigned short>(w10m::half(o, 294)); }
inline void counter_step(unsigned char* o, unsigned short h) {
    if ((h & 8u) == 0) w10m::word(o, 256) = w10m::word(o, 256) - 1u;
    if (static_cast<int>(w10m::word(o, 256)) > 0) return;
    set_state(o, 3);
}
inline float reciprocal_root(float length_squared) {
    const float one = 1.0f;
#if defined(__mips__)
    float result;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return one / __builtin_sqrtf(length_squared);
#endif
}
inline void* skill_word(unsigned offset) { return w10m::from_word(w10m::word(skills, offset)); }
inline Point3 facing_point(unsigned char* o) {
    const float c = icos__Fi(w10m::half(o, 56));
    const float s = isin__Fi(w10m::half(o, 56));
    Point3 p;
    p.x = w10m::real(o, 16) + c * 300.0f;
    p.y = w10m::real(o, 20) + s * 300.0f;
    p.z = w10m::real(o, 24);
    return p;
}
inline void face_or_steer(void* self, int found) {
    if (found != 0) turnToFaceEnemy__8Creature(self);
    else steerAndRouteToEnemy__8Creature(self);
}
inline bool less(unsigned char* o, unsigned left, unsigned right) {
    return static_cast<int>(w10m::word(o, left)) < static_cast<int>(w10m::word(o, right));
}
}  // namespace

extern "C" void msg_run__8Skeleton(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12))) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advanced = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, w10m::bits(0x3f333333u));
    w10m::call_entry(o, 168);
    switch (static_cast<unsigned>(static_cast<int>(w10m::half(o, 572)))) {
    case 0u:
        if (static_cast<signed char>(o[972]) != 0 && enemyInZone__8Creaturefs(self, w10m::bits(0x43480000u), 32767) != 0) {
            set_state(o, 3);
        } else if (w10m::word(o, 576) != 0) {
            set_state(o, 3);
        } else if (less(o, 256, 808)) {
            set_state(o, 1);
        }
        break;
    case 1u:
        if (static_cast<signed char>(o[972]) != 0 && enemyInZone__8Creaturefs(self, w10m::bits(0x43480000u), 32767) != 0) {
            set_state(o, 3);
        } else if (w10m::word(o, 576) != 0) {
            set_state(o, 3);
        } else {
            smartWander__8Creature(self);
        }
        break;
    case 3u:
        if (w10m::word(o, 576) == 0) {
            set_state(o, 0);
        } else if (w10m::word(o, 968) == 4u) {
            if (enemyInZone2__8Creatureffssfi(self, 0.0f, w10m::bits(0x43c80000u), w10m::half(o, 56), 2000, 0.0f, 1) != 0) {
                set_state(o, 14);
            } else {
                face_or_steer(self, enemyInZone2__8Creatureffssfi(self, 0.0f, w10m::bits(0x43c80000u), w10m::half(o, 56), 32767, 0.0f, 1));
            }
        } else if (enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41a00000u), 6000) != 0) {
            set_state(o, 6);
        } else if (objectIsCrowded__8CreatureP10GameObjectffi(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 576)), 12)),
                                                              w10m::bits(0x43960000u), w10m::real(o, 28) * w10m::bits(0x40400000u), 0) != 0) {
            const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 240u;
            w10m::word(o, 256) = w10m::word(o, 808) + roll + 240u;
            set_state(o, 5);
        } else {
            face_or_steer(self, enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41a00000u), 32767));
        }
        break;
    case 5u: {
        const unsigned variant = w10m::word(o, 968);
        bool zone_checked = false;
        if (variant == 4u) {
            if (enemyInZone__8Creaturefs(self, w10m::bits(0x43c80000u), 2000) != 0) {
                set_state(o, 14);
                break;
            }
            zone_checked = (w10m::word(o, 968) == variant);
        }
        if (!zone_checked && enemyInZone__8Creaturefs(self, w10m::real(o, 28) + w10m::bits(0x41a00000u), 6000) != 0) {
            set_state(o, 6);
            break;
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) break;
        if (less(o, 256, 808)) {
            set_state(o, 3);
            break;
        }
        if (w10m::word(o, 576) == 0) break;
        if (objectIsCrowded__8CreatureP10GameObjectffi(self, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 576)), 12)),
                                                       w10m::bits(0x43960000u), w10m::real(o, 28) * w10m::bits(0x40400000u), 1) != 0) break;
        set_state(o, 3);
        break;
    }
    case 6u: {
        unsigned short h;
        if (w10m::word(o, 576) != 0) {
            if ((advanced & 1) != 0) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, w10m::bits(0x42100000u), w10m::half(o, 200), w10m::half(o, 202),
                                                                       w10m::half(o, 204), 8, o + 1048);
            }
            h = h294(o);
            if ((h & 8u) == 0) {
                w10m::dword(o, 160) = w10m::dword(o, 160) | 0x200ull;
            } else {
                turnToFaceEnemy__8Creature(self);
                h = h294(o);
            }
        } else {
            h = h294(o);
        }
        counter_step(o, h);
        break;
    }
    case 8u:
        if ((h294(o) & 0x10u) == 0) set_state(o, 3);
        break;
    case 9u:
        if (less(o, 804, 808)) set_state(o, 0);
        break;
    case 10u:
        objectRemoveFromRunList__FP10GameObject(self);
        break;
    case 14u: {
        if (w10m::word(o, 576) == 0) {
            set_state(o, 0);
            counter_step(o, h294(o));
            break;
        }
        Point3 target = facing_point(o);
        findEnemyLeadPosition__8CreaturefP6Point3(self, w10m::bits(0x43c80000u), &target);
        if ((advanced & 1) != 0) {
            Point3 muzzle;
            muzzle.x = w10m::real(o, 16);
            muzzle.y = w10m::real(o, 20);
            muzzle.z = (w10m::real(o, 24) + w10m::real(o, 32) * 0.5f) + w10m::bits(0x41400000u);
            w10m::word(o, 1124) = 0;
            Point3 direction;
            direction.y = target.y - muzzle.y;
            direction.z = target.z - muzzle.z;
            direction.x = target.x - muzzle.x;
            int angle = iatan2__Fff(direction.y, direction.x);
            int difference = static_cast<int>(static_cast<unsigned>(angle) - static_cast<unsigned>(static_cast<int>(w10m::half(o, 56))));
            if (difference < 0) difference = static_cast<int>(0u - static_cast<unsigned>(difference));
            if (difference >= 1001) {
                target = facing_point(o);
                angle = iatan2__Fff(direction.y, direction.x);
            }
            const float inverse = reciprocal_root(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
            direction.x = direction.x * inverse;
            direction.y = direction.y * inverse;
            direction.z = direction.z * inverse;
            direction.x = direction.x * w10m::bits(0x43c80000u);
            direction.y = direction.y * w10m::bits(0x43c80000u);
            direction.z = direction.z * w10m::bits(0x43c80000u);
            Point3 scatter;
            w10m::unit_ball(o, scatter, [](unsigned char* object) { return eRand__10GameObject(object); });
            direction.x = direction.x + scatter.x * w10m::bits(0x42400000u);
            direction.y = direction.y + scatter.y * w10m::bits(0x42400000u);
            direction.z = direction.z + scatter.z * w10m::bits(0x42400000u);
            char name[32];
            sprintf(name, "swsh_arrow%d.anm", (eRand__10GameObject(self) & 3) + 1);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), self, 1.0f);
            const int kind = static_cast<int>(w10m::word(o, 208));
            const int level = static_cast<int>(worldDifficulty);
            if (kind == 2) {
                void* made = blockAlloc__Fi(116);
                if (made != nullptr) ExplodeArrowProjectile(made, &muzzle, angle, self, level, skill_word(188));
            } else if (kind == 1) {
                void* made = blockAlloc__Fi(176);
                if (made != nullptr) ColdArrowProjectile(made, &muzzle, angle, self, level, skill_word(176));
            } else if (kind == 3) {
                void* made = blockAlloc__Fi(176);
                if (made != nullptr) FireArrowProjectile(made, &muzzle, angle, self, level, skill_word(172));
            } else if (kind == 4) {
                unsigned char* skill = static_cast<unsigned char*>(skill_word(184));
                const int count = static_cast<int>(w10m::call_entry_from(skill, 96, 96, 2, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
                short spread = static_cast<short>(angle - static_cast<int>(static_cast<float>(count) * 0.5f * w10m::bits(0x43fa0000u)));
                for (int i = 0; i < count; ++i) {
                    void* made = blockAlloc__Fi(120);
                    if (made != nullptr) MultiFireProjectile(made, &muzzle, spread, self, static_cast<int>(worldDifficulty), skill_word(184));
                    spread = static_cast<short>(spread + 1000);
                }
            } else {
                void* made = blockAlloc__Fi(452);
                if (made != nullptr) {
                    MissileWeapon(made, 0, w10m::from_word(w10m::word(_4item$globalItem, 8)), w10m::from_word(w10m::word(_4item$globalItem, 12)), &muzzle,
                                  &direction, angle - 16384, self, nullptr, nullptr, w10m::half(o, 200), w10m::half(o, 202) / 2, 0, 0,
                                  w10m::half(o, 204), 4, nullptr);
                }
            }
        }
        if ((advanced & 2) != 0) w10m::word(o, 1124) = 1;
        unsigned short h = h294(o);
        if ((h & 8u) != 0) {
            w10m::call_entry(o, 176, 0, reinterpret_cast<__UINTPTR_TYPE__>(&target));
            h = h294(o);
        }
        counter_step(o, h);
        break;
    }
    case 15u:
        if (less(o, 256, 808)) set_state(o, 3);
        break;
    default:
        break;
    }
}
