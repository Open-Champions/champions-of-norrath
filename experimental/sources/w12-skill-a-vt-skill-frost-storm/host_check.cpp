// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_skill_frost_storm.cpp
// Every slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstddef>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void vt_fn_0() asm("__tf15SkillFrostStorm");
extern "C" void vt_fn_0() { hit = "__tf15SkillFrostStorm"; }
extern "C" void vt_fn_1() asm("canCast__5SkillP8Creature");
extern "C" void vt_fn_1() { hit = "canCast__5SkillP8Creature"; }
extern "C" void vt_fn_2() asm("cast__15SkillFrostStormiP8CreatureG6Point3s");
extern "C" void vt_fn_2() { hit = "cast__15SkillFrostStormiP8CreatureG6Point3s"; }
extern "C" void vt_fn_3() asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
extern "C" void vt_fn_3() { hit = "piggyBackCast__5SkilliP8CreatureG6Point3s"; }
extern "C" void vt_fn_4() asm("getRankDescription__15SkillFrostStormPUsi");
extern "C" void vt_fn_4() { hit = "getRankDescription__15SkillFrostStormPUsi"; }
extern "C" void vt_fn_5() asm("getSkillPointCost__5Skilli");
extern "C" void vt_fn_5() { hit = "getSkillPointCost__5Skilli"; }
extern "C" void vt_fn_6() asm("getLevelRequirement__5Skilli");
extern "C" void vt_fn_6() { hit = "getLevelRequirement__5Skilli"; }
extern "C" void vt_fn_7() asm("getEnergyCost__15SkillFrostStormi");
extern "C" void vt_fn_7() { hit = "getEnergyCost__15SkillFrostStormi"; }
extern "C" void vt_fn_8() asm("getIcon__15SkillFrostStorm");
extern "C" void vt_fn_8() { hit = "getIcon__15SkillFrostStorm"; }
extern "C" void vt_fn_9() asm("getRampDamage__5SkilliifiPiT5");
extern "C" void vt_fn_9() { hit = "getRampDamage__5SkilliifiPiT5"; }
extern "C" void vt_fn_10() asm("initRamps__15SkillFrostStorm");
extern "C" void vt_fn_10() { hit = "initRamps__15SkillFrostStorm"; }
extern "C" void vt_fn_11() asm("evaluateRampAtRank__5Skillii");
extern "C" void vt_fn_11() { hit = "evaluateRampAtRank__5Skillii"; }
extern "C" void vt_fn_12() asm("evaluateCreatureRampAtRank__5Skillii");
extern "C" void vt_fn_12() { hit = "evaluateCreatureRampAtRank__5Skillii"; }
extern "C" void vt_fn_13() asm("evaluateRampAtRank__5SkilliiP10GameObject");
extern "C" void vt_fn_13() { hit = "evaluateRampAtRank__5SkilliiP10GameObject"; }
extern "C" void vt_fn_14() asm("evaluateRampAtRank__5SkilliiP8ListHead");
extern "C" void vt_fn_14() { hit = "evaluateRampAtRank__5SkilliiP8ListHead"; }
extern "C" void vt_fn_15() asm("tweekRank__5Skillii");
extern "C" void vt_fn_15() { hit = "tweekRank__5Skillii"; }
extern "C" void vt_fn_16() asm("energyCostRamp4__5Skillii");
extern "C" void vt_fn_16() { hit = "energyCostRamp4__5Skillii"; }
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_skill_frost_storm[18] asm("_vt$15SkillFrostStorm");

int main() {
    static const char* const want[17] = {
    "__tf15SkillFrostStorm",
    "canCast__5SkillP8Creature",
    "cast__15SkillFrostStormiP8CreatureG6Point3s",
    "piggyBackCast__5SkilliP8CreatureG6Point3s",
    "getRankDescription__15SkillFrostStormPUsi",
    "getSkillPointCost__5Skilli",
    "getLevelRequirement__5Skilli",
    "getEnergyCost__15SkillFrostStormi",
    "getIcon__15SkillFrostStorm",
    "getRampDamage__5SkilliifiPiT5",
    "initRamps__15SkillFrostStorm",
    "evaluateRampAtRank__5Skillii",
    "evaluateCreatureRampAtRank__5Skillii",
    "evaluateRampAtRank__5SkilliiP10GameObject",
    "evaluateRampAtRank__5SkilliiP8ListHead",
    "tweekRank__5Skillii",
    "energyCostRamp4__5Skillii",
    };
    if (sizeof(vt_skill_frost_storm) != 18 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 17; ++i) {
        if (vt_skill_frost_storm[i].adjustment != 0) return 2;
        hit = "";
        vt_skill_frost_storm[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_skill_frost_storm[17].adjustment != 0 || vt_skill_frost_storm[17].function != nullptr) return 4;
    std::printf("ok 15SkillFrostStorm 18 slots\n");
    return 0;
}
