// Observed msg_run__11MaleDarkElf at 0x00287790, 4232 bytes: the per-frame routine of MaleDarkElf.
// "object" is the argument; offsets are bytes. t is the resource block pointed to by the word at +36
// (its word +12 is a model header). R is the result of creatureAdvanceAnimation (bit 0 and the
// others are tested below). v is the signed byte at +960 (a variant number; unsigned where noted).
// h294 is the halfword at +294. "SS(n)" is virtual slot 184 (table entry at word(+72) + 184: a signed
// halfword adjustment, then a function word at +4) called with the object adjusted and n. The state
// is the halfword at +572. W(x) is the word at +x. Comparisons of W values are signed.
//  1. newDamageHandler(object, t[12]); a nonzero everyFrame(object, t[12]) returns at once; a zero
//     objectActivationCheck(object, 600) returns at once. R = creatureAdvanceAnimation(object, 1.0f);
//     slideBack(object, 0.8f); the table entry at +168 runs with the adjusted object alone.
//  2. A state of 17 or more (the halfword is compared unsigned) returns; otherwise it selects a case.
//     States 2, 4, 11 and 12 do nothing.
// Cases (eiz(r, s) is enemyInZone(object, r, s); A is the float at +28; target is W(+576); crowded(k)
// is objectIsCrowded(object, 400.0f, A * 3.0f, W(W(+576) + 12), k)):
//   0: the single at +788 is saved and set to 1000.0f; when eiz(300, 32767) or eiz(400, 16000) SS(3).
//      If the state is still 0 the single is restored and: bFarFromHome gives SS(1); otherwise, when W(+256)
//      is below W(+808), SS(1). In any other case the single is restored and nothing else happens.
//   1: save and set the single as in case 0; when eiz(300, 32767) or eiz(400, 16000) SS(0), otherwise
//      smartWander; then restore the single.
//   3: no target: SS(0). For v 1..3 (unsigned) or 5: eiz(A + 30, 6000) -> SS(6); else when crowded(0)
//      is false: eiz(A + 30, 32767) true -> turnToFaceEnemy, false -> steerAndRouteToEnemy; crowded:
//      W(+256) = W(+808) + eRand % 240 + 240, then SS(5). For v 4: eiz(A + 30, 6000) -> SS(6); else
//      eiz(A + 30, 32767) -> turnToFaceEnemy; else enemyInZone2(0, 400, h56, 2000, 0, 1) false ->
//      steerAndRouteToEnemy, true -> SS(13) when an eRand bit is set, else SS(14). For v 0:
//      alternateEnemyInZone(0, 80, 0, 32767) -> SS(7); else enemyInZone2 as above true -> SS(15),
//      false -> steerAndRouteToEnemy. Other v: nothing.
//   5: for v 1..3 or 5, eiz(A + 30, 6000) -> SS(6); then for v 4 the same test; then for v 0
//      enemyInZone2(0, 400, h56, 2000, 0, 1) true -> SS(15). Then, when bExitStatePersueIdle is
//      nonzero: W(+256) < W(+808) -> SS(3); else with a target and not crowded(1) -> SS(3).
//   6: move2 with a zero vector; with a target: when R bit 0 is set, melee damage is produced
//      (when W(+208) is 2 and an eRand roll below one half, creatureSlamEffect(object, W(target + 12), 1)
//      and the two damage halfwords +200 and +202 are doubled), produceMeleeDamage(36.0f, h200, h202,
//      h204, 8, description at +1040); then when h294 has bit 3 set turnToFaceEnemy, else for v 2..5
//      (unsigned 2..3, signed 4..5) bit 9 of the word at +160 is set. Without bit 3 in h294 the word
//      at +256 is decremented; in both cases SS(3) when W(+256) is then not above zero.
//   7: W(+808) < W(+256) with a target: flee2(object, W(target + 12)); otherwise SS(0).
//   8: h294 bit 4 clear -> SS(3).   9: W(+804) < W(+808) -> SS(3).   16: W(+256) < W(+808) -> SS(3).
//  10: h294 bit 7 set: remove from the run list. R bit 4 set and bit 1 of W(+12) clear: floorAdd at the
//      point copy of +16 with a random short, a single 20 + eRand % 10, 0 and 1.
//  13, 14, 15: no target: SS(0) and the common tail below. Otherwise a lead position is computed
//      (findEnemyLeadPosition(400.0f, ...) on the point 300 units ahead of the heading h56) and, when R
//      bits 1 or 2 are set, an arrow or missile is released as in the source below; every path ends in
//      the common tail.
// Common tail: when h294 has bit 3 set, the table entry at +176 runs with (adjusted object, 0, the lead
// point); then, if bit 3 of h294 is set, a positive W(+256) returns and anything else is SS(3);
// otherwise, for v 0 alternateEnemyInZone(0, 80, 0, 32767) true -> SS(7); then W(+256) is
// decremented and a positive value returns, anything else is SS(3).
// Inferred: arrows of five kinds (cold, explode, fire, poison, multi) are selected by W(+208) minus
// one, from the skills[] entries 44, 47, 43, 45 and 46; anything else is a MissileWeapon built from
// the global item table or the resource block (words +436, +444). Unknown: what the states mean.
// Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short heading, short c, float d, int e);
int alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
int bFarFromHome__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void flee2__8CreatureP10GameObject(void* self, void* other);
void move2__8CreatureG6Point3(void* self, const void* delta);
void creatureSlamEffect__8CreatureP8Creaturei(void* self, void* other, int flag);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, void* target);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* header, const void* point, short heading, void* animation, int index, void* position, void* other, float scale);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* position, short angle, float size, int a, int b);
void cold_arrow_construct(void* self, const void* position, short heading, void* owner, int difficulty, void* skill)
    __asm__("__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill");
