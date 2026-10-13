// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313250 (8 bytes):
// return zero for every integer argument; neither input is read.
struct Skill;
extern "C" int get_level_requirement(const Skill*, int) asm("getLevelRequirement__5Skilli");
extern "C" int get_level_requirement(const Skill*, int) {
    return 0;
}
