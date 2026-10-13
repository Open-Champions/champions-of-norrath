# w10-actor-a: spell, skill and actor virtuals (Creature to AntLanternCtors)

Package `w10-actor-a`, branch `agents/w10-actor-a`. Private reference: the original
executable `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes or disassembly listings are committed.
Everything below was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has
run on a PS2 or in a game; "paired" means the authored object was run next to the original
function in a private R5900 interpreter (kept outside git).

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the
  original's address, then run the original and the authored function from identical memory
  snapshots with identical stub callees. Compared: the ordered callee log (argument registers,
  float arguments, pointed-to vectors and descriptions, and a hash of the object at every
  call, which catches store-order differences), the return value where the original defines
  one, and every changed byte outside the stack. Scenarios use fixed random seeds. Where
  noted, instruction coverage of the original was measured and mutations of single constants
  or boundaries were confirmed to fail.
- Host contract (`host_check.cpp` in each directory, not a build input): restates the
  expected behaviour as stub-callee sequences and field values. Units that read 32-bit pointer
  words use `W10_ACTOR_A_HOST`, which turns those words into offsets from a host base.
- The interpreter was corrected during this package: divide-by-zero and float overflow now give
  the R5900 largest value (bits 0x7fffffff) instead of infinity. This mattered for the light
  range floats in the PlanarSteel constructor.

## Functions

| directory | symbol | original extent | compiled bytes | check |
|---|---|---|---|---|
| `w10-actor-a-static-init-*` (15 directories) | the 15 `_GLOBAL_$I$...` wrappers in the task list | see earlier commits | 112 to 648 | paired (call order, every argument, record alignment) and host contract |
| `w10-actor-a-terminate-spell` | `terminateSpell__8CreatureRC9type_info` | 0x113890, 64 | 68 | 40 paired scenarios, host contract |
| `w10-actor-a-tf-15spellconeoffire` | `__tf15SpellConeOfFire` | 0x319918, 112 | 164 | 9 paired scenarios, host contract |
| `w10-actor-a-ctor-spell-horn-omen-effect` | `__19SpellHornOmenEffectiP8CreatureP5Skill` | 0x2ca2c8, 144 | 144 | 60 paired scenarios (object bytes at the particle call), host contract |
| `w10-actor-a-ctor-spell-planar-steel-effect` | `__22SpellPlanarSteelEffectiP8CreatureP5Skill` | 0x2c8bc0, 488 | 384 | 60 paired scenarios, host contract |
| `w10-actor-a-ctor-animation-state8-copy` | `__15AnimationState8RC15AnimationState8` | 0x163010, 588 | 356 | 40 paired scenarios (incl. 4-byte-misaligned objects), mutation caught, host contract |
| `w10-actor-a-dtor-3ant` | `_$_3Ant` | 0x30e848, 100 | 88 | 24 paired scenarios, host contract |
| `w10-actor-a-tf-3ant` | `__tf3Ant` | 0x30e8b0, 80 | 92 | 9 paired scenarios, host contract |
| `w10-actor-a-msg-draw-3ant` | `msg_draw__3Ant` | 0x208820, 76 | 40 | 24 paired scenarios, host contract |
| `w10-actor-a-msg-save-3ant` | `msg_save__3AntPc` | 0x208870, 72 | 56 | 24 paired scenarios, host contract |
| `w10-actor-a-msg-load-3ant` | `msg_load__3AntPc` | 0x2088b8, 124 | 136 | 24 paired scenarios, host contract |
| `w10-actor-a-clone-3ant` | `clone__3Ant` | 0x116c70, 1400 | 864 | 10 paired scenarios incl. null allocation (object hash at each call), mutation caught, host contract |
| `w10-actor-a-set-state-3ant` | `setState__3Anti` | 0x206628, 1100 | 1320 | 400 paired scenarios, 273 of 275 original words covered (the rest are nops), host contract |
| `w10-actor-a-msg-run-3ant` | `msg_run__3Ant` | 0x205980, 3236 | 2544 (plus two file-local helpers) | 1500 mixed plus focused paired scenarios, 784 of 809 original words covered (the rest are nops, annulled delay slots and loop-continuation paths that cannot be reached), mutations caught, host contract |
| `w10-actor-a-view-42c760` | `norrath_ctors_ant_lantern_view_42c760` (`chestResourceMap`) | 0x42c760, 40 | data | strings read from the image, host comparison |
| `w10-actor-a-view-42c788` | `norrath_ctors_ant_lantern_view_42c788` (`chest2ResourceMap`) | 0x42c788, 40 | data | strings read from the image, host comparison |

## Observed, inferred, unknown

Observed: every field offset, constant, call order, branch and table slot used in the sources
comes from the original function bodies; the `msg_run__3Ant` and `setState__3Anti` jump tables
have 14 entries each; the PlanarSteel light floats at +32 and +40 are 1.0f/0.0f (bits
0x7fffffff on the EE); the AnimationState8 copy constructor copies a 264-byte block and then
duplicates up to eight 3868-byte records through `getFreeFrameState__Fv`.

Inferred: that `_$_3Ant` is a destructor and `_$_3Ant`'s flag bit 0 means "free"; that
`msg_save/msg_load` are a pair around the word at +208; that the +36 block holds animation
header pointers; that 21 and 23 are damage-type codes. All from names and shapes only.

Unknown: the meaning of nearly every object field (+208, +256, +804, +808, +964, +968, +972,
+976, +980 and the halfword at +572 beyond "state"); what the state numbers stand for; the
purpose of the two-element countdown stores at +24/+26 in the PlanarSteel constructor; the
real names of the data views `skills[13]`, `skills[26]` and the file-local particle definition
at 0x7d7170.

## Build-plan entries needed

Append these 30 authored inputs with fresh `input_order` values after the current tip (for
`sources/w10-actor-a-*` paths); `default_authored_inputs` grows by 30. Every unit uses profile
`authored_cpp`. `host_check.cpp` files are not inputs. The static-initializer units include
`../w8-spell-static-init-a-common/particle_def.hpp`, already in the plan. Body sizes are from
the reviewed EE g++ build of these exact files and were not compared with the original
executable. `msg_run.o` also emits two file-local helper functions, listed in its entry.

```json
{"emitted_functions":[{"compiled_body_bytes":864,"name":"clone__3Ant"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"1c5528080067f50c035c84429c4fbe7ca60fe3130a21fd7a5b20a239e7c72b7e","source":"sources/w10-actor-a-clone-3ant/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":356,"name":"__15AnimationState8RC15AnimationState8"}],"header_closure":[],"kind":"independently_authored_source","object_name":"animation_state8_copy.o","profile":"authored_cpp","sha256":"99f6b0150637f4d0a98c95cb2a34029a1b3164363debfd43e5b917fc10d5433e","source":"sources/w10-actor-a-ctor-animation-state8-copy/animation_state8_copy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"__19SpellHornOmenEffectiP8CreatureP5Skill"}],"header_closure":[],"kind":"independently_authored_source","object_name":"spell_horn_omen_effect_ctor.o","profile":"authored_cpp","sha256":"9c654db04163fcd7db2345648b16cf6cfc48746012db394d6dd6b52b7a4dcc26","source":"sources/w10-actor-a-ctor-spell-horn-omen-effect/spell_horn_omen_effect_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":384,"name":"__22SpellPlanarSteelEffectiP8CreatureP5Skill"}],"header_closure":[],"kind":"independently_authored_source","object_name":"spell_planar_steel_effect_ctor.o","profile":"authored_cpp","sha256":"4c49cce536ef810d8cbc30e8ffde8b9c3925973f8cb4e1e36a924732901653c1","source":"sources/w10-actor-a-ctor-spell-planar-steel-effect/spell_planar_steel_effect_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_3Ant"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"0ec2b06994e6b55a4291855e5927c2553ab6a7ebb7fc1fe1dc73ba611b96f241","source":"sources/w10-actor-a-dtor-3ant/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":40,"name":"msg_draw__3Ant"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"3dd2686e364ac4031a249f338f239e7aa1149b7e048f136ce3eda3df012c7d30","source":"sources/w10-actor-a-msg-draw-3ant/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"msg_load__3AntPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"a5873daaf1672b3b194bae2d148337a508787a2be6292def5c94bfb4a3427048","source":"sources/w10-actor-a-msg-load-3ant/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":324,"name":"_ZN12_GLOBAL__N_1L16roll_descriptionEPhjs"},{"compiled_body_bytes":224,"name":"_ZN12_GLOBAL__N_1L9unit_ballEPhR6Point3"},{"compiled_body_bytes":2544,"name":"msg_run__3Ant"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"9b7d8acb614290ed9707a04736f280edacd54450f9657e738b38384648459845","source":"sources/w10-actor-a-msg-run-3ant/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"msg_save__3AntPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_save.o","profile":"authored_cpp","sha256":"9b74bc06cac5290d0f830c52b74cd0f68bc47f0b9643ce537d2848158e71ed2e","source":"sources/w10-actor-a-msg-save-3ant/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1320,"name":"setState__3Anti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"01dff31e96473fdf869c0199e1af7233981061b6f54d180b3d727a892b08f073","source":"sources/w10-actor-a-set-state-3ant/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_ancestral_call_static_init.o","profile":"authored_cpp","sha256":"61d857baac28011ae1a31b62c9ab0474db6005e4cc8d31db199733ca3970d572","source":"sources/w10-actor-a-static-init-skill-ancestral-call/skill_ancestral_call_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_GLOBAL_$I$getRankDescription__9SkillBashPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_bash_static_init.o","profile":"authored_cpp","sha256":"17412fd6616da2abbfe425dcf1e72f9cbaa396886229637cda27edb9ee2be2eb","source":"sources/w10-actor-a-static-init-skill-bash/skill_bash_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":332,"name":"_GLOBAL_$I$getRankDescription__11SkillChargePUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_charge_static_init.o","profile":"authored_cpp","sha256":"e771b20a87ec90171069ac0356f8a678e8093ce46b961a7f1147029398bf6e62","source":"sources/w10-actor-a-static-init-skill-charge/skill_charge_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_GLOBAL_$I$getCriticalChance__16SkillCriticalHitP6Playeri"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_critical_hit_static_init.o","profile":"authored_cpp","sha256":"2a9666dce74fd466b2256b76c2e7b9aaa1b532cbe72aae9e1b9de03834dd358b","source":"sources/w10-actor-a-static-init-skill-critical-hit/skill_critical_hit_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":648,"name":"_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_ground_pound_static_init.o","profile":"authored_cpp","sha256":"3d4c6c678a434d2398310b8b3a7d77e8793403c8cca44726d77c716956b4faca","source":"sources/w10-actor-a-static-init-skill-ground-pound/skill_ground_pound_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_GLOBAL_$I$getRankDescription__12SkillHealingPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_healing_static_init.o","profile":"authored_cpp","sha256":"e935e4376b7447c3f58a8d8eccc5571f741a4959e4b774e2653807e82e5dbd3a","source":"sources/w10-actor-a-static-init-skill-healing/skill_healing_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_GLOBAL_$I$getRankDescription__15SkillHolyShieldPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_holy_shield_static_init.o","profile":"authored_cpp","sha256":"8d552da40b2b03df226eabdb110ae01f882e1e72e65f31bee6a0496526e2bd81","source":"sources/w10-actor-a-static-init-skill-holy-shield/skill_holy_shield_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":312,"name":"_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_poison_arrow_static_init.o","profile":"authored_cpp","sha256":"36da7c06b08471acecacc14fbd0929b51c69bbc93d44a09fd42d7817a3c7874c","source":"sources/w10-actor-a-static-init-skill-poison-arrow/skill_poison_arrow_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_ranger_critical_hit_static_init.o","profile":"authored_cpp","sha256":"1e75bec735691578c4e07aa17f6c91ffe1d3c3b8a9455f054805ab21b2846c5f","source":"sources/w10-actor-a-static-init-skill-ranger-critical-hit/skill_ranger_critical_hit_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"_GLOBAL_$I$getRankDescription__12SkillRipostePUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_riposte_static_init.o","profile":"authored_cpp","sha256":"8cb8ce2d4c20066dfd7606292a51fadd54a265aecf85f62ced654a292cb0eab3","source":"sources/w10-actor-a-static-init-skill-riposte/skill_riposte_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"_GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_root_static_init.o","profile":"authored_cpp","sha256":"46c1f730c7ff31296a72808d4b3c66ba747bdfc3ee52dc01b7bd5c2e0d0604f1","source":"sources/w10-actor-a-static-init-skill-root/skill_root_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_GLOBAL_$I$getRankDescription__15SkillShieldBashPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_shield_bash_static_init.o","profile":"authored_cpp","sha256":"657efa958d768f76a6dee9156873f4a66c3bdf5a689c737596398802a33367bf","source":"sources/w10-actor-a-static-init-skill-shield-bash/skill_shield_bash_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":204,"name":"_GLOBAL_$I$getRankDescription__10SkillShockPUsi"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_shock_static_init.o","profile":"authored_cpp","sha256":"112c3844ff726ba9b78c846ebfa416360f37ad644637201462586c7e91cfece2","source":"sources/w10-actor-a-static-init-skill-shock/skill_shock_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":204,"name":"_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"skill_summon_skeleton_static_init.o","profile":"authored_cpp","sha256":"adb25890f04a04720b2826523bcade78245613b1a47b745124efc389f66d83c4","source":"sources/w10-actor-a-static-init-skill-summon-skeleton/skill_summon_skeleton_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s"}],"header_closure":["sources/w8-spell-static-init-a-common/particle_def.hpp"],"kind":"independently_authored_source","object_name":"spell_horn_omen_effect_static_init.o","profile":"authored_cpp","sha256":"e02ec8c8617c742abfbfada5863b3753525d81f20945435fba2588a5e6792281","source":"sources/w10-actor-a-static-init-spell-horn-omen-effect/spell_horn_omen_effect_static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"terminateSpell__8CreatureRC9type_info"}],"header_closure":[],"kind":"independently_authored_source","object_name":"terminate_spell.o","profile":"authored_cpp","sha256":"f0a21d26cd97fa2b38b619f2f9cc761b431ab22591bb67e5650d13cfe7e81506","source":"sources/w10-actor-a-terminate-spell/terminate_spell.cpp"}
{"emitted_functions":[{"compiled_body_bytes":164,"name":"__tf15SpellConeOfFire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"15b6abfce7d79d988b977f00c54718e4d5539a0cea1a258cd6e3eca1d08cda54","source":"sources/w10-actor-a-tf-15spellconeoffire/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf3Ant"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"339a63d0cb248caac616d9f43bbef0ffacd2896dc802e20167f3b1b8ee995c7e","source":"sources/w10-actor-a-tf-3ant/typeinfo.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"norrath_ctors_ant_lantern_view_42c760.o","profile":"authored_cpp","sha256":"d2cff9fa92b06b68432d1c3ab4e26be59ba777a318f9428a93a671f30822bc70","source":"sources/w10-actor-a-view-42c760/norrath_ctors_ant_lantern_view_42c760.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"norrath_ctors_ant_lantern_view_42c788.o","profile":"authored_cpp","sha256":"2dfc5e463c2d1b7ffe76901901eaac37b87970b13003365f03bcb480286bba0b","source":"sources/w10-actor-a-view-42c788/norrath_ctors_ant_lantern_view_42c788.cpp"}
```

## Imports closed

All of the following left the unresolved list in a link of my objects against the previous
partial build's unresolved list (30 of 32): `terminateSpell__8CreatureRC9type_info`,
`_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi`,
`_GLOBAL_$I$getRankDescription__9SkillBashPUsi`, `_GLOBAL_$I$getRankDescription__11SkillChargePUsi`,
`_GLOBAL_$I$getCriticalChance__16SkillCriticalHitP6Playeri`,
`_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi`,
`_GLOBAL_$I$getRankDescription__12SkillHealingPUsi`,
`_GLOBAL_$I$getRankDescription__15SkillHolyShieldPUsi`,
`_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi`,
`_GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi`,
`_GLOBAL_$I$getRankDescription__12SkillRipostePUsi`,
`_GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s`,
`_GLOBAL_$I$getRankDescription__15SkillShieldBashPUsi`,
`_GLOBAL_$I$getRankDescription__10SkillShockPUsi`,
`_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s`, `__tf15SpellConeOfFire`,
`_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s`, `__19SpellHornOmenEffectiP8CreatureP5Skill`,
`__22SpellPlanarSteelEffectiP8CreatureP5Skill`, `__15AnimationState8RC15AnimationState8`,
`_$_3Ant`, `__tf3Ant`, `clone__3Ant`, `msg_draw__3Ant`, `msg_load__3AntPc`, `msg_run__3Ant`,
`msg_save__3AntPc`, `setState__3Anti`, `norrath_ctors_ant_lantern_view_42c760`,
`norrath_ctors_ant_lantern_view_42c788`.

## Not closed (blocked)

- `norrath_ctors_ant_lantern_view_34b474` is the address 0x34b474, which is element 13 of the
  63-word table `skills` (0x34b440, 252 bytes). `norrath_ctors_ant_lantern_view_34b6b1` is the
  address 0x34b6b1, byte 0x51 inside `levelName` (0x34b660, 322 bytes). Both are interior
  addresses of arrays that other packages own, so a stand-alone definition here would be a
  second copy with different storage, not the original object. They can only be closed by the
  owner of `skills` and `levelName` providing offset aliases (or by changing the callers to use
  `skills` / `levelName` plus an offset). Not guessed.

## Imports opened

Symbols the new sources reference but do not define (others own them under these names, or
they are still open): `P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_CreateParticleSystem__Fv`, `_$_8Creature`, `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature`, `__8ListHeadRC8ListHead`, `__rtti_si`, `__rtti_user`, `__tf8Creature`, `__ti5Spell`, `__ti8Creature`, `_vt$10GameObject`, `_vt$19SpellHornOmenEffect`, `_vt$22SpellPlanarSteelEffect`, `_vt$3Ant`, `_vt$8Creature`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animGlobalSync__FP14AnimationState`, `ant_msg_run_particle_7d7170`, `bExitStatePersueIdle__8Creature`, `blockAlloc__Fi`, `blockFree__FPv`, `checkIfSpellIsRunning__8CreatureRC9type_info`, `creatureAdvanceAnimation__8Creaturef`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `dropTreasure__8CreatureG6Point3iffffffPc`, `eRand__10GameObject`, `enemyInZone__8Creaturefs`, `everyFrame__8CreatureP12_modelHeader`, `floorAdd__FG6Point3sfii`, `frameNm`, `getFreeFrameState__Fv`, `getSmartRandomDestination__8Creaturei`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `lightAdd__FP5Light`, `loseBodyPart__3Anti`, `lumpFindResource__FPcT0`, `move2__8CreatureG6Point3`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `newDamageHandler__8CreatureP12_modelHeader`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `resetRouting__8Creature`, `setStateProcessing__8CreaturePi`, `skills`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundPlaySequence__FP15AnimationHeaderG6Point3f`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `steerAndRouteToEnemy__8Creature`, `syncRandState`, `turnToFaceEnemy__8Creature`, `worldDifficulty`.
Notes: `ant_msg_run_particle_7d7170` stands for the file-local particle definition at 0x7d7170
(`ectopileParticle1` in the Ant unit; the Ant static initializer source defines a unit-local copy
that cannot be linked from another unit), so someone must give it a linkable name. `skills` and
`worldDifficulty` are the executable's data names. The `__ti5Spell`, `__ti8Creature`, `_vt$...`
symbols are type-info nodes and tables the authored code only addresses.

## Authored but never executed

All of it, on the EE. The private paired runs and the host contracts execute the sources only
in an interpreter or on a host. In particular the stack frames, spills and instruction
scheduling of the compiled objects differ from the original (compiled sizes differ), the
`msg_run__3Ant`/`setState__3Anti` virtual calls were exercised only with hooked targets, and
no scenario ran a real game object through them.

## What is unknown or deferred

- `P_AddParticle` / particle-definition ownership, the `item::description` record layout (only
  the written bytes are reproduced; bytes 0..15 and 34..35 stay untouched as in the original),
  and the Skill virtual at table+96 are all outside this package.
- The 15 static-initializer units were written in an earlier part of this work; their notes live
  in each unit's header comment and commit message.
