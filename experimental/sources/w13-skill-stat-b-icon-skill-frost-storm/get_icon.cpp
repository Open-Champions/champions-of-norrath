// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00319740 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icfroststorm.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillFrostStorm;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillFrostStorm*) asm("getIcon__15SkillFrostStorm");
extern "C" void* get_icon(const SkillFrostStorm*) {
    return lump_find_resource("icons.lmp", "icfroststorm.tex");
}
