// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00317818 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icharmtouch.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillHarmTouch;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillHarmTouch*) asm("getIcon__14SkillHarmTouch");
extern "C" void* get_icon(const SkillHarmTouch*) {
    return lump_find_resource("icons.lmp", "icharmtouch.tex");
}
