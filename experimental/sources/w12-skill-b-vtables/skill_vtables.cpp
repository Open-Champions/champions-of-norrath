// Observed from the hash-pinned private SLUS_205.65 image. Each table is
// 18 adjustment/function pairs (144 bytes); adjustment words are zero and
// slot 17 is null. Slot function names are direct symbol-table matches.
struct SkillVtSlot { unsigned adjustment; void (*function)(); };
#if defined(__mips__)
static_assert(sizeof(SkillVtSlot) == 8, "EE vtable slot layout");
#endif
extern "C" {
void slot_0() asm("__tf15SkillPoisonRain");
void slot_1() asm("canCast__15SkillPoisonRainP8Creature");
void slot_2() asm("cast__15SkillPoisonRainiP8CreatureG6Point3s");
void slot_3() asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
void slot_4() asm("getRankDescription__15SkillPoisonRainPUsi");
void slot_5() asm("getSkillPointCost__15SkillPoisonRaini");
void slot_6() asm("getLevelRequirement__15SkillPoisonRaini");
void slot_7() asm("getEnergyCost__15SkillPoisonRaini");
void slot_8() asm("getIcon__5Skill");
void slot_9() asm("getRampDamage__5SkilliifiPiT5");
void slot_10() asm("initRamps__15SkillPoisonRain");
void slot_11() asm("evaluateRampAtRank__5Skillii");
void slot_12() asm("evaluateCreatureRampAtRank__5Skillii");
void slot_13() asm("evaluateRampAtRank__5SkilliiP10GameObject");
void slot_14() asm("evaluateRampAtRank__5SkilliiP8ListHead");
void slot_15() asm("tweekRank__5Skillii");
void slot_16() asm("energyCostRamp4__5Skillii");
void slot_17() asm("__tf15SkillShieldBash");
void slot_18() asm("canCast__15SkillShieldBashP8Creature");
void slot_19() asm("cast__15SkillShieldBashiP8CreatureG6Point3s");
void slot_20() asm("getRankDescription__15SkillShieldBashPUsi");
void slot_21() asm("getSkillPointCost__5Skilli");
void slot_22() asm("getLevelRequirement__15SkillShieldBashi");
void slot_23() asm("getEnergyCost__15SkillShieldBashi");
void slot_24() asm("getIcon__15SkillShieldBash");
void slot_25() asm("initRamps__15SkillShieldBash");
void slot_26() asm("__tf15SkillWizardBeam");
void slot_27() asm("canCast__5SkillP8Creature");
void slot_28() asm("cast__15SkillWizardBeamiP8CreatureG6Point3s");
void slot_29() asm("getRankDescription__15SkillWizardBeamPUsi");
void slot_30() asm("getSkillPointCost__15SkillWizardBeami");
void slot_31() asm("getLevelRequirement__15SkillWizardBeami");
void slot_32() asm("getEnergyCost__15SkillWizardBeami");
void slot_33() asm("getIcon__15SkillWizardBeam");
void slot_34() asm("initRamps__15SkillWizardBeam");
void slot_35() asm("__tf16SkillColdWeapons");
void slot_36() asm("canCast__16SkillColdWeaponsP8Creature");
void slot_37() asm("cast__16SkillColdWeaponsiP8CreatureG6Point3s");
void slot_38() asm("getRankDescription__16SkillColdWeaponsPUsi");
void slot_39() asm("getSkillPointCost__16SkillColdWeaponsi");
void slot_40() asm("getLevelRequirement__16SkillColdWeaponsi");
void slot_41() asm("getEnergyCost__16SkillColdWeaponsi");
void slot_42() asm("getIcon__16SkillColdWeapons");
void slot_43() asm("initRamps__16SkillColdWeapons");
void slot_44() asm("__tf16SkillConeOfFrost");
void slot_45() asm("cast__16SkillConeOfFrostiP8CreatureG6Point3s");
void slot_46() asm("getRankDescription__16SkillConeOfFrostPUsi");
void slot_47() asm("getLevelRequirement__5Skilli");
void slot_48() asm("getEnergyCost__16SkillConeOfFrosti");
void slot_49() asm("getIcon__16SkillConeOfFrost");
void slot_50() asm("initRamps__16SkillConeOfFrost");
void slot_51() asm("__tf16SkillCriticalHit");
void slot_52() asm("canCast__16SkillCriticalHitP8Creature");
void slot_53() asm("cast__16SkillCriticalHitiP8CreatureG6Point3s");
void slot_54() asm("getRankDescription__16SkillCriticalHitPUsi");
void slot_55() asm("getLevelRequirement__16SkillCriticalHiti");
void slot_56() asm("getEnergyCost__16SkillCriticalHiti");
void slot_57() asm("getIcon__16SkillCriticalHit");
void slot_58() asm("initRamps__16SkillCriticalHit");
void slot_59() asm("__tf16SkillDiseaseBolt");
void slot_60() asm("canCast__16SkillDiseaseBoltP8Creature");
void slot_61() asm("cast__16SkillDiseaseBoltiP8CreatureG6Point3s");
void slot_62() asm("getRankDescription__16SkillDiseaseBoltPUsi");
void slot_63() asm("getSkillPointCost__16SkillDiseaseBolti");
void slot_64() asm("getLevelRequirement__16SkillDiseaseBolti");
void slot_65() asm("getEnergyCost__16SkillDiseaseBolti");
void slot_66() asm("getIcon__16SkillDiseaseBolt");
void slot_67() asm("initRamps__16SkillDiseaseBolt");
void slot_68() asm("__tf16SkillFireWeapons");
void slot_69() asm("canCast__16SkillFireWeaponsP8Creature");
void slot_70() asm("cast__16SkillFireWeaponsiP8CreatureG6Point3s");
void slot_71() asm("getRankDescription__16SkillFireWeaponsPUsi");
void slot_72() asm("getSkillPointCost__16SkillFireWeaponsi");
void slot_73() asm("getLevelRequirement__16SkillFireWeaponsi");
void slot_74() asm("getEnergyCost__16SkillFireWeaponsi");
void slot_75() asm("getIcon__16SkillFireWeapons");
void slot_76() asm("initRamps__16SkillFireWeapons");
void slot_77() asm("__tf16SkillGroundPound");
void slot_78() asm("canCast__16SkillGroundPoundP8Creature");
void slot_79() asm("cast__16SkillGroundPoundiP8CreatureG6Point3s");
void slot_80() asm("getRankDescription__16SkillGroundPoundPUsi");
void slot_81() asm("getEnergyCost__16SkillGroundPoundi");
void slot_82() asm("getIcon__16SkillGroundPound");
void slot_83() asm("initRamps__16SkillGroundPound");
void slot_84() asm("__tf16SkillPoisonArrow");
void slot_85() asm("cast__16SkillPoisonArrowiP8CreatureG6Point3s");
void slot_86() asm("getRankDescription__16SkillPoisonArrowPUsi");
void slot_87() asm("getEnergyCost__16SkillPoisonArrowi");
void slot_88() asm("getIcon__16SkillPoisonArrow");
void slot_89() asm("initRamps__16SkillPoisonArrow");
void slot_90() asm("__tf16SkillShockOfFire");
void slot_91() asm("cast__16SkillShockOfFireiP8CreatureG6Point3s");
void slot_92() asm("getRankDescription__16SkillShockOfFirePUsi");
void slot_93() asm("getLevelRequirement__16SkillShockOfFirei");
void slot_94() asm("getEnergyCost__16SkillShockOfFirei");
void slot_95() asm("getIcon__16SkillShockOfFire");
void slot_96() asm("initRamps__16SkillShockOfFire");
void slot_97() asm("__tf17SkillConvertEnemy");
void slot_98() asm("cast__17SkillConvertEnemyiP8CreatureG6Point3s");
void slot_99() asm("getRankDescription__17SkillConvertEnemyPUsi");
void slot_100() asm("getEnergyCost__17SkillConvertEnemyi");
void slot_101() asm("getIcon__17SkillConvertEnemy");
void slot_102() asm("initRamps__17SkillConvertEnemy");
void slot_103() asm("__tf17SkillDiseaseTrail");
void slot_104() asm("canCast__17SkillDiseaseTrailP8Creature");
void slot_105() asm("cast__17SkillDiseaseTrailiP8CreatureG6Point3s");
void slot_106() asm("getRankDescription__17SkillDiseaseTrailPUsi");
void slot_107() asm("getSkillPointCost__17SkillDiseaseTraili");
void slot_108() asm("getLevelRequirement__17SkillDiseaseTraili");
void slot_109() asm("getEnergyCost__17SkillDiseaseTraili");
void slot_110() asm("getIcon__17SkillDiseaseTrail");
void slot_111() asm("initRamps__17SkillDiseaseTrail");
void slot_112() asm("__tf17SkillExplodeArrow");
void slot_113() asm("cast__17SkillExplodeArrowiP8CreatureG6Point3s");
void slot_114() asm("getRankDescription__17SkillExplodeArrowPUsi");
void slot_115() asm("getEnergyCost__17SkillExplodeArrowi");
void slot_116() asm("getIcon__17SkillExplodeArrow");
void slot_117() asm("initRamps__17SkillExplodeArrow");
void slot_118() asm("__tf17SkillMinorHealing");
void slot_119() asm("cast__17SkillMinorHealingiP8CreatureG6Point3s");
void slot_120() asm("getRankDescription__17SkillMinorHealingPUsi");
void slot_121() asm("getLevelRequirement__17SkillMinorHealingi");
void slot_122() asm("getEnergyCost__17SkillMinorHealingi");
void slot_123() asm("getIcon__17SkillMinorHealing");
void slot_124() asm("initRamps__17SkillMinorHealing");
void slot_125() asm("__tf17SkillRegeneration");
void slot_126() asm("cast__17SkillRegenerationiP8CreatureG6Point3s");
void slot_127() asm("getRankDescription__17SkillRegenerationPUsi");
void slot_128() asm("getSkillPointCost__17SkillRegenerationi");
void slot_129() asm("getLevelRequirement__17SkillRegenerationi");
void slot_130() asm("getEnergyCost__5Skilli");
void slot_131() asm("initRamps__17SkillRegeneration");
void slot_132() asm("__tf17SkillShockOfFrost");
void slot_133() asm("canCast__17SkillShockOfFrostP8Creature");
void slot_134() asm("cast__17SkillShockOfFrostiP8CreatureG6Point3s");
void slot_135() asm("getRankDescription__17SkillShockOfFrostPUsi");
void slot_136() asm("getSkillPointCost__17SkillShockOfFrosti");
void slot_137() asm("getLevelRequirement__17SkillShockOfFrosti");
void slot_138() asm("getEnergyCost__17SkillShockOfFrosti");
void slot_139() asm("getIcon__17SkillShockOfFrost");
void slot_140() asm("initRamps__17SkillShockOfFrost");
void slot_141() asm("__tf17SkillUndeadShield");
void slot_142() asm("canCast__17SkillUndeadShieldP8Creature");
void slot_143() asm("cast__17SkillUndeadShieldiP8CreatureG6Point3s");
void slot_144() asm("getRankDescription__17SkillUndeadShieldPUsi");
void slot_145() asm("getSkillPointCost__17SkillUndeadShieldi");
void slot_146() asm("getLevelRequirement__17SkillUndeadShieldi");
void slot_147() asm("getEnergyCost__17SkillUndeadShieldi");
void slot_148() asm("getIcon__17SkillUndeadShield");
void slot_149() asm("initRamps__17SkillUndeadShield");
void slot_150() asm("__tf18SkillAncestralCall");
void slot_151() asm("canCast__18SkillAncestralCallP8Creature");
void slot_152() asm("cast__18SkillAncestralCalliP8CreatureG6Point3s");
void slot_153() asm("getRankDescription__18SkillAncestralCallPUsi");
void slot_154() asm("getEnergyCost__18SkillAncestralCalli");
void slot_155() asm("getIcon__18SkillAncestralCall");
void slot_156() asm("initRamps__18SkillAncestralCall");
void slot_157() asm("__tf18SkillBlindingLight");
void slot_158() asm("canCast__18SkillBlindingLightP8Creature");
void slot_159() asm("cast__18SkillBlindingLightiP8CreatureG6Point3s");
void slot_160() asm("getRankDescription__18SkillBlindingLightPUsi");
void slot_161() asm("getSkillPointCost__18SkillBlindingLighti");
void slot_162() asm("getLevelRequirement__18SkillBlindingLighti");
void slot_163() asm("getEnergyCost__18SkillBlindingLighti");
void slot_164() asm("getIcon__18SkillBlindingLight");
void slot_165() asm("initRamps__18SkillBlindingLight");
void slot_166() asm("__tf18SkillConvertUndead");
void slot_167() asm("canCast__18SkillConvertUndeadP8Creature");
void slot_168() asm("cast__18SkillConvertUndeadiP8CreatureG6Point3s");
void slot_169() asm("getRankDescription__18SkillConvertUndeadPUsi");
void slot_170() asm("getEnergyCost__18SkillConvertUndeadi");
void slot_171() asm("getIcon__18SkillConvertUndead");
void slot_172() asm("initRamps__18SkillConvertUndead");
void slot_173() asm("__tf18SkillDiseaseShield");
void slot_174() asm("canCast__18SkillDiseaseShieldP8Creature");
void slot_175() asm("cast__18SkillDiseaseShieldiP8CreatureG6Point3s");
void slot_176() asm("getRankDescription__18SkillDiseaseShieldPUsi");
void slot_177() asm("getSkillPointCost__18SkillDiseaseShieldi");
void slot_178() asm("getLevelRequirement__18SkillDiseaseShieldi");
void slot_179() asm("getEnergyCost__18SkillDiseaseShieldi");
void slot_180() asm("getIcon__18SkillDiseaseShield");
void slot_181() asm("initRamps__18SkillDiseaseShield");
void slot_182() asm("__tf18SkillDismissUndead");
void slot_183() asm("cast__18SkillDismissUndeadiP8CreatureG6Point3s");
void slot_184() asm("getRankDescription__18SkillDismissUndeadPUsi");
void slot_185() asm("getSkillPointCost__18SkillDismissUndeadi");
void slot_186() asm("getLevelRequirement__18SkillDismissUndeadi");
void slot_187() asm("getEnergyCost__18SkillDismissUndeadi");
void slot_188() asm("getIcon__18SkillDismissUndead");
void slot_189() asm("initRamps__18SkillDismissUndead");
void slot_190() asm("__tf18SkillHammerOfWrath");
void slot_191() asm("canCast__18SkillHammerOfWrathP8Creature");
void slot_192() asm("cast__18SkillHammerOfWrathiP8CreatureG6Point3s");
void slot_193() asm("getRankDescription__18SkillHammerOfWrathPUsi");
void slot_194() asm("getSkillPointCost__18SkillHammerOfWrathi");
void slot_195() asm("getLevelRequirement__18SkillHammerOfWrathi");
void slot_196() asm("getEnergyCost__18SkillHammerOfWrathi");
void slot_197() asm("getIcon__18SkillHammerOfWrath");
void slot_198() asm("initRamps__18SkillHammerOfWrath");
void slot_199() asm("__tf18SkillRepulseUndead");
void slot_200() asm("cast__18SkillRepulseUndeadiP8CreatureG6Point3s");
void slot_201() asm("getRankDescription__18SkillRepulseUndeadPUsi");
void slot_202() asm("getSkillPointCost__18SkillRepulseUndeadi");
void slot_203() asm("getLevelRequirement__18SkillRepulseUndeadi");
void slot_204() asm("getEnergyCost__18SkillRepulseUndeadi");
void slot_205() asm("getIcon__18SkillRepulseUndead");
void slot_206() asm("initRamps__18SkillRepulseUndead");
void slot_207() asm("__tf19SkillSummonSkeleton");
void slot_208() asm("canCast__19SkillSummonSkeletonP8Creature");
void slot_209() asm("cast__19SkillSummonSkeletoniP8CreatureG6Point3s");
void slot_210() asm("getRankDescription__19SkillSummonSkeletonPUsi");
void slot_211() asm("getSkillPointCost__19SkillSummonSkeletoni");
void slot_212() asm("getLevelRequirement__19SkillSummonSkeletoni");
void slot_213() asm("getEnergyCost__19SkillSummonSkeletoni");
void slot_214() asm("getIcon__19SkillSummonSkeleton");
void slot_215() asm("initRamps__19SkillSummonSkeleton");
void slot_216() asm("__tf21SkillLightningWeapons");
void slot_217() asm("canCast__21SkillLightningWeaponsP8Creature");
void slot_218() asm("cast__21SkillLightningWeaponsiP8CreatureG6Point3s");
void slot_219() asm("getRankDescription__21SkillLightningWeaponsPUsi");
void slot_220() asm("getSkillPointCost__21SkillLightningWeaponsi");
void slot_221() asm("getLevelRequirement__21SkillLightningWeaponsi");
void slot_222() asm("getEnergyCost__21SkillLightningWeaponsi");
void slot_223() asm("getIcon__21SkillLightningWeapons");
void slot_224() asm("initRamps__21SkillLightningWeapons");
void slot_225() asm("__tf22SkillRangerCriticalHit");
void slot_226() asm("canCast__22SkillRangerCriticalHitP8Creature");
void slot_227() asm("cast__22SkillRangerCriticalHitiP8CreatureG6Point3s");
void slot_228() asm("getRankDescription__22SkillRangerCriticalHitPUsi");
void slot_229() asm("getLevelRequirement__22SkillRangerCriticalHiti");
void slot_230() asm("getEnergyCost__22SkillRangerCriticalHiti");
void slot_231() asm("getIcon__22SkillRangerCriticalHit");
void slot_232() asm("initRamps__22SkillRangerCriticalHit");
void slot_233() asm("__tf9SkillBash");
void slot_234() asm("canCast__9SkillBashP8Creature");
void slot_235() asm("cast__9SkillBashiP8CreatureG6Point3s");
void slot_236() asm("getRankDescription__9SkillBashPUsi");
void slot_237() asm("getLevelRequirement__9SkillBashi");
void slot_238() asm("getEnergyCost__9SkillBashi");
void slot_239() asm("getIcon__9SkillBash");
void slot_240() asm("initRamps__9SkillBash");
void slot_241() asm("__tf9SkillRoot");
void slot_242() asm("cast__9SkillRootiP8CreatureG6Point3s");
void slot_243() asm("getRankDescription__9SkillRootPUsi");
void slot_244() asm("getLevelRequirement__9SkillRooti");
void slot_245() asm("getEnergyCost__9SkillRooti");
void slot_246() asm("getIcon__9SkillRoot");
void slot_247() asm("initRamps__9SkillRoot");
SkillVtSlot _vt_15SkillPoisonRain[18] asm("_vt$15SkillPoisonRain") = {
    {0u, slot_0},
    {0u, slot_1},
    {0u, slot_2},
    {0u, slot_3},
    {0u, slot_4},
    {0u, slot_5},
    {0u, slot_6},
    {0u, slot_7},
    {0u, slot_8},
    {0u, slot_9},
    {0u, slot_10},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_15SkillPoisonRain) == 144, "observed table size");
