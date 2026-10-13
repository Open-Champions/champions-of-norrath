// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003191e0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "ichealing.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillHealing;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillHealing*) asm("getIcon__12SkillHealing");
extern "C" void* get_icon(const SkillHealing*) {
    return lump_find_resource("icons.lmp", "ichealing.tex");
}
