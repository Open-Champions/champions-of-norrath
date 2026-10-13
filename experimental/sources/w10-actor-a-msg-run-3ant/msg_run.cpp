// Observed msg_run__3Ant at 0x00205980, 3236 bytes: the per-frame run routine of Ant.
// "this" below is the object; offsets are bytes. t is the pointer stored at +36 (a resource
// block whose word +12 is a model header). Flag F is bit 24 of the 64-bit word at +160. The state
// is the halfword at +572. "setState(n)" is the table entry at (word +72) + 184, called with the
// object adjusted by the entry's signed halfword and n; the entry at +168 is called with the
// adjusted object alone. Order of events, as observed:
//  1. If F is set, float +24 += float +972.
//  2. newDamageHandler(this, t[12]); a nonzero everyFrame(this, t[12]) returns at once, without
//     the undo in step 6; a zero objectActivationCheck(this, 600) also returns at once.
//  3. creatureAdvanceAnimation(this, 1.0) gives a flag word R (only bit 0 is used). slideBack(this,
//     0.0) when the signed byte at +968 is 2, else slideBack(this, 0.5). Then the +168 entry runs.
//  4. A state of 14 or more goes to step 6. Otherwise the state selects a case, below.
//  5. States 2, 4, 7, 11 and 12 do nothing.
//  6. If F is set, float +24 -= float +972. Return.
// Cases (e(r, s) is enemyInZone(this, r, s); w(x) is the word at +x; comparisons are signed):
//   0: if e(400, 32767) or e(500, 16000): setState(3); else if w(256) < w(808): setState(1).
//   1: if e(400, 32767) or e(500, 16000): setState(0); else smartWander(this).
//   3: w(576) == 0: setState(0). Else if e(float(28) + 20, 6000): setState(6). Else, when the byte
//      at +968 is 1 and w(980) < w(808) and e(270, 6000): setState(13). Otherwise, when
//      e(float(28) + 20, 32767): turnToFaceEnemy, else steerAndRouteToEnemy.
//   5: if e(float(28) + 20, 6000): setState(6); else when the byte at +968 is not 2 and
//      bExitStatePersueIdle(this) is nonzero: setState(3).
//   6: when w(576) is nonzero: if R bit 0 is set, w(208) = 1 or 2 selects a damage description
//      (a skill roll from skills[13] with entry code 21, or from skills[26] with code 23), any
//      other value passes none; produceMeleeDamage(this, 36.0, h200, h202, h204, 8, description)
//      runs and w(964) = 1. Then, if w(964) is zero and the byte +968 is not 2, turnToFaceEnemy.
//      Always: if halfword +294 bit 3 (mask 8) is clear, w(256) is decremented; then when
//      w(256) is at or below zero, setState(3).
//   8: if halfword +294 bit 4 (mask 16) is clear: setState(3).
//   9: if w(804) < w(808): setState(0).
//  10: death countdown, see below.
//  13: move2(this, zero vector). When w(576) is nonzero: if R bit 0 is set, one spit is fired
//      (a heading toward the enemy limited to +-4000 from the current one and perturbed by
//      eRand % 2000 - 1000, a velocity of 300 along it with z = eRand % 15 + 80, the sound
//      "spel_ant_spit.anm" from "ant.lmp", and a PoisonProjectile built from a 168-byte block);
//      then if halfword +294 bit 3 is clear: setState(3), else turnToFaceEnemy. With w(576) zero
//      only the last check runs: bit 3 clear -> setState(3).
// Death countdown (state 10): with F set: when w(976) is zero and halfword +294 has bit 7 (mask
//   128) set, w(976) = 1 and w(256) = w(808); when w(976) is nonzero and w(808) - w(256) is 81 or
//   more, three particles are added (random start offsets in the unit ball, scaled), float +972
//   += 0.15 and float +60 = float(24) - float(972); with the difference 241 or more the object is
//   added to the delete list. With F clear and halfword +294 bit 7 set the object is removed from
//   the run list. Then, when R bit 0 is set and bit 1 of w(12) is clear, floorAdd is called with
//   the point (x + icos(h56), y + isin(h56), z), a random short, 15 + (eRand & 3), 6 and 1.
// Nothing here ran on the EE. What the fields mean is unknown; names for callees come from the
// executable's symbol names.
struct Point3 { float x, y, z; };

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void smartWander__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void move2__8CreatureG6Point3(void* self, Point3 delta);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void poison_projectile_construct(void* self, void* map, int a, int b, int c, int d, Point3 position, Point3 velocity, int heading, float f, void* owner)
    __asm__("__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature");
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(Point3 position, short a, float b, int c, int d);
extern float worldDifficulty;
extern unsigned skills[];
extern unsigned char ant_msg_run_particle_7d7170[];
}

