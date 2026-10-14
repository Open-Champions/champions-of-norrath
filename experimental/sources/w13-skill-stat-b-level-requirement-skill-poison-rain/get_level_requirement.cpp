// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bf238 (8 bytes):
// return the constant 20 for every rank. The receiver and the rank are not read.
struct SkillPoisonRain;
extern "C" int get_level_requirement(const SkillPoisonRain*, int) asm("getLevelRequirement__15SkillPoisonRaini");
extern "C" int get_level_requirement(const SkillPoisonRain*, int) {
    return 20;
}
