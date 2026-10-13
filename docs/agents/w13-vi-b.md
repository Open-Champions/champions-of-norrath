# w13-vi-b: VI engine rasterizer, scene, world, file, list and font methods (part 2 of 4)

Branch `agents/w13-vi-b`. Private reference executable `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Names, addresses and
extents are the image's own symbol table. All 38 symbols of the package are authored, each in its own
directory `experimental/sources/w13-vi-b-*` with a host contract (`host_contract.cpp`, not a plan input) and
its own commit. No unit is claimed as an original-byte match and nothing ran on hardware or in the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and
  compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0). Compiled sizes differ from the original extents in most units (the original keeps pointer
  fields in memory across stores; the sources do not claim that layout of instructions).
- Every unit has a native host contract (`c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off`, exit 0).
  The 64-bit host cannot lay out 32-bit pointer words, so contracts model pointer fields as 32-bit offsets
  into an arena through the `NORRATH_LOAD_POINTER` / `NORRATH_STORE_POINTER` macros that the sources define
  under `NORRATH_HOST_CONTRACT`; scratchpad accesses go through `NORRATH_SCRATCH_BYTE`. Callees are recording
  stubs. On the EE build the macros expand to the plain pointer accesses.
- Private paired runs (a small R5900 interpreter kept outside git): the original image function and the
  freshly compiled unit run from the same memory snapshot with identical hooked callees; call logs (argument
  registers, floats and bytes behind pointer arguments) and every changed byte outside the stack are compared;
  a halted run counts as a failure. Scenario counts are in the table. Randomized scenarios build coherent
  structures (trees, lists, glyph tables, material pools) and branch conditions are forced explicitly.
  Float edge cases (NaN, out-of-range float-to-unsigned conversions, values exactly on a threshold) are
  included where the code compares or converts floats.
- Mutation checks: each unit was run through the paired comparison with source mutations (changed constants,
  offsets, comparison directions, call arguments, stored values). Every behaviour-changing mutation that
  was tried is detected, except a few equivalent ones: `SetZBias` with `bias >= 0` (a zero bias stores
  0.0f, bit-identical to the integer 0), the sphere zero-fill of `QueryIntersection` (overwritten by the
  hooked `Init`), and the `0x0fffffff` mask of `RenderByVolume` until a large-index scenario was added.
  Some mutations of `QueryEmittersAtLocation` make the walk diverge into an endless list and are detected by
  the run not finishing.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- No full partial build was run (the machine hosts many parallel sessions). Instead the 38 units were
  compiled and linked with `ld -r -m elf32lr5900` (no duplicate definitions), and the exported and
  undefined symbols were compared with the shared tip's `unresolved-imports.json` and
  `game-components-partial.o` (plan13 build). The plan entries below were generated from those objects.

## Units added (observed, inferred, unknown)

Each source file states its own observed / inferred / unknown split in its leading comment.

| Symbol | Address, extent | Compiled bytes | Paired runs | What it does |
|---|---|---|---|---|
| `RemHead__7VILList` | 0x01072838, 64 | 60 | 4 | detaches the first node of a doubly linked list |
| `IsIopStreamingEnabled__6VIFile` | 0x010cade8, 12 | 12 | 5 | reads the static word _6VIFile$sIopStream |
| `Open__6VIFileiPQ26VIFile7MemSegmil` | 0x010caed0, 20 | 8 | 6 | tail-forwards its four arguments to MemorySegmOpen |
| `RenderByVolume__7VIWorldiR15VISceneRendVars` | 0x010fbb48, 644 | 524 | 58 | plane-tree walk ordered by eye side, zone tail call |
| `QueryEmitters__7VIWorldR23VISceneEmitterQueryVars` | 0x010feac0, 108 | 88 | 5 | serial number and mode, advances the serial on success |
| `EndTriFan__8VIRaster` | 0x0110ca88, 260 | 184 | 5 | closes the fan packet, header words and DMA tag |
| `Vertex__8VIRasterRC7VIVect3` | 0x0110d5e0, 240 | 152 | 4 | appends the four vertex entries and counts the vertex |
| `AddMaterial__8VIRasteriPP10_sceDmaTag` | 0x0110dd08, 768 | 812 | 70 | surface caching, packet slot recycling, reference tag |
| `FlushDMABufferIfFull__8VIRaster` | 0x0110fab0, 280 | 296 | 10 | terminates and sends the display list, switches buffers |
| `SetFogRegisters__8VIRaster` | 0x0110fbc8, 504 | 460 | 8 | appends the fog color register packet |
| `UploadMatrices__8VIRasterPP10_sceDmaTag` | 0x01111f50, 804 | 736 | 8 | 23-entry matrix and lighting packet from the scratchpad |
| `UploadRasterMicro__8VIRasterPP10_sceDmaTag` | 0x01112460, 232 | 192 | 4 | raster microcode upload packets |
| `UploadBillboardMicro__8VIRasterPP10_sceDmaTag` | 0x01112548, 196 | 172 | 4 | billboard microcode upload packets |
| `SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3` | 0x01112fb0, 84 | 88 | 5 | common offset and vector table copy |
| `BeginTriFan__8VIRaster` | 0x011131b8, 124 | 176 | 5 | optional microcode and matrix upload, opens the fan cursors |
| `UV__8VIRasterRC7VIVect2` | 0x01113268, 28 | 24 | 5 | stores the texture coordinate pair |
| `GetMaterialPalSize__8VIRasteri` | 0x01113b80, 32 | 32 | 8 | size word of the indexed material record |
| `SetAmbient__8VIRasterRC10VIColor32F` | 0x01113e88, 56 | 44 | 5 | 16-byte copy to the scratchpad plus changed flag |
| `EnableAmbient__8VIRasteri` | 0x01113ec0, 24 | 24 | 6 | scratchpad switch byte plus changed flag |
| `SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight` | 0x01113ed8, 148 | 136 | 5 | stages the light direction and color, rotates and normalizes |
| `EnableSunLight__8VIRasteri` | 0x01113f70, 24 | 24 | 6 | scratchpad switch byte plus changed flag |
| `EnableAllDirLights__8VIRasteri` | 0x01114068, 16 | 16 | 4 | sets the changed flag, ignores the argument |
| `MaxPointLights__8VIRaster` | 0x01114168, 8 | 8 | 3 | returns the constant 1 |
| `EnableTessellation__8VIRasteri` | 0x011141e0, 8 | 8 | 5 | stores the argument at raster+60 |
| `CalcMatrices__8VIRaster` | 0x01114388, 176 | 156 | 3 | derived scratchpad matrices and sun direction |
| `SetZBias__8VIRasteri` | 0x01114438, 84 | 108 | 8 | bias * 166.0f, then CalcClipToScreen |
| `Store2D__8VIRaster` | 0x01114490, 112 | 104 | 3 | saves scratchpad matrices and switches, clears the switches |
| `Restore2D__8VIRaster` | 0x01114500, 96 | 96 | 3 | restores saved 2D matrices and switches to the scratchpad |
| `RenderThroughPortals__6VIZoneiR15VISceneRendVars` | 0x011166e0, 628 | 776 | 60 | portal walk with eye-side and clip tests, recursion into RenderRoom |
| `Cache__7VIRFontR8VIRasterPCciPi` | 0x01134a18, 972 | 1048 | 33 | font glyph cache: LRU slots, texture quads, row upload |
| `Collide__7VISceneR9VICollRayR13VISceneFilter` | 0x0113dfd0, 172 | 132 | 4 | builds the ray-variable block, forwards to the world |
| `RasterActor__7VISceneR8VICameraiR11VIClipStack` | 0x011420c0, 492 | 552 | 16 | per-actor stamp, flag, range and clip culling, then the actor draw slot |
| `QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery` | 0x01143518, 252 | 280 | 7 | leaf lookup, then per-actor emitter query along a list |
| `QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList` | 0x011437f8, 328 | 228 | 4 | builds the 156-byte intersection block, queries the embedded world |
| `RasterSprite__7VISceneR8VICameraiUi` | 0x01144bf8, 100 | 80 | 5 | dispatches the indexed object through its table slot or returns -1 |
| `Collide__7VISceneR8VIEnvRay` | 0x01145478, 60 | 60 | 4 | sets the ray flag, forwards to the world, bumps the scene counter |
| `GetActorTransform__7VISceneiR10VIMatrix44` | 0x011456b8, 44 | 44 | 4 | matrix Transform from two vectors and a float of a record |
| `Init__8VIEnvRayRC7VIVect3T1Ui` | 0x0116d118, 240 | 184 | 5 | start, end, normalized direction, length, reciprocal |

Common conventions: an `int` parameter that the code adds to directly (actor and material records in
`GetActorTransform`, `RasterActor`, `AddMaterial`) is treated as an address; the scene's embedded world is at
scene + 4 and the scene counter at scene + 1348 (`Collide` x2, `QueryIntersection`,
`QueryEmittersAtLocation`); display-list builders take the address of the DMA write pointer (raster + 19492)
and re-read it at each step like the image does.

## Findings worth keeping (inferred unless stated)

- Observed: `UploadMatrices`, `Store2D`, `Restore2D`, `CalcMatrices`, `SetSunLight` and `SetAmbient` exchange
  data through scratchpad addresses 0x70000000..0x700002ff. Bytes 0x70000220 and 0x70000221 are written by
  `EnableAmbient` and `EnableSunLight`; `Store2D` clears them and `Restore2D` restores them. The byte at
  0x70000222 is read (nothing in this package writes it) and gates one extra vector transform in
  `CalcMatrices` and two entries in `UploadMatrices`.
- Observed: `raster + 17888` is set to 1 by `SetAmbient`, `SetSunLight`, `EnableAmbient`, `EnableSunLight`,
  `EnableAllDirLights`, `SetZBias`, `Store2D`, `Restore2D` and `UploadRasterMicro`, and cleared by
  `UploadMatrices`; `raster + 19532` is 1 after the raster microprogram upload and 2 after the billboard one.
- Observed: in `RenderByVolume` zone references carry the bit 0x40000000, and 592-byte records are used both
  for zones and for the per-leaf records in `QueryEmittersAtLocation` (same stride, different pointer fields).
- Observed: the two scene `Collide` wrappers, `QueryIntersection` and `QueryEmittersAtLocation` all add 1 to the
  same scene counter at +1348 (and the `Collide(VIEnvRay)` wrapper also stores 1 at ray + 52).
- Observed: in `RasterActor` the image loads -1 into the result register in a branch delay slot before
  the label that stores it, so when `Add__t8VIVector1Z18VISceneDisplayElem...` succeeds the scene words
  +1092 / +1096 end up as -1 rather than the value of the call; the source keeps that behaviour.

## Imports closed (38)

- `AddMaterial__8VIRasteriPP10_sceDmaTag`
- `BeginTriFan__8VIRaster`
- `Cache__7VIRFontR8VIRasterPCciPi`
- `CalcMatrices__8VIRaster`
- `Collide__7VISceneR8VIEnvRay`
- `Collide__7VISceneR9VICollRayR13VISceneFilter`
- `EnableAllDirLights__8VIRasteri`
- `EnableAmbient__8VIRasteri`
- `EnableSunLight__8VIRasteri`
- `EnableTessellation__8VIRasteri`
- `EndTriFan__8VIRaster`
- `FlushDMABufferIfFull__8VIRaster`
- `GetActorTransform__7VISceneiR10VIMatrix44`
- `GetMaterialPalSize__8VIRasteri`
- `Init__8VIEnvRayRC7VIVect3T1Ui`
- `IsIopStreamingEnabled__6VIFile`
- `MaxPointLights__8VIRaster`
- `Open__6VIFileiPQ26VIFile7MemSegmil`
- `QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery`
- `QueryEmitters__7VIWorldR23VISceneEmitterQueryVars`
- `QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList`
- `RasterActor__7VISceneR8VICameraiR11VIClipStack`
- `RasterSprite__7VISceneR8VICameraiUi`
- `RemHead__7VILList`
- `RenderByVolume__7VIWorldiR15VISceneRendVars`
- `RenderThroughPortals__6VIZoneiR15VISceneRendVars`
- `Restore2D__8VIRaster`
- `SetAmbient__8VIRasterRC10VIColor32F`
- `SetFogRegisters__8VIRaster`
- `SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3`
- `SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight`
- `SetZBias__8VIRasteri`
- `Store2D__8VIRaster`
- `UV__8VIRasterRC7VIVect2`
- `UploadBillboardMicro__8VIRasterPP10_sceDmaTag`
- `UploadMatrices__8VIRasterPP10_sceDmaTag`
- `UploadRasterMicro__8VIRasterPP10_sceDmaTag`
- `Vertex__8VIRasterRC7VIVect3`

## Imports opened

Names the 38 units reference that are not defined by the shared tip's plan13 partial object and are not yet
in its `unresolved-imports.json` (so they are new imports, each belonging to another VI package or to the
image's data): `Begin__Ct6VIPool1Z19VIRasterMaterialLRU`, `Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi`, `CalcClipToScreen__8VIRaster`, `ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3`, `Collide__7VIWorldR18VISceneCollRayVars`, `Collide__7VIWorldR8VIEnvRay`, `ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord`, `CullByTop__11VIClipStackRC8VISphere`, `FindChar__7VIRFontUs`, `Init__8VISphereRC6VIBBox`, `InvertAffine__10VIMatrix44RC10VIMatrix44`, `LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc`, `MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil`, `MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi`, `NormalizeMagn__7VIVect3`, `PushPortal__11VIClipStackR8VICameraiP7VIVect3`, `QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery`, `QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars`, `QueryIntersection__7VIWorldR20VISceneIntersectVars`, `RenderByVolume__6VIZoneR15VISceneRendVarsi`, `SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag`, `Store__C6VIBBoxP7VIVect3`, `SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3`, `Surface__8VIRasteri`, `Transform__10VIMatrix44RC7VIVect3fT1`, `UnlockMipLevel__9VISurfacei`, `UploadPerMaterial__8VIRasterPP10_sceDmaTag`, `VIDMABillboardMicrocode`, `VIDMARasterMicrocode`, `VIEEDMASend__FiPv`, `_6VIFile$sIopStream`, `_8VIRaster$PrimBuffer1`, `_8VIRaster$PrimBuffer2`, `_8VIRaster$RFanOffset`, `_8VIRaster$RInitOffset`.

Already unresolved at the shared tip and still so (not new): `Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem`, `Flip__8VIRaster`, `SetModelView__8VIRasterRC10VIMatrix44`, `SetStaticLighting__8VIRasterRC10VIColor32F`.
Already provided by the shared plan (resolved after linking, nothing to do): `FindLeaf__C7VIWorldRC7VIVect3RiT2`, `FlushCache`, `GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `Normalize__7VIVect3`, `Pop__11VIClipStack`, `RenderByFrustum__7VIWorldR15VISceneRendVars`, `RenderRoom__6VIZoneiR15VISceneRendVars`, `SurfCache`, `VIEEDMASync__Fi`, `memcpy`, `sceVu0CopyMatrix`.

