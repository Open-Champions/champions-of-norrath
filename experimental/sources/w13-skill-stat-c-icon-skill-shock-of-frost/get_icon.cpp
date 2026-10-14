// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00319e60 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icshockoffrost.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillShockOfFrost;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillShockOfFrost*) asm("getIcon__17SkillShockOfFrost");
extern "C" void* get_icon(const SkillShockOfFrost*) {
    return lump_find_resource("icons.lmp", "icshockoffrost.tex");
}
