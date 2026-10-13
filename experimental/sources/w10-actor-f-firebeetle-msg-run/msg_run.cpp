#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__10FireBeetle at 0x00210620, 2668 bytes. Notation: h56 is the signed halfword at +56, pos the
// floats at +16, +20, +24, size the float at +28, W the word at +576 (a list head), h294 the unsigned halfword at
// +294, slot(n) the virtual call at table byte n on this object (adjusted by the slot's signed halfword),
// st(n) = slot(184) with the integer n (setState in the vtables seen), zone(d, n) = enemyInZone(object, d, n),
// clock the word at +808 as read right after step 2 (re-read inside step 3 when it runs), and the signed
// words A = +968 (a toggle), B = +972 and C = +964, D = +256, E = +804, F = +960.
//  1. newDamageHandler and everyFrame with the model at group+12 (group = word at +36, re-read each time); a
//     non-zero everyFrame result returns. objectActivationCheck(object, 600); zero returns.
//  2. flags = creatureAdvanceAnimation(1.0); slideBack(0.4); slot(168).
//  3. When B < clock: with A zero a wander target is built: heading k = h56 plus or minus 8192 (minus when an
//     eRand roll has its low bit set, wrapped to 16 bits), W' = (pos.x + icos(k) * 100, pos.y + isin(k) * 100,
//     pos.z + 0 * 100) is copied to +976..987, and the due time is clock + 10 (clock re-read); with A non-zero the
//     due time is clock + 15. B takes the due time and A is flipped (xor 1).
//  4. The signed halfword state at +572 selects (15 and above, and 2, 4, 7, 11, 12: nothing). All paths end here
//     without further stores; an early return is a return.
//   0: zone(400, 32767) or zone(400, 18000): st(3). Else when D < clock: st(1).
//   1: zone(400, 32767) or zone(400, 18000): st(3). Else smartWander.
//   3: W zero: st(0). zone(size + 20, 4000): st(6). When C < clock and not zone(100, 32767) and zone(400, 3000):
//      st(14), the word at +248 becomes frameNm, a 188-byte block is allocated and, if non-null, LightEffect is
//      built on it (see the code), then return. Otherwise zone(200, 32767) false and zone(400, 8000) true: st(13).
//      Then zone(size + 20, 32767): turnToFaceEnemy; else A zero: steerAndRouteToEnemy; else
//      steerAndRouteToPosition with a copy of the 12 bytes at +976.
//   5: zone(size + 20, 4000): st(6). When C < clock and not zone(100, 32767) and zone(400, 3000): the same st(14),
//      +248 stamp and LightEffect as in 3. Otherwise bExitStatePersueIdle non-zero: st(3).
//   6: W non-zero: with flags bit 0, produceMeleeDamage(50.0, the signed halfwords at +200, +202, +204, 8, the
//      description at +992); when h294 bit 3 is set then turnToFaceEnemy. Common end: when h294 bit 3 is clear D is
//      decremented; D greater than zero returns, otherwise st(3). (With W zero only the common end runs.)
//   8: st(3) unless h294 bit 3 is set.
//   9: st(0) when E < clock.
//  10: h294 bit 7 (0x80) set: the word at +12 gains 1 and objectAddToSlowRunList. With flags bit 1 set and bit 1 of
//      the word at +12 clear: two eRand rolls r1, r2: floorAdd(copy of pos, (short)r1, (float)(r2 % 20 + 35), 6, 1).
//  13: W zero: st(1). Not clock < F: st(3). Else steerAndRouteToEnemy.
//  14: W non-zero with flags bit 0: T = (pos.x + icos(h56) * 300, pos.y + isin(h56) * 300, pos.z);
//      findEnemyLeadPosition(300, &T); start = (pos.x + icos(h56) * 10, pos.y + isin(h56) * 10, pos.z + 5);
//      k = (short)(16384 - iatan2(T.x - pos.x, T.y - pos.y)) clamped to within 4000 of h56 and moved by
//      eRand % 1000 - 500; velocity = (icos(k) * 300, isin(k) * 300, ((float)(eRand % 50) + 200) * distance *
//      0.003333), distance being the length of pos minus T; a 460-byte block is allocated and, if non-null,
//      beetleProjectile is built with (start, velocity, h56 - 16384, this, the halfwords at +200, +202, 0, 0, +204,
//      the animation header word at group+140). soundPlaySequence(the header word at group+132, object, 1.0)
//      follows whether or not the allocation succeeded. Then the same common end as in 6 (with turnToFaceEnemy
//      when h294 bit 3 is set; with W zero only the common end).
// Unknown: what the states, the toggle, the due times, D, E, F, the +248 stamp and the LightEffect numbers mean.
extern "C" {
extern int frameNm;
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float first, float second);
int enemyInZone__8Creaturefs(void* self, float distance, short count);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const void* point);
void turnToFaceEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float scale, int a, int b, int c, int d, void* description);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float distance, void* point);
void* blockAlloc__Fi(int size);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
void __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader(
    void* block, const void* start, const void* velocity, int heading, void* creature, int a, int b, int c, int d, int e,
    void* header);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectAddToSlowRunList__FP10GameObject(void* object);
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

