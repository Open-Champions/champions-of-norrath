// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null slots observed in the image.
#include "vt_skillresistpoison.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf17SkillResistPoison"; }
extern "C" void slot_fn_1() { hit = "canCast__5SkillP8Creature"; }
extern "C" void slot_fn_2() { hit = "cast__5SkilliP8CreatureG6Point3s"; }
extern "C" void slot_fn_3() { hit = "piggyBackCast__5SkilliP8CreatureG6Point3s"; }
extern "C" void slot_fn_4() { hit = "getRankDescription__17SkillResistPoisonPUsi"; }
extern "C" void slot_fn_5() { hit = "getSkillPointCost__17SkillResistPoisoni"; }
extern "C" void slot_fn_6() { hit = "getLevelRequirement__17SkillResistPoisoni"; }
extern "C" void slot_fn_7() { hit = "getEnergyCost__5Skilli"; }
extern "C" void slot_fn_8() { hit = "getIcon__5Skill"; }
extern "C" void slot_fn_9() { hit = "getRampDamage__5SkilliifiPiT5"; }
extern "C" void slot_fn_10() { hit = "initRamps__5Skill"; }
extern "C" void slot_fn_11() { hit = "evaluateRampAtRank__5Skillii"; }
extern "C" void slot_fn_12() { hit = "evaluateCreatureRampAtRank__5Skillii"; }
extern "C" void slot_fn_13() { hit = "evaluateRampAtRank__5SkilliiP10GameObject"; }
extern "C" void slot_fn_14() { hit = "evaluateRampAtRank__5SkilliiP8ListHead"; }
extern "C" void slot_fn_15() { hit = "tweekRank__5Skillii"; }
extern "C" void slot_fn_16() { hit = "energyCostRamp4__5Skillii"; }

static const char* const expected[18] = {
    "__tf17SkillResistPoison",
    "canCast__5SkillP8Creature",
    "cast__5SkilliP8CreatureG6Point3s",
    "piggyBackCast__5SkilliP8CreatureG6Point3s",
    "getRankDescription__17SkillResistPoisonPUsi",
    "getSkillPointCost__17SkillResistPoisoni",
    "getLevelRequirement__17SkillResistPoisoni",
    "getEnergyCost__5Skilli",
    "getIcon__5Skill",
    "getRampDamage__5SkilliifiPiT5",
    "initRamps__5Skill",
    "evaluateRampAtRank__5Skillii",
    "evaluateCreatureRampAtRank__5Skillii",
    "evaluateRampAtRank__5SkilliiP10GameObject",
    "evaluateRampAtRank__5SkilliiP8ListHead",
    "tweekRank__5Skillii",
    "energyCostRamp4__5Skillii",
    nullptr,
};

int main() {
    if (sizeof(_vt_17SkillResistPoison) / sizeof(_vt_17SkillResistPoison[0]) != 18) return 1;
    for (int i = 0; i < 18; ++i) {
        if (_vt_17SkillResistPoison[i].adjustment != 0) return 2;
        if (expected[i] == nullptr) {
            if (_vt_17SkillResistPoison[i].function != nullptr) return 4;
            continue;
        }
        hit = "";
        _vt_17SkillResistPoison[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 3;
    }
    return 0;
}
