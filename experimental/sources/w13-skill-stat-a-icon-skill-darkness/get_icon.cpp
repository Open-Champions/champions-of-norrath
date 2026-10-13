// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317298 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icengulfingdarkness.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillDarkness;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillDarkness*) asm("getIcon__13SkillDarkness");
extern "C" void* get_icon(const SkillDarkness*) {
    return lump_find_resource("icons.lmp", "icengulfingdarkness.tex");
}
