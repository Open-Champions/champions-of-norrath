# Textures, models, world and lumps (1/2)

Package `w8-render-world-a`, branch `agents/w8-render-world-a`. New source is under `experimental/sources/w8-render-world-a-*`. Each directory has a `host_check.cpp` that includes host headers and must stay out of the EE plan. Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here has run on the EE or as gameplay. The compiled sizes are the reviewed EE `authored_cpp` objects and are not claims of byte equality with the image.

## Evidence

- Host checks: every unit has one. `c++ -std=c++17 -o /tmp/hc experimental/sources/<dir>/host_check.cpp && /tmp/hc` (add `-Wno-unused-parameter -Wno-unused-function -Wno-cast-function-type-mismatch` for the larger ones). All 25 passed at the last commit. The checks stub the callees, so they fail on a wrong offset, branch or call order but do not execute the image.
- Differential comparison with the image: `docs/agents/w8-render-world-a-compare.py <function|all> [runs]`, with `NORRATH_EXE` set to the private executable and `NORRATH_EE` to the reviewed EE toolchain. It reuses the small R5900 interpreter of `docs/agents/combat-reactions-compare.py` (imported by path), patched in memory for `sqrt.s`, `min.s`, `max.s` and for the `teq` that the authored build adds to integer division. Each function runs from the image and from the freshly compiled and linked authored object, at a scratch address, on the same randomized memory with the same stubbed callees. The stub call logs (argument registers, the bytes behind pointer arguments, strings, stack arguments), the return value and every compared byte must be equal. Data the function reaches by name is placed at the image's address of that name for both runs. The last run was 300 seeds per function (`all 300`) with zero mismatches for all 24 functions. Deliberately wrong constants, offsets and branch senses make the check fail for every unit (done by hand for each; the larger ones with 20 and more variants). Floats in the scenarios are small values or exact in single precision; the interpreter rounds to nearest while the EE truncates, so both runs see the same result.
- Whole-image effect, measured on the shared tip `d4fa081` with the 25 units added through a private plan and snapshot kept outside git (`--check-plan` passed): the base partial build had 1195 unresolved imports and 1107 function bodies; with these units 1212 unresolved, 1134 bodies, zero COMMON. 30 names left the list and 47 appeared (below). `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

## Units

| Authored function | Image address | Image bytes | Compiled bytes | How it relates to the image |
|---|---|---|---|---|
| `lumpCheck__Fv` | 0x001663b8 | 52 | 36 | counted loop over `nmLumps` with no effect |
| `lumpFindByType__FPvPci` | 0x00166650 | 208 | 196 | complete body |
| `lumpQuery__FPc` | 0x00166e30 | 220 | 228 | complete body |
| `lumpUnload__FPv` | 0x00164800 | 528 | 544 | complete body |
| `loadDirectory__FP9directoryii` | 0x00164c70 | 488 | 432 | complete body (file-local in the image) |
| `blockAlloc__Fi` | 0x001441b8 | 188 | 8 | name for published `norrath_block_alloc` |
| `blockAllocInit__Fv` | 0x001440d8 | 220 | 8 | name for published `norrath_block_alloc_init` |
| `blockGetNmFree__Fv` | 0x00144778 | 96 | 8 | name for published `norrath_block_get_number_free` |
| `floorClear__Fv` | 0x00191de0 | 276 | 8 | name for published `norrath_floor_clear` |
| `checkCanAllocDescriptor__Fv` | 0x00187268 | 88 | 64 | complete body (file-local in the image) |
| `evalHermite__Ffffff` | 0x00159330 | 96 | 84 | complete body, same operation order |
| `getMegatileCenter__FG6Point2` | 0x00265810 | 340 | 304 | complete body |
| `getMegatileOriginalCenter__FG6Point2` | 0x00265968 | 348 | 316 | complete body |
| `megatileInit__FP12MegatileRootPv` | 0x0025e0e0 | 708 | 652 | complete body |
| `_textureDrawColor__FP8_textureiiiUi` | 0x0016e658 | 516 | 508 | complete body |
| `creatureAdvanceAnimation__8Creaturef` | 0x00111830 | 420 | 352 | complete body |
| `runStandardGlow__8Creature` | 0x00109710 | 244 | 120 | complete body (jump table read from the image) |
| `setDirLight__Fsssssfb` | 0x00109568 | 420 | 404 | complete body (file-local in the image) |
| `draw__8CreatureP12_modelHeaderP8_textureT1` | 0x00109808 | 2716 | 2824 | complete body |
| `everyFrame__8CreatureP12_modelHeader` | 0x0010ebf8 | 4136 | 3948 | complete body |
| `_GLOBAL_$I$modelClippingOn` | 0x0016ce68 | 32 | 12 | wrapper; its static initializer (8 bytes in the image) is an empty routine |
| `_GLOBAL_$I$edgearray` | 0x00266ec8 | 32 | 12 | wrapper plus the unit's static initializer (clears the first word of `zeroTile`) |
| `_GLOBAL_$I$worldTileEntityLightScale` | 0x0017dd70 | 32 | 12 | wrapper plus the static initializer (shared helper, construction half) |
| `_GLOBAL_$D$worldTileEntityLightScale` | 0x0017dd90 | 32 | 12 | wrapper (destruction half of the same helper) |

Storage units: `w8-render-world-a-render-storage` (`lastNmBlocksFree` = 100000, `cacheHead`, `cacheTail`, `checkBlock`, `lastNmQ.1088`, `loadSampleRate.784`, `decompressStack` 16384 bytes, `cache` 192 bytes at 0x006e5d80) and `w8-render-world-a-megatile-storage` (`mgrg`, `mt_tiledata` 320 bytes, `zeroTile` 72 bytes). Extents are the span to the next symbol; the image bytes are zero except `lastNmBlocksFree`.

The four forwarders (`blockAlloc__Fi`, `blockAllocInit__Fv`, `blockGetNmFree__Fv`, `floorClear__Fv`) exist because the callers import the original names while earlier packages published the bodies as `norrath_block_alloc`, `norrath_block_alloc_init`, `norrath_block_get_number_free` and `norrath_floor_clear`. Each forwarder was compared with the image together with the published body, and a deliberate change in the published body makes that comparison fail. They are not new reconstructions; each commit says so. They need those published inputs to stay in the plan.

## Observed

Each source header lists the observations for its function: offsets, branch order, call order, constants and the bytes of the arguments. Short form of the less obvious ones:

- The lump record is 40 bytes: name at +0, the lump data pointer at +16, a flag word at +20 whose bit 0 hides a record from `lumpQuery__FPc`, the lock word at +32 and a word at +36 that triggers `memFree__FPv` in `lumpUnload__FPv`. A directory is a 16-bit count and 12-byte entries from +4 whose first word points to the name. The disc directory is 32-byte rows with the count at +24576.
- `lumpUnload__FPv` picks the release call for a single-file lump (flag bit 0 set) from the extension of the name stored at the lump pointer: `tex`, `world` or `uni`, on the pointer plus 256.
- Struct-by-value `Point3` and `Point2` arguments are passed by address to a caller-made copy in this image. `getMegatileCenter__FG6Point2` and `getMegatileOriginalCenter__FG6Point2` take the result address first and scale the caller's copy in place by the single 0x3ab60b60 (just below 1/720).
- `draw__8Creature...` calls virtual entry 3 (`vtable + 24`, entries of a halfword adjustment and a pointer) of each object on the list at +888; `everyFrame__8Creature...` calls entries at `vtable(+72) + 136` and `+ 152`.
- In `draw__8Creature...` the image repeats the 14-step effect selection 14 times with identical results. The authored body does it once. In `everyFrame__8Creature...` the image computes two integer divisions whose results are discarded; they are omitted.
- `everyFrame__8Creature...` divides by `hi - lo + 1` of two halfwords with no zero check; a zero range is not exercised and its result is unknown.
- `_GLOBAL_$I$worldTileEntityLightScale` runs, in order: sixteen `ParticleDef` constructors on `ParticleEmitterArray`, flag clears in `DynamicLightArray` and `routeQueue`, three full `ParticleDef` constructors (`droplet`, `rainSplashParticle`, `ParticleEmitterTestParticle`) and the `worldAnimation` byte; its `$D$` twin destroys `worldAnimation`, `routeQueue`, `DynamicLightArray` last to first.

## Inferred

- The block, floor and megatile forwarders are the same functions as the published `norrath_*` bodies, from the paired comparisons above.
- `setDirLight__Fsssssfb` is a directional light from yaw and pitch angles with flicker noise; `_textureDrawColor__FP8_textureiiiUi` builds a coloured sprite packet using the GS register map names for the A+D addresses (0x3f, 0x14, 0x01, 0x42, 0x00, 0x47). The image shows the numbers; the register names are from the public GS map.
- The `everyFrame__8Creature...` groups are path following, player pursuit, status timers (fire, shock, slow, stun, burn, poison), aggro and a vertical spring. The names are guesses at purpose and are not used in the source.
- `lumpCheck__Fv` and `megatileInit__FP12MegatileRootPv` contain empty counted loops that look like remains of removed checks. The authored bodies keep them with an empty `asm volatile` so the iteration count survives.
- The record at `mt_tiledata` (10 signed bytes) holds two rectangles of a tile; `getMegatileCenter__FG6Point2` reads bytes 7, 8, 4, 3 for the test and the centre, `getMegatileOriginalCenter__FG6Point2` reads bytes 0, 1, 5, 6 for the centre.

## Unknown

- Every field name in the Creature, lump, megatile root and path-state layouts; the meaning of the bytes the source reads but does not name.
- What the second pair of 32-bit words of a lump directory entry holds, and the meaning of the +36 word of a lump record.
- The size of the stack objects the image passes by address (`gameLinePathState`, `item::description`); the authored bodies reserve 48 and 96 bytes and write only the bytes the image writes.
- `megatileGenerate__FP12_worldHeader` (0x0025e3a8, 19680 bytes, 4921 instructions) was not attempted. It does its arithmetic through the SN double-precision helpers `litodp`, `dpmul`, `dpadd`, `dpsub` and `dptofp`, which this reviewed profile cannot produce (its `double` code calls libgcc names), and it calls `floodclear__FPA215_cii`, `sprintf`, `scePrintf`, `strlen`, `_engineCheckStack__FPCci` and `textureReleaseCacheBuffer__Fv`. A faithful rebuild needs a decision on the double-precision helper binding first.
- The second local symbol named `cache` (0x007cb770, 3520 bytes) has no authored user here; only the 192-byte `cache` at 0x006e5d80 that `textureInternalInit__Fv` uses is defined.

## Imports closed

`_GLOBAL_$D$worldTileEntityLightScale`, `_GLOBAL_$I$edgearray`, `_GLOBAL_$I$modelClippingOn`, `_GLOBAL_$I$worldTileEntityLightScale`, `_textureDrawColor__FP8_textureiiiUi`, `blockAllocInit__Fv`, `blockAlloc__Fi`, `blockGetNmFree__Fv`, `cache`, `cacheHead`, `cacheTail`, `checkBlock`, `checkCanAllocDescriptor__Fv`, `creatureAdvanceAnimation__8Creaturef`, `decompressStack`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `evalHermite__Ffffff`, `everyFrame__8CreatureP12_modelHeader`, `floorClear__Fv`, `getMegatileCenter__FG6Point2`, `getMegatileOriginalCenter__FG6Point2`, `lastNmBlocksFree`, `lastNmQ.1088`, `loadDirectory__FP9directoryii`, `loadSampleRate.784`, `lumpCheck__Fv`, `lumpFindByType__FPvPci`, `lumpQuery__FPc`, `lumpUnload__FPv`, `megatileInit__FP12MegatileRootPv` (30 of the 33 in the brief).

Not closed:

- `floorAdd__FG6Point3sfii`: already authored as `floor-world-view-floor-add/floor_add.cpp` (package `floor-world-view`, forwards to the published `norrath_floor_add`). That unit is not in the plan at this tip, so the name stays unresolved until the integrator adds it.
- `lumps`: the storage already exists as `norrath_constructor_lump_cache_view` (object-infra, 128 records of 40 bytes at 0x006cb280, same extent). A second definition would split one table in two. The name needs a linker-level alias to that view, which the plan format has a slot for (`symbol_aliases`) but `tools/build_partial.py` rejects. Left for the integrator.
- `megatileGenerate__FP12_worldHeader`: see Unknown.

## Imports opened

Measured by diffing the unresolved lists of the two builds above. None of these is defined by this package; they are other packages' symbols reached by the new bodies.

Functions: `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_5Light`, `__11ParticleDef`, `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `addEffect__FiG6Point3`, `animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps`, `drawTexture__FR6PacketP8_textureiiiiii`, `eRand__10GameObject`, `ef1Noise__Fi`, `freeLump__FPv`, `fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff`, `fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi`, `fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `gameLineGet__FPc`, `gameLineGetVert__Fii`, `gameLineInitPathState__FP17gameLinePathStatePcP6Point3`, `getClosestLivingPlayer__FG6Point3f`, `initPosition__FP17gameLinePathState`, `isBoss__FP10GameObject`, `lightSetAmbientColor__FPs`, `lightSetAmbientOffset__FPs`, `lightSetColorScale__Ffff`, `lightSetPrimaryDirectional__FG6Point3Ps`, `lightSetSecondaryDirectional__FG6Point3Ps`, `matMakeTransYawScale__FP8Matrix34G6Point3sf`, `megatileBankInit__FP16MegatileInstBankPv`, `megatileItemInit__FP8MegatilePv`, `modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `mtcInitialize__Fv`, `netCheckActivation__FG6Point3f`, `netGetLocalObject__FP8Creature`, `sceCdRead`, `strrchr`, `textRemoveText__FPv`, `textureFreeTexture__FP8_texture`, `worldFreeWorld__FP12_worldHeader`, `worldHitscan__FP12_worldHeaderG6Point3T1fi`.

