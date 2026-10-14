# w16-spellmsg-a: spell and projectile messages, spell run and item functions

Branch `agents/w16-spellmsg-a`. Every function below is its own commit (the commit hash is in the table). Sources are under
`experimental/sources/w16-spellmsg-a-*`; each source starts with a comment that separates what was observed in the private
image, what is inferred and what is unknown. Nothing from the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`. Addresses and sizes are that file's symbol values (`nm -S`); "compiled
bytes" is the size of the function compiled with the reviewed EE g++ and the `authored_cpp` profile. These are behaviour
reconstructions, not byte matches, so the sizes differ from the image.

## How each function was checked

Every function was compared with the original in a private R5900 interpreter harness (outside git): the original and the authored
function run from identical memory snapshots with identical Python stubs for every callee; the callee logs (argument registers,
float registers and the bytes behind pointer arguments), the changed memory outside the stack (and the returned value or the float
result where one exists) must match, and instruction coverage of the original is recorded (all but nop and delay-slot words for most
units). Each comparison was also run against deliberately wrong variants (a constant off by one ulp, a wrong offset, a flipped
compare, a changed loop count, a wrong table slot). Survivors are listed under "Known weak spots". Nothing here ran on an EE, and
there is no committed host test: the evidence is the private paired run recorded in each commit message and summarised here.
A few findings that shaped the sources (all observed): the table calls take their arguments exactly as written in each source
comment; constant float arithmetic that the EE g++ would fold (for example `x + 0.2` with both constants known) rounds differently
from the machine, so those additions are kept as run-time operations; `rsqrt.s` and `sqrt.s` are used where the image uses them; and
several image functions compute values that are never used (a position, a roll), which the sources keep only when they have an
observable effect (a roll draw consumes the random stream, so it is kept).

## Units


| unit | symbol | image | compiled bytes | commit |
|---|---|---|---|---|
| `w16-spellmsg-a-already-chained-14hateprojectile` | `alreadyChained__14HateProjectileP10GameObject` | `0x001ff6b8`, 64 bytes | 68 | `2a512d67` |
| `w16-spellmsg-a-already-chained-15shockprojectile` | `alreadyChained__15ShockProjectileP10GameObject` | `0x0028da08`, 64 bytes | 68 | `6788f1c2` |
| `w16-spellmsg-a-dtor-15spellconeoffire` | `_$_15SpellConeOfFire` | `0x002b5a00`, 216 bytes | 228 | `c3049a76` |
| `w16-spellmsg-a-msg-collision-10frostshard` | `msg_collision__10FrostShardP10GameObject` | `0x002bc528`, 604 bytes | 556 | `f4615faa` |
| `w16-spellmsg-a-msg-collision-10holystrike` | `msg_collision__10HolyStrikeP10GameObject` | `0x002b2520`, 680 bytes | 612 | `283e6ba5` |
| `w16-spellmsg-a-msg-collision-8holybolt` | `msg_collision__8HolyBoltP10GameObject` | `0x002bb4b0`, 980 bytes | 848 | `8c35c662` |
| `w16-spellmsg-a-msg-draw-10frostshard` | `msg_draw__10FrostShard` | `0x002bbd78`, 284 bytes | 280 | `bad6f076` |
| `w16-spellmsg-a-msg-draw-10holystrike` | `msg_draw__10HolyStrike` | `0x002b27c8`, 348 bytes | 260 | `5dc81355` |
| `w16-spellmsg-a-msg-draw-12glowingshaft` | `msg_draw__12GlowingShaft` | `0x001e15b0`, 868 bytes | 788 | `38163052` |
| `w16-spellmsg-a-msg-draw-13hammerofwrath` | `msg_draw__13HammerOfWrath` | `0x002b4bf0`, 1236 bytes | 1136 | `3f3763b6` |
| `w16-spellmsg-a-msg-draw-18froststormiceshard` | `msg_draw__18FrostStormIceShard` | `0x001e1918`, 292 bytes | 308 | `d50fe6b1` |
| `w16-spellmsg-a-msg-draw-8holybolt` | `msg_draw__8HolyBolt` | `0x002bb2f0`, 444 bytes | 484 | `41043f0a` |
| `w16-spellmsg-a-msg-draw-8lostsoul` | `msg_draw__8LostSoul` | `0x0021dd30`, 8 bytes | 8 | `2369ab46` |
| `w16-spellmsg-a-msg-run-10frostshard` | `msg_run__10FrostShard` | `0x002bbe98`, 1676 bytes | 988 | `c82f7b51` |
| `w16-spellmsg-a-msg-run-10holystrike` | `msg_run__10HolyStrike` | `0x002b1658`, 3780 bytes | 1104 | `b7f3f708` |
| `w16-spellmsg-a-msg-run-12glowingshaft` | `msg_run__12GlowingShaft` | `0x001e69c8`, 72 bytes | 64 | `a10bdeee` |
| `w16-spellmsg-a-msg-run-13hammerofwrath` | `_ZN12_GLOBAL__N_1L4pullEPhRK6Point3f` | n/a | 148 | `264d8c15` |
| `w16-spellmsg-a-msg-run-13hammerofwrath` | `msg_run__13HammerOfWrath` | `0x002b3968`, 4744 bytes | 4080 | `264d8c15` |
| `w16-spellmsg-a-msg-run-18darknessprojectile` | `msg_run__18DarknessProjectile` | `0x002979b0`, 1824 bytes | 1660 | `60036016` |
| `w16-spellmsg-a-msg-run-18froststormiceshard` | `msg_run__18FrostStormIceShard` | `0x001e1a40`, 1448 bytes | 520 | `c62eb2ae` |
| `w16-spellmsg-a-msg-run-19blindinglighteffect` | `msg_run__19BlindingLightEffect` | `0x002b9868`, 440 bytes | 424 | `a8413aff` |
| `w16-spellmsg-a-msg-run-8holybolt` | `msg_run__8HolyBolt` | `0x002bacd0`, 1568 bytes | 1516 | `d3cc38c5` |
| `w16-spellmsg-a-msg-run-8lostsoul` | `msg_run__8LostSoul` | `0x00219858`, 744 bytes | 600 | `7be9f5f5` |
| `w16-spellmsg-a-pos-in-aura-18spellancestralcall` | `posInAura__18SpellAncestralCallP6Point3PfT2` | `0x002a81e0`, 336 bytes | 368 | `3a52b28b` |
| `w16-spellmsg-a-run-12spellhealing` | `run__12SpellHealingG6Point3s` | `0x002aeb38`, 1600 bytes | 1248 | `d0a1ab07` |
| `w16-spellmsg-a-run-13spellentangle` | `run__13SpellEntangleG6Point3s` | `0x002c6630`, 1612 bytes | 1568 | `c3c0b5b9` |
| `w16-spellmsg-a-run-15spellclericroot` | `run__15SpellClericRootG6Point3s` | `0x002c59b8`, 1612 bytes | 1568 | `976ce50c` |
| `w16-spellmsg-a-run-15spellhateeffect` | `run__15SpellHateEffectG6Point3s` | `0x001fe8d0`, 548 bytes | 516 | `45d37fc7` |
| `w16-spellmsg-a-run-15spellholyshield` | `run__15SpellHolyShieldG6Point3s` | `0x002ae590`, 264 bytes | 244 | `38cf7a9a` |
| `w16-spellmsg-a-run-16spellconeoffrost` | `run__16SpellConeOfFrostG6Point3s` | `0x002af508`, 1616 bytes | 1460 | `6adafc3b` |
| `w16-spellmsg-a-run-16spellcriticalhit` | `run__16SpellCriticalHitG6Point3s` | `0x002ad528`, 1408 bytes | 1084 | `d719fb4c` |
| `w16-spellmsg-a-run-17spellminorhealing` | `run__17SpellMinorHealingG6Point3s` | `0x002a71c0`, 252 bytes | 240 | `fc435393` |
| `w16-spellmsg-a-skill-archery-get-damage-scale` | `getDamageScale__12SkillArcheryi` | `0x002a2c58`, 108 bytes | 104 | `dada389f` |

## Imports closed

Of the assigned names, these are defined by the units above (the compiled symbol names):

- `alreadyChained__14HateProjectileP10GameObject`
- `alreadyChained__15ShockProjectileP10GameObject`
- `_$_15SpellConeOfFire`
- `msg_collision__10FrostShardP10GameObject`
- `msg_collision__10HolyStrikeP10GameObject`
- `msg_collision__8HolyBoltP10GameObject`
- `msg_draw__10FrostShard`
- `msg_draw__10HolyStrike`
- `msg_draw__12GlowingShaft`
- `msg_draw__13HammerOfWrath`
- `msg_draw__18FrostStormIceShard`
- `msg_draw__8HolyBolt`
- `msg_draw__8LostSoul`
- `msg_run__10FrostShard`
- `msg_run__10HolyStrike`
- `msg_run__12GlowingShaft`
- `_ZN12_GLOBAL__N_1L4pullEPhRK6Point3f`
- `msg_run__13HammerOfWrath`
- `msg_run__18DarknessProjectile`
- `msg_run__18FrostStormIceShard`
- `msg_run__19BlindingLightEffect`
- `msg_run__8HolyBolt`
- `msg_run__8LostSoul`
- `posInAura__18SpellAncestralCallP6Point3PfT2`
- `run__12SpellHealingG6Point3s`
- `run__13SpellEntangleG6Point3s`
- `run__15SpellClericRootG6Point3s`
- `run__15SpellHateEffectG6Point3s`
- `run__15SpellHolyShieldG6Point3s`
- `run__16SpellConeOfFrostG6Point3s`
- `run__16SpellCriticalHitG6Point3s`
- `run__17SpellMinorHealingG6Point3s`
- `getDamageScale__12SkillArcheryi`

## Known weak spots

- Some comparisons survived deliberately wrong variants because the variant is observably equivalent (`<` against `<=` where the operands can never be equal, a modulus test
  shifted by one at a counter value that is also excluded elsewhere). Others are single-ulp float constants and exact-equality edges that the scenario generator reached
  only rarely; for `msg_run__13HammerOfWrath` the `dd < 0x46610000` edge and the `sword * 512 > 0x10000` edge were not distinguished.
- Scenario memory is random with chosen special values, so unexplored combinations of object fields may exist.

## Imports opened

These are the new names the units reference and nothing in the plan defines. The `norrath_w16_spellmsg_a_view_<address>` names are
address-named views of file-local bss objects of the image (particle definitions, a pair of `ResourceMap` objects and their two
first-use flags); their layouts are not established here, only the addresses and, for particle definitions, the 80-byte size.

- Views: `norrath_w16_spellmsg_a_view_486508`, `_48650c` (two first-use flag words of the HolyBolt draw), `_7d6ab0`, `_8d6b40`, `_8d76d0`, `_8d7770`,
  `_8d77c0`, `_8d7810`, `_8d7860`, `_8d7950`, `_8d79a0`, `_8d79f0`, `_8d7a40`, `_8d7d10`, `_8d7ef0`, `_8d7f00` (the HolyBolt `ResourceMap` objects), `_8d8000`,
  `_8d8050`, `_8d8290`, `_8d82e0`, `_8d8330`, `_8d8380`, `_8d83d0`, `_8d8420`.
- Functions and data of other packages (left unresolved on purpose): `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`,
  `__12DummyDamagerG6Point3T1iiifiP8Creatures`, `addEffect__FiG6Point3`, `matScale__FP8Matrix34fff`,
  `runEffect__16SpellCriticalHitP8Creature`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` (my package, not closed),
  plus the usual helpers (`eRand__*`, `icos__Fi`, `isin__Fi`, `iatan2__Fff`, `move__10ProjectileG6Point3`, `lightUpdatePosition__FP5Light`, ...) which the plan
  already defines, and the soft-double helpers `litodp` and `dpcmp`.

