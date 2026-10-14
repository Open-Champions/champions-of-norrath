// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0031a628 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccriticalhit.tex") and return its result. The receiver is not read.
struct SkillRangerCriticalHit;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillRangerCriticalHit*) asm("getIcon__22SkillRangerCriticalHit");
extern "C" void* get_icon(const SkillRangerCriticalHit*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("iccriticalhit.tex"));
}
