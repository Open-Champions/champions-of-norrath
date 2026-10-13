# Skill energy cost, level requirement, point cost and icon accessors, part 3: w13-skill-stat-c

Branch `agents/w13-skill-stat-c`. Forty functions, one per commit, each with a host contract and a paired run against the
private image function. Reference: the hash-pinned executable `SLUS-20565-sha256-9a60f13c5c31`. No game binary, raw
disassembly, decompiler output or extracted asset is committed; the only image-derived content is symbol names, addresses,
sizes, small integer/float constants and the two resource-name strings each icon accessor passes.

## Observed

- Level requirement and point cost accessors are leaf functions returning a constant for every rank (receiver and rank not
  read). Level requirement: DiseaseBolt 3, FireWeapons 20, ShockOfFire 15, DiseaseTrail 8, MinorHealing 20, Regeneration 5,
  ShockOfFrost 1, UndeadShield 10, BlindingLight 15. Point cost is 1 for DiseaseBolt, FireWeapons, DiseaseTrail,
  Regeneration, ShockOfFrost and UndeadShield.
- Twelve icon accessors tail-call `lumpFindResource__FPcT0("icons.lmp", "<name>.tex")` and return its result; the receiver
  is not read. The names are in each source file.
- Seven energy cost accessors (GroundPound 4, PoisonArrow 20, ShockOfFire 20, ConvertEnemy 4, ExplodeArrow 9, MinorHealing 20,
  AncestralCall 20) read the table word at object+96, take the entry at byte 128 (halfword adjusting the receiver, function
  word at +4) and call it with (receiver + adjust, rank unchanged, constant), returning its float result untouched. In the
  `_vt$` tables of `docs/agents/w12-skill-b.md` / `w12-skill-a.md` byte 128 is slot 16, `energyCostRamp4__5Skillii`.
- Six energy cost accessors compute `(float)(rank - 1) * step + base` without touching the receiver:

  | Class | step (bits) | base |
  |---|---|---:|
  | SkillDiseaseBolt | `0x40000036` (small-data word) | 9.0 |
  | SkillFireWeapons | `0x41200000` (10.0, an immediate) | 195.0 |
  | SkillDiseaseTrail | `0x403ffff3` (small-data word) | 28.0 |
  | SkillShockOfFrost | `0x4040008a` (small-data word) | 7.0 |
  | SkillUndeadShield | `0x40800034` (small-data word) | 41.0 |
  | SkillBlindingLight | `0x403ffff7` (small-data word) | 45.0 |

  The small-data factors are 4-byte words read through gp; their owners are unknown. The source builds them with
  `__builtin_bit_cast`, so the compiled code loads them as immediates, not from the small-data area.

## Checks performed

- Each unit compiled alone with the reviewed `authored_cpp` EE flags (all 40 succeeded). Compiled `.text` sizes are in the
  table; they differ from the image sizes (icon accessors build without a stack frame; small-data loads become immediates).
  These are semantic reconstructions, not byte matches.
- Host contract per unit (`c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp <unit>.cpp`), all passing. The
  table-call units check the receiver adjustment, the unchanged rank, the constant and the untouched float result against a
  fake table; the others check boundary ranks (INT_MIN..INT_MAX) against the formula; icon units check both strings.
- Paired run of each authored unit against the image function in a private R5900 interpreter, 12 rank scenarios each
  (0, 1, 2, 3, 7, 19, 100, extremes), with identical stub callees, comparing call logs, return values / `$f0`, and changed
  memory: 0 mismatches over 40 functions, no HALT. Mutating one constant in a unit of each kind (energy-cost base, call
  constant, icon string, level requirement) made the paired run report mismatches.
- Nothing ran on the EE hardware.

