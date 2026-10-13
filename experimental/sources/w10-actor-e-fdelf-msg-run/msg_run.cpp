#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_run__13FemaleDarkElf at 0x285830, 2504 bytes. T is the header table word at object+36, "state" is the
// signed halfword at +572, F is the flag word returned by creatureAdvanceAnimation, "kind" is the signed byte at
// +960, and setState(n) is the virtual slot at byte offset 184 called with the adjusted object and n.
// enemyInZone(d, a) is enemyInZone__8Creaturefs(object, d, a); the single at +28 is called S.
//  1. newDamageHandler(object, T+12 word); a nonzero everyFrame(object, T+12 word) returns; a zero
//     objectActivationCheck(object, 600) returns. F = creatureAdvanceAnimation(object, 1.0). slideBack(object, 0.3)
//     when the kind is 1, else slideBack(object, 0.5); then the virtual slot at byte offset 168. States 15 and above
//     return; the jump table at 0x477130 selects. States 2, 4, 11 and 12 do nothing.
//  2. 0: enemyInZone(300, 32767) or enemyInZone(400, 16000) -> setState(3); otherwise when the signed word +256 is
//        below the signed word +808 -> setState(1).
//     1: the same two tests -> setState(0); otherwise smartWander.
//     3: a zero word +576 -> setState(0). For kind 0: enemyInZone(S + 20, 6000) -> setState(6); enemyInZone(S + 20,
//        32767) -> turnToFaceEnemy; otherwise objectIsCrowded(object, word(+576)+12 word, 400, S * 3, 0): when it is
//        non-zero, +256 = word(+808) + R mod 240 + 240 and setState(5) (the store comes first), else
//        steerAndRouteToEnemy. For other kinds: enemyInZone(100, 32767) -> setState(7); enemyInZone2(object, 0, 400,
//        halfword(+56), 2000, 0, 1) non-zero -> setState(13); otherwise steerAndRouteToEnemy.
//     5: for kind 0 with a non-zero word +576, enemyInZone(S + 20, 6000) -> setState(6). For kind 1,
//        enemyInZone2(0, 400, halfword(+56), 2000, 0, 1) non-zero -> setState(13). Then, when bExitStatePersueIdle is
//        non-zero: the signed word +256 below +808 -> setState(3); otherwise, with a non-zero word +576 and
//        objectIsCrowded(object, word(+576)+12 word, 400, S * 3, 1) zero -> setState(3).
//     6: move2(object, zero point). With a non-zero word +576: when F has bit 0, produceMeleeDamage(object, 36.0,
//        halfwords +200/+202/+204, 8, object+1032); then when bit 3 of the halfword at +294 is set, turnToFaceEnemy, else
//        bit 9 of the 64-bit value at +160 is set. Then the common countdown below.
//     7: when the signed word +808 is below +256: with a non-zero word +576, flee2(object, word(+576)+12 word), otherwise
//        setState(3); when it is not, setState(3).
//     8: with s the 64-bit value at +288: setState(3) when (s and 0x0030000000000000) is zero or (s and
//        0x00b0000000000000) equals 0x00a0000000000000.
//     9: the signed word +804 below +808 -> setState(3).   14: the signed word +256 below +808 -> setState(3).
//     10: bit 7 of the halfword at +294 -> objectRemoveFromRunList. With F bit 4 and bit 1 of word +12 clear:
//         floorAdd(copy of +16, short of the first roll, (second roll mod 10) + 20, 0, 1).
//     13: three rolls are taken first (scaled by 2^-16 into a local the routine never reads). With a non-zero word +576:
//         the unit-length heading direction of halfword +56 (icos, isin) scaled by 300 gives a point from the position
//         (z kept); findEnemyLeadPosition(object, 400, point); turnToFaceEnemy; and when F has bit 0, a roll R picks
//         r = R and 1, replaced by 3 when strticmp(levelName + 81, "islcaves") differs and
//         ISeeDeadPeople(object, 300) returns an object. r = 0 sets six halfwords at +140..+154 and spawns a DiseaseBolt
//         (124 bytes) at the position with z + S/2; r = 1 sets another six and spawns a PoisonRain (108 bytes);
//         r = 3 sets six and, with that object, calls its slot at byte offset 184 with 15 and plays the sound found in
//         fx.lmp as "spel_he_heal.anm" on it. Then: when bit 3 of the halfword at +294 is clear and
//         enemyInZone(100, 32767) -> setState(7).
//     Common countdown (6 and 13): when bit 3 of the halfword at +294 is clear, word +256 is decremented; when the
//     result is not positive, setState(3).
// Unknown: the state names, the kinds, what F's bits and the words at +256, +804 and +576 mean, and what the
// halfwords written in state 13 are.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float rate);
void slideBack__8Creaturef(void* self, float value);
int enemyInZone__8Creaturefs(void* self, float distance, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float a, float b, int c);
int smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void flee2__8CreatureP10GameObject(void* self, void* other);
void move2__8CreatureG6Point3(void* self, const void* point);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float scale, int a, int b, int c, int d, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* point, short angle, float size, int a, int b);
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, void* point);
int strticmp__FPCcT0(const char* a, const char* b);
void* ISeeDeadPeople__8Creaturef(void* self, float range);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* other, float range);
void* blockAlloc__Fi(int size);
// The first parameter is the short yaw; the original passes the full result register unmodified, so it is declared int.
void __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(void* self, int a, int b, const void* position, void* owner, void* skill);
void __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(void* self, int a, void* owner, const void* position, void* skill, void* header);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
extern char levelName[];
extern unsigned char skills[];
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline float f(unsigned bits) { return single_from_bits(bits); }
__attribute__((always_inline)) inline void* table_word(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void set_state(void* self, int value) {
    typedef void (*SlotCall)(void*, int);
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
    call(adjusted, value);
}
__attribute__((always_inline)) inline int kind(Address object) { return read_s8(object + 960u); }
__attribute__((always_inline)) inline bool near_enemy(void* self, float distance, int angle) {
    return enemyInZone__8Creaturefs(self, distance, static_cast<short>(angle)) != 0;
}
// The second target position test used by states 3 and 5.
__attribute__((always_inline)) inline bool zone2(void* self, Address object) {
    return enemyInZone2__8Creatureffssfi(self, 0.0f, f(0x43c80000), static_cast<short>(read_i16(object + 56u)), 2000, 0.0f, 1) != 0;
}
__attribute__((always_inline)) inline void* target_inner(Address object) {
    return pointer(read_word(address(pointer(read_word(object + 576u))) + 12u));
}
// The countdown shared by states 6 and 13. A positive counter returns; otherwise the state returns to 3.
__attribute__((always_inline)) inline void countdown(void* self, Address object) {
    if ((read_u16(object + 294u) & 8u) == 0u) write_word(object + 256u, read_word(object + 256u) - 1u);
    if (signed_word(read_word(object + 256u)) <= 0) set_state(self, 3);
}
}

