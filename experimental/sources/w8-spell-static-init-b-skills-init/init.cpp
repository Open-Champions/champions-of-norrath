// _GLOBAL_$I$skills at 0x15eb60, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x15e1b8, 1868 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff. Then, each step only when the first argument is 1, it walks the 61
// skill slots (slot k is the k-th 104-byte object from 0x602908) in order: 52 slots get a
// constructor call taking only the slot address, and the other 9 (slots 2, 24, 25, 26, 27, 28, 29, 59, 60)
// get five stores: pointer to a name string at 0, zero at 4, a small word at 8, the halfword 30
// at 12, and at 96 the address of a `_vt$...` symbol. The strings and words are the image's.
// Inferred: the stores are an inlined base-class construction of the classes whose
// virtual tables they name; the slot order follows the construction order.
// Unknown: what the stored words and the halfword mean.
// Fresh compilation is not an original-byte match; nothing here has run on the EE.
#include "../w8-spell-static-init-b-skills-table/slot.hpp"

extern "C" {
void __15SkillWizardBeam(void* self);
void __10SkillDodge(void* self);
void __16SkillDiseaseBolt(void* self);
void __18SkillDiseaseShield(void* self);
void __17SkillDiseaseTrail(void* self);
void __14SkillFireStorm(void* self);
void __15SkillFireFlares(void* self);
void __12SkillRiposte(void* self);
void __12SkillBlessed(void* self);
void __19SkillSummonSkeleton(void* self);
void __21SkillLightningWeapons(void* self);
void __16SkillFireWeapons(void* self);
void __16SkillColdWeapons(void* self);
void __17SkillUndeadShield(void* self);
void __15SkillHolyStrike(void* self);
void __18SkillHammerOfWrath(void* self);
void __15SkillConeOfFire(void* self);
void __17SkillRegeneration(void* self);
void __18SkillBlindingLight(void* self);
void __18SkillRepulseUndead(void* self);
void __18SkillDismissUndead(void* self);
void __17SkillShockOfFrost(void* self);
void __15SkillPoisonRain(void* self);
void __15SkillClericRoot(void* self);
void __13SkillEntangle(void* self);
void __22SkillRangerCriticalHit(void* self);
void __15SkillShieldBash(void* self);
void __11SkillCharge(void* self);
void __13SkillDarkness(void* self);
void __10SkillShock(void* self);
void __16SkillGroundPound(void* self);
void __17SkillConvertEnemy(void* self);
void __18SkillConvertUndead(void* self);
void __12SkillCyclone(void* self);
void __14SkillHarmTouch(void* self);
void __14SkillFireArrow(void* self);
void __14SkillColdArrow(void* self);
void __16SkillPoisonArrow(void* self);
void __14SkillMultiFire(void* self);
void __17SkillExplodeArrow(void* self);
void __12SkillArchery(void* self);
void __9SkillBash(void* self);
void __17SkillMinorHealing(void* self);
void __18SkillAncestralCall(void* self);
void __9SkillRoot(void* self);
void __16SkillCriticalHit(void* self);
void __15SkillHolyShield(void* self);
void __12SkillHealing(void* self);
void __16SkillConeOfFrost(void* self);
void __16SkillShockOfFire(void* self);
void __15SkillFrostStorm(void* self);
void __12SkillLifeTap(void* self);

extern unsigned char _vt$17SkillResistPoison[];
extern unsigned char _vt$19SkillDiseaseWeapons[];
extern unsigned char _vt$18SkillPoisonWeapons[];
extern unsigned char _vt$13SkillHateBeam[];
extern unsigned char _vt$15SkillBallOfHate[];
extern unsigned char _vt$18SkillDiseaseEffect[];
extern unsigned char _vt$15SkillHateEffect[];
extern unsigned char _vt$10SkillBlunt[];
extern unsigned char _vt$13SkillSlashing[];

static void norrath_construct_inline(norrath_skill_slot& slot, const char* name, unsigned word, const void* table) {
    slot.table_pointer = table;
    slot.fourth_half = 30;
    slot.first_pointer = name;
    slot.third_word = word;
    slot.second_word = 0;
}

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1) __15SkillWizardBeam(&norrath_skill_slots[0]);
    if (initialize_p == 1) __10SkillDodge(&norrath_skill_slots[1]);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[2], "resistpoison", 3, _vt$17SkillResistPoison);
    if (initialize_p == 1) __16SkillDiseaseBolt(&norrath_skill_slots[3]);
    if (initialize_p == 1) __18SkillDiseaseShield(&norrath_skill_slots[4]);
    if (initialize_p == 1) __17SkillDiseaseTrail(&norrath_skill_slots[5]);
    if (initialize_p == 1) __14SkillFireStorm(&norrath_skill_slots[6]);
    if (initialize_p == 1) __15SkillFireFlares(&norrath_skill_slots[7]);
    if (initialize_p == 1) __12SkillRiposte(&norrath_skill_slots[8]);
    if (initialize_p == 1) __12SkillBlessed(&norrath_skill_slots[9]);
    if (initialize_p == 1) __19SkillSummonSkeleton(&norrath_skill_slots[10]);
    if (initialize_p == 1) __21SkillLightningWeapons(&norrath_skill_slots[11]);
    if (initialize_p == 1) __16SkillFireWeapons(&norrath_skill_slots[12]);
    if (initialize_p == 1) __16SkillColdWeapons(&norrath_skill_slots[13]);
    if (initialize_p == 1) __17SkillUndeadShield(&norrath_skill_slots[14]);
    if (initialize_p == 1) __15SkillHolyStrike(&norrath_skill_slots[15]);
    if (initialize_p == 1) __18SkillHammerOfWrath(&norrath_skill_slots[16]);
    if (initialize_p == 1) __15SkillConeOfFire(&norrath_skill_slots[17]);
    if (initialize_p == 1) __17SkillRegeneration(&norrath_skill_slots[18]);
    if (initialize_p == 1) __18SkillBlindingLight(&norrath_skill_slots[19]);
    if (initialize_p == 1) __18SkillRepulseUndead(&norrath_skill_slots[20]);
    if (initialize_p == 1) __18SkillDismissUndead(&norrath_skill_slots[21]);
    if (initialize_p == 1) __17SkillShockOfFrost(&norrath_skill_slots[22]);
    if (initialize_p == 1) __15SkillPoisonRain(&norrath_skill_slots[23]);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[24], "diseaseweapons", 0, _vt$19SkillDiseaseWeapons);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[25], "poisonweapons", 0, _vt$18SkillPoisonWeapons);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[26], "hatebeam", 1, _vt$13SkillHateBeam);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[27], "hatebeam", 1, _vt$15SkillBallOfHate);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[28], "diseaseeffect", 1, _vt$18SkillDiseaseEffect);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[29], "hateeffect", 1, _vt$15SkillHateEffect);
    if (initialize_p == 1) __15SkillClericRoot(&norrath_skill_slots[30]);
    if (initialize_p == 1) __13SkillEntangle(&norrath_skill_slots[31]);
    if (initialize_p == 1) __22SkillRangerCriticalHit(&norrath_skill_slots[32]);
    if (initialize_p == 1) __15SkillShieldBash(&norrath_skill_slots[33]);
    if (initialize_p == 1) __11SkillCharge(&norrath_skill_slots[34]);
    if (initialize_p == 1) __13SkillDarkness(&norrath_skill_slots[35]);
    if (initialize_p == 1) __10SkillShock(&norrath_skill_slots[36]);
    if (initialize_p == 1) __16SkillGroundPound(&norrath_skill_slots[37]);
    if (initialize_p == 1) __17SkillConvertEnemy(&norrath_skill_slots[38]);
    if (initialize_p == 1) __18SkillConvertUndead(&norrath_skill_slots[39]);
    if (initialize_p == 1) __12SkillCyclone(&norrath_skill_slots[40]);
    if (initialize_p == 1) __14SkillHarmTouch(&norrath_skill_slots[41]);
    if (initialize_p == 1) __14SkillFireArrow(&norrath_skill_slots[42]);
    if (initialize_p == 1) __14SkillColdArrow(&norrath_skill_slots[43]);
    if (initialize_p == 1) __16SkillPoisonArrow(&norrath_skill_slots[44]);
    if (initialize_p == 1) __14SkillMultiFire(&norrath_skill_slots[45]);
    if (initialize_p == 1) __17SkillExplodeArrow(&norrath_skill_slots[46]);
    if (initialize_p == 1) __12SkillArchery(&norrath_skill_slots[47]);
    if (initialize_p == 1) __9SkillBash(&norrath_skill_slots[48]);
    if (initialize_p == 1) __17SkillMinorHealing(&norrath_skill_slots[49]);
    if (initialize_p == 1) __18SkillAncestralCall(&norrath_skill_slots[50]);
    if (initialize_p == 1) __9SkillRoot(&norrath_skill_slots[51]);
    if (initialize_p == 1) __16SkillCriticalHit(&norrath_skill_slots[52]);
    if (initialize_p == 1) __15SkillHolyShield(&norrath_skill_slots[53]);
    if (initialize_p == 1) __12SkillHealing(&norrath_skill_slots[54]);
    if (initialize_p == 1) __16SkillConeOfFrost(&norrath_skill_slots[55]);
    if (initialize_p == 1) __16SkillShockOfFire(&norrath_skill_slots[56]);
    if (initialize_p == 1) __15SkillFrostStorm(&norrath_skill_slots[57]);
    if (initialize_p == 1) __12SkillLifeTap(&norrath_skill_slots[58]);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[59], "Blunt", 3, _vt$10SkillBlunt);
    if (initialize_p == 1) norrath_construct_inline(norrath_skill_slots[60], "Slashing", 3, _vt$13SkillSlashing);
}

void _GLOBAL_$I$skills() { norrath_static_init(1, 0xffff); }
}
