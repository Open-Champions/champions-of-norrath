// Observed getRankDescription__5SkillPUsi at 0x00313210 (weak symbol, 56 bytes): arguments the Skill
// object (unused), a UTF-16 output buffer and a rank (unused). It looks up the text for the key "no"
// with textGetString__FPc and copies the result into the buffer with ustrcpy__FPUsT0. Nothing is returned.
// Inferred: this is the default description for a skill that has no rank text; the class tables list it
// for classes that do not name their own.
// Unknown: what text the key "no" resolves to.
extern "C" {
const unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from);

void getRankDescription__5SkillPUsi(void* self, unsigned short* out, int rank) {
    (void)self;
    (void)rank;
    ustrcpy__FPUsT0(out, textGetString__FPc("no"));
}
}
