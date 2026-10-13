// Observed __9SkillRoot at 0x002ace88, 44 bytes (this). Constructor of the SkillRoot
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "root"
//   +4  word   0
//   +8  word   0
//   +12 half   30
//   +96 word   address of the SkillRoot table (symbol _vt$9SkillRoot)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillRoot[] __asm__("_vt$9SkillRoot");
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

extern "C" void __9SkillRoot(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillRoot;
    self->name = "root";
    self->value = 0;
    self->zero = 0;
}
