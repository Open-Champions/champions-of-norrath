#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__8Froglock at 0x001ffef8, 3040 bytes. Notation: h56 is the signed halfword at +56, pos the
// floats at +16, +20, +24, W the word at +576 (a list head whose first node's word at +12 is an object), P the
// signed byte at +568, Q the byte at +1105, h294 the unsigned halfword at +294, w256, w804, w808, w960 the words
// at those offsets (w256 and w804 are due times, w808 the clock), slot(n) the virtual call at table byte n on
// this object (adjusted by the slot's signed halfword), st(n) = slot(184) with the integer n (setState in the
// vtables seen), and zone(d, n) = enemyInZone(object, d, n). T below is the 12-byte point named in case 13/14.
//  1. newDamageHandler and everyFrame with the model at group+12 (group = word at +36, re-read each time); a
//     non-zero everyFrame result returns at once. objectActivationCheck(object, 600); zero returns at once.
//  2. flags = creatureAdvanceAnimation(1.0); slideBack(0.5); slot(168); move2(object, a zero point).
//  3. When ((frameNm xor the signed byte at +569) & 7) is zero: worldCheckForWater(world, point, 12.0, null)
//     with point = (pos.x + icos(h56) * 130, pos.y + isin(h56) * 130, pos.z) sets Q to 1 for a non-zero result
//     and to 0 otherwise.
//  4. The signed halfword state at +572 selects (16 and above, and 2, 4, 11, 12: nothing). Every path that does not
//     return early ends with: the byte at +1104 gets P and the byte at +1106 gets Q ("tail").
//   0: zone(400, 32767) or zone(500, 16000) true: st(3). Else when w256 < w808 (signed): st(1).
//   1: zone(400, 32767) or zone(500, 16000) true: st(0). Else smartWander; when bCrossingWater: st(1).
//   3: W zero: st(0). With w960 equal to 16: zone(100, 32767): st(7); else zone(400, 6000): st(14) if P is
//      non-zero else st(13); else steerAndRouteToEnemy and, when bCrossingWater, st(3). Otherwise (w960 not 16):
//      zone(size + 40, 6000) with size the float at +28: st(6); else zone(300, 32767) false and zone(450, 1000)
//      true: st(15); else zone(size + 40, 32767): turnToFaceEnemy; else steerAndRouteToEnemy and, when
//      bCrossingWater, st(3).
//   5: with w960 16: zone(400, 6000): st(13). Then (unless that returned or w960 changed) zone(size + 20, 6000):
//      st(6); then bExitStatePersueIdle non-zero: st(3).
//   6: W zero: st(3) unless bit 3 (8) of h294 is set. Else when flags bit 0: produceMeleeDamage(36.0, the signed
//      halfwords at +200, +202, +204, 8, the description at +1032); then st(3) unless h294 bit 3 is set, in
//      which case turnToFaceEnemy and the same bit test again.
//   7: bCrossingWater: st(3); w808 not below w256: st(3); W zero: st(3); else flee2(object, the object at node+12).
//   8, 15: st(3) unless bit 4 (0x10) of h294 is set.
//   9: st(0) when w804 < w808 (signed).
//  10: bit 7 (0x80) of h294 set: objectRemoveFromRunList. With flags bit 4 set and bit 1 of the word at +12 clear:
//      two eRand rolls r1, r2: floorAdd(copy of pos, (short)r1, (float)(r2 % 10 + 25), 0, 1).
//  13, 14: W zero: st(0), then the common end below. Otherwise: T = (pos.x + icos(h56) * 300, pos.y + isin(h56) *
//      300, pos.z); findEnemyLeadPosition(400, &T). When flags bit 0 is set: aim = iatan2(T.x - pos.x, T.y - pos.y),
//      a heading k = (short)(16384 - aim) clamped to within 1000 of h56 (short arithmetic); k += eRand % 1800 - 900
//      (short); velocity = (icos(k) * 400, isin(k) * 400, (float)(eRand % 60 - 30)); sprintf of "swsh_arrow%d.anm"
//      with eRand & 3 plus one; soundPlaySequence(lumpFindResource("fx.lmp", that name), object, 1.0). The word at
//      +208 selects a projectile: 1 PoisonArrowProjectile (176 bytes, skills[45]), 2 ColdArrowProjectile (176,
//      skills[44]), 3 ExplodeArrowProjectile (116, skills[47]), 4 FireArrowProjectile (176, skills[43]); each is
//      built at (pos.x, pos.y, pos.z + float(+32) * 0.5 + 12) with heading k, this object, the truncated global float
//      worldDifficulty (gp-11588) and the skill pointer. Any other value builds a 452-byte MissileWeapon (item::globalItem
//      words +8 and +12, start (pos.x, pos.y, pos.z + 48), velocity, k - 16384, this, and the halfwords at +200,
//      +202 / 2, +204 with 4). Then, when h294 bit 3 is set: slot(176) with (0, &T).
//      Common end: when h294 bit 3 is still set, w256 > 0 gives the tail and otherwise st(3). Else with w960 16
//      and zone(100, 32767): st(7); then w256 is decremented and w256 > 0 gives the tail, otherwise st(3).
// Unknown: what the states, P, Q and the words at +256, +804 and +960 mean.
extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char skills[];
extern unsigned char globalItem[] asm("_4item$globalItem");
extern float worldDifficulty;
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, const void* point);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float first, float second);
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, const void* point, float depth, float* result);
int enemyInZone__8Creaturefs(void* self, float distance, short count);
void smartWander__8Creature(void* self);
int bCrossingWater__8Froglock(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void flee2__8CreatureP10GameObject(void* self, void* target);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float scale, int a, int b, int c, int d, void* description);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float distance, void* point);
int sprintf(char* buffer, const char* format, ...);
unsigned eRand__10GameObject(void* self);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, const void* point, short heading, void* shooter, int power, void* skill);
void __19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, const void* point, short heading, void* shooter, int power, void* skill);
void __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, const void* point, short heading, void* shooter, int power, void* skill);
void __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, const void* point, short heading, void* shooter, int power, void* skill);
void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int first, void* model, void* texture, const void* start, const void* velocity, int heading, void* creature,
    void* vag_a, void* vag_b, int a, int b, int c, int d, int e, int f, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* point, short angle, float size, int a, int b);
}

