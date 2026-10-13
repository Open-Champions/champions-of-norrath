#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_run__7Cyclops at 0x202a08, 2520 bytes. T is the header table word at object+36, "state" is the signed
// halfword at +572, F is the flag word returned by creatureAdvanceAnimation, and setState(n) is the virtual slot at
// byte offset 184 called with the adjusted object and n. Every "enemyInZone(d, a)" is
// enemyInZone__8Creaturefs(object, d, a).
//  1. newDamageHandler(object, T+12 word); a nonzero everyFrame(object, T+12 word) returns; a zero
//     objectActivationCheck(object, 600) returns.
//  2. F = creatureAdvanceAnimation(object, 1.0). When F has bit 2: modelGetCharAttachmentPos(T+12 word, copy of +16,
//     halfword +56, object+288, index 3, out, null, 1.0) and generateDust(object, out); otherwise, when F has bit 3,
//     the same with index 2.
//  3. slideBack(object, 0.2); the virtual slot at byte offset 168 on the adjusted object. Then, by state:
//     0: enemyInZone(400, 32767) or enemyInZone(500, 16000) -> setState(3); otherwise when the signed word +256 is below
//        the signed word +808 -> setState(1).
//     1: enemyInZone(400, 32767) or enemyInZone(500, 16000) -> setState(0); otherwise smartWander.
//     2: the same two tests -> setState(0); otherwise smartWander, and when that returns zero -> setState(0).
//     3: a zero word +576 -> setState(0). Otherwise, when the signed word +960 is below the signed word +808 and
//        alternateEnemyInZone(object, 0.0, 100.0, halfword(+56) - 32767, 5000) is non-zero -> setState(16). Then
//        enemyInZone(100, 1000): roll R, k = (R mod 6) and 0xffff: 0 -> setState(14), 1 -> setState(15), else
//        setState(6); else enemyInZone(100, 3000) -> setState(13); else enemyInZone(100, 32767) ->
//        turnToFaceEnemy, otherwise steerAndRouteToEnemy.
//     5: enemyInZone(100, 1000) picks 14, 15 or 6 as in state 3 and calls setState, then continues; then
//        bExitStatePersueIdle non-zero -> setState(3).
//     6, 13, 14, 15, 16: a zero word +576 only runs the tail below. Otherwise, only when F has bit 0:
//        14: icos/isin of halfword +56 scale the position by 80 (z + 36) for a 108-byte dustCloud(point, 300, 40, 0, 1)
//            and soundPlaySequence(T+204 word, object, 1.0);
//        15: one poison projectile (168 bytes): the position is object + 10 * (icos, isin) with z + 120; the aim is the
//            heading to the point at (word(+576)+12)+16 turned to 16384 - heading, clamped to halfword(+56) +- 4000, plus a
//            roll mod 2000 minus 1000; velocity 260 * (icos, isin) with z = roll mod 15 - 150; PoisonProjectile(object+36
//            word, 19, 20, 21, 23, position, velocity, halfword(+56) - 16384, roll * 2^-16 * 0.5 + 0.5, object);
//            then soundPlaySequence(T+180 word, object, 1.0);
//        16: a 112-byte poisonGasSpell at object - 80 * (icos, isin), z + 36, (300, 40, 1, object, 5) and
//            soundPlaySequence(T+196 word, object, 1.0);
//        6 and 13: attachment index 4 when bit 4 of the word +244 is set, else 5, generateDust on its position, then
//            produceMeleeDamage(object, 60.0, halfwords +200/+202/+204, 0x100001, null).
//        Tail: unless bit 3 of the halfword at +294 is set -> setState(3).
//     8: unless bit 4 of the halfword at +294 is set -> setState(3).
//     9: when the signed word +804 is below the signed word +808 -> setState(0).
//     10: bit 7 of the halfword at +294 -> objectRemoveFromRunList. When F has bit 0 and bit 1 of word +12 is clear:
//         floorAdd at the position + 100 * (icos, isin) of halfword +56, angle (short of the first roll), size
//         (second roll mod 10) + 40, (0, 1). Then, when F has bit 1 and bit 1 of word +12 is clear, bit 0 of +12 is set.
//     Other states do nothing.
// Unknown: the state names, what F's bits and the words at +256, +804, +576, +808 and +960 mean.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float rate);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short a, void* animation, int index, void* out_a, void* out_b, float scale);
void generateDust__7CyclopsG6Point3(void* self, const void* point);
void slideBack__8Creaturef(void* self, float value);
int enemyInZone__8Creaturefs(void* self, float distance, short angle);
int alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
int smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void* blockAlloc__Fi(int size);
void __9dustCloudG6Point3iiii(void* self, const void* point, int a, int b, int c, int d);
void __14poisonGasSpellG6Point3iiiP10GameObjecti(void* self, const void* point, int a, int b, int c, void* owner, int d);
void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
    void* self, void* map, int a, int b, int c, int d, const void* position, const void* velocity, int angle, float scale, void* owner);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float scale, int a, int b, int c, int d, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* point, short angle, float size, int a, int b);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void* table_word(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void set_state(void* self, int value) {
    typedef void (*SlotCall)(void*, int);
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
    call(adjusted, value);
}
__attribute__((always_inline)) inline void copy_point(unsigned char* to, Address from) {
    for (unsigned i = 0; i < 12u; ++i) to[i] = static_cast<unsigned char>(read_u8(from + i));
}
__attribute__((always_inline)) inline void attachment(void* self, Address object, int index, void* out) {
    unsigned char point[12];
    copy_point(point, object + 16u);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        table_word(object, 12u), point, static_cast<short>(read_i16(object + 56u)), bytes(object + 288u), index, out, nullptr, 1.0f);
}
// The roll-and-pick used by states 3 and 5: 0 -> 14, 1 -> 15, anything else -> 6.
__attribute__((always_inline)) inline void pick_attack(void* self) {
    const unsigned pick = (static_cast<unsigned>(eRand__10GameObject(self)) % 6u) & 0xffffu;
    set_state(self, pick == 0u ? 14 : pick == 1u ? 15 : 6);
}
__attribute__((always_inline)) inline void run_tail(void* self, Address object) {
    if ((read_u16(object + 294u) & 8u) == 0u) set_state(self, 3);
}
__attribute__((always_inline)) inline float f(unsigned bits) { return single_from_bits(bits); }
}