bool zone(void* self, unsigned distance_bits, short count) {
    return enemyInZone__8Creaturefs(self, constant(distance_bits), count) != 0;
}

short short_of(unsigned halfword_sum) { return static_cast<short>(halfword_sum & 0xffffu); }

// The flash effect built after st(14) in states 3 and 5.
void spawn_light(void* self, Address object) {
    write_word(object + 248u, static_cast<unsigned>(frameNm));
    void* block = blockAlloc__Fi(188);
    if (block == nullptr) return;
    const float zero = constant(0u);
    float point[3];
    point[0] = read_single(object + 16u) + zero;
    point[1] = read_single(object + 20u);
    point[2] = read_single(object + 24u) + constant(0x42100000u);
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(
        block, point, constant(0x437f0000u), constant(0x431b0000u), zero, constant(0xc2c00000u), zero, zero, zero, zero,
        constant(0x43340000u), constant(0x43100000u), zero, constant(0x3fc00000u), self, 0);
}

// The shared end of states 6 and 14: D is decremented when bit 3 of h294 is clear.
void decrement_end(void* self, Address object, bool test_bit_first) {
    if (test_bit_first && (read_u16(object + 294u) & 8u) != 0u) {
        if (signed_word(read_word(object + 256u)) > 0) return;
        set_state(self, 3);
        return;
    }
    write_word(object + 256u, read_word(object + 256u) - 1u);
    if (signed_word(read_word(object + 256u)) > 0) return;
    set_state(self, 3);
}
}  // namespace

