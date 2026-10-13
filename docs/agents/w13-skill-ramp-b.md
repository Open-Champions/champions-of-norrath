# Skill ramp initialisation and ramp evaluation, part 2 of 2 (w13-skill-ramp-b)

Branch `agents/w13-skill-ramp-b`. Twenty-nine functions, one per commit (plus six comment-only corrections of
recorded symbol sizes). Addresses and sizes come from the private image whose hash is the snapshot
`target_sha256`; nothing from the image is committed beyond symbol names, addresses, sizes and the numeric
constants the functions store. No disassembly, decompiler output or asset is in any commit.

## Functions

| Function | Address | Image size | Compiled bytes | Source (host check beside it) |
|---|---|---:|---:|---|
| `energyCostRamp4__5Skillii` | 0x00313788 | 96 | 88 | `experimental/sources/w13-skill-ramp-b-energy-cost-ramp4/energy_cost_ramp4.cpp` |
| `evaluateCreatureRampAtRank__5Skillii` | 0x003135e8 | 156 | 144 | `experimental/sources/w13-skill-ramp-b-evaluate-creature-ramp-at-rank/evaluate_creature_ramp_at_rank.cpp` |
| `evaluateRampAtRank__5Skillii` | 0x00313548 | 156 | 144 | `experimental/sources/w13-skill-ramp-b-evaluate-ramp-at-rank/evaluate_ramp_at_rank.cpp` |
| `evaluateRampAtRank__5SkilliiP8ListHead` | 0x00313700 | 124 | 80 | `experimental/sources/w13-skill-ramp-b-evaluate-ramp-at-rank-list/evaluate_ramp_at_rank_list.cpp` |
| `evaluateRampAtRank__5SkilliiP10GameObject` | 0x00313688 | 116 | 72 | `experimental/sources/w13-skill-ramp-b-evaluate-ramp-at-rank-object/evaluate_ramp_at_rank_object.cpp` |
| `getRampDamage__5SkilliifiPiT5` | 0x00313290 | 692 | 628 | `experimental/sources/w13-skill-ramp-b-get-ramp-damage/get_ramp_damage.cpp` |
| `initRamps__18SkillAncestralCall` | 0x002a8420 | 108 | 128 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-ancestral-call/init_ramps.cpp` |
| `initRamps__9SkillBash` | 0x002a5d58 | 108 | 124 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-bash/init_ramps.cpp` |
| `initRamps__18SkillBlindingLight` | 0x002ba110 | 100 | 104 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-blinding-light/init_ramps.cpp` |
| `initRamps__17SkillConvertEnemy` | 0x00298aa8 | 88 | 100 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-convert-enemy/init_ramps.cpp` |
| `initRamps__18SkillConvertUndead` | 0x00299500 | 88 | 100 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-convert-undead/init_ramps.cpp` |
| `initRamps__18SkillDiseaseShield` | 0x00296790 | 116 | 132 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-disease-shield/init_ramps.cpp` |
| `initRamps__17SkillDiseaseTrail` | 0x00295458 | 52 | 56 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-disease-trail/init_ramps.cpp` |
| `initRamps__18SkillDismissUndead` | 0x002bbc48 | 72 | 72 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-dismiss-undead/init_ramps.cpp` |
| `initRamps__17SkillExplodeArrow` | 0x002a2678 | 164 | 184 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-explode-arrow/init_ramps.cpp` |
| `initRamps__16SkillFireWeapons` | 0x002ab3c0 | 112 | 120 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-fire-weapons/init_ramps.cpp` |
| `initRamps__16SkillGroundPound` | 0x002908b8 | 112 | 128 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-ground-pound/init_ramps.cpp` |
| `initRamps__18SkillHammerOfWrath` | 0x002b57c0 | 72 | 72 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-hammer-of-wrath/init_ramps.cpp` |
| `initRamps__21SkillLightningWeapons` | 0x002a9d50 | 112 | 120 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-lightning-weapons/init_ramps.cpp` |
| `initRamps__17SkillMinorHealing` | 0x002a72f0 | 48 | 56 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-minor-healing/init_ramps.cpp` |
| `initRamps__16SkillPoisonArrow` | 0x002a0bb0 | 124 | 148 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-poison-arrow/init_ramps.cpp` |
| `initRamps__22SkillRangerCriticalHit` | 0x002c7c00 | 96 | 100 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-ranger-critical-hit/init_ramps.cpp` |
| `initRamps__17SkillRegeneration` | 0x002b6b88 | 28 | 32 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-regeneration/init_ramps.cpp` |
| `initRamps__18SkillRepulseUndead` | 0x002ba630 | 56 | 56 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-repulse-undead/init_ramps.cpp` |
| `initRamps__9SkillRoot` | 0x002aceb8 | 84 | 100 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-root/init_ramps.cpp` |
| `initRamps__16SkillShockOfFire` | 0x002b31a0 | 184 | 216 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-shock-of-fire/init_ramps.cpp` |
| `initRamps__17SkillShockOfFrost` | 0x002be038 | 140 | 152 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-shock-of-frost/init_ramps.cpp` |
| `initRamps__19SkillSummonSkeleton` | 0x002a9070 | 56 | 56 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-summon-skeleton/init_ramps.cpp` |
| `initRamps__17SkillUndeadShield` | 0x002b1200 | 76 | 80 | `experimental/sources/w13-skill-ramp-b-init-ramps-skill-undead-shield/init_ramps.cpp` |

