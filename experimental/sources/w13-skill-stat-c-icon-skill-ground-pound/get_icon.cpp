// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00316af0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icgroundpound.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillGroundPound;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillGroundPound*) asm("getIcon__16SkillGroundPound");
extern "C" void* get_icon(const SkillGroundPound*) {
    return lump_find_resource("icons.lmp", "icgroundpound.tex");
}
