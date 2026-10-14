#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_run__5Demon at 0x1ea028, 3592 bytes. T is the header table word at object+36, "state" is the signed
// halfword at +572, F is the flag word returned by creatureAdvanceAnimation, setState(n) is the virtual slot at byte
// offset 184 called with the adjusted object and n, and S is the single at +28. enemyInZone(d, a) is
// enemyInZone__8Creaturefs(object, d, a). E is the object whose pointer is the word at (word(+576) + 12).
// A "skill call" reads the word skills[k] (the 252-byte table at 0x34b440), takes the table pointer at +96 of that
// object and calls the virtual function at the given byte offset with the object adjusted by that slot's halfword.
//  1. When the word at +960 is non-zero, fxRunWeapon draws the weapon effect exactly as the FemaleDarkElf draw routine does
//     with its index at +964, here with the index at +960 (model at (T + 8 * word) + 4, description at +1032, state at +968,
//     a copy of +16, halfword +56, T+12 model, animation at +288, 0.5, 1 when the state is 6, 0, 1.0, null matrix).
//  2. newDamageHandler(object, T+12 word); a nonzero everyFrame(object, T+12 word) returns; objectActivationCheck(object, 600)
//     runs and its result is not used; a zero word at +236 returns. F = creatureAdvanceAnimation(object, 1.0).
//     slideBack(object, 0.2); the virtual slot at byte offset 168; then, by state (18 and above, and 2, 4, 7, 11, 12 return):
//     0: enemyInZone(400, 32767) -> setState(3); otherwise when the signed word +256 is below +808 -> setState(1).
//     1: move2(object, zero point); enemyInZone(400, 32767) -> setState(3); the signed word +256 below +808 -> setState(0);
//        otherwise smartWander.
//     3: a zero word +576 -> setState(0). Otherwise E's type (the function at vtable byte 4, a null E calls
//        __throw_bad_typeid first) is compared with __tf4Soul. Soul: enemyInZone(S + 40, 5000) -> setState(13), else
//        enemyInZone(S + 40, 32767) -> turnToFaceEnemy, else steerAndRouteToEnemy. Other types: enemyInZone(S + 40, 5000) ->
//        roll R, setState(14) when R mod 5 is 0 else setState(6); enemyInZone(S + 40, 32767) -> turnToFaceEnemy; with the
//        word +208 equal to 4 and enemyInZone2(object, 0, 400, halfword(+56), 5000, 0, 1) -> setState(15); when
//        objectIsCrowded(object, E, 400, S * 3, 0) is non-zero, +256 = word(+808) + roll mod 240 + 240 and setState(5),
//        else steerAndRouteToEnemy.
//     5: with the word +208 equal to 4 and enemyInZone2(0, 400, halfword(+56), 5000, 0, 1) -> setState(15). enemyInZone(S + 50,
//        5000) -> roll R, setState(14) when R mod 5 is 0 else setState(6). Otherwise, when bExitStatePersueIdle is non-zero:
//        the signed word +256 below +808 -> setState(3); otherwise with a non-zero word +576 and objectIsCrowded(object, E,
//        400, S * 3, 1) zero -> setState(3).
//     6, 16, 17: move2(object, zero point). With a non-zero word +576 and F bit 0: halfwords +200 and +202 are D0 and D1 and flags
//        s3 = 0. With the word +208 equal to 2 and the word +964 below the global frameNm: skills[42]'s slot at byte offset 96 gives a
//        rank (called with 1, 1); +964 = frameNm + 60 * rank; a 28-byte SpellHarmTouch(1, object, skills[42]) is built and passed
//        to addSpell; checkIfSpellIsRunning(object, __tf14SpellHarmTouch) non-zero -> s3 = 0x40000 and the spell's attack(E,
//        &D0, &D1). With the word +208 equal to 3: a roll scaled by 2^-16 below 0.5 -> creatureSlamEffect(object, E, 1) and D0 and
//        D1 doubled. State 17 adds 0x80000 to s3. produceMeleeDamage(object, 36, D0, D1, halfword(+204), s3 | 8, object+1032).
//        Without F bit 0 only turnToFaceEnemy runs (as it does after the melee). Then: bit 3 of the halfword at +294 clear
//        -> setState(3). Otherwise, with F bit 1: a roll with bit 0 clear and enemyInZone(S + 40, 5000) -> setState(16) from 6
//        and setState(17) from 16.
//     8: move2; unless bit 4 of the halfword at +294 is set -> setState(0).
//     9: the signed word +804 below +808 -> setState(3).   14: the signed word +256 below +808 -> setState(3).
//     10: move2; fxRunCreatureFire(T+12 model, object+288, copy of +16, halfword +56, object+824, 1.0, single +224, single +228,
//         the byte at +40 as a single). With a non-zero word +960 and the signed word +256 + 60 below +808: dropWeapon__5Demon, word
//         +12 |= 2 and bit 41 of the 64-bit value at +160 set. Then when (single of word +256) + 75 is below the single of word +808,
//         objectAddToDeleteList.
//     13: move2. A zero word +576, or a non-Soul E, or enemyInZone(S + 40, 5000) false: setState(3) and animAddOneShot(T+132, 0.1,
//         0.1, 32). Otherwise (a Soul in range), with F bit 0, E's slot at byte offset 16 is called with (single of halfword +202,
//         object, 0); turnToFaceEnemy; then setState(3) when the single at E+560 is not above zero.
//     15: with a non-zero word +576: a unit direction of halfword +56 scaled by 300 gives a lead point (findEnemyLeadPosition with
//         400). With F bit 0 a roll picks: bit 0 clear sets the six effect halfwords at +1048..+1062 to 255/128/0/50/255/128/0/100,
//         asks skills[57]'s slot at byte offset 104 for four numbers (called with 0, 1, 2 and 3, 1 and the object) and builds a 184-byte
//         Sparks from them; bit 0 set sets the halfwords to the same values and builds a 168-byte FireStorm(1, object, point + 6 in z,
//         skills[7]). The object's virtual slot at byte offset 176 then runs with (0, lead point). Then, with or without a target: the
//         signed word +256 below +808 -> setState(3); otherwise enemyInZone(S + 40, 5000) -> setState(6).
// Unknown: the state names, what F's bits and the words at +208, +236, +256, +804, +964 and +576 mean.
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
void move2__8CreatureG6Point3(void* self, const void* point);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float scale, int a, int b, int c, int d, void* description);
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, void* point);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* other, float range);
void* blockAlloc__Fi(int size);
void* __tf4Soul();
void* __tf14SpellHarmTouch();
int __eq__C9type_infoRC9type_info(void* left, void* right);
void __throw_bad_typeid();
void __14SpellHarmTouchiP8CreatureP5Skill(void* self, int a, void* owner, void* skill);
void addSpell__8CreatureP5Spell(void* self, void* spell);
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* self, void* type);
void attack__14SpellHarmTouchP10GameObjectPiT2(void* spell, void* target, int* a, int* b);
void creatureSlamEffect__8CreatureP8Creaturei(void* self, void* other, int flag);
void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(void* self, const void* position, const void* velocity, void* owner,
                                                        void* header_a, void* header_b, int a, int b, int c, int d);
