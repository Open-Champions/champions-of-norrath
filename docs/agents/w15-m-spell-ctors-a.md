# w15-m-spell-ctors-a: spell and projectile constructors and destructors

Branch `agents/w15-m-spell-ctors-a`. Each function is its own commit under
`experimental/sources/w15-m-spell-ctors-a-*`. Each source begins with a comment giving the observed address and
size, what the function does, what is inferred, and that nothing ran on the EE. No bytes, disassembly or image
data are committed. The private paired-run harness is `priv-w15-m-spell-ctors-a/` outside git.

Private reference executable: `work/private/SLUS_205.65` (ignored by git). Addresses and sizes are that file's
symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; it is not a byte match with the image.

## Units

| source directory | symbol | image address and size | status |
|---|---|---|---|
| `w15-m-spell-ctors-a-ctor-9spellroot` | `__9SpellRootiP8CreatureP5Skill` | `0x002ad058`, 164 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-13firebomblet` | `_$_13FireBombletJr` | `0x003121b8`, 100 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-14firestormflare` | `_$_14FireStormFlare` | `0x003179c0`, 100 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-18hammerofwrath` | `_$_18SpellHammerOfWrath` | `0x00319768`, 116 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-19summonskeleton` | `_$_19SpellSummonSkeleton` | `0x003188d8`, 116 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-ctor-blinding-light` | `__18SpellBlindingLightiP8CreatureT2P5Skill` | `0x002ba028`, 184 bytes | authored, paired-checked |

## Checks performed

- **EE compile.** Each unit compiles with `work/toolchains/ee` (GCC 15.2.0) and the `authored_cpp` flags with `-c`
  (`priv-w15-m-spell-ctors-a/cc.sh`). `nm -g` on each object shows exactly the expected definition and undefined
  references (listed per unit below).
- **Host contracts.** Each unit's `host_check.cpp` builds with `clang++ -std=c++17 -Wall -Wextra` and prints `ok`.
  Host contracts read pointer-width fields at host width and check stored fields, the call order and arguments.
- **Paired R5900 comparison.** The private interpreter runs the original image function and the authored function
  from identical memory snapshots, with stub callees and the image's own vtable and global addresses. Each unit
  reports 0 mismatches and no `HALT` line. Each unit was also mutated (one defect at a time) and the comparison
  caught every mutation.
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK (run on this branch after
  the sixth unit).

- **`__9SpellRootiP8CreatureP5Skill`.** Same shape as the cleric-root and entangle constructors, with the class
  vtable `_vt$9SpellRoot`. The EE object defines the symbol; undefined references are `P_CreateParticleSystem__Fv`,
  `_vt$9SpellRoot` and `syncRandState`. Paired comparison: 210 scenarios (7 kinds x 5 adjustments x 6 seeds),
  0 mismatches. A mutation that reads the kind byte unsigned gives 90 mismatches.

- **`_$_13FireBombletJr`.** Stores `_vt$13FireBombletJr` at +72, calls the GameObject base destructor
  `_$_10GameObject` with flags 0, then `blockFree__FPv` when bit 0 is set. Undefined references: `_$_10GameObject`,
  `_vt$13FireBombletJr`, `blockFree__FPv`. Paired comparison: 24 scenarios (8 flag values x 3 fills), 0
  mismatches. Mutation (bit 1 tested instead of bit 0) gives 12 mismatches.

- **`_$_14FireStormFlare`.** The same shape as the fire-bomblet destructor, with `_vt$14FireStormFlare`. Paired
  comparison 24 scenarios, 0 mismatches. Mutation gives 12 mismatches.

- **`_$_18SpellHammerOfWrath`.** Stores `_vt$18SpellHammerOfWrath` at +20, calls `listDestroy__FP8ListHead` on +24,
  stores `_vt$5Spell` at +20, calls it on +4, then frees when bit 0 is set. Undefined references:
  `_vt$18SpellHammerOfWrath`, `_vt$5Spell`, `listDestroy__FP8ListHead`, `blockFree__FPv`. Paired comparison 24
  scenarios, 0 mismatches. Mutation gives 12 mismatches.

- **`_$_19SpellSummonSkeleton`.** The same shape as the hammer-of-wrath destructor, with `_vt$19SpellSummonSkeleton`.
  Paired comparison 24 scenarios, 0 mismatches. Mutation gives 12 mismatches.

- **`__18SpellBlindingLightiP8CreatureT2P5Skill`.** Same Spell base and random step as the root constructor. It
  stores the base table at +20, then the class table `_vt$18SpellBlindingLight`. The skill call receives
  `S + adjustment`, the constant 2, and the full int kind. After the call it stores `frameNm + 60 * result` at +24
  and the first creature at +28; the second creature is at +12. Undefined references: `_vt$18SpellBlindingLight`,
  `_vt$5Spell`, `frameNm`, `syncRandState`. Paired comparison: 210 scenarios, 0 mismatches, no HALT. The integer
  return register is excluded (`compare_ret=False`): the function is `void`, and the authored build leaves a
  different value in `v0`; logs and memory match in every scenario. Mutation (call argument 2 changed to 1) gives
  210 mismatches.

## Imports closed

Names from the brief's list that this branch now defines (take effect when the build-plan entries below are merged
and the partial build is repeated; not yet confirmed by a fresh build):

- `__9SpellRootiP8CreatureP5Skill`
- `_$_13FireBombletJr`
- `_$_14FireStormFlare`
- `_$_18SpellHammerOfWrath`
- `_$_19SpellSummonSkeleton`
- `__18SpellBlindingLightiP8CreatureT2P5Skill`

Six of the 26 names. The reference list used for this work is `work/plan15-build/unresolved-imports.json` (915
entries) from the private repo; it predates the current plan.

## Imports opened

Names these units reference that are not defined by this package (the owners are the packages named in the brief,
or the shared Spell and GameObject base code). All appear as undefined in the object files:

- `P_CreateParticleSystem__Fv` (particle package)
- `syncRandState` (sync random state; shared with the other spell constructors)
- `frameNm` (frame counter global, also used by the hate-effect constructor)
- `_$_10GameObject` (GameObject base destructor, gameobject-base package)
- `blockFree__FPv`, `listDestroy__FP8ListHead` (memory and list helpers)
- `_vt$5Spell` (Spell base vtable; the base destructor unit `_$_5Spell` is in the w14 package)
- class vtables: `_vt$9SpellRoot`, `_vt$13FireBombletJr`, `_vt$14FireStormFlare`, `_vt$18SpellHammerOfWrath`,
  `_vt$19SpellSummonSkeleton`, `_vt$18SpellBlindingLight`

Weak binding: the four `_$_` destructors and `_$_` names are weak in the private image. The authored objects define
them as strong global symbols. If another package also defines one of these names, the link will report it;
check with `--warn-common` and the partial link before merging.

## Build-plan entries needed

These six entries belong in `experimental/build-plan.json` `inputs`, in the order the integrator assigns (`input_order`
is left for the integrator). They were generated from the sources on this branch with the reviewed `authored_cpp`
flags, and `compiled_body_bytes` is the size of each unit's function in the compiled object. Nothing in shared files
was edited here.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_ctor_9spellroot.o",
  "profile": "authored_cpp",
  "sha256": "be7018f401d1003d8bb9ede7a24ccfe5ca6c1874a7d04cc2c15f88003c8a2509",
  "source": "sources/w15-m-spell-ctors-a-ctor-9spellroot/spell_root_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__9SpellRootiP8CreatureP5Skill",
    "compiled_body_bytes": 172
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_dtor_13firebomblet.o",
  "profile": "authored_cpp",
  "sha256": "66e0a5af9b227531b3b7d46f4c5707dd9fa38ff67b23fabd2b76269544b79922",
  "source": "sources/w15-m-spell-ctors-a-dtor-13firebomblet/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13FireBombletJr",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_dtor_14firestormflare.o",
  "profile": "authored_cpp",
  "sha256": "ff910b862fd06b23b38e469f19ed8be0fd011acde21e6b376054eb6d88d5b2d0",
  "source": "sources/w15-m-spell-ctors-a-dtor-14firestormflare/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_14FireStormFlare",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_dtor_18hammerofwrath.o",
  "profile": "authored_cpp",
  "sha256": "7a053f106cb9368854a23076d6d327345f8cd59c8c08a753d5872168dd19db73",
  "source": "sources/w15-m-spell-ctors-a-dtor-18hammerofwrath/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_18SpellHammerOfWrath",
    "compiled_body_bytes": 116
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_dtor_19summonskeleton.o",
  "profile": "authored_cpp",
  "sha256": "e287829eae1aa12c34a96ca6594ada7f88851df0a064f8810c365c86803b155b",
  "source": "sources/w15-m-spell-ctors-a-dtor-19summonskeleton/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_19SpellSummonSkeleton",
    "compiled_body_bytes": 116
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15mspell_ctor_blinding_light.o",
  "profile": "authored_cpp",
  "sha256": "aabe847b5d4a04459e37d02a5a63b6379e3c5672589aa3d1531995fe282d71f6",
  "source": "sources/w15-m-spell-ctors-a-ctor-blinding-light/spell_blinding_light_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__18SpellBlindingLightiP8CreatureT2P5Skill",
    "compiled_body_bytes": 192
   }
  ]
 }
]
```

