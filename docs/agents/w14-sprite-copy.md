# w14-sprite-copy: sprite copy and release virtuals

Branch `agents/w14-sprite-copy`. One commit per function. Source is under `experimental/sources/w14-sprite-copy-*`; each directory holds the
authored source (`copy.cpp` or `release.cpp`) and a `host_check.cpp` that includes host headers and is not a plan input. Addresses and sizes are
from the pinned private `SLUS_205.65` (`work/private/`, read locally with the reviewed EE binutils; no listing or bytes are committed). Every source
file opens with an Observed / Inferred / Unknown comment. Nothing here ran on the EE or as gameplay; compiled sizes are the reviewed EE `authored_cpp`
objects and are not claims of byte identity.

Three early commit titles (the `Release__` functions for the point sprite, point light and stream-audio sprite) quote the *compiled* size, 60 bytes; their image size is 64
bytes. Every later commit title quotes the image size. The table below is authoritative.

## What the functions do (shared shape)

Observed. Mangled signatures read `Copy(VIScene&, VIRaster*, VISoundDevice*, VICollide&, int)` and `Release(VIScene&, VIDictionary&, VIRaster*, VISoundDevice*, VICollide&)`, so in
the EE register convention the sprite is a0, the scene a1, and the remaining arguments follow in a2, a3, t0, t1; a `Release` takes the dictionary in a2, raster in a3, sound device in
t0 and collide in t1, a `Copy` takes the raster in a2, sound device in a3, collide in t0 and the integer in t1. A `Copy` of a sprite subclass creates a sprite of the same kind code
(the word at +52) with `CreateSprite__7VIScene12VISpriteTypei(scene, kind, i)`, returns -1 when that is negative, fetches the new sprite with `Sprite__7VIScenei`, copies the 44 common
bytes at +8..+51 and the kind word at +52 and then the class-specific fields, and takes references on shared resources (`Share*` calls) with -1 returned on a negative result where the
result is tested. A `Release` removes the sprite from the dictionary when its first word is nonzero (-1 on a negative result) and releases the class-specific resources. The base-class
entries `Copy__8VISprite` and `Release__8VISprite` (8 bytes each, not in this package) only return -1 and 0 respectively.

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags (`go.sh`-style single-unit compiles; no memcpy or other runtime import appears).
* Every unit has a host check (`c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp && ./a.out`, return 0) with recording stubs. They compare the call order, arguments, results,
  and every byte of the new sprite against an expectation written from the observed behaviour; the larger ones build the sprite pools and child vtables in a host arena
  (`NORRATH_HOST_ARENA`) so that 32-bit pointer words work on a 64-bit host. Hand mutants of the larger host checks fail them.
* Every function was also compared with the image in a private paired run (a small R5900 interpreter outside git): the original function and the freshly compiled authored function
  run from identical memory snapshots with identical stubbed callees, and the call logs (arguments), the return value and every changed byte outside the stack must be equal. Where a
  callee is a tiny accessor or a pool iterator (`Begin/Next/End` of `VIPool<VIHSpriteAttachment>` and `<VIHSpritePlay>`, the `Location/Direction/Space` emitter accessors, the effect-volume
  `Type/Density/Flags/Surface` accessors) the image's own code ran instead of a stub. Scenario counts, the callee-result classes covered and the original-word coverage are in each commit message.
  Missed original words are annulled branch-delay-slot nops (and, for `Copy__13VIGroupSprite`, the words of the aligned or the unaligned copy path that the alignment of one run does not take; a second
  run at a shifted alignment covers the other path).
* Every function was mutated on purpose (copy lengths, offsets, strides, loop bounds, comparisons such as `< 0` versus `<= 0` and `>= 0` versus `> 0`, dropped calls and stores, swapped words) and
  each mutant that can differ in behaviour fails the paired run. A weak first version of the HSprite result tests was strengthened (success results 3 and 0, failure results -4 and -1) after a
  `< -1` mutant survived; the final tests kill it. Two harness mistakes were found by checking original-word coverage (a scenario control word overlapped another and made every case return -1
  early); the final runs reach 518/519 words of `Copy__9VICSprite` and 186/187 of `Release__9VICSprite`.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