#endif
SkillVtSlot _vt_15SkillShieldBash[18] asm("_vt$15SkillShieldBash") = {
    {0u, slot_17},
    {0u, slot_18},
    {0u, slot_19},
    {0u, slot_3},
    {0u, slot_20},
    {0u, slot_21},
    {0u, slot_22},
    {0u, slot_23},
    {0u, slot_24},
    {0u, slot_9},
    {0u, slot_25},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_15SkillShieldBash) == 144, "observed table size");
#endif
SkillVtSlot _vt_15SkillWizardBeam[18] asm("_vt$15SkillWizardBeam") = {
    {0u, slot_26},
    {0u, slot_27},
    {0u, slot_28},
    {0u, slot_3},
    {0u, slot_29},
    {0u, slot_30},
    {0u, slot_31},
    {0u, slot_32},
    {0u, slot_33},
    {0u, slot_9},
    {0u, slot_34},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_15SkillWizardBeam) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillColdWeapons[18] asm("_vt$16SkillColdWeapons") = {
    {0u, slot_35},
    {0u, slot_36},
    {0u, slot_37},
    {0u, slot_3},
    {0u, slot_38},
    {0u, slot_39},
    {0u, slot_40},
    {0u, slot_41},
    {0u, slot_42},
    {0u, slot_9},
    {0u, slot_43},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillColdWeapons) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillConeOfFrost[18] asm("_vt$16SkillConeOfFrost") = {
    {0u, slot_44},
    {0u, slot_27},
    {0u, slot_45},
    {0u, slot_3},
    {0u, slot_46},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_48},
    {0u, slot_49},
    {0u, slot_9},
    {0u, slot_50},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillConeOfFrost) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillCriticalHit[18] asm("_vt$16SkillCriticalHit") = {
    {0u, slot_51},
    {0u, slot_52},
    {0u, slot_53},
    {0u, slot_3},
    {0u, slot_54},
    {0u, slot_21},
    {0u, slot_55},
    {0u, slot_56},
    {0u, slot_57},
    {0u, slot_9},
    {0u, slot_58},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillCriticalHit) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillDiseaseBolt[18] asm("_vt$16SkillDiseaseBolt") = {
    {0u, slot_59},
    {0u, slot_60},
    {0u, slot_61},
    {0u, slot_3},
    {0u, slot_62},
    {0u, slot_63},
    {0u, slot_64},
    {0u, slot_65},
    {0u, slot_66},
    {0u, slot_9},
    {0u, slot_67},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillDiseaseBolt) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillFireWeapons[18] asm("_vt$16SkillFireWeapons") = {
    {0u, slot_68},
    {0u, slot_69},
    {0u, slot_70},
    {0u, slot_3},
    {0u, slot_71},
    {0u, slot_72},
    {0u, slot_73},
    {0u, slot_74},
    {0u, slot_75},
    {0u, slot_9},
    {0u, slot_76},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillFireWeapons) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillGroundPound[18] asm("_vt$16SkillGroundPound") = {
    {0u, slot_77},
    {0u, slot_78},
    {0u, slot_79},
    {0u, slot_3},
    {0u, slot_80},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_81},
    {0u, slot_82},
    {0u, slot_9},
    {0u, slot_83},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillGroundPound) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillPoisonArrow[18] asm("_vt$16SkillPoisonArrow") = {
    {0u, slot_84},
    {0u, slot_27},
    {0u, slot_85},
    {0u, slot_3},
    {0u, slot_86},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_87},
    {0u, slot_88},
    {0u, slot_9},
    {0u, slot_89},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillPoisonArrow) == 144, "observed table size");
