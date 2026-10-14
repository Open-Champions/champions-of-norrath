// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00316c50 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icdiseasebolt.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillDiseaseBolt;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillDiseaseBolt*) asm("getIcon__16SkillDiseaseBolt");
extern "C" void* get_icon(const SkillDiseaseBolt*) {
    return lump_find_resource("icons.lmp", "icdiseasebolt.tex");
}
