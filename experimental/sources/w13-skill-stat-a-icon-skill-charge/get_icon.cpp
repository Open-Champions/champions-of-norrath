// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00316bd8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iccharge.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillCharge;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillCharge*) asm("getIcon__11SkillCharge");
extern "C" void* get_icon(const SkillCharge*) {
    return lump_find_resource("icons.lmp", "iccharge.tex");
}