namespace {
using namespace vslots_b;
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }

void* group_model(Address object) { return pointer(read_word(address(pointer(read_word(object + 36u))) + 12u)); }

void slot_no_args(void* self, unsigned slot_offset) {
    typedef void (*Call)(void*);
    void* adjusted;
    Call call = slot_function<Call>(self, slot_offset, &adjusted);
    call(adjusted);
}

void set_state(void* self, int state) {
    typedef void (*Call)(void*, int);
    void* adjusted;
    Call call = slot_function<Call>(self, 184u, &adjusted);
    call(adjusted, state);
}

void tail(Address object) {
    const unsigned first = read_u8(object + 568u);
    const unsigned second = read_u8(object + 1105u);
    write_u8(object + 1104u, first);
    write_u8(object + 1106u, second);
}

bool zone(void* self, unsigned distance_bits, short count) {
    return enemyInZone__8Creaturefs(self, constant(distance_bits), count) != 0;
}

short short_of(unsigned halfword_sum) { return static_cast<short>(halfword_sum & 0xffffu); }
}  // namespace

extern "C" void msg_run__8Froglock(void* self) {
    const Address object = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, group_model(object));
    if (everyFrame__8CreatureP12_modelHeader(self, group_model(object)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int flags = creatureAdvanceAnimation__8Creaturef(self, constant(0x3f800000u));
    slideBack__8Creaturef(self, constant(0x3f000000u));
    slot_no_args(self, 168u);
    {
        float nothing[3] = {constant(0u), constant(0u), constant(0u)};
        move2__8CreatureG6Point3(self, nothing);
    }
    {
        const unsigned tag = read_u8(object + 569u);
        if (((frameNm ^ signed_word(tag < 0x80u ? tag : tag | 0xffffff00u)) & 7) == 0) {
            const float scale = constant(0x43020000u);
            const float cosine = icos__Fi(read_i16(object + 56u));
            const float x = read_single(object + 16u) + cosine * scale;
            const float sine = isin__Fi(read_i16(object + 56u));
            const float y = read_single(object + 20u) + sine * scale;
            float point[3] = {x, y, read_single(object + 24u)};
            write_u8(object + 1105u, worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, point, constant(0x41400000u), nullptr) != 0 ? 1u : 0u);
        }
    }
    const unsigned state = static_cast<unsigned>(static_cast<int>(read_i16(object + 572u)));
    float lead[3];
    int entry = 0;  // 0: at the turn test, 1: at the second turn test
    switch (state) {
    case 0u:
        if (zone(self, 0x43c80000u, 32767)) { set_state(self, 3); tail(object); return; }
        if (zone(self, 0x43fa0000u, 16000)) { set_state(self, 3); tail(object); return; }
        if (!(signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u)))) { tail(object); return; }
        set_state(self, 1);
        tail(object);
        return;
    case 1u:
        if (zone(self, 0x43c80000u, 32767)) { set_state(self, 0); tail(object); return; }
        if (zone(self, 0x43fa0000u, 16000)) { set_state(self, 0); tail(object); return; }
        smartWander__8Creature(self);
        if (bCrossingWater__8Froglock(self) != 0) set_state(self, 1);
        tail(object);
        return;
    case 7u:
        if (bCrossingWater__8Froglock(self) != 0) { set_state(self, 3); tail(object); return; }
        if (!(signed_word(read_word(object + 808u)) < signed_word(read_word(object + 256u)))) { set_state(self, 3); tail(object); return; }
        if (read_word(object + 576u) == 0u) { set_state(self, 3); tail(object); return; }
        flee2__8CreatureP10GameObject(self, pointer(read_word(address(pointer(read_word(object + 576u))) + 12u)));
        tail(object);
        return;
    case 3u:
        if (read_word(object + 576u) == 0u) { set_state(self, 0); tail(object); return; }
        if (read_word(object + 960u) == 16u) {
            if (zone(self, 0x42c80000u, 32767)) { set_state(self, 7); tail(object); return; }
            if (zone(self, 0x43c80000u, 6000)) {
                set_state(self, read_u8(object + 568u) != 0u ? 14 : 13);
                tail(object);
                return;
            }
            steerAndRouteToEnemy__8Creature(self);
            if (bCrossingWater__8Froglock(self) != 0) set_state(self, 3);
            tail(object);
            return;
        }
        if (enemyInZone__8Creaturefs(self, read_single(object + 28u) + constant(0x42200000u), 6000) != 0) {
            set_state(self, 6);
            tail(object);
            return;
        }
        if (!zone(self, 0x43960000u, 32767) && zone(self, 0x43e10000u, 1000)) {
            set_state(self, 15);
            tail(object);
            return;
        }
        if (enemyInZone__8Creaturefs(self, read_single(object + 28u) + constant(0x42200000u), 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
            tail(object);
            return;
        }
        steerAndRouteToEnemy__8Creature(self);
        if (bCrossingWater__8Froglock(self) != 0) set_state(self, 3);
        tail(object);
        return;
    case 5u: {
        const unsigned kept = read_word(object + 960u);
        bool skip_zone = false;
        if (kept == 16u) {
            if (zone(self, 0x43c80000u, 6000)) { set_state(self, 13); tail(object); return; }
            skip_zone = read_word(object + 960u) == kept;
        }
        if (!skip_zone &&
            enemyInZone__8Creaturefs(self, read_single(object + 28u) + constant(0x41a00000u), 6000) != 0) {
            set_state(self, 6);
            tail(object);
            return;
        }
        if (bExitStatePersueIdle__8Creature(self) != 0) set_state(self, 3);
        tail(object);
        return;
    }
    case 6u:
        if (read_word(object + 576u) == 0u) {
            if ((read_u16(object + 294u) & 8u) == 0u) set_state(self, 3);
            tail(object);
            return;
        }
        if ((flags & 1) != 0) {
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, constant(0x42100000u), read_i16(object + 200u),
                                                                    read_i16(object + 202u), read_i16(object + 204u), 8,
                                                                    bytes(object + 1032u));
        }
        if ((read_u16(object + 294u) & 8u) == 0u) { set_state(self, 3); tail(object); return; }
        turnToFaceEnemy__8Creature(self);
        if ((read_u16(object + 294u) & 8u) == 0u) set_state(self, 3);
        tail(object);
        return;
    case 8u:
    case 15u:
        if ((read_u16(object + 294u) & 0x10u) == 0u) set_state(self, 3);
        tail(object);
        return;
    case 9u:
        if (signed_word(read_word(object + 804u)) < signed_word(read_word(object + 808u))) set_state(self, 0);
        tail(object);
        return;
    case 10u: {
        if ((read_u16(object + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        if ((flags & 0x10) == 0 || (read_word(object + 12u) & 2u) != 0u) { tail(object); return; }
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        const unsigned first = eRand__10GameObject(self);
        const unsigned second = eRand__10GameObject(self);
        floorAdd__FG6Point3sfii(point, static_cast<short>(first & 0xffffu),
                                static_cast<float>(static_cast<int>(second % 10u + 25u)), 0, 1);
        tail(object);
        return;
    }
    case 13u:
    case 14u: {
        if (read_word(object + 576u) == 0u) {
            set_state(self, 0);
            entry = 1;
        } else {
            const float cosine = icos__Fi(read_i16(object + 56u));
            const float sine = isin__Fi(read_i16(object + 56u));
            const float three_hundred = constant(0x43960000u);
            lead[0] = read_single(object + 16u) + cosine * three_hundred;
            lead[1] = read_single(object + 20u) + sine * three_hundred;
            lead[2] = read_single(object + 24u);
            findEnemyLeadPosition__8CreaturefP6Point3(self, constant(0x43c80000u), lead);
            if ((flags & 1) != 0) {
                float start[3];
                start[0] = read_single(object + 16u);
                start[1] = read_single(object + 20u);
                start[2] = read_single(object + 24u) + constant(0x42400000u);
                const int aim = iatan2__Fff(lead[0] - start[0], lead[1] - start[1]);
                short heading = static_cast<short>(16384 - aim);
                const unsigned base = read_u16(object + 56u);
                const short difference = short_of(static_cast<unsigned>(heading) - base);
                if (difference > 1000) heading = short_of(base + 1000u);
                else if (difference < -1000) heading = short_of(base - 1000u);
                const unsigned jitter = eRand__10GameObject(self) % 1800u;
                heading = short_of(static_cast<unsigned>(heading) + jitter - 900u);
                const float cosine_k = icos__Fi(heading);
                const float sine_k = isin__Fi(heading);
                const float four_hundred = constant(0x43c80000u);
                float velocity[3];
                velocity[0] = cosine_k * four_hundred;
                velocity[1] = sine_k * four_hundred;
                const unsigned spread = eRand__10GameObject(self) % 60u;
                velocity[2] = static_cast<float>(static_cast<int>(spread) - 30);
                char name[48];
                sprintf(name, "swsh_arrow%d.anm", static_cast<int>((eRand__10GameObject(self) & 3u) + 1u));
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                    lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, constant(0x3f800000u));
                const unsigned kind = read_word(object + 208u);
                unsigned projectile_size = 176u;
                unsigned skill_index = 0u;
                switch (kind) {
                case 1u: skill_index = 45u; break;
                case 2u: skill_index = 44u; break;
                case 3u: projectile_size = 116u; skill_index = 47u; break;
                case 4u: skill_index = 43u; break;
                default: break;
                }
                void* block = blockAlloc__Fi(kind - 1u < 4u ? static_cast<int>(projectile_size) : 452);
                if (block != nullptr) {
                    if (kind - 1u < 4u) {
                        float direction[3];
                        direction[0] = constant(0u);
                        direction[1] = constant(0u);
                        direction[2] = read_single(object + 32u) * constant(0x3f000000u) + constant(0x41400000u);
                        const int power = static_cast<int>(worldDifficulty);
                        void* skill = pointer(read_word(address(skills) + 4u * skill_index));
                        float point[3];
                        point[0] = read_single(object + 16u) + direction[0];
                        point[1] = read_single(object + 20u);
                        point[2] = read_single(object + 24u) + direction[2];
                        switch (kind) {
                        case 1u: __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(block, point, heading, self, power, skill); break;
                        case 2u: __19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill(block, point, heading, self, power, skill); break;
                        case 3u: __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(block, point, heading, self, power, skill); break;
                        default: __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill(block, point, heading, self, power, skill); break;
                        }
                    } else {
                        __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
                            block, 0, pointer(read_word(address(globalItem) + 8u)), pointer(read_word(address(globalItem) + 12u)),
                            start, velocity, heading - 16384, self, nullptr, nullptr, read_i16(object + 200u),
                            static_cast<int>(read_i16(object + 202u)) / 2, 0, 0, read_i16(object + 204u), 4, nullptr);
                    }
                }
            }
            entry = 0;
        }
        if (entry == 0 && (read_u16(object + 294u) & 8u) != 0u) {
            typedef void (*TurnCall)(void*, int, void*);
            void* adjusted;
            TurnCall turn = slot_function<TurnCall>(self, 176u, &adjusted);
            turn(adjusted, 0, lead);
            entry = 1;
        }
        if (entry == 1 && (read_u16(object + 294u) & 8u) != 0u) {
            if (signed_word(read_word(object + 256u)) <= 0) set_state(self, 3);
            tail(object);
            return;
        }
        if (read_word(object + 960u) == 16u && zone(self, 0x42c80000u, 32767)) set_state(self, 7);
        write_word(object + 256u, read_word(object + 256u) - 1u);
        if (signed_word(read_word(object + 256u)) <= 0) set_state(self, 3);
        tail(object);
        return;
    }
    default:
        tail(object);
        return;
    }
}
