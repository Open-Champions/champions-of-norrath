// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317d00 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccoldarrow.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillColdArrow;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillColdArrow*) asm("getIcon__14SkillColdArrow");
extern "C" void* get_icon(const SkillColdArrow*) {
    return lump_find_resource("icons.lmp", "iccoldarrow.tex");
}
