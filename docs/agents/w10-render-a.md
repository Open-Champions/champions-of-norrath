# w10-render-a: animation, model, light and render (part 1 of 4)

Branch `agents/w10-render-a`. Private reference executable `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); symbol names,
addresses and extents are the image's own symbol table. Nothing here ran on hardware or inside
the game, and no unit is claimed as an original-byte match.

An earlier interrupted run of this package had already committed six functions and left three
more uncommitted; those were reviewed (compiled, host contract run, paired run repeated) and
committed first. Everything below is one unit per directory `experimental/sources/w10-render-a-*`,
each with a `host_contract.cpp` (a host test, not a plan input).

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable
  C++ and compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0). Every unit compiled.
- Every unit has a host contract built with the host C++ compiler (`-ffp-contract=off` for the
  float-heavy ones) and exits 0.
- Private paired runs (a small R5900 interpreter outside git): the original function and the freshly
  compiled unit run from the same memory snapshot with identical stub callees; the call logs
  (argument registers, float registers, bytes behind pointer arguments), the return value where the
  function returns one, and every changed byte outside the stack are compared. A halted run counts
  as a failure. Scenario counts and mutation checks are in each commit message.
- Instruction coverage of the original was measured for the two largest functions: 769/775 words
  for `animAdvanceAnimationTime` (the rest are nops, an unreachable weight clamp and its delay
  slots) and 677/691 for `drawWorldPolyset` (nops, and a tag rewrite that the function's own tag
  writes cannot reach).
- The shared interpreter copy lacked the pipeline-1 multiply/divide opcodes (`div1`, `mflo1`, ...)
  that `fxCreatureToIce` uses; they were added to this package's private copy only.
- Found while doing this: the EE compiler **truncates decimal float literals toward zero** (it
  models the R5900 FPU). A literal written with fewer digits than the exact value of the wanted
  float can land one unit low. `animAdvanceAnimationTime` therefore uses C++17 hex float literals,
  and `fxRunCreatureFire` carries enough decimal digits; both were checked against the image's
  constant bytes. Anyone adding float constants should compare the compiled `.rodata.cst4` words
  with the image.
- `python3 -m unittest discover -s tools -p 'test_*.py'` was run at the end (result in the final
  message of the task).
- A private plan copy with the 19 units appended (`docs/agents/w10-render-a-plan-entries.py
  --private-root`, kept outside git) was used for one `tools/build_partial.py` run; its result is
  listed at the end of this file.

## Units (observed, inferred, unknown)

Observed means read from the image or checked by the paired run; each source file states its own
inferred/unknown split. Names are the image's symbols; no name was guessed.

| Symbol | Address, extent | Notes |
|---|---|---|
| `boxSetEmpty__FP3Box` | 0x0015a8b8, 36 | box reset |
| `boxAdd__FP3BoxT0` | 0x0015a6b8, 128 | box union |
| `ef1Noise__Fi` | 0x0015a540, 128 | interpolated lookup in `noiseTable` |
| `animSetToEndFullyBlended__FP14AnimationState` | 0x00163c58, 84 | last component to final frame |
| `animGetAnimComponent__FP14AnimationStateP15AnimationHeader` | 0x00163d58, 116 | component lookup by header |
| `animRemoveNonpermanentAnimations__FP14AnimationState` | 0x0015fcb0, 232 | packs components with bit 0x1000 |
| `P_StartTrail__Fiifiiiii` | 0x001828d8, 256 | trail slot allocation (`trail`, `trailSequenceNm`) |
| `P_ExtendTrail__FiG6Point3` | 0x00183dd0, 252 | appends a half-unit point to a 128-entry ring |
| `HUDInit__Fv` | 0x001c40c0, 208 | two engine tasks, four player textures |
| `animGetBoneMatrix__FP14AnimationStateiP8Matrix34` | 0x001627a0, 320 | fixed-point 4.12 rotation, 1/64 translation |
| `animLoadBoneMatricies__FP6PacketP14AnimationState` | 0x001628e0, 268 | result recency list, 2-quadword packet |
| `fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff` | 0x001e5f98, 240 | one effect per shadow point |
| `fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff` | 0x001defe0, 696 | spawns LooseIceChunk props |
| `fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff` | 0x001dace0, 1524 | fire then smoke particles |
| `drawTexture__FR6PacketP8_textureiiiiii` | 0x0016dc60, 1008 | three packet layouts by mode |
| `animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps` | 0x00160830, 3100 | component stepping, events, blending |
| `drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi` | 0x00179b40, 2764 | world polyset packet builder |
| data: `ParticleEmitterTestParticle` 80, `droplet` 80, `castLightColor` 64, `fontGrid` 128, `displayTextDisable` 4, `animLoadBuffer` 1, `bevel` 4 | see `w10-render-a-data-zero/data_zero.cpp` | zero at load; extents from symbol spacing |
| data: `effectModeSize` 40, `defaultHalos` 16, `fxResourceMap` 112 | see `w10-render-a-data-tables/data_tables.cpp` | values read from the image; `effectModeSize` and `defaultHalos` bytes compared with the compiled `.data` |

## Imports closed (by this package's units)

HUDInit__Fv, P_ExtendTrail__FiG6Point3, P_StartTrail__Fiifiiiii, ParticleEmitterTestParticle,
animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps,
animGetAnimComponent__FP14AnimationStateP15AnimationHeader,
animGetBoneMatrix__FP14AnimationStateiP8Matrix34,
animLoadBoneMatricies__FP6PacketP14AnimationState, animLoadBuffer,
animRemoveNonpermanentAnimations__FP14AnimationState,
animSetToEndFullyBlended__FP14AnimationState, bevel, boxAdd__FP3BoxT0, boxSetEmpty__FP3Box,
castLightColor, defaultHalos, displayTextDisable,
drawTexture__FR6PacketP8_textureiiiiii, drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi,
droplet, ef1Noise__Fi, effectModeSize, fontGrid,
fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff,
fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff,
fxResourceMap, fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff.

## Names in the assigned list that were not closed

- `floorAdd__FG6Point3sfii`: already authored by `floor-world-view-floor-add`
  (`experimental/sources/floor-world-view-floor-add/floor_add.cpp`); it needs a plan entry from that
  package, not a second copy here.
- `ballParticle` (0x007d6010) and `candle2Particle`: not defined here on purpose. `ballParticle`
  is declared `static` in `w8-fx-particles-init-fx/static_init.cpp`, which initialises it, while
  `w9-render-math-b-make-explosion` imports it; a second definition here would create a second,
  never-initialised object. The `static` must be dropped in that unit (not this package's file).
  `candle2Particle` is a non-static definition in `w8-spell-static-init-a-torch/torch_static_init.cpp`,
  which is not in the build plan. (The image also has a second, unrelated `ballParticle` at 0x008d6320.)

## Imports opened (new undefined references introduced by these units)

Functions: HUDDrawFrame__Fv, HUDRunFrame__Fv, P_AddParticle__FP11ParticleDefP6Point3T1f,
`__6PhPropG6Point3PcN22liPQ24item11descriptionf`, addEffect__FiG6Point3, blockAlloc__Fi,
drawAddToList__FPviP8_textureP11_drawRecordi, drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi,
drawWorldDynamicLight__FP13PolysetHeader, drawWorldObject__FP12_worldHeaderP13PolysetHeaderi,
drawWorldParticleEmitter__FP13PolysetHeader, engineAddTask__FPFv_viPci, engineGetField__Fv,
evaluateAnimation__FP14AnimationStateb, gameCloseToCamera__FP6Point3, gameIsPaused__Fv,
iatan2__Fff, icos__Fi, isin__Fi, lightSetupAmbientWorld__FP6Packeti,
lightSetupForWorld__FP6PacketP13PolysetHeader, lumpFindResource__FPcT0,
modelGetNmShadowPoints__FP12_modelHeader,
modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f,
playSoundGroup__FP15AnimationHeaderiiP6Point3, soundPlaySequence__FP15AnimationHeaderG6Point3f,
sprintf, texCacheData__FP8_texturei, updateAnimation__FP13animComponent.

Data: _vt$13LooseIceChunk, arList, arListBack, bustAlpha, cFireParticle, cSmokeParticle,
crystalTex, drawFrameNm, eRandState, emergencyResult, engineFullFrameMode, frameNm, glistp,
hudBustCovered, hudNetWaiting, identityResult, levelFlags, noiseTable, soundGroupLimit,
soundLimit, supersampleMode, syncRandState, texturePageLoc, texturePageTable,
textureUncompBufferLocked, trail, trailSequenceNm, worldCameraMatrix, worldLastDrawSucceded,
worldWorldViewMatrix.

Most are owned by other packages (lights, particles, model, sound, engine); several were already
in the unresolved list. None is satisfied by a stub or an alias here.

## Build-plan entries needed

The 19 entries (one per `w10-render-a-*` directory, `object_name` `w10ra_<suffix>.o`, profile
`authored_cpp`, header closure `sources/level-anim-load-common/mem.hpp` where the unit includes it)
are produced exactly by:

```sh
python3 docs/agents/w10-render-a-plan-entries.py --toolchain /path/to/reviewed/ee
```

which prints one JSON entry per unit (hashes, emitted function names and compiled body bytes). With
`--private-root DIR` it also writes a private plan/snapshot/sources copy under `DIR` (outside git)
that `tools/build_partial.py` accepts. Nothing in `experimental/build-plan.json` or
`experimental/snapshot.json` was edited. The data units add no function bodies; the
`animAdvanceAnimationTime`, `drawWorldPolyset`, `fxRunCreatureFire` and `drawTexture` entries are
the large ones.

## Authored but never executed beyond the stubbed paired runs

All 17 functions ran only inside the private interpreter with stub callees, never in the game and
never against the real callees. Specifically:

- `animAdvanceAnimationTime`: sound playback, `updateAnimation`, `iatan2` and the event tables were
  stubs; null `motion`/`turn` pointers are dereferenced in some paths exactly as the image does and
  were never exercised with real callers.
- `drawWorldPolyset`: the lighting and `drawAddToList` callees were stubs; the tag-ID rewrite
  (1 to 6) was not reachable in any run.
- `fxCreatureToIce`: a divide by zero when the shadow count becomes zero inside the loop traps in
  this source, while the image has no trap there; not exercised.
- `animLoadBoneMatricies` and `drawTexture` build packets whose consumers (VIF/GS) were not run.
- The data objects are only extents and values; no authored startup code was run to fill the zero
  words, and `fxResourceMap`'s odd words stay zero.

## Unknown

- What the result record, `emergencyResult` and `identityResult` hold beyond the fields used.
- The meaning of most component flags and header fields in `animAdvanceAnimationTime` beyond their
  arithmetic effect (the names in the source comments are inferences).
- GS/VIF tag layouts behind the numeric constants in `drawTexture`, `animLoadBoneMatricies` and
  `drawWorldPolyset`.
- Whether `defaultHalos` is a table of floats or bytes (its four words are stored verbatim).
- What `FxCreatureState`, the short angle and the unread float arguments of the creature effect
  functions mean.
- The original source file boundaries (e.g. why `ballParticle` and `cFireParticle` are file-local).

## Private partial build result

One `tools/build_partial.py` run with the private plan copy (shared selection plus these 19 units,
3493 inputs, output kept under the ignored `work/`) compiled every unit and linked one relocatable
object: 2501 function bodies, 1325 unresolved imports, no common symbols, no duplicate-definition
error. None of the names closed by this package remains in `unresolved-imports.json`. This is a
partial relocatable link, not an executable or gameplay verification.
