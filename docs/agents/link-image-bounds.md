# Link image bounds

Package work is on `agents/link-image-bounds`. New source is under `experimental/sources/link-image-bounds-*`. Host checks sit beside that source and stay out of the EE plan. Function units use profile `authored_cpp`. Original absolute image labels are evidence only, not partial-link definitions. Addresses were read from the pinned SLUS-20565 symbol table. Fresh EE objects are not original-byte matches. No EE startup stage has run.

`__sce_sema_id` is not aliased to `norrath_player_malloc_semaphore_view`. Callers `_start`, `InitThread`, `__do_global_ctors`, `__do_global_dtors`, `supplement_crt0`, and `engineInit` stay imports.

## __register_frame_info

Observed at `0x002fc428`, 104 bytes. The caller passes the frame pointer and a six-word object. This body stores the frame pointer at word 2, clears words 0, 1, 3, and 4, then waits on `__sce_eh_sema_id`. After the wait it stores the previous `objects` head at word 5 and makes this object the head, then signals the semaphore. The id is loaded again after the wait. The compiled body is 92 bytes. The host contract fails if a word is left poisoned, if the link changes before the wait, or if the signal sees the old id or the old head.

`objects` is the local BSS word at `0x008eab18`. This unit defines it as a global four-byte pointer so the deregistrar can share it. The original binding is local. Its initial value is zero.

## atexit

Observed at `0x002ff8d8`, 160 bytes. It loads the object addressed by `_impure_ptr`. The word at offset 328 selects a 136-byte block; a zero word selects the block at offset 332 and stores that address. The block is a next address, a signed count, and 32 function addresses. Count 32 or more calls `__wrap_malloc(136)`. A null allocation returns -1 and leaves the previous block unchanged. Otherwise the new block's next word is the previous block, the function is stored at the current count, and the count increases by one. Success returns 0. The compiled body is 372 bytes. The host contract fails if the 32nd registration allocates, if the 33rd does not allocate 136 bytes, if the new block is not linked in front, or if allocation failure still returns 0.

## __deregister_frame_info

Observed at `0x002fc558`, 188 bytes. It waits on `__sce_eh_sema_id` and walks `objects`. A match is the node whose word 2 is the caller's frame pointer. That node is unlinked. Word 3 is passed to `__wrap_free` only when it is nonzero and is not the same pointer. The semaphore is signaled and the node is returned. No match signals the semaphore and calls `abort`. The compiled body is 160 bytes. The host contract fails if the middle node stays linked, if a null or identical word 3 is freed, if the signal uses the id from before the wait, or if a missing node skips `abort`.

## Takeover review: constructor and destructor tables

Observed against the freshly SHA-256-checked baseline: `.ctors` at
`0x0031f250` holds 103 words (all-ones sentinel, 101 callbacks, zero), and
`.dtors` at `0x0031f3ec` holds 13 words (sentinel, 11 callbacks, zero).
Every callback initializer names the retained symbol at that original slot.
The authored tables contain relocations to those names, not original addresses.
The callbacks remain imports; no callback implementation is supplied here.
The EE compiler emitted both data-only objects, with no initializer functions.
Both host contracts checked every slot and both sentinels. A private reference
comparison independently checked all 112 callback names against original slots.

## Semaphore storage

Observed `__sce_sema_id` at `0x00444a30` and `__sce_eh_sema_id` at
`0x00444a34`: separate four-byte `.data` objects, each initially zero.
The authored definitions are separate zero-initialized words (emitted in BSS).
The host contract checks initial values, distinct addresses, and independent
mutation. The reviewed EE compilation passed. The final image must initialize
this BSS before use. `norrath_player_malloc_semaphore_view` remains separate,
as required by the integration decision; this package adds no alias.

## Verification and review outcome

All three pre-existing function commits were retained. Their three host contracts
were rerun successfully, and fresh reviewed EE compilations reproduced the noted
92/372/160-byte function sizes. Constructor, destructor and semaphore host
contracts also passed (six host executables total). The reference check is
reproducible without publishing original data:

```sh
python3 docs/agents/link-image-bounds-check.py --original /path/to/private/SLUS_205.65
```

Compile each `host_contract.cpp`, `register_host.cpp`, and `deregister_host.cpp`
with `clang++ -std=c++17 -O2`, then execute it. These host files include their
corresponding source; do not also link a second copy or include them in the EE plan.
The two table objects have sizes 412 and 52 bytes; each semaphore word has size 4.