Data: `ParticleEmitterTestParticle`, `bossMeterName`, `droplet`, `localPlayersAlive`, `rainSplashParticle`, `routeQueue`, `worldAnimation`.

Names these bodies reference that were already unresolved and stay so: `DynamicLightArray`, `ParticleEmitterArray`, `_$_14AnimationState`, `_$_16ListSafeIterator`, `__16ListSafeIteratorP8ListHeadi`, `bossMeterCurrent`, `bossMeterMax`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `localNmPlayersAlive`, `lumps`, `memFree__FPv`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `next__16ListSafeIterator`, `nmLumps`, `player`, `resetRouting__8Creature`, `sceCdSync`, `strcasecmp`, `strcpy`, `world`, `worldViewCenter`. Names these bodies reference that are defined elsewhere in the plan or by this package's storage units: `CDmode`, `GLISTSIZE`, `descListMark`, `drawAddToList__FPviP8_textureP11_drawRecordi`, `findTagString__10GameObjectPci`, `frameNm`, `gameIsPaused__Fv`, `glistp`, `gstart`, `haloTexture1`, `haloTexture2`, `levelFlags`, `levelName`, `lightResetOptionalParams__Fv`, `mgrg`, `mt_tiledata`, `netNetworkMode`, `nmPlayers`, the four published `norrath_*` bodies, `norrath_list_destroy`, `objectUpdateInGrid__FP10GameObject`, `sscanf`, `strcmp`, `zeroTile`. `runStandardGlow__8Creature` and `setDirLight__Fsssssfb` were opened by `draw__8Creature...` during the work and are now defined here, so they are not in the list above.

