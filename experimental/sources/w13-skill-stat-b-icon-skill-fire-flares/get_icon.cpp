// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003182d0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icfireflares.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillFireFlares;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillFireFlares*) asm("getIcon__15SkillFireFlares");
extern "C" void* get_icon(const SkillFireFlares*) {
    return lump_find_resource("icons.lmp", "icfireflares.tex");
}
