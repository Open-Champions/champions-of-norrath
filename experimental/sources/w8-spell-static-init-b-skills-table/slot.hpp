#pragma once
// Layout of one skill object slot as seen through the initializer of the skill
// table. Observed: the 61 slots sit back to back at 104-byte spacing starting at
// 0x602908 in the pinned SLUS-20565 image, and the initializer stores a pointer at
// offset 0, a word at 4, a word at 8, a halfword at 12 and a pointer at 96
// (always the address of a `_vt$...` symbol). Most slots instead get a constructor
// call that fills them. Unknown: the meaning of the individual fields.
struct norrath_skill_slot {
    const void* first_pointer;
    unsigned second_word;
    unsigned third_word;
    unsigned short fourth_half;
    unsigned char untouched_by_initializer[82];
    const void* table_pointer;
    unsigned char tail[4];
};
static_assert(sizeof(norrath_skill_slot) == 104, "slot spacing observed in the image");

extern "C" norrath_skill_slot norrath_skill_slots[61];
