# w12-visky-a: VI scene, atmosphere, world, sound-track and battle-music methods (A)

Branch `agents/w12-visky-a`. Private reference executable `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Symbol names, addresses and
extents are the image's own symbol table. All 22 symbols of the package are authored, each in its own
directory `experimental/sources/w12-visky-a-*` with a host check (`host_check.cpp`, not a plan input) and
one commit. No unit is claimed as an original-byte match and nothing ran on hardware or in the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and
  compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0).
- Every unit has a host check built natively (`c++ -std=c++17 -ffp-contract=off -Wall -Wextra -Werror`) that
  exits 0. The checks use recording stubs for every callee and recompute expected values from the notes
  at the top of each source. For each unit the checks were mutated (changed constants, comparison
  direction, offsets, counts, call arguments); every mutation that changes behaviour is killed. The
  survivors that remain are equivalent mutants (for example `hour <= 18.0` in `ProcessTransitions`, where
  both sides give 0.0, or the clamp to 1.0 in `RenderStars`, which no input can reach).
- Private paired runs (a small R5900 interpreter outside git): the original and the freshly compiled unit run
  from the same memory snapshot with identical hooked callees; call logs (argument registers, float
  registers, bytes behind pointer arguments) and every changed byte outside the stack are compared. A
  halted run counts as a failure. The scenario counts are in the table. Mutated copies of the sources
  were also run through these comparisons for all small units, `RenderRoom`, `RenderSplashes` and
  `RenderStars` (killed unless named in a commit message).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- No partial build was run for this package (the machine was heavily loaded); the import lists below come from
  `nm -u` of the 22 per-unit objects compared with the shared tip's `unresolved-imports.json` and
  `game-components-partial.o` (827 unresolved before). Compiled bodies and `emitted_functions` sizes are
  from those objects.
- The R5900 has no NaN. Float comparisons in the sources are written the way the original reads them; the
  private paired scenarios of the first five small units include NaN inputs but the host checks do not
  claim NaN behaviour.
- Commit messages cite some scenario counts that were slightly off: `RenderStars` ran 69 paired scenarios (23
  states, three rand scripts), not 57; `ProcessTransitions` ran 160 random plus 120 forced-state seeds, not
  130. The table below has the right numbers.

## Units added (observed, inferred, unknown)

Each source file states its own observed/inferred/unknown split in its leading comment.

| Symbol | Address, extent | Compiled bytes | Paired original/authored runs | What it does |
|---|---|---|---|---|
| `GetTime__7VIScene` | 0x01144a08, 8 | 8 | 2 paired cases | 8-byte 64-bit read at scene+1152 |
| `SetEffectMix__12VIAtmospheref` | 0x0106b5e8, 8 | 8 | 2 | float store at +128 |
| `SetIntensity__12VIAtmospheref` | 0x0106b5f8, 84 | 100 | 25 (sign, zero and NaN combinations) | store at +72; ClearAllSplashes on an off-to-on change |
| `SetLavastormIntensity__12VIAtmospheref` | 0x0106b680, 112 | 132 | 81 | two -1 guards at +4480/+4484; store at +76; ClearAllFireballs on off-to-on |
| `GetOvercastInterp__12VIAtmosphere` | 0x0106b578, 104 | 100 | 32 | overcast weight from the words at +1516/+1520 and the blend float at +1544 |
| `ClearAllSplashes__12VIAtmosphere` | 0x0106b8b0, 176 | 176 | 32 | rebuilds the splash free list from a pool of 36-byte nodes |
| `ClearAllFireballs__12VIAtmosphere` | 0x0106b960, 200 | 200 | 24 | destroys two emitters per active fireball, then rebuilds a 32-record free list |
| `Init__13VIBattleMusicP7VIScene` | 0x0106de60, 52 | 48 | 2 | seven field stores, returns 0 |
| `Free__13VIBattleMusic` | 0x0106de98, 64 | 72 | 2 | Unload then three stores when the scene word is set |
| `ReleaseCollBuffer__9VICollidei` | 0x01097020, 108 | 132 | 120 | reference count, pool erase, destructor; includes the zero-entry-word path that reads and writes address 4 |
| `RenderByVolume__7VIWorldR15VISceneRendVars` | 0x010fe5b0, 108 | 88 | 13 | publishes the counter, calls the mode overload, increments on success |
| `Allocate__14VIQueryTriList` | 0x010d0f20, 176 | 148 | 8 | 40-byte records, nine floats zeroed in each |
| `Init__11VIWorldTreeii` | 0x01100238, 92 | 80 | 9 | node array then leaf array |
| `Clear__11VIWorldTree` | 0x01100298, 80 | 64 | 9 | node array then leaf array |
| `CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2` | 0x01141668, 280 | 240 | 11 (including output aliasing) | two four-term weighted sums with per-term output stores |
| `SetPretranslations__6VIZoneR8VIRasterR8VICamera` | 0x01115828, 204 | 148 | 15 | eye-relative vectors, then the raster call |
| `RenderRoom__6VIZoneiR15VISceneRendVars` | 0x011164d8, 520 | 528 | 62 (all words reached except nops) | portal, sprite and actor passes for one room |
| `RenderSplashes__12VIAtmosphereR7VISceneR8VICamera` | 0x0106ab38, 420 | 472 | 120 | billboard batches over the splash list with age-4 recycling |
| `RenderStars__12VIAtmosphereR7VISceneR8VICamera` | 0x0106a4d0, 1640 | 1620 | 69 (23 states, three rand scripts) | star twinkle, shooting star, second layer |
| `ProcessWeather__12VIAtmosphereR7VISceneR8VICamera` | 0x01068780, 2632 | 1580 | 70 seeds, 656 of 658 original words (two nops missed) | rain drops: camera delta, reseed by ray casts, wind steps, wraps, splash spawn |
| `ProcessTransitions__12VIAtmosphereR7VISceneff` | 0x010691c8, 3672 | 3312 | 160 random seeds plus 120 forced-state seeds, 903 of 918 words (13 nops, one dead delay slot) | sky and weather blend, sprite alpha fades, rain sounds, state advance, lightning timer |
| `RenderShadow__7VIScenei` | 0x0113bed8, 2300 | 1580 | 50 seeds, 573 of 575 words (two nops) | blob shadow onto nearby triangles |

Compiled bytes are the `.text` size of the one-unit object with the reviewed flags, including the
compiler's alignment padding; they are not image sizes. `ProcessWeather` also emits one local helper
(`cast_drop`, 624 bytes); `ProcessTransitions` emits two (`fade_sprite`, `refresh_volumes`).

## Findings worth keeping

Observed in the image unless marked inferred.

- `RenderRoom`: the frame-counter store (`record[36] = frame`) sits in a branch delay slot, so it happens even
  when the sprite id is -1. Only the sprite id equal to -1 skips the drawing.
- `ProcessWeather`: a wrap of the y coordinate does not set the "recast" flag; only wraps of x or z recast.
  The recast and the reseed share one sequence (ray from 5000 above to 5000 below, offset by the stored
  origin, then a random blend of the relative height). The stack objects are a `VICollRay` (96 bytes), a
  `VISceneFilter`-derived filter whose vtable word at filter+16 is `_vt$13WeatherFilter` (0x01204180, 24
  bytes; its method `FilterActor__13WeatherFilterR7VIScenei` is at 0x0106b308) and a `VIEnvRay` (64 bytes);
  the sizes are inferred from the original frame layout. No destructor runs for any of them.
- `ProcessTransitions`: `0x42800000` is 64.0, not 128.0, so the second and third layer alphas use a scale
  of 64. The two sound handles in the atmosphere (`W[4496]`, `W[4500]`) are compared with 0 for "running"
  when stopping but with -1 for "not started" when starting, so a never-started (-1) handle is also passed to
  `Stop`. The 7-entry intensity table (0, .15, .5, 1, .15, .5, 1) is the 28 bytes at 0x011cd760 and is indexed by the raw
  state words without a bounds check.
- `ReleaseCollBuffer`: the buffer pointer is dereferenced (count at +4) before the null test; the authored source
  keeps the pointer as a 32-bit word so the compiler does not fold the later null test away. With a zero
  entry word the original reads and writes address 4.
- `RenderShadow`: the three matrices are zero-filled (the original does it with a loop plus VU0 `vsub`/`sqc2`;
  the authored source uses plain float zeroes) and each gets `_$_10VIMatrix44(matrix, 2)` on every exit. The six
  words copied to filter+16 ({5, 6, 4, 7, 0, 1}, 0x011db500) are node ids passed to
  `CalcSlotWorldTransform`; the last two corners are scaled by 0.5.
- `RenderStars`/`RenderSplashes`: billboards are 48 bytes (float position at +0..+8, a word at +12, two floats at
  +16 and +20, an RGBA float colour at +32..+47); the two functions use the same
  `BeginBillboards`/`EndBillboards` batching with the count remaining as the second argument.
- `GetOvercastInterp`, `ProcessTransitions`: the two small state words at +1516 and +1520 behave as
  previous/next; only the values 0 and 1 are distinguished, so they may be an enum with more members
  (inferred).

## Imports closed

All 22 package symbols: `Allocate__14VIQueryTriList`,
`CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2`, `ClearAllFireballs__12VIAtmosphere`,
`ClearAllSplashes__12VIAtmosphere`, `Clear__11VIWorldTree`, `Free__13VIBattleMusic`,
`GetOvercastInterp__12VIAtmosphere`, `GetTime__7VIScene`, `Init__11VIWorldTreeii`,
`Init__13VIBattleMusicP7VIScene`, `ProcessTransitions__12VIAtmosphereR7VISceneff`,
`ProcessWeather__12VIAtmosphereR7VISceneR8VICamera`, `ReleaseCollBuffer__9VICollidei`,
`RenderByVolume__7VIWorldR15VISceneRendVars`, `RenderRoom__6VIZoneiR15VISceneRendVars`,
`RenderShadow__7VIScenei`, `RenderSplashes__12VIAtmosphereR7VISceneR8VICamera`,
`RenderStars__12VIAtmosphereR7VISceneR8VICamera`, `SetEffectMix__12VIAtmospheref`,
`SetIntensity__12VIAtmospheref`, `SetLavastormIntensity__12VIAtmospheref`,
`SetPretranslations__6VIZoneR8VIRasterR8VICamera`. All 22 appear in the shared tip's 827-name list.

## Imports opened

Names the 22 new objects reference that are neither defined in the shared tip's partial object nor in its
827-name unresolved list (56): `AddSplash__12VIAtmosphereR7VIVect3`, `Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem`, `BeginTriFan__8VIRaster`, `CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44`, `Clear__t7VIArray1Z11VIWorldLeaf`, `Clear__t7VIArray1Z11VIWorldNode`, `Collide__7VISceneR8VIEnvRay`, `Collide__7VISceneR9VICollRayR13VISceneFilter`, `CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary`, `CullByTop__11VIClipStackRC6VIBBox`, `Destroy__17VIParticleEmitter`, `DistanceSquared__C6VIBBoxRC7VIVect3`, `EndTriFan__8VIRaster`, `Erase__t6VIPool1ZP12VICollBufferi`, `GetActorLocation__C7VIScenei`, `GetActorSpritePtr__C7VIScenei`, `GetActorTransform__7VISceneiR10VIMatrix44`, `GetHitPoint__9VICollRay`, `GetHitPoint__C8VIEnvRay`, `GetLayerColor__C10VIMateriali`, `GetMaterialPalEntry__8VIRasterii`, `GetMaterialPalSize__8VIRasteri`, `GetMaterialPal__C14VISimpleSprite`, `Init__6VIBBoxiP7VIVect3`, `Init__8VIEnvRayRC7VIVect3T1Ui`, `Init__9VICollRayRC7VIVect3T1`, `Init__t7VIArray1Z11VIWorldLeafi`, `Init__t7VIArray1Z11VIWorldNodei`, `Play__13VISoundDeviceii12VISOUNDCLASS`, `ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera`, `QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList`, `RasterActor__7VISceneR8VICameraiR11VIClipStack`, `RasterSprite__7VISceneR8VICameraiUi`, `RemHead__7VILList`, `RenderByVolume__7VIWorldiR15VISceneRendVars`, `RenderThroughPortals__6VIZoneiR15VISceneRendVars`, `ResetLightning__12VIAtmosphere`, `Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f`, `SetLayerColor__10VIMaterialiRC9VIColor32`, `SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary`, `SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary`, `SetPan__13VISoundDeviceif`, `SetPitch__13VISoundDeviceii`, `SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3`, `SetVolume__13VISoundDeviceif`, `SetZBias__8VIRasteri`, `Transform__C6VIBBoxRC10VIMatrix44R6VIBBox`, `UV__8VIRasterRC7VIVect2`, `Unload__13VIBattleMusic`, `Vertex__8VIRasterRC7VIVect3`, `_$_12VICollBuffer`, `__13VIActorFilter`, `__13VISceneFilter`, `__8VIEnvRay`, `__9VICollRay`, `_vt$13WeatherFilter`.

Referenced and already unresolved at the shared tip (still unresolved, not new): `AddTail__7VILListP7VILNode`, `BeginBillboards__8VIRasterPP11VIBillboardi`, `Color__8VIRasterRC10VIColor32F`, `EndBillboards__8VIRaster`, `Init__7VILList`, `Invert__10VIMatrix44RC10VIMatrix44`, `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `Remove__7VILNode`, `SetFog__8VIRasteriffRC10VIColor32F`, `SetMaterial__8VIRasteri`, `SetModelView__8VIRasterRC10VIMatrix44`, `SetStaticLighting__8VIRasterRC10VIColor32F`, `Sprite__7VIScenei`, `Stop__13VISoundDevicei`, `cosf`, `sceVu0CopyMatrix`, `sinf`.

