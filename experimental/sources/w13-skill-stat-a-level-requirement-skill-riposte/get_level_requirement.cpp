// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a5110 (8 bytes):
// return the constant 12 for every rank. The receiver and the rank are not read.
struct SkillRiposte;
extern "C" int get_level_requirement(const SkillRiposte*, int) asm("getLevelRequirement__12SkillRipostei");
extern "C" int get_level_requirement(const SkillRiposte*, int) {
    return 12;
}
