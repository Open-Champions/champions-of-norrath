// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318450 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icbash.tex") and return its result. The receiver is not read.
struct SkillBash;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillBash*) asm("getIcon__9SkillBash");
extern "C" void* get_icon(const SkillBash*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icbash.tex"));
}
