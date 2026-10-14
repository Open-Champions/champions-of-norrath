// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317630 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icconvertundead.tex") and return its result. The receiver is not read.
struct SkillConvertUndead;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillConvertUndead*) asm("getIcon__18SkillConvertUndead");
extern "C" void* get_icon(const SkillConvertUndead*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icconvertundead.tex"));
}
