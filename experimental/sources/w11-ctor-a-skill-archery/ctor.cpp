// Observed __12SkillArchery at 0x2a2b38, 44 or 48 bytes. The argument is the skill object (the callers
// pass a 104-byte slot). It performs only stores and does not return a value:
//  +0   pointer to the text "archery"
//  +4   0
//  +8   3
//  +12  halfword 30
//  +96  pointer to the class table _vt$12SkillArchery
// No other bytes are read or written. Inferred: +0 is a name or key string; what +4, +8 and
// the halfword mean is unknown, and nothing here ran on the EE.
extern "C" {
extern unsigned char vt_skill[] __asm__("_vt$12SkillArchery");
void __12SkillArchery(void* self);
}

namespace {
const char text[] = "archery";
unsigned word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void __12SkillArchery(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(o + 0) = word(text);
    *reinterpret_cast<unsigned*>(o + 4) = 0;
    *reinterpret_cast<unsigned*>(o + 8) = 3;
    *reinterpret_cast<unsigned short*>(o + 12) = 30;
    *reinterpret_cast<unsigned*>(o + 96) = word(vt_skill);
}