Compiled sizes are not image sizes: the image reaches small-data globals through the global pointer, and the
reviewed profile (`-G0`) cannot; the compiler also emits tail calls where the image does not. No byte match is claimed.

## Observed

- A Skill object keeps, at fixed offsets, two sets of ramp data. Set one: five signed halfword thresholds at +14,
  five halfword bases at +24, five floats (steps) at +36. Set two ("creature"): thresholds at +56, bases at +66,
  floats at +76. `evaluateRampAtRank__5Skillii` and `evaluateCreatureRampAtRank__5Skillii` read set one and set two:
  level 0 gives 0; a level below the threshold gives 0; otherwise `base + step * (level - threshold)` is rounded
  half away from zero. The slot index is not range checked (offsets beyond 4 reach the next run).
- The object holds a table address at +96. Table entries are 8 bytes: a signed halfword adjustment added to the
  receiver, then a function word at +4. Table offset 88 is the slot filled with `evaluateRampAtRank__5Skillii` in the
  class tables, 96 with `evaluateCreatureRampAtRank__5Skillii`, and 120 with `tweekRank__5Skillii` (see
  `docs/agents/w12-skill-a.md` for the table layout).
- `energyCostRamp4__5Skillii` calls the +120 slot with its two arguments, then the +88 slot with index 4 and that
  result, and returns the integer as a float.
- `evaluateRampAtRank__5SkilliiP10GameObject` and `...P8ListHead` pick the +88 slot when bit 0x40 of the word at
  +12 of a game object is set (taken from the argument, or from the object in the word at +12 of the first node of the
  list), and the +96 slot when the object is null, the list is empty, or the bit is clear. The list variant does not
  test the node's object pointer for null.
- `getRampDamage__5SkilliifiPiT5` takes slot index, level, a float fraction, a flag and two int output pointers.
  It evaluates the +88 slot, writes `low` from `v - v*f` and `high` from `v + v*f`, each rounded half away from zero
  and clamped to at least 1. With a non-zero flag the generator word `syncRandState` steps (`s*0x19660d + 0x3c6ef35f`)
  and `(s >> 16) * 2^-16` scales the fraction; the rounding expression is evaluated twice, so each output takes two
  draws (four steps in all, low first). The first draw of each pair only decides the sign branch.
- The 23 `initRamps` functions each store constant halfwords and floats at the offsets above and nothing else.
  Several of the float constants are not short decimals (for example 0x3dcccccc, which is just under 0.1);
  they are stored as the observed 32-bit patterns.

## Inferred

- Set one is the player-facing ramp and set two the creature one, from the method names only. The bit 0x40 of the
  object word selects between them in the two object-taking evaluators; the meaning of the bit is not established.
- `getRampDamage` produces a damage range around the ramp value; `energyCostRamp4` is the fifth ramp (index 4)
  evaluated at the tweaked rank. These come from the method names and the arithmetic only.
- The odd float patterns in the `initRamps` functions (values just below a round number) are consistent with the
  original compiler converting decimal literals by truncation. This is not shown and affects nothing here.

## Checks performed

- Paired runs with the private R5900 interpreter (outside git; every undefined symbol bound to the image address,
  original and authored function run from identical snapshots, call logs, return value, float result and every
  changed byte compared): `evaluateRampAtRank__5Skillii` and `evaluateCreatureRampAtRank__5Skillii` 2400 runs each
  (40 random object states x 5 slots x 12 levels, including boundaries, +-32768 halfwords and huge floats), 0
  mismatches; `energyCostRamp4__5Skillii` 144 runs with hooked slots; the two object/list evaluators 56 and 55 runs
  with hooked slots over nine flag words and the null and empty cases; `getRampDamage__5SkilliifiPiT5` 672 runs
  (14 values x 12 fractions x flag 0/1/5/2^32, seeds including 0 and 0xffffffff), comparing outputs and the
  generator word; each `initRamps` function four fill patterns (0xa5, 0x5a, 0x00, 0xff) with 0 differences. Every
  run printed no HALT, and instruction coverage of the original reached every word except nops/annulled slots.
