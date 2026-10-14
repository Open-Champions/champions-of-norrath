// Observed msg_run__3Orc at 0x002277d8, 5068 bytes: the per-frame routine of Orc. t is the pointer at
// object+36 (word 12 of it is a model header); the state is the halfword at +572; type is the word at
// +1116; "setState(n)" is the table entry at (word +72) + 184, called on the object adjusted by the
// entry's signed halfword with n; E is the enemy object (word 12 of the record at word +576, which is
// zero when there is no enemy). Opening steps, in order:
//  1. newDamageHandler__8CreatureP12_modelHeader(object, t[12]); a nonzero everyFrame__8Creature... result
//     returns at once; a zero objectActivationCheck__8Creaturei(object, 600) also returns at once.
//  2. advance = creatureAdvanceAnimation__8Creaturef(object, 1.0); slideBack__8Creaturef(object, 0.4);
//     the table entry at +168 is called on the object; a state of 17 or more (unsigned) returns.
// Cases (zone(r, a) is enemyInZone__8Creaturefs, zone2(...) enemyInZone2__8Creatureffssfi; floats are
// bit patterns of the executable's constants):
//   0: type below 2 and commanderValid false: joinCommander. For type 4: zone2(0, 400, angle +56, 32767,
//      0, 1) or, while type is still 4, zone2(0, 500, +56, 16000, 0, 1) set state 3. Then zone(400,
//      32767) or zone(500, 16000) set state 3, else word +256 below word +808 sets state 1.
//   1: the type 4 test, then zone(400/500) set state 0, else smartWander.   2: as 1 but a zero
//      smartWander result also sets state 0.
//   3: no enemy: state 0. Type below 4: zone(f+30, 6000) sets 6; a nearer zone turns to face the enemy;
//      otherwise (types 2/3 with byte +1132 above 0, type 1 with numInRegion >= 2 around the enemy, type 0
//      with objectIsCrowded) state 13; else one chance in 30 of a numInRegion/heading test that sets
//      state 4, else steerAndRouteToEnemy; then the enemy position (+16) is copied to +960. Type 4 or
//      more: zone(90, 32767) sets 14, zone2(0, 400, +56, 6000, 0, 1) sets 6, else steer.
//   4: no enemy: state 0. zone(f+30, 6000) sets 6, a nearer zone turns to face the enemy; one chance in
//      30 of a distance test from +960 (below about 1.333) that sets state 3, else steer; the enemy
//      position is copied to +960.
//   5: type below 4: zone(f+30, 6000) sets 6; types 4 and 5: zone2(0, 400, +56, 6000, 0, 1) sets 6;
//      bExitStatePersueIdle sets state 3.   8: bit 4 of halfword +294 clear sets 3.   9: word +804
//      below +808 sets 0.   16: word +256 below +808 sets 3.
//  13: no enemy: state 0. Type 5: zone(90) sets 14 else +1120 below +808 sets 3. Types 1 to 3: zone sets 3
//      and the type 2/3 byte and type 1 region tests set 3. Other types: zone(f+30, 6000), +256 below
//      +808 or not objectIsCrowded(..., 1) set 3. Then turnToFaceEnemy.
//  14: +808 below +256 with an enemy: flee2(object, E) when float +272 or +276 is zero, else
//      steerAndRouteToPosition with the point at +272; else state 0.
//   6: with an enemy: type 4 aims 300 units ahead, runs findEnemyLeadPosition, and when advance bit 0 is
//      set fires an arrow (heading limited to 1000 either side of the current, plus eRand%1800-900;
//      sound "swsh_arrow%d.anm" from lump "fx.lmp"; the kind in word +208 picks a cold, explode or fire
//      arrow projectile or a MissileWeapon); type 5 casts a heal on a dead ally (ISeeDeadPeople) or
//      builds a PoisonRain; other types do melee damage. All end by the shared countdown: when bit 3 of
//      +294 is clear, +256 decreases and +1112 nonzero sets bit 9 of the +160 word; at +256 <= 0 the
//      state becomes 3, or for type 5 word +1120 = now + eRand%180 + 180 and state 13.
//  10: bit 7 of +294 adds the object to the slow run list; advance bit 0 with bit 1 of word +12 clear
//      adds floor effect (floorAdd) at the position moved back 30 units along the heading; advance bit
//      1 sets bit 0 of word +12 when bit 1 is clear.   7, 11, 12, 15: nothing.
// Nothing here ran on the EE; the meaning of the fields is unknown.
#include "../w10-actor-k-common/access.hpp"

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int commanderValid__3Orc(void* self);
void joinCommander__3Orc(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
int smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void steerAndRouteToPosition__8CreatureG6Point3(void* self, Point3 position);
void flee2__8CreatureP10GameObject(void* self, void* object);
int numInRegion__8creatureG6Point3f(Point3 position, float radius);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* object, float a, float b, int c);
int iatan2__Fff(float y, float x);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
int bExitStatePersueIdle__8Creature(void* self);
void objectAddToSlowRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(Point3 position, short a, float b, int c, int d);
int sprintf(char* out, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void* ISeeDeadPeople__8Creaturef(void* self, float range);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void cold_arrow_construct(void* self, Point3 position, short heading, void* owner, int a, void* skill)
    __asm__("__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill");
void explode_arrow_construct(void* self, Point3 position, short heading, void* owner, int a, void* skill)
    __asm__("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
void fire_arrow_construct(void* self, Point3 position, short heading, void* owner, int a, void* skill)
    __asm__("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
void missile_weapon_construct(void* self, int a, void* model, void* texture, Point3 position, Point3 velocity, int heading, void* owner,
    void* vag1, void* vag2, int i1, int i2, int i3, int i4, int i5, int i6, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
void poison_rain_construct(void* self, int a, void* owner, Point3 position, void* skill, void* header)
    __asm__("__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader");
extern unsigned skills[];
extern unsigned char global_item[] __asm__("_4item$globalItem");
}

namespace {
using w10k::bits;
using w10k::half;
using w10k::real;
using w10k::word;

void set_state(unsigned char* o, int state) { w10k::call_entry(o, 184, static_cast<unsigned long>(state)); }
unsigned char* enemy_link(unsigned char* o) { return w10k::from_word(word(o, 576)); }
unsigned char* enemy_of(unsigned char* o) { return w10k::from_word(word(enemy_link(o), 12)); }
void copy_enemy_position(unsigned char* o) { *reinterpret_cast<Point3*>(o + 960) = *reinterpret_cast<Point3*>(enemy_of(o) + 16); }
}  // namespace

namespace {
// The two zone tests that state 0, 1 and 2 make for type 4 (the word at +1116); true when either
// reports an enemy. The second runs only while the type word still reads 4.
bool type4_zones(unsigned char* o, unsigned type) {
    if (enemyInZone2__8Creatureffssfi(o, 0.0f, 400.0f, half(o, 56), 32767, 0.0f, 1)) return true;
    if (word(o, 1116) == type) {
        if (enemyInZone2__8Creatureffssfi(o, 0.0f, 500.0f, half(o, 56), 16000, 0.0f, 1)) return true;
    }
    return false;
}

// The tail shared by state 6: the countdown at +256, then the follow-up state.
void attack_tail(unsigned char* o) {
    if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) == 0) {
        w10k::sword(o, 256) = w10k::sword(o, 256) - 1;
        if (word(o, 1112) != 0) w10k::dword(o, 160) |= 0x200ull;
    }
    if (w10k::sword(o, 256) > 0) return;
    if (word(o, 1116) != 5) {
        set_state(o, 3);
    } else {
        word(o, 1120) = word(o, 808) + eRand__10GameObject(o) % 180u + 180u;
        set_state(o, 13);
    }
}
}  // namespace

extern "C" void msg_run__3Orc(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = w10k::from_word(word(o, 36));
    void* const model = w10k::from_word(word(t, 12));
    newDamageHandler__8CreatureP12_modelHeader(self, model);
    if (everyFrame__8CreatureP12_modelHeader(self, w10k::from_word(word(w10k::from_word(word(o, 36)), 12)))) return;
    if (!objectActivationCheck__8Creaturei(self, 600)) return;
    const unsigned advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, bits(0x3eccccccu));
    w10k::call_entry(o, 168);
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 17u) return;
    switch (state) {
    case 0: {
        unsigned type = word(o, 1116);
        if (type < 2u) {
            if (!commanderValid__3Orc(self)) joinCommander__3Orc(self);
            type = word(o, 1116);
        }
        if (type == 4 && type4_zones(o, type)) { set_state(o, 3); return; }
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767)) { set_state(o, 3); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000)) { set_state(o, 3); return; }
        if (w10k::sword(o, 256) < w10k::sword(o, 808)) set_state(o, 1);
        return;
    }
    case 1: {
        const unsigned type = word(o, 1116);
        if (type == 4 && type4_zones(o, type)) { set_state(o, 3); return; }
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767)) { set_state(o, 0); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000)) { set_state(o, 0); return; }
        smartWander__8Creature(self);
        return;
    }
    case 2: {
        const unsigned type = word(o, 1116);
        if (type == 4 && type4_zones(o, type)) { set_state(o, 3); return; }
        if (enemyInZone__8Creaturefs(self, 400.0f, 32767)) { set_state(o, 0); return; }
        if (enemyInZone__8Creaturefs(self, 500.0f, 16000)) { set_state(o, 0); return; }
        if (!smartWander__8Creature(self)) set_state(o, 0);
        return;
    }
    case 14:
        if (w10k::sword(o, 808) < w10k::sword(o, 256) && word(o, 576) != 0) {
            if (real(o, 272) == 0.0f || real(o, 276) == 0.0f) {
                flee2__8CreatureP10GameObject(self, enemy_of(o));
            } else {
                steerAndRouteToPosition__8CreatureG6Point3(self, *reinterpret_cast<Point3*>(o + 272));
            }
            return;
        }
        set_state(o, 0);
        return;
    case 3: {
        if (word(o, 576) == 0) { set_state(o, 0); return; }
        const unsigned type = word(o, 1116);
        if (type < 4u) {
            if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 6000)) {
                set_state(o, 6);
            } else if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 32767)) {
                turnToFaceEnemy__8Creature(self);
            } else {
                unsigned kind = word(o, 1116);
                bool moved = false;
                if (kind - 2u < 2u && static_cast<signed char>(o[1132]) > 0) {
                    set_state(o, 13);
                    moved = true;
                }
                if (!moved && kind == 1) {
                    unsigned char* const enemy = enemy_of(o);
                    const Point3 at = *reinterpret_cast<Point3*>(enemy + 16);
                    if (numInRegion__8creatureG6Point3f(at, real(enemy, 28) + real(enemy, 28)) >= 2) {
                        set_state(o, 13);
                        moved = true;
                    }
                }
                if (!moved) {
                    kind = word(o, 1116);
                    if (kind == 0) {
                        if (objectIsCrowded__8CreatureP10GameObjectffi(self, enemy_of(o), 500.0f, real(o, 28) * bits(0x40400000u), 0)) {
                            set_state(o, 13);
                            moved = true;
                        } else {
                            kind = word(o, 1116);
                        }
                    }
                }
                if (!moved) {
                    bool steer = true;
                    if (kind - 2u >= 2u && eRand__10GameObject(self) % 30u == 0) {
                        unsigned char* const enemy = enemy_of(o);
                        const Point3 at = *reinterpret_cast<Point3*>(enemy + 16);
                        if (numInRegion__8creatureG6Point3f(at, real(enemy, 28) * bits(0x3fe00000u)) < 3) {
                            unsigned char* const target = enemy_of(o);
                            const int angle = iatan2__Fff(real(target, 20) - real(o, 20), real(target, 16) - real(o, 16));
                            const int diff = angle - half(enemy_of(o), 56);
                            if (static_cast<unsigned>(diff + 16383) < 32767u) {
                                set_state(o, 4);
                                steer = false;
                            }
                        }
                    }
                    if (steer) steerAndRouteToEnemy__8Creature(self);
                }
            }
            copy_enemy_position(o);
            return;
        }
        if (enemyInZone__8Creaturefs(self, bits(0x42b40000u), 32767)) { set_state(o, 14); return; }
        if (enemyInZone2__8Creatureffssfi(self, 0.0f, 400.0f, half(o, 56), 6000, 0.0f, 1)) { set_state(o, 6); return; }
        steerAndRouteToEnemy__8Creature(self);
        return;
    }
    case 4:
        if (word(o, 576) == 0) { set_state(o, 0); return; }
        if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 6000)) {
            set_state(o, 6);
        } else if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 32767)) {
            turnToFaceEnemy__8Creature(self);
        }
        if (eRand__10GameObject(self) % 30u == 0) {
            unsigned char* const enemy = enemy_of(o) + 16;
            const float dx = real(enemy, 0) - real(o, 960);
            const float dy = real(enemy, 4) - real(o, 964);
            if (__builtin_sqrtf(dx * dx + dy * dy) < bits(0x3faaaaaau)) {
                set_state(o, 3);
            } else {
                steerAndRouteToEnemy__8Creature(self);
            }
        } else {
            steerAndRouteToEnemy__8Creature(self);
        }
        copy_enemy_position(o);
        return;
    case 13: {
        if (word(o, 576) == 0) { set_state(o, 0); return; }
        const unsigned type = word(o, 1116);
        if (type == 5) {
            if (enemyInZone__8Creaturefs(self, bits(0x42b40000u), 32767)) {
                set_state(o, 14);
            } else if (w10k::sword(o, 1120) < w10k::sword(o, 808)) {
                set_state(o, 3);
            }
        } else if (type - 1u < 3u) {
            if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 32767)) {
                set_state(o, 3);
            } else {
                const unsigned kind = word(o, 1116);
                if (kind - 2u < 2u && static_cast<signed char>(o[1132]) <= 0) {
                    set_state(o, 3);
                } else if (kind == 1) {
                    unsigned char* const enemy = enemy_of(o);
                    const Point3 at = *reinterpret_cast<Point3*>(enemy + 16);
                    if (numInRegion__8creatureG6Point3f(at, real(enemy, 28) + real(enemy, 28)) < 2) set_state(o, 3);
                }
            }
        } else {
            if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 6000)) {
                set_state(o, 3);
            } else if (w10k::sword(o, 256) < w10k::sword(o, 808)) {
                set_state(o, 3);
            } else if (!objectIsCrowded__8CreatureP10GameObjectffi(self, enemy_of(o), 500.0f, real(o, 28) * bits(0x40400000u), 1)) {
                set_state(o, 3);
            }
        }
        turnToFaceEnemy__8Creature(self);
        return;
    }
    case 5: {
        unsigned type = word(o, 1116);
        bool leave = false;
        if (type < 4u) {
            if (enemyInZone__8Creaturefs(self, real(o, 28) + bits(0x41f00000u), 6000)) {
                set_state(o, 6);
                leave = true;
            } else {
                type = word(o, 1116);
            }
        }
        if (!leave && type - 4u < 2u) {
            if (enemyInZone2__8Creatureffssfi(self, 0.0f, 400.0f, half(o, 56), 6000, 0.0f, 1)) set_state(o, 6);
        }
        if (bExitStatePersueIdle__8Creature(self)) set_state(o, 3);
        return;
    }
    case 16:
        if (w10k::sword(o, 256) < w10k::sword(o, 808)) set_state(o, 3);
        return;
    case 8:
        if ((*reinterpret_cast<unsigned short*>(o + 294) & 0x10) == 0) set_state(o, 3);
        return;
    case 9:
        if (w10k::sword(o, 804) < w10k::sword(o, 808)) set_state(o, 0);
        return;
    case 10: {
        if ((*reinterpret_cast<unsigned short*>(o + 294) & 0x80) != 0) objectAddToSlowRunList__FP10GameObject(self);
        if ((advance & 1) != 0 && (word(o, 12) & 2u) == 0) {
            Point3 p = *reinterpret_cast<Point3*>(o + 16);
            p.x = p.x - icos__Fi(half(o, 56)) * bits(0x41f00000u);
            p.y = p.y - isin__Fi(half(o, 56)) * bits(0x41f00000u);
            const unsigned first = eRand__10GameObject(self);
            const unsigned second = eRand__10GameObject(self);
            floorAdd__FG6Point3sfii(p, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 30u)), 0, 1);
        }
        if ((advance & 2) != 0 && (word(o, 12) & 2u) == 0) word(o, 12) |= 1u;
        return;
    }
    case 6: {
        if (word(o, 576) != 0) {
            const unsigned type = word(o, 1116);
            if (type == 4) {
                // Archer: aim point 300 units ahead along the heading, then one projectile.
                const float cs = icos__Fi(half(o, 56));
                const float sn = isin__Fi(half(o, 56));
                Point3 aim;
                aim.x = real(o, 16) + cs * bits(0x43960000u);
                aim.y = real(o, 20) + sn * bits(0x43960000u);
                aim.z = real(o, 24);
                findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &aim);
                if ((advance & 1) != 0) {
                    Point3 muzzle;
                    muzzle.x = real(o, 16);
                    muzzle.y = real(o, 20);
                    muzzle.z = real(o, 24) + bits(0x42400000u);
                    word(o, 1124) = 0;
                    const int angle = iatan2__Fff(aim.x - muzzle.x, aim.y - real(o, 20));
                    short heading = static_cast<short>(16384 - angle);
                    const unsigned short current = *reinterpret_cast<unsigned short*>(o + 56);
                    const short diff = static_cast<short>(heading - current);
                    if (diff > 1000) {
                        heading = static_cast<short>(current + 1000);
                    } else if (diff < -1000) {
                        heading = static_cast<short>(current - 1000);
                    }
                    heading = static_cast<short>(heading + eRand__10GameObject(self) % 1800u - 900);
                    Point3 velocity;
                    velocity.x = icos__Fi(heading) * bits(0x43c80000u);
                    velocity.y = isin__Fi(heading) * bits(0x43c80000u);
                    velocity.z = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 60u) - 30);
                    char name[32];
                    sprintf(name, "swsh_arrow%d.anm", (eRand__10GameObject(self) & 3u) + 1);
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, 1.0f);
                    const int kind = w10k::sword(o, 208);
                    unsigned char* projectile;
                    if (kind == 2 || kind == 1 || kind == 3) {
                        projectile = static_cast<unsigned char*>(blockAlloc__Fi(kind == 2 ? 116 : 176));
                        if (projectile != nullptr) {
                            Point3 offset;
                            offset.x = 0.0f;
                            offset.y = 0.0f;
                            offset.z = real(o, 32) * bits(0x3f000000u) + bits(0x41400000u);
                            Point3 position;
                            position.x = real(o, 16) + offset.x;
                            position.y = real(o, 20);
                            position.z = real(o, 24) + offset.z;
                            const int one = static_cast<int>(bits(0x3f800000u));
                            if (kind == 2) {
                                explode_arrow_construct(projectile, position, heading, self, one, w10k::from_word(skills[47]));
                            } else if (kind == 1) {
                                cold_arrow_construct(projectile, position, heading, self, one, w10k::from_word(skills[44]));
                            } else {
                                fire_arrow_construct(projectile, position, heading, self, one, w10k::from_word(skills[43]));
                            }
                        }
                    } else {
                        projectile = static_cast<unsigned char*>(blockAlloc__Fi(452));
                        if (projectile != nullptr) {
                            missile_weapon_construct(projectile, 0, w10k::from_word(word(global_item, 8)), w10k::from_word(word(global_item, 12)), muzzle, velocity,
                                heading - 16384, self, nullptr, nullptr, half(o, 200), half(o, 202), 0, 0, half(o, 204), 4, nullptr);
                        }
                    }
                }
                if ((advance & 2) != 0) word(o, 1124) = 1;
                if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) != 0) w10k::call_entry(o, 176, 0);
            } else if (type == 5) {
                // Caster: the same aim point, then either a heal on a dead ally or a poison rain.
                const float cs = icos__Fi(half(o, 56));
                const float sn = isin__Fi(half(o, 56));
                Point3 aim;
                aim.x = real(o, 16) + cs * bits(0x43960000u);
                aim.y = real(o, 20) + sn * bits(0x43960000u);
                aim.z = real(o, 24);
                findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &aim);
                if ((advance & 1) != 0) {
                    unsigned char* const ally = static_cast<unsigned char*>(ISeeDeadPeople__8Creaturef(self, bits(0x43960000u)));
                    if (ally != nullptr) {
                        half(o, 146) = 50;
                        half(o, 150) = 128;
                        half(o, 152) = 255;
                        half(o, 142) = 128;
                        half(o, 144) = 255;
                        half(o, 154) = 150;
                        half(o, 140) = 0;
                        half(o, 148) = 0;
                        w10k::call_entry(ally, 184, 17);
                        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                            lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_heal.anm")), ally, 1.0f);
                    } else {
                        half(o, 146) = 50;
                        half(o, 150) = 255;
                        half(o, 154) = 100;
                        half(o, 140) = 0;
                        half(o, 142) = 255;
                        half(o, 144) = 0;
                        half(o, 148) = 0;
                        half(o, 152) = 0;
                        void* const rain = blockAlloc__Fi(108);
                        if (rain != nullptr) {
                            poison_rain_construct(rain, static_cast<int>(bits(0x3f800000u)), self, aim, w10k::from_word(skills[24]),
                                w10k::from_word(word(w10k::from_word(word(o, 36)), 364)));
                        }
                    }
                }
                if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) != 0) w10k::call_entry(o, 176, 0);
            } else {
                // Melee.
                if ((advance & 3) != 0) {
                    if ((word(enemy_of(o), 12) & 0x40u) != 0) {
                        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, bits(0x42100000u), half(o, 200), half(o, 202), half(o, 204), 8, o + 1040);
                    } else {
                        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, bits(0x42100000u), half(o, 200), half(o, 202), half(o, 204), 0x40008, o + 1040);
                    }
                    if ((advance & 1) != 0 && eRand__10GameObject(self) % 5u == 0) set_state(o, 3);
                }
                if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) != 0) turnToFaceEnemy__8Creature(self);
            }
        }
        attack_tail(o);
        return;
    }
    default:
        return;
    }
}
