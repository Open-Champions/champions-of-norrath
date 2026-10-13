// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317fa0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icmultishot.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillMultiFire;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillMultiFire*) asm("getIcon__14SkillMultiFire");
extern "C" void* get_icon(const SkillMultiFire*) {
    return lump_find_resource("icons.lmp", "icmultishot.tex");
}