| Function | Original address / bytes | Compiled bytes | Source |
|---|---:|---:|---|
| `getLevelRequirement__16SkillDiseaseBolti` | `0x00293870` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-disease-bolt/get_level_requirement.cpp` |
| `getLevelRequirement__16SkillFireWeaponsi` | `0x002ab4c0` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-fire-weapons/get_level_requirement.cpp` |
| `getLevelRequirement__16SkillShockOfFirei` | `0x002b3290` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-shock-of-fire/get_level_requirement.cpp` |
| `getLevelRequirement__17SkillDiseaseTraili` | `0x00295538` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-disease-trail/get_level_requirement.cpp` |
| `getLevelRequirement__17SkillMinorHealingi` | `0x002a7480` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-minor-healing/get_level_requirement.cpp` |
| `getLevelRequirement__17SkillRegenerationi` | `0x002b6c60` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-regeneration/get_level_requirement.cpp` |
| `getLevelRequirement__17SkillShockOfFrosti` | `0x002be160` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-shock-of-frost/get_level_requirement.cpp` |
| `getLevelRequirement__17SkillUndeadShieldi` | `0x002b13d8` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-undead-shield/get_level_requirement.cpp` |
| `getLevelRequirement__18SkillBlindingLighti` | `0x002ba180` / 8 | 8 | `experimental/sources/w13-skill-stat-c-level-requirement-skill-blinding-light/get_level_requirement.cpp` |
| `getSkillPointCost__16SkillDiseaseBolti` | `0x00293868` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-disease-bolt/get_skill_point_cost.cpp` |
| `getSkillPointCost__16SkillFireWeaponsi` | `0x002ab4b8` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-fire-weapons/get_skill_point_cost.cpp` |
| `getSkillPointCost__17SkillDiseaseTraili` | `0x00295530` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-disease-trail/get_skill_point_cost.cpp` |
| `getSkillPointCost__17SkillRegenerationi` | `0x002b6c58` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-regeneration/get_skill_point_cost.cpp` |
| `getSkillPointCost__17SkillShockOfFrosti` | `0x002be158` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-shock-of-frost/get_skill_point_cost.cpp` |
| `getSkillPointCost__17SkillUndeadShieldi` | `0x002b13d0` / 8 | 8 | `experimental/sources/w13-skill-stat-c-point-cost-skill-undead-shield/get_skill_point_cost.cpp` |
| `getIcon__16SkillFireWeapons` | `0x00318c48` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-fire-weapons/get_icon.cpp` |
| `getIcon__16SkillGroundPound` | `0x00316af0` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-ground-pound/get_icon.cpp` |
| `getIcon__16SkillPoisonArrow` | `0x00317e50` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-poison-arrow/get_icon.cpp` |
| `getIcon__16SkillShockOfFire` | `0x003196c8` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-shock-of-fire/get_icon.cpp` |
| `getIcon__17SkillConvertEnemy` | `0x003174d0` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-convert-enemy/get_icon.cpp` |
| `getIcon__17SkillDiseaseTrail` | `0x00316e90` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-disease-trail/get_icon.cpp` |
| `getIcon__17SkillExplodeArrow` | `0x003180d8` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-explode-arrow/get_icon.cpp` |
| `getIcon__17SkillMinorHealing` | `0x00318688` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-minor-healing/get_icon.cpp` |
| `getIcon__17SkillShockOfFrost` | `0x00319e60` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-shock-of-frost/get_icon.cpp` |
| `getIcon__17SkillUndeadShield` | `0x003194f8` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-undead-shield/get_icon.cpp` |
| `getIcon__18SkillAncestralCall` | `0x003187d8` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-ancestral-call/get_icon.cpp` |
| `getIcon__18SkillBlindingLight` | `0x00319c40` / 36 | 24 | `experimental/sources/w13-skill-stat-c-icon-skill-blinding-light/get_icon.cpp` |
| `getEnergyCost__16SkillGroundPoundi` | `0x00290ac8` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-ground-pound/get_energy_cost.cpp` |
| `getEnergyCost__16SkillPoisonArrowi` | `0x002a0d00` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-poison-arrow/get_energy_cost.cpp` |
| `getEnergyCost__16SkillShockOfFirei` | `0x002b3258` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-shock-of-fire/get_energy_cost.cpp` |
| `getEnergyCost__17SkillConvertEnemyi` | `0x00298ca8` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-convert-enemy/get_energy_cost.cpp` |
| `getEnergyCost__17SkillExplodeArrowi` | `0x002a28f8` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-explode-arrow/get_energy_cost.cpp` |
| `getEnergyCost__17SkillMinorHealingi` | `0x002a7448` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-minor-healing/get_energy_cost.cpp` |
| `getEnergyCost__18SkillAncestralCalli` | `0x002a8530` / 52 | 24 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-ancestral-call/get_energy_cost.cpp` |
| `getEnergyCost__16SkillDiseaseBolti` | `0x00293878` / 36 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-disease-bolt/get_energy_cost.cpp` |
| `getEnergyCost__16SkillFireWeaponsi` | `0x002ab4c8` / 40 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-fire-weapons/get_energy_cost.cpp` |
| `getEnergyCost__17SkillDiseaseTraili` | `0x00295540` / 36 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-disease-trail/get_energy_cost.cpp` |
| `getEnergyCost__17SkillShockOfFrosti` | `0x002be168` / 36 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-shock-of-frost/get_energy_cost.cpp` |
| `getEnergyCost__17SkillUndeadShieldi` | `0x002b13e0` / 36 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-undead-shield/get_energy_cost.cpp` |
| `getEnergyCost__18SkillBlindingLighti` | `0x002ba188` / 36 | 40 | `experimental/sources/w13-skill-stat-c-energy-cost-skill-blinding-light/get_energy_cost.cpp` |