- Mutations that failed the paired run (then restored): changed rounding constant, changed rounding of the negative
  branch (evaluators), moved base offset, changed slot index or tested bit, swapped slot, changed generator constant,
  changed clamp threshold, swapped sign in the high expression; for every `initRamps` function an altered value and a
  dropped store. One mutation did NOT fail: changing the `- 0.5f` constant of the negative branch in `getRampDamage`
  cannot be seen, because a negative value is clamped to 1 afterwards either way.
- Host checks (`c++ -std=c++17 -Wall -Wextra -Werror`, native build): each function has a `host_check.cpp`. The five
  functions that call through a table use the `NORRATH_HOST_CONTRACT` hooks (the same pattern as
  `game-state-skill-ramps`) to supply a fake table, slot results and `syncRandState`; each fails under the mutations above.
  The `initRamps` host checks compare every byte of a guarded buffer with an independent byte list.
- Every unit compiles with the reviewed flags (`authored_cpp` profile) and each emits exactly one function.
- Private partial build: the shared plan plus these 29 entries, kept outside git
  (`docs/agents/w13-skill-ramp-b-plan-entries.py --private-root`), compiled 5,557 sources and linked once. None of
  the 29 names is in `unresolved-imports.json` (1,139 unresolved in that private copy, which is not comparable to the
  shared branch). The 29 `initRamps__*` names of the sibling package `w13-skill-ramp-a` remain unresolved there, as expected.

## Imports closed

`energyCostRamp4__5Skillii`, `evaluateCreatureRampAtRank__5Skillii`, `evaluateRampAtRank__5Skillii`, `evaluateRampAtRank__5SkilliiP8ListHead`, `evaluateRampAtRank__5SkilliiP10GameObject`, `getRampDamage__5SkilliifiPiT5`, `initRamps__18SkillAncestralCall`, `initRamps__9SkillBash`, `initRamps__18SkillBlindingLight`, `initRamps__17SkillConvertEnemy`, `initRamps__18SkillConvertUndead`, `initRamps__18SkillDiseaseShield`, `initRamps__17SkillDiseaseTrail`, `initRamps__18SkillDismissUndead`, `initRamps__17SkillExplodeArrow`, `initRamps__16SkillFireWeapons`, `initRamps__16SkillGroundPound`, `initRamps__18SkillHammerOfWrath`, `initRamps__21SkillLightningWeapons`, `initRamps__17SkillMinorHealing`, `initRamps__16SkillPoisonArrow`, `initRamps__22SkillRangerCriticalHit`, `initRamps__17SkillRegeneration`, `initRamps__18SkillRepulseUndead`, `initRamps__9SkillRoot`, `initRamps__16SkillShockOfFire`, `initRamps__17SkillShockOfFrost`, `initRamps__19SkillSummonSkeleton`, `initRamps__17SkillUndeadShield`.

## Imports opened

None new. `getRampDamage__5SkilliifiPiT5` reads and writes `syncRandState`, which another package defines (it did
not appear as unresolved in the private build). The five table-calling functions use only the table at object+96,
so no `_vt$` symbol is imported.

## Build-plan entries needed

29 entries; raise `default_authored_inputs` by 29 and add each source hash to the `files` maps. The exact JSON is
reproduced by `python3 docs/agents/w13-skill-ramp-b-plan-entries.py --toolchain <ee> [--private-root DIR]`; these are
the lines it printed (profile `authored_cpp`, empty `header_closure`, one emitted function each; `input_order` is
assigned by the integrator):