void __9FireStormiP10GameObjectG6Point3P5Skill(void* self, int a, void* owner, const void* position, void* skill);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, const void* point, short angle, void* second_model, void* animation,
    float first, int flag, int zero, float second, void* matrix);
void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
    void* model, void* animation, const void* point, short angle, void* fx_state, float a, float b, float c, float d);
void dropWeapon__5Demon(void* self);
void objectAddToDeleteList__FP10GameObject(void* object);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
extern int frameNm;
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
__attribute__((always_inline)) inline bool near_enemy(void* self, float distance, int angle) {
    return enemyInZone__8Creaturefs(self, distance, static_cast<short>(angle)) != 0;
}
__attribute__((always_inline)) inline void* inner(Address object) {
    return pointer(read_word(address(pointer(read_word(object + 576u))) + 12u));
}
__attribute__((always_inline)) inline void* skill_object(unsigned index) {
    return pointer(read_word(address(skills) + 4u * index));
}
// The type query used for typeid(*E): the function at vtable byte 4, called without arguments.
__attribute__((always_inline)) inline void* query_type(void* object) {
    if (object == nullptr) {
        __throw_bad_typeid();
        return nullptr;
    }
    typedef void* (*TypeCall)();
    return reinterpret_cast<TypeCall>(pointer(read_word(address(pointer(read_word(address(object) + 72u))) + 4u)))();
}
__attribute__((always_inline)) inline void pick_attack(void* self) {
    set_state(self, static_cast<unsigned>(eRand__10GameObject(self)) % 5u == 0u ? 14 : 6);
}
__attribute__((always_inline)) inline void set_effect_halfwords(Address object, unsigned h1048, unsigned h1050, unsigned h1052, unsigned h1054,
                                                               unsigned h1056, unsigned h1058, unsigned h1060, unsigned h1062) {
    write_u16(object + 1056u, h1056); write_u16(object + 1048u, h1048); write_u16(object + 1054u, h1054); write_u16(object + 1058u, h1058);
    write_u16(object + 1050u, h1050); write_u16(object + 1062u, h1062); write_u16(object + 1052u, h1052); write_u16(object + 1060u, h1060);
}
}

