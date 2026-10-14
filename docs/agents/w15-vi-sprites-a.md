# w15-vi-sprites-a: hierarchical and character sprite functions

Branch `agents/w15-vi-sprites-a`. One commit per function; source under `experimental/sources/w15-vi-sprites-a-*`, each directory holding the authored
source and a `host_check.cpp` (host-only, not a plan input). Addresses and sizes are from the pinned private `SLUS_205.65` read locally with the reviewed EE
binutils; no listing or bytes are committed. Every source opens with an Observed / Inferred / Unknown comment. Nothing here ran on the EE or as gameplay;
compiled sizes come from the reviewed EE `authored_cpp` objects and are not claims of byte identity.

## How each function was checked

* Each unit compiles with the reviewed EE g++ and the `authored_cpp` flags (single-unit compiles, no runtime import beyond the named callees).
* Each unit has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0) with recording stubs. `ProcessPlayList`, the three pool copy-initialisers, `CreateParticleEmitters` and
  `ProcessWeaponTrails` use the `NORRATH_HOST_ARENA` address arena for 32-bit pointer words. The `SetAnimPriorities` host check takes its expected offsets from the same lists as
  the source, so it does not check them independently; the paired run does.
* Each function was compared with the image in a private paired run (an R5900 interpreter kept outside git): the original and the freshly compiled authored function run from
  identical memory snapshots with identical stubbed callees; call logs (arguments, plus the memory a callee sees where it matters), the result and every changed byte outside the stack
  must be equal, and a halted run counts as a failure. Scenario counts and the mutations are in the commit messages.
* Each function was mutated on purpose (offsets, strides, counts, comparison senses, swapped fields, dropped calls) and the mutants that can differ in behaviour fail the paired run.
  Equivalent mutants: `0.4f` for `0.39999998f` (the toolchain rounds `0.4f` down to the same word, so the source keeps the exact spelling); a `count <= 0` guard in `ProcessPlayList`
  was removed from the source because the loop makes it redundant. A next-link offset mutation in the play pool copy-initialiser makes the original chain walk non-terminating (detected by hang).
  A first version of the `ProcessPlayList` test returned early in every scenario (the time stamps matched); the mutants exposed this and the test was fixed before the commit.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
* A private object-level check: the 36 units compile, `ld -r` combines them without duplicate definitions, and the defined names equal the 36 imports below. No full partial build was run
  (the task asked for at most one at a time; none was needed to link-check this package).

## Units (36, one commit each)

| Function | Image address | Image bytes | Compiled bytes |
|---|---|---|---|
| `Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType` | 0x0105cf48 | 188 | 188 |
| `SetKeyframe__13VIHSpriteAnimiif` | 0x0105d208 | 28 | 28 |
| `SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame` | 0x0105d180 | 136 | 96 |
| `SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame` | 0x0105d100 | 128 | 120 |
| `SetNodeRefID__13VIHSpriteAnimii` | 0x0105d0e8 | 24 | 24 |
| `Init__t7VIArray1Z20VICSpritePartEmitteri` | 0x012082c8 | 92 | 72 |
| `Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger` | 0x012020b8 | 168 | 208 |
| `CalcLODOffDist__9VICSpriteR7VIScene` | 0x010b2f90 | 128 | 164 |
| `Clear__9VICSprite` | 0x010add20 | 472 | 336 |
| `CreateParticleEmitters__9VICSpriteR7VIScene` | 0x010b01b8 | 200 | 236 |
| `Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi` | 0x010b27c0 | 180 | 176 |
| `ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44` | 0x010afbe8 | 1484 | 1004 |
| `SetAnimPriorities__9VICSprite` | 0x010b20d0 | 488 | 96 |
| `SetAnimSoundChannels__9VICSprite` | 0x010b22b8 | 320 | 112 |
| `SetDefaultEntries__9VICSprite` | 0x010b1838 | 1576 | 656 |
| `SetDefaults__9VICSpriteR7VIScene` | 0x010b1690 | 424 | 424 |
| `Begin__Ct6VIPool1Z19VIHSpriteAttachment` | 0x01203738 | 44 | 32 |
| `Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment` | 0x01202190 | 316 | 412 |
| `End__Ct6VIPool1Z19VIHSpriteAttachment` | 0x012037a8 | 12 | 12 |
| `Next__Ct6VIPool1Z19VIHSpriteAttachmenti` | 0x01203768 | 36 | 48 |
| `Begin__Ct6VIPool1Z13VIHSpritePlay` | 0x01203808 | 44 | 32 |
| `Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay` | 0x01202328 | 372 | 416 |
| `End__Ct6VIPool1Z13VIHSpritePlay` | 0x01203838 | 12 | 12 |
| `Next__Ct6VIPool1Z13VIHSpritePlayi` | 0x01203848 | 52 | 52 |
| `Size__Ct6VIPool1Z13VIHSpritePlay` | 0x01203730 | 8 | 8 |
| `Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode` | 0x01202510 | 428 | 500 |
| `AddPlay__9VIHSpriteiR7VIScene` | 0x01063a78 | 84 | 68 |
| `CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44` | 0x010631e0 | 160 | 128 |
| `CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44` | 0x010644a8 | 120 | 112 |
| `Clear__9VIHSprite` | 0x0105f9f0 | 160 | 168 |
| `Init__9VIHSprite` | 0x01063480 | 44 | 32 |
| `ProcessPlayList__9VIHSpriteiR7VIScene` | 0x01061df0 | 2220 | 1560 |
| `ProcessUpHierarchy__9VIHSpritei` | 0x01062d58 | 392 | 352 |
| `Process__9VIHSpriteiR7VIScene` | 0x01064400 | 168 | 176 |
| `SetBindPose__9VIHSprite` | 0x01064520 | 120 | 128 |
| `Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode` | 0x01201da8 | 504 | 524 |

