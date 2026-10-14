// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00319de8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icdismissundead.tex") and return its result. The receiver is not read.
struct SkillDismissUndead;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillDismissUndead*) asm("getIcon__18SkillDismissUndead");
extern "C" void* get_icon(const SkillDismissUndead*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icdismissundead.tex"));
}
