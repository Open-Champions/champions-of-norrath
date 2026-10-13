// Observed canCast__12SkillCycloneP8Creature at 0x0029a740, 132 bytes in .text; the skill argument is not read.
// If bit 0x40 of the word at creature+12 is clear the function returns one. Otherwise the word at creature+960
// indexes the player table (record size 23984 bytes, byte at record+23582). When that byte is not 4 the function
// returns one. When it is 4, the text string for the key "cyclone_warn" is fetched with textGetString__FPc and
// passed to hudSetInfoMessage__FPUsiic with (text, 180, the word at creature+960, 0), and the function returns zero.
// Inferred: a player-controlled creature (flag bit 0x40) may not cast the cyclone while that byte of its record
// holds 4, and is shown a warning message instead. Unknown: what the byte is, what bit 0x40 means in general, and what
// 180 and the last argument of the message call select. Nothing here ran on the EE.
extern "C" {
unsigned short* textGetString__FPc(const char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
extern unsigned char player[];
int can_cast_cyclone(void* skill, void* creature) asm("canCast__12SkillCycloneP8Creature");
}
typedef int AliasedInt __attribute__((may_alias));
extern "C" int can_cast_cyclone(void*, void* creature) {
    unsigned char* bytes = static_cast<unsigned char*>(creature);
    int result = 1;
    if (*reinterpret_cast<AliasedInt*>(bytes + 12) & 0x40) {
        const int index = *reinterpret_cast<AliasedInt*>(bytes + 960);
        if (static_cast<signed char>(player[index * 23984 + 23582]) == 4) {
            result = 0;
            hudSetInfoMessage__FPUsiic(textGetString__FPc("cyclone_warn"), 180, *reinterpret_cast<AliasedInt*>(bytes + 960), 0);
        }
    }
    return result;
}
