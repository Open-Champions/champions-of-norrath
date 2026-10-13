# w13-spell-msg-a: spell and projectile object handlers (part 1 of 2)

Branch `agents/w13-spell-msg-a`. Every function below is its own commit (one small follow-up commit, `ec01f3ff`, adds the helper
`call_entry_112_mode` to the shared header `sources/w13-spell-msg-a-common/spell.hpp`; it was left out of the `msg_run__12ShockOfFrost`
commit by mistake, so that one commit does not compile alone; the cone-of-fire commit adds `call_entry_104_mode` to the same header). Sources are under `experimental/sources/w13-spell-msg-a-*`; each source starts with a comment stating what
was observed in the private image, what is inferred and what is unknown. Nothing from the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`. Addresses and sizes below are that file's symbol values (`nm -S`);
"compiled bytes" is the size of the function compiled with the reviewed EE g++ and the `authored_cpp` flags. These are behaviour
reconstructions, not byte matches, so sizes differ from the image (several image functions repeat the unit-ball draw inline, the
sources use loops).

## What was done

All 29 assigned imports are closed by 29 units. Each was compared with the original in a private R5900 interpreter harness (outside
git): original and authored function run from identical memory snapshots with identical Python stubs for every callee; callee logs
(argument registers, float registers, and the bytes behind pointer arguments), the final state and every changed byte outside the
stack must match, and instruction coverage of the original is recorded. Each comparison was also run against deliberately wrong
variants (mutants: a constant off by one ulp or one unit, a wrong offset, a flipped compare, a changed loop count, a wrong mask); the
survivors are listed under "Known weak spots". Nothing here ran on an EE.

Findings that shaped the sources (observed): the function-pointer table calls take their first argument word from the call site
(`1` for the frost shard caster and the cone-of-fire caster, `0` elsewhere); `Point3` arguments are passed by address; the
double-precision sequence in `msg_run__15FrostStormSpell` is `0.0 - |x|` and is matched exactly; the single-precision R5900 has no
NaN so tests never use one; the cone-of-fire routine uses `mult1` (MMI) and `eRand__5Spell`.

## Units

| unit | symbol | image | compiled bytes | commit |
|---|---|---|---|---|
| `w13-spell-msg-a-clone-14poisongasspell` | `clone__14poisonGasSpell` | `0x001298e8`, 332 bytes | 288 | `f7532bdb` |
| `w13-spell-msg-a-clone-15froststormspell` | `clone__15FrostStormSpell` | `0x00124240`, 492 bytes | 356 | `16480926` |
| `w13-spell-msg-a-clone-15shockprojectile` | `clone__15ShockProjectile` | `0x00134ec0`, 8 bytes | 8 | `a9ddf01b` |
| `w13-spell-msg-a-clone-16beetleprojectile` | `clone__16beetleProjectile` | `0x001266d0`, 1008 bytes | 772 | `6dc52d74` |
| `w13-spell-msg-a-msg-collision-11diseasebolt` | `msg_collision__11DiseaseBoltP10GameObject` | `0x002930f0`, 548 bytes | 480 | `7370f70d` |
| `w13-spell-msg-a-msg-collision-12shockoffrost` | `msg_collision__12ShockOfFrostP10GameObject` | `0x002bd800`, 588 bytes | 524 | `50735476` |
| `w13-spell-msg-a-msg-collision-16poisonprojectile` | `msg_collision__16PoisonProjectileP10GameObject` | `0x0020fe10`, 280 bytes | 232 | `f86db0c2` |
| `w13-spell-msg-a-msg-draw-11diseasebolt` | `msg_draw__11DiseaseBolt` | `0x00293628`, 268 bytes | 256 | `39b2b10e` |
| `w13-spell-msg-a-msg-draw-12shockoffrost` | `msg_draw__12ShockOfFrost` | `0x002bdef0`, 280 bytes | 272 | `d2f86c98` |
| `w13-spell-msg-a-msg-draw-14hateprojectile` | `msg_draw__14HateProjectile` | `0x001fe670`, 284 bytes | 308 | `63ac7140` |
| `w13-spell-msg-a-msg-draw-14poisongasspell` | `msg_draw__14poisonGasSpell` | `0x001e6820`, 8 bytes | 8 | `b0eae6c7` |
| `w13-spell-msg-a-msg-draw-15froststormspell` | `msg_draw__15FrostStormSpell` | `0x001e6b08`, 8 bytes | 8 | `26f2f6db` |
| `w13-spell-msg-a-msg-draw-15shockprojectile` | `msg_draw__15ShockProjectile` | `0x0028d4d0`, 280 bytes | 308 | `ba072e45` |
| `w13-spell-msg-a-msg-draw-16beetleprojectile` | `msg_draw__16beetleProjectile` | `0x00212618`, 8 bytes | 8 | `0c7b153b` |
| `w13-spell-msg-a-msg-draw-16poisonprojectile` | `msg_draw__16PoisonProjectile` | `0x0020fd48`, 200 bytes | 188 | `65663600` |
| `w13-spell-msg-a-msg-run-10mummysnake` | `msg_run__10MummySnake` | `0x0023b6c0`, 1448 bytes | 1352 | `70c738f4` |
| `w13-spell-msg-a-msg-run-10poisonrain` | `msg_run__10PoisonRain` | `0x002be6d0`, 1708 bytes | 1804 | `96cb9e11` |
| `w13-spell-msg-a-msg-run-10spawnmummy` | `msg_run__10SpawnMummy` | `0x0023b588`, 312 bytes | 384 | `b5c9f7c8` |
| `w13-spell-msg-a-msg-run-11boltgoround` | `msg_run__11BoltGoRound` | `0x001e9528`, 1000 bytes | 952 | `1b43f076` |
| `w13-spell-msg-a-msg-run-11diseasebolt` | `msg_run__11DiseaseBolt` | `0x00292358`, 3476 bytes | 904 | `6d423cd8` |
| `w13-spell-msg-a-msg-run-12shockoffrost` | `msg_run__12ShockOfFrost` | `0x002bca20`, 3548 bytes | 1724 | `be61d5c1` |
| `w13-spell-msg-a-msg-run-14hateprojectile` | `msg_run__14HateProjectile` | `0x001fd9a8`, 3268 bytes | 2888 | `c24b9be6` |
| `w13-spell-msg-a-msg-run-14poisongasspell` | `msg_run__14poisonGasSpell` | `0x001e0ce0`, 1196 bytes | 1136 | `1a8480b1` |
| `w13-spell-msg-a-msg-run-15froststormspell` | `msg_run__15FrostStormSpell` | `0x001e23f8`, 3596 bytes | 3340 | `1728bed0` |
| `w13-spell-msg-a-msg-run-15shockprojectile` | `msg_run__15ShockProjectile` | `0x0028c910`, 3004 bytes | 2412 | `5a103121` |
| `w13-spell-msg-a-msg-run-16beetleprojectile` | `msg_run__16beetleProjectile` | `0x00211f70`, 1204 bytes | 1216 | `c9edbe04` |
| `w13-spell-msg-a-msg-run-16poisonprojectile` | `msg_run__16PoisonProjectile` | `0x0020f390`, 908 bytes | 956 | `54cd5825` |
| `w13-spell-msg-a-run-14spellharmtouch` | `run__14SpellHarmTouchG6Point3s` | `0x0029b170`, 168 bytes | 176 | `c958a23b` |
| `w13-spell-msg-a-run-15spellconeoffire` | `run__15SpellConeOfFireG6Point3s` | `0x002b5ad8`, 3248 bytes | 1148 | `f885e724` |

Twenty-three units were committed by an earlier, interrupted run of this package (the commits up to and including `70c738f4`);
the other six (`msg_run__14HateProjectile`, `msg_run__11DiseaseBolt`, `msg_run__12ShockOfFrost`, `msg_run__15ShockProjectile`,
`msg_run__15FrostStormSpell`, `run__15SpellConeOfFireG6Point3s`) were reviewed or authored in this run. The hate projectile source
was found uncommitted in the worktree; it was re-run against the original (0 mismatches over 400 scenarios, coverage 813/817), its
mutants were run, and it was committed unchanged.

## Imports closed

- `clone__14poisonGasSpell`
- `clone__15FrostStormSpell`
- `clone__15ShockProjectile`
- `clone__16beetleProjectile`
- `msg_collision__11DiseaseBoltP10GameObject`
- `msg_collision__12ShockOfFrostP10GameObject`
- `msg_collision__16PoisonProjectileP10GameObject`
- `msg_draw__11DiseaseBolt`
- `msg_draw__12ShockOfFrost`
- `msg_draw__14HateProjectile`
- `msg_draw__14poisonGasSpell`
- `msg_draw__15FrostStormSpell`
- `msg_draw__15ShockProjectile`
- `msg_draw__16beetleProjectile`
- `msg_draw__16PoisonProjectile`
- `msg_run__10MummySnake`
- `msg_run__10PoisonRain`
- `msg_run__10SpawnMummy`
- `msg_run__11BoltGoRound`
- `msg_run__11DiseaseBolt`
- `msg_run__12ShockOfFrost`
- `msg_run__14HateProjectile`
- `msg_run__14poisonGasSpell`
- `msg_run__15FrostStormSpell`
- `msg_run__15ShockProjectile`
- `msg_run__16beetleProjectile`
- `msg_run__16PoisonProjectile`
- `run__14SpellHarmTouchG6Point3s`
- `run__15SpellConeOfFireG6Point3s`

## Imports opened

Names that the new sources reference and that no unit in the shared snapshot defines (so they stay unresolved imports until another
package or the lead integrator supplies them); this list was derived by compiling each unit and diffing undefined against defined
symbols, not from a full partial build:

- `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`
- `SnakeParticle`
- `__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill`
- `__5MummyG6Point3sPPcPc`
- `__9LooseFireG6Point3P10GameObject`
- `__tf15SpellHateEffect`
- `_vt$10FrostShard`
- `_vt$10GameObject`
- `_vt$10Projectile`
- `_vt$13MissileWeapon`
- `_vt$14poisonGasSpell`
- `_vt$15FrostStormSpell`
- `_vt$16beetleProjectile`
- `_vt$18FrostStormIceShard`
- `addEffect__FiG6Point3`
- `buildShockChainList__14HateProjectileP6Point3`
- `buildShockChainList__15ShockProjectileP6Point3`
- `checkIfSpellIsRunning__8CreatureRC9type_info`
- `frameNm`
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`
- `haloTexture5`
- `msg_run__9DustDevil`
- `player`
- `skills`
- `syncRandState`
- `world`
- `worldDifficulty`