* A private partial build with the 28 units added (6,263 inputs, `--check-plan` passed on the private plan) closes all 28 imports: unresolved 957 -> 982 (28 closed, 53 newly visible callees, listed below).
  The baseline build of the unchanged shared plan listed the same 957 and all 28 of this package's names.

## Units

| Function | Image address | Image bytes | Compiled bytes |
|---|---|---|---|
| `Copy__13VIFloraSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010060b0 | 296 | 264 |
| `Copy__19VIStreamAudioSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010191f8 | 252 | 224 |
| `Copy__13VIGroupSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x0101d830 | 644 | 536 |
| `Copy__9VIHSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x01061320 | 888 | 780 |
| `Copy__11VILODSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010765e0 | 388 | 368 |
| `Copy__12VINameSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010a0b60 | 328 | 296 |
| `Copy__16VIParticleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010aa4d8 | 388 | 352 |
| `Copy__9VICSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010b0b80 | 2076 | 2024 |
| `Copy__12VIPointLightR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010c3cc0 | 256 | 224 |
| `Copy__13VIPointSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x010cbab0 | 224 | 192 |
| `Copy__20VIEffectVolumeSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x0114e5a8 | 324 | 312 |
| `Copy__14VISimpleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x01166de8 | 340 | 304 |
| `Copy__12VISkinSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x0116d2e8 | 304 | 272 |
| `Copy__13VISoundSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | 0x0117c2c0 | 264 | 232 |
| `Release__13VIFloraSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010062e8 | 132 | 136 |
| `Release__19VIStreamAudioSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010192f8 | 64 | 60 |
| `Release__13VIGroupSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x0101dc80 | 160 | 164 |
| `Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x01061698 | 332 | 336 |
| `Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x01076a40 | 160 | 164 |
| `Release__12VINameSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010a1220 | 132 | 116 |
| `Release__16VIParticleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010aa740 | 132 | 120 |
| `Release__9VICSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010b13a0 | 748 | 748 |
| `Release__12VIPointLightR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010c3dc0 | 64 | 60 |
| `Release__13VIPointSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x010cbb90 | 64 | 60 |
| `Release__20VIEffectVolumeSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x0114e878 | 96 | 92 |
| `Release__14VISimpleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x01167198 | 156 | 160 |
| `Release__12VISkinSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x0116d570 | 132 | 136 |
| `Release__13VISoundSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | 0x0117c3c8 | 176 | 192 |

`Copy__13VIGroupSprite` and `Copy__9VICSprite` copy large blocks with `long long` accesses of alignment 1; the image tests alignment at run time, the authored loops do not. Byte results are equal.

## Observed, inferred, unknown

The leading comment of each source file is the record; the points that matter across units:

* Observed: field offsets used (all offsets relative to the sprite). `+0` registration word (nonzero means registered in the dictionary); `+8..+51` eleven common floats; `+52` kind code (3 skin, 7 LOD in sibling
  constructors); class words: point light `+64..+79` (16 bytes), stream audio `+64,+68` and a text buffer at `+72` (128 bytes), name `+64` and a text buffer at `+68` (32 bytes) plus 16 bytes at `+100`, skin
  `+64,+68,+72`, flora `+64,+68`, simple `+64,+68,+72`, sound `+64` and a float at `+84`, particle sprite `+64` (definition id, -1 none), emitter at `+80` and a word at `+1072`, LOD `+64,+68` (count) with
  5 child ids at `+76+8*i`, group `+64` (count) with up to 5 children in 32-byte records from `+68`, effect volume an embedded volume at `+88`, H sprite containers at `+64,+76,+88,+96,+120,+144` and
  words at `+384,+388`, C sprite the H sprite layout plus a 66-record table at `+424` (52-byte stride), surface ids at `+3908` (24-byte stride), an emitter array at `+4616/+4620` (1056-byte entries) and others.
* Observed: the C sprite copy calls `GetSpriteType__7VIScenei` up to three times per attachment (once per comparison with 6, 3 and 7) and the level-of-detail branch calls `SetLevelSprite__11VILODSpriteiiR7VIScene`
  per child of kind 3; the group copy calls the function in each child's class table entry at `+16/+20` (halfword adjustment, function word) with the child and the scene, and calls `CopySprite__7VISceneii` for the child only when that returns exactly 1. Inferred: that function is a duplicate-or-share query.