extern "C" void msg_run__10FireBeetle(void* self) {
    const Address object = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, group_model(object));
    if (everyFrame__8CreatureP12_modelHeader(self, group_model(object)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int flags = creatureAdvanceAnimation__8Creaturef(self, constant(0x3f800000u));
    slideBack__8Creaturef(self, constant(0x3ecccccc));
    slot_no_args(self, 168u);
    int clock = signed_word(read_word(object + 808u));
    if (signed_word(read_word(object + 972u)) < clock) {
        unsigned toggle = read_word(object + 968u);
        unsigned due = static_cast<unsigned>(clock) + 15u;
        if (toggle == 0u) {
            const int heading = read_i16(object + 56u);
            const unsigned roll = eRand__10GameObject(self);
            const short turned = static_cast<short>((roll & 1u) != 0u ? heading - 8192 : heading + 8192);
            const float cosine = icos__Fi(turned);
            const float sine = isin__Fi(turned);
            const float hundred = constant(0x42c80000u);
            const float zero = constant(0u);
            float target[3];
            target[0] = read_single(object + 16u) + cosine * hundred;
            target[1] = read_single(object + 20u) + sine * hundred;
            target[2] = read_single(object + 24u) + zero * hundred;
            for (unsigned i = 0; i < 12u; ++i) bytes(object + 976u)[i] = reinterpret_cast<unsigned char*>(target)[i];
            clock = signed_word(read_word(object + 808u));
            toggle = read_word(object + 968u);
            due = static_cast<unsigned>(clock) + 10u;
        }
        write_word(object + 972u, due);
        write_word(object + 968u, toggle ^ 1u);
    }
    const unsigned state = static_cast<unsigned>(static_cast<int>(read_i16(object + 572u)));
    switch (state) {
    case 0u:
        if (zone(self, 0x43c80000u, 32767) || zone(self, 0x43c80000u, 18000)) { set_state(self, 3); return; }
        if (!(signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u)))) return;
        set_state(self, 1);
        return;
    case 1u:
        if (zone(self, 0x43c80000u, 32767) || zone(self, 0x43c80000u, 18000)) { set_state(self, 3); return; }
        smartWander__8Creature(self);
        return;
    case 3u:
    case 5u: {
        if (state == 3u && read_word(object + 576u) == 0u) { set_state(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, read_single(object + 28u) + constant(0x41a00000u), 4000) != 0) {
            set_state(self, 6);
            return;
        }
        if (signed_word(read_word(object + 964u)) < signed_word(read_word(object + 808u)) &&
            !zone(self, 0x42c80000u, 32767) && zone(self, 0x43c80000u, 3000)) {
            set_state(self, 14);
            spawn_light(self, object);
            return;
        }
        if (state == 5u) {
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(self, 3);
            return;
        }
        if (!zone(self, 0x43480000u, 32767) && zone(self, 0x43c80000u, 8000)) { set_state(self, 13); return; }
        if (enemyInZone__8Creaturefs(self, read_single(object + 28u) + constant(0x41a00000u), 32767) != 0) {
            turnToFaceEnemy__8Creature(self);
            return;
        }
        if (read_word(object + 968u) == 0u) {
            steerAndRouteToEnemy__8Creature(self);
        } else {
            unsigned char copy[12];
            for (unsigned i = 0; i < 12u; ++i) copy[i] = bytes(object + 976u)[i];
            steerAndRouteToPosition__8CreatureG6Point3(self, copy);
        }
        return;
    }
    case 6u:
        if (read_word(object + 576u) == 0u) { decrement_end(self, object, true); return; }
        if ((flags & 1) != 0) {
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, constant(0x42480000u), read_i16(object + 200u),
                                                                    read_i16(object + 202u), read_i16(object + 204u), 8,
                                                                    bytes(object + 992u));
        }
        if ((read_u16(object + 294u) & 8u) == 0u) { decrement_end(self, object, false); return; }
        turnToFaceEnemy__8Creature(self);
        decrement_end(self, object, true);
        return;
    case 8u:
        if ((read_u16(object + 294u) & 8u) == 0u) set_state(self, 3);
        return;
    case 9u:
        if (signed_word(read_word(object + 804u)) < clock) set_state(self, 0);
        return;
    case 10u: {
        if ((read_u16(object + 294u) & 0x80u) != 0u) {
            write_word(object + 12u, read_word(object + 12u) | 1u);
            objectAddToSlowRunList__FP10GameObject(self);
        }
        if ((flags & 2) == 0 || (read_word(object + 12u) & 2u) != 0u) return;
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        const unsigned first = eRand__10GameObject(self);
        const unsigned second = eRand__10GameObject(self);
        floorAdd__FG6Point3sfii(point, static_cast<short>(first & 0xffffu),
                                static_cast<float>(static_cast<int>(second % 20u + 35u)), 6, 1);
        return;
    }
    case 13u:
        if (read_word(object + 576u) == 0u) { set_state(self, 1); return; }
        if (!(clock < signed_word(read_word(object + 960u)))) { set_state(self, 3); return; }
        steerAndRouteToEnemy__8Creature(self);
        return;
    case 14u: {
        if (read_word(object + 576u) == 0u) { decrement_end(self, object, true); return; }
        if ((flags & 1) != 0) {
            float lead[3];
            const float cosine = icos__Fi(read_i16(object + 56u));
            const float sine = isin__Fi(read_i16(object + 56u));
            const float three_hundred = constant(0x43960000u);
            lead[0] = read_single(object + 16u) + cosine * three_hundred;
            lead[1] = read_single(object + 20u) + sine * three_hundred;
            lead[2] = read_single(object + 24u);
            findEnemyLeadPosition__8CreaturefP6Point3(self, three_hundred, lead);
            const float ten = constant(0x41200000u);
            float start[3];
            const float cosine_start = icos__Fi(read_i16(object + 56u));
            start[0] = read_single(object + 16u) + cosine_start * ten;
            const float sine_start = isin__Fi(read_i16(object + 56u));
            start[1] = read_single(object + 20u) + sine_start * ten;
            start[2] = read_single(object + 24u) + constant(0x40a00000u);
            const int aim = iatan2__Fff(lead[0] - read_single(object + 16u), lead[1] - read_single(object + 20u));
            short heading = static_cast<short>(16384 - aim);
            const unsigned base = read_u16(object + 56u);
            const short difference = short_of(static_cast<unsigned>(heading) - base);
            if (difference > 4000) heading = short_of(base + 4000u);
            else if (difference < -4000) heading = short_of(base - 4000u);
            const unsigned jitter = eRand__10GameObject(self) % 1000u;
            heading = short_of(static_cast<unsigned>(heading) + jitter - 500u);
            const float cosine_k = icos__Fi(heading);
            const float sine_k = isin__Fi(heading);
            float velocity[3];
            velocity[0] = cosine_k * three_hundred;
            velocity[1] = sine_k * three_hundred;
            velocity[2] = constant(0u);
            const float dx = read_single(object + 16u) - lead[0];
            const float dy = read_single(object + 20u) - lead[1];
            const float dz = read_single(object + 24u) - lead[2];
            float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            distance = distance * constant(0x3b5a740du);
            const unsigned spread = eRand__10GameObject(self) % 50u;
            velocity[2] = (static_cast<float>(static_cast<int>(spread)) + constant(0x43480000u)) * distance;
            void* block = blockAlloc__Fi(460);
            if (block != nullptr) {
                __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader(
                    block, start, velocity, read_i16(object + 56u) - 16384, self, read_i16(object + 200u),
                    read_i16(object + 202u), 0, 0, read_i16(object + 204u),
                    pointer(read_word(address(pointer(read_word(object + 36u))) + 140u)));
            }
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                pointer(read_word(address(pointer(read_word(object + 36u))) + 132u)), self, constant(0x3f800000u));
        }
        if ((read_u16(object + 294u) & 8u) == 0u) { decrement_end(self, object, false); return; }
        turnToFaceEnemy__8Creature(self);
        decrement_end(self, object, true);
        return;
    }
    default:
        return;
    }
}
