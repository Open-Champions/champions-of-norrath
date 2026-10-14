// Observed __14SkillMultiFire at 0x002a1bb0, 48 bytes (this). Constructor of the SkillMultiFire
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "multifire"
//   +4  word   0
//   +8  word   8
//   +12 half   30
//   +96 word   address of the SkillMultiFire table (symbol _vt$14SkillMultiFire)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillMultiFire[] __asm__("_vt$14SkillMultiFire");
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

extern "C" void __14SkillMultiFire(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillMultiFire;
    self->name = "multifire";
    self->value = 8;
    self->zero = 0;
}