```
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "energyCostRamp4__5Skillii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_energy_cost_ramp4.o", "profile": "authored_cpp", "sha256": "65f32829d4ae46a4e71de491f79607264674e9f940cfefe6cfe8d632c84935e7", "source": "sources/w13-skill-ramp-b-energy-cost-ramp4/energy_cost_ramp4.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "evaluateCreatureRampAtRank__5Skillii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_evaluate_creature_ramp_at_rank.o", "profile": "authored_cpp", "sha256": "bfab00aba9986deafda2e38fc7403121d8baa45577609d735d25e32385db0830", "source": "sources/w13-skill-ramp-b-evaluate-creature-ramp-at-rank/evaluate_creature_ramp_at_rank.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "evaluateRampAtRank__5Skillii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_evaluate_ramp_at_rank.o", "profile": "authored_cpp", "sha256": "3446de7d9c0a19fc63ea99f97297b79b12f4d4c1ad421ea3d9e8af4ade451d50", "source": "sources/w13-skill-ramp-b-evaluate-ramp-at-rank/evaluate_ramp_at_rank.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "evaluateRampAtRank__5SkilliiP8ListHead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_evaluate_ramp_at_rank_list.o", "profile": "authored_cpp", "sha256": "2168e41afce8f27ab0d6c14692e3e8d62aa7c489efd9981e4d29049b4625e768", "source": "sources/w13-skill-ramp-b-evaluate-ramp-at-rank-list/evaluate_ramp_at_rank_list.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "evaluateRampAtRank__5SkilliiP10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_evaluate_ramp_at_rank_object.o", "profile": "authored_cpp", "sha256": "92eba4d41ff8b926f5483f578ccb34fd1f432c4a4fb4089bc1193eda659cb242", "source": "sources/w13-skill-ramp-b-evaluate-ramp-at-rank-object/evaluate_ramp_at_rank_object.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 628, "name": "getRampDamage__5SkilliifiPiT5"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_get_ramp_damage.o", "profile": "authored_cpp", "sha256": "b68e9463791352b2e6ac0980bf32ff994b8aa8093e0bf81970346163c22c7b63", "source": "sources/w13-skill-ramp-b-get-ramp-damage/get_ramp_damage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "initRamps__18SkillAncestralCall"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_ancestral_call.o", "profile": "authored_cpp", "sha256": "8c100fe094cbf663b8bd1a59e765f0c7f4fdab11f31eee10c640f51820e51861", "source": "sources/w13-skill-ramp-b-init-ramps-skill-ancestral-call/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "initRamps__9SkillBash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_bash.o", "profile": "authored_cpp", "sha256": "ae36715d10811b48bc91cc56cd24611ccb697555a3a7d27ee3124e0158080400", "source": "sources/w13-skill-ramp-b-init-ramps-skill-bash/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "initRamps__18SkillBlindingLight"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_blinding_light.o", "profile": "authored_cpp", "sha256": "b934f9a8ba0a9ccc4e2a24d7c2dafda60db33c23b611e007f85677751e840d0e", "source": "sources/w13-skill-ramp-b-init-ramps-skill-blinding-light/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__17SkillConvertEnemy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_convert_enemy.o", "profile": "authored_cpp", "sha256": "bc75972dd02a0a1d5f4a186dd81d1b9fd854fd17d22c234017caccb756e19b54", "source": "sources/w13-skill-ramp-b-init-ramps-skill-convert-enemy/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__18SkillConvertUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_convert_undead.o", "profile": "authored_cpp", "sha256": "18abea40beecdf7ffbd1834ed6e79ee969a194442f3fb57f51dade4277de222a", "source": "sources/w13-skill-ramp-b-init-ramps-skill-convert-undead/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 132, "name": "initRamps__18SkillDiseaseShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_disease_shield.o", "profile": "authored_cpp", "sha256": "b807bc266f7722748009206d786e192639a9612d55412647ec39baff32a52fcd", "source": "sources/w13-skill-ramp-b-init-ramps-skill-disease-shield/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "initRamps__17SkillDiseaseTrail"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_disease_trail.o", "profile": "authored_cpp", "sha256": "8d216b1c825f317725ab3afb80ec31135d0a4affcc265287a0e53e00026d9819", "source": "sources/w13-skill-ramp-b-init-ramps-skill-disease-trail/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "initRamps__18SkillDismissUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_dismiss_undead.o", "profile": "authored_cpp", "sha256": "ecfbfcab77cc22fedf094f4e3d721e5d604b30f253ba2068394e6ae9ed17d9a6", "source": "sources/w13-skill-ramp-b-init-ramps-skill-dismiss-undead/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "initRamps__17SkillExplodeArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_explode_arrow.o", "profile": "authored_cpp", "sha256": "88044aae61384c2326a305443fef81330dba4210aadd67296d6a542c06ca972b", "source": "sources/w13-skill-ramp-b-init-ramps-skill-explode-arrow/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "initRamps__16SkillFireWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_fire_weapons.o", "profile": "authored_cpp", "sha256": "a5727afa65180d1cab87dec29487dee857902fff034e3ee22d6231ced00e3f2f", "source": "sources/w13-skill-ramp-b-init-ramps-skill-fire-weapons/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "initRamps__16SkillGroundPound"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_ground_pound.o", "profile": "authored_cpp", "sha256": "5595b21c00f19ac130d59fd15a8871b32cf5d6b55235764635ca726caec6f411", "source": "sources/w13-skill-ramp-b-init-ramps-skill-ground-pound/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "initRamps__18SkillHammerOfWrath"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_hammer_of_wrath.o", "profile": "authored_cpp", "sha256": "f9069cf7bd270d1ed24b707e22ea2e2caea8ab747949c16f18d0c9f7ea1aff1e", "source": "sources/w13-skill-ramp-b-init-ramps-skill-hammer-of-wrath/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "initRamps__21SkillLightningWeapons"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_lightning_weapons.o", "profile": "authored_cpp", "sha256": "6f8466940440d78096439485ecf62bd8722f61e8986c695b5c17e9a47d48fd02", "source": "sources/w13-skill-ramp-b-init-ramps-skill-lightning-weapons/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "initRamps__17SkillMinorHealing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_minor_healing.o", "profile": "authored_cpp", "sha256": "28a5b3097c81892ce0e525d701bb252e0c64f2e46ec92a0a7c3cec666ef68350", "source": "sources/w13-skill-ramp-b-init-ramps-skill-minor-healing/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "initRamps__16SkillPoisonArrow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_poison_arrow.o", "profile": "authored_cpp", "sha256": "795307d0154ee3be75ee3b635c21667f6b268dcad079a1498434903c82f70e8a", "source": "sources/w13-skill-ramp-b-init-ramps-skill-poison-arrow/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__22SkillRangerCriticalHit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_ranger_critical_hit.o", "profile": "authored_cpp", "sha256": "41cc3328fccf731edfde25890652811eb3f5d5408a40520e1140b011792625e5", "source": "sources/w13-skill-ramp-b-init-ramps-skill-ranger-critical-hit/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "initRamps__17SkillRegeneration"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_regeneration.o", "profile": "authored_cpp", "sha256": "8883c7053d6f96b8fac75917b3b3fb7c75b105207a5138bc2a500e107f661acb", "source": "sources/w13-skill-ramp-b-init-ramps-skill-regeneration/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "initRamps__18SkillRepulseUndead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_repulse_undead.o", "profile": "authored_cpp", "sha256": "f2bc575508f859fd43836119623fadbe825c584ce5d4b9e4ba0d91411df4e977", "source": "sources/w13-skill-ramp-b-init-ramps-skill-repulse-undead/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "initRamps__9SkillRoot"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_root.o", "profile": "authored_cpp", "sha256": "f94bd170c195d288d8cfc03afadb6a144a0da3660d37ad9a688a7a1302e75ca2", "source": "sources/w13-skill-ramp-b-init-ramps-skill-root/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 216, "name": "initRamps__16SkillShockOfFire"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_shock_of_fire.o", "profile": "authored_cpp", "sha256": "e598cfc46e44d51720d59784eff910a96864d7d59c45213d4a89f7b5ec5a8849", "source": "sources/w13-skill-ramp-b-init-ramps-skill-shock-of-fire/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "initRamps__17SkillShockOfFrost"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_shock_of_frost.o", "profile": "authored_cpp", "sha256": "363615e9b020997d0f4c91cc5ae277717d661af627ffcadcfd4548cb8678959c", "source": "sources/w13-skill-ramp-b-init-ramps-skill-shock-of-frost/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "initRamps__19SkillSummonSkeleton"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_summon_skeleton.o", "profile": "authored_cpp", "sha256": "c609bff1dcbc30c54efbfda1ccb75c82d2c9d6a214cf64e75f42ca67de694194", "source": "sources/w13-skill-ramp-b-init-ramps-skill-summon-skeleton/init_ramps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "initRamps__17SkillUndeadShield"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13skillrampb_init_ramps_skill_undead_shield.o", "profile": "authored_cpp", "sha256": "25292b43cd663e00123af42de9d174cf49d27bf67e84e10c6f2bcc7cbb3322fe", "source": "sources/w13-skill-ramp-b-init-ramps-skill-undead-shield/init_ramps.cpp"}
```

## Authored but never executed

All 29 functions ran only in the host checks and in the private interpreter against the image function, with the
table slots, `syncRandState` and objects supplied by the test. None was run by the game, and no caller (skill system
start-up, cast or damage code) was involved. The four table-calling functions' slot targets were hooks, not the
real evaluators.

## What is unknown

- Names of the object members and the game meaning of every number the `initRamps` functions store.
- Which flag bit 0x40 at +12 of a game object stands for, and what the first list node represents.
- Behaviour for a slot index above 4, a null receiver, aliasing `low`/`high`, or output pointers into `syncRandState`.
- Whether the odd float patterns come from the original compiler's literal conversion.
- Byte-level equality with the image: compiled bodies differ in size (global-pointer access, tail calls); none is claimed.
