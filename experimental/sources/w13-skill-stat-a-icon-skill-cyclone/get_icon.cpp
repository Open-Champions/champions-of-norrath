// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003176a8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccycloneattack.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillCyclone;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillCyclone*) asm("getIcon__12SkillCyclone");
extern "C" void* get_icon(const SkillCyclone*) {
    return lump_find_resource("icons.lmp", "iccycloneattack.tex");
}