* Observed: `Copy__16VIParticleSprite` sets the emitter location twice with identical arguments; the duplicate call is kept.
* Observed: the null tests on `scene + 1352` (embedded particle system) are folded away by the reviewed compiler in `Copy__16VIParticleSprite`, `Release__16VIParticleSprite` and `Release__9VICSprite`; the image tests the address.
  No real scene address is zero, so no run can tell the two apart.
* Inferred: the common floats are position/orientation/scale; the kind codes; which containers are attachments and plays (names come from template instance names in symbols, not from layouts);
  that `scene + 19352` is the resource dictionary. None of these names is used in the source.
* Unknown: every field's meaning; the sprite kind 6 (treated like a skin sprite by the C sprite copy: copied, but no material-palette call); what `strcpy8__8VIStringPcUiPCc` guarantees at the end of a short source (the stubs only
  record the call); the true return types of the many callees (all assumed `int` or `void` with matching width; none of the results marked unused is used by the image); the first-word registration semantics; the size of every sprite
  object (the sprite allocation size is not read here); why several loops reread their count after every call.

## Imports closed

Copy__11VILODSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__12VINameSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__12VIPointLightR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__12VISkinSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__13VIFloraSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__13VIGroupSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__13VIPointSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__13VISoundSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__14VISimpleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__16VIParticleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__19VIStreamAudioSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__20VIEffectVolumeSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__9VICSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Copy__9VIHSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei, Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__12VINameSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__12VIPointLightR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__12VISkinSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__13VIFloraSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__13VIGroupSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__13VIPointSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__13VISoundSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__14VISimpleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__16VIParticleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__19VIStreamAudioSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__20VIEffectVolumeSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__9VICSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide, Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide.

## Imports opened

Newly visible unresolved names after adding these units (53; none was on the baseline list). These are callees of the units above and belong to other areas of the sprite, raster, pool, particle and effect-volume code.
This package does not define any of them.

AttachSurface__14VIEffectVolumeii, Begin__Ct6VIPool1Z13VIHSpritePlay, Begin__Ct6VIPool1Z19VIHSpriteAttachment, CopyMaterialPal__8VIRasterii, CopySprite__7VISceneii, CreateParticleEmitters__9VICSpriteR7VIScene, Density__C14VIEffectVolume, Direction__C17VIParticleEmitter, End__Ct6VIPool1Z13VIHSpritePlay, End__Ct6VIPool1Z19VIHSpriteAttachment, Flags__C14VIEffectVolume, Free__14VIEffectVolume, GetSpriteType__7VIScenei, Init__14VIEffectVolumeRC6VIBBox, Init__20VIEffectVolumeSpriteRC6VIBBox, Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay, Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode, Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment, Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger, Init__t7VIArray1Z20VICSpritePartEmitteri, Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44, Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode, IsPlaying__13VISoundDevicei, Location__C17VIParticleEmitter, Next__Ct6VIPool1Z13VIHSpritePlayi, Next__Ct6VIPool1Z19VIHSpriteAttachmenti, ReleaseAnimation__7VIScenei, ReleaseFont__8VIRasteriR12VIDictionary, ReleaseMaterialPal__8VIRasteriR12VIDictionary, ReleasePrimBuffer__8VIRasteriR12VIDictionary, ReleaseRefMap__7VIScenei, ReleaseSprite__7VIScenei, SetDirection__17VIParticleEmitterRC10VIMatrix33, SetFlags__14VIEffectVolumeUi, SetLevelSprite__11VILODSpriteiiR7VIScene, SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary, SetSpace__17VIParticleEmitterRC10VIMatrix44, SetType__14VIEffectVolume18VIEffectVolumeTypef, ShareAnimation__7VIScenei, ShareCollBuffer__9VICollidei, ShareFont__8VIRasteri, ShareMaterialPal__8VIRasteri, ShareParticleDefinition__16VIParticleSystemi, SharePrimBuffer__8VIRasteri, ShareRefMap__7VIScenei, ShareSound__13VISoundDevicei, ShareSprite__7VIScenei, Share__9VICSpriteR7VISceneP8VIRaster, Space__C17VIParticleEmitter, Surface__C14VIEffectVolumei, Type__C14VIEffectVolume, VITrailFxEnd__FP9VITrailFx, strcpy8__8VIStringPcUiPCc.