Address-named views of the file-local particle definitions (80-byte bss objects of the image whose names are local symbols and
cannot be linked by name). Each is `unsigned char norrath_w13_spell_msg_a_view_<hex>[80]` and must be provided as storage at that
address by whoever owns the data layout (this package did not define them, to avoid claiming storage):

- `norrath_w13_spell_msg_a_view_7d6970`
- `norrath_w13_spell_msg_a_view_7d6a10`
- `norrath_w13_spell_msg_a_view_7d6a60`
- `norrath_w13_spell_msg_a_view_7d6b00`
- `norrath_w13_spell_msg_a_view_7d6fc0`
- `norrath_w13_spell_msg_a_view_7d7010`
- `norrath_w13_spell_msg_a_view_7d7260`
- `norrath_w13_spell_msg_a_view_7d72b0`
- `norrath_w13_spell_msg_a_view_7d7300`
- `norrath_w13_spell_msg_a_view_8d6320`
- `norrath_w13_spell_msg_a_view_8d6370`
- `norrath_w13_spell_msg_a_view_8d67d0`
- `norrath_w13_spell_msg_a_view_8d6820`
- `norrath_w13_spell_msg_a_view_8d6870`
- `norrath_w13_spell_msg_a_view_8d7b30`
- `norrath_w13_spell_msg_a_view_8d7b80`
- `norrath_w13_spell_msg_a_view_8d7bd0`
- `norrath_w13_spell_msg_a_view_8d80a0`
- `norrath_w13_spell_msg_a_view_8d80f0`
- `norrath_w13_spell_msg_a_view_8d8140`
- `norrath_w13_spell_msg_a_view_8d8190`
- `norrath_w13_spell_msg_a_view_8d81e0`

