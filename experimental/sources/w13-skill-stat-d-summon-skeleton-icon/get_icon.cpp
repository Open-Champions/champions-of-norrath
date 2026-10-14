// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318a10 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icraiseskeleton.tex") and return its result. The receiver is not read.
struct SkillSummonSkeleton;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillSummonSkeleton*) asm("getIcon__19SkillSummonSkeleton");
extern "C" void* get_icon(const SkillSummonSkeleton*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icraiseskeleton.tex"));
}
