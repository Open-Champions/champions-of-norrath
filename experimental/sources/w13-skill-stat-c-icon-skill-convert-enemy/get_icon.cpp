// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003174d0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icconvertenemy.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillConvertEnemy;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillConvertEnemy*) asm("getIcon__17SkillConvertEnemy");
extern "C" void* get_icon(const SkillConvertEnemy*) {
    return lump_find_resource("icons.lmp", "icconvertenemy.tex");
}