## Unknown

- What the constants passed to slot 16 (4, 9, 20) mean, and the owners of the small-data step factors.
- Member names and class layouts; only the table word at +96 is used here.
- What the record returned by `lumpFindResource__FPcT0` for a `.tex` name is.

## Imports closed

`getLevelRequirement__16SkillDiseaseBolti`
`getLevelRequirement__16SkillFireWeaponsi`
`getLevelRequirement__16SkillShockOfFirei`
`getLevelRequirement__17SkillDiseaseTraili`
`getLevelRequirement__17SkillMinorHealingi`
`getLevelRequirement__17SkillRegenerationi`
`getLevelRequirement__17SkillShockOfFrosti`
`getLevelRequirement__17SkillUndeadShieldi`
`getLevelRequirement__18SkillBlindingLighti`
`getSkillPointCost__16SkillDiseaseBolti`
`getSkillPointCost__16SkillFireWeaponsi`
`getSkillPointCost__17SkillDiseaseTraili`
`getSkillPointCost__17SkillRegenerationi`
`getSkillPointCost__17SkillShockOfFrosti`
`getSkillPointCost__17SkillUndeadShieldi`
`getIcon__16SkillFireWeapons`
`getIcon__16SkillGroundPound`
`getIcon__16SkillPoisonArrow`
`getIcon__16SkillShockOfFire`
`getIcon__17SkillConvertEnemy`
`getIcon__17SkillDiseaseTrail`
`getIcon__17SkillExplodeArrow`
`getIcon__17SkillMinorHealing`
`getIcon__17SkillShockOfFrost`
`getIcon__17SkillUndeadShield`
`getIcon__18SkillAncestralCall`
`getIcon__18SkillBlindingLight`
`getEnergyCost__16SkillGroundPoundi`
`getEnergyCost__16SkillPoisonArrowi`
`getEnergyCost__16SkillShockOfFirei`
`getEnergyCost__17SkillConvertEnemyi`
`getEnergyCost__17SkillExplodeArrowi`
`getEnergyCost__17SkillMinorHealingi`
`getEnergyCost__18SkillAncestralCalli`
`getEnergyCost__16SkillDiseaseBolti`
`getEnergyCost__16SkillFireWeaponsi`
`getEnergyCost__17SkillDiseaseTraili`
`getEnergyCost__17SkillShockOfFrosti`
`getEnergyCost__17SkillUndeadShieldi`
`getEnergyCost__18SkillBlindingLighti`

These 40 names have authored definitions on this branch. They are not selected by the shared plan, so no post-integration
import-count reduction is claimed.

## Imports opened

None beyond what already existed: the 12 icon units reference `lumpFindResource__FPcT0` (not defined here; it was already an
unresolved or other-package symbol). The seven table-call energy cost units call through the object's table and reference no
symbol.

## Build-plan entries needed

Each source uses `authored_cpp`, has empty `header_closure`, and needs its hash in the plan `files` map. The integrator assigns
`input_order`.

