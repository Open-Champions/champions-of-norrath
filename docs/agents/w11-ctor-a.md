# w11-ctor-a: skill, spell and projectile constructors (1 of 3)

Branch `agents/w11-ctor-a`. Sources are under `experimental/sources/w11-ctor-a-*`; shared helpers are
`w11-ctor-a-common/ctor.hpp` (offset accessors, light-record fill, table-call helpers),
`w11-ctor-a-common/host_support.hpp` (host arena) and `w11-ctor-a-common/phprop.hpp` (steps both
PhProp constructors share). One verified function per commit; every unit has a `ctor.cpp` (the
function, with an observed/unknown comment block) and a `host_check.cpp` (host contract; not an EE
input).

## How each function was checked

Each function was compiled with the reviewed `authored_cpp` flags and then run side by side with the
original at the pinned image address in a private R5900 interpreter (outside git): identical memory
snapshots, identical stub callees, comparison of the call log (arguments, object contents at call
time) and every changed byte outside the stack. Scenarios covered each branch, and the original's
instruction coverage was reported (all words except nop or annulled delay slots, noted in the commit
messages where one remained). Mutations of constants, masks and operand order were each caught. A
host contract (`host_check.cpp`) with independently written expected logs also passes. For the two
PhProp constructors the paired run covered every word of the original (209 and 216 words).

## Imports closed (27 names)

`__11BoltGoRoundG6Point3siP15AnimationHeader`, `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`, `__9DustDevilG6Point3`,
`__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`__9FireStormiP10GameObjectG6Point3P5Skill`, `__15FrostStormSpellG6Point3ifiP10GameObjectT1`,
`__8HateBeamiG6Point3T2P8CreatureP5Skill`, `__14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader`,
`__8HateSoulG6Point3T1iii`, `__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill`,
`__10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader`,
`__6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf`,
`__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf`, `getSkillRank__6Playeri`,
`__21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature`,
`__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`, `__12ShockOfFrostG6Point3siP10GameObjectP5Skill`,
`__15ShockProjectileG6Point3sP10GameObjectP5Skilli`, `__18SkillAncestralCall`, `__12SkillArchery`,
`__9SkillBash`, `__12SkillBlessed`, `__18SkillBlindingLight`, `__11SkillCharge`.

Not authored here: `__7CritterPcG6Point3sPPc`. It is already defined by the ctors-ant-lantern package
(`ctors-ant-lantern-constructors/critter.cpp`), so it is not mine to reconstruct.

## Imports opened

Undefined symbols of the 27 units that no unit of this package defines (not compared against a full
partial build, so some may already be closed or listed by other packages; I ran none):
`P_CreateParticleSystem__Fv`, `P_ExtendTrail__FiG6Point3`, `P_StartLightning__FG6Point3T0iiffUi`,
`P_StartTrail__Fiifiiiii`, `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`,
`__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap`,
`__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__4PropG6Point3PcN22li`,
`animAdd__FP14AnimationStateP15AnimationHeaderi`, `animGetBoneMatrix__FP14AnimationStateiP8Matrix34`,
`animInitState__FP14AnimationState`, `animRandomizeTime__FP14AnimationState`, `blockAlloc__Fi`,
`eRand__10GameObject`, `frameNm`, `icos__Fi`, `initStuff__6PhPropi`, `isin__Fi`, `lightAdd__FP5Light`,
`lightDeactivate__FP5Light`, `lightUpdatePosition__FP5Light`, `listDestroy__FP8ListHead`,
`listOnewayLink__FP8ListHeadT0Pv`, `lumpFindResource__FPcT0`, `matMakeTransYawScale__FP8Matrix34G6Point3sf`,
`matMul__FP8Matrix34N20`, `matTransform__FP6Point3P8Matrix34T0`, `modelGetPrimaryBone__FP12_modelHeaderi`,
`nmPlayersAlive`, `objectAddToRunList__FP10GameObject`, `objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`,
`player`, `playersAlive`, `soundMakePersistant__Fi`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`sprintf`, and the table symbols `_vt$` for 10MummySnake, 10PoisonRain, 11BoltGoRound, 11DiseaseBolt,
11LightEffect, 11SkillCharge, 12ShockOfFrost, 12SkillArchery, 12SkillBlessed, 14HateProjectile,
15FrostStormSpell, 15ShockProjectile, 16PoisonProjectile, 18SkillAncestralCall, 18SkillBlindingLight,
19ColdArrowProjectile, 19FireArrowProjectile, 19MultiFireProjectile, 21PoisonArrowProjectile,
22ExplodeArrowProjectile, 6PhProp, 8HateBeam, 8HateSoul, 9DustDevil, 9FireStorm and 9SkillBash.

## Build-plan entries needed

27 entries (profile `authored_cpp`, one emitted function each, hashes included) are printed by
`python3 docs/agents/w11-ctor-a-plan-entries.py --toolchain <ee>` (copy of the w10-actor-c script with
this package's prefix; `--private-root DIR` writes a private plan/snapshot copy for a check build).
The shared `experimental/build-plan.json` and `snapshot.json` were not edited. The header closure of
the two PhProp units adds `sources/w11-ctor-a-common/phprop.hpp`; every other unit uses `ctor.hpp`.
The two PhProp `host_check.cpp` files define `vt_PhProp` and `w11_sprintf`, so they are not EE inputs.

## Authored but never executed

All 27 functions have run only in the private interpreter against the original image's code and in
host contracts with stubbed callees; none ran on an EE, in a game, or inside a full link. None is a
byte match of the original (compiled bodies differ in size from the image extents).

## Unknown

- The meaning of most object fields: the light records, the skill-table entries called by byte offset,
  the description area at +328 of a PhProp, the words at +232, +556 and +564, and the yaw offset of
  16384 in the model-placement PhProp.
- The bodies of every imported callee (LightEffect, Prop, initStuff, the particle functions, etc.);
  stores here may be overwritten by them.
- Whether the interpreter's float rounding matches the hardware on every input (it rounds to nearest;
  min.s/max.s and the divide in the storm constructors were exercised only on the scenarios listed
  in the per-function comments).
- The Point3-by-value convention: the image passes a pointer to a copy; the reconstruction declares the
  parameter by value and relies on the reviewed compiler doing the same (confirmed by the paired runs).