A partial build of a private copy of the plan (shared plan plus my entries, kept outside git) compiled all sources with the reviewed toolchain and
one relocatable link; of the assigned names the 30 imports listed under "Imports closed" were gone from `unresolved-imports.json`. The two units added
after that build (`msg_run__10HolyStrike`, `msg_run__13HammerOfWrath`) were compiled alone with the same flags; their entries are in the helper script.

## Build-plan entries needed

`experimental/build-plan.json` and `snapshot.json` are not edited by this package. Run

```
python3 docs/agents/w16-spellmsg-a-plan-entries.py --toolchain <reviewed ee> [--private-root DIR]
```

to print one JSON line per unit (source, header closure, hashes, emitted functions, compiled sizes); with `--private-root` it also writes a private copy of
the shared plan and snapshot with the entries added (nothing is written in the repository). The header closure reaches into the common headers of
packages `w12-actor-spell-c`, `w13-spell-msg-a` and `w15-spell-fx-a`, which are already selected. Two units compile a file-local helper (`burst`) that appears in
the emitted-function list as `_ZL5burst...` and must be present in the recorded functions.

## Not closed, and why

- `msg_run__6Player` (27,496 bytes): not attempted. It is roughly a quarter of the size of everything else in this package together and could not be read, written and
  verified in one sitting; earlier notes (w13-frontend-game-b, w12-hud-loop-b) reached the same conclusion.
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` (9,184 bytes): decoded only in part (observed: the function takes the victim, the attacker, two floats, four ints, a description and a flag; it has a deflect block that spawns 50 sparks, kind-dependent branches for kinds 21 to 29, 31 to 34, 38 and 41, an armour scale, a crit roll, and ends by playing floating text, a hit sound and calling the victim's hurt entry in its table). A prefix-checkpoint reconstruction was drafted privately but never compiled or compared, so nothing from it is committed; not closed.
- `disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi`: already authored by `w9-render-math-a-disintegration-effect` (its notes record 250 of
  251 instruction words reached) but not selected in the shared plan; no second copy was written.
- `monsterSpawn__FPPcG6Point3sb`: already authored by `level-anim-load-monster-spawn` under the invented name `norrath_monster_spawn`; the plan entry exports
  that name, not the image name. A link-level alias (or renaming the unit's symbol) closes it; no forwarding wrapper was written because a wrapper is not the
  original body.
- `norrath_item_create` and `norrath_item_create_random`: invented names for `create__4itemPcPQ24item11descriptionii` and `createRandom__4itemPQ24item11descriptioniiii`, both
  already authored (`w10-item-a-item-create`, `w9-hud-inventory-items-c-create-random`); the same alias situation recorded in w11-data-b and w12-hud-loop-b.
- `pack` and `poisonPoof`: bss particle definitions. `pack` is already defined as `ParticleDefStorage pack;` by
  `w8-spell-static-init-a-cold-arrow-projectile` and `poisonPoof` by `w10-actor-a-static-init-skill-poison-arrow`; neither unit is in the plan (the same finding is in
  w15-libs-eh-a and w14-projectile-b). Their owners should be selected; I wrote no second definition.

## Authored but never executed

All of it. No unit ran on an EE or in the game; the only evidence is the private R5900 interpreter comparison against the image, described above, which is not committed.

## Unknown

The meaning of most object fields (counters, state words, handles), the pool constants (named only by their bits), what the many file-local particle definitions and
resource maps contain, and the type names behind the table calls. Names in the sources are descriptive only where the function's visible behaviour supports them.
