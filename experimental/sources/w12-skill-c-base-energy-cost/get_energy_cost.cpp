// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313258 (16 bytes):
// return the IEEE-754 single value 1.0 for every integer argument.
struct Skill;
extern "C" float get_energy_cost(const Skill*, int) asm("getEnergyCost__5Skilli");
extern "C" float get_energy_cost(const Skill*, int) {
    return 1.0f;
}
