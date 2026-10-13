// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003199d8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icconeoffire.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillConeOfFire;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillConeOfFire*) asm("getIcon__15SkillConeOfFire");
extern "C" void* get_icon(const SkillConeOfFire*) {
    return lump_find_resource("icons.lmp", "icconeoffire.tex");
}
