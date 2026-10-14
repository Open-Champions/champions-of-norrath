# w14-projectile-a: projectile, spell and monster message handlers and actor constructors (part a)

Branch `agents/w14-projectile-a`. Every function below is its own commit (the shared accessor header `sources/w14-projectile-a-common/access.hpp`
came with the first commit and grew with the poison-projectile collision commit; one later commit only corrects a comment). Sources are under
`experimental/sources/w14-projectile-a-*`; each starts with a comment stating what was observed in the private image, what is inferred and what is
unknown. Nothing from the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and
sizes below are that file's symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; these are behaviour reconstructions, not byte matches, so the sizes differ from the image.

## What was done

All 27 assigned imports are closed by 27 units. Three groups:

- **Written from the image (17 units):** the constructors of `GlowingShaft`, `DarknessProjectile`, `LostSoul`, `Skelight`, `Prop` (the overload that takes a
  model and a texture), `HolyStrike` (the symbol `__10HolyStrikes...` is class `HolyStrike` followed by a short and an int, decoded from the
  mangling and the `_vt$10HolyStrike` table name), `HammerOfWrath`, `BlindingLightEffect` and `HolyBolt`; `clone__13FireBombletJr` and
  `msg_draw__13FireBombletJr`; `gameRadialDamage`; `Clear__12VICollBuffer`; and the four no-op bodies `msg_draw__14poisonGasSpell`,
  `msg_draw__15FrostStormSpell`, `clone__15ShockProjectile` (returns null) and `fxRunBallLightning`.
- **Re-homed and re-verified (7 units):** the draw handlers of `HateProjectile`, `ShockProjectile`, `ShockOfFrost` and `DiseaseBolt` and the collision
  handlers of `PoisonProjectile`, `DiseaseBolt` and `ShockOfFrost` already existed as reviewed `w13-spell-msg-a-*` units, but the integrator left them
  out of the plan because they have no committed check. This package may not edit those directories, so each body was copied into a
  `w14-projectile-a-*` unit (provenance line at the top), run again against the original in a fresh paired comparison, and given a host contract. The
  three collision units now use this package's header (it gained `init_description`, `fill_description` and a host-routable `call_entry_112`).
  **Select one unit of each pair, not both.**
- **Renamed re-homes (3 units):** `initType__8Skeletonb`, `init__10StaticFire` and `Init__9VIStationiPCcPcT3T2T2i`. The first two exist in
  `w9-rtti-vtables-entities-c-*` but are emitted under the adapter names `norrath_skeleton_init_type` and `norrath_static_fire_init`, so the symbols that the
  Skeleton and StaticFire constructors import stayed unresolved; the new units emit the real names (the old adapter units can stay selected, they
  are then simply unused). `Init__9VIStation...` exists as `w9-frontend-a-vi-station-init`, which is in the snapshot but not in the plan, and has no host
  contract. The original comparison scripts of the earlier packages were adapted and run again.

## Units

