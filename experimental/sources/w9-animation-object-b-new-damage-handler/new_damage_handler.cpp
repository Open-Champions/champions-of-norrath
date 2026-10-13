// Observed newDamageHandler__8CreatureP12_modelHeader at 0x10d428, extent 5872. The
// creature's reaction to damage that was just applied: it spawns the visual effects for
// the damage type, handles death (experience, life tap), and otherwise sets the hurt
// reaction. Arguments after the creature: the creature's model (may be null). No result.
// Creature words used: +12 flags, +16/+20/+24 position, +28 radius, +56 and +748
// halfword angles, +72 virtual table, +160 64-bit flags, +192 a float "pending damage",
// +220/+224/+228/+560/+564/+756/+856..+872 floats, +288 the animation state, +572 a
// halfword state, +744 the damage-type bit set, +752 a pointer to the attacker, +824 an
// effect state record. Damage-type bits (+744): 0x1 0x4 0x8 take the gore path (0xd);
// 0x10, 0x20 and 0x2000000 select disintegration or removal; 0x40800 together (64-bit
// test) allow the debris spray; 0x40000/0x80000/0x100000/0x200000 pick the hit speed in
// +756; 0x400000 skips the reaction; 0x4000000 allows a second state call.
// Outline, in image order:
//   1. If the float at +192 is not above zero the damage was already handled: only the
//      hit-speed table (50, 75, 100 or a constant) and the state call run.
//   2. With nmPlayers == 1 an alert message is sent from the position (600.0). The float
//      at +192 becomes 0. A state of 10 returns at once, leaving +744 untouched. Flag
//      0x100 is set in +12.
//   3. When +744 & 0xd is nonzero: the effect point p is the position raised 36.0 (and,
//      for a radius of at least 25, moved 0.75 radius along the facing + 32767, and raised
//      by half the model's height when the box top is under 36). The effect is chosen
//      from the float at +860 and the 64-bit flags: 0x4000 effect 10, 0x8000 effect 11,
//      0x10000 effect 2, 0x20000 a sound plus fifty particles, 0x2000 effect 3, 0x1000
//      effect 9, else (blood setting or network mode, and flag 0x40000) effect 0; gut
//      chunks follow with chance one in three.
//   4. If the float at +560 is above zero the creature is hurt and not dead: hit speed,
//      and the state-8 call. Otherwise it is dead: floats +856..+872 are clamped to 1.0,
//      flags |= 9, disintegration/freezing/debris effects run depending on the bits, the
//      experience and life-tap rewards are given to the attacking player(s), and the
//      "Dead" tag decides whether the animation jumps to its end.
// Each routine of the original that is reached through shared tails is kept as goto
// labels so the order of the random draws is the original's. Draws from the creature's
// own generator (eRand) and from asyncRandState are marked where they matter.
// Inferred: this is Creature::newDamageHandler. Unknown: the meaning of most bits and of
// the constants (hit speeds 100/150/200/225 and 312.5, 375.0, 156.25), the gut-chunk and
// effect numbering, and the attacker key at +12 of the attacker object.

struct Point3 {
    float x, y, z;
};
struct VirtualEntry {
    short delta;
    short reserved;
    unsigned function;
};
static_assert(sizeof(VirtualEntry) == 8, "table entries are eight bytes");
// Host contracts define NORRATH_HOST_CONTRACT: a host function address does not fit the 32-bit
// entry word, so they supply the handle -> function mapping. On the target the word is the address.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_function(unsigned handle);
#endif