Data symbols referenced (type assumed, not established): `_6VIFile$sIopStream` (int), `_8VIRaster$PrimBuffer1`,
`_8VIRaster$PrimBuffer2`, `_8VIRaster$RFanOffset`, `_8VIRaster$RInitOffset` (unsigned int, static class
members, 4 bytes each), `VIDMARasterMicrocode`, `VIDMABillboardMicrocode` (only their addresses are used) and
`SurfCache` (only its address is passed). The sources import them with `__asm__("...")` labels for the names
containing `$`.

## Build-plan entries needed

All 38 entries, in snapshot order after the current last input, are in
[w13-vi-b-plan-entries.json](w13-vi-b-plan-entries.json) (profile `authored_cpp`, empty header closure, one
source per entry, SHA-256 of the source, compiled body bytes from the objects checked above). `host_contract.cpp`
files must not be planned (they use host headers); `snapshot.json` must record the 38 sources before
`tools/check_source_snapshot.py` accepts the tree. This note does not edit either file.

## Authored but never executed

- Every unit was executed only on the private R5900 interpreter (paired against the original with hooked
  callees) and natively on the host. None ran on hardware, in the game, or together with its real callees.
- Callee behaviour is unverified in every unit: the paired runs replace each callee by a hook, so only the
  arguments and the bytes behind them are compared. Return types of many callees are assumed `int`.
