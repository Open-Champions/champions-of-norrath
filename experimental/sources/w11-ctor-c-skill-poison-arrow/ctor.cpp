// Observed __16SkillPoisonArrow at 0x002a0b80, 48 bytes (this). Constructor of the SkillPoisonArrow
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "poisonarrow"
//   +4  word   0
//   +8  word   8
//   +12 half   30
//   +96 word   address of the SkillPoisonArrow table (symbol _vt$16SkillPoisonArrow)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillPoisonArrow[] __asm__("_vt$16SkillPoisonArrow");
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

extern "C" void __16SkillPoisonArrow(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillPoisonArrow;
    self->name = "poisonarrow";
    self->value = 8;
    self->zero = 0;
}