struct DamageCreature {
    unsigned char before_flags[12];
    unsigned flags;
    Point3 position;
    float radius;
    unsigned char before_angle[56 - 32];
    short angle;
    unsigned char before_vtable[72 - 58];
    const VirtualEntry* vtable;
    unsigned char before_flags64[160 - 76];
    unsigned long long flags64;
    unsigned char before_pending[192 - 168];
    float pending;
    unsigned char before_220[220 - 196];
    float f220;
    float f224;
    float f228;
    unsigned char before_animation[288 - 232];
    unsigned char animation_state[560 - 288];
    float f560;
    float f564;
    unsigned char before_state[572 - 568];
    short state;
    unsigned char before_bits[744 - 574];
    unsigned bits;
    short facing;
    unsigned char pad750[2];
    const void* attacker;
    float hit_speed;
    unsigned char before_effect[824 - 760];
    unsigned char effect_state[856 - 824];
    float f856;
    float f860;
    float f864;
    float f868;
    float f872;
};
struct AttackerKey {
    unsigned char before_key[12];
    unsigned key;
};
struct Chunk {
    unsigned char before_velocity[252];
    float velocity_x;
    float velocity_y;
};
struct PlayerView {
    unsigned char before_80[80];
    int word80;
    unsigned char before_89[89 - 84];
    signed char byte89;
    unsigned char before_172[172 - 90];
    float float172;
    unsigned char before_196[196 - 176];
    float float196;
    float float200;
    unsigned char before_21872[21872 - 204];
    unsigned key21872;
    unsigned char tail[23984 - 21876];
};
#if defined(__mips__)
static_assert(__builtin_offsetof(DamageCreature, flags) == 12 && __builtin_offsetof(DamageCreature, position) == 16 &&
                  __builtin_offsetof(DamageCreature, radius) == 28 && __builtin_offsetof(DamageCreature, angle) == 56 &&
                  __builtin_offsetof(DamageCreature, vtable) == 72 && __builtin_offsetof(DamageCreature, flags64) == 160 &&
                  __builtin_offsetof(DamageCreature, pending) == 192 && __builtin_offsetof(DamageCreature, f220) == 220 &&
                  __builtin_offsetof(DamageCreature, f228) == 228 && __builtin_offsetof(DamageCreature, animation_state) == 288 &&
                  __builtin_offsetof(DamageCreature, f560) == 560 && __builtin_offsetof(DamageCreature, f564) == 564 &&
                  __builtin_offsetof(DamageCreature, state) == 572 && __builtin_offsetof(DamageCreature, bits) == 744 &&
                  __builtin_offsetof(DamageCreature, facing) == 748 && __builtin_offsetof(DamageCreature, attacker) == 752 &&
                  __builtin_offsetof(DamageCreature, hit_speed) == 756 && __builtin_offsetof(DamageCreature, effect_state) == 824 &&
                  __builtin_offsetof(DamageCreature, f856) == 856 && __builtin_offsetof(DamageCreature, f872) == 872 &&
                  __builtin_offsetof(AttackerKey, key) == 12 && __builtin_offsetof(Chunk, velocity_y) == 256 &&
                  __builtin_offsetof(PlayerView, word80) == 80 && __builtin_offsetof(PlayerView, byte89) == 89 &&
                  __builtin_offsetof(PlayerView, float172) == 172 && __builtin_offsetof(PlayerView, float196) == 196 &&
                  __builtin_offsetof(PlayerView, float200) == 200 && __builtin_offsetof(PlayerView, key21872) == 21872 &&
                  sizeof(PlayerView) == 23984, "observed offsets");
#endif

extern "C" {
void sendAlertMessage__FifG6Point3P10GameObject(int kind, float radius, const Point3* from, void* object);
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* model, int index, int* a, int* b, int* c, int* d, int* e, int* f);
float icos__Fi(int angle);
float isin__Fi(int angle);
void addEffect__FiG6Point3(int kind, const Point3* where);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3iif(void* chunk, const Point3* where, int a, int b, float scale);
int eRand__10GameObject(void* object);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const Point3* at, short angle, void* animation,
                                                                                         int a, Point3* out, int b, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* where, Point3* velocity, float life);
void floorAdd__FG6Point3sfii(const Point3* where, short a, float scale, int b, int c);
void initStuff__6PhPropi(void* chunk, int kind);
void disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(void* object, void* model, void* animation, const Point3* at,
                                                                                         short angle, float a, float b, int flag);
void fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(void* model, void* animation, const Point3* at, short angle,
                                                                                         void* state, float a, float b, float c);
int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
unsigned animSetToEndFullyBlended__FP14AnimationState(void* state);
float powf(float base, float exponent);
int playerGetSkillRank__Fii(int player, int skill);
float getChance__12SkillLifeTapi(void* skill, int rank);
void __7OniBallG6Point3iP10GameObjectP10_vagHeader(void* ball, const Point3* at, int angle, void* owner, void* header);
int gameCheckPlayerAlive__Fv();
void givePlayerExperience__Fii(int player, int amount);
extern int nmPlayers;
extern int g_BloodLevel;
extern signed char netNetworkMode;
extern int asyncRandState;
extern unsigned char trailParticleSpark[];
extern PlayerView player[];
extern void* skills[];
}

