# Skill energy cost, level requirement, point cost and icon accessors, part 4 (w13-skill-stat-d)

Branch: `agents/w13-skill-stat-d`. All files are independently authored. Private reference: hash-pinned executable `SLUS-20565-sha256-9a60f13c5c31`. No game binary, raw disassembly, decompiler output or extracted asset is included.

## Observed and checked

Each of the 37 functions below was authored from its complete private symbol extent, compiled separately with the reviewed EE toolchain (`authored_cpp` flags), and tested through a host contract, one commit per function. These are semantic reconstructions, not original-byte matches (the compiled size differs from the original for most, because the original uses gp-relative literals and its own string placement).

| Function | Original address / bytes | Compiled bytes | Source | SHA-256 |
|---|---:|---:|---|---|
| `getSkillPointCost__18SkillBlindingLighti` | `0x002ba178` / 8 | 8 | `experimental/sources/w13-skill-stat-d-blinding-light-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-blinding-light-point-cost/host_check.cpp` | `46d59e4bd69ef1ee7a7d2441bc35c3cdb3b873ac0f4e6a349f2e9e5c63106409` |
| `getSkillPointCost__18SkillDiseaseShieldi` | `0x002968a0` / 8 | 8 | `experimental/sources/w13-skill-stat-d-disease-shield-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-disease-shield-point-cost/host_check.cpp` | `a26da00a8f1348b2697620eed3d2e92de6953ac61f6825fbc22321147420e09a` |
| `getLevelRequirement__18SkillDiseaseShieldi` | `0x002968a8` / 8 | 8 | `experimental/sources/w13-skill-stat-d-disease-shield-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-disease-shield-level-requirement/host_check.cpp` | `af7d620f1f31deced68264ca699cb54be699a035bd9df3383a5f92ca2caf585e` |
| `getSkillPointCost__18SkillDismissUndeadi` | `0x002bbd20` / 8 | 8 | `experimental/sources/w13-skill-stat-d-dismiss-undead-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-dismiss-undead-point-cost/host_check.cpp` | `5edf8772c86773c9b1597fdc351255b08d064bbee68234e55afdff221d20b936` |
| `getLevelRequirement__18SkillDismissUndeadi` | `0x002bbd28` / 8 | 8 | `experimental/sources/w13-skill-stat-d-dismiss-undead-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-dismiss-undead-level-requirement/host_check.cpp` | `97760c2930c4ef48e1d32318435f40cca209b9d9a1bfaf4efe761c463f26aa59` |
| `getSkillPointCost__18SkillHammerOfWrathi` | `0x002b5808` / 8 | 8 | `experimental/sources/w13-skill-stat-d-hammer-of-wrath-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-hammer-of-wrath-point-cost/host_check.cpp` | `797d1f851fb67d8115209cb2680944dfad20c83f3697d6c610da3a63da9839c7` |
| `getLevelRequirement__18SkillHammerOfWrathi` | `0x002b5810` / 8 | 8 | `experimental/sources/w13-skill-stat-d-hammer-of-wrath-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-hammer-of-wrath-level-requirement/host_check.cpp` | `6f40933366f260534e744360bda088d252826564a00b578cdddbc31db5ee8d94` |
| `getSkillPointCost__18SkillRepulseUndeadi` | `0x002ba730` / 8 | 8 | `experimental/sources/w13-skill-stat-d-repulse-undead-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-repulse-undead-point-cost/host_check.cpp` | `b45b38414edb2f154d82af5867aa66591cf3d44d0d935a7371ea2fe36d7e099c` |
| `getLevelRequirement__18SkillRepulseUndeadi` | `0x002ba738` / 8 | 8 | `experimental/sources/w13-skill-stat-d-repulse-undead-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-repulse-undead-level-requirement/host_check.cpp` | `4a2c98bc9c46531420454de456948059d720c648eb4d85ff6af0c2dd081c8b35` |
| `getSkillPointCost__19SkillSummonSkeletoni` | `0x002a9170` / 8 | 8 | `experimental/sources/w13-skill-stat-d-summon-skeleton-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-summon-skeleton-point-cost/host_check.cpp` | `7a1b9c39f588119e7b3db6e08030cc709ac8ce5f7e854e449db8c2307bbc83e8` |
| `getLevelRequirement__19SkillSummonSkeletoni` | `0x002a9178` / 8 | 8 | `experimental/sources/w13-skill-stat-d-summon-skeleton-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-summon-skeleton-level-requirement/host_check.cpp` | `9e4aadc34f6b311ad611b993f76c494b170afc2b1f8fa2c50b77f02e460da429` |
| `getSkillPointCost__21SkillLightningWeaponsi` | `0x002a9e48` / 8 | 8 | `experimental/sources/w13-skill-stat-d-lightning-weapons-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-lightning-weapons-point-cost/host_check.cpp` | `f080cee04800bf21ca746adacbdee962dd4dc19c5de19d3a1737bfb3906c23b3` |
| `getLevelRequirement__21SkillLightningWeaponsi` | `0x002a9e50` / 8 | 8 | `experimental/sources/w13-skill-stat-d-lightning-weapons-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-lightning-weapons-level-requirement/host_check.cpp` | `4dc2ade11fcf64e81e09182cc733aeb7296ea4531c2a901279594d4533cc7cd4` |
| `getEnergyCost__18SkillDiseaseShieldi` | `0x002968b0` / 36 | 40 | `experimental/sources/w13-skill-stat-d-disease-shield-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-disease-shield-energy-cost/host_check.cpp` | `8f4924f4651e8f2c151c4c0f1a307529bf9967f407ac7755f506718de923bf91` |
| `getEnergyCost__18SkillDismissUndeadi` | `0x002bbd30` / 36 | 40 | `experimental/sources/w13-skill-stat-d-dismiss-undead-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-dismiss-undead-energy-cost/host_check.cpp` | `4f9e12c87bd6d494ad72cc79b6625fd4157593b5c5f1d75ea4116dfee76d2037` |
| `getEnergyCost__18SkillHammerOfWrathi` | `0x002b5818` / 40 | 40 | `experimental/sources/w13-skill-stat-d-hammer-of-wrath-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-hammer-of-wrath-energy-cost/host_check.cpp` | `69eebd03eb2a24d038bc34c90691472178fe27f2504ba2d81f2efd58079b2536` |
| `getEnergyCost__18SkillRepulseUndeadi` | `0x002ba740` / 40 | 40 | `experimental/sources/w13-skill-stat-d-repulse-undead-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-repulse-undead-energy-cost/host_check.cpp` | `c22eb207ed77b161778c5e488589efdd497debb82d725e01690c460dee27d82d` |
| `getEnergyCost__19SkillSummonSkeletoni` | `0x002a9180` / 40 | 40 | `experimental/sources/w13-skill-stat-d-summon-skeleton-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-summon-skeleton-energy-cost/host_check.cpp` | `c0bd1bd71fdeb656e8544580581ab813c7456771ef31b7b97b900a4a4cf54454` |
| `getEnergyCost__21SkillLightningWeaponsi` | `0x002a9e58` / 40 | 40 | `experimental/sources/w13-skill-stat-d-lightning-weapons-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-lightning-weapons-energy-cost/host_check.cpp` | `a4dd0a637f67b65cba03e24292ece73f2055c65c0cbb0ffe9d7b92bba3976d9e` |
| `getEnergyCost__18SkillConvertUndeadi` | `0x00299700` / 52 | 24 | `experimental/sources/w13-skill-stat-d-convert-undead-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-convert-undead-energy-cost/host_check.cpp` | `d95a129432d3bea778d88020bd87622f71525583d28676aa1516b044b54eec00` |
| `getEnergyCost__22SkillRangerCriticalHiti` | `0x002c7d20` / 52 | 24 | `experimental/sources/w13-skill-stat-d-ranger-critical-hit-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-ranger-critical-hit-energy-cost/host_check.cpp` | `da64aab9d449a77c96053f55ca11cf075cae6b0a795d1f6414df1f255b94fa8b` |
| `getLevelRequirement__22SkillRangerCriticalHiti` | `0x002c7d58` / 8 | 8 | `experimental/sources/w13-skill-stat-d-ranger-critical-hit-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-ranger-critical-hit-level-requirement/host_check.cpp` | `0df6d95b813e565c4f9df9edf8e7800efb621c1971c31ccd9c9f6e2dd099c263` |
| `getEnergyCost__9SkillBashi` | `0x002a5f08` / 52 | 24 | `experimental/sources/w13-skill-stat-d-bash-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-bash-energy-cost/host_check.cpp` | `1e6728dc76d8b9c0908295c71788d7e58868b2a7299eb9d16c8b3e363b093db5` |
| `getLevelRequirement__9SkillBashi` | `0x002a5f40` / 8 | 8 | `experimental/sources/w13-skill-stat-d-bash-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-bash-level-requirement/host_check.cpp` | `70f448ebb25cb8c019add4b36df2be312606a34e50d84d8991b663da07356884` |
| `getEnergyCost__9SkillRooti` | `0x002ad018` / 52 | 24 | `experimental/sources/w13-skill-stat-d-root-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w13-skill-stat-d-root-energy-cost/host_check.cpp` | `9b9ad2bb128f08d29bbd7806b21b46159a610ac36bc2495cd948849a2c8282b2` |
| `getLevelRequirement__9SkillRooti` | `0x002ad050` / 8 | 8 | `experimental/sources/w13-skill-stat-d-root-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w13-skill-stat-d-root-level-requirement/host_check.cpp` | `db0af088a47a3ce25fba606e193b8fad0da454d6c72213bb5639b9e86dc251e2` |
| `getIcon__18SkillConvertUndead` | `0x00317630` / 36 | 20 | `experimental/sources/w13-skill-stat-d-convert-undead-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-convert-undead-icon/host_check.cpp` | `9366e779978e9216376bed878c415ac544d42880a1f91d24463c48890f682b0a` |
| `getIcon__18SkillDiseaseShield` | `0x003170a0` / 36 | 20 | `experimental/sources/w13-skill-stat-d-disease-shield-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-disease-shield-icon/host_check.cpp` | `dc2df201151cc0235d55a91a996e83756e8b5696cfb74403de35ba310210ed17` |
| `getIcon__18SkillDismissUndead` | `0x00319de8` / 36 | 20 | `experimental/sources/w13-skill-stat-d-dismiss-undead-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-dismiss-undead-icon/host_check.cpp` | `eb478b004b9a8f97ac265d43300a34b125227435f9061da20ff795d7ba953466` |
| `getIcon__18SkillHammerOfWrath` | `0x003198a0` / 36 | 20 | `experimental/sources/w13-skill-stat-d-hammer-of-wrath-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-hammer-of-wrath-icon/host_check.cpp` | `10a3e75fae3099295e96f90be63bac7e3e2d428ac5ab6bc5cd627f43bd29e3b2` |
| `getIcon__18SkillRepulseUndead` | `0x00319d70` / 36 | 20 | `experimental/sources/w13-skill-stat-d-repulse-undead-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-repulse-undead-icon/host_check.cpp` | `b63d2c72a89795d61d580b93f95902021dac27b1e9733c2db089c6a7bfc736a7` |
| `getIcon__19SkillSummonSkeleton` | `0x00318a10` / 36 | 20 | `experimental/sources/w13-skill-stat-d-summon-skeleton-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-summon-skeleton-icon/host_check.cpp` | `cd18845e24ffd6988d89fd179a9c9ae23c99632a9e5f19a855d84f84ad95ff6a` |
| `getIcon__21SkillLightningWeapons` | `0x00318b60` / 36 | 20 | `experimental/sources/w13-skill-stat-d-lightning-weapons-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-lightning-weapons-icon/host_check.cpp` | `dbe14c264088101bc8719c277c6940dc30b9990408e564b57bfbc2e9521f470c` |
| `getIcon__22SkillRangerCriticalHit` | `0x0031a628` / 36 | 20 | `experimental/sources/w13-skill-stat-d-ranger-critical-hit-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-ranger-critical-hit-icon/host_check.cpp` | `fba48ab929e37ca8de9e8cbe9ac49842576b5fa6a78d6b2319cc83ef1d9e38e3` |
| `getIcon__5Skill` | `0x00313268` / 36 | 20 | `experimental/sources/w13-skill-stat-d-base-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-base-icon/host_check.cpp` | `d1751ff2a65a0860b85c0c2daabe1e2c30a96f46794ad3a25fc1e7c228bcbf21` |
| `getIcon__9SkillBash` | `0x00318450` / 36 | 20 | `experimental/sources/w13-skill-stat-d-bash-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-bash-icon/host_check.cpp` | `383a524bd1f85e8c77038beb25d383fcbc7f3ec119a4d12821e9dcf560a20678` |
| `getIcon__9SkillRoot` | `0x00318e10` / 36 | 20 | `experimental/sources/w13-skill-stat-d-root-icon/get_icon.cpp`; host test `experimental/sources/w13-skill-stat-d-root-icon/host_check.cpp` | `3d29f4dc0d26a09ef3f7f4aca90088864cd5dd4f126698c3e68c950c09af5687` |

