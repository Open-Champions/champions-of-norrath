// `skills` at 0x34b440 (252 bytes, initialized data) in the pinned SLUS-20565 image.
// Observed: 63 words. The first and the last are zero; the 61 between them are the
// addresses of the 61 consecutive 104-byte skill slots that start at 0x602908,
// in order. The slot storage is defined here with an external name because the
// code that uses the slots lives in other units.
// Inferred: that `skills` is indexed by a skill number counted from 1.
// Unknown: which number belongs to which skill class beyond the construction
// order recorded in the initializer unit.
// Nothing here has run on the EE.
#include "slot.hpp"

extern "C" {
norrath_skill_slot norrath_skill_slots[61];

norrath_skill_slot* skills[63] = {
    nullptr,
    &norrath_skill_slots[0],
    &norrath_skill_slots[1],
    &norrath_skill_slots[2],
    &norrath_skill_slots[3],
    &norrath_skill_slots[4],
    &norrath_skill_slots[5],
    &norrath_skill_slots[6],
    &norrath_skill_slots[7],
    &norrath_skill_slots[8],
    &norrath_skill_slots[9],
    &norrath_skill_slots[10],
    &norrath_skill_slots[11],
    &norrath_skill_slots[12],
    &norrath_skill_slots[13],
    &norrath_skill_slots[14],
    &norrath_skill_slots[15],
    &norrath_skill_slots[16],
    &norrath_skill_slots[17],
    &norrath_skill_slots[18],
    &norrath_skill_slots[19],
    &norrath_skill_slots[20],
    &norrath_skill_slots[21],
    &norrath_skill_slots[22],
    &norrath_skill_slots[23],
    &norrath_skill_slots[24],
    &norrath_skill_slots[25],
    &norrath_skill_slots[26],
    &norrath_skill_slots[27],
    &norrath_skill_slots[28],
    &norrath_skill_slots[29],
    &norrath_skill_slots[30],
    &norrath_skill_slots[31],
    &norrath_skill_slots[32],
    &norrath_skill_slots[33],
    &norrath_skill_slots[34],
    &norrath_skill_slots[35],
    &norrath_skill_slots[36],
    &norrath_skill_slots[37],
    &norrath_skill_slots[38],
    &norrath_skill_slots[39],
    &norrath_skill_slots[40],
    &norrath_skill_slots[41],
    &norrath_skill_slots[42],
    &norrath_skill_slots[43],
    &norrath_skill_slots[44],
    &norrath_skill_slots[45],
    &norrath_skill_slots[46],
    &norrath_skill_slots[47],
    &norrath_skill_slots[48],
    &norrath_skill_slots[49],
    &norrath_skill_slots[50],
    &norrath_skill_slots[51],
    &norrath_skill_slots[52],
    &norrath_skill_slots[53],
    &norrath_skill_slots[54],
    &norrath_skill_slots[55],
    &norrath_skill_slots[56],
    &norrath_skill_slots[57],
    &norrath_skill_slots[58],
    &norrath_skill_slots[59],
    &norrath_skill_slots[60],
    nullptr,
};
}
