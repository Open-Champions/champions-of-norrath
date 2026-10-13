// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313268 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icblank.tex") and return its result. The receiver is not read.
struct Skill;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const Skill*) asm("getIcon__5Skill");
extern "C" void* get_icon(const Skill*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icblank.tex"));
}
