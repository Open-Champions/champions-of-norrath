// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318e10 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icroot.tex") and return its result. The receiver is not read.
struct SkillRoot;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillRoot*) asm("getIcon__9SkillRoot");
extern "C" void* get_icon(const SkillRoot*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icroot.tex"));
}
