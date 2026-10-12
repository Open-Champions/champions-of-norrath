# Spell, skill and actor static initializers (2/2)

Package `w8-spell-static-init-b`, branch `agents/w8-spell-static-init-b`. Addresses are from the pinned private `SLUS_205.65` (`.text`, `.data`, `.bss`). Nothing here has run on the EE or as gameplay. Compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image.

## What the symbols are

**Observed.** Twenty-five of the names are the `.ctors` entries of the image (the list at 0x31f250 is in `link-image-bounds-ctors`). Each one is 32 bytes: it calls the translation unit's own local routine `__static_initialization_and_destruction_0` with the pair (1, 0xffff). That routine does nothing unless the second argument is 0xffff; then each step runs only when the first argument is 1. In 23 units the steps are constructions of 80-byte `ParticleDef`-named objects (the image's constructor `__11ParticleDefiiiiiiiifffiiiPFPv_v`, 0x180d78, takes this, eight integers, three floats, three integers and a null callback). The Projectile unit also builds four 16-byte `ResourceMap`-named objects from file names, and the `skills` unit makes 52 constructor calls on skill slots plus nine inline slot fills. The remaining five names are the table `skills` and four ordinary constructors.

**Inferred.** The object kinds come from the mangled constructor names. The slots hold skill objects because their constructors and `_vt$...Skill...` tables say so. The inline slot fills are inlined base-class constructions of the classes whose `_vt$` symbol they store.

**Unknown.** What the 14 numeric arguments of `ParticleDef` set, which code reads each particle object, and what the words of a skill slot mean beyond the five stored by the inline fills.

## Layout decisions

- Each initializer unit is one file `experimental/sources/w8-spell-static-init-b-<class>-<function>/init.cpp`. It defines the exported wrapper and a `noipa` static routine `norrath_static_init(initialize_p, priority)` that follows the image (the compare script calls it with all four combinations, which needs the routine to survive as a symbol).
- The particle objects are local statics in the image, with names that repeat across units (`sparkParticle` appears four times). Here each gets an external name `norrath_<class>_<function>_<image name>_bytes`, 80 bytes (16 for the resource maps), 16-byte aligned, because the units that use them are not authored. If another package authors the unit's real functions it must refer to these names rather than define its own.
- The skill slot block is `norrath_skill_slots[61]` (104 bytes each, spacing observed from 0x602908), and `skills` is the 63-word table with a zero at each end and the 61 slot addresses between, as the image stores it (0x34b440, 252 bytes). `slot.hpp` fixes the five stored fields.

## Evidence

`docs/agents/w8-spell-static-init-b-compare.py` runs the wrapper and the unit's own routine, from the private image and from the freshly built object, in the small R5900 interpreter of `combat-reactions-compare.py` (not modified). Every callee is a stub that logs the registers it really reads (objects by order of first use or by slot number, strings by content, stack words and float registers for `ParticleDef`). The logs must be equal for the wrapper and for each of (initialize, priority) in {0,1} x {0xffff,0}; slot writes (strings, vtable addresses and words) must be equal; the `skills` table must be equal slot for slot. Changing a float, an integer, a string, a slot index or a stored word in a source makes it fail.

`docs/agents/w8-spell-static-init-b-ctors-compare.py` does the same for the four constructors with randomized object memory, seeded globals (`syncRandState`, `frameNm`, `_4item$globalItem`) and stubbed callees; 800 runs each for HornOmen, PlanarSteel and ItemProp and 300 for the animation-state copy, zero mismatches. Mutating a constant, offset or mask in each source fails it, with two exceptions that are real equivalences: the byte store of 1 at offset 5 in the animation-state copy is overwritten by the copy that follows, and in ItemProp the second mask `~0x3000` only differs from `~0x2000` in a bit the earlier `~0x1800` already cleared. Both stay in the source to follow the image.