Host contracts passed for all 37. Constant accessors are checked across signed boundary ranks; the ramp energy costs are compared bit-for-bit against an independent float expression, including INT_MIN/INT_MAX wraparound; virtual-call accessors run against a fake table for several receiver adjustments and check receiver, rank and constant; icon accessors check both strings and that the result of the lump lookup is passed through. No paired run against the original image was done.

## Observed facts about the data

- Point costs are the constant 1 (SkillBlindingLight, DiseaseShield, DismissUndead, HammerOfWrath, RepulseUndead, SummonSkeleton, LightningWeapons). Level requirements: DiseaseShield 10, DismissUndead 25, HammerOfWrath 10, RepulseUndead 15, SummonSkeleton 15, LightningWeapons 12, RangerCriticalHit 10, Bash 10, Root 25.
- Six classes compute energy cost as `(rank-1)*step + base` in single float: DiseaseShield step 0x40800023 (about 4.0000167) base 43; DismissUndead step 0x40a0000e (about 5.0000067) base 107; HammerOfWrath 4 / 48; RepulseUndead 4 / 59; SummonSkeleton 5 / 81; LightningWeapons 9 / 102. The two non-round steps are read from the original's gp-relative float pool; the exact bit patterns are written as hexadecimal float literals. Why they are not exactly 4 and 5 is unknown.
- ConvertUndead (10), RangerCriticalHit (20), Bash (19), Root (20) forward their rank and that constant to the virtual method in pair 16 of the table pointed to by the word at receiver offset 96, adjusting the receiver by that pair's halfword. Pair 16 is the entry the w12-skill-b table authoring lists as `energyCostRamp4__5Skillii`. That slot-to-name match is inferred from the table source, not re-read here.
- Icons: every accessor calls the lump lookup with `icons.lmp` and a per-class texture name (names in the sources); the base accessor uses `icblank.tex`. The lookup's return type is unknown (declared as a pointer).

