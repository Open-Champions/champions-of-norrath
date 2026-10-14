// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00319b10 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "icwizardbeam.tex") and return its result.
// The receiver is not read. What the returned record is, is not established here.
struct SkillWizardBeam;
extern "C" void* lump_find_resource(const char*, const char*) asm("lumpFindResource__FPcT0");
extern "C" void* get_icon(const SkillWizardBeam*) asm("getIcon__15SkillWizardBeam");
extern "C" void* get_icon(const SkillWizardBeam*) {
    return lump_find_resource("icons.lmp", "icwizardbeam.tex");
}