## Build-plan entries needed

All `authored_cpp`, empty header closure, `sha256` of the source at the last commit of this branch. `docs/agents/w14-sprite-copy-plan-entries.py --toolchain DIR [--private-root DIR]` regenerates them (with
`emitted_functions`) and writes a private plan and snapshot copy; the shared `experimental/build-plan.json` and `experimental/snapshot.json` are not edited by this package.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_csprite_copy.o", "profile": "authored_cpp", "sha256": "17cb0f56a3cfc457f4b933ebb336b716f2eba70b7a9c2ab39f5e49b58edc22b8", "source": "sources/w14-sprite-copy-csprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_csprite_release.o", "profile": "authored_cpp", "sha256": "77e290cb7feb0f6505dfff490f557a052ef6bc094b3ee7415ec0f6a4bbf2d674", "source": "sources/w14-sprite-copy-csprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_effect_volume_sprite_copy.o", "profile": "authored_cpp", "sha256": "7beb5667d1ca73421856c4d59baafd057fae5401595f456463922157801767aa", "source": "sources/w14-sprite-copy-effect-volume-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_effect_volume_sprite_release.o", "profile": "authored_cpp", "sha256": "b9165d66e7a783147ee5c5f07d8e27b68ae12e8f1f9b0dc50e27b58198c6ed0d", "source": "sources/w14-sprite-copy-effect-volume-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_flora_sprite_copy.o", "profile": "authored_cpp", "sha256": "060f15a6055498e26c127baf9b7925b64f0ae8674ed1895ac2a11e58e20d077d", "source": "sources/w14-sprite-copy-flora-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_flora_sprite_release.o", "profile": "authored_cpp", "sha256": "d9901f9e69642af8ae85a8d76c16a2d860887ec4e577cb9a679e84b17eb6a789", "source": "sources/w14-sprite-copy-flora-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_group_sprite_copy.o", "profile": "authored_cpp", "sha256": "42c5deecc99ec95fd52ea1ed9a29e91cea65052f1aafca0d611bd9d213a21597", "source": "sources/w14-sprite-copy-group-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_group_sprite_release.o", "profile": "authored_cpp", "sha256": "84445cbb0d689a91a5c97a2a57465de0c5b825e26e1e464acebb966d66b5a715", "source": "sources/w14-sprite-copy-group-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_hsprite_copy.o", "profile": "authored_cpp", "sha256": "66e58832b4840289558db5a8448d17f3589efd28bbfc2fa55f297f53c07fd2c9", "source": "sources/w14-sprite-copy-hsprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_hsprite_release.o", "profile": "authored_cpp", "sha256": "da4b4814e464a538abab960ae89a345810fe7b60dc49aa89815d4f96ed1965f5", "source": "sources/w14-sprite-copy-hsprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_lod_sprite_copy.o", "profile": "authored_cpp", "sha256": "6a359f75b7703ae2acab30711249ebe3c50fdf33806087c99793259189c5e795", "source": "sources/w14-sprite-copy-lod-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_lod_sprite_release.o", "profile": "authored_cpp", "sha256": "65069cd952236efb593435780f741e63af7fdcc6bfecf1663eb76d797e196131", "source": "sources/w14-sprite-copy-lod-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_name_sprite_copy.o", "profile": "authored_cpp", "sha256": "5d889bf177a5080dfe094384decbdff2b97e87fb9c423fdffa35e1da4bb41750", "source": "sources/w14-sprite-copy-name-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_name_sprite_release.o", "profile": "authored_cpp", "sha256": "d86993bb9ca4e7e7f20c5d56d51b94a10fa6a2b5934e65921039b1592d4ac7d0", "source": "sources/w14-sprite-copy-name-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_particle_sprite_copy.o", "profile": "authored_cpp", "sha256": "8d8980237291196def355eb23230fd84a2953dabaf441c17d022bc0a866067d2", "source": "sources/w14-sprite-copy-particle-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_particle_sprite_release.o", "profile": "authored_cpp", "sha256": "1a57297700feabcfc7a3acd08b4ae67fdfb15c8a92cab3b737fab728a5de20f6", "source": "sources/w14-sprite-copy-particle-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_point_light_copy.o", "profile": "authored_cpp", "sha256": "0d75825b34b7cdbba38883b726d2d7197ed354b423fdbda0c6634d45402d6f83", "source": "sources/w14-sprite-copy-point-light-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_point_light_release.o", "profile": "authored_cpp", "sha256": "d3410ba7b81b691ebbfec6733a4e9cb523c665ec255279774d3630063fedfb5a", "source": "sources/w14-sprite-copy-point-light-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_point_sprite_copy.o", "profile": "authored_cpp", "sha256": "4556ae5b28061875fdf4c292cae57b2298658a27635461d7f891cd59989dcadb", "source": "sources/w14-sprite-copy-point-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_point_sprite_release.o", "profile": "authored_cpp", "sha256": "35b9c387f845c8896bcbf33cf0d91456f4fa64727330d12be8e6a5bfd8b539ec", "source": "sources/w14-sprite-copy-point-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_simple_sprite_copy.o", "profile": "authored_cpp", "sha256": "c93de89e3bc220b581c4865b9a38fe36999a7ab9448efcd448d227e3a4bb0c08", "source": "sources/w14-sprite-copy-simple-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_simple_sprite_release.o", "profile": "authored_cpp", "sha256": "a204435e78ae43860fd8888e1d536941343b34240d86f5d7dcda60ece858d550", "source": "sources/w14-sprite-copy-simple-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_skin_sprite_copy.o", "profile": "authored_cpp", "sha256": "880924aab115e4d7450430cc26e026d6527915ae1bfcdac76227ed97549583e6", "source": "sources/w14-sprite-copy-skin-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_skin_sprite_release.o", "profile": "authored_cpp", "sha256": "1da6391704177131c73aad69abb0a10fedc897137c851f180a71ec7ba116c0a2", "source": "sources/w14-sprite-copy-skin-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_sound_sprite_copy.o", "profile": "authored_cpp", "sha256": "d22da831b12fb257adc18ef6ae0b68aef188b154e1dceb15dcdb853a98b36f74", "source": "sources/w14-sprite-copy-sound-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_sound_sprite_release.o", "profile": "authored_cpp", "sha256": "1c6794f71047d5917fba62bb5836282feb63dd6e845337eb8bbd4a6c95595f56", "source": "sources/w14-sprite-copy-sound-sprite-release/release.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_stream_audio_sprite_copy.o", "profile": "authored_cpp", "sha256": "62ed213ab5921b814923c0f79d92b7bada174866f45c53caf3e254c987f0a8eb", "source": "sources/w14-sprite-copy-stream-audio-sprite-copy/copy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w14sc_stream_audio_sprite_release.o", "profile": "authored_cpp", "sha256": "672f1d7fe2ff5953774579285ad73f0b446328cb26dac8a866ab602e0172d5b3", "source": "sources/w14-sprite-copy-stream-audio-sprite-release/release.cpp"}
```

## Authored but never executed

Everything. No unit has run on the EE, an emulator or as gameplay. The paired runs execute the image's original function on a small interpreter with stubbed callees and compare it with the freshly
compiled authored function on the same interpreter. The callees that matter for correctness in the game (scene sprite creation and lookup, the `Share*` and `Release*` families, the container
initializers, `SetMaterialPal`, `SetLevelSprite`, `CreateParticleEmitters`, the virtual duplicate-query slot in the group copy) are stubs in every check, so call order and memory effects are verified but the
callee behaviour is not. The pointer-word reads (pool entry arrays, emitter arrays, class tables, the dictionary pointer at `scene + 19352`) use real memory only in the paired runs; the host checks use an arena.

## Still unknown or not done

* `Copy__8VISprite` and `Release__8VISprite` (base class, 8 bytes each) were not in this scope.
* The callees listed under imports opened are not authored here.
* Sprite object sizes, the full layouts of the 52-byte, 32-byte, 16-byte, 104-byte and 1056-byte records, and what kind code 6 is remain unknown.
* The vtable placement of these functions (which slot of each class table points at them) is not established here; the callers in scene traversal, collision, picking and render loops were not read.
