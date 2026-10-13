// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003187d8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icancestralcall.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillAncestralCall;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillAncestralCall*) asm("getIcon__18SkillAncestralCall");
extern "C" void* get_icon(const SkillAncestralCall*) {
    return lump_find_resource("icons.lmp", "icancestralcall.tex");
}