## Build-plan entries needed

The shared plan and snapshot were not edited. The integrator needs, for each source below, a `files` entry (path to sha256) in both `experimental/build-plan.json` and `experimental/snapshot.json`, one `inputs` entry in the plan, and one `inputs` entry in the snapshot, with `input_order` assigned at merge, and the plan's `default_authored_inputs` raised by 25 and its `snapshot_sha256` refreshed. The four forwarders require the published inputs `block-alloc-source-v2/*`, `block-allocator-lifecycle-build/*`, `memory-block-storage/block_storage.cpp` and `game-floor-clear-source/floor_clear.cpp`, which are already in the plan. `_GLOBAL_$D$worldTileEntityLightScale` and `_GLOBAL_$I$worldTileEntityLightScale` share one source. A private copy of the plan and snapshot with these entries appended (kept outside git, input orders 1409 to 1433, snapshot `emitted_functions` taken from the compiled objects with `elf_inventory`) passed `--check-plan` and built.

Plan `inputs` entries (add `input_order`):

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "block_alloc_init.o",
  "profile": "authored_cpp",
  "sha256": "b7437cc254fcf5694153195afe23467115a512281d68247b1b1dcf14a1db18ef",
  "source": "sources/w8-render-world-a-block-alloc-init/block_alloc_init.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "block_alloc.o",
  "profile": "authored_cpp",
  "sha256": "a5a295ee2788d8cb8cdb35a98b531568f276d52dfe0d4518cf9373fed81fee8c",
  "source": "sources/w8-render-world-a-block-alloc/block_alloc.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "block_get_nm_free.o",
  "profile": "authored_cpp",
  "sha256": "46a876a4d361c233e89772c283f713a56a5a6c0ec8f7a776f522e4ff73003f88",
  "source": "sources/w8-render-world-a-block-get-nm-free/block_get_nm_free.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "check_can_alloc_descriptor.o",
  "profile": "authored_cpp",
  "sha256": "c939fdc11cc08c76255a98b7e13fd68584f998ff887ea332bf070c8969982093",
  "source": "sources/w8-render-world-a-check-can-alloc-descriptor/check_can_alloc_descriptor.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "creature_advance_animation.o",
  "profile": "authored_cpp",
  "sha256": "cc664c50b7ae7a949c561e8710f5f05abcc43791129f9d1bb596b5f381d01cb2",
  "source": "sources/w8-render-world-a-creature-advance-animation/creature_advance_animation.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "creature_draw.o",
  "profile": "authored_cpp",
  "sha256": "0f0c53a29094a05da5e96dee80740ddd8e6caa37704cb430a40e9d4708ab7c5f",
  "source": "sources/w8-render-world-a-creature-draw/creature_draw.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "creature_every_frame.o",
  "profile": "authored_cpp",
  "sha256": "1ebf0418856281cd86f33daa9644a15a5c23cac69f6a838a84e8c38c85c35a78",
  "source": "sources/w8-render-world-a-creature-every-frame/creature_every_frame.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "eval_hermite.o",
  "profile": "authored_cpp",
  "sha256": "c8427fbfd2ac2fec98de8e6fbd8379b810ed228b0667f6b3a0553966a49f1938",
  "source": "sources/w8-render-world-a-eval-hermite/eval_hermite.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "floor_clear.o",
  "profile": "authored_cpp",
  "sha256": "de82e2bb1dc0df8c677f103979d30a9b3e0883d98487fe6b31001fa4efa3a814",
  "source": "sources/w8-render-world-a-floor-clear/floor_clear.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "get_megatile_center.o",
  "profile": "authored_cpp",
  "sha256": "5ff55f0c1528f0d6b0aca90473d2b7b4fd7a633549bc39f0745f167c2d4eda3d",
  "source": "sources/w8-render-world-a-get-megatile-center/get_megatile_center.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "get_megatile_original_center.o",
  "profile": "authored_cpp",
  "sha256": "af7e470fad5a34d56e118a52f14da5b9ffb403a43b1e0cb1ac5924fda96e7356",
  "source": "sources/w8-render-world-a-get-megatile-original-center/get_megatile_original_center.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "global_init_edgearray.o",
  "profile": "authored_cpp",
  "sha256": "3c38620169e03995fedbf70d121843e8318a8b26261c62d352fc0df32a93fa85",
  "source": "sources/w8-render-world-a-global-init-edgearray/global_init_edgearray.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "global_init_model_clipping_on.o",
  "profile": "authored_cpp",
  "sha256": "86ece6162712e6a07a61210ed1fd923c91e2f9b3156bc1c9640e35be26fe8385",
  "source": "sources/w8-render-world-a-global-init-model-clipping-on/global_init_model_clipping_on.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "global_init_world_tile.o",
  "profile": "authored_cpp",
  "sha256": "285a0b3212224e4adc9f4003deed98a06d92752fa9cee380f489b463557c62cd",
  "source": "sources/w8-render-world-a-global-init-world-tile/global_init_world_tile.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "load_directory.o",
  "profile": "authored_cpp",
  "sha256": "b2156ecd61871ac1fdfcf1a569236cacbf6773df5d3eff530f49e72536a6e292",
  "source": "sources/w8-render-world-a-load-directory/load_directory.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "lump_check.o",
  "profile": "authored_cpp",
  "sha256": "17ba1dedc226a1e1c2233532852e91e641ee0138664344f0d017662c8b72a13d",
  "source": "sources/w8-render-world-a-lump-check/lump_check.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "lump_find_by_type.o",
  "profile": "authored_cpp",
  "sha256": "1a0c3354dd0de8452bf532f6661612c636b13b827cf38afbfbf8cb91395c8c11",
  "source": "sources/w8-render-world-a-lump-find-by-type/lump_find_by_type.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "lump_query.o",
  "profile": "authored_cpp",
  "sha256": "ed3a0182cbe62a68f0e48ba8092c9e642602ffce33448907b43097859c03400f",
  "source": "sources/w8-render-world-a-lump-query/lump_query.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "lump_unload.o",
  "profile": "authored_cpp",
  "sha256": "81a4aeaf126dc3995d9621e07ae0b35862e81809d5188572e7f75041beb6de83",
  "source": "sources/w8-render-world-a-lump-unload/lump_unload.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "megatile_init.o",
  "profile": "authored_cpp",
  "sha256": "adb02b7b3224440e6dbad5fe83294d1a49f840aa6f6e510524fbf0abc49f50fd",
  "source": "sources/w8-render-world-a-megatile-init/megatile_init.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "megatile_storage.o",
  "profile": "authored_cpp",
  "sha256": "395c1d22218bcdbc4cff862a69c7f21d8974089b427b1642fb5ed66cd96913cc",
  "source": "sources/w8-render-world-a-megatile-storage/megatile_storage.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "render_storage.o",
  "profile": "authored_cpp",
  "sha256": "e45c2085c104ffaab030aa55874ca5ab3da5019a295282e04d986b930543080e",
  "source": "sources/w8-render-world-a-render-storage/render_storage.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "run_standard_glow.o",
  "profile": "authored_cpp",
  "sha256": "518a74b2346ab9be19ca2dab90bf8038949981b106299ed91677efc7e380c84c",
  "source": "sources/w8-render-world-a-run-standard-glow/run_standard_glow.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "set_dir_light.o",
  "profile": "authored_cpp",
  "sha256": "526ef18050a1c5725b6a92aea404f61617bf8cdb0c16b147bcc994b1fbb1bbab",
  "source": "sources/w8-render-world-a-set-dir-light/set_dir_light.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "texture_draw_color.o",
  "profile": "authored_cpp",
  "sha256": "f3eebd9f15e783fa83390249c2878c8223279c5e282f1d35b9e3f77fcc69b533",
  "source": "sources/w8-render-world-a-texture-draw-color/texture_draw_color.cpp"
 }
]
```

Snapshot `inputs` entries (add `input_order`; `private_object_sha256` is for the integrator's own object):

```json
[
 {
  "component": "w8-render-world-a-block-alloc-init",
  "object_name": "block_alloc_init.o",
  "emitted_functions": [
   {
    "name": "blockAllocInit__Fv",
    "compiled_body_bytes": 8
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-block-alloc-init/block_alloc_init.cpp"
 },
 {
  "component": "w8-render-world-a-block-alloc",
  "object_name": "block_alloc.o",
  "emitted_functions": [
   {
    "name": "blockAlloc__Fi",
    "compiled_body_bytes": 8
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-block-alloc/block_alloc.cpp"
 },
 {
  "component": "w8-render-world-a-block-get-nm-free",
  "object_name": "block_get_nm_free.o",
  "emitted_functions": [
   {
    "name": "blockGetNmFree__Fv",
    "compiled_body_bytes": 8
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-block-get-nm-free/block_get_nm_free.cpp"
 },
 {
  "component": "w8-render-world-a-check-can-alloc-descriptor",
  "object_name": "check_can_alloc_descriptor.o",
  "emitted_functions": [
   {
    "name": "checkCanAllocDescriptor__Fv",
    "compiled_body_bytes": 64
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-check-can-alloc-descriptor/check_can_alloc_descriptor.cpp"
 },
 {
  "component": "w8-render-world-a-creature-advance-animation",
  "object_name": "creature_advance_animation.o",
  "emitted_functions": [
   {
    "name": "creatureAdvanceAnimation__8Creaturef",
    "compiled_body_bytes": 352
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-creature-advance-animation/creature_advance_animation.cpp"
 },
 {
  "component": "w8-render-world-a-creature-draw",
  "object_name": "creature_draw.o",
  "emitted_functions": [
   {
    "name": "draw__8CreatureP12_modelHeaderP8_textureT1",
    "compiled_body_bytes": 2824
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-creature-draw/creature_draw.cpp"
 },
 {
  "component": "w8-render-world-a-creature-every-frame",
  "object_name": "creature_every_frame.o",
  "emitted_functions": [
   {
    "name": "everyFrame__8CreatureP12_modelHeader",
    "compiled_body_bytes": 3948
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-creature-every-frame/creature_every_frame.cpp"
 },
 {
  "component": "w8-render-world-a-eval-hermite",
  "object_name": "eval_hermite.o",
  "emitted_functions": [
   {
    "name": "evalHermite__Ffffff",
    "compiled_body_bytes": 84
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-eval-hermite/eval_hermite.cpp"
 },
 {
  "component": "w8-render-world-a-floor-clear",
  "object_name": "floor_clear.o",
  "emitted_functions": [
   {
    "name": "floorClear__Fv",
    "compiled_body_bytes": 8
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-floor-clear/floor_clear.cpp"
 },
 {
  "component": "w8-render-world-a-get-megatile-center",
  "object_name": "get_megatile_center.o",
  "emitted_functions": [
   {
    "name": "getMegatileCenter__FG6Point2",
    "compiled_body_bytes": 304
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-get-megatile-center/get_megatile_center.cpp"
 },
 {
  "component": "w8-render-world-a-get-megatile-original-center",
  "object_name": "get_megatile_original_center.o",
  "emitted_functions": [
   {
    "name": "getMegatileOriginalCenter__FG6Point2",
    "compiled_body_bytes": 316
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-get-megatile-original-center/get_megatile_original_center.cpp"
 },
 {
  "component": "w8-render-world-a-global-init-edgearray",
  "object_name": "global_init_edgearray.o",
  "emitted_functions": [
   {
    "name": "_ZL21static_initializationii",
    "compiled_body_bytes": 48
   },
   {
    "name": "_GLOBAL_$I$edgearray",
    "compiled_body_bytes": 12
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-global-init-edgearray/global_init_edgearray.cpp"
 },
 {
  "component": "w8-render-world-a-global-init-model-clipping-on",
  "object_name": "global_init_model_clipping_on.o",
  "emitted_functions": [
   {
    "name": "_ZL21static_initializationii",
    "compiled_body_bytes": 8
   },
   {
    "name": "_GLOBAL_$I$modelClippingOn",
    "compiled_body_bytes": 12
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-global-init-model-clipping-on/global_init_model_clipping_on.cpp"
 },
 {
  "component": "w8-render-world-a-global-init-world-tile",
  "object_name": "global_init_world_tile.o",
  "emitted_functions": [
   {
    "name": "_ZL21static_initializationii",
    "compiled_body_bytes": 648
   },
   {
    "name": "_GLOBAL_$I$worldTileEntityLightScale",
    "compiled_body_bytes": 12
   },
   {
    "name": "_GLOBAL_$D$worldTileEntityLightScale",
    "compiled_body_bytes": 12
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-global-init-world-tile/global_init_world_tile.cpp"
 },
 {
  "component": "w8-render-world-a-load-directory",
  "object_name": "load_directory.o",
  "emitted_functions": [
   {
    "name": "loadDirectory__FP9directoryii",
    "compiled_body_bytes": 432
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-load-directory/load_directory.cpp"
 },
 {
  "component": "w8-render-world-a-lump-check",
  "object_name": "lump_check.o",
  "emitted_functions": [
   {
    "name": "lumpCheck__Fv",
    "compiled_body_bytes": 36
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-lump-check/lump_check.cpp"
 },
 {
  "component": "w8-render-world-a-lump-find-by-type",
  "object_name": "lump_find_by_type.o",
  "emitted_functions": [
   {
    "name": "lumpFindByType__FPvPci",
    "compiled_body_bytes": 196
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-lump-find-by-type/lump_find_by_type.cpp"
 },
 {
  "component": "w8-render-world-a-lump-query",
  "object_name": "lump_query.o",
  "emitted_functions": [
   {
    "name": "lumpQuery__FPc",
    "compiled_body_bytes": 228
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-lump-query/lump_query.cpp"
 },
 {
  "component": "w8-render-world-a-lump-unload",
  "object_name": "lump_unload.o",
  "emitted_functions": [
   {
    "name": "lumpUnload__FPv",
    "compiled_body_bytes": 544
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-lump-unload/lump_unload.cpp"
 },
 {
  "component": "w8-render-world-a-megatile-init",
  "object_name": "megatile_init.o",
  "emitted_functions": [
   {
    "name": "megatileInit__FP12MegatileRootPv",
    "compiled_body_bytes": 652
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-megatile-init/megatile_init.cpp"
 },
 {
  "component": "w8-render-world-a-megatile-storage",
  "object_name": "megatile_storage.o",
  "emitted_functions": [],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-megatile-storage/megatile_storage.cpp"
 },
 {
  "component": "w8-render-world-a-render-storage",
  "object_name": "render_storage.o",
  "emitted_functions": [],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-render-storage/render_storage.cpp"
 },
 {
  "component": "w8-render-world-a-run-standard-glow",
  "object_name": "run_standard_glow.o",
  "emitted_functions": [
   {
    "name": "runStandardGlow__8Creature",
    "compiled_body_bytes": 120
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-run-standard-glow/run_standard_glow.cpp"
 },
 {
  "component": "w8-render-world-a-set-dir-light",
  "object_name": "set_dir_light.o",
  "emitted_functions": [
   {
    "name": "setDirLight__Fsssssfb",
    "compiled_body_bytes": 404
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-set-dir-light/set_dir_light.cpp"
 },
 {
  "component": "w8-render-world-a-texture-draw-color",
  "object_name": "texture_draw_color.o",
  "emitted_functions": [
   {
    "name": "_textureDrawColor__FP8_textureiiiUi",
    "compiled_body_bytes": 508
   }
  ],
  "kind": "independently_authored_source",
  "source": "sources/w8-render-world-a-texture-draw-color/texture_draw_color.cpp"
 }
]
```

## Authored but never executed

All of it. None of these bodies has run on the EE, in an emulator or as part of the game; the interpreter comparison and the host checks are the only execution. In particular:

- `draw__8Creature...` and `everyFrame__8Creature...` are verified against the image only through stubbed callees; the real callees (model drawing, hit scans, the attack resolver, light state) are unverified.
- `_textureDrawColor__FP8_textureiiiUi` writes GIF/VIF/DMA words; the stub for `drawTexture` and the draw list is not the real one. DMA timing and cache behavior are not covered.
- `loadDirectory__FP9directoryii` uses a stubbed `sceCdRead`/`sceCdSync`; the real disc read is not exercised.
- The `_GLOBAL_$I$` and `$D$` wrappers are listed in the image's constructor list but nothing here has run the startup chain.
- The empty `asm volatile` loops in `lumpCheck__Fv` and `megatileInit__FP12MegatileRootPv` have no observable effect; the comparison checks that no memory differs, not elapsed time.

## Remaining blockers

- `megatileGenerate__FP12_worldHeader` (above): needs the double-precision helper decision.
- `lumps`: needs a linker-level alias (above).
- `floorAdd__FG6Point3sfii`: needs the `floor-world-view-floor-add` input added to the plan.
- The 47 opened imports belong to other packages (lights, models, floor/world view, game line, effects, particles, net, text, textures and hud storage).