A fresh full source build used a private copied source root, snapshot and plan
under ignored `work/link-image-bounds-review/source-root`. The unchanged public
driver and reviewed EE toolchain compiled all 851 authored inputs and linked
once into `work/link-image-bounds-complete`: 726 function bodies, zero COMMON,
986 unresolved imports. Against the retained successful 845-input/879-import
baseline, seven imports closed and 114 opened. The increase exposes real callback
dependencies. No original input, fixed-address import, alias or stub entered that
build. `python3 -m unittest discover -s tools -p 'test_*.py'` passed all 214 tests.
Public shared plans, snapshots, receipts, startup sources and tools were unchanged.

The uncommitted exception-frame byte array was rejected and removed from source.
It copied the original `.eh_frame` payload, including original PC addresses and
unwind rules. Those rules do not describe the freshly compiled bodies. It remains
only in ignored local review material and has never been committed. The other
three uncommitted directories were reviewed, checked and committed.

`git fetch origin` failed because of the unrelated malformed local ref
`refs/heads/agents/iop-module-boot 2`; no shared ref was repaired or deleted.
Explicit pushes of this package branch succeeded after each new commit.

## Imports closed

`__register_frame_info`, `atexit`, `__deregister_frame_info`, `__CTOR_LIST__`,
`__DTOR_LIST__`, `__sce_sema_id`, `__sce_eh_sema_id` (seven).

## Imports you opened

The measured delta opens `__wrap_free`, `abort`, 101 constructor callbacks and
11 destructor callbacks. All exact newly opened names follow. `objects` is defined
by the registrar. `WaitSema` and `__wrap_malloc` were already supplied;
`SignalSema` and `_impure_ptr` remain pre-existing imports. The callback callers
and implementations remain outside this package.

