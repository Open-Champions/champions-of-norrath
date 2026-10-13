// Observed __17SkillMinorHealing at 0x002a72c0, 44 bytes (this). Constructor of the SkillMinorHealing
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "minorhealing"
//   +4  word   0
//   +8  word   0
//   +12 half   30
//   +96 word   address of the SkillMinorHealing table (symbol _vt$17SkillMinorHealing)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillMinorHealing[] __asm__("_vt$17SkillMinorHealing");
}

struct SkillObject {
    const char* name;
    int zero;
    int value;
    short half;
    char untouched[82];
    const void* table;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(SkillObject, table) == 96, "table at +96");
#endif

extern "C" void __17SkillMinorHealing(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillMinorHealing;
    self->name = "minorhealing";
    self->value = 0;
    self->zero = 0;
}
