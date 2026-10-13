// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318ed8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccriticalhit.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillCriticalHit;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillCriticalHit*) asm("getIcon__16SkillCriticalHit");
extern "C" void* get_icon(const SkillCriticalHit*) {
    return lump_find_resource("icons.lmp", "iccriticalhit.tex");
}
