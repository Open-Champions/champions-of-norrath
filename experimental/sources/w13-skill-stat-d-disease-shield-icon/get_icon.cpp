// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003170a0 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icdiseaseshield.tex") and return its result. The receiver is not read.
struct SkillDiseaseShield;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillDiseaseShield*) asm("getIcon__18SkillDiseaseShield");
extern "C" void* get_icon(const SkillDiseaseShield*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("icdiseaseshield.tex"));
}