#if defined(W10_ACTOR_A_HOST)
// Host builds keep 32-bit words as offsets from a base and function words as indexes.
extern unsigned char* w10_actor_a_host_base;
void w10_actor_a_host_call1(unsigned function_word, void* object);
void w10_actor_a_host_call2(unsigned function_word, void* object, int argument);
void w10_actor_a_host_call3(unsigned function_word, void* object, int a, int b, int* result);
#endif

namespace {
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned char* from_word(unsigned value) {
#if defined(W10_ACTOR_A_HOST)
    return w10_actor_a_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
float& real(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
int sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
int half(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
bool flag(unsigned char* o) { return (*reinterpret_cast<unsigned long long*>(o + 160) & 0x01000000ull) != 0; }

// Table entries are {signed halfword adjustment, pad, function}.
unsigned char* entry(unsigned char* o, unsigned tableOffset) { return from_word(word(o, 72)) + tableOffset; }
void call_adjusted(unsigned char* o, unsigned tableOffset) {
    unsigned char* e = entry(o, tableOffset);
    short adjust = *reinterpret_cast<short*>(e);
#if defined(W10_ACTOR_A_HOST)
    w10_actor_a_host_call1(*reinterpret_cast<unsigned*>(e + 4), o + adjust);
#else
    reinterpret_cast<void (*)(void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(o + adjust);
#endif
}
void set_state(unsigned char* o, int state) {
    unsigned char* e = entry(o, 184);
    short adjust = *reinterpret_cast<short*>(e);
#if defined(W10_ACTOR_A_HOST)
    w10_actor_a_host_call2(*reinterpret_cast<unsigned*>(e + 4), o + adjust, state);
#else
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(o + adjust, state);
#endif
}
// Skill roll: the object at skills[index] has an entry at (word +96) + 96 called as (object, 0, (int)worldDifficulty).
int skill_roll(unsigned index) {
    unsigned char* skill = from_word(skills[index]);
    unsigned char* e = from_word(*reinterpret_cast<unsigned*>(skill + 96)) + 96;
    short adjust = *reinterpret_cast<short*>(e);
    int difficulty = static_cast<int>(worldDifficulty);
#if defined(W10_ACTOR_A_HOST)
    int result;
    w10_actor_a_host_call3(*reinterpret_cast<unsigned*>(e + 4), skill + adjust, 0, difficulty, &result);
    return result;
#else
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(skill + adjust, 0, difficulty);
#endif
}


// Description for a melee damage roll. Bytes 0..15 and 34..35 are never written.
struct Description {
    unsigned char raw[80];
};

void roll_description(unsigned char* object, unsigned index, short code) {
    Description description;
    unsigned char* d = description.raw;
    for (int i = 3; i >= 0; --i) *reinterpret_cast<short*>(d + 36 + 8 * i) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<short*>(d + 16 + 2 * i) = 0;
        *reinterpret_cast<short*>(d + 24 + 2 * i) = 0;
    }
    int rolled = skill_roll(index);
    float amount = static_cast<float>(rolled) * bits(0x3e4ccccc);
    float unit = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * bits(0x37800000u);
    int boosted = rolled + static_cast<int>(amount * unit);
    for (int i = 0; i < 4; ++i) {
        short existing = *reinterpret_cast<short*>(d + 36 + 8 * i);
        if (existing == -1 || existing == code) {
            *reinterpret_cast<short*>(d + 36 + 8 * i) = code;
            *reinterpret_cast<short*>(d + 36 + 8 * i + 6) = 0;
            *reinterpret_cast<short*>(d + 36 + 8 * i + 2) = static_cast<short>(rolled);
            *reinterpret_cast<short*>(d + 36 + 8 * i + 4) = static_cast<short>(boosted);
            break;
        }
    }
    produceMeleeDamage__8CreaturefiiiiPQ24item11description(object, 36.0f, half(object, 200), half(object, 202), half(object, 204), 8, d);
}

float unit_component(unsigned char* o) {
    float v = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * bits(0x37800000u);
    return v + v - 1.0f;
}

void unit_ball(unsigned char* o, Point3& p) {
    do {
        p.x = unit_component(o);
        p.y = unit_component(o);
        p.z = unit_component(o);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}

void death_particles(unsigned char* o) {
    const float scale = bits(0x41a60000u);
    for (int i = 0; i < 3; ++i) {
        Point3 start, velocity;
        unit_ball(o, start);
        start.x = start.x * scale;
        start.y = start.y * scale;
        start.z = start.z * scale * 0.75f + bits(0x3d8f5c28u);
        unit_ball(o, velocity);
        velocity.x = velocity.x * bits(0x40980000u);
        velocity.y = velocity.y * bits(0x40980000u);
        start.x = start.x + real(o, 16);
        start.y = start.y + real(o, 20);
        start.z = start.z + real(o, 24);
        velocity.z = 0.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(ant_msg_run_particle_7d7170, &start, &velocity, 0.0f);
    }
    float g = real(o, 972) + bits(0x3e199999u);
    float h = real(o, 24) - g;
    real(o, 972) = g;
    real(o, 60) = h;
}

void spit(unsigned char* o) {
    unsigned char* target = from_word(word(from_word(word(o, 576)), 12));
    Point3 aim = *reinterpret_cast<Point3*>(target + 16);
    int heading = half(o, 56);
    float c = icos__Fi(heading);
    float spawnX = real(o, 16) + c * 10.0f;
    float s = isin__Fi(heading);
    Point3 spawn;
    spawn.x = spawnX;
    spawn.y = real(o, 20) + s * 10.0f;
    spawn.z = real(o, 24) + 12.0f;
    int toEnemy = iatan2__Fff(aim.x - real(o, 16), aim.y - real(o, 20));
    short desired = static_cast<short>(16384 - toEnemy);
    unsigned short current = *reinterpret_cast<unsigned short*>(o + 56);
    short diff = static_cast<short>(desired - current);
    if (diff >= 4001) desired = static_cast<short>(current + 4000);
    else if (diff < -4000) desired = static_cast<short>(current - 4000);
    unsigned spread = eRand__10GameObject(o) % 2000u;
    short flight = static_cast<short>(desired + spread - 1000);
    float vx = icos__Fi(flight) * 300.0f;
    float vy = isin__Fi(flight) * 300.0f;
    Point3 velocity;
    velocity.x = vx;
    velocity.y = vy;
    velocity.z = static_cast<float>(eRand__10GameObject(o) % 15u) + 80.0f;
    void* header = lumpFindResource__FPcT0(const_cast<char*>("ant.lmp"), const_cast<char*>("spel_ant_spit.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, o, 1.0f);
    unsigned char* projectile = static_cast<unsigned char*>(blockAlloc__Fi(168));
    if (projectile != nullptr) {
        float f = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * bits(0x37800000u) * 0.5f + 0.5f;
        poison_projectile_construct(projectile, from_word(word(o, 36)), 15, 16, 17, 19, spawn, velocity,
                                    half(o, 56) - 16384, f, o);
    }
}

void floor_splash(unsigned char* o) {
    Point3 point;
    point.x = real(o, 16) + icos__Fi(half(o, 56));
    point.y = real(o, 20) + isin__Fi(half(o, 56));
    point.z = real(o, 24);
    short a = static_cast<short>(eRand__10GameObject(o));
    unsigned b = eRand__10GameObject(o);
    floorAdd__FG6Point3sfii(point, a, static_cast<float>(static_cast<int>(15 + (b & 3))), 6, 1);
}
}  // namespace

extern "C" void msg_run__3Ant(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (flag(o)) real(o, 24) = real(o, 24) + real(o, 972);
    newDamageHandler__8CreatureP12_modelHeader(o, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(o, from_word(word(from_word(word(o, 36)), 12)))) return;
    if (!objectActivationCheck__8Creaturei(o, 600)) return;
    unsigned advance = creatureAdvanceAnimation__8Creaturef(o, 1.0f);
    if (static_cast<signed char>(o[968]) == 2) slideBack__8Creaturef(o, 0.0f);
    else slideBack__8Creaturef(o, 0.5f);
    call_adjusted(o, 168);
    unsigned state = static_cast<unsigned>(half(o, 572));
    if (state < 14) {
        switch (state) {
        case 0:
            if (enemyInZone__8Creaturefs(o, 400.0f, 32767) || enemyInZone__8Creaturefs(o, 500.0f, 16000)) set_state(o, 3);
            else if (sword(o, 256) < sword(o, 808)) set_state(o, 1);
            break;
        case 1:
            if (enemyInZone__8Creaturefs(o, 400.0f, 32767) || enemyInZone__8Creaturefs(o, 500.0f, 16000)) set_state(o, 0);
            else smartWander__8Creature(o);
            break;
        case 3:
            if (word(o, 576) == 0) {
                set_state(o, 0);
            } else if (enemyInZone__8Creaturefs(o, real(o, 28) + 20.0f, 6000)) {
                set_state(o, 6);
            } else {
                bool queued = false;
                if (static_cast<signed char>(o[968]) == 1 && sword(o, 980) < sword(o, 808) &&
                    enemyInZone__8Creaturefs(o, 270.0f, 6000)) {
                    set_state(o, 13);
                    queued = true;
                }
                if (!queued) {
                    if (enemyInZone__8Creaturefs(o, real(o, 28) + 20.0f, 32767)) turnToFaceEnemy__8Creature(o);
                    else steerAndRouteToEnemy__8Creature(o);
                }
            }
            break;
        case 5:
            if (enemyInZone__8Creaturefs(o, real(o, 28) + 20.0f, 6000)) {
                set_state(o, 6);
            } else if (static_cast<signed char>(o[968]) != 2) {
                if (bExitStatePersueIdle__8Creature(o)) set_state(o, 3);
            }
            break;
        case 6:
            if (word(o, 576) != 0) {
                if ((advance & 1) != 0) {
                    unsigned kind = word(o, 208);
                    if (kind == 1) roll_description(o, 13, 21);
                    else if (kind == 2) roll_description(o, 26, 23);
                    else produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, 36.0f, half(o, 200), half(o, 202), half(o, 204), 8, nullptr);
                    word(o, 964) = 1;
                }
                if (word(o, 964) == 0 && static_cast<signed char>(o[968]) != 2) turnToFaceEnemy__8Creature(o);
            }
            if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) == 0) word(o, 256) = word(o, 256) - 1;
            if (sword(o, 256) <= 0) set_state(o, 3);
            break;
        case 8:
            if ((*reinterpret_cast<unsigned short*>(o + 294) & 16) == 0) set_state(o, 3);
            break;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 0);
            break;
        case 10:
            if (flag(o)) {
                if (word(o, 976) == 0) {
                    if ((*reinterpret_cast<unsigned short*>(o + 294) & 0x80) != 0) {
                        word(o, 976) = 1;
                        word(o, 256) = word(o, 808);
                    }
                } else {
                    int remaining = sword(o, 808) - sword(o, 256);
                    if (remaining > 80) {
                        death_particles(o);
                        remaining = sword(o, 808) - sword(o, 256);
                    }
                    if (remaining > 240) objectAddToDeleteList__FP10GameObject(o);
                }
            } else if ((*reinterpret_cast<unsigned short*>(o + 294) & 0x80) != 0) {
                objectRemoveFromRunList__FP10GameObject(o);
            }
            if ((advance & 1) != 0 && (word(o, 12) & 2) == 0) floor_splash(o);
            break;
        case 13: {
            Point3 zero = {0.0f, 0.0f, 0.0f};
            move2__8CreatureG6Point3(o, zero);
            if (word(o, 576) != 0) {
                if ((advance & 1) != 0) spit(o);
                if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) == 0) {
                    set_state(o, 3);
                    break;
                }
                turnToFaceEnemy__8Creature(o);
            }
            if ((*reinterpret_cast<unsigned short*>(o + 294) & 8) == 0) set_state(o, 3);
            break;
        }
        default:
            break;
        }
    }
    if (flag(o)) real(o, 24) = real(o, 24) - real(o, 972);
}