Both scripts need `NORRATH_EXE` (the private executable) and `NORRATH_EE` (the reviewed toolchain). The ctors script patches the shared interpreter so a float overflow or division by zero gives the EE's largest float (0x7fffffff) instead of an infinity; the EE has no infinities. That is also what the compiler folds `1.0f / 0.0f` to in the PlanarSteel constructor.

Build check: the 30 entries below were added to a private copy of the plan and snapshot (outside git) and built with `tools/build_partial.py` on this branch's base: 1195 unresolved imports became 1236, with all 29 package names closed and 71 names opened (listed below). `python3 -m unittest discover -s tools -p 'test_*.py'` passes.

## Imports closed

- `_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s`
- `_GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s`
- `_GLOBAL_$I$getCriticalChance__16SkillCriticalHitP6Playeri`
- `_GLOBAL_$I$getRankDescription__10SkillShockPUsi`
- `_GLOBAL_$I$getRankDescription__11SkillChargePUsi`
- `_GLOBAL_$I$getRankDescription__12SkillHealingPUsi`
- `_GLOBAL_$I$getRankDescription__12SkillRipostePUsi`
- `_GLOBAL_$I$getRankDescription__15SkillHolyShieldPUsi`
- `_GLOBAL_$I$getRankDescription__15SkillShieldBashPUsi`
- `_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi`
- `_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi`
- `_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi`
- `_GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi`
- `_GLOBAL_$I$getRankDescription__9SkillBashPUsi`
- `_GLOBAL_$I$move__10ProjectileG6Point3`
- `_GLOBAL_$I$msg_draw__10FrostShard`
- `_GLOBAL_$I$msg_run__12DiseaseTrail`
- `_GLOBAL_$I$msg_run__13DiseaseCorpse`
- `_GLOBAL_$I$msg_run__14FireStormFlare`
- `_GLOBAL_$I$msg_run__22ExplodeArrowProjectile`
- `_GLOBAL_$I$msg_run__7AntTrap`
- `_GLOBAL_$I$msg_run__9FireFlare`
- `_GLOBAL_$I$numInRegion__8creatureG6Point3f`
- `_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s`
- `_GLOBAL_$I$skills`
- `__15AnimationState8RC15AnimationState8`
- `__19SpellHornOmenEffectiP8CreatureP5Skill`
- `__22SpellPlanarSteelEffectiP8CreatureP5Skill`
- `__8ItemPropG6Point3PQ24item11descriptioni`
- `skills`

## Imports opened