#endif
SkillVtSlot _vt_16SkillShockOfFire[18] asm("_vt$16SkillShockOfFire") = {
    {0u, slot_90},
    {0u, slot_27},
    {0u, slot_91},
    {0u, slot_3},
    {0u, slot_92},
    {0u, slot_21},
    {0u, slot_93},
    {0u, slot_94},
    {0u, slot_95},
    {0u, slot_9},
    {0u, slot_96},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_16SkillShockOfFire) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillConvertEnemy[18] asm("_vt$17SkillConvertEnemy") = {
    {0u, slot_97},
    {0u, slot_27},
    {0u, slot_98},
    {0u, slot_3},
    {0u, slot_99},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_100},
    {0u, slot_101},
    {0u, slot_9},
    {0u, slot_102},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillConvertEnemy) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillDiseaseTrail[18] asm("_vt$17SkillDiseaseTrail") = {
    {0u, slot_103},
    {0u, slot_104},
    {0u, slot_105},
    {0u, slot_3},
    {0u, slot_106},
    {0u, slot_107},
    {0u, slot_108},
    {0u, slot_109},
    {0u, slot_110},
    {0u, slot_9},
    {0u, slot_111},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillDiseaseTrail) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillExplodeArrow[18] asm("_vt$17SkillExplodeArrow") = {
    {0u, slot_112},
    {0u, slot_27},
    {0u, slot_113},
    {0u, slot_3},
    {0u, slot_114},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_115},
    {0u, slot_116},
    {0u, slot_9},
    {0u, slot_117},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillExplodeArrow) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillMinorHealing[18] asm("_vt$17SkillMinorHealing") = {
    {0u, slot_118},
    {0u, slot_27},
    {0u, slot_119},
    {0u, slot_3},
    {0u, slot_120},
    {0u, slot_21},
    {0u, slot_121},
    {0u, slot_122},
    {0u, slot_123},
    {0u, slot_9},
    {0u, slot_124},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillMinorHealing) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillRegeneration[18] asm("_vt$17SkillRegeneration") = {
    {0u, slot_125},
    {0u, slot_27},
    {0u, slot_126},
    {0u, slot_3},
    {0u, slot_127},
    {0u, slot_128},
    {0u, slot_129},
    {0u, slot_130},
    {0u, slot_8},
    {0u, slot_9},
    {0u, slot_131},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillRegeneration) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillShockOfFrost[18] asm("_vt$17SkillShockOfFrost") = {
    {0u, slot_132},
    {0u, slot_133},
    {0u, slot_134},
    {0u, slot_3},
    {0u, slot_135},
    {0u, slot_136},
    {0u, slot_137},
    {0u, slot_138},
    {0u, slot_139},
    {0u, slot_9},
    {0u, slot_140},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillShockOfFrost) == 144, "observed table size");
