// Observed __15SkillWizardBeam at 0x002b9368, 48 bytes (this). Constructor of the SkillWizardBeam
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "wizardbeam"
//   +4  word   0
//   +8  word   1
//   +12 half   30
//   +96 word   address of the SkillWizardBeam table (symbol _vt$15SkillWizardBeam)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillWizardBeam[] __asm__("_vt$15SkillWizardBeam");
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

extern "C" void __15SkillWizardBeam(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillWizardBeam;
    self->name = "wizardbeam";
    self->value = 1;
    self->zero = 0;
}