```text
_GLOBAL_$D$displayQuestInfo__FP11questStructi
_GLOBAL_$D$inventoryLoad__Fv
_GLOBAL_$D$lastkey
_GLOBAL_$D$objectIgnoreFlags
_GLOBAL_$D$optionMenu
_GLOBAL_$D$orc1ResourceMap
_GLOBAL_$D$shopActiveFrame
_GLOBAL_$D$soundSfxVolume
_GLOBAL_$D$wesResourceMap
_GLOBAL_$D$worldTileEntityLightScale
_GLOBAL_$D$wraithResourceMap
_GLOBAL_$I$_3map$mapOffset
_GLOBAL_$I$__10HolyStrikesiG6Point3P10GameObjectP5Skill
_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader
_GLOBAL_$I$__10StaticFireG6Point3fif
_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill
_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill
_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill
_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill
_GLOBAL_$I$__15SpellConeOfFireiP8CreatureP5Skill
_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill
_GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill
_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill
_GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill
_GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill
_GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill
_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill
_GLOBAL_$I$__19BlindingLightEffectG6Point3iP8CreatureP5Skill
_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill
_GLOBAL_$I$__22SpellPlanarSteelEffectiP8CreatureP5Skill
_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc
_GLOBAL_$I$__5TorchG6Point3PPci
_GLOBAL_$I$__6SwitchG6Point3PPc
_GLOBAL_$I$__7VehicleG6Point3sPPci
_GLOBAL_$I$__8FireBombG6Point3sPPc
_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill
_GLOBAL_$I$__8JumpGateG6Point3sPPc
_GLOBAL_$I$_fontStringSize__FP4FontPci
_GLOBAL_$I$addpmlist__FPc
_GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState
_GLOBAL_$I$antResourceMap
_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2
_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature
_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s
_GLOBAL_$I$cast__13SkillEntangleiP8CreatureG6Point3s
_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s
_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s
_GLOBAL_$I$cast__18SkillRepulseUndeadiP8CreatureG6Point3s
_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s
_GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s
_GLOBAL_$I$cloudgiantResourceMap
_GLOBAL_$I$cutMode
_GLOBAL_$I$cyclopsResourceMap
_GLOBAL_$I$displayQuestInfo__FP11questStructi
_GLOBAL_$I$edgearray
_GLOBAL_$I$engineLastAITime
_GLOBAL_$I$fireBeetleResourceMap
_GLOBAL_$I$fxResourceMap
_GLOBAL_$I$getCriticalChance__16SkillCriticalHitP6Playeri
_GLOBAL_$I$getRankDescription__10SkillShockPUsi
_GLOBAL_$I$getRankDescription__11SkillChargePUsi
_GLOBAL_$I$getRankDescription__12SkillHealingPUsi
_GLOBAL_$I$getRankDescription__12SkillRipostePUsi
_GLOBAL_$I$getRankDescription__15SkillHolyShieldPUsi
_GLOBAL_$I$getRankDescription__15SkillShieldBashPUsi
_GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi
_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi
_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi
_GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi
_GLOBAL_$I$getRankDescription__9SkillBashPUsi
_GLOBAL_$I$goblinResourceMap
_GLOBAL_$I$hasLightOrParticleNumber__FPPci
_GLOBAL_$I$hudNoDraw
_GLOBAL_$I$innoruukResourceMap
_GLOBAL_$I$inventoryLoad__Fv
_GLOBAL_$I$lastkey
_GLOBAL_$I$lavaMonsterResourceMap
_GLOBAL_$I$mcDisableStartupScript
_GLOBAL_$I$modelClippingOn
_GLOBAL_$I$move__10ProjectileG6Point3
_GLOBAL_$I$msg_draw__10FrostShard
_GLOBAL_$I$msg_run__12DiseaseTrail
_GLOBAL_$I$msg_run__13DiseaseCorpse
_GLOBAL_$I$msg_run__14FireStormFlare
_GLOBAL_$I$msg_run__22ExplodeArrowProjectile
_GLOBAL_$I$msg_run__7AntTrap
_GLOBAL_$I$msg_run__9FireFlare
_GLOBAL_$I$mummyResourceMap
_GLOBAL_$I$netInterfaceIsUp
_GLOBAL_$I$nmPlayers
_GLOBAL_$I$noAmbientOffset
_GLOBAL_$I$numInRegion__8creatureG6Point3f
_GLOBAL_$I$objectIgnoreFlags
_GLOBAL_$I$optionMenu
_GLOBAL_$I$orc1ResourceMap
_GLOBAL_$I$particlesPaused
_GLOBAL_$I$playerSelectSuccessful
_GLOBAL_$I$playerStartPos
_GLOBAL_$I$route_generation
_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s
_GLOBAL_$I$scriptParticles
_GLOBAL_$I$shopActiveFrame
_GLOBAL_$I$skills
_GLOBAL_$I$soulResourceMap
_GLOBAL_$I$soundSfxVolume
_GLOBAL_$I$spiderResourceMap
_GLOBAL_$I$udkResourceMap
_GLOBAL_$I$vampireResourceMap
_GLOBAL_$I$wesResourceMap
_GLOBAL_$I$worldTileEntityLightScale
_GLOBAL_$I$wraithResourceMap
_GLOBAL_$I$wrathHammerCount
__wrap_free
abort
```

## Build-plan entries needed

Append these exact six inputs; assign fresh contiguous `input_order` values at
integration (847–852 below are the tested private orders). Do not select the
host contracts or reference checker. Mirror the source hashes in snapshot
`files`, add matching snapshot inputs, and regenerate the plan snapshot hash.
The emitted-function lists are registrar `__register_frame_info`/92 bytes,
`atexit`/372 bytes, deregistrar `__deregister_frame_info`/160 bytes; the three
data inputs have empty emitted-function lists. Use the unchanged `authored_cpp`
profile. The shared header hash is
`c00ee8e6b6fcafae2ba56a064337c133bc081bc557f98d68bce0514bcf3a629d`.