Referenced and already defined by the shared tree: `SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode`,
`_$_10VIMatrix44`, `__builtin_vec_new`, `rand`.

`_vt$13WeatherFilter` is a data object (address of its first byte is taken); every other name above is a
function. No new data views were needed: the two small tables the originals copy to the stack
(`{0, .15, .5, 1, .15, .5, 1}` and `{5, 6, 4, 7, 0, 1}`) are written as literals.

## Build-plan entries needed

`docs/agents/w12-visky-a-plan-entries.json` holds one `independently_authored_source` entry per unit (22
entries, `profile` `authored_cpp`, empty `header_closure`, `object_name` `w12_visky_a_<unit>.o`, `sha256` of the
source, and the `emitted_functions` with compiled body bytes). The shared `files` map needs each source path
with the same hash; `input_order` is left to the integrator. The host checks are not plan inputs. None of
the sources includes a local header.

## Authored but never executed

- Everything: no unit has run on the EE or in the original process. The paired runs executed the original and
  the authored code inside a private interpreter with hooked callees; the results depend on the hooks.
- The callees (collision rays, material palette calls, sound device, raster packet builders, list and pool
  templates, `rand`, `sinf`, `cosf`) are stubbed in every check, so their real effects on the data these
  functions read afterwards are untested. `ProcessWeather` and `RenderStars` use scripted random values.
