// Observed _GLOBAL_$I$skills at 0x0015eb60, 32 bytes, and the unit-local static
// initializer it calls with (1, 0xffff) (0x0015e1b8, 1868 bytes). When the first
// argument is 1 and the second is 0xffff, 61 consecutive 104-byte objects
// (0x00602908..0x006041d0) are constructed in this order: 52 by their class
// constructors (which write the name, table pointer at +96 and fields themselves)
// and 9 whose constructor the compiler inlined: it stores the class name text at
// +0, 0 at +4, a value at +8, the halfword 30 at +12 and the class's table pointer
// at +96 (Blunt, Slashing and ResistPoison use 3, the two weapon-enchant classes
// 0, the effect/hate classes 1). With any other arguments nothing happens.
// The pointer table `skills` (0x0034b440, 63 words) holds a null, the addresses of
// the 61 objects in construction order, and a null. The image keeps the objects as
// separate unit-local statics; here they are one array in the same order.
// The classes and ctor order below come from the constructor symbols in the image.
// Unknown: what the "skill" objects are used for beyond the table, the meaning of
// the value and halfword fields, and the exact priority scheme of the original
// (only priority 0xffff does anything).
extern "C" {
struct SkillStorage { unsigned char bytes[104]; };

void __15SkillWizardBeam(void*);
void __10SkillDodge(void*);
void __16SkillDiseaseBolt(void*);
void __18SkillDiseaseShield(void*);
void __17SkillDiseaseTrail(void*);
void __14SkillFireStorm(void*);
void __15SkillFireFlares(void*);
void __12SkillRiposte(void*);
void __12SkillBlessed(void*);
void __19SkillSummonSkeleton(void*);
void __21SkillLightningWeapons(void*);
void __16SkillFireWeapons(void*);
void __16SkillColdWeapons(void*);
void __17SkillUndeadShield(void*);
void __15SkillHolyStrike(void*);
void __18SkillHammerOfWrath(void*);
void __15SkillConeOfFire(void*);
void __17SkillRegeneration(void*);
void __18SkillBlindingLight(void*);
void __18SkillRepulseUndead(void*);
void __18SkillDismissUndead(void*);
void __17SkillShockOfFrost(void*);
void __15SkillPoisonRain(void*);
void __15SkillClericRoot(void*);
void __13SkillEntangle(void*);
void __22SkillRangerCriticalHit(void*);
void __15SkillShieldBash(void*);
void __11SkillCharge(void*);
void __13SkillDarkness(void*);
void __10SkillShock(void*);
void __16SkillGroundPound(void*);
void __17SkillConvertEnemy(void*);
void __18SkillConvertUndead(void*);
void __12SkillCyclone(void*);
void __14SkillHarmTouch(void*);
void __14SkillFireArrow(void*);
void __14SkillColdArrow(void*);
void __16SkillPoisonArrow(void*);
void __14SkillMultiFire(void*);
void __17SkillExplodeArrow(void*);
void __12SkillArchery(void*);
void __9SkillBash(void*);
void __17SkillMinorHealing(void*);
void __18SkillAncestralCall(void*);
void __9SkillRoot(void*);
void __16SkillCriticalHit(void*);
void __15SkillHolyShield(void*);
void __12SkillHealing(void*);
void __16SkillConeOfFrost(void*);
void __16SkillShockOfFire(void*);
void __15SkillFrostStorm(void*);
void __12SkillLifeTap(void*);

extern unsigned char _vt$10SkillBlunt[];
extern unsigned char _vt$13SkillHateBeam[];
extern unsigned char _vt$13SkillSlashing[];
extern unsigned char _vt$15SkillBallOfHate[];
extern unsigned char _vt$15SkillHateEffect[];
extern unsigned char _vt$17SkillResistPoison[];
extern unsigned char _vt$18SkillDiseaseEffect[];
extern unsigned char _vt$18SkillPoisonWeapons[];
extern unsigned char _vt$19SkillDiseaseWeapons[];

static SkillStorage skillStorage[61];

static inline void inlineSkill(SkillStorage* object, const char* name, const void* table, int value) {
    unsigned char* b = object->bytes;
    *reinterpret_cast<unsigned*>(b + 96) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
    *reinterpret_cast<short*>(b + 12) = 30;
    *reinterpret_cast<unsigned*>(b + 0) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(name));
    *reinterpret_cast<int*>(b + 8) = value;
    *reinterpret_cast<int*>(b + 4) = 0;
}

