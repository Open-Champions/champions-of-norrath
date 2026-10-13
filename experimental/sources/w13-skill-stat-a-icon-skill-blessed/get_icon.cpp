// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318610 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icblessed.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillBlessed;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillBlessed*) asm("getIcon__12SkillBlessed");
extern "C" void* get_icon(const SkillBlessed*) {
    return lump_find_resource("icons.lmp", "icblessed.tex");
}