(Two of the vtable names used by the new sources, `_vt$10FrostShard` and `_vt$18FrostStormIceShard`, are the class tables of
objects that other packages construct; they are only read as addresses and stored into a new object's word at +72.)

## Build-plan entries needed

`docs/agents/w13-spell-msg-a-plan-entries.py --toolchain <ee toolchain> [--private-root DIR]` compiles all units of this package and
prints their plan entries (one JSON object per line, input order assigned by the integrator); with `--private-root` it writes a
private plan/snapshot copy outside the repository for a check build. The shared `experimental/build-plan.json` and
`experimental/snapshot.json` are not edited by this package. Each unit's header closure is `sources/w13-spell-msg-a-common/spell.hpp`
and `sources/w12-actor-spell-c-common/access.hpp` (the unit-ball and table-call accessors). The script prints the exact entries;
they are not reproduced here to avoid a stale copy.

## Authored but never executed

All 29 units: nothing was run on an EE or inside a build of the game. They were executed only in the private interpreter harness
described above, against the original function, in synthetic memory with stubbed callees. Specifically never executed with real
callees: the table calls (`call_entry*`), the PhProp, Projectile, GameObject and SpellHateEffect constructors, `objectFindInBox`,
`gameAttackResolver`, the lightning and particle calls, and the inlined FrostShard / FrostStormIceShard object setup.

## Known weak spots (mutants the comparison did not catch)

- `msg_run__14HateProjectile`: the loop-bound distance and the 0.2 factor at one-ulp changes (a larger change is caught).
- `msg_run__11DiseaseBolt`: `magnitude < limit` against `<=` (behaviourally identical at equality).
- `msg_run__15ShockProjectile`: a one-ulp change of the 0.2 factor (a 20-ulp change is caught).
- `msg_run__15FrostStormSpell`: a one-ulp change of two pool constants and `<=` against `<` at an exact float equality.
- All units: the item-description search loop (`fill_description`) only ever sees a fresh description with no entries, so its
  non-first-entry branches are uncovered (the original has them as unreachable code in these callers).

## Unknown

- What the object fields at the offsets used here mean (the per-class words and singles: for example O+76..+96, +384..+396 in the
  hate projectile, +392..+404 in the shock projectile, +76..+108 in the frost storm). Sources name them only by offset.
- What the pool constants mean (several are non-round, such as 9.0369, 31.198, 6.9999, 16.000006, 1.716, 44521.0).
- What the flag words (`0x421d`, `274`, `0x4081`), item-description kinds (`21`, `22`, `24`, `38`) and the hit kinds select.
- The meaning of the mode word (`0` or `1`) of the table calls at offsets 104 and 112.
- The byte-exact layouts of `ListHead`, `Matrix34`, the 80-byte particle definitions and the 124-byte FrostShard object.
- Why the image repeats some unit-ball draws inline (compiler unrolling is the only evidence).
