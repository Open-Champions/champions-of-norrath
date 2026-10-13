# w11-ctor-c: skill, spell and projectile constructors (3 of 3)

Branch `agents/w11-ctor-c`. Private reference: `work/private/SLUS_205.65` (the image the sibling
packages in this repository pin, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes, disassembly or tool output are committed.
The only game data stated in the sources are short identifiers (class-name texts such as `riposte`,
resource file names, one level-name text) and single-precision constants, each given as bits.

Sources live in `experimental/sources/w11-ctor-c-*`. Every directory has the authored source and a
`host_check.cpp` (not an EE input; builds with `c++ -std=c++17 -Wall -Werror host_check.cpp`, all 26 pass
on this machine, arm64 macOS clang 21). Each source was also compiled with the reviewed EE g++ `authored_cpp`
flags and run against the original function in a private R5900 interpreter harness (outside git): the
original and the authored function ran from identical memory snapshots with identical stub callees, and
call logs (arguments, plus a digest or copy of the object at each call), return registers and every changed
byte outside the stack were compared. Mutation checks done: one changed constant fails the paired run for `SkillRoot` and
`SpellDiseaseShield` and the host check of `SpellDiseaseShield` and `UndeadKnightClone`; the paired run
caught a real error of mine in `UndeadKnightClone` (a doubleword constant) before the commit. The paired run covered 191 of 191 original words of
`SpellDiseaseShield`, 250 of 251 of `UndeadKnightClone` (the one miss is padding behind an unconditional
branch), and all words of the straight-line functions. Nothing ran on an EE or in gameplay. Compiled
bodies differ in size from the image; no byte match is claimed.

`python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK. `tools/check_source_snapshot.py` fails
with "Unrecorded or missing source files" because the new directories are not in `snapshot.json`
(shared file, not edited here), the same as the other agent branches.

## Functions

| source dir (`experimental/sources/`) | symbol | image | compiled bytes | sha256 |
|---|---|---|---|---|
| `w11-ctor-c-beetle-projectile` | `__16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader` | `0x212520`, 244 bytes | 220 | `299386711064dc2a9c4929721a43b72994f9a0b146c26a64e3b93b62daa626a9` |
| `w11-ctor-c-dust-cloud` | `__9dustCloudG6Point3iiii` | `0x1e6828`, 228 bytes | 200 | `8e6803797fb2d9bcedda385c8b000f0ca3408e5988d76e4dff550a9f1f86a306` |
| `w11-ctor-c-poison-gas-spell` | `__14poisonGasSpellG6Point3iiiP10GameObjecti` | `0x1e6708`, 276 bytes | 252 | `f00b3ee7024d1d55e22ed30b0a8424397f88183f36ad1e57d3424e6e53c8106d` |
| `w11-ctor-c-skill-life-tap` | `__12SkillLifeTap` | `0x2be1b8`, 48 bytes | 48 | `9cc35a8723b50404cdb992f5a5dbd48666e7c33a69e7db137b5a95ffb8e0354b` |
| `w11-ctor-c-skill-lightning-weapons` | `__21SkillLightningWeapons` | `0x2a9d20`, 44 bytes | 44 | `2e63d1b41958fc7a6fbe8c5d9f19438157c794bc1b82992802671f28b7aea017` |
| `w11-ctor-c-skill-minor-healing` | `__17SkillMinorHealing` | `0x2a72c0`, 44 bytes | 44 | `e36410d0c19626ddfb36e701ee12d20d77c79d86ca94905fcc697d911c9ea1c9` |
| `w11-ctor-c-skill-multi-fire` | `__14SkillMultiFire` | `0x2a1bb0`, 48 bytes | 48 | `e4593c4f6db06bd1e27d2a5651cff3974705ada8083035907d08afbc8332abb0` |
| `w11-ctor-c-skill-poison-arrow` | `__16SkillPoisonArrow` | `0x2a0b80`, 48 bytes | 48 | `a5677f714c593cd30b535c4f71d91d73b4e0c9a0b3256254240d1ff7aa866461` |
| `w11-ctor-c-skill-poison-rain` | `__15SkillPoisonRain` | `0x2bf198`, 44 bytes | 44 | `f599cb0645a8f5f2992716b670eae32078bc8e84d48c97b808574d9baddabd9f` |
| `w11-ctor-c-skill-ranger-critical-hit` | `__22SkillRangerCriticalHit` | `0x2c7bd0`, 44 bytes | 44 | `4eb9c31c51ac1b81615aa456074f7ac7bee538bbba1b8a3ce190f0e77b8edc31` |
| `w11-ctor-c-skill-regeneration` | `__17SkillRegeneration` | `0x2b6b58`, 48 bytes | 48 | `2d97bb6c17a905d72fab14b9c74e101454b73f62c0f061754fde0a125bc1fb5f` |
| `w11-ctor-c-skill-repulse-undead` | `__18SkillRepulseUndead` | `0x2ba600`, 44 bytes | 44 | `366e1d7f7f2c68aab06f2c38fe56b068715fe64ffe362b59a7553492279634ca` |
| `w11-ctor-c-skill-riposte` | `__12SkillRiposte` | `0x2a5058`, 48 bytes | 48 | `e81a960d64a621982846645ec774e021fc45ecc1d8fc7f922f7645e52a26d1df` |
| `w11-ctor-c-skill-root` | `__9SkillRoot` | `0x2ace88`, 44 bytes | 44 | `13a9c84117fd3f3a4288934091e1b3ffed8969437cbc03f07775b6244ea60cf2` |
| `w11-ctor-c-skill-shield-bash` | `__15SkillShieldBash` | `0x2c89a8`, 44 bytes | 44 | `32cf89baa4f6c1007dde4cce4bda61b537cfd90afb70e3fb01272b1e0d55805a` |
| `w11-ctor-c-skill-shock-of-fire` | `__16SkillShockOfFire` | `0x2b3170`, 44 bytes | 44 | `06a134677a9eba97ae52c8b1a71f2b8d097f193655d12b5bfa13274a60c0435d` |
| `w11-ctor-c-skill-shock-of-frost` | `__17SkillShockOfFrost` | `0x2be008`, 44 bytes | 44 | `5e147ec13c01de05f13388684e0facc386a307aa4dffb13583efb092b73295f8` |
| `w11-ctor-c-skill-shock` | `__10SkillShock` | `0x28d810`, 44 bytes | 44 | `9f7d6f5e44c075685757f6c1e80d5b9946228a72341b7208c36978ce07cea68b` |
| `w11-ctor-c-skill-summon-skeleton` | `__19SkillSummonSkeleton` | `0x2a9040`, 44 bytes | 44 | `e315442390f62f4ec834b0b0bbcbfd6dabf913147e6377c8018f0748d2439b37` |
| `w11-ctor-c-skill-undead-shield` | `__17SkillUndeadShield` | `0x2b11d0`, 44 bytes | 44 | `d50e00b1d4293f5b844656dc6f5ea6fcd7ddf7afd616343866f785ee33f587cb` |
| `w11-ctor-c-skill-wizard-beam` | `__15SkillWizardBeam` | `0x2b9368`, 48 bytes | 48 | `85b37e7df744b11afa27c876585bf2cc71a6309ce2c90b8551a96b696c71c33b` |
| `w11-ctor-c-spawn-mummy` | `__10SpawnMummyG6Point3PcP10GameObject` | `0x23c150`, 228 bytes | 196 | `1138f73f90a5475ea3762c6f00b845e50f4efff40131aeae9f814ac1e628d3ec` |
| `w11-ctor-c-spell-cone-of-fire` | `__15SpellConeOfFireiP8CreatureP5Skill` | `0x2b5898`, 360 bytes | 300 | `e580d0b3ab51902894bc9ab96367a5b3b433bcef9ae8a8d631ea7c617ce4282f` |
| `w11-ctor-c-spell-disease-shield` | `__18SpellDiseaseShieldiP8CreatureP5Skill` | `0x2955c0`, 764 bytes | 720 | `e0a22f9bf50d85828b93aa6ebba3c33a49fbc64530a4f798802033f450038c96` |
| `w11-ctor-c-spell-harm-touch` | `__14SpellHarmTouchiP8CreatureP5Skill` | `0x29b0d0`, 160 bytes | 164 | `af8c80ad61ceddbc801ca615b8c47df58f5f4703a667d7ed24f88597f478c0a9` |
| `w11-ctor-c-undead-knight-clone` | `__17UndeadKnightCloneG6Point3sPPc` | `0x23d798`, 1004 bytes | 868 | `13c9e647ccb2593249b6e915babd5b87bf2fddb9d15255134609295e63d0c843` |
| `w11-ctor-c-skill-life-tap-get-chance-check` | `getChance__12SkillLifeTapi` (source `w10-item-a-skill-life-tap-get-chance/get_chance.cpp`, not mine) | `0x2be340`, 108 bytes | 104 | check only |

## Notes per function (observed / inferred / unknown)

**Skill constructors (18).** Observed: each writes the address of a lower-case class-name text at +0,
zero at +4, a value at +8 (3 for `lifetap`, `regeneration`, `riposte`; 8 for `multifire`, `poisonarrow`;
1 for `wizardbeam`; 0 for the other twelve), the halfword 30 at +12, and the class table (`_vt$<class>`)
at +96. Nothing else is read or written, the return register is not meaningful (declared `void`). The
callers (`_GLOBAL_$I$skills` in `w10-item-a-skills`) pass a pointer to a 104-byte record. Inferred:
the +0 word is the skill's name string. Unknown: what +8 and the halfword at +12 mean. The authored
name texts are string literals, so their addresses differ from the image's; the paired run compares
the text, not the address.

**`getChance__12SkillLifeTapi`.** The source already exists (w10-item-a, unselected because it had no
committed check). This package adds only `w11-ctor-c-skill-life-tap-get-chance-check/host_check.cpp` (it
includes that source) and a paired run (two table entries at table+120 and table+88, returned float).

**`__9dustCloudG6Point3iiii`, `__14poisonGasSpellG6Point3iiiP10GameObjecti`.** Observed: GameObject
constructor, own table, a copy of the position, an end frame (frameNm + argument), a clamp of one int
to at least 10 (signed), flags |= 3, 10.0f, `objectAddToRunList__FP10GameObject`; the gas spell also
clears a word that heads a one-way list and links its GameObject argument with
`listOnewayLink__FP8ListHeadT0Pv` when it is not null. Inferred: the list head and the end-frame
meaning. Unknown: the meaning of the ints and of the float.

**`__16beetleProjectile...`.** Observed: `MissileWeapon` constructor called with 17 arguments (nulls,
both positions, the int, the creature, five ints, the int 4, null description), own table, a cleared list
head at +452, link of the creature when not null, header pointer at +456. Unknown: meanings of the ints.

**`__10SpawnMummyG6Point3PcP10GameObject`.** Observed: it contains the inlined `DustDevil` constructor
steps (DustDevil table, particle system at +84, end frame +80 = frameNm + 60, run list) and then
switches to its own table; the link of the GameObject argument is not null-tested. Inferred: DustDevil is
the parent (from the table symbol). Unknown: what the text pointer at +88 is used for.

**`__14SpellHarmTouch...`, `__15SpellConeOfFire...`, `__18SpellDiseaseShield...`.** Observed: the Spell
base constructor inlined (layout in `w10-actor-a-ctor-spell-horn-omen-effect`), then per-spell work
described in each source's header comment (resource and sound calls; a light record at +24 for the cone;
two arrays of 16 rejection-sampled points for the disease shield). The `1.0f / 0.0f` stored by the cone is a
run-time divide in the image; the source keeps it at run time (empty `asm` on the divisor) because a
folded constant would be infinity while the EE divide gives its largest float (the interpreter models
0x7fffffff; real hardware not run). In the authored build the compiler drops the first store of the `Spell` table
at +20 (it is overwritten before anything can read it); the image keeps it. No call or read sits in between, so
the paired call logs agree.

**`__17UndeadKnightCloneG6Point3sPPc`.** Observed: see the header comment of the source. Two counted
loops with empty bodies in the image are not reproduced. The record at +2624 receives the same
initialisation the Mummy constructor gives its record at +1032. Unknown: the meaning of the numeric
parameters, the light fields and what the `battle3` test selects. `levelName + 81` is the text compared.

## Build-plan entries needed

Profile `authored_cpp`, no header closure. `input_order` is left for the integrator. Do not add
`host_check.cpp`; `snapshot.json` must list the new files.

```json
{"emitted_functions": [{"compiled_body_bytes": 220, "name": "__16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_beetle_projectile.o", "profile": "authored_cpp", "sha256": "299386711064dc2a9c4929721a43b72994f9a0b146c26a64e3b93b62daa626a9", "source": "sources/w11-ctor-c-beetle-projectile/beetle_projectile_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "__9dustCloudG6Point3iiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_dust_cloud.o", "profile": "authored_cpp", "sha256": "8e6803797fb2d9bcedda385c8b000f0ca3408e5988d76e4dff550a9f1f86a306", "source": "sources/w11-ctor-c-dust-cloud/dust_cloud_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 252, "name": "__14poisonGasSpellG6Point3iiiP10GameObjecti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_poison_gas_spell.o", "profile": "authored_cpp", "sha256": "f00b3ee7024d1d55e22ed30b0a8424397f88183f36ad1e57d3424e6e53c8106d", "source": "sources/w11-ctor-c-poison-gas-spell/poison_gas_spell_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__12SkillLifeTap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_life_tap.o", "profile": "authored_cpp", "sha256": "9cc35a8723b50404cdb992f5a5dbd48666e7c33a69e7db137b5a95ffb8e0354b", "source": "sources/w11-ctor-c-skill-life-tap/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__21SkillLightningWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_lightning_weapons.o", "profile": "authored_cpp", "sha256": "2e63d1b41958fc7a6fbe8c5d9f19438157c794bc1b82992802671f28b7aea017", "source": "sources/w11-ctor-c-skill-lightning-weapons/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__17SkillMinorHealing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_minor_healing.o", "profile": "authored_cpp", "sha256": "e36410d0c19626ddfb36e701ee12d20d77c79d86ca94905fcc697d911c9ea1c9", "source": "sources/w11-ctor-c-skill-minor-healing/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__14SkillMultiFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_multi_fire.o", "profile": "authored_cpp", "sha256": "e4593c4f6db06bd1e27d2a5651cff3974705ada8083035907d08afbc8332abb0", "source": "sources/w11-ctor-c-skill-multi-fire/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__16SkillPoisonArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_poison_arrow.o", "profile": "authored_cpp", "sha256": "a5677f714c593cd30b535c4f71d91d73b4e0c9a0b3256254240d1ff7aa866461", "source": "sources/w11-ctor-c-skill-poison-arrow/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__15SkillPoisonRain"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_poison_rain.o", "profile": "authored_cpp", "sha256": "f599cb0645a8f5f2992716b670eae32078bc8e84d48c97b808574d9baddabd9f", "source": "sources/w11-ctor-c-skill-poison-rain/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__22SkillRangerCriticalHit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_ranger_critical_hit.o", "profile": "authored_cpp", "sha256": "4eb9c31c51ac1b81615aa456074f7ac7bee538bbba1b8a3ce190f0e77b8edc31", "source": "sources/w11-ctor-c-skill-ranger-critical-hit/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__17SkillRegeneration"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_regeneration.o", "profile": "authored_cpp", "sha256": "2d97bb6c17a905d72fab14b9c74e101454b73f62c0f061754fde0a125bc1fb5f", "source": "sources/w11-ctor-c-skill-regeneration/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__18SkillRepulseUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_repulse_undead.o", "profile": "authored_cpp", "sha256": "366e1d7f7f2c68aab06f2c38fe56b068715fe64ffe362b59a7553492279634ca", "source": "sources/w11-ctor-c-skill-repulse-undead/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__12SkillRiposte"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_riposte.o", "profile": "authored_cpp", "sha256": "e81a960d64a621982846645ec774e021fc45ecc1d8fc7f922f7645e52a26d1df", "source": "sources/w11-ctor-c-skill-riposte/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__9SkillRoot"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_root.o", "profile": "authored_cpp", "sha256": "13a9c84117fd3f3a4288934091e1b3ffed8969437cbc03f07775b6244ea60cf2", "source": "sources/w11-ctor-c-skill-root/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__15SkillShieldBash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_shield_bash.o", "profile": "authored_cpp", "sha256": "32cf89baa4f6c1007dde4cce4bda61b537cfd90afb70e3fb01272b1e0d55805a", "source": "sources/w11-ctor-c-skill-shield-bash/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__16SkillShockOfFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_shock_of_fire.o", "profile": "authored_cpp", "sha256": "06a134677a9eba97ae52c8b1a71f2b8d097f193655d12b5bfa13274a60c0435d", "source": "sources/w11-ctor-c-skill-shock-of-fire/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__17SkillShockOfFrost"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_shock_of_frost.o", "profile": "authored_cpp", "sha256": "5e147ec13c01de05f13388684e0facc386a307aa4dffb13583efb092b73295f8", "source": "sources/w11-ctor-c-skill-shock-of-frost/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__10SkillShock"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_shock.o", "profile": "authored_cpp", "sha256": "9f7d6f5e44c075685757f6c1e80d5b9946228a72341b7208c36978ce07cea68b", "source": "sources/w11-ctor-c-skill-shock/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__19SkillSummonSkeleton"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_summon_skeleton.o", "profile": "authored_cpp", "sha256": "e315442390f62f4ec834b0b0bbcbfd6dabf913147e6377c8018f0748d2439b37", "source": "sources/w11-ctor-c-skill-summon-skeleton/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "__17SkillUndeadShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_undead_shield.o", "profile": "authored_cpp", "sha256": "d50e00b1d4293f5b844656dc6f5ea6fcd7ddf7afd616343866f785ee33f587cb", "source": "sources/w11-ctor-c-skill-undead-shield/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "__15SkillWizardBeam"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_skill_wizard_beam.o", "profile": "authored_cpp", "sha256": "85b37e7df744b11afa27c876585bf2cc71a6309ce2c90b8551a96b696c71c33b", "source": "sources/w11-ctor-c-skill-wizard-beam/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 196, "name": "__10SpawnMummyG6Point3PcP10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_spawn_mummy.o", "profile": "authored_cpp", "sha256": "1138f73f90a5475ea3762c6f00b845e50f4efff40131aeae9f814ac1e628d3ec", "source": "sources/w11-ctor-c-spawn-mummy/spawn_mummy_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 300, "name": "__15SpellConeOfFireiP8CreatureP5Skill"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_spell_cone_of_fire.o", "profile": "authored_cpp", "sha256": "e580d0b3ab51902894bc9ab96367a5b3b433bcef9ae8a8d631ea7c617ce4282f", "source": "sources/w11-ctor-c-spell-cone-of-fire/spell_cone_of_fire_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 720, "name": "__18SpellDiseaseShieldiP8CreatureP5Skill"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_spell_disease_shield.o", "profile": "authored_cpp", "sha256": "e0a22f9bf50d85828b93aa6ebba3c33a49fbc64530a4f798802033f450038c96", "source": "sources/w11-ctor-c-spell-disease-shield/spell_disease_shield_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 164, "name": "__14SpellHarmTouchiP8CreatureP5Skill"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_spell_harm_touch.o", "profile": "authored_cpp", "sha256": "af8c80ad61ceddbc801ca615b8c47df58f5f4703a667d7ed24f88597f478c0a9", "source": "sources/w11-ctor-c-spell-harm-touch/spell_harm_touch_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 868, "name": "__17UndeadKnightCloneG6Point3sPPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_ctor_c_undead_knight_clone.o", "profile": "authored_cpp", "sha256": "13c9e647ccb2593249b6e915babd5b87bf2fddb9d15255134609295e63d0c843", "source": "sources/w11-ctor-c-undead-knight-clone/undead_knight_clone_ctor.cpp"}
```

## Imports closed

`__12SkillLifeTap`, `__21SkillLightningWeapons`, `__17SkillMinorHealing`, `__14SkillMultiFire`,
`__16SkillPoisonArrow`, `__15SkillPoisonRain`, `__22SkillRangerCriticalHit`, `__17SkillRegeneration`,
`__18SkillRepulseUndead`, `__12SkillRiposte`, `__9SkillRoot`, `__15SkillShieldBash`, `__10SkillShock`,
`__16SkillShockOfFire`, `__17SkillShockOfFrost`, `__19SkillSummonSkeleton`, `__17SkillUndeadShield`,
`__15SkillWizardBeam`, `__10SpawnMummyG6Point3PcP10GameObject`, `__15SpellConeOfFireiP8CreatureP5Skill`,
`__18SpellDiseaseShieldiP8CreatureP5Skill`, `__14SpellHarmTouchiP8CreatureP5Skill`,
`__17UndeadKnightCloneG6Point3sPPc`, `__16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader`,
`__9dustCloudG6Point3iiii`, `__14poisonGasSpellG6Point3iiiP10GameObjecti`.
`getChance__12SkillLifeTapi` was already authored (w10-item-a, source in the snapshot) but its plan entry is
missing; this package only adds its check. Closing it needs the integrator to add the existing unit to the plan
(the entry is in `docs/agents/w10-item-a.md` line 207).

## Imports opened

Symbols the new units reference and that this package does not define (some may be defined by other
packages or already be in the plan; I did not run a full build): functions
`__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`,
`__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`,
`__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc`, `P_CreateParticleSystem__Fv`,
`animAdd__FP14AnimationStateP15AnimationHeaderi`, `animInitState__FP14AnimationState`, `eRand__5Spell`,
`finishConstruction__8Creature`, `lightAdd__FP5Light`, `listOnewayLink__FP8ListHeadT0Pv`,
`lumpFindResource__FPcT0`, `objectAddToRunList__FP10GameObject`, `objectAddToSlowRunList__FP10GameObject`,
`setAnimEventFlags__8Creatureififififi`, `setParameters__8Creaturefffffi`, `soundMakePersistant__Fi`,
`soundPlaySequence__FP15AnimationHeaderG6Point3f`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`strcmp`; data `frameNm`, `levelName`, `syncRandState`, `udkResourceMap`; tables
`_vt$<class>` for the 18 skills plus `_vt$9dustCloud`, `_vt$14poisonGasSpell`, `_vt$16beetleProjectile`,
`_vt$10SpawnMummy`, `_vt$9DustDevil`, `_vt$14SpellHarmTouch`, `_vt$15SpellConeOfFire`,
`_vt$18SpellDiseaseShield`, `_vt$17UndeadKnightClone`. (`_vt$5Spell` is declared in three sources but the
authored build drops the store, so it is not an import.)

## Authored but never executed

All 26 units. They compiled with the reviewed EE g++ and ran only in a private R5900 interpreter (and the
host checks on arm64 macOS). The interpreter models float divide-by-zero as 0x7fffffff; no EE hardware,
game executable link or gameplay run exists. The host checks of `UndeadKnightClone` and the skill
constructors use a 64-bit host layout, and the structures there are compared field by field, not by
byte layout; the EE offsets are asserted by `static_assert` where the layout is a plain struct and by the
paired runs otherwise.

## Unknown

Meaning of the +8 value and the halfword 30 in the skill records; every integer parameter of the
projectile, cloud and gas constructors and of the creature numeric parameters; what the light records
control; why the image has two empty loops in `UndeadKnightClone`; which level the `battle3` text names;
whether the divide-by-zero result on real hardware equals the interpreter's model; the types of the
functions whose return values are not used (declared `unsigned`/`void` only to fit the call sites).
