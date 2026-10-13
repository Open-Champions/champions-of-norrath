// Observed __22SkillRangerCriticalHit at 0x002c7bd0, 44 bytes (this). Constructor of the SkillRangerCriticalHit
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "rangercriticalhit"
//   +4  word   0
//   +8  word   0
//   +12 half   30
//   +96 word   address of the SkillRangerCriticalHit table (symbol _vt$22SkillRangerCriticalHit)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillRangerCriticalHit[] __asm__("_vt$22SkillRangerCriticalHit");
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

extern "C" void __22SkillRangerCriticalHit(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillRangerCriticalHit;
    self->name = "rangercriticalhit";
    self->value = 0;
    self->zero = 0;
}
