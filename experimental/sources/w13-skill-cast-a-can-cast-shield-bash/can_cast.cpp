// Observed canCast__15SkillShieldBashP8Creature at 0x002c8a40, 120 bytes in .text; the skill argument is not read.
// If bit 0x40 of the word at creature+12 is clear the function returns one. Otherwise the word at creature+960
// indexes the player table (record size 23984 bytes, word at record+21908). When that word is not zero the function
// returns one. When it is zero, the text string for the key "shieldbash_warn" is fetched with textGetString__FPc and
// passed to hudSetInfoMessage__FPUsiic with (text, 180, the word at creature+960, 0), and the function returns zero.
// Inferred: a player-controlled creature (flag bit 0x40) may not shield-bash while that word of its record is zero,
// and is shown a warning message instead. Unknown: what the word is, what bit 0x40 means in general, and what 180 and
// the last argument of the message call select. Nothing here ran on the EE.
extern "C" {
unsigned short* textGetString__FPc(const char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
extern unsigned char player[];
int can_cast_shield_bash(void* skill, void* creature) asm("canCast__15SkillShieldBashP8Creature");
}
typedef int AliasedInt __attribute__((may_alias));
extern "C" int can_cast_shield_bash(void*, void* creature) {
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    int result = 1;
    if (*reinterpret_cast<AliasedInt*>(bytes + 12) & 0x40) {
        const int index = *reinterpret_cast<AliasedInt*>(bytes + 960);
        if (*reinterpret_cast<AliasedInt*>(player + index * 23984 + 21908) == 0) {
            result = 0;
            hudSetInfoMessage__FPUsiic(textGetString__FPc("shieldbash_warn"), 180, *reinterpret_cast<AliasedInt*>(bytes + 960), 0);
        }
    }
    return result;
}
