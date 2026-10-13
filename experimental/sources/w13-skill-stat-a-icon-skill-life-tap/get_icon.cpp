// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0031a000 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iclifetap.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillLifeTap;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillLifeTap*) asm("getIcon__12SkillLifeTap");
extern "C" void* get_icon(const SkillLifeTap*) {
    return lump_find_resource("icons.lmp", "iclifetap.tex");
}