## Inferred

- Class meanings are by symbol name only. Whether the virtual call's constant is a ramp index, a cost id or something else is unknown. The base `Skill` receiver having a vtable word at offset 96 is observed only through these four functions.

## Imports closed

`getSkillPointCost__18SkillBlindingLighti`
`getSkillPointCost__18SkillDiseaseShieldi`
`getLevelRequirement__18SkillDiseaseShieldi`
`getSkillPointCost__18SkillDismissUndeadi`
`getLevelRequirement__18SkillDismissUndeadi`
`getSkillPointCost__18SkillHammerOfWrathi`
`getLevelRequirement__18SkillHammerOfWrathi`
`getSkillPointCost__18SkillRepulseUndeadi`
`getLevelRequirement__18SkillRepulseUndeadi`
`getSkillPointCost__19SkillSummonSkeletoni`
`getLevelRequirement__19SkillSummonSkeletoni`
`getSkillPointCost__21SkillLightningWeaponsi`
`getLevelRequirement__21SkillLightningWeaponsi`
`getEnergyCost__18SkillDiseaseShieldi`
`getEnergyCost__18SkillDismissUndeadi`
`getEnergyCost__18SkillHammerOfWrathi`
`getEnergyCost__18SkillRepulseUndeadi`
`getEnergyCost__19SkillSummonSkeletoni`
`getEnergyCost__21SkillLightningWeaponsi`
`getEnergyCost__18SkillConvertUndeadi`
`getEnergyCost__22SkillRangerCriticalHiti`
`getLevelRequirement__22SkillRangerCriticalHiti`
`getEnergyCost__9SkillBashi`
`getLevelRequirement__9SkillBashi`
`getEnergyCost__9SkillRooti`
`getLevelRequirement__9SkillRooti`
`getIcon__18SkillConvertUndead`
`getIcon__18SkillDiseaseShield`
`getIcon__18SkillDismissUndead`
`getIcon__18SkillHammerOfWrath`
`getIcon__18SkillRepulseUndead`
`getIcon__19SkillSummonSkeleton`
`getIcon__21SkillLightningWeapons`
`getIcon__22SkillRangerCriticalHit`
`getIcon__5Skill`
`getIcon__9SkillBash`
`getIcon__9SkillRoot`

