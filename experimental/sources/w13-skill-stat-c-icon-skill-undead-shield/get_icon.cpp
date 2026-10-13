// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003194f8 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icundeadshield.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillUndeadShield;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillUndeadShield*) asm("getIcon__17SkillUndeadShield");
extern "C" void* get_icon(const SkillUndeadShield*) {
    return lump_find_resource("icons.lmp", "icundeadshield.tex");
}