#endif
SkillVtSlot _vt_17SkillUndeadShield[18] asm("_vt$17SkillUndeadShield") = {
    {0u, slot_141},
    {0u, slot_142},
    {0u, slot_143},
    {0u, slot_3},
    {0u, slot_144},
    {0u, slot_145},
    {0u, slot_146},
    {0u, slot_147},
    {0u, slot_148},
    {0u, slot_9},
    {0u, slot_149},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_17SkillUndeadShield) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillAncestralCall[18] asm("_vt$18SkillAncestralCall") = {
    {0u, slot_150},
    {0u, slot_151},
    {0u, slot_152},
    {0u, slot_3},
    {0u, slot_153},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_154},
    {0u, slot_155},
    {0u, slot_9},
    {0u, slot_156},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillAncestralCall) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillBlindingLight[18] asm("_vt$18SkillBlindingLight") = {
    {0u, slot_157},
    {0u, slot_158},
    {0u, slot_159},
    {0u, slot_3},
    {0u, slot_160},
    {0u, slot_161},
    {0u, slot_162},
    {0u, slot_163},
    {0u, slot_164},
    {0u, slot_9},
    {0u, slot_165},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillBlindingLight) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillConvertUndead[18] asm("_vt$18SkillConvertUndead") = {
    {0u, slot_166},
    {0u, slot_167},
    {0u, slot_168},
    {0u, slot_3},
    {0u, slot_169},
    {0u, slot_21},
    {0u, slot_47},
    {0u, slot_170},
    {0u, slot_171},
    {0u, slot_9},
    {0u, slot_172},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillConvertUndead) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillDiseaseShield[18] asm("_vt$18SkillDiseaseShield") = {
    {0u, slot_173},
    {0u, slot_174},
    {0u, slot_175},
    {0u, slot_3},
    {0u, slot_176},
    {0u, slot_177},
    {0u, slot_178},
    {0u, slot_179},
    {0u, slot_180},
    {0u, slot_9},
    {0u, slot_181},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillDiseaseShield) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillDismissUndead[18] asm("_vt$18SkillDismissUndead") = {
    {0u, slot_182},
    {0u, slot_27},
    {0u, slot_183},
    {0u, slot_3},
    {0u, slot_184},
    {0u, slot_185},
    {0u, slot_186},
    {0u, slot_187},
    {0u, slot_188},
    {0u, slot_9},
    {0u, slot_189},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillDismissUndead) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillHammerOfWrath[18] asm("_vt$18SkillHammerOfWrath") = {
    {0u, slot_190},
    {0u, slot_191},
    {0u, slot_192},
    {0u, slot_3},
    {0u, slot_193},
    {0u, slot_194},
    {0u, slot_195},
    {0u, slot_196},
    {0u, slot_197},
    {0u, slot_9},
    {0u, slot_198},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillHammerOfWrath) == 144, "observed table size");
