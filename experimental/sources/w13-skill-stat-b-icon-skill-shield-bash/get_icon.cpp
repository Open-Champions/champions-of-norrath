// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0031a790 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icbash.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillShieldBash;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillShieldBash*) asm("getIcon__15SkillShieldBash");
extern "C" void* get_icon(const SkillShieldBash*) {
    return lump_find_resource("icons.lmp", "icbash.tex");
}
