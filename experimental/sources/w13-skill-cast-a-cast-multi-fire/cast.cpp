// Observed cast__14SkillMultiFireiP8CreatureG6Point3s at 0x002a0d58, 1408 bytes in .text. The fifth argument is a
// signed halfword (the trailing "s" of the mangled name), used as an integer angle.
// Steps, in order (R is the tweaked rank, P the incoming point; "entry104" is the skill table entry at byte 104, a signed
// halfword receiver adjustment and a function word at +4, called as (skill + adjustment, ramp, rank, creature)):
//  1. R = table entry at byte 120 called with (skill + adjustment, rank, 20).
//  2. 188 bytes are allocated with blockAlloc__Fi; when the block is not null,
//     __11LightEffectG6Point3ffffffffffffP10GameObjecti is called with (block, (P.x + 0.0, P.y, P.z + 20.0), 124.0,
//     115.0, 204.0, five zeros, 130.09157, 60.0, 0.5, 1.0, null, 0).
//  3. N = entry104(2, R), converted to float n; start = (short)(angle - (int)(n * 0.5 * 534.5)); speed = (float)entry104(1, R).
//  4. Damage pair (low, high), item word and bonus, and flags. If bit 0x40 of the word at creature+12 is set (the
//     creature is a player): with i = the word at creature+960 and record = player + 23984 * i, the item pointer is the word at
//     record + 4 * (signed byte at record+23572) + 21960; k = (float)entry104(0, rank) * 0.01 / n; low and high are
//     (float)(word at record+212) * k and (float)(word at record+216) * k, each rounded half away from zero
//     (add or subtract 0.5 by the sign, then truncate); bonus = getAttackBonus__4itemPQ24item11description(item); the item word is the
//     word at item+8; flags = 0x40040000. Otherwise: base = entry104(0, rank); x = (float)base * 0.2 (as this toolchain rounds the literal, bit pattern
//     0x3e4ccccc, the value the original loads); two draws of the eRandState generator (state = state * 0x19660d + 0x3c6ef35f, value = (state >> 16) *
//     2^-16) give r1 and r2 (only the second state is stored, the first is dead); low = base; high = base + (int)(x * r2 +
//     0.5) when x * r1 >= 0, otherwise base + (int)(x * r2 - 0.5); bonus = 0, item word = 0, flags = 0x40000000.
//  5. When N > 0, N times, with the angle a starting at `start` and increasing by 1069 (as a short) each pass: velocity
//     v = (icos(a) * speed, isin(a) * speed, 0); a draw of syncRandState with the same generator gives s; the point used
//     is (P.x, P.y, (P.z - s * 6.0) + -6.0); 452 bytes are allocated and, when the block is not null,
//     __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description is called with
//     (block, 0, word at global-item+8, word at global-item+12, point, v, a - 16384, creature, null, null, low, high, bonus,
//     item word, 10000, flags, null), where global-item is the data object _4item$globalItem.
//  6. Always: lumpFindResource__FPcT0 with "fx.lmp" and "spel_we_firearrow.anm", then
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf with (the result, creature, 1.0). The function returns zero.
// The table entries at bytes 120 and 104 are named, in the class tables, tweekRank__5Skillii and
// evaluateRampAtRank__5SkilliiP10GameObject; those readings are inferred from the tables. Inferred: this fires a fan of
// N missiles around the heading, whose damage comes from the skill's ramp (or from the player's two stored values
// scaled by the ramp), with a small random height jitter, and plays the effect sound. Unknown: what the player record
// words at +212, +216 and the byte at +23572 are, what the flags and the constants 10000, 534.5, 0.01 and 0.2 mean, which
// values the LightEffect floats set, the angle unit, and the layouts of skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
int getAttackBonus__4itemPQ24item11description(void* description);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void light_effect_ctor(void* self, Point3 position, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11,
                       float f12, void* object, int number) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void missile_weapon_ctor(void* self, int a, void* model, void* texture, Point3 position, Point3 velocity, int angle, void* creature, void* vag1, void* vag2, int low, int high,
                         int bonus, int item_word, int constant, int flags, void* description)
    asm("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
extern unsigned char player[];
extern unsigned char global_item[] asm("_4item$globalItem");
extern int eRandState;
extern int syncRandState;
int cast_multi_fire(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__14SkillMultiFireiP8CreatureG6Point3s");
}
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedBytes __attribute__((may_alias));
static inline int word_at(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
static inline int round_away(float value) { return static_cast<int>(0.0f <= value ? value + 0.5f : value - 0.5f); }

extern "C" int cast_multi_fire(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* const skill_bytes = static_cast<unsigned char*>(skill);
    unsigned char* const creature_bytes = static_cast<unsigned char*>(creature);
    Point3 jittered = point;
    int bonus = 0;
    int item_word = 0;
    const int tweaked = w13sca::call_skill_entry(skill_bytes, 120, rank, 20);
    void* light = blockAlloc__Fi(188);
    if (light) {
        Point3 position;
        position.x = point.x + 0.0f;
        position.y = point.y;
        position.z = point.z + 20.0f;
        light_effect_ctor(light, position, 124.0f, 115.0f, 204.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 130.09157f, 60.0f, 0.5f, 1.0f, nullptr, 0);
    }
    const int count = w13sca::call_skill_entry3(skill_bytes, 104, 2, tweaked, creature);
    const float count_f = static_cast<float>(count);
    const short start = static_cast<short>(angle - static_cast<int>(count_f * 0.5f * 534.5f));
    const float speed = static_cast<float>(w13sca::call_skill_entry3(skill_bytes, 104, 1, tweaked, creature));
    int low, high, flags;
    if (word_at(creature_bytes, 12) & 0x40) {
        unsigned char* const record = player + word_at(creature_bytes, 960) * 23984;
        const int item = word_at(record + 4 * static_cast<signed char>(record[23572]), 21960);
        const float ramp0 = static_cast<float>(w13sca::call_skill_entry3(skill_bytes, 104, 0, rank, creature));
        const float scale = ramp0 * 0.01f / count_f;
        unsigned char* const record2 = player + word_at(creature_bytes, 960) * 23984;
        low = round_away(static_cast<float>(word_at(record2, 212)) * scale);
        unsigned char* const record3 = player + word_at(creature_bytes, 960) * 23984;
        high = round_away(static_cast<float>(word_at(record3, 216)) * scale);
        unsigned char* const item_bytes = w13sca::from_word(static_cast<unsigned>(item));
        bonus = getAttackBonus__4itemPQ24item11description(item_bytes);
        item_word = word_at(item_bytes, 8);
        flags = 0x40040000;
    } else {
        const int base = w13sca::call_skill_entry3(skill_bytes, 104, 0, rank, creature);
        const float x = static_cast<float>(base) * 0.2f;
        const unsigned state1 = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
        const float r1 = static_cast<float>(state1 >> 16) * 0.0000152587890625f;
        const unsigned state2 = state1 * 0x19660du + 0x3c6ef35fu;
        eRandState = static_cast<int>(state2);
        const float r2 = static_cast<float>(state2 >> 16) * 0.0000152587890625f;
        low = base;
        high = base + static_cast<int>(0.0f <= x * r1 ? x * r2 + 0.5f : x * r2 - 0.5f);
        flags = 0x40000000;
    }
    if (count > 0) {
        short a = start;
        for (int remaining = count; remaining != 0; --remaining) {
            Point3 velocity;
            velocity.x = icos__Fi(a) * speed;
            velocity.y = isin__Fi(a) * speed;
            velocity.z = 0.0f;
            const unsigned state = static_cast<unsigned>(syncRandState) * 0x19660du + 0x3c6ef35fu;
            syncRandState = static_cast<int>(state);
            jittered.z = (point.z - static_cast<float>(state >> 16) * 0.0000152587890625f * 6.0f) + -6.0f;
            void* missile = blockAlloc__Fi(452);
            if (missile)
                missile_weapon_ctor(missile, 0, w13sca::from_word(static_cast<unsigned>(word_at(global_item, 8))),
                                    w13sca::from_word(static_cast<unsigned>(word_at(global_item, 12))), jittered, velocity, a - 16384, creature, nullptr,
                                    nullptr, low, high, bonus, item_word, 10000, flags, nullptr);
            a = static_cast<short>(a + 1069);
        }
    }
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_firearrow.anm")), creature, 1.0f);
    return 0;
}