#endif
SkillVtSlot _vt_18SkillRepulseUndead[18] asm("_vt$18SkillRepulseUndead") = {
    {0u, slot_199},
    {0u, slot_27},
    {0u, slot_200},
    {0u, slot_3},
    {0u, slot_201},
    {0u, slot_202},
    {0u, slot_203},
    {0u, slot_204},
    {0u, slot_205},
    {0u, slot_9},
    {0u, slot_206},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_18SkillRepulseUndead) == 144, "observed table size");
#endif
SkillVtSlot _vt_19SkillSummonSkeleton[18] asm("_vt$19SkillSummonSkeleton") = {
    {0u, slot_207},
    {0u, slot_208},
    {0u, slot_209},
    {0u, slot_3},
    {0u, slot_210},
    {0u, slot_211},
    {0u, slot_212},
    {0u, slot_213},
    {0u, slot_214},
    {0u, slot_9},
    {0u, slot_215},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_19SkillSummonSkeleton) == 144, "observed table size");
#endif
SkillVtSlot _vt_21SkillLightningWeapons[18] asm("_vt$21SkillLightningWeapons") = {
    {0u, slot_216},
    {0u, slot_217},
    {0u, slot_218},
    {0u, slot_3},
    {0u, slot_219},
    {0u, slot_220},
    {0u, slot_221},
    {0u, slot_222},
    {0u, slot_223},
    {0u, slot_9},
    {0u, slot_224},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_21SkillLightningWeapons) == 144, "observed table size");