namespace {
void call_entry(DamageCreature* object, unsigned offset, int argument) {
    const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(reinterpret_cast<const char*>(object->vtable) + offset);
#if defined(NORRATH_HOST_CONTRACT)
    void* function = norrath_host_function(entry->function);
#else
    void* function = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(entry->function));
#endif
    reinterpret_cast<void (*)(void*, int)>(function)(reinterpret_cast<char*>(object) + entry->delta, argument);
}
// One gut chunk at a fixed point: allocate, construct with the given numbers.
void gut(const Point3* where, int a, int b) {
    void* chunk = blockAlloc__Fi(616);
    if (chunk != nullptr) __8GutChunkG6Point3iif(chunk, where, a, b, 1.0f);
}
// One thrown chunk: the chunk's own velocity gets an extra push at a random angle.
void push_chunk(DamageCreature* self, void* chunk_memory, float factor, float magnitude_scale, float magnitude_base, unsigned spread_range,
                unsigned spread_base) {
    Chunk* chunk = static_cast<Chunk*>(chunk_memory);
    float magnitude = static_cast<float>(eRand__10GameObject(self));
    magnitude = magnitude * 0x1.0p-16f;
    magnitude = magnitude * magnitude_scale;
    magnitude = magnitude + magnitude_base;
    const unsigned draw = static_cast<unsigned>(eRand__10GameObject(self));
    const short offset = static_cast<short>(spread_base + draw % spread_range);
    const float c = icos__Fi(self->facing + offset);
    chunk->velocity_x = chunk->velocity_x * factor + c * magnitude;
    const float s = isin__Fi(self->facing + offset);
    chunk->velocity_y = chunk->velocity_y * factor + s * magnitude;
    initStuff__6PhPropi(chunk, 18);
}
}

extern "C" void newDamageHandler__8CreatureP12_modelHeader(DamageCreature* self, void* model) {
    Point3 p;
    int box[6];
    Point3 pos_copy;
    unsigned long long fl64;
    unsigned bits;
    static char dead_tag[] = "Dead";
    static char fx_lump[] = "fx.lmp";
    char name[64];

    if (!(0.0f < self->pending)) goto already_handled;

    if (nmPlayers == 1) {
        pos_copy = self->position;
        sendAlertMessage__FifG6Point3P10GameObject(0, 600.0f, &pos_copy, self);
    }
    self->pending = 0.0f;
    if (self->state == 10) return;
    self->flags |= 0x100u;
    if ((self->bits & 0xdu) == 0) goto check_health;

    for (int i = 0; i < 6; ++i) box[i] = 0;
    if (model != nullptr) modelGetBoundingBox__FP12_modelHeaderlPiN52(model, -1, &box[0], &box[1], &box[2], &box[3], &box[4], &box[5]);
    if (25.0f <= self->radius) {
        const float c = icos__Fi(self->facing + 32767);
        const float x = self->position.x + c * (self->radius * 0.75f);
        const float s = isin__Fi(self->facing + 32767);
        p.x = x;
        p.y = self->position.y + s * (self->radius * 0.75f);
        p.z = self->position.z + 36.0f;
    } else {
        p.x = self->position.x;
        p.y = self->position.y;
        p.z = self->position.z + 36.0f;
    }
    if (box[5] < 36) {
        const int span = box[5] - box[2];
        if (span > 0) p.z = (self->position.z + static_cast<float>(span / 2)) + 12.0f;
    }

    if (0.0f < self->f860) {
        addEffect__FiG6Point3(10, &p);
        goto check_health;
    }
    fl64 = self->flags64;
    if ((fl64 & 0x4000u) != 0) {
        addEffect__FiG6Point3(10, &p);
        goto check_health;
    }
    if ((fl64 & 0x8000u) != 0) {
        addEffect__FiG6Point3(11, &p);
        goto check_health;
    }
    if ((fl64 & 0x10000u) != 0) {
        addEffect__FiG6Point3(2, &p);
        goto check_health;
    }
    if ((fl64 & 0x20000u) != 0) {
        sprintf(name, "hit_mtlplate_bludg_med.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(fx_lump, name), self, 1.0f);
        Point3 attach_a, attach_b, mid, velocity, random;
        pos_copy = self->position;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, &pos_copy, self->angle, self->animation_state, 0, &attach_a, 0, 1.0f);
        pos_copy = self->position;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, &pos_copy, self->angle, self->animation_state, 1, &attach_b, 0, 1.0f);
        mid.x = (attach_a.x + attach_b.x) * 0.5f;
        mid.y = (attach_a.y + attach_b.y) * 0.5f;
        mid.z = (attach_a.z + attach_b.z) * 0.5f;
        velocity.x = icos__Fi(self->angle) * 10.0f;
        velocity.y = isin__Fi(self->angle) * 10.0f;
        velocity.z = 10.0f;
        for (int n = 0; n < 50; ++n) {
            do {
                random.x = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
                random.x = random.x + random.x;
                random.x = random.x - 1.0f;
                random.y = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
                random.y = random.y + random.y;
                random.y = random.y - 1.0f;
                random.z = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
                random.z = random.z + random.z;
                random.z = random.z - 1.0f;
            } while (1.0f < random.x * random.x + random.y * random.y + random.z * random.z);
            random.x = random.x * 5.0f;
            random.y = random.y * 5.0f;
            random.z = random.z * 5.0f;
            velocity.x = velocity.x + random.x;
            velocity.y = velocity.y + random.y;
            velocity.z = velocity.z + random.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(trailParticleSpark, &mid, &velocity, 0.0f);
        }
        goto check_health;
    }
    if ((fl64 & 0x2000u) != 0) {
        addEffect__FiG6Point3(3, &p);
        gut(&p, 2, 5);
        if (eRand__10GameObject(self) % 3 == 0) gut(&p, 2, 5);
        if (!(self->f560 <= 0.0f)) goto alive;
        if (eRand__10GameObject(self) % 3 == 0) gut(&p, 2, 5);
        goto check_health;
    }
    if ((fl64 & 0x1000u) != 0) {
        addEffect__FiG6Point3(9, &p);
        gut(&p, 2, 3);
        if (eRand__10GameObject(self) % 3 == 0) gut(&p, 2, 3);
        if (!(self->f560 <= 0.0f)) goto alive;
        if (eRand__10GameObject(self) % 3 == 0) gut(&p, 2, 3);
        goto check_health;
    }
    if ((g_BloodLevel > 0 || netNetworkMode != 0) && (fl64 & 0x40000u) != 0) {
        addEffect__FiG6Point3(0, &p);
        gut(&p, 2, 1);
        if (eRand__10GameObject(self) % 3 == 0) {
            gut(&p, 2, 1);
            if (eRand__10GameObject(self) % 3 == 0) gut(&p, 2, 0);
        }
        if (!(self->f560 <= 0.0f)) goto alive;
        if (eRand__10GameObject(self) % 3 == 0) {
            void* chunk = blockAlloc__Fi(616);
            if (chunk != nullptr) {
                const int number = static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u) + 1;
                __8GutChunkG6Point3iif(chunk, &p, number, 0, 1.0f);
            }
        }
    }
    goto check_health;