static void static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize != 1) return;
    __15SkillWizardBeam(&skillStorage[0]);
    __10SkillDodge(&skillStorage[1]);
    inlineSkill(&skillStorage[2], "resistpoison", _vt$17SkillResistPoison, 3);
    __16SkillDiseaseBolt(&skillStorage[3]);
    __18SkillDiseaseShield(&skillStorage[4]);
    __17SkillDiseaseTrail(&skillStorage[5]);
    __14SkillFireStorm(&skillStorage[6]);
    __15SkillFireFlares(&skillStorage[7]);
    __12SkillRiposte(&skillStorage[8]);
    __12SkillBlessed(&skillStorage[9]);
    __19SkillSummonSkeleton(&skillStorage[10]);
    __21SkillLightningWeapons(&skillStorage[11]);
    __16SkillFireWeapons(&skillStorage[12]);
    __16SkillColdWeapons(&skillStorage[13]);
    __17SkillUndeadShield(&skillStorage[14]);
    __15SkillHolyStrike(&skillStorage[15]);
    __18SkillHammerOfWrath(&skillStorage[16]);
    __15SkillConeOfFire(&skillStorage[17]);
    __17SkillRegeneration(&skillStorage[18]);
    __18SkillBlindingLight(&skillStorage[19]);
    __18SkillRepulseUndead(&skillStorage[20]);
    __18SkillDismissUndead(&skillStorage[21]);
    __17SkillShockOfFrost(&skillStorage[22]);
    __15SkillPoisonRain(&skillStorage[23]);
    inlineSkill(&skillStorage[24], "diseaseweapons", _vt$19SkillDiseaseWeapons, 0);
    inlineSkill(&skillStorage[25], "poisonweapons", _vt$18SkillPoisonWeapons, 0);
    inlineSkill(&skillStorage[26], "hatebeam", _vt$13SkillHateBeam, 1);
    inlineSkill(&skillStorage[27], "hatebeam", _vt$15SkillBallOfHate, 1);
    inlineSkill(&skillStorage[28], "diseaseeffect", _vt$18SkillDiseaseEffect, 1);
    inlineSkill(&skillStorage[29], "hateeffect", _vt$15SkillHateEffect, 1);
    __15SkillClericRoot(&skillStorage[30]);
    __13SkillEntangle(&skillStorage[31]);
    __22SkillRangerCriticalHit(&skillStorage[32]);
    __15SkillShieldBash(&skillStorage[33]);
    __11SkillCharge(&skillStorage[34]);
    __13SkillDarkness(&skillStorage[35]);
    __10SkillShock(&skillStorage[36]);
    __16SkillGroundPound(&skillStorage[37]);
    __17SkillConvertEnemy(&skillStorage[38]);
    __18SkillConvertUndead(&skillStorage[39]);
    __12SkillCyclone(&skillStorage[40]);
    __14SkillHarmTouch(&skillStorage[41]);
    __14SkillFireArrow(&skillStorage[42]);
    __14SkillColdArrow(&skillStorage[43]);
    __16SkillPoisonArrow(&skillStorage[44]);
    __14SkillMultiFire(&skillStorage[45]);
    __17SkillExplodeArrow(&skillStorage[46]);
    __12SkillArchery(&skillStorage[47]);
    __9SkillBash(&skillStorage[48]);
    __17SkillMinorHealing(&skillStorage[49]);
    __18SkillAncestralCall(&skillStorage[50]);
    __9SkillRoot(&skillStorage[51]);
    __16SkillCriticalHit(&skillStorage[52]);
    __15SkillHolyShield(&skillStorage[53]);
    __12SkillHealing(&skillStorage[54]);
    __16SkillConeOfFrost(&skillStorage[55]);
    __16SkillShockOfFire(&skillStorage[56]);
    __15SkillFrostStorm(&skillStorage[57]);
    __12SkillLifeTap(&skillStorage[58]);
    inlineSkill(&skillStorage[59], "Blunt", _vt$10SkillBlunt, 3);
    inlineSkill(&skillStorage[60], "Slashing", _vt$13SkillSlashing, 3);
}

SkillStorage* skills[63] = {
    nullptr,
    &skillStorage[0],
    &skillStorage[1],
    &skillStorage[2],
    &skillStorage[3],
    &skillStorage[4],
    &skillStorage[5],
    &skillStorage[6],
    &skillStorage[7],
    &skillStorage[8],
    &skillStorage[9],
    &skillStorage[10],
    &skillStorage[11],
    &skillStorage[12],
    &skillStorage[13],
    &skillStorage[14],
    &skillStorage[15],
    &skillStorage[16],
    &skillStorage[17],
    &skillStorage[18],
    &skillStorage[19],
    &skillStorage[20],
    &skillStorage[21],
    &skillStorage[22],
    &skillStorage[23],
    &skillStorage[24],
    &skillStorage[25],
    &skillStorage[26],
    &skillStorage[27],
    &skillStorage[28],
    &skillStorage[29],
    &skillStorage[30],
    &skillStorage[31],
    &skillStorage[32],
    &skillStorage[33],
    &skillStorage[34],
    &skillStorage[35],
    &skillStorage[36],
    &skillStorage[37],
    &skillStorage[38],
    &skillStorage[39],
    &skillStorage[40],
    &skillStorage[41],
    &skillStorage[42],
    &skillStorage[43],
    &skillStorage[44],
    &skillStorage[45],
    &skillStorage[46],
    &skillStorage[47],
    &skillStorage[48],
    &skillStorage[49],
    &skillStorage[50],
    &skillStorage[51],
    &skillStorage[52],
    &skillStorage[53],
    &skillStorage[54],
    &skillStorage[55],
    &skillStorage[56],
    &skillStorage[57],
    &skillStorage[58],
    &skillStorage[59],
    &skillStorage[60],
    nullptr,
};

void _GLOBAL_$I$skills() {
    static_initialization_and_destruction_0(1, 0xffff);
}
}