## Behaviour notes (observed unless stated)

* `Clear__9VICSprite`, `SetAnimPriorities`, `SetAnimSoundChannels` write fixed reset values into the character object; `SetAnimPriorities` and `SetAnimSoundChannels` touch a table of 52-byte per-animation
  records (stride 52 observed; the record contents are unknown).
* `SetDefaultEntries__9VICSprite` fills the 66 entries (52 bytes, from this+424) whose first word is -1 from entry 0, 22, 23 or 41 (six-way jump table at 0x011d42b0). Observed. The role
  of the entries is inferred only.
* `ProcessWeaponTrails` and `ProcessPlayList` are described in their source headers (jump tables at 0x011d4250 and no table respectively).
* Names for fields are not claimed; offsets are used throughout.

## Imports closed (36)

`Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType`, `SetKeyframe__13VIHSpriteAnimiif`, `SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame`, `SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame`, `SetNodeRefID__13VIHSpriteAnimii`, `Init__t7VIArray1Z20VICSpritePartEmitteri`, `Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger`, `CalcLODOffDist__9VICSpriteR7VIScene`, `Clear__9VICSprite`, `CreateParticleEmitters__9VICSpriteR7VIScene`, `Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi`, `ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44`, `SetAnimPriorities__9VICSprite`, `SetAnimSoundChannels__9VICSprite`, `SetDefaultEntries__9VICSprite`, `SetDefaults__9VICSpriteR7VIScene`, `Begin__Ct6VIPool1Z19VIHSpriteAttachment`, `Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment`, `End__Ct6VIPool1Z19VIHSpriteAttachment`, `Next__Ct6VIPool1Z19VIHSpriteAttachmenti`, `Begin__Ct6VIPool1Z13VIHSpritePlay`, `Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay`, `End__Ct6VIPool1Z13VIHSpritePlay`, `Next__Ct6VIPool1Z13VIHSpritePlayi`, `Size__Ct6VIPool1Z13VIHSpritePlay`, `Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode`, `AddPlay__9VIHSpriteiR7VIScene`, `CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44`, `CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44`, `Clear__9VIHSprite`, `Init__9VIHSprite`, `ProcessPlayList__9VIHSpriteiR7VIScene`, `ProcessUpHierarchy__9VIHSpritei`, `Process__9VIHSpriteiR7VIScene`, `SetBindPose__9VIHSprite`, `Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode`

## Imports opened (callees left unresolved by these sources and not yet on the shared unresolved list, 45; three more callees were already on it)

`AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene`, `AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3`, `Allocate__13VIHSpriteAnim`, `Allocate__t6VIPool1Z13VIHSpritePlay`, `Allocate__t6VIPool1Z17VIHSpritePlayNode`, `Allocate__t6VIPool1Z19VIHSpriteAttachment`, `Allocate__t7VIArray1Z16VIHSpriteTrigger`, `Allocate__t7VIArray1Z20VICSpritePartEmitter`, `Allocate__t8VIVector1Z13VIHSpriteNode`, `AttachSkin__9VIHSpriteiR7VIScene`, `Begin__Ct6VIPool1Z17VIHSpritePlayNode`, `CalcDataSize__13VIHSpriteAnim`, `CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44`, `Clear__t6VIPool1Z13VIHSpritePlay`, `Clear__t6VIPool1Z17VIHSpritePlayNode`, `Clear__t6VIPool1Z19VIHSpriteAttachment`, `Clear__t7VIArray1Z16VIHSpriteTrigger`, `Clear__t7VIArray1Z20VICSpritePartEmitter`, `Clear__t8VIVector1Z10VIMatrix44`, `Clear__t8VIVector1Z13VIHSpriteNode`, `Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition`, `DestroyPool__t6VIPool1Z13VIHSpritePlay`, `DestroyPool__t6VIPool1Z17VIHSpritePlayNode`, `DestroyPool__t6VIPool1Z19VIHSpriteAttachment`, `DestroyPool__t7VIArray1Z16VIHSpriteTrigger`, `DestroyPool__t8VIVector1Z13VIHSpriteNode`, `End__Ct6VIPool1Z17VIHSpritePlayNode`, `GetAttachSprite__C9VIHSpritei`, `GetTime__7VIScene`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Next__Ct6VIPool1Z17VIHSpritePlayNodei`, `ParticleDefinition__16VIParticleSystemi`, `ProcessHierarchy__9VIHSprite`, `ProcessPlayTiming__9VIHSpriteiR7VIScene`, `SetHair__9VICSpriteiiiR7VIScene`, `Slerp__6VIQuatRC6VIQuatT1f`, `Sprite__7VIScenei`, `Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2`, `Transform__10VIMatrix44RC6VIQuatfRC7VIVect3`, `UpdateCenterTransform__9VIHSpriteR7VIScene`, `VITrailFxBegin__Flif`, `_$_10VIMatrix44`, `__10VIMatrix44RC10VIMatrix44`, `__builtin_vec_delete`, `rand`

Several of these belong to other packages or to container templates (`Allocate__`, `Clear__t...`, `DestroyPool__` of the `VIPool`/`VIArray`/`VIVector` instances; `Begin`/`End`/`Next` of the play-node pool);
`GetTime__7VIScene`, `Sprite__7VIScenei`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Slerp__6VIQuatRC6VIQuatT1f`, `rand`, `__builtin_vec_delete` and `_$_10VIMatrix44` are shared engine or libc symbols.
`AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene` (the four-argument overload called by `SetDefaults`) is a different symbol from the `AddPlay__9VIHSpriteiR7VIScene` closed here.

## Build-plan entries needed

`docs/agents/w15-vi-sprites-a-plan-entries.json` holds the 36 plan entries (one per source directory, `profile` `authored_cpp`, sha256 of the source, emitted function and compiled size).
The lead integrator merges them into `experimental/build-plan.json`; this package did not edit the shared plan, snapshot or receipts.

## Authored but never executed

Every function here was run only in the private interpreter against the image's own function with stubbed callees, and in native host checks. None ran inside a game build or on the EE.
Not executed even there: the real callees (everything listed under imports opened is stubbed), and in `ProcessWeaponTrails` / `ProcessPlayList` the VU0 matrix operators and `Slerp`.

## Unknown

* What the object fields at +400..+420, +3856, +4364..+4592 of the character sprite mean; what the 66 entries and their ids are; the meaning of the values 22..44 at +4492 in `ProcessWeaponTrails`.
* The unit of the time stamps in `SetDefaults` (+4528) and `ProcessWeaponTrails` (+4568, 50 units between segments).
* Whether `DestroyPool` can fail in the pool copy-initialisers (its result is not read).
* Names of the enumerations (skeleton type, race, sex) and of the sprite kinds 0, 1, 2, 3, 7, 11.
