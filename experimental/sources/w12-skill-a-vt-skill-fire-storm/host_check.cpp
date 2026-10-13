// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_skill_fire_storm.cpp
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
extern "C" void vt_fn_0() asm("__tf14SkillFireStorm");
extern "C" void vt_fn_0() { hit = "__tf14SkillFireStorm"; }
extern "C" void vt_fn_1() asm("canCast__14SkillFireStormP8Creature");
extern "C" void vt_fn_1() { hit = "canCast__14SkillFireStormP8Creature"; }
extern "C" void vt_fn_2() asm("cast__14SkillFireStormiP8CreatureG6Point3s");
extern "C" void vt_fn_2() { hit = "cast__14SkillFireStormiP8CreatureG6Point3s"; }
extern "C" void vt_fn_3() asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
extern "C" void vt_fn_3() { hit = "piggyBackCast__5SkilliP8CreatureG6Point3s"; }
extern "C" void vt_fn_4() asm("getRankDescription__14SkillFireStormPUsi");
extern "C" void vt_fn_4() { hit = "getRankDescription__14SkillFireStormPUsi"; }
extern "C" void vt_fn_5() asm("getSkillPointCost__14SkillFireStormi");
extern "C" void vt_fn_5() { hit = "getSkillPointCost__14SkillFireStormi"; }
extern "C" void vt_fn_6() asm("getLevelRequirement__14SkillFireStormi");
extern "C" void vt_fn_6() { hit = "getLevelRequirement__14SkillFireStormi"; }
extern "C" void vt_fn_7() asm("getEnergyCost__14SkillFireStormi");
extern "C" void vt_fn_7() { hit = "getEnergyCost__14SkillFireStormi"; }
extern "C" void vt_fn_8() asm("getIcon__14SkillFireStorm");
extern "C" void vt_fn_8() { hit = "getIcon__14SkillFireStorm"; }
extern "C" void vt_fn_9() asm("getRampDamage__5SkilliifiPiT5");
extern "C" void vt_fn_9() { hit = "getRampDamage__5SkilliifiPiT5"; }
extern "C" void vt_fn_10() asm("initRamps__14SkillFireStorm");
extern "C" void vt_fn_10() { hit = "initRamps__14SkillFireStorm"; }
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
extern "C" VtSlot vt_skill_fire_storm[18] asm("_vt$14SkillFireStorm");

int main() {
    static const char* const want[17] = {
    "__tf14SkillFireStorm",
    "canCast__14SkillFireStormP8Creature",
    "cast__14SkillFireStormiP8CreatureG6Point3s",
    "piggyBackCast__5SkilliP8CreatureG6Point3s",
    "getRankDescription__14SkillFireStormPUsi",
    "getSkillPointCost__14SkillFireStormi",
    "getLevelRequirement__14SkillFireStormi",
    "getEnergyCost__14SkillFireStormi",
    "getIcon__14SkillFireStorm",
    "getRampDamage__5SkilliifiPiT5",
    "initRamps__14SkillFireStorm",
    "evaluateRampAtRank__5Skillii",
    "evaluateCreatureRampAtRank__5Skillii",
    "evaluateRampAtRank__5SkilliiP10GameObject",
    "evaluateRampAtRank__5SkilliiP8ListHead",
    "tweekRank__5Skillii",
    "energyCostRamp4__5Skillii",
    };
    if (sizeof(vt_skill_fire_storm) != 18 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 17; ++i) {
        if (vt_skill_fire_storm[i].adjustment != 0) return 2;
        hit = "";
        vt_skill_fire_storm[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_skill_fire_storm[17].adjustment != 0 || vt_skill_fire_storm[17].function != nullptr) return 4;
    std::printf("ok 14SkillFireStorm 18 slots\n");
    return 0;
}