| unit | symbol | image | compiled bytes | paired scenarios | original words covered | source sha256 |
|---|---|---|---|---|---|---|
| `w14-projectile-a-vi-coll-buffer-clear` | `Clear__12VICollBuffer` | `0x0108b9e0`, 124 bytes | 172 | 10 | 31 / 31 | `fe4bcb217a0e75ff4878445556822bf4ec03276831c9b3cf50d52b0914ddfe21` |
| `w14-projectile-a-init-vi-station` | `Init__9VIStationiPCcPcT3T2T2i` | `0x010179c8`, 1260 bytes | 1192 | 48 | 314 / 315 | `a5e005adcc712805668145c30d3abc2a0ecf2b2c4d91745fc8dc8c02fb55cdb4` |
| `w14-projectile-a-ctor-holystrike` | `__10HolyStrikesiG6Point3P10GameObjectP5Skill` | `0x002b1460`, 504 bytes | 412 | 48 | 126 / 126 | `dc2822da3ce8571301b82265b2f570ba9a430e0ffa029bd5b29b40e5b976d880` |
| `w14-projectile-a-ctor-glowingshaft` | `__12GlowingShaftG6Point3iiii` | `0x001e6910`, 180 bytes | 176 | 40 | 45 / 45 | `a58b896e10acee0d408eb38782e4019231383fb88bf33a639901ec8aeb031193` |
| `w14-projectile-a-ctor-hammerofwrath` | `__13HammerOfWrathG6Point3sP10GameObjectiP5Skill` | `0x002b3700`, 612 bytes | 460 | 60 | 152 / 153 | `61523e38437727461bcaada745509db5aedb780113d82fddd2b97441396c24d1` |
| `w14-projectile-a-ctor-darknessprojectile` | `__18DarknessProjectileG6Point3T1P5SkilliP10GameObject` | `0x00297790`, 544 bytes | 472 | 30 | 136 / 136 | `e1cdc055568ccbd7e5abf7c42979a84ee3524c42c686757021aaf17178bebb28` |
| `w14-projectile-a-ctor-blindinglighteffect` | `__19BlindingLightEffectG6Point3iP8CreatureP5Skill` | `0x002b94d0`, 916 bytes | 832 | 150 | 229 / 229 | `62d69386a1b3a605c84afce23bbca7faeaa1f2d5ed31667b0c7a8eb5855d24cc` |
| `w14-projectile-a-ctor-prop-model` | `__4PropG6Point3P12_modelHeaderP8_textureli` | `0x001478b0`, 404 bytes | 448 | 60 | 101 / 101 | `4f53b3363c2a2cf7de6fee874a3f7115e2dafb3906dec7759b6a88874c318479` |
| `w14-projectile-a-ctor-holybolt` | `__8HolyBoltG6Point3iiP8CreatureP5Skill` | `0x002ba788`, 1348 bytes | 1192 | 105 | 336 / 337 | `64198eaad7ce580c150450287cd724e13386f033b5b749461a3e92a19e3dd0ff` |
| `w14-projectile-a-ctor-lostsoul` | `__8LostSoulG6Point3T1iP10_vagHeaderii` | `0x00219638`, 540 bytes | 476 | 40 | 134 / 135 | `6fd6bd19a683a99ed3008a56948cc91719c0e2e95b0da2ff12bb037b42065d78` |
| `w14-projectile-a-ctor-skelight` | `__8SkelightG6Point3sPPcci` | `0x00246310`, 648 bytes | 652 | 40 | 162 / 162 | `e277e6448f2f0bb3d6a6525dfbb8f3f39434992a820db1d3ab8a370490dc15a1` |
| `w14-projectile-a-clone-firebombletjr` | `clone__13FireBombletJr` | `0x00128830`, 328 bytes | 344 | 7 | 82 / 82 | `34ba99825c7a17e170640494172d40e59052629225f2457e00f3b5c69f854a60` |
| `w14-projectile-a-clone-shockprojectile` | `clone__15ShockProjectile` | `0x00134ec0`, 8 bytes | 8 | 6 | 2 / 2 | `27c24e8dc5fc8321e16c2e8876b8404c9af01e97839232a5a26380925705fc20` |
| `w14-projectile-a-fx-run-ball-lightning` | `fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii` | `0x001df560`, 36 bytes | 8 | 6 | 9 / 9 | `7204e67a36d325f27958d108fa8795e3bc881eb1628c123b31bd50bad0bb639d` |
| `w14-projectile-a-game-radial-damage` | `gameRadialDamage__FP10GameObjectG6Point3iffiii` | `0x001d0698`, 616 bytes | 572 | 306 | 153 / 154 | `1ce4904ba212d47d912e4af83677c7a01c5afdf3a5c351333c87331b5ea02882` |
| `w14-projectile-a-init-type-skeleton` | `initType__8Skeletonb` | `0x002442b8`, 1088 bytes | 1488 | 1911 | 267 / 272 | `63662c5b4fafaea0a8920e04a2767daa8bf9dbd73285f1a1bd4f4cf2124de6ae` |
| `w14-projectile-a-init-static-fire` | `init__10StaticFire` | `0x0028df70`, 276 bytes | 640 | 108 | 68 / 69 | `7939f9847b0ee231b0465b9552f1bab954b85d0843aa662e475a1bc2ad63175e` |
| `w14-projectile-a-msg-collision-diseasebolt` | `msg_collision__11DiseaseBoltP10GameObject` | `0x002930f0`, 548 bytes | 480 | 400 | 129 / 137 | `e44ae7efe59a0bbebe5df9b138e7372a7129083ab6c3fd7de59ab9bcf77d7b0a` |
| `w14-projectile-a-msg-collision-shockoffrost` | `msg_collision__12ShockOfFrostP10GameObject` | `0x002bd800`, 588 bytes | 524 | 430 | 138 / 147 | `d80a74a881f35a2fd602ff11eb16a50edb8b991eccdc8387e2d4eb4a6f027d1c` |
| `w14-projectile-a-msg-collision-poisonprojectile` | `msg_collision__16PoisonProjectileP10GameObject` | `0x0020fe10`, 280 bytes | 300 | 80 | 60 / 70 | `61c98f7f77d51ec0f97bcf055c6a7127aa0209d8af52b6bc65e0cf36cb25434f` |
| `w14-projectile-a-msg-draw-diseasebolt` | `msg_draw__11DiseaseBolt` | `0x00293628`, 268 bytes | 256 | 40 | 67 / 67 | `9a3ac75eaa5313564b9f11d9d193393c9f2dba16ff217a0e81e76f474c372735` |
| `w14-projectile-a-msg-draw-shockoffrost` | `msg_draw__12ShockOfFrost` | `0x002bdef0`, 280 bytes | 272 | 40 | 70 / 70 | `e7010e05d4e4abd2dfc84b86d067de9bc48682c206a81c55c969d61315a01aa9` |
| `w14-projectile-a-msg-draw-firebombletjr` | `msg_draw__13FireBombletJr` | `0x0028b468`, 216 bytes | 172 | 10 | 54 / 54 | `85abc384c46bf106f8221fee40f90a3d7832498c75e16b43bf9212a2f2eed5f0` |
| `w14-projectile-a-msg-draw-hateprojectile` | `msg_draw__14HateProjectile` | `0x001fe670`, 284 bytes | 308 | 150 | 71 / 71 | `59f891eceda6193ffed8e2412ce2bdf5dfaeddee7c06d604437a688a1197d9fb` |
| `w14-projectile-a-msg-draw-poisongasspell` | `msg_draw__14poisonGasSpell` | `0x001e6820`, 8 bytes | 8 | 6 | 2 / 2 | `ac2fa8b3ac213d9a6e22e69be0a53e0c51902236280272f614e2f4cda03c4bff` |
| `w14-projectile-a-msg-draw-froststormspell` | `msg_draw__15FrostStormSpell` | `0x001e6b08`, 8 bytes | 8 | 6 | 2 / 2 | `4764dc839d73eaa58f80a96823a788b1f8121abb15038f900606797ea15cd004` |
| `w14-projectile-a-msg-draw-shockprojectile` | `msg_draw__15ShockProjectile` | `0x0028d4d0`, 280 bytes | 308 | 150 | 70 / 70 | `87cddba26086cb5917415cbafb66dc89453d00cb6ad1fdd8854a31530d67fba2` |