extern "C" void msg_run__13FemaleDarkElf(void* self) {
    const Address object = address(self);
    newDamageHandler__8CreatureP12_modelHeader(self, table_word(object, 12u));
    if (everyFrame__8CreatureP12_modelHeader(self, table_word(object, 12u)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    unsigned flags = static_cast<unsigned>(creatureAdvanceAnimation__8Creaturef(self, 1.0f));
    slideBack__8Creaturef(self, kind(object) == 1 ? f(0x3e999999) : f(0x3f000000));
    {
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const int state = read_i16(object + 572u);
    if (static_cast<unsigned>(state) >= 15u) return;
    switch (state) {
    case 0:
        if (near_enemy(self, f(0x43960000), 32767)) { set_state(self, 3); return; }
        if (near_enemy(self, f(0x43c80000), 16000)) { set_state(self, 3); return; }
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) set_state(self, 1);
        return;
    case 1:
        if (near_enemy(self, f(0x43960000), 32767)) { set_state(self, 0); return; }
        if (near_enemy(self, f(0x43c80000), 16000)) { set_state(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 7:
        if (signed_word(read_word(object + 808u)) < signed_word(read_word(object + 256u))) {
            if (read_word(object + 576u) != 0u) flee2__8CreatureP10GameObject(self, target_inner(object));
            else set_state(self, 3);
        } else {
            set_state(self, 3);
        }
        return;
    case 3:
        if (read_word(object + 576u) == 0u) { set_state(self, 0); return; }
        if (kind(object) == 0) {
            if (near_enemy(self, read_single(object + 28u) + f(0x41a00000), 6000)) { set_state(self, 6); return; }
            if (near_enemy(self, read_single(object + 28u) + f(0x41a00000), 32767)) { turnToFaceEnemy__8Creature(self); return; }
            if (objectIsCrowded__8CreatureP10GameObjectffi(self, target_inner(object), f(0x43c80000), read_single(object + 28u) * f(0x40400000), 0) != 0) {
                const unsigned r = static_cast<unsigned>(eRand__10GameObject(self)) % 240u;
                write_word(object + 256u, read_word(object + 808u) + r + 240u);
                set_state(self, 5);
                return;
            }
        } else {
            if (near_enemy(self, f(0x42c80000), 32767)) { set_state(self, 7); return; }
            if (zone2(self, object)) { set_state(self, 13); return; }
        }
        steerAndRouteToEnemy__8Creature(self);
        return;
    case 5:
        if (kind(object) == 0 && read_word(object + 576u) != 0u) {
            if (near_enemy(self, read_single(object + 28u) + f(0x41a00000), 6000)) { set_state(self, 6); return; }
        }
        if (kind(object) == 1) {
            if (zone2(self, object)) { set_state(self, 13); return; }
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) { set_state(self, 3); return; }
        if (read_word(object + 576u) == 0u) return;
        if (objectIsCrowded__8CreatureP10GameObjectffi(self, target_inner(object), f(0x43c80000), read_single(object + 28u) * f(0x40400000), 1) != 0) return;
        set_state(self, 3);
        return;
    case 14:
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) set_state(self, 3);
        return;
    case 6: {
        float zero[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero);
        if (read_word(object + 576u) != 0u) {
            if ((flags & 1u) != 0u)
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, f(0x42100000), read_i16(object + 200u), read_i16(object + 202u),
                                                                        read_i16(object + 204u), 8, bytes(object + 1032u));
            if ((read_u16(object + 294u) & 8u) != 0u) turnToFaceEnemy__8Creature(self);
            else write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
        }
        countdown(self, object);
        return;
    }
    case 8: {
        const unsigned long long s = read_u64(object + 288u);
        if ((s & 0x0030000000000000ull) == 0ull) set_state(self, 3);
        else if ((s & 0x00b0000000000000ull) == 0x00a0000000000000ull) set_state(self, 3);
        return;
    }
    case 9:
        if (signed_word(read_word(object + 804u)) < signed_word(read_word(object + 808u))) set_state(self, 3);
        return;
    case 10:
        if ((read_u16(object + 294u) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
        if ((flags & 0x10u) != 0u && (read_word(object + 12u) & 2u) == 0u) {
            float point[3];
            for (unsigned i = 0; i < 12u; ++i) reinterpret_cast<unsigned char*>(point)[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
            const int first = eRand__10GameObject(self);
            const unsigned second = static_cast<unsigned>(eRand__10GameObject(self));
            floorAdd__FG6Point3sfii(point, static_cast<short>(first), static_cast<float>(static_cast<int>(second % 10u + 20u)), 0, 1);
        }
        return;
    case 13: {
        // Three rolls whose scaled values go to a local that is never read.
        eRand__10GameObject(self);
        eRand__10GameObject(self);
        eRand__10GameObject(self);
        if (read_word(object + 576u) != 0u) {
            const int heading = read_i16(object + 56u);
            const float c = icos__Fi(heading);
            const float s = isin__Fi(heading);
            float point[3];
            point[0] = read_single(object + 16u) + c * f(0x43960000);
            point[1] = read_single(object + 20u) + s * f(0x43960000);
            point[2] = read_single(object + 24u);
            findEnemyLeadPosition__8CreaturefP6Point3(self, f(0x43c80000), point);
            turnToFaceEnemy__8Creature(self);
            if ((flags & 1u) != 0u) {
                void* dead = nullptr;
                int pick = eRand__10GameObject(self) & 1;
                if (strticmp__FPCcT0(levelName + 81, "islcaves") != 0) {
                    dead = ISeeDeadPeople__8Creaturef(self, f(0x43960000));
                    if (dead != nullptr) pick = 3;
                }
                if (pick == 1) {
                    write_u16(object + 146u, 50u);
                    write_u16(object + 150u, 255u);
                    write_u16(object + 154u, 100u);
                    write_u16(object + 140u, 0u);
                    write_u16(object + 142u, 255u);
                    write_u16(object + 144u, 0u);
                    write_u16(object + 148u, 0u);
                    write_u16(object + 152u, 0u);
                    void* block = blockAlloc__Fi(108);
                    if (block != nullptr)
                        __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(
                            block, 1, self, point, pointer(read_word(address(skills) + 96u)), table_word(object, 316u));
                } else if (pick == 0) {
                    write_u16(object + 150u, 255u);
                    write_u16(object + 154u, 100u);
                    write_u16(object + 140u, 255u);
                    write_u16(object + 142u, 255u);
                    write_u16(object + 148u, 255u);
                    write_u16(object + 146u, 50u);
                    write_u16(object + 144u, 0u);
                    write_u16(object + 152u, 0u);
                    const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, target_inner(object), f(0x43af0000));
                    void* block = blockAlloc__Fi(124);
                    if (block != nullptr) {
                        float position[3];
                        position[0] = read_single(object + 16u) + 0.0f;
                        position[1] = read_single(object + 20u);
                        position[2] = read_single(object + 24u) + read_single(object + 32u) * f(0x3f000000);
                        __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(block, yaw, 1, position, self,
                                                                       pointer(read_word(address(skills) + 16u)));
                    }
                } else if (pick == 3 && dead != nullptr) {
                    write_u16(object + 146u, 50u);
                    write_u16(object + 150u, 128u);
                    write_u16(object + 152u, 255u);
                    write_u16(object + 142u, 128u);
                    write_u16(object + 144u, 255u);
                    write_u16(object + 154u, 150u);
                    write_u16(object + 140u, 0u);
                    write_u16(object + 148u, 0u);
                    typedef void (*SlotCall)(void*, int);
                    void* adjusted;
                    SlotCall call = slot_function<SlotCall>(dead, 184u, &adjusted);
                    call(adjusted, 15);
                    char lump[] = "fx.lmp";
                    char name[] = "spel_he_heal.anm";
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, name), dead, 1.0f);
                }
            }
            if ((read_u16(object + 294u) & 8u) == 0u) {
                if (near_enemy(self, f(0x42c80000), 32767)) set_state(self, 7);
            }
        }
        countdown(self, object);
        return;
    }
    default:
        return;
    }
}