All 37 have authored definitions in this branch; none is selected by the shared plan yet, so no post-integration import-count reduction is claimed. (The package listed 38 names; the list as given contains 37 distinct symbols.)

## Imports opened

`lumpFindResource__FPcT0` (owned elsewhere) is referenced by the 11 icon accessors. The four virtual-call energy costs reference no symbol (dispatch is through the receiver).

## Build-plan entries needed

Each source uses `authored_cpp`, has empty `header_closure`; the integrator assigns `input_order` and adds the hash to the plan `files` map.

```json
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__18SkillBlindingLighti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "46d59e4bd69ef1ee7a7d2441bc35c3cdb3b873ac0f4e6a349f2e9e5c63106409", "source": "sources/w13-skill-stat-d-blinding-light-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__18SkillDiseaseShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "a26da00a8f1348b2697620eed3d2e92de6953ac61f6825fbc22321147420e09a", "source": "sources/w13-skill-stat-d-disease-shield-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__18SkillDiseaseShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "af7d620f1f31deced68264ca699cb54be699a035bd9df3383a5f92ca2caf585e", "source": "sources/w13-skill-stat-d-disease-shield-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__18SkillDismissUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "5edf8772c86773c9b1597fdc351255b08d064bbee68234e55afdff221d20b936", "source": "sources/w13-skill-stat-d-dismiss-undead-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__18SkillDismissUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "97760c2930c4ef48e1d32318435f40cca209b9d9a1bfaf4efe761c463f26aa59", "source": "sources/w13-skill-stat-d-dismiss-undead-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__18SkillHammerOfWrathi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "797d1f851fb67d8115209cb2680944dfad20c83f3697d6c610da3a63da9839c7", "source": "sources/w13-skill-stat-d-hammer-of-wrath-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__18SkillHammerOfWrathi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "6f40933366f260534e744360bda088d252826564a00b578cdddbc31db5ee8d94", "source": "sources/w13-skill-stat-d-hammer-of-wrath-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__18SkillRepulseUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "b45b38414edb2f154d82af5867aa66591cf3d44d0d935a7371ea2fe36d7e099c", "source": "sources/w13-skill-stat-d-repulse-undead-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__18SkillRepulseUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "4a2c98bc9c46531420454de456948059d720c648eb4d85ff6af0c2dd081c8b35", "source": "sources/w13-skill-stat-d-repulse-undead-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__19SkillSummonSkeletoni"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "7a1b9c39f588119e7b3db6e08030cc709ac8ce5f7e854e449db8c2307bbc83e8", "source": "sources/w13-skill-stat-d-summon-skeleton-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__19SkillSummonSkeletoni"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "9e4aadc34f6b311ad611b993f76c494b170afc2b1f8fa2c50b77f02e460da429", "source": "sources/w13-skill-stat-d-summon-skeleton-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__21SkillLightningWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "f080cee04800bf21ca746adacbdee962dd4dc19c5de19d3a1737bfb3906c23b3", "source": "sources/w13-skill-stat-d-lightning-weapons-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__21SkillLightningWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "4dc2ade11fcf64e81e09182cc733aeb7296ea4531c2a901279594d4533cc7cd4", "source": "sources/w13-skill-stat-d-lightning-weapons-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__18SkillDiseaseShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "8f4924f4651e8f2c151c4c0f1a307529bf9967f407ac7755f506718de923bf91", "source": "sources/w13-skill-stat-d-disease-shield-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__18SkillDismissUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "4f9e12c87bd6d494ad72cc79b6625fd4157593b5c5f1d75ea4116dfee76d2037", "source": "sources/w13-skill-stat-d-dismiss-undead-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__18SkillHammerOfWrathi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "69eebd03eb2a24d038bc34c90691472178fe27f2504ba2d81f2efd58079b2536", "source": "sources/w13-skill-stat-d-hammer-of-wrath-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__18SkillRepulseUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "c22eb207ed77b161778c5e488589efdd497debb82d725e01690c460dee27d82d", "source": "sources/w13-skill-stat-d-repulse-undead-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__19SkillSummonSkeletoni"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "c0bd1bd71fdeb656e8544580581ab813c7456771ef31b7b97b900a4a4cf54454", "source": "sources/w13-skill-stat-d-summon-skeleton-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__21SkillLightningWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "a4dd0a637f67b65cba03e24292ece73f2055c65c0cbb0ffe9d7b92bba3976d9e", "source": "sources/w13-skill-stat-d-lightning-weapons-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__18SkillConvertUndeadi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "d95a129432d3bea778d88020bd87622f71525583d28676aa1516b044b54eec00", "source": "sources/w13-skill-stat-d-convert-undead-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__22SkillRangerCriticalHiti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "da64aab9d449a77c96053f55ca11cf075cae6b0a795d1f6414df1f255b94fa8b", "source": "sources/w13-skill-stat-d-ranger-critical-hit-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__22SkillRangerCriticalHiti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "0df6d95b813e565c4f9df9edf8e7800efb621c1971c31ccd9c9f6e2dd099c263", "source": "sources/w13-skill-stat-d-ranger-critical-hit-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__9SkillBashi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "1e6728dc76d8b9c0908295c71788d7e58868b2a7299eb9d16c8b3e363b093db5", "source": "sources/w13-skill-stat-d-bash-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__9SkillBashi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "70f448ebb25cb8c019add4b36df2be312606a34e50d84d8991b663da07356884", "source": "sources/w13-skill-stat-d-bash-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__9SkillRooti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "9b9ad2bb128f08d29bbd7806b21b46159a610ac36bc2495cd948849a2c8282b2", "source": "sources/w13-skill-stat-d-root-energy-cost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__9SkillRooti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "db0af088a47a3ce25fba606e193b8fad0da454d6c72213bb5639b9e86dc251e2", "source": "sources/w13-skill-stat-d-root-level-requirement/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__18SkillConvertUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "9366e779978e9216376bed878c415ac544d42880a1f91d24463c48890f682b0a", "source": "sources/w13-skill-stat-d-convert-undead-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__18SkillDiseaseShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "dc2df201151cc0235d55a91a996e83756e8b5696cfb74403de35ba310210ed17", "source": "sources/w13-skill-stat-d-disease-shield-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__18SkillDismissUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "eb478b004b9a8f97ac265d43300a34b125227435f9061da20ff795d7ba953466", "source": "sources/w13-skill-stat-d-dismiss-undead-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__18SkillHammerOfWrath"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "10a3e75fae3099295e96f90be63bac7e3e2d428ac5ab6bc5cd627f43bd29e3b2", "source": "sources/w13-skill-stat-d-hammer-of-wrath-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__18SkillRepulseUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "b63d2c72a89795d61d580b93f95902021dac27b1e9733c2db089c6a7bfc736a7", "source": "sources/w13-skill-stat-d-repulse-undead-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__19SkillSummonSkeleton"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "cd18845e24ffd6988d89fd179a9c9ae23c99632a9e5f19a855d84f84ad95ff6a", "source": "sources/w13-skill-stat-d-summon-skeleton-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__21SkillLightningWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "dbe14c264088101bc8719c277c6940dc30b9990408e564b57bfbc2e9521f470c", "source": "sources/w13-skill-stat-d-lightning-weapons-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__22SkillRangerCriticalHit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "fba48ab929e37ca8de9e8cbe9ac49842576b5fa6a78d6b2319cc83ef1d9e38e3", "source": "sources/w13-skill-stat-d-ranger-critical-hit-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__5Skill"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "d1751ff2a65a0860b85c0c2daabe1e2c30a96f46794ad3a25fc1e7c228bcbf21", "source": "sources/w13-skill-stat-d-base-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__9SkillBash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "383a524bd1f85e8c77038beb25d383fcbc7f3ec119a4d12821e9dcf560a20678", "source": "sources/w13-skill-stat-d-bash-icon/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "getIcon__9SkillRoot"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "3d29f4dc0d26a09ef3f7f4aca90088864cd5dd4f126698c3e68c950c09af5687", "source": "sources/w13-skill-stat-d-root-icon/get_icon.cpp"},
```

## Authored but never executed

All 37 compiled and passed host contracts. None ran on an EE, through game dispatch or in a game session, and none was compared against the original in an interpreter.

## Unknown

Which skill-tree and spell-UI callers use each accessor, the meaning of the constants noted above, the type returned by the lump lookup, the full Skill layout, and why two energy steps are not round numbers.
