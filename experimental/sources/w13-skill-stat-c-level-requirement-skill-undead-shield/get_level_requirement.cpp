// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b13d8 (8 bytes):
// return the constant 10 for every rank. The receiver and the rank are not read.
struct SkillUndeadShield;
extern "C" int get_level_requirement(const SkillUndeadShield*, int) asm("getLevelRequirement__17SkillUndeadShieldi");
extern "C" int get_level_requirement(const SkillUndeadShield*, int) {
    return 10;
}
