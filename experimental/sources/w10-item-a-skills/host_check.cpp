// Host check for _GLOBAL_$I$skills and the skills table. Not an EE plan input.
#include "skills.cpp"
#include <cstring>
#include <string>
static std::string order;
extern "C" {
unsigned char _vt$10SkillBlunt[8];
unsigned char _vt$13SkillHateBeam[8];
unsigned char _vt$13SkillSlashing[8];
unsigned char _vt$15SkillBallOfHate[8];
unsigned char _vt$15SkillHateEffect[8];
unsigned char _vt$17SkillResistPoison[8];
unsigned char _vt$18SkillDiseaseEffect[8];
unsigned char _vt$18SkillPoisonWeapons[8];
unsigned char _vt$19SkillDiseaseWeapons[8];
void __15SkillWizardBeam(void* self) { order += "__15SkillWizardBeam@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __10SkillDodge(void* self) { order += "__10SkillDodge@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillDiseaseBolt(void* self) { order += "__16SkillDiseaseBolt@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillDiseaseShield(void* self) { order += "__18SkillDiseaseShield@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillDiseaseTrail(void* self) { order += "__17SkillDiseaseTrail@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __14SkillFireStorm(void* self) { order += "__14SkillFireStorm@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillFireFlares(void* self) { order += "__15SkillFireFlares@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillRiposte(void* self) { order += "__12SkillRiposte@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillBlessed(void* self) { order += "__12SkillBlessed@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __19SkillSummonSkeleton(void* self) { order += "__19SkillSummonSkeleton@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __21SkillLightningWeapons(void* self) { order += "__21SkillLightningWeapons@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillFireWeapons(void* self) { order += "__16SkillFireWeapons@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillColdWeapons(void* self) { order += "__16SkillColdWeapons@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillUndeadShield(void* self) { order += "__17SkillUndeadShield@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillHolyStrike(void* self) { order += "__15SkillHolyStrike@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillHammerOfWrath(void* self) { order += "__18SkillHammerOfWrath@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillConeOfFire(void* self) { order += "__15SkillConeOfFire@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillRegeneration(void* self) { order += "__17SkillRegeneration@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillBlindingLight(void* self) { order += "__18SkillBlindingLight@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillRepulseUndead(void* self) { order += "__18SkillRepulseUndead@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillDismissUndead(void* self) { order += "__18SkillDismissUndead@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillShockOfFrost(void* self) { order += "__17SkillShockOfFrost@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillPoisonRain(void* self) { order += "__15SkillPoisonRain@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillClericRoot(void* self) { order += "__15SkillClericRoot@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __13SkillEntangle(void* self) { order += "__13SkillEntangle@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __22SkillRangerCriticalHit(void* self) { order += "__22SkillRangerCriticalHit@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillShieldBash(void* self) { order += "__15SkillShieldBash@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __11SkillCharge(void* self) { order += "__11SkillCharge@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __13SkillDarkness(void* self) { order += "__13SkillDarkness@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __10SkillShock(void* self) { order += "__10SkillShock@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillGroundPound(void* self) { order += "__16SkillGroundPound@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillConvertEnemy(void* self) { order += "__17SkillConvertEnemy@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillConvertUndead(void* self) { order += "__18SkillConvertUndead@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillCyclone(void* self) { order += "__12SkillCyclone@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __14SkillHarmTouch(void* self) { order += "__14SkillHarmTouch@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __14SkillFireArrow(void* self) { order += "__14SkillFireArrow@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __14SkillColdArrow(void* self) { order += "__14SkillColdArrow@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillPoisonArrow(void* self) { order += "__16SkillPoisonArrow@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __14SkillMultiFire(void* self) { order += "__14SkillMultiFire@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillExplodeArrow(void* self) { order += "__17SkillExplodeArrow@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillArchery(void* self) { order += "__12SkillArchery@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __9SkillBash(void* self) { order += "__9SkillBash@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __17SkillMinorHealing(void* self) { order += "__17SkillMinorHealing@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __18SkillAncestralCall(void* self) { order += "__18SkillAncestralCall@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __9SkillRoot(void* self) { order += "__9SkillRoot@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillCriticalHit(void* self) { order += "__16SkillCriticalHit@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillHolyShield(void* self) { order += "__15SkillHolyShield@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillHealing(void* self) { order += "__12SkillHealing@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillConeOfFrost(void* self) { order += "__16SkillConeOfFrost@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __16SkillShockOfFire(void* self) { order += "__16SkillShockOfFire@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __15SkillFrostStorm(void* self) { order += "__15SkillFrostStorm@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
void __12SkillLifeTap(void* self) { order += "__12SkillLifeTap@" + std::to_string((static_cast<SkillStorage*>(self) - skillStorage)) + ";"; }
}
int main() {
    _GLOBAL_$I$skills();
    // 52 constructor calls in order, each on its own slot; the inline slots are skipped
    int calls = 0;
    for (size_t at = order.find(';'); at != std::string::npos; at = order.find(';', at + 1)) calls++;
    if (calls != 52) return 1;
    if (order.rfind("__15SkillWizardBeam@0;__10SkillDodge@1;__16SkillDiseaseBolt@3;", 0) != 0) return 2;
    if (order.find("__15SkillPoisonRain@23;__15SkillClericRoot@30;") == std::string::npos) return 3;
    if (order.substr(order.size() - 20) != "__12SkillLifeTap@58;") return 4;
    struct Expect { int slot; const char* name; const void* table; int value; };
    const Expect inlineSlots[] = {
        {2, "resistpoison", _vt$17SkillResistPoison, 3}, {24, "diseaseweapons", _vt$19SkillDiseaseWeapons, 0},
        {25, "poisonweapons", _vt$18SkillPoisonWeapons, 0}, {26, "hatebeam", _vt$13SkillHateBeam, 1},
        {27, "hatebeam", _vt$15SkillBallOfHate, 1}, {28, "diseaseeffect", _vt$18SkillDiseaseEffect, 1},
        {29, "hateeffect", _vt$15SkillHateEffect, 1}, {59, "Blunt", _vt$10SkillBlunt, 3},
        {60, "Slashing", _vt$13SkillSlashing, 3}};
    for (const Expect& e : inlineSlots) {
        unsigned char* b = skillStorage[e.slot].bytes;
        // pointer words are 32-bit in the image, so the host only sees their low halves
        if (*reinterpret_cast<unsigned*>(b) == 0) return 10 + e.slot;
        if (*reinterpret_cast<int*>(b + 4) != 0 || *reinterpret_cast<int*>(b + 8) != e.value || *reinterpret_cast<short*>(b + 12) != 30) return 20 + e.slot;
        if (*reinterpret_cast<unsigned*>(b + 96) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(e.table))) return 30 + e.slot;
    }
    // the table: null, the 61 objects in order, null
    if (skills[0] != nullptr || skills[62] != nullptr) return 5;
    for (int i = 0; i < 61; i++) if (skills[i + 1] != &skillStorage[i]) return 6;
    return 0;
}