check_health:
    if (!(self->f560 <= 0.0f)) goto alive;

    // Dead.
    self->flags |= 9u;
    if (1.0f < self->f856) self->f856 = 1.0f;
    if (1.0f < self->f860) self->f860 = 1.0f;
    if (1.0f < self->f864) self->f864 = 1.0f;
    if (1.0f < self->f868) self->f868 = 1.0f;
    if (1.0f < self->f872) self->f872 = 1.0f;
    bits = self->bits;
    if ((bits & 0x20u) != 0 || 1.0f < self->f856) {
        self->flags |= 0x2000000u;
        goto award;
    }
    if ((bits & 0x10u) != 0 && model != nullptr) {
        pos_copy = self->position;
        disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(self, model, self->animation_state, &pos_copy, self->angle,
                                                                                            self->f224, self->f228, 0);
        goto award;
    }
    if ((bits & 0x2000000u) != 0 && model != nullptr) {
        pos_copy = self->position;
        disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(self, model, self->animation_state, &pos_copy, self->angle,
                                                                                            self->f224, self->f228, 1);
        goto award;
    }
    fl64 = self->flags64;
    if ((fl64 & 0x20000000u) != 0) goto award;
    if (0.0f < self->f860 && model != nullptr) {
        if ((eRand__10GameObject(self) & 7) == 0) {
            pos_copy = self->position;
            fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(model, self->animation_state, &pos_copy, self->angle,
                                                                                                self->effect_state, self->radius, self->f224, self->f228);
            self->flags |= 0x4002u;
            self->flags64 |= 0x20000000000ull;
            goto award;
        }
        fl64 = self->flags64;
    }
    if ((self->bits & 0x40800u) != 0x40800u) goto award;
    if ((fl64 & 0x3000u) == 0) {
        if (!(g_BloodLevel > 0 || netNetworkMode != 0)) goto award;
        if ((fl64 & 0x40000u) == 0) goto award;
    }

    {
        // Debris spray.
        Point3 origin = self->position;
        if (g_BloodLevel > 0) {
            const int base = (fl64 & 0x1000u) != 0 ? 7 : ((fl64 & 0x2000u) != 0 ? 4 : 1);
            unsigned state = static_cast<unsigned>(asyncRandState);
            const unsigned first = state * 0x19660du + 0x3c6ef35fu;
            const unsigned second = first * 0x19660du + 0x3c6ef35fu;
            asyncRandState = static_cast<int>(second);
            floorAdd__FG6Point3sfii(&origin, static_cast<short>(static_cast<int>(first) >> 16), 0x1.6p+5f, static_cast<int>((second >> 16) & 1u) + base, 0);
            for (int k = 2; k >= 0; --k) {
                unsigned t = static_cast<unsigned>(asyncRandState);
                Point3 v;
                unsigned third;
                for (;;) {
                    const unsigned r1 = t * 0x19660du + 0x3c6ef35fu;
                    const unsigned r2 = r1 * 0x19660du + 0x3c6ef35fu;
                    third = r2 * 0x19660du + 0x3c6ef35fu;
                    float x = static_cast<float>(static_cast<int>(r1 >> 16)) * 0x1.0p-16f;
                    float y = static_cast<float>(static_cast<int>(r2 >> 16)) * 0x1.0p-16f;
                    float z = static_cast<float>(static_cast<int>(third >> 16)) * 0x1.0p-16f;
                    x = x + x;
                    y = y + y;
                    z = z + z;
                    v.x = x - 1.0f;
                    v.y = y - 1.0f;
                    v.z = z - 1.0f;
                    if (!(1.0f < v.x * v.x + v.y * v.y + v.z * v.z)) break;
                    t = third;
                }
                fl64 = self->flags64;
                asyncRandState = static_cast<int>(third);
                Point3 target;
                target.x = v.x * 10.0f;
                target.y = v.y * 10.0f;
                target.z = v.z * 10.0f;
                const Point3 lift = {0.0f, 0.0f, 36.0f};
                Point3 anchor;
                anchor.x = origin.x + lift.x;
                anchor.y = origin.y;
                anchor.z = origin.z + 36.0f;
                target.x = target.x + anchor.x;
                target.y = target.y + anchor.y;
                target.z = target.z + anchor.z;
                addEffect__FiG6Point3((fl64 & 0x1000u) != 0 ? 9 : ((fl64 & 0x2000u) != 0 ? 3 : 1), &target);
            }
            fl64 = self->flags64;
        }
        for (int i = 0; i < 2; ++i) {
            fl64 = self->flags64;
            const int kind = (fl64 & 0x1000u) != 0 ? 2 : ((fl64 & 0x2000u) != 0 ? 5 : 0);
            void* chunk = blockAlloc__Fi(616);
            if (chunk != nullptr) {
                __8GutChunkG6Point3iif(chunk, &origin, static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u), kind, 1.0f);
                push_chunk(self, chunk, 0x1.3333320p-2f, 80.0f, 20.0f, 16000u, 0xe0c0u);
            }
        }
        for (int i = 3; i >= 0; --i) {
            fl64 = self->flags64;
            const int kind = (fl64 & 0x1000u) != 0 ? 2 : ((fl64 & 0x2000u) != 0 ? 5 : 1);
            void* chunk = blockAlloc__Fi(616);
            if (chunk != nullptr) {
                __8GutChunkG6Point3iif(chunk, &origin, static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u), kind, 1.0f);
                push_chunk(self, chunk, 0x1.3333320p-2f, 50.0f, 10.0f, 40000u, 0xb1e0u);
            }
        }
        self->flags |= 0x4002u;
        self->flags64 |= 0x20000000000ull;
    }