```json
[
  {
    "input_order": 847,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-frame/register_frame_info.cpp",
    "object_name": "register_frame_info.o",
    "profile": "authored_cpp",
    "sha256": "d411827e78da3b50b47cbf60718440bb7f124a9cf4590590527ad8ecd6c704b6",
    "header_closure": [
      "sources/link-image-bounds-frame/frame_object.hpp"
    ]
  },
  {
    "input_order": 848,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-atexit/atexit.cpp",
    "object_name": "atexit.o",
    "profile": "authored_cpp",
    "sha256": "a10d689de659f2241cfba16e03e4ac374d4c3babba0c36a90c21aa6d17064d41",
    "header_closure": []
  },
  {
    "input_order": 849,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-frame/deregister_frame_info.cpp",
    "object_name": "deregister_frame_info.o",
    "profile": "authored_cpp",
    "sha256": "0066efd85942a95f2d21a23a974c5b3aaea561d4c65f248a0c239fa8cac96b8a",
    "header_closure": [
      "sources/link-image-bounds-frame/frame_object.hpp"
    ]
  },
  {
    "input_order": 850,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-ctors/ctor_list.cpp",
    "object_name": "ctor_list.o",
    "profile": "authored_cpp",
    "sha256": "064447ff2d9a721b80212cd01b4994e1c4c917f1bc76f2555075e4840e1ff27b",
    "header_closure": []
  },
  {
    "input_order": 851,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-dtors/dtor_list.cpp",
    "object_name": "dtor_list.o",
    "profile": "authored_cpp",
    "sha256": "c15c120e3cf9d8b46306b5c1afccd94a2d893cff51f0955339dd7668d2168160",
    "header_closure": []
  },
  {
    "input_order": 852,
    "kind": "independently_authored_source",
    "source": "sources/link-image-bounds-sema/sema_ids.cpp",
    "object_name": "sema_ids.o",
    "profile": "authored_cpp",
    "sha256": "8403e4ea29efbb5cfe8091316d8075d3ee20851baf417f5e1cecc9229f273f04",
    "header_closure": []
  }
]
```

## What is authored but never executed

All seven supplied symbols have only host/model and compile/link validation.
The three functions have not executed on EE hardware or an emulator. No actual
constructor/destructor callback, exception unwind, kernel semaphore operation,
startup stage or game flow has run through these definitions. The callback table
references are authored data, not implemented callback bodies. No exact-match
ledger credit or gameplay compatibility is claimed.

## What is unknown

Six requested symbols remain unresolved; none is silently supplied by an address
alias, undersized placeholder or original binary payload:

- `_fbss`, `_bssend`, `_end`, `_gp`: the pinned values are respectively
  `0x00486800`, `0x008eab1c`, `0x008ec91c`, `0x004879f0`. Observed `_fbss` begins
  `.sbss`; `_bssend` follows `.bss`/empty `.vubss`, before the SRS callback tables;
  `_end` follows those tables and COMMON; `_gp` is an absolute linker value.
  Inference: these are final-image layout responsibilities, not ordinary C++
  storage objects. The package cannot pick their rebuilt values before the lead
  supplies the final section layout and small-data/GP policy. `_bssend` must not
  be equated with `_end`: entry deliberately excludes SRS tables and COMMON from
  its clear. Original `_gp` equals the `.lit4` start plus `0x7ff0`; that observation
  is not a reviewed rebuilt-image policy. The current portable recipe explicitly
  prohibits absolute address imports and executable linking.
- `_args`: observed local zero-size NOTYPE symbol at `0x005192c0` in BSS. The next
  named object is `dataBlock` at `0x00519440`, leaving a 384-byte interval. That
  interval is an upper bound, not a verified allocation size (padding and unnamed
  objects are not ruled out). Entry passes its address to kernel syscall 60, then
  reads argc at offset 0 and supplies argv at offset 4. Required kernel write
  capacity, pointer/string storage layout and alignment remain unverified. The
  owner needs a supported syscall argument-buffer contract or original allocation
  evidence before defining its storage. No 4-byte or guessed 384-byte substitute
  was admitted.
- `__EH_FRAME_BEGIN__`: observed first byte of the 0x974-byte `.eh_frame` at
  `0x00453970`. The owner must select a compatible unwind strategy and generate
  metadata for authored code before a final linker start symbol can be validly
  supplied. The current `-fno-exceptions -fno-unwind-tables` source profile does
  not supply those records. Neither an empty sentinel nor the rejected original
  frame array is evidence of equivalent exception behavior.

Other open questions: semaphore service semantics; frame words 0, 1 and 4;
final linkage visibility of the originally local `objects` head; reentrancy bytes
outside atexit's observed offsets; invalid negative counts; `abort` behavior;
and whether the integrator will eventually unify the distinct malloc semaphore
view with `__sce_sema_id`. No such unification is made here. The symbol and retail
revision uncertainties of the pinned baseline remain unchanged.