Commit of each unit: `git log --diff-filter=A --format=%h -- experimental/sources/<unit>` (all on this branch; the last commit is the comment fix).

## Checks performed

- Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags (`docs/agents/w14-projectile-a-plan-entries.py` compiles all 27 with the
  driver's own flags and prints the plan entries below). A private loop (host build of each `host_check.cpp`, run, EE compile of the unit) was run
  again for all 27 units after the last change to the shared header; all 27 host contracts exit 0.
- Each unit has a **host contract** (`host_check.cpp`, not a plan input) that stubs every callee, logs the calls and asserts argument values, branch
  choices, store offsets and the bytes that must stay untouched. Typical boundary cases: null and non-null owner links, the filter bits, a roll of exactly
  zero, boss-halved thresholds, null allocation results, every failure position of `Init__9VIStation` and the unsigned/signed conversions.
- Each unit was compared with the **original function** in a private R5900 interpreter (outside git; `priv-w14-projectile-a`, derived from the
  `priv-w13-spell-msg-b` copy): original and authored function run from identical memory snapshots with identical Python stubs for every callee; the
  call logs (argument registers, float registers and the bytes behind pointer arguments), the return state and every changed byte outside the stack
  must match. Scenario counts and original-word coverage are in the table. The uncovered words are delay-slot nops, the non-first-entry
  branches of the item-description search that a fresh description never reaches (the three attack-resolver collisions), and the null-object
  `__throw_bad_typeid` paths of `msg_collision__16PoisonProjectileP10GameObject` (the image goes on after that call; the authored code declares it
  non-returning, so those paths cannot be compared; the host contract checks that the throw is reached before any read).
- Mutants (a constant, an offset, a compare, a mask, a loop count, a stored value, a call order) were run against both the paired comparison and the host
  contract for every unit. Survivors, by design or by weak scenarios, are listed under "Weak spots" below.
- The paired runs found real mistakes during authoring that were fixed before each commit: the Skelight constructor first stored the 64-bit chunk mask
  after `animInitState` (the image stores it in the call's delay slot, before the call); the byte flag and short type of that constructor needed an
  explicit sign extension (the image extends on entry); the DarknessProjectile and LostSoul scripts needed a per-run reset of the resource counter; the
  BlindingLight boss halving mutant (`+ 2`) first survived until rolls near the threshold were added.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests, OK). `python3 tools/check_source_snapshot.py` reports unrecorded source
  files, as it does for any branch that adds sources without the integrator's snapshot update.

## Partial build

A private partial build (shared plan plus the 27 entries, written outside git with `docs/agents/w14-projectile-a-plan-entries.py --private-root`) compiled
6,262 inputs with one link: 4,948 function definitions, 946 unresolved imports, zero COMMON symbols. None of the 27 assigned names is among the
unresolved imports; all 27 were referenced by other plan units before (so each was a real import). The tip it was measured on had 957 unresolved
imports; 957 - 27 closed + 16 opened = 946. No retry, no alias and no original input was used. This is a partial relocatable object, not an executable.

## Deviations from the image, none hidden

- `initType`, `Skelight` and `HammerOfWrath`/`HolyStrike`/`HolyBolt` take their short or byte arguments as `int` and narrow them explicitly, because the image
  sign-extends those on entry (callee side) and the current compiler would otherwise assume the caller did.
- `msg_collision__12ShockOfFrostP10GameObject` reads the flag word of its second argument before testing it for null, as the image does; the host
  contract never passes null there (the paired run does, 30 scenarios).
- File-local string and table data that the units use (`"fx.lmp"`, sound names, the item-name switch of `HammerOfWrath`) are compiled as local data of
  each unit; the image has its own copies at other addresses, so only string contents are compared, never addresses.
- The `0x3e4ccccc`, `0x3eaa7ef9`, `0x3ea4dd2f`, `0x3891a2b3`, `0x3c888888` and similar singles are read from the image's small-data/`.lit4` pool and given
  as bit patterns; the image has no symbol for them.
- `msg_collision__11DiseaseBoltP10GameObject` and `msg_collision__12ShockOfFrostP10GameObject`: the `item::description` is a stack buffer of 80
  bytes of which only the written bytes are compared (the rest is uninitialised on the target too).

## Weak spots (mutants the checks did not catch)

- `gameRadialDamage`: a one-ulp change of the 0.333 factor (`0x3eaa7ef9`).
- `__19BlindingLightEffect...`: a one-ulp change of the 100.0 roll scale.
- `__8HolyBolt...`: a one-ulp change of the 10,000,000.0 start distances (not observable with the scenario sizes used).
- `msg_draw__14HateProjectile`, `msg_draw__15ShockProjectile`: one-ulp changes of the pool singles and `1.0 < u` against `<=` at exactly `u == 1.0` (the same value results).
- `msg_collision__11DiseaseBoltP10GameObject`: a one-ulp change of the 0.2 single is not caught by its host contract (the paired runs catch it).

## Imports closed

- `Clear__12VICollBuffer`
- `Init__9VIStationiPCcPcT3T2T2i`
- `__10HolyStrikesiG6Point3P10GameObjectP5Skill`
- `__12GlowingShaftG6Point3iiii`
- `__13HammerOfWrathG6Point3sP10GameObjectiP5Skill`
- `__18DarknessProjectileG6Point3T1P5SkilliP10GameObject`
- `__19BlindingLightEffectG6Point3iP8CreatureP5Skill`
- `__4PropG6Point3P12_modelHeaderP8_textureli`
- `__8HolyBoltG6Point3iiP8CreatureP5Skill`
- `__8LostSoulG6Point3T1iP10_vagHeaderii`
- `__8SkelightG6Point3sPPcci`
- `clone__13FireBombletJr`
- `clone__15ShockProjectile`
- `fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii`
- `gameRadialDamage__FP10GameObjectG6Point3iffiii`
- `initType__8Skeletonb`
- `init__10StaticFire`
- `msg_collision__11DiseaseBoltP10GameObject`
- `msg_collision__12ShockOfFrostP10GameObject`
- `msg_collision__16PoisonProjectileP10GameObject`
- `msg_draw__11DiseaseBolt`
- `msg_draw__12ShockOfFrost`
- `msg_draw__13FireBombletJr`
- `msg_draw__14HateProjectile`
- `msg_draw__14poisonGasSpell`
- `msg_draw__15FrostStormSpell`
- `msg_draw__15ShockProjectile`

## Imports opened

Names that the 27 units reference, that no unit of the shared plan defines, and that no other plan unit referenced before (measured with the
partial build above, from the per-unit undefined-symbol lists):

- `Init__20VIWndGenericRenderer`
- `SetPassword__20VIWndGenericRendererPCc`
- `SetStationName__20VIWndGenericRendererPCc`
- `SetText__9VIWndEulaPCUsi`
- `__18SpellBlindingLightiP8CreatureT2P5Skill`
- `_vt$10HolyStrike`
- `_vt$12GlowingShaft`
- `_vt$13HammerOfWrath`
- `_vt$18DarknessProjectile`
- `_vt$19BlindingLightEffect`
- `_vt$8HolyBolt`
- `_vt$8LostSoul`
- `_vt$8Skelight`
- `initWeapon__8Skelight`
- `lightGetAmbient__FPs`
- `skelightResourceMap`

The first are callees (`Init__20VIWndGenericRenderer`, `SetPassword__20VIWndGenericRendererPCc`, `SetStationName__20VIWndGenericRendererPCc`,
`SetText__9VIWndEulaPCUsi`, `__18SpellBlindingLightiP8CreatureT2P5Skill`, `initWeapon__8Skelight`, `lightGetAmbient__FPs`); the `_vt$...` names are the class
tables the constructors store (`HolyStrike`, `GlowingShaft`, `HammerOfWrath`, `DarknessProjectile`, `BlindingLightEffect`, `HolyBolt`, `LostSoul`,
`Skelight`) and `skelightResourceMap` is the data table the Skelight constructor passes. Names the units also use but that other plan units already
imported (so they stay unresolved without being new): `frameNm`, `worldDifficulty`, `wrathHammerCount`, `skills`, `haloTexture5`,
`P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `lumpFindResource__FPcT0`, `lightResetOptionalParams__Fv`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `memset`,
`snprintf16__8VIStringPUsiPCce`, `strlen16__8VIStringPCUs`, `_vt$4Prop`, `_vt$10GameObject`, `_vt$13FireBombletJr`, and the other callees named in the unit comments.

## Build-plan entries needed

Append these 27 entries (JSON, one per line, without `input_order`; the integrator assigns it) to `experimental/build-plan.json` and the matching
snapshot rows. `docs/agents/w14-projectile-a-plan-entries.py --toolchain <ee> [--private-root DIR]` regenerates them and can write a private plan/snapshot
copy. Header closures reach `sources/w14-projectile-a-common/access.hpp` and, for four units, headers of other packages
(`sources/w12-actor-spell-c-common/access.hpp` for the HateProjectile and ShockProjectile draws, `sources/w9-rtti-vtables-entities-c-bytes/*.hpp` for `initType__8Skeletonb` and
`init__10StaticFire`). Conflicts with existing sources (select one of each pair; none of the old ones is in the plan except the two w9 adapter units, which
define different symbol names): `w13-spell-msg-a-{msg-draw-11diseasebolt,msg-draw-12shockoffrost,msg-draw-14hateprojectile,msg-draw-15shockprojectile,msg-draw-14poisongasspell,msg-draw-15froststormspell,clone-15shockprojectile,msg-collision-11diseasebolt,msg-collision-12shockoffrost,msg-collision-16poisonprojectile}`,
`w12-actor-spell-c-fx-run-ball-lightning`, `w9-frontend-a-vi-station-init`.

```
{"emitted_functions": [{"compiled_body_bytes": 344, "name": "clone__13FireBombletJr"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_clone_firebombletjr.o", "profile": "authored_cpp", "sha256": "34ba99825c7a17e170640494172d40e59052629225f2457e00f3b5c69f854a60", "source": "sources/w14-projectile-a-clone-firebombletjr/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "clone__15ShockProjectile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_clone_shockprojectile.o", "profile": "authored_cpp", "sha256": "27c24e8dc5fc8321e16c2e8876b8404c9af01e97839232a5a26380925705fc20", "source": "sources/w14-projectile-a-clone-shockprojectile/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 832, "name": "__19BlindingLightEffectG6Point3iP8CreatureP5Skill"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_blindinglighteffect.o", "profile": "authored_cpp", "sha256": "62d69386a1b3a605c84afce23bbca7faeaa1f2d5ed31667b0c7a8eb5855d24cc", "source": "sources/w14-projectile-a-ctor-blindinglighteffect/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 472, "name": "__18DarknessProjectileG6Point3T1P5SkilliP10GameObject"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_darknessprojectile.o", "profile": "authored_cpp", "sha256": "e1cdc055568ccbd7e5abf7c42979a84ee3524c42c686757021aaf17178bebb28", "source": "sources/w14-projectile-a-ctor-darknessprojectile/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "__12GlowingShaftG6Point3iiii"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_glowingshaft.o", "profile": "authored_cpp", "sha256": "a58b896e10acee0d408eb38782e4019231383fb88bf33a639901ec8aeb031193", "source": "sources/w14-projectile-a-ctor-glowingshaft/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 460, "name": "__13HammerOfWrathG6Point3sP10GameObjectiP5Skill"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_hammerofwrath.o", "profile": "authored_cpp", "sha256": "61523e38437727461bcaada745509db5aedb780113d82fddd2b97441396c24d1", "source": "sources/w14-projectile-a-ctor-hammerofwrath/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1192, "name": "__8HolyBoltG6Point3iiP8CreatureP5Skill"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_holybolt.o", "profile": "authored_cpp", "sha256": "64198eaad7ce580c150450287cd724e13386f033b5b749461a3e92a19e3dd0ff", "source": "sources/w14-projectile-a-ctor-holybolt/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 412, "name": "__10HolyStrikesiG6Point3P10GameObjectP5Skill"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_holystrike.o", "profile": "authored_cpp", "sha256": "dc2822da3ce8571301b82265b2f570ba9a430e0ffa029bd5b29b40e5b976d880", "source": "sources/w14-projectile-a-ctor-holystrike/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 476, "name": "__8LostSoulG6Point3T1iP10_vagHeaderii"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_lostsoul.o", "profile": "authored_cpp", "sha256": "6fd6bd19a683a99ed3008a56948cc91719c0e2e95b0da2ff12bb037b42065d78", "source": "sources/w14-projectile-a-ctor-lostsoul/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 448, "name": "__4PropG6Point3P12_modelHeaderP8_textureli"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_prop_model.o", "profile": "authored_cpp", "sha256": "4f53b3363c2a2cf7de6fee874a3f7115e2dafb3906dec7759b6a88874c318479", "source": "sources/w14-projectile-a-ctor-prop-model/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 652, "name": "__8SkelightG6Point3sPPcci"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_ctor_skelight.o", "profile": "authored_cpp", "sha256": "e277e6448f2f0bb3d6a6525dfbb8f3f39434992a820db1d3ab8a370490dc15a1", "source": "sources/w14-projectile-a-ctor-skelight/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_fx_run_ball_lightning.o", "profile": "authored_cpp", "sha256": "7204e67a36d325f27958d108fa8795e3bc881eb1628c123b31bd50bad0bb639d", "source": "sources/w14-projectile-a-fx-run-ball-lightning/fx_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 572, "name": "gameRadialDamage__FP10GameObjectG6Point3iffiii"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_game_radial_damage.o", "profile": "authored_cpp", "sha256": "1ce4904ba212d47d912e4af83677c7a01c5afdf3a5c351333c87331b5ea02882", "source": "sources/w14-projectile-a-game-radial-damage/damage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 640, "name": "init__10StaticFire"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_init_static_fire.o", "profile": "authored_cpp", "sha256": "7939f9847b0ee231b0465b9552f1bab954b85d0843aa662e475a1bc2ad63175e", "source": "sources/w14-projectile-a-init-static-fire/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1488, "name": "initType__8Skeletonb"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp", "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_init_type_skeleton.o", "profile": "authored_cpp", "sha256": "63662c5b4fafaea0a8920e04a2767daa8bf9dbd73285f1a1bd4f4cf2124de6ae", "source": "sources/w14-projectile-a-init-type-skeleton/init_type.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1192, "name": "Init__9VIStationiPCcPcT3T2T2i"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_init_vi_station.o", "profile": "authored_cpp", "sha256": "a5e005adcc712805668145c30d3abc2a0ecf2b2c4d91745fc8dc8c02fb55cdb4", "source": "sources/w14-projectile-a-init-vi-station/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 480, "name": "msg_collision__11DiseaseBoltP10GameObject"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_collision_diseasebolt.o", "profile": "authored_cpp", "sha256": "e44ae7efe59a0bbebe5df9b138e7372a7129083ab6c3fd7de59ab9bcf77d7b0a", "source": "sources/w14-projectile-a-msg-collision-diseasebolt/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 300, "name": "msg_collision__16PoisonProjectileP10GameObject"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_collision_poisonprojectile.o", "profile": "authored_cpp", "sha256": "61c98f7f77d51ec0f97bcf055c6a7127aa0209d8af52b6bc65e0cf36cb25434f", "source": "sources/w14-projectile-a-msg-collision-poisonprojectile/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 524, "name": "msg_collision__12ShockOfFrostP10GameObject"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_collision_shockoffrost.o", "profile": "authored_cpp", "sha256": "d80a74a881f35a2fd602ff11eb16a50edb8b991eccdc8387e2d4eb4a6f027d1c", "source": "sources/w14-projectile-a-msg-collision-shockoffrost/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 256, "name": "msg_draw__11DiseaseBolt"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_diseasebolt.o", "profile": "authored_cpp", "sha256": "9a3ac75eaa5313564b9f11d9d193393c9f2dba16ff217a0e81e76f474c372735", "source": "sources/w14-projectile-a-msg-draw-diseasebolt/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "msg_draw__13FireBombletJr"}], "header_closure": ["sources/w14-projectile-a-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_firebombletjr.o", "profile": "authored_cpp", "sha256": "85abc384c46bf106f8221fee40f90a3d7832498c75e16b43bf9212a2f2eed5f0", "source": "sources/w14-projectile-a-msg-draw-firebombletjr/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__15FrostStormSpell"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_froststormspell.o", "profile": "authored_cpp", "sha256": "4764dc839d73eaa58f80a96823a788b1f8121abb15038f900606797ea15cd004", "source": "sources/w14-projectile-a-msg-draw-froststormspell/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 308, "name": "msg_draw__14HateProjectile"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_hateprojectile.o", "profile": "authored_cpp", "sha256": "59f891eceda6193ffed8e2412ce2bdf5dfaeddee7c06d604437a688a1197d9fb", "source": "sources/w14-projectile-a-msg-draw-hateprojectile/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__14poisonGasSpell"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_poisongasspell.o", "profile": "authored_cpp", "sha256": "ac2fa8b3ac213d9a6e22e69be0a53e0c51902236280272f614e2f4cda03c4bff", "source": "sources/w14-projectile-a-msg-draw-poisongasspell/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 272, "name": "msg_draw__12ShockOfFrost"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_shockoffrost.o", "profile": "authored_cpp", "sha256": "e7010e05d4e4abd2dfc84b86d067de9bc48682c206a81c55c969d61315a01aa9", "source": "sources/w14-projectile-a-msg-draw-shockoffrost/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 308, "name": "msg_draw__15ShockProjectile"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pa_msg_draw_shockprojectile.o", "profile": "authored_cpp", "sha256": "87cddba26086cb5917415cbafb66dc89453d00cb6ad1fdd8854a31530d67fba2", "source": "sources/w14-projectile-a-msg-draw-shockprojectile/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "Clear__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pa_vi_coll_buffer_clear.o", "profile": "authored_cpp", "sha256": "fe4bcb217a0e75ff4878445556822bf4ec03276831c9b3cf50d52b0914ddfe21", "source": "sources/w14-projectile-a-vi-coll-buffer-clear/clear.cpp"}
```

## Authored but never executed

All 27 units: nothing was run on an EE or inside a build of the game. They were executed only in the private interpreter, against the original function, in
synthetic memory with stubbed callees, and in host contracts with stubbed callees. Specifically never executed with real callees: every constructor that
the units call (GameObject, Projectile, Creature, LightEffect, SpellBlindingLight, the GlowingShaft and trail functions), `objectFindInBox`, the table
(virtual) calls at byte offsets 88, 112, 160 and 216, `gameAttackResolver`, `item::create`, `modelDraw` and the matrix routines, `memAlloc`, and
everything `Init__9VIStation...` calls (raster, collision, dictionary, sound, scene, UI, window and fader code). The `.lit4` constants were taken from the image
and never produced by the target's own startup.

## Unknown

- What most object fields mean (for example `+84..+104` of the HolyStrike, `+180..+196` of the HammerOfWrath, `+88..+112` of the HolyBolt, `+960` and
  `+208` of the Skelight, the 80-byte light records at `+88`, `+100` and `+116`); the units name them by offset only.
- What the integer arguments mean beyond the stored form (`A`, `B`, `N` of the spell constructors; the Repulse Undead cast passes the ring angle, 1 and 0
  to the LostSoul), and what the table entries at bytes 88, 112, 160 and 216 compute (160 is used as an enemy test, 88 as a rank-dependent amount; names
  are not claimed).
- The meaning of the many singles and integers (350.0, 4000.0, 450.0, 0.2, 168.0, the trail numbers 750, 107, 100, 234, 171, the 0x421b, 0x4001 and 0x40001
  flag masks), of the byte `+172 == 16` type tag, of the `wrathHammerCount` global and of the 64-bit word at `+160` of objects.
- The member layout of `VICollBuffer` and `VIStation`, the meaning of the words at `0xb468`, `0xb794` and `0x17a30` of the station, and what `initWeapon__8Skelight`,
  `SpellBlindingLight` and the `LightEffect` constructor do (they are imports).
- Whether `initType__8Skeletonb` takes a real `bool` (the callers only pass 0 and 1; the units declare an `int`).
