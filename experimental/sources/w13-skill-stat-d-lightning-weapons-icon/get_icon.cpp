// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00318b60 (36 bytes):
// tail-call lumpFindResource("icons.lmp", "iclightningweapons.tex") and return its result. The receiver is not read.
struct SkillLightningWeapons;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillLightningWeapons*) asm("getIcon__21SkillLightningWeapons");
extern "C" void* get_icon(const SkillLightningWeapons*) {
    return lumpFindResource__FPcT0(const_cast<char*>("icons.lmp"), const_cast<char*>("iclightningweapons.tex"));
}