extern "C" void msg_run__7Cyclops(void* self) {
    const Address object = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, table_word(object, 12u));
    if (everyFrame__8CreatureP12_modelHeader(self, table_word(object, 12u)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const unsigned flags = static_cast<unsigned>(creatureAdvanceAnimation__8Creaturef(self, 1.0f));
    if ((flags & 4u) != 0u) {
        unsigned char out[12];
        attachment(self, object, 3, out);
        generateDust__7CyclopsG6Point3(self, out);
    } else if ((flags & 8u) != 0u) {
        unsigned char out[12];
        attachment(self, object, 2, out);
        generateDust__7CyclopsG6Point3(self, out);
    }
    slideBack__8Creaturef(self, f(0x3e4ccccc));
    {
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const int state = read_i16(object + 572u);
    if (static_cast<unsigned>(state) >= 17u) return;
    switch (state) {
    case 0:
        if (enemyInZone__8Creaturefs(self, f(0x43c80000), 32767) != 0) { set_state(self, 3); return; }
        if (enemyInZone__8Creaturefs(self, f(0x43fa0000), 16000) != 0) { set_state(self, 3); return; }
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) set_state(self, 1);
        return;
    case 1:
        if (enemyInZone__8Creaturefs(self, f(0x43c80000), 32767) != 0) { set_state(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, f(0x43fa0000), 16000) != 0) { set_state(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 2:
        if (enemyInZone__8Creaturefs(self, f(0x43c80000), 32767) != 0) { set_state(self, 0); return; }
        if (enemyInZone__8Creaturefs(self, f(0x43fa0000), 16000) != 0) { set_state(self, 0); return; }
        if (smartWander__8Creature(self) != 0) return;
        set_state(self, 0);
        return;
    case 3:
        if (read_word(object + 576u) == 0u) { set_state(self, 0); return; }
        if (signed_word(read_word(object + 960u)) < signed_word(read_word(object + 808u))) {
            if (alternateEnemyInZone__8Creatureffss(self, 0.0f, f(0x42c80000), static_cast<short>(read_u16(object + 56u) - 32767u), 5000) != 0) {
                set_state(self, 16);
                return;
            }
        }
        if (enemyInZone__8Creaturefs(self, f(0x42c80000), 1000) != 0) { pick_attack(self); return; }
        if (enemyInZone__8Creaturefs(self, f(0x42c80000), 3000) != 0) { set_state(self, 13); return; }
        if (enemyInZone__8Creaturefs(self, f(0x42c80000), 32767) != 0) turnToFaceEnemy__8Creature(self);
        else steerAndRouteToEnemy__8Creature(self);
        return;
    case 5:
        if (enemyInZone__8Creaturefs(self, f(0x42c80000), 1000) != 0) pick_attack(self);
        if (bExitStatePersueIdle__8Creature(self) != 0) set_state(self, 3);
        return;
    case 6:
    case 13:
    case 14:
    case 15:
    case 16:
        if (read_word(object + 576u) == 0u) { run_tail(self, object); return; }
        if (state == 14) {
            if ((flags & 1u) != 0u) {
                const int heading = read_i16(object + 56u);
                float point[3];
                point[0] = read_single(object + 16u) + icos__Fi(heading) * f(0x42a00000);
                point[1] = read_single(object + 20u) + isin__Fi(heading) * f(0x42a00000);
                point[2] = read_single(object + 24u) + f(0x42100000);
                void* block = blockAlloc__Fi(108);
                if (block != nullptr) __9dustCloudG6Point3iiii(block, point, 300, 40, 0, 1);
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(table_word(object, 204u), self, 1.0f);
            }
        } else if (state == 15) {
            if ((flags & 1u) != 0u) {
                const Address target = address(pointer(read_word(address(pointer(read_word(object + 576u))) + 12u))) + 16u;
                float aim[3];
                for (unsigned i = 0; i < 12u; ++i) reinterpret_cast<unsigned char*>(aim)[i] = static_cast<unsigned char>(read_u8(target + i));
                const int facing = read_i16(object + 56u);
                float position[3];
                position[0] = read_single(object + 16u) + icos__Fi(facing) * f(0x41200000);
                const float sine = isin__Fi(facing) * f(0x41200000);
                position[1] = read_single(object + 20u) + sine;
                position[2] = read_single(object + 24u) + f(0x42f00000);
                const int heading = iatan2__Fff(aim[0] - read_single(object + 16u), aim[1] - read_single(object + 20u));
                short angle = static_cast<short>(16384 - heading);
                const short difference = static_cast<short>(angle - read_u16(object + 56u));
                if (difference < 4001) {
                    if (difference < -4000) angle = static_cast<short>(read_u16(object + 56u) - 4000u);
                } else {
                    angle = static_cast<short>(read_u16(object + 56u) + 4000u);
                }
                angle = static_cast<short>(angle + static_cast<unsigned>(eRand__10GameObject(self)) % 2000u - 1000u);
                float velocity[3];
                velocity[0] = icos__Fi(angle) * f(0x43820000);
                velocity[1] = isin__Fi(angle) * f(0x43820000);
                velocity[2] = static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) % 15u)) + f(0xc3160000);
                void* block = blockAlloc__Fi(168);
                if (block != nullptr) {
                    const float scale = static_cast<float>(eRand__10GameObject(self)) * f(0x37800000) * f(0x3f000000) + f(0x3f000000);
                    __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
                        block, pointer(read_word(object + 36u)), 19, 20, 21, 23, position, velocity, facing - 16384, scale, self);
                }
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(table_word(object, 180u), self, 1.0f);
            }
        } else if (state == 16) {
            if ((flags & 1u) != 0u) {
                const int facing = read_i16(object + 56u);
                float point[3];
                point[0] = read_single(object + 16u) - icos__Fi(facing) * f(0x42a00000);
                point[1] = read_single(object + 20u) - isin__Fi(facing) * f(0x42a00000);
                point[2] = read_single(object + 24u) + f(0x42100000);
                void* block = blockAlloc__Fi(112);
                if (block != nullptr) __14poisonGasSpellG6Point3iiiP10GameObjecti(block, point, 300, 40, 1, self, 5);
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(table_word(object, 196u), self, 1.0f);
            }
        } else if ((flags & 1u) != 0u) {
            unsigned char out[12];
            attachment(self, object, (read_word(object + 244u) & 0x10u) == 0u ? 5 : 4, out);
            generateDust__7CyclopsG6Point3(self, out);
            produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, f(0x42700000), read_i16(object + 200u), read_i16(object + 202u),
                                                                    read_i16(object + 204u), 0x100001, nullptr);
        }
        run_tail(self, object);
        return;
    case 8:
        if ((read_u16(object + 294u) & 0x10u) == 0u) set_state(self, 3);
        return;
    case 9:
        if (signed_word(read_word(object + 804u)) < signed_word(read_word(object + 808u))) set_state(self, 0);
        return;
    case 10:
        if ((read_u16(object + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        if ((flags & 1u) != 0u && (read_word(object + 12u) & 2u) == 0u) {
            float point[3];
            const int facing = read_i16(object + 56u);
            for (unsigned i = 0; i < 12u; ++i) reinterpret_cast<unsigned char*>(point)[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
            point[0] = point[0] + icos__Fi(facing) * f(0x42c80000);
            point[1] = point[1] + isin__Fi(facing) * f(0x42c80000);
            const int first = eRand__10GameObject(self);
            const unsigned second = static_cast<unsigned>(eRand__10GameObject(self));
            floorAdd__FG6Point3sfii(point, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 40u)), 0, 1);
        }
        if ((flags & 2u) != 0u) {
            const unsigned word = read_word(object + 12u);
            if ((word & 2u) == 0u) write_word(object + 12u, word | 1u);
        }
        return;
    default:
        return;
    }
}
