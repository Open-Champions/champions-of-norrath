// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003180d8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icexplodingarrow.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillExplodeArrow;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillExplodeArrow*) asm("getIcon__17SkillExplodeArrow");
extern "C" void* get_icon(const SkillExplodeArrow*) {
    return lump_find_resource("icons.lmp", "icexplodingarrow.tex");
}
