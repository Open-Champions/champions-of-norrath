// Observed canCast__16SkillGroundPoundP8Creature at 0x00290928, 172 bytes in .text.
// The skill argument is not read. Reading the creature (second argument), with p = the signed word at creature+960 and
// R = the player record player + p * 23984 (the stride used by other authored code):
//  1. When bit 0x40 of the word at creature+12 is clear the function returns one and does nothing else.
//  2. Otherwise ok = (the signed byte R[23582] == 2). When the word at R + 21960 + 4 * (signed byte R[23572]) is not
//     zero, ok also requires the word at ((that word's first word) + 32) to equal one; otherwise ok is zero.
//  3. When ok is zero the message for the key "groundpound_warn" is looked up with textGetString__FPc and shown with
//     hudSetInfoMessage__FPUsiic(text, 180, the word at creature+960 read again, 0).
//  4. The function returns ok (zero or one).
// Inferred: bit 0x40 marks a creature controlled by a player; the byte at R[23582] is a per-player state value and
// the word array at R+21960 holds the player's equipped slots indexed by the byte at R[23572]; so the skill needs the
// state value 2 and, with something in the selected slot, a particular value in that item's description. The key
// text suggests the message is a warning. None of this is established here.
// Unknown: the meaning of the state values, of the description word at +32, and of the bit.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
extern unsigned char player[];
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag);
int can_cast_ground_pound(void* skill, void* creature) asm("canCast__16SkillGroundPoundP8Creature");
}
static char warn_key[] = "groundpound_warn";
extern "C" int can_cast_ground_pound(void*, void* creature) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    if (!(*reinterpret_cast<unsigned*>(c + 12) & 0x40)) return 1;
    unsigned char* record = player + *reinterpret_cast<int*>(c + 960) * 23984;
    int ok = static_cast<signed char>(record[23582]) == 2;
    const unsigned item = *reinterpret_cast<unsigned*>(record + 21960 + 4 * static_cast<signed char>(record[23572]));
    if (item) {
        const unsigned description = *reinterpret_cast<unsigned*>(w13scb::from_word(item));
        if (*reinterpret_cast<int*>(w13scb::from_word(description) + 32) != 1) ok = 0;
    }
    if (!ok) hudSetInfoMessage__FPUsiic(textGetString__FPc(warn_key), 180, *reinterpret_cast<int*>(c + 960), 0);
    return ok;
}