#endif
SkillVtSlot _vt_22SkillRangerCriticalHit[18] asm("_vt$22SkillRangerCriticalHit") = {
    {0u, slot_225},
    {0u, slot_226},
    {0u, slot_227},
    {0u, slot_3},
    {0u, slot_228},
    {0u, slot_21},
    {0u, slot_229},
    {0u, slot_230},
    {0u, slot_231},
    {0u, slot_9},
    {0u, slot_232},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_22SkillRangerCriticalHit) == 144, "observed table size");
#endif
SkillVtSlot _vt_9SkillBash[18] asm("_vt$9SkillBash") = {
    {0u, slot_233},
    {0u, slot_234},
    {0u, slot_235},
    {0u, slot_3},
    {0u, slot_236},
    {0u, slot_21},
    {0u, slot_237},
    {0u, slot_238},
    {0u, slot_239},
    {0u, slot_9},
    {0u, slot_240},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_9SkillBash) == 144, "observed table size");
#endif
SkillVtSlot _vt_9SkillRoot[18] asm("_vt$9SkillRoot") = {
    {0u, slot_241},
    {0u, slot_27},
    {0u, slot_242},
    {0u, slot_3},
    {0u, slot_243},
    {0u, slot_21},
    {0u, slot_244},
    {0u, slot_245},
    {0u, slot_246},
    {0u, slot_9},
    {0u, slot_247},
    {0u, slot_11},
    {0u, slot_12},
    {0u, slot_13},
    {0u, slot_14},
    {0u, slot_15},
    {0u, slot_16},
    {0u, nullptr},
};
#if defined(__mips__)
static_assert(sizeof(_vt_9SkillRoot) == 144, "observed table size");
#endif
}
