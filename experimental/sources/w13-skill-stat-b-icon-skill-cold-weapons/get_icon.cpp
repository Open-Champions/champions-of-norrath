// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318d98 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccoldweapons.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillColdWeapons;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillColdWeapons*) asm("getIcon__16SkillColdWeapons");
extern "C" void* get_icon(const SkillColdWeapons*) {
    return lump_find_resource("icons.lmp", "iccoldweapons.tex");
}
