// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0031a560 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icroot.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillEntangle;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillEntangle*) asm("getIcon__13SkillEntangle");
extern "C" void* get_icon(const SkillEntangle*) {
    return lump_find_resource("icons.lmp", "icroot.tex");
}
