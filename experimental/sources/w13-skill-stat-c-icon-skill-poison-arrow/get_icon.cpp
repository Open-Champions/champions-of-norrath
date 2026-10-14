// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317e50 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icpoisonarrow.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillPoisonArrow;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillPoisonArrow*) asm("getIcon__16SkillPoisonArrow");
extern "C" void* get_icon(const SkillPoisonArrow*) {
    return lump_find_resource("icons.lmp", "icpoisonarrow.tex");
}
