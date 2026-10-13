// Observed cast__16SkillShockOfFireiP8CreatureG6Point3s at 0x002b2e70, 416 bytes in .text.
// Steps, in order (skill = first argument, rank = second, creature = third, point = the by-value Point3, angle = the
// trailing short; the function returns zero in every case):
//  1. the skill's table (the word at skill+96) entry at byte 120 is called as (skill + adjustment, rank, 20); its integer
//     result is R. (Slot 15 of this class's table holds tweekRank__5Skillii, observed in the private image.)
//  2. the entry at byte 72 (slot 9, getRampDamage__5SkilliifiPiT5 in the image) is called as (skill + adjustment, 0, R,
//     0.2 (bits 0x3e4ccccc, a pool word at gp-19620), 0, &low, &high) with two local integers for the two outputs;
//  3. the entry at byte 88 (slot 11, evaluateRampAtRank__5Skillii) is called three times as (skill + adjustment, k, R)
//     for k = 1, 2, 3, giving A, B and C;
//  4. a velocity-like Point3 is built: (icos__Fi(angle) * C, isin__Fi(angle) * C, 0), with C converted to float;
//  5. 184 bytes are allocated with blockAlloc__Fi; when the block is not null,
//     __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii is called with (block, the point, the velocity-like point,
//     creature, null, null, low, high, A, 12 * B). The block pointer is not returned.
// Inferred: the object is a spray of sparks starting at the point; low/high/A/12*B are damage-ramp values for it. Slot
// meanings come from the symbol names of the class table, not from this function.
// Unknown: the meaning of the Sparks arguments, of the constants 20 and 0.2, and the layouts of skill and creature.
// Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void sparks_ctor(void* self, Point3 position, Point3 velocity, void* creature, void* first_vag, void* second_vag, int low, int high, int a, int b) asm("__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii");
int cast_shock_of_fire(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__16SkillShockOfFireiP8CreatureG6Point3s");
}
extern "C" int cast_shock_of_fire(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* s = static_cast<unsigned char*>(skill);
    const int adjusted_rank = w13scb::call_skill_entry(s, 120, rank, 20);
    int low, high;
    w13scb::call_ramp_damage(s, 72, 0, adjusted_rank, 0x1.999998p-3f, 0, &low, &high);
    const int a = w13scb::call_skill_entry(s, 88, 1, adjusted_rank);
    const int b = w13scb::call_skill_entry(s, 88, 2, adjusted_rank);
    const int c = w13scb::call_skill_entry(s, 88, 3, adjusted_rank);
    const float speed = static_cast<float>(c);
    Point3 velocity;
    velocity.x = icos__Fi(angle) * speed;
    velocity.y = isin__Fi(angle) * speed;
    velocity.z = 0;
    void* sparks = blockAlloc__Fi(184);
    if (sparks) sparks_ctor(sparks, point, velocity, creature, nullptr, nullptr, low, high, a, b * 12);
    return 0;
}
