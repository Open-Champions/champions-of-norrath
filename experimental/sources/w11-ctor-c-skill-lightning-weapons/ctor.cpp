// Observed __21SkillLightningWeapons at 0x002a9d20, 44 bytes (this). Constructor of the SkillLightningWeapons
// record, one of the 104-byte skill objects built by the skills static initialiser.
// It writes, and reads nothing:
//   +0  word   address of the text "lightningweapons"
//   +4  word   0
//   +8  word   0
//   +12 half   30
//   +96 word   address of the SkillLightningWeapons table (symbol _vt$21SkillLightningWeapons)
// Other bytes of the object are not touched. The return register is not set
// to a meaningful value, so the function is declared void.
// Unknown: what the word at +8 and the halfword at +12 mean.
extern "C" {
extern unsigned char vt_SkillLightningWeapons[] __asm__("_vt$21SkillLightningWeapons");
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

extern "C" void __21SkillLightningWeapons(SkillObject* self) {
    self->half = 30;
    self->table = vt_SkillLightningWeapons;
    self->name = "lightningweapons";
    self->value = 0;
    self->zero = 0;
}
