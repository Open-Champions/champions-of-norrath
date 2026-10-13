// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00316a78 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icshock.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillShock;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillShock*) asm("getIcon__10SkillShock");
extern "C" void* get_icon(const SkillShock*) {
    return lump_find_resource("icons.lmp", "icshock.tex");
}