These appear in the private build and were not unresolved before. Some are defined by other packages and need no work from this one (`lightAdd__FP5Light`, `_4item$globalItem` are authored elsewhere but were not in the plan when measured; the integrator's merged plan may already resolve them).

- `P_CreateParticleSystem__Fv`
- `_4item$globalItem`
- `__10SkillDodge`
- `__10SkillShock`
- `__11ParticleDefiiiiiiiifffiiiPFPv_v`
- `__11SkillCharge`
- `__12SkillArchery`
- `__12SkillBlessed`
- `__12SkillCyclone`
- `__12SkillHealing`
- `__12SkillLifeTap`
- `__12SkillRiposte`
- `__13SkillDarkness`
- `__13SkillEntangle`
- `__14SkillColdArrow`
- `__14SkillFireArrow`
- `__14SkillFireStorm`
- `__14SkillHarmTouch`
- `__14SkillMultiFire`
- `__15SkillClericRoot`
- `__15SkillConeOfFire`
- `__15SkillFireFlares`
- `__15SkillFrostStorm`
- `__15SkillHolyShield`
- `__15SkillHolyStrike`
- `__15SkillPoisonRain`
- `__15SkillShieldBash`
- `__15SkillWizardBeam`
- `__16SkillColdWeapons`
- `__16SkillConeOfFrost`
- `__16SkillCriticalHit`
- `__16SkillDiseaseBolt`
- `__16SkillFireWeapons`
- `__16SkillGroundPound`
- `__16SkillPoisonArrow`
- `__16SkillShockOfFire`
- `__17SkillConvertEnemy`
- `__17SkillDiseaseTrail`
- `__17SkillExplodeArrow`
- `__17SkillMinorHealing`
- `__17SkillRegeneration`
- `__17SkillShockOfFrost`
- `__17SkillUndeadShield`
- `__18SkillAncestralCall`
- `__18SkillBlindingLight`
- `__18SkillConvertUndead`
- `__18SkillDiseaseShield`
- `__18SkillDismissUndead`
- `__18SkillHammerOfWrath`
- `__18SkillRepulseUndead`
- `__19SkillSummonSkeleton`
- `__21SkillLightningWeapons`
- `__22SkillRangerCriticalHit`
- `__4PropG6Point3PcN22li`
- `__9SkillBash`
- `__9SkillRoot`
- `_vt$10SkillBlunt`
- `_vt$13SkillHateBeam`
- `_vt$13SkillSlashing`
- `_vt$15SkillBallOfHate`
- `_vt$15SkillHateEffect`
- `_vt$17SkillResistPoison`
- `_vt$18SkillDiseaseEffect`
- `_vt$18SkillPoisonWeapons`
- `_vt$19SkillDiseaseWeapons`
- `_vt$19SpellHornOmenEffect`
- `_vt$22SpellPlanarSteelEffect`
- `_vt$8ItemProp`
- `getFreeFrameState__Fv`
- `getProxyMeshId__4itemPQ24item11description`
- `lightAdd__FP5Light`

The 52 `__<N>Skill...` constructors and the nine `_vt$...Skill...` tables belong to the skill classes' owner. `P_CreateParticleSystem__Fv`, `getFreeFrameState__Fv`, `getProxyMeshId__4itemPQ24item11description` and `__4PropG6Point3PcN22li` are called by the new constructors.

## Build-plan entries needed

Profile `authored_cpp`; no `input_order` (the integrator assigns it). `sha256` is of the committed file at the branch tip. `header_closure` lists `slot.hpp` for the two skill units. Each unit also emits its static routine `norrath_static_init` as a local helper; the `emitted_functions` lists below come from compiling with the plan's own flags.

```json
{"emitted_functions":[{"compiled_body_bytes":440,"name":"__15AnimationState8RC15AnimationState8"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-animstate-copy.o","profile":"authored_cpp","sha256":"b9dff0af5aa550d3297f2d10c9df8367f3037a2dd288ef6b8ed8807d2e8ffac9","source":"sources/w8-spell-static-init-b-animstate-copy/copy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":248,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__7AntTrap"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-anttrap-msg_run.o","profile":"authored_cpp","sha256":"85fe23b9504998f02655d20fbaeb85bde89078e074e16ee901cdb3487f8d1979","source":"sources/w8-spell-static-init-b-anttrap-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":232,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$numInRegion__8creatureG6Point3f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-creature-numinregion.o","profile":"authored_cpp","sha256":"4fe2487e2867dc08d218bd63af8f1e6c29578651b7d38ce138579aea021da591","source":"sources/w8-spell-static-init-b-creature-numinregion/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":312,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__13DiseaseCorpse"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-diseasecorpse-msg_run.o","profile":"authored_cpp","sha256":"817b2fb3741222cd783517274c2919801796dec6649294324b05aa63bc0ed056","source":"sources/w8-spell-static-init-b-diseasecorpse-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__12DiseaseTrail"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-diseasetrail-msg_run.o","profile":"authored_cpp","sha256":"599c7ad3800971ea83a109289fd710828dee1b4d8ae66c3b845b7134ca50e8b6","source":"sources/w8-spell-static-init-b-diseasetrail-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__22ExplodeArrowProjectile"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-explodearrowprojectile-msg_run.o","profile":"authored_cpp","sha256":"4c51c1258f6c3a6e8bb6137bee983ea256907b924ca57a721b54a0a2e4f60d54","source":"sources/w8-spell-static-init-b-explodearrowprojectile-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__9FireFlare"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-fireflare-msg_run.o","profile":"authored_cpp","sha256":"ad27ac238e78e7a94489520d8d269e8c8d36d8c9985b20283ffeb2cbe42edc51","source":"sources/w8-spell-static-init-b-fireflare-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_run__14FireStormFlare"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-firestormflare-msg_run.o","profile":"authored_cpp","sha256":"4c53b02c8a883d8acec10ffb1a7e98cab5bff05cb8c05940060579679cabe4a7","source":"sources/w8-spell-static-init-b-firestormflare-msg_run/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":332,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$msg_draw__10FrostShard"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-frostshard-msg_draw.o","profile":"authored_cpp","sha256":"fe2b668750fa5aabbe308a61a79c4922f270a51d1a67e0ac0884c62d758bda6a","source":"sources/w8-spell-static-init-b-frostshard-msg_draw/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"__19SpellHornOmenEffectiP8CreatureP5Skill"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-hornomen-ctor.o","profile":"authored_cpp","sha256":"b736b9d0b5f17f27e9c117a5cb7288de1a8fbe58146aa26f2272bb0ae4939e1c","source":"sources/w8-spell-static-init-b-hornomen-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"__8ItemPropG6Point3PQ24item11descriptioni"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-itemprop-ctor.o","profile":"authored_cpp","sha256":"ad5dfd5637d44b6c4d2fc99ffcedaf494c0efa63fa99e217c47e3ec9327d9058","source":"sources/w8-spell-static-init-b-itemprop-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":400,"name":"__22SpellPlanarSteelEffectiP8CreatureP5Skill"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-planarsteel-ctor.o","profile":"authored_cpp","sha256":"b08d6821317e6a70bf3cca41c03e24032a5f8bca62aa6d04638f9a1c5ae0de74","source":"sources/w8-spell-static-init-b-planarsteel-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1484,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$move__10ProjectileG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-projectile-move.o","profile":"authored_cpp","sha256":"92ad321596f2924f6942fb702b0026299f66846801909a68e72c4498d3f4dad7","source":"sources/w8-spell-static-init-b-projectile-move/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":140,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillancestralcall-getrankdescription.o","profile":"authored_cpp","sha256":"7604265bb39a4d96dba7b5e07100fda63693ea7d2770116e87c67117bd125cce","source":"sources/w8-spell-static-init-b-skillancestralcall-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__9SkillBashPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillbash-getrankdescription.o","profile":"authored_cpp","sha256":"6ba5ca350dc14e19c87c5bb6ca734ec1b0a0c06695204819567347edfde38118","source":"sources/w8-spell-static-init-b-skillbash-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":356,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__11SkillChargePUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillcharge-getrankdescription.o","profile":"authored_cpp","sha256":"6b52d47e927902f1ca972ce8b8d31b85c930ef050c2d32a67d71fd1af92b5ede","source":"sources/w8-spell-static-init-b-skillcharge-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":148,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getCriticalChance__16SkillCriticalHitP6Playeri"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillcriticalhit-getcriticalchance.o","profile":"authored_cpp","sha256":"360fe3453adc16e5a1bc9efed6fbe1d16e9b6f629ba869fa7c11896f078dfd57","source":"sources/w8-spell-static-init-b-skillcriticalhit-getcriticalchance/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":684,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillgroundpound-getrankdescription.o","profile":"authored_cpp","sha256":"239762580df63b8da733db9d4fc65858a49ba49d0e0ad247f511663061e16c74","source":"sources/w8-spell-static-init-b-skillgroundpound-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":148,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__12SkillHealingPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillhealing-getrankdescription.o","profile":"authored_cpp","sha256":"cf76ba467a4381f5401ea62115e6d2345d2f2021b00656bd1752a789b0002e30","source":"sources/w8-spell-static-init-b-skillhealing-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__15SkillHolyShieldPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillholyshield-getrankdescription.o","profile":"authored_cpp","sha256":"3416d5d14c412f8eb9ebf6053301e0df0d7066ae033a339eee6d4464a85de30a","source":"sources/w8-spell-static-init-b-skillholyshield-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":336,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillpoisonarrow-getrankdescription.o","profile":"authored_cpp","sha256":"47a98e0b7de5c1fbeab7d3ed86811bec259e3843b3e4534fa4a9475965f5f5eb","source":"sources/w8-spell-static-init-b-skillpoisonarrow-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":148,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillrangercriticalhit-getrankdescription.o","profile":"authored_cpp","sha256":"78f43b074855be38f2257d1fcfcd1293b3914d8e2837f08490c0ec7c4024d780","source":"sources/w8-spell-static-init-b-skillrangercriticalhit-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__12SkillRipostePUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillriposte-getrankdescription.o","profile":"authored_cpp","sha256":"f584a87145dca3781bc17f42e421526efe80b10a4c09f002e7cdee6871a3fafd","source":"sources/w8-spell-static-init-b-skillriposte-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":356,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillroot-cast.o","profile":"authored_cpp","sha256":"eba40a3e347fa0f8d6f89164fa015b172dbe2e98deff2b01f4fe29a4cedaf2b9","source":"sources/w8-spell-static-init-b-skillroot-cast/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1048,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$skills"}],"header_closure":["sources/w8-spell-static-init-b-skills-table/slot.hpp"],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skills-init.o","profile":"authored_cpp","sha256":"331c26c38b50da1b5754c96a2247f0c88ad95cf20c4fb27abd1f16bf7dc993cd","source":"sources/w8-spell-static-init-b-skills-init/init.cpp"}
{"emitted_functions":[],"header_closure":["sources/w8-spell-static-init-b-skills-table/slot.hpp"],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skills-table.o","profile":"authored_cpp","sha256":"abcf40136bf864b19173a54e98c07149826cb4a0a4a064dad0407cd7bce7ec07","source":"sources/w8-spell-static-init-b-skills-table/skills.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__15SkillShieldBashPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillshieldbash-getrankdescription.o","profile":"authored_cpp","sha256":"c06a911087bd38ef04a6f5d83fb4028db29ae6e051007c8ed2d86e3a66d7645d","source":"sources/w8-spell-static-init-b-skillshieldbash-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":232,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$getRankDescription__10SkillShockPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillshock-getrankdescription.o","profile":"authored_cpp","sha256":"73bec765dad86eb216d113c6a52232900ab029a1cb93ad071c574a85afae457e","source":"sources/w8-spell-static-init-b-skillshock-getrankdescription/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":232,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-skillsummonskeleton-cast.o","profile":"authored_cpp","sha256":"e671199a7e35bd7e370309a4179b5241590e0e59d3a3dee97e6320cd04917777","source":"sources/w8-spell-static-init-b-skillsummonskeleton-cast/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"norrath_static_init"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w8-spell-static-init-b-spellhornomeneffect-run.o","profile":"authored_cpp","sha256":"43dc31c93427df56c7bb67a05fda683c783526b879e3dccab6dbc515249bba26","source":"sources/w8-spell-static-init-b-spellhornomeneffect-run/init.cpp"}
```

## Authored but never executed

All of it on real hardware or in the game. The initializer units and constructors have only run inside the comparison interpreter, with the callees stubbed. The particle objects, resource maps and skill slots are only ever written by these units here; whatever reads them is unauthored. The `ParticleDef` constructor, the 52 skill constructors, `ResourceMap`, `Prop`, `lightAdd`, the sound and lump calls and `getFreeFrameState` are all stubs in the comparison.

## Unknown

- What the 14 `ParticleDef` arguments mean, what the 16-byte `ResourceMap` objects are used for, and who reads each object.
- Which skill number each slot is beyond the construction order; the meaning of the slot words and of the halfword 30 in the inline fills.
- The Light record fields in the PlanarSteel constructor and the halfword scale at 28; the meaning of the bits tested in the ItemProp constructor and of the animation-state flag bits 0x4800.
- Whether the original sources had named, shared definitions for the particle objects; the external names here are this package's, not the original's.
