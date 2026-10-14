// Observed canCast__9SkillBashP8Creature at 0x002a5dc8, 120 bytes in .text.
// The skill argument is not read. Reading the creature (second argument), with p = the signed word at creature+960:
//  1. When bit 0x40 of the word at creature+12 is clear the function returns one and does nothing else.
//  2. Otherwise, when the word at (player + p * 23984) + 21908 is not zero, it returns one.
//  3. Otherwise the message for the key "bash_warn" is looked up with textGetString__FPc and shown with
//     hudSetInfoMessage__FPUsiic(text, 180, the word at creature+960 read again, 0), and the function returns zero.
// Inferred: bit 0x40 marks a creature controlled by a player and the word at record+21908 is a slot that must hold
// something (such as an equipped item) for the skill to be usable; the key text suggests a warning. Unknown: what that
// word is, and the bit.
extern "C" {
extern unsigned char player[];
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag);
int can_cast_bash(void* skill, void* creature) asm("canCast__9SkillBashP8Creature");
}
static char warn_key[] = "bash_warn";
extern "C" int can_cast_bash(void*, void* creature) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    if (!(*reinterpret_cast<unsigned*>(c + 12) & 0x40)) return 1;
    if (*reinterpret_cast<unsigned*>(player + *reinterpret_cast<int*>(c + 960) * 23984 + 21908)) return 1;
    hudSetInfoMessage__FPUsiic(textGetString__FPc(warn_key), 180, *reinterpret_cast<int*>(c + 960), 0);
    return 0;
}