award:
    bits = self->bits;
    if ((bits & 0x40000u) != 0) self->hit_speed = 100.0f;
    else if ((bits & 0x80000u) != 0) self->hit_speed = 150.0f;
    else if ((bits & 0x100000u) != 0) self->hit_speed = 200.0f;
    else if ((bits & 0x200000u) != 0) self->hit_speed = 0x1.388p+8f;
    call_entry(self, 184, 10);
    self->flags |= 0x2000u;
    if (findTagInt__10GameObjectPci(self, dead_tag, 0) != 0) {
        animSetToEndFullyBlended__FP14AnimationState(self->animation_state);
        self->bits = 0;
        return;
    }
    {
        int alive_count = 0;
        if (nmPlayers > 0) {
            for (int i = 0; i < nmPlayers; ++i) alive_count += player[i].byte89;
        }
        const float share = self->f220 / static_cast<float>(alive_count);
        const int amount = static_cast<int>(powf(share, 0x1.666666p+0f));
        int killer = -1;
        if (self->attacker != nullptr) {
            const unsigned key = static_cast<const AttackerKey*>(self->attacker)->key;
            if (player[0].key21872 == key) {
                killer = 0;
            } else if (nmPlayers >= 2) {
                for (int i = 1; i < nmPlayers; ++i) {
                    if (player[i].key21872 == key) killer = i;
                }
            }
        }
        const int rank = playerGetSkillRank__Fii(killer, 59);
        float chance = getChance__12SkillLifeTapi(skills[59], rank);
        const int draw = eRand__10GameObject(self);
        chance = chance * 65536.0f;
        if (static_cast<float>(draw % 65536) < chance) {
            void* ball = blockAlloc__Fi(184);
            if (ball != nullptr) {
                pos_copy = self->position;
                __7OniBallG6Point3iP10GameObjectP10_vagHeader(ball, &pos_copy, self->angle, self, nullptr);
            }
        }
        const int reward = static_cast<int>(static_cast<float>(amount) * 0.75f);
        if (gameCheckPlayerAlive__Fv() >= 2) {
            for (int i = 0; i < nmPlayers; ++i) {
                float weight = 0.5f;
                if (killer != -1) {
                    weight = 0x1.3333320p-1f;
                    if (killer != i) weight = 0x1.999998p-2f;
                }
                PlayerView* view = &player[i];
                if (0.0f < view->float172) {
                    givePlayerExperience__Fii(i, static_cast<int>(static_cast<float>(reward) * weight));
                    if (view->word80 == 0 && killer == i) {
                        const float gain = self->f564 * 0x1.111110p-5f;
                        const float sum = view->float196 + gain;
                        const float cap = view->float200;
                        view->float196 = sum;
                        if (cap < sum) view->float196 = cap;
                    }
                }
            }
        } else {
            for (int i = 0; i < nmPlayers; ++i) {
                PlayerView* view = &player[i];
                if (0.0f < view->float172) {
                    givePlayerExperience__Fii(i, reward);
                    if (view->word80 == 0) {
                        const float gain = self->f564 * 0x1.111110p-5f;
                        const float sum = view->float196 + gain;
                        const float cap = view->float200;
                        view->float196 = sum;
                        if (cap < sum) view->float196 = cap;
                    }
                }
            }
        }
    }
    self->bits = 0;
    return;