## Authored but never executed

All six units were executed only by the private R5900 interpreter, against stub callees, with the image's vtable and
global addresses. Nothing ran on the EE, in the game, or in the partial link. The skill's secondary table and call
target were stubbed; the interpreter did not use real game objects, creatures, skills, or the `frameNm` value of a
running frame.

## Open names and why

These names from the brief's list are not closed. Each was looked at, but none has an authored unit yet:

- `__11SpellChargeiP8CreatureP5Skill` (0x00290e78, 632 bytes). Float trail loop calling `P_StartTrail`,
  `P_ExtendTrail`, light, sound, resource lookup and `icos`/`isin`. Not attempted: too many external effects for a
  paired run in this sitting.
- `__9SpellBashiP8CreatureP5Skill` (0x002a52c0, 384 bytes). Resource lookup, one-shot animation, sorted creature list
  walk and `listDestroy`. Not attempted.
- `_$_15SpellConeOfFire` (0x002b5a00, 216 bytes). Light deactivation, sound stop and play, a light destructor, and a
  conditional free. Not attempted.
- The remaining ctor names in the brief's list (`__12SpellBlessed`, `__12SpellCyclone`, `__15SpellFireFlares`,
  `__15SpellShieldBash`, `__15SpellWizardBeam`, `__16SpellColdWeapons`, `__16SpellFireWeapons`,
  `__17SpellConvertEnemy`, `__17SpellDiseaseTrail`, `__17SpellUndeadShield`, `__18SpellAncestralCall`,
  `__18SpellConvertUndead`, `__21SpellLightningWeapons`, `__22SpellRangerCriticalHit`, `__5Mummy`, `__9LooseFire`)
  have not been read. `__5Mummy` (36 references) and `__9LooseFire` also have authored sources in other packages'
  directories (`ctors-lava-wraith-mummy`, `w13-spell-msg-a`, `w14-projectile-b`); those packages may already close
  them. Check before authoring a duplicate.

## Unknown

- The class behind each skill's secondary table at +96 and the call target at +92 (shared by the cleric-root,
  entangle, root and blinding-light constructors).
- The owner and layout of `frameNm` beyond the observed `frameNm + 60 * result` store.
- The element type of the list at +24 in the two list-based destructors, and why the base-table store falls between
  the two list calls.
- The meaning of the stored fields (+12 and +28 creatures, +24 frame value) beyond the observed stores.
- The owner of `_$_10GameObject` beyond the reference name (gameobject-base package).

## Build state

A fresh partial build with `tools/build_partial.py` (reviewed toolchain, output `work/w15-m-spell-ctors-a-base`,
ignored by git) was started on the shared plan and was still running at the last check: 2,674 of 6,763 inputs.
No `unresolved-imports.json` exists yet, so the closure statements above come from the reference list in the private
repo, not from a fresh build. Once the build finishes, check that the six closed names are absent and that the
opened names appear.