extern "C" void msg_run__5Demon(void* self) {
    const Address object = address(self);
    if (read_word(object + 960u) != 0u) {
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        const Address group = address(pointer(read_word(object + 36u)));
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            bytes(object + 1032u), pointer(read_word(group + (read_word(object + 960u) << 3) + 4u)), bytes(object + 968u), point,
            static_cast<short>(read_i16(object + 56u)), pointer(read_word(group + 12u)), bytes(object + 288u), 0.5f,
            read_i16(object + 572u) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    newDamageHandler__8CreatureP12_modelHeader(self, table_word(object, 12u));
    if (everyFrame__8CreatureP12_modelHeader(self, table_word(object, 12u)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    if (read_word(object + 236u) == 0u) return;
    const unsigned flags = static_cast<unsigned>(creatureAdvanceAnimation__8Creaturef(self, 1.0f));
    slideBack__8Creaturef(self, f(0x3e4ccccc));
    {
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 168u, &adjusted);
        call(adjusted);
    }
    const int state = read_i16(object + 572u);
    if (static_cast<unsigned>(state) >= 18u) return;
    float zero[3] = {0.0f, 0.0f, 0.0f};
    switch (state) {
    case 0:
        if (near_enemy(self, f(0x43c80000), 32767)) { set_state(self, 3); return; }
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) set_state(self, 1);
        return;
    case 1:
        move2__8CreatureG6Point3(self, zero);
        if (near_enemy(self, f(0x43c80000), 32767)) { set_state(self, 3); return; }
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) { set_state(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 3: {
        if (read_word(object + 576u) == 0u) { set_state(self, 0); return; }
        void* const target = inner(object);
        const float reach = read_single(object + 28u) + f(0x42200000);
        if (__eq__C9type_infoRC9type_info(query_type(target), __tf4Soul()) != 0) {
            if (near_enemy(self, reach, 5000)) { set_state(self, 13); return; }
            if (near_enemy(self, reach, 32767)) turnToFaceEnemy__8Creature(self);
            else steerAndRouteToEnemy__8Creature(self);
            return;
        }
        if (near_enemy(self, reach, 5000)) { pick_attack(self); return; }
        if (near_enemy(self, read_single(object + 28u) + f(0x42200000), 32767)) { turnToFaceEnemy__8Creature(self); return; }
        if (read_word(object + 208u) == 4u &&
            enemyInZone2__8Creatureffssfi(self, 0.0f, f(0x43c80000), static_cast<short>(read_i16(object + 56u)), 5000, 0.0f, 1) != 0) {
            set_state(self, 15);
            return;
        }
        if (objectIsCrowded__8CreatureP10GameObjectffi(self, inner(object), f(0x43c80000), read_single(object + 28u) * f(0x40400000), 0) == 0) {
            steerAndRouteToEnemy__8Creature(self);
            return;
        }
        write_word(object + 256u, read_word(object + 808u) + static_cast<unsigned>(eRand__10GameObject(self)) % 240u + 240u);
        set_state(self, 5);
        return;
    }
    case 5:
        if (read_word(object + 208u) == 4u &&
            enemyInZone2__8Creatureffssfi(self, 0.0f, f(0x43c80000), static_cast<short>(read_i16(object + 56u)), 5000, 0.0f, 1) != 0) {
            set_state(self, 15);
            return;
        }
        if (near_enemy(self, read_single(object + 28u) + f(0x42480000), 5000)) { pick_attack(self); return; }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) { set_state(self, 3); return; }
        if (read_word(object + 576u) == 0u) return;
        if (objectIsCrowded__8CreatureP10GameObjectffi(self, inner(object), f(0x43c80000), read_single(object + 28u) * f(0x40400000), 1) != 0) return;
        set_state(self, 3);
        return;
    case 6:
    case 16:
    case 17:
        move2__8CreatureG6Point3(self, zero);
        if (read_word(object + 576u) != 0u) {
            if ((flags & 1u) != 0u) {
                unsigned s3 = 0u;
                int first = read_i16(object + 200u);
                int second = read_i16(object + 202u);
                const unsigned mode = read_word(object + 208u);
                if (mode == 2u) {
                    if (signed_word(read_word(object + 964u)) < frameNm) {
                        void* const skill = skill_object(42u);
                        typedef int (*RankCall)(void*, int, int);
                        const Address slot = address(pointer(read_word(address(skill) + 96u))) + 96u;
                        const int rank = reinterpret_cast<RankCall>(pointer(read_word(slot + 4u)))(static_cast<unsigned char*>(skill) + read_i16(slot), 1, 1);
                        write_word(object + 964u, static_cast<unsigned>(frameNm) + 60u * static_cast<unsigned>(rank));
                        void* block = blockAlloc__Fi(28);
                        if (block != nullptr) __14SpellHarmTouchiP8CreatureP5Skill(block, 1, self, skill_object(42u));
                        addSpell__8CreatureP5Spell(self, block);
                        void* const running = checkIfSpellIsRunning__8CreatureRC9type_info(self, __tf14SpellHarmTouch());
                        if (running != nullptr) {
                            s3 = 0x40000u;
                            attack__14SpellHarmTouchP10GameObjectPiT2(running, inner(object), &first, &second);
                        }
                    }
                } else if (mode == 3u) {
                    const float roll = static_cast<float>(eRand__10GameObject(self)) * f(0x37800000);
                    if (roll < f(0x3f000000)) {
                        creatureSlamEffect__8CreatureP8Creaturei(self, inner(object), 1);
                        first = first * 2;
                        second = second * 2;
                    }
                }
                if (read_i16(object + 572u) == 17) s3 |= 0x80000u;
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, f(0x42100000), first, second, read_i16(object + 204u),
                                                                        static_cast<int>(s3 | 8u), bytes(object + 1032u));
            }
            turnToFaceEnemy__8Creature(self);
        }
        if ((read_u16(object + 294u) & 8u) == 0u) { set_state(self, 3); return; }
        if ((flags & 2u) == 0u) return;
        if ((static_cast<unsigned>(eRand__10GameObject(self)) & 1u & 0xffffu) != 0u) return;
        if (!near_enemy(self, read_single(object + 28u) + f(0x42200000), 5000)) return;
        if (read_i16(object + 572u) == 6) set_state(self, 16);
        else if (read_i16(object + 572u) == 16) set_state(self, 17);
        return;
    case 8:
        move2__8CreatureG6Point3(self, zero);
        if ((read_u16(object + 294u) & 0x10u) == 0u) set_state(self, 0);
        return;
    case 9:
        if (signed_word(read_word(object + 804u)) < signed_word(read_word(object + 808u))) set_state(self, 3);
        return;
    case 14:
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) set_state(self, 3);
        return;
    case 10: {
        move2__8CreatureG6Point3(self, zero);
        float point[3];
        for (unsigned i = 0; i < 12u; ++i) reinterpret_cast<unsigned char*>(point)[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
            table_word(object, 12u), bytes(object + 288u), point, static_cast<short>(read_i16(object + 56u)), bytes(object + 824u), 1.0f,
            read_single(object + 224u), read_single(object + 228u), static_cast<float>(read_u8(object + 40u)));
        if (read_word(object + 960u) != 0u && signed_word(read_word(object + 256u) + 60u) < signed_word(read_word(object + 808u))) {
            dropWeapon__5Demon(self);
            write_word(object + 12u, read_word(object + 12u) | 2u);
            write_u64(object + 160u, read_u64(object + 160u) | (1ull << 41));
        }
        if (static_cast<float>(signed_word(read_word(object + 256u))) + f(0x42960000) < static_cast<float>(signed_word(read_word(object + 808u))))
            objectAddToDeleteList__FP10GameObject(self);
        return;
    }
    case 13: {
        move2__8CreatureG6Point3(self, zero);
        void* target = nullptr;
        bool soul_in_range = false;
        if (read_word(object + 576u) != 0u) {
            target = inner(object);
            if (__eq__C9type_infoRC9type_info(query_type(target), __tf4Soul()) != 0)
                soul_in_range = near_enemy(self, read_single(object + 28u) + f(0x42200000), 5000);
        }
        if (!soul_in_range) {
            set_state(self, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), table_word(object, 132u), f(0x3dcccccc), f(0x3dcccccc), 32);
            return;
        }
        if ((flags & 1u) != 0u) {
            typedef void (*HurtCall)(void*, float, void*, int);
            void* adjusted;
            HurtCall call = slot_function<HurtCall>(inner(object), 16u, &adjusted);
            call(adjusted, static_cast<float>(read_i16(object + 202u)), self, 0);
        }
        turnToFaceEnemy__8Creature(self);
        if (read_single(address(inner(object)) + 560u) <= 0.0f) set_state(self, 3);
        return;
    }
    case 15:
        if (read_word(object + 576u) != 0u) {
            const int heading = read_i16(object + 56u);
            const float c = icos__Fi(heading);
            const float s = isin__Fi(heading);
            float lead[3];
            lead[0] = read_single(object + 16u) + c * f(0x43960000);
            lead[1] = read_single(object + 20u) + s * f(0x43960000);
            lead[2] = read_single(object + 24u);
            findEnemyLeadPosition__8CreaturefP6Point3(self, f(0x43c80000), lead);
            if ((flags & 1u) != 0u) {
                const int pick = eRand__10GameObject(self) & 1;
                if (pick == 0) {
                    set_effect_halfwords(object, 255u, 128u, 0u, 50u, 255u, 128u, 0u, 100u);
                    void* const skill = skill_object(57u);
                    typedef int (*FourCall)(void*, int, int, void*);
                    const Address slot = address(pointer(read_word(address(skill) + 96u))) + 104u;
                    FourCall call = reinterpret_cast<FourCall>(pointer(read_word(slot + 4u)));
                    const int first = call(static_cast<unsigned char*>(skill) + read_i16(slot), 0, 1, self);
                    const float scaled_first = static_cast<float>(first) * f(0x3e4ccccc);
                    const float roll = static_cast<float>(eRand__10GameObject(self)) * f(0x37800000);
                    const int total = first + static_cast<int>(scaled_first * roll);
                    void* const skill1 = skill_object(57u);
                    const Address slot1 = address(pointer(read_word(address(skill1) + 96u))) + 104u;
                    const int second = reinterpret_cast<FourCall>(pointer(read_word(slot1 + 4u)))(static_cast<unsigned char*>(skill1) + read_i16(slot1), 1, 1, self);
                    void* const skill2 = skill_object(57u);
                    const Address slot2 = address(pointer(read_word(address(skill2) + 96u))) + 104u;
                    const int third = reinterpret_cast<FourCall>(pointer(read_word(slot2 + 4u)))(static_cast<unsigned char*>(skill2) + read_i16(slot2), 2, 1, self);
                    void* const skill3 = skill_object(57u);
                    const Address slot3 = address(pointer(read_word(address(skill3) + 96u))) + 104u;
                    const int fourth = reinterpret_cast<FourCall>(pointer(read_word(slot3 + 4u)))(static_cast<unsigned char*>(skill3) + read_i16(slot3), 3, 1, self);
                    const int step = third * 3 * 4;
                    const float speed = static_cast<float>(fourth);
                    const int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, inner(object), speed);
                    float velocity[3];
                    velocity[0] = icos__Fi(yaw) * speed;
                    velocity[1] = isin__Fi(yaw) * speed;
                    velocity[2] = 0.0f;
                    void* block = blockAlloc__Fi(184);
                    if (block != nullptr) {
                        float position[3];
                        position[0] = read_single(object + 16u) + 0.0f;
                        position[1] = read_single(object + 20u);
                        position[2] = read_single(object + 24u) + read_single(object + 32u) * f(0x3f000000);
                        __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(block, position, velocity, self, nullptr, nullptr, first, total, second, step);
                    }
                } else if (pick == 1) {
                    set_effect_halfwords(object, 255u, 128u, 0u, 50u, 255u, 128u, 0u, 100u);
                    void* block = blockAlloc__Fi(168);
                    if (block != nullptr) {
                        float position[3];
                        position[0] = lead[0] + 0.0f;
                        position[1] = lead[1];
                        position[2] = lead[2] + f(0x40c00000);
                        __9FireStormiP10GameObjectG6Point3P5Skill(block, 1, self, position, skill_object(7u));
                    }
                }
            }
            typedef void (*TurnCall)(void*, int, const void*);
            void* adjusted;
            TurnCall call = slot_function<TurnCall>(self, 176u, &adjusted);
            call(adjusted, 0, lead);
        }
        if (signed_word(read_word(object + 256u)) < signed_word(read_word(object + 808u))) { set_state(self, 3); return; }
        if (near_enemy(self, read_single(object + 28u) + f(0x42200000), 5000)) set_state(self, 6);
        return;
    default:
        return;
    }
}