alive:
    if (self->state == 10) goto finish_zero;
    bits = self->bits;
    if ((bits & 0x40000u) != 0) self->hit_speed = 100.0f;
    else if ((bits & 0x80000u) != 0) self->hit_speed = 150.0f;
    else if ((bits & 0x100000u) != 0) self->hit_speed = 225.0f;
    else if ((bits & 0x200000u) != 0) self->hit_speed = 375.0f;
    if ((bits & 0x400000u) != 0) {
        self->bits = 0;
        return;
    }
    if ((self->flags64 & 0x400u) != 0) {
        if (eRand__10GameObject(self) % 3 != 0 && self->state != 8) goto call_eight;
    }
    if (eRand__10GameObject(self) % 3 != 0) goto second_check;
    if (self->state != 8) goto call_eight;
    goto second_check;

already_handled:
    if (self->state == 10) {
        self->bits = 0;
        return;
    }
    bits = self->bits;
    if ((bits & 0x40000u) != 0) self->hit_speed = 50.0f;
    else if ((bits & 0x80000u) != 0) self->hit_speed = 75.0f;
    else if ((bits & 0x100000u) != 0) self->hit_speed = 100.0f;
    else if ((bits & 0x200000u) != 0) self->hit_speed = 0x1.388p+7f;
    if (bits != 8192u) {
        self->bits = 0;
        return;
    }
    if (eRand__10GameObject(self) % 3 != 0) goto second_check;
    if (self->state == 8) goto second_check;

call_eight:
    call_entry(self, 184, 8);
    self->bits = 0;
    return;

second_check:
    if ((self->bits & 0x4000000u) != 0 && self->state != 8) call_entry(self, 184, 8);

finish_zero:
    self->bits = 0;
}