void explode_arrow_construct(void* self, const void* position, short heading, void* owner, int difficulty, void* skill)
    __asm__("__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill");
void fire_arrow_construct(void* self, const void* position, short heading, void* owner, int difficulty, void* skill)
    __asm__("__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill");
void poison_arrow_construct(void* self, const void* position, short heading, void* owner, int difficulty, void* skill)
    __asm__("__21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill");
void multi_fire_construct(void* self, const void* position, short heading, void* owner, int difficulty, void* skill)
    __asm__("__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill");
void missile_weapon_construct(void* self, int zero, void* model, void* texture, const void* position, const void* velocity, int angle,
                              void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
extern float worldDifficulty;
extern unsigned skills[];
extern unsigned char norrath_player_view_31f540[];
}

namespace {
struct Point3 { float x, y, z; };
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
float& real(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
int half(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
unsigned uhalf(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
unsigned char* pointer_at(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned char**>(o + offset); }

unsigned char* entry(unsigned char* o, unsigned table_offset) { return pointer_at(o, 72) + table_offset; }
void set_state(unsigned char* o, int n) {
    unsigned char* e = entry(o, 184);
    reinterpret_cast<void (*)(void*, int)>(word(e, 4))(o + half(e, 0), n);
}
// The target's list node holds an object pointer at +12.
void* target_object(unsigned char* o) { return pointer_at(pointer_at(o, 576), 12); }
bool crowded(unsigned char* o, int k) {
    return objectIsCrowded__8CreatureP10GameObjectffi(o, target_object(o), bits(0x43c80000), real(o, 28) * bits(0x40400000), k) != 0;
}
bool in_zone(unsigned char* o, float range, short angle) { return enemyInZone__8Creaturefs(o, range, angle) != 0; }
bool in_zone2(unsigned char* o) {
    return enemyInZone2__8Creatureffssfi(o, 0.0f, bits(0x43c80000), static_cast<short>(half(o, 56)), 2000, 0.0f, 1) != 0;
}

// The projectile start: the creature's position raised by half of the single at +32 plus 12.
Point3 launch_point(unsigned char* o) {
    const float lift = real(o, 32) * bits(0x3f000000) + bits(0x41400000);
    const float offset[3] = {0.0f, 0.0f, lift};
    Point3 p;
    p.x = real(o, 16) + offset[0];
    p.y = real(o, 20);
    p.z = real(o, 24) + offset[2];
    return p;
}

void release_arrow(unsigned char* o, short heading) {
    const unsigned kind = word(o, 208) - 1u;
    if (kind >= 5u) return;
    if (kind == 4u) {
        unsigned char* skill = reinterpret_cast<unsigned char*>(skills[46]);
        unsigned char* e = pointer_at(skill, 96) + 96;
        const int count = reinterpret_cast<int (*)(void*, int, int)>(word(e, 4))(skill + half(e, 0), 2, static_cast<int>(worldDifficulty));
        short aim = static_cast<short>(heading - static_cast<int>(static_cast<float>(count) * bits(0x3f000000) * bits(0x43fa0000)));
        for (int i = 0; i < count; ++i) {
            void* made = blockAlloc__Fi(120);
            if (made != nullptr) {
                const Point3 p = launch_point(o);
                multi_fire_construct(made, &p, aim, o, static_cast<int>(worldDifficulty), reinterpret_cast<void*>(skills[46]));
            }
            aim = static_cast<short>(aim + 1000);
        }
        return;
    }
    static const int sizes[4] = {176, 116, 176, 176};
    static const int skill_index[4] = {44, 47, 43, 45};
    void* made = blockAlloc__Fi(sizes[kind]);
    if (made == nullptr) return;
    const Point3 p = launch_point(o);
    void* skill = reinterpret_cast<void*>(skills[skill_index[kind]]);
    const int difficulty = static_cast<int>(worldDifficulty);
    switch (kind) {
    case 0: cold_arrow_construct(made, &p, heading, o, difficulty, skill); break;
    case 1: explode_arrow_construct(made, &p, heading, o, difficulty, skill); break;
    case 2: fire_arrow_construct(made, &p, heading, o, difficulty, skill); break;
    default: poison_arrow_construct(made, &p, heading, o, difficulty, skill); break;
    }
}

// The tail shared by the ranged states and the state 13..15 target-less exit.
void tail_after_bit(unsigned char* o) {
    if ((uhalf(o, 294) & 8u) != 0u) {
        if (sword(o, 256) > 0) return;
        set_state(o, 3);
        return;
    }
    if (static_cast<signed char>(o[960]) == 0) {
        if (alternateEnemyInZone__8Creatureffss(o, 0.0f, bits(0x42a00000), 0, 32767) != 0) set_state(o, 7);
    }
    word(o, 256) = word(o, 256) - 1u;
    if (sword(o, 256) > 0) return;
    set_state(o, 3);
}
void tail_ranged(unsigned char* o, Point3* target) {
    if ((uhalf(o, 294) & 8u) != 0u) {
        unsigned char* e = entry(o, 176);
        reinterpret_cast<void (*)(void*, int, void*)>(word(e, 4))(o + half(e, 0), 0, target);
    }
    tail_after_bit(o);
}
}

extern "C" void msg_run__11MaleDarkElf(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, pointer_at(pointer_at(o, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, pointer_at(pointer_at(o, 36), 12)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const unsigned R = creatureAdvanceAnimation__8Creaturef(self, bits(0x3f800000));
    slideBack__8Creaturef(self, bits(0x3f4ccccc));
    {
        unsigned char* e = entry(o, 168);
        reinterpret_cast<void (*)(void*)>(word(e, 4))(o + half(e, 0));
    }
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 17u) return;
    switch (state) {
    case 0:
    case 1: {
        const float saved = real(o, 788);
        real(o, 788) = bits(0x447a0000);
        if (state == 0) {
            if (in_zone(o, bits(0x43960000), 32767)) set_state(o, 3);
            else if (in_zone(o, bits(0x43c80000), 16000)) set_state(o, 3);
            real(o, 788) = saved;
            if (half(o, 572) != 0) return;
            if (bFarFromHome__8Creature(self) != 0) {
                set_state(o, 1);
                return;
            }
            if (half(o, 572) != 0) return;
            if (sword(o, 256) < sword(o, 808)) set_state(o, 1);
            return;
        }
        if (in_zone(o, bits(0x43960000), 32767)) set_state(o, 0);
        else if (in_zone(o, bits(0x43c80000), 16000)) set_state(o, 0);
        else smartWander__8Creature(self);
        real(o, 788) = saved;
        return;
    }
    case 7:
        if (sword(o, 808) < sword(o, 256) && word(o, 576) != 0u) {
            flee2__8CreatureP10GameObject(self, target_object(o));
            return;
        }
        set_state(o, 0);
        return;
    case 3: {
        if (word(o, 576) == 0u) {
            set_state(o, 0);
            return;
        }
        const unsigned char raw = o[960];
        const signed char v = static_cast<signed char>(raw);
        const float thirty = bits(0x41f00000);
        if (static_cast<unsigned char>(raw - 1) < 3u || v == 5) {
            if (in_zone(o, real(o, 28) + thirty, 6000)) {
                set_state(o, 6);
                return;
            }
            if (!crowded(o, 0)) {
                if (in_zone(o, real(o, 28) + thirty, 32767)) turnToFaceEnemy__8Creature(self);
                else steerAndRouteToEnemy__8Creature(self);
                return;
            }
            word(o, 256) = word(o, 808) + eRand__10GameObject(self) % 240u + 240u;
            set_state(o, 5);
            return;
        }
        if (v == 4) {
            if (in_zone(o, real(o, 28) + thirty, 6000)) {
                set_state(o, 6);
                return;
            }
            if (in_zone(o, real(o, 28) + thirty, 32767)) {
                turnToFaceEnemy__8Creature(self);
                return;
            }
            if (!in_zone2(o)) {
                steerAndRouteToEnemy__8Creature(self);
                return;
            }
            if (((eRand__10GameObject(self) & 1u) & 0xffffu) != 0u) set_state(o, 13);
            else set_state(o, 14);
            return;
        }
        if (v != 0) return;
        if (alternateEnemyInZone__8Creatureffss(self, 0.0f, bits(0x42a00000), 0, 32767) != 0) {
            set_state(o, 7);
            return;
        }
        if (in_zone2(o)) {
            set_state(o, 15);
            return;
        }
        steerAndRouteToEnemy__8Creature(self);
        return;
    }
    case 5: {
        const unsigned char raw = o[960];
        const signed char v = static_cast<signed char>(raw);
        if (static_cast<unsigned char>(raw - 1) < 3u || v == 5 || v == 4) {
            if (in_zone(o, real(o, 28) + bits(0x41f00000), 6000)) {
                set_state(o, 6);
                return;
            }
        }
        if (o[960] == 0) {
            if (in_zone2(o)) {
                set_state(o, 15);
                return;
            }
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        if (sword(o, 256) < sword(o, 808)) {
            set_state(o, 3);
            return;
        }
        if (word(o, 576) == 0u) return;
        if (crowded(o, 1)) return;
        set_state(o, 3);
        return;
    }
    case 6: {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
        unsigned half294 = 0;
        if (word(o, 576) != 0u) {
            if ((R & 1u) != 0u) {
                int low = half(o, 200);
                int high = half(o, 202);
                if (word(o, 208) == 2u) {
                    const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000);
                    if (roll < bits(0x3f000000)) {
                        creatureSlamEffect__8CreatureP8Creaturei(self, target_object(o), 1);
                        low <<= 1;
                        high <<= 1;
                    }
                }
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, bits(0x42100000), low, high, half(o, 204), 8, o + 1040);
            }
            half294 = uhalf(o, 294);
            if ((half294 & 8u) != 0u) {
                turnToFaceEnemy__8Creature(self);
                half294 = uhalf(o, 294);
            } else {
                const unsigned char raw = o[960];
                const signed char v = static_cast<signed char>(raw);
                if (static_cast<unsigned char>(raw - 2) < 2u || v == 5 || v == 4) {
                    *reinterpret_cast<unsigned long long*>(o + 160) |= 0x200ull;
                }
            }
        } else {
            half294 = uhalf(o, 294);
        }
        if ((half294 & 8u) == 0u) word(o, 256) = word(o, 256) - 1u;
        if (sword(o, 256) > 0) return;
        set_state(o, 3);
        return;
    }
    case 8:
        if ((uhalf(o, 294) & 0x10u) == 0u) set_state(o, 3);
        return;
    case 9:
        if (sword(o, 804) < sword(o, 808)) set_state(o, 3);
        return;
    case 16:
        if (sword(o, 256) < sword(o, 808)) set_state(o, 3);
        return;
    case 10:
        if ((uhalf(o, 294) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        if ((R & 0x10u) == 0u) return;
        if ((word(o, 12) & 2u) != 0u) return;
        {
            const Point3 position = *reinterpret_cast<const Point3*>(o + 16);
            const short angle = static_cast<short>(eRand__10GameObject(self));
            const unsigned second = eRand__10GameObject(self);
            floorAdd__FG6Point3sfii(&position, angle, static_cast<float>(static_cast<int>(second % 10u + 20u)), 0, 1);
        }
        return;
    case 13:
    case 14:
    case 15: {
        if (word(o, 576) == 0u) {
            set_state(o, 0);
            tail_after_bit(o);
            return;
        }
        const float c = icos__Fi(half(o, 56));
        const float s = isin__Fi(half(o, 56));
        const float ahead_x = c * bits(0x43960000);
        const float ahead_y = s * bits(0x43960000);
        Point3 target;
        target.x = real(o, 16) + ahead_x;
        target.y = real(o, 20) + ahead_y;
        target.z = real(o, 24);
        findEnemyLeadPosition__8CreaturefP6Point3(self, bits(0x43c80000), &target);
        const signed char v = static_cast<signed char>(o[960]);
        if ((R & 1u) != 0u) {
            if (v == 0) {
                word(o, 968) = 66u;
            } else {
                word(o, 964) = 54u;
                word(o, 968) = 56u;
            }
        }
        if ((R & 6u) == 0u) {
            tail_ranged(o, &target);
            return;
        }
        Point3 origin;
        origin.x = real(o, 16);
        origin.y = real(o, 20);
        origin.z = real(o, 24) + bits(0x42700000);
        if (v != 0) {
            const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
            Point3 other;
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                pointer_at(pointer_at(o, 36), 12), &here, static_cast<short>(uhalf(o, 56) - 16384), o + 288, static_cast<int>((R >> 1) & 1u),
                &origin, &other, 1.0f);
        }
        const int toward = iatan2__Fff(target.x - origin.x, target.y - origin.y);
        short aim = static_cast<short>(16384 - toward);
        const int heading = half(o, 56);
        const short delta = static_cast<short>(aim - uhalf(o, 56));
        if (delta >= 1001) aim = static_cast<short>(uhalf(o, 56) + 1000);
        else if (delta < -1000) aim = static_cast<short>(uhalf(o, 56) - 1000);
        (void)heading;
        const float four_hundred = bits(0x43c80000);
        aim = static_cast<short>(aim + static_cast<int>(eRand__10GameObject(self) % 1800u) - 900);
        const float shot_x = icos__Fi(aim) * four_hundred;
        const float shot_y = isin__Fi(aim) * four_hundred;
        Point3 velocity;
        velocity.x = shot_x;
        velocity.y = shot_y;
        velocity.z = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 60u) - 30);
        char name[48];
        sprintf(name, "swsh_arrow%d.anm", static_cast<int>(eRand__10GameObject(self) & 3u) + 1);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, 1.0f);
        if (v == 0) {
            word(o, 968) = 0u;
            const unsigned kind = word(o, 208) - 1u;
            if (kind < 5u) {
                release_arrow(o, aim);
            } else {
                void* made = blockAlloc__Fi(452);
                if (made != nullptr) {
                    unsigned char* item = const_cast<unsigned char*>(norrath_player_view_31f540);
                    const int b = half(o, 202) / 2;
                    missile_weapon_construct(made, 0, reinterpret_cast<void*>(word(item, 8)), reinterpret_cast<void*>(word(item, 12)), &origin,
                                             &velocity, aim - 16384, self, nullptr, nullptr, half(o, 200), b, 0, 0, half(o, 204), 4, nullptr);
                }
            }
        } else {
            if ((R & 1u) != 0u) word(o, 968) = 0u;
            if ((R & 2u) != 0u) word(o, 964) = 0u;
            unsigned char* made = static_cast<unsigned char*>(blockAlloc__Fi(452));
            if (made != nullptr) {
                unsigned char* block = pointer_at(o, 36);
                const int b = half(o, 202) / 2;
                missile_weapon_construct(made, 0, reinterpret_cast<void*>(word(block, 436)), reinterpret_cast<void*>(word(block, 444)), &origin,
                                         &velocity, aim - 16384, self, nullptr, nullptr, half(o, 200), b, 0, 0, half(o, 204), 4, nullptr);
                made[254] = 2;
                *reinterpret_cast<unsigned short*>(made + 250) = 3000;
            }
        }
        tail_ranged(o, &target);
        return;
    }
    default:
        return;
    }
}