- Stack objects: the sizes and the filter layout used by `ProcessWeather` and `RenderShadow` come from the
  original frame; the constructors run only as hooked no-ops.
- The original `RenderShadow` zero-fills its matrices with VU0 macro instructions; the paired run exercised
  them with the interpreter's two-instruction subset, the authored source uses scalar zeroes.
- State words beyond the values the code distinguishes (`ProcessTransitions`, `GetOvercastInterp`) and
  weather indices 7 and above (the table is not bounds checked) were not run.
- The paired mutation sweeps for `RenderRoom`, `RenderSplashes` and the small units were run; the sweeps for
  `ProcessWeather`, `ProcessTransitions` and `RenderShadow` were run against the host checks only (the
  paired runs for those three passed unmutated with high coverage).

## Unknown

- All member names and the layouts of `VIAtmosphere`, `VIScene`, `VIZone`, `VIWorld`, `VIBattleMusic`,
  `VICollide`, `VIQueryTriList`, the drop, splash and billboard records beyond the offsets used in each source.
- The meaning of most float constants (named only by their bit patterns in the sources) and of the state
  words in `ProcessTransitions`.
- What the leading word of a triangle record in `RenderShadow` is, what the six node ids are, and what
  `Unload__13VIBattleMusic`, `AddSplash`, `ResetLightning`, `ProcessLavastorm` and the other opened callees do.
- Whether `ProcessWeather`'s record word at +32 is a "hit water" flag (its use suggests so).