```json
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__16SkillDiseaseBolti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "ee8dfa6c3a415b2c865e3fbee66ae495aa0c14bbe3e25b345f055be72fcd5fea", "source": "sources/w13-skill-stat-c-level-requirement-skill-disease-bolt/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__16SkillFireWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "772775259685ca1a95adf65f6634ea8cddbbde4bce9a3f26ee6733f1601becfb", "source": "sources/w13-skill-stat-c-level-requirement-skill-fire-weapons/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__16SkillShockOfFirei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "67d5784b47a2e0659048bf0171442ec8b629afb870a9fcf49e6dee1c6dd21658", "source": "sources/w13-skill-stat-c-level-requirement-skill-shock-of-fire/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__17SkillDiseaseTraili"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "a463572cd52905c6055d2aba5945bc6e1c0a899fa22dcb6cd560b0c62e734254", "source": "sources/w13-skill-stat-c-level-requirement-skill-disease-trail/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__17SkillMinorHealingi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "f6a4aad9942d7ab75850fc29470db5c9d3a43e33035c1f1ba5d5a29772e97cb2", "source": "sources/w13-skill-stat-c-level-requirement-skill-minor-healing/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__17SkillRegenerationi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "86e9681627062bcc564af5163d391ede1c1398f12374e8a3ce7e118c90cf04e4", "source": "sources/w13-skill-stat-c-level-requirement-skill-regeneration/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__17SkillShockOfFrosti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "a20e82728a569df3ae6dc01b4c4bd30981b0ede5d69f446c4837d16d0a035aff", "source": "sources/w13-skill-stat-c-level-requirement-skill-shock-of-frost/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__17SkillUndeadShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "5563aa48e84f04fd0bb6f802a1061b23f071e1c67c9f96372e007e28cfa16e46", "source": "sources/w13-skill-stat-c-level-requirement-skill-undead-shield/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getLevelRequirement__18SkillBlindingLighti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_level_requirement.o", "profile": "authored_cpp", "sha256": "60b40e457055040ffc4bb6282af50c45a301f4bd24355ae105a883447fea70fb", "source": "sources/w13-skill-stat-c-level-requirement-skill-blinding-light/get_level_requirement.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__16SkillDiseaseBolti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "a9a7b9a273ba6c73114ffab81c3c91d45562fbf0b282787e4efbc2ba4eab5f20", "source": "sources/w13-skill-stat-c-point-cost-skill-disease-bolt/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__16SkillFireWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "45f08b441400207c2d007d992b07767cc939cd521eddb5d444cfeb9b9060c36d", "source": "sources/w13-skill-stat-c-point-cost-skill-fire-weapons/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__17SkillDiseaseTraili"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "1f7c5dfc2f4e5482379965e08cd8fc2cb370d04402cb434bf3ed5a16d6847834", "source": "sources/w13-skill-stat-c-point-cost-skill-disease-trail/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__17SkillRegenerationi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "df29e453fa93b2360c9107e02d8a1ef9c12fd555f54cf61d6c33fa90cda0f053", "source": "sources/w13-skill-stat-c-point-cost-skill-regeneration/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__17SkillShockOfFrosti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "cb292728f0c6a31bac563606bceecefc87d2293e9c3805fbe27faf2e2107bb28", "source": "sources/w13-skill-stat-c-point-cost-skill-shock-of-frost/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getSkillPointCost__17SkillUndeadShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_skill_point_cost.o", "profile": "authored_cpp", "sha256": "ae1bd36cecfa0bb09ed127c205679d63908804d2739f5e15cf5ce976de494710", "source": "sources/w13-skill-stat-c-point-cost-skill-undead-shield/get_skill_point_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__16SkillFireWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "a749693eb266c81625029b8f694d8b3d27bf2d9f77fdcc9a2fa2c1b318939189", "source": "sources/w13-skill-stat-c-icon-skill-fire-weapons/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__16SkillGroundPound"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "488c3364f21164344d5b9ac99ef480635b60de96ff67c6c514abc1718a49b8df", "source": "sources/w13-skill-stat-c-icon-skill-ground-pound/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__16SkillPoisonArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "5af2543c7c92800df575aa5bde0672ed0dca746bfbfb3c56d369238772c566f8", "source": "sources/w13-skill-stat-c-icon-skill-poison-arrow/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__16SkillShockOfFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "ffc1b450f1f3059c0c3ccc27a6a486fe7447869923cf35789ef75ccbb8f28f4f", "source": "sources/w13-skill-stat-c-icon-skill-shock-of-fire/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillConvertEnemy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "40538e7ca22ed4fb95db9b190078c156250e047bedaad1b4d926d59a107bed3f", "source": "sources/w13-skill-stat-c-icon-skill-convert-enemy/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillDiseaseTrail"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "b097798597ef94b93dbe9ef2ed9b0cc7f1ddfe6cc3ac8eba29f3ed5082b09444", "source": "sources/w13-skill-stat-c-icon-skill-disease-trail/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillExplodeArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "3e9aa9e832633ca3a9c2468e6546a058a7cee9c11271caa99d230af949334022", "source": "sources/w13-skill-stat-c-icon-skill-explode-arrow/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillMinorHealing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "b99e98a0806be7843c6108ffe8ba1e4461a80e4825c2ea138afdd84ebcfe6880", "source": "sources/w13-skill-stat-c-icon-skill-minor-healing/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillShockOfFrost"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "d6253483d46073fdae71524f3076b2ad4c27c92a923c010f469b68382ec4ebc7", "source": "sources/w13-skill-stat-c-icon-skill-shock-of-frost/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__17SkillUndeadShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "cd399dd4f87634fa5d990c0cc53acf7ca7b1af30efa42e661d8f8277b5b65202", "source": "sources/w13-skill-stat-c-icon-skill-undead-shield/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__18SkillAncestralCall"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "b0c40bdf5b37dd78610ced5d6c843b35ee6a64d43ae924e45547d254ce131215", "source": "sources/w13-skill-stat-c-icon-skill-ancestral-call/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getIcon__18SkillBlindingLight"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_icon.o", "profile": "authored_cpp", "sha256": "6d0f9b59e3d9ec7102faa5e321f81229e1546c92a4e3f594e7d91abbd63be52b", "source": "sources/w13-skill-stat-c-icon-skill-blinding-light/get_icon.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__16SkillGroundPoundi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "df0dce2abc11e04b65d3412887813fd60d78ec046c38e3b74217fc083dcf8178", "source": "sources/w13-skill-stat-c-energy-cost-skill-ground-pound/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__16SkillPoisonArrowi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "93a9206726737334ce895d67d94d70b1e42fb43fc4d585dddd511ba056e0be54", "source": "sources/w13-skill-stat-c-energy-cost-skill-poison-arrow/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__16SkillShockOfFirei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "ddca4df1455ddefa6a36a5b486d3aee7e5c55a6fe19c1d500b79d64f0f0df97a", "source": "sources/w13-skill-stat-c-energy-cost-skill-shock-of-fire/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__17SkillConvertEnemyi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "6ec6c821a9bf92d065f740d67f730ec231e58cf45fe91252a5b86f5260d5a9cc", "source": "sources/w13-skill-stat-c-energy-cost-skill-convert-enemy/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__17SkillExplodeArrowi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "ab73290bd63cbb932176185b72658e152431b5c0bc8c64b17562a578045bc9a6", "source": "sources/w13-skill-stat-c-energy-cost-skill-explode-arrow/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__17SkillMinorHealingi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "76537bf495f12c6a31f7647f7b3529fa647d81056e090b9f049d0527737f9a8e", "source": "sources/w13-skill-stat-c-energy-cost-skill-minor-healing/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "getEnergyCost__18SkillAncestralCalli"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "ab5e38adbdbe695e2524927f0c92edb8407bc938d07cc6e0e00b7e1c1a62eef0", "source": "sources/w13-skill-stat-c-energy-cost-skill-ancestral-call/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__16SkillDiseaseBolti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "381112979fa4055dcb191c2e3fad9e492869372a4f6059762f7a008ee03e48de", "source": "sources/w13-skill-stat-c-energy-cost-skill-disease-bolt/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__16SkillFireWeaponsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "3ba59e7f084cbc5cd9b855048f12324fac066b26ffc38445f37d202ea8ce629d", "source": "sources/w13-skill-stat-c-energy-cost-skill-fire-weapons/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__17SkillDiseaseTraili"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "4561d3a4f3b53a8de74088e0b9b7e9bf751a00da4e95c4323cf10d3f70ede636", "source": "sources/w13-skill-stat-c-energy-cost-skill-disease-trail/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__17SkillShockOfFrosti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "7071af0f07d2f31d0831bbc97ba5d21a526ac679e7306a4fd8d8dc25cf2e8df6", "source": "sources/w13-skill-stat-c-energy-cost-skill-shock-of-frost/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__17SkillUndeadShieldi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "62565a7b476149b2075031d75c651552fa8f15380095d23099f0d591b4509e14", "source": "sources/w13-skill-stat-c-energy-cost-skill-undead-shield/get_energy_cost.cpp"},
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "getEnergyCost__18SkillBlindingLighti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_energy_cost.o", "profile": "authored_cpp", "sha256": "f3a27244e72fd96c2cecbaf083c41608b84586ae11810a5ac7fe5575d12c0c2a", "source": "sources/w13-skill-stat-c-energy-cost-skill-blinding-light/get_energy_cost.cpp"}
```

## Authored but never executed

All 40 functions were executed only in the private R5900 interpreter and host contracts, never on the EE, and never inside
the partially linked startup chain.