- Display-list builders (`BeginTriFan`, `EndTriFan`, `Vertex`, `Upload*Micro`, `UploadMatrices`, `SetFogRegisters`,
  `FlushDMABufferIfFull`, `AddMaterial`) were checked for the bytes they write; DMA/GS/VIF semantics, cache
  flushes (`FlushCache`) and the start of the transfer (`VIEEDMASend`) were not executed.
- Scratchpad accesses were modelled as ordinary memory at 0x70000000 in the private runs.
- `RenderByVolume`, `RenderThroughPortals`, `RasterActor` and `Cache` (font) depend on structure layouts
  inferred from field accesses; whether real scenes satisfy the assumed invariants (list end markers,
  bounded loops, valid indices) is not shown.

## Unknown

- Member and type names for all records (rooms, portals, zone/leaf records, material pool items, glyph and slot
  records, display elements). None are named beyond what the image's symbol names state.
- The meaning of the constants 166.0 (`SetZBias`), 821 and 0x9200 (`FlushDMABufferIfFull`), 0.02 (`RenderThroughPortals`
  near-plane epsilon), and of the VIF/GIF words written by the packet builders beyond their bit patterns.
- Whether `RenderThroughPortals`' portal flag bit 0 and the room record's bit 0 mean what the notes in the source
  suggest (a path marker and a visited/frustum-only marker).
- Whether the `VIFile` memory-segment `Open` and `IsIopStreamingEnabled` are used with values other than 0 and 1.
- Exact types of `VIPixelStore`, `VIQWord`, `VIPlane` and the vector class templates; the sources use opaque pointers.
