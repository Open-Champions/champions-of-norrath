// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00319d70 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icrepulseundead.tex") and return its result. The receiver is not read.
struct SkillRepulseUndead;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillRepulseUndead*) asm("getIcon__18SkillRepulseUndead");
extern "C" void* get_icon(const SkillRepulseUndead*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icrepulseundead.tex"));
}
