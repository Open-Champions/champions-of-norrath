# Prim and collision buffers, collision queries, vector and matrix helpers (part 1)

Package `w16-prim-a`, branch `agents/w16-prim-a`. All 36 assigned imports are closed by 36 independently authored
units under `experimental/sources/w16-prim-a-*`, one commit each (two later commits only correct a byte count recorded in
a source comment). Addresses and sizes were read from the pinned private image `work/private/SLUS_205.65` with the
reviewed EE binutils, using a private symbol dump; nothing from the image is committed. Every source starts with an
Observed / Inferred / Unknown comment. Fresh compilations are not original-byte matches, and no byte match is claimed.

## Functions

| source (under `experimental/sources/`) | symbol | image address, size | compiled bytes |
|---|---|---|---|
| `w16-prim-a-collbuffer-begin/begin.cpp` | `Begin__12VICollBufferi` | `0x0108bb38`, 116 bytes | 80 |
| `w16-prim-a-collbuffer-begin-prim-group/begin_prim_group.cpp` | `BeginPrimGroup__12VICollBuffer` | `0x0108baf0`, 44 bytes | 44 |
| `w16-prim-a-collbuffer-calc-bvolumes/calc_bvolumes.cpp` | `CalcBVolumes__12VICollBufferiP7VIVect3` | `0x0108b390`, 1152 bytes | 940 |
| `w16-prim-a-collbuffer-end/end.cpp` | `End__12VICollBuffer` | `0x0108b2e0`, 172 bytes | 136 |
| `w16-prim-a-collbuffer-end-prim-group/end_prim_group.cpp` | `EndPrimGroup__12VICollBuffer` | `0x0108bb20`, 20 bytes | 20 |
| `w16-prim-a-collbuffer-flora-type/flora_type.cpp` | `FloraType__12VICollBufferSc` | `0x0108bc40`, 12 bytes | 12 |
| `w16-prim-a-collbuffer-init/init.cpp` | `Init__12VICollBuffer16VICollBufferTypeiii` | `0x0108b950`, 144 bytes | 144 |
| `w16-prim-a-collbuffer-lock/lock.cpp` | `Lock__12VICollBuffer` | `0x0108ba60`, 32 bytes | 32 |
| `w16-prim-a-collide-coll-buffer/coll_buffer.cpp` | `CollBuffer__9VICollidei` | `0x01096fd0`, 28 bytes | 28 |
| `w16-prim-a-collide-create-coll-buffer/create_coll_buffer.cpp` | `CreateCollBuffer__9VICollidei` | `0x01096f40`, 140 bytes | 148 |
| `w16-prim-a-collide-ray-buffer/ray_buffer.cpp` | `Collide__9VICollideR9VICollRayR12VICollBuffer` | `0x01097178`, 104 bytes | 72 |
| `w16-prim-a-collide-sphere-buffer/sphere_buffer.cpp` | `Collide__9VICollideR12VICollSphereR12VICollBuffer` | `0x01097110`, 104 bytes | 72 |
| `w16-prim-a-collide-sphere-capsule/sphere_capsule.cpp` | `Collide__9VICollideR12VICollSphereR9VICapsule` | `0x01095170`, 988 bytes | 900 |
| `w16-prim-a-collray-init/init.cpp` | `Init__9VICollRayRC9VICollRayRC10VIMatrix44` | `0x0109bff0`, 468 bytes | 324 |
| `w16-prim-a-collsphere-init/init.cpp` | `Init__12VICollSphereRC12VICollSphereRC10VIMatrix44` | `0x010a0538`, 548 bytes | 372 |
| `w16-prim-a-groupsprite-calc-member-transform/calc_member_transform.cpp` | `CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44` | `0x0101dc40`, 60 bytes | 52 |
| `w16-prim-a-pickseg-init/init.cpp` | `Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44` | `0x010bf4e0`, 400 bytes | 284 |
| `w16-prim-a-primbuffer-begin/begin.cpp` | `Begin__12VIPrimBufferi` | `0x01101390`, 288 bytes | 264 |
| `w16-prim-a-primbuffer-begin-material/begin_material.cpp` | `BeginMaterial__12VIPrimBufferi` | `0x01102c48`, 80 bytes | 80 |
| `w16-prim-a-primbuffer-calc-bvolumes/calc_bvolumes.cpp` | `CalcBVolumes__12VIPrimBufferiP7VIVect3` | `0x01102140`, 1240 bytes | 1020 |
| `w16-prim-a-primbuffer-color/color.cpp` | `Color__12VIPrimBufferR9VIColor32` | `0x01102da0`, 32 bytes | 48 |
| `w16-prim-a-primbuffer-end/end.cpp` | `End__12VIPrimBuffer` | `0x01101ca0`, 1184 bytes | 488 |
| `w16-prim-a-primbuffer-end-material/end_material.cpp` | `EndMaterial__12VIPrimBuffer` | `0x01102c98`, 52 bytes | 52 |
| `w16-prim-a-primbuffer-init/init.cpp` | `Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii` | `0x01102a10`, 136 bytes | 136 |
| `w16-prim-a-primbuffer-lock/lock.cpp` | `Lock__12VIPrimBuffer` | `0x01102b68`, 32 bytes | 32 |
| `w16-prim-a-primbuffer-normal/normal.cpp` | `Normal__12VIPrimBufferR7VIVect3` | `0x01102d50`, 40 bytes | 40 |
| `w16-prim-a-raster-create-prim-buffer/create_prim_buffer.cpp` | `CreatePrimBuffer__8VIRasteri` | `0x011132e0`, 140 bytes | 148 |
| `w16-prim-a-raster-cull/cull.cpp` | `Cull__8VIRasterR13VIPickSegmentR6VIBBox` | `0x010f9e18`, 224 bytes | 152 |
| `w16-prim-a-raster-draw-primbuffer-packvu/draw_packvu.cpp` | `DrawPrimBufferPackVU__8VIRasteriifUi` | `0x011117e8`, 476 bytes | 464 |
| `w16-prim-a-raster-draw-primbuffer-packvunbw/draw_packvunbw.cpp` | `DrawPrimBufferPackVUNBW__8VIRasteriiUi` | `0x01111620`, 456 bytes | 448 |
| `w16-prim-a-raster-draw-primbuffer-packvunc/draw_packvunc.cpp` | `DrawPrimBufferPackVUNC__8VIRasteriiUi` | `0x01110358`, 628 bytes | 592 |
| `w16-prim-a-raster-draw-primbuffer-packvunc3/draw_packvunc3.cpp` | `DrawPrimBufferPackVUNC__8VIRasteriiiUi` | `0x011119c8`, 672 bytes | 640 |
| `w16-prim-a-raster-draw-primbuffer-packvuncg/draw_packvuncg.cpp` | `DrawPrimBufferPackVUNCG__8VIRasteriiUi` | `0x011105d0`, 532 bytes | 528 |
| `w16-prim-a-raster-draw-primbuffer-vunc/draw_vunc.cpp` | `DrawPrimBufferVUNC__8VIRasteriiUi` | `0x011101b8`, 416 bytes | 420 |
| `w16-prim-a-zone-collide-env-ray/collide_env_ray.cpp` | `Collide__6VIZoneR8VIEnvRayR7VIVect3T2` | `0x01119770`, 112 bytes | 52 |
| `w16-prim-a-zone-collide-scene-vars/collide_scene_vars.cpp` | `Collide__6VIZoneR18VISceneCollRayVars` | `0x01115dc8`, 164 bytes | 152 |

## How each function was checked

* Compiled with the reviewed EE g++ and the plan's `authored_cpp` flags (single units first, then the whole private plan, see
  "Measured effect").
* Each unit has a `host_check.cpp` (not a plan input) that returns nonzero when a checked call order, argument, stored
  value, branch or result is wrong. Build and run natively, for example
  `c++ -std=c++17 -Wall -o /tmp/chk experimental/sources/w16-prim-a-<unit>/host_check.cpp && /tmp/chk`. Objects store 32-bit
  target addresses, so checks that follow pointer words use the `NORRATH_HOST_ARENA` convention (an arena of bytes, pointer words
  hold arena offsets; the draw checks also supply a scratchpad array). Native arm64 was used.
* Each function was also run against the original in the private R5900 interpreter (outside git): identical arguments and
  memory snapshot, callees replaced by identical Python stubs that log their arguments (contents, not stack addresses),
  then comparing the call log, every changed byte outside the stack (this includes the static words in the image and the
  scratchpad) and the return value. A run that halts counts as a mismatch. Scenario counts: Lock 40 each, Normal 40, Color 40,
  FloraType 40, EndPrimGroup 40, BeginPrimGroup 40, CalcMemberTransform 40, CollBuffer 40, Collide (sphere/ray against buffer) 30
  each, VICollBuffer Begin/Init/CreateCollBuffer 48 each, BeginMaterial/EndMaterial 48 each, VIPrimBuffer Init/CreatePrimBuffer 48
  each, VIZone Collide (scene vars) 60 and (environment ray) 24, Cull 800, the three Init-with-matrix functions 24 each,
  VICollBuffer End 60, VIPrimBuffer Begin 66 and End 110, the two CalcBVolumes 96 and 100, sphere/capsule Collide 160, and the
  six draw functions 80 to 120 each. The original's instruction coverage was 100 percent of reachable words in every run; the
  uncovered words are alignment `nop`s and one branch-delay `nop` (listed in the paired-run output, not committed).
* Every function had at least one deliberate mutation of the authored source (a stored offset, comparison, constant, order of two
  stores, a substituted field or an omitted store), usually several, and the paired comparison failed each time. Exceptions:
  two mutants of the capsule routine were equivalent by construction (a buffer that holds identical contents) and were not counted;
  one mutant of `VICollBuffer::CalcBVolumes` (point count read from the wrong halfword) made the loop run too long to finish and was
  killed, and a few regular-expression mutants that spanned lines did not apply (the run printed no change) and were repeated.
* The tools' unit tests pass (`python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests).

## What the structures look like (observed unless marked)

Offsets are bytes. "word" is a 32-bit value; pointer words are stored as 32-bit addresses.

* `VICollBuffer`: type at +8 (Init stores 2 as 3; every routine here accepts 0, 1 or 3 and returns -1 otherwise), lock flag at +12
  (`Lock` sets it only when it was 0), float at +16 (scale of packed vertices), own bounding box at +20 (min xyz) and +32 (max xyz),
  group count at +44 (and `BeginPrimGroup`/`EndPrimGroup` index and advance the same word), current entry index at +52, three
  creation integers at +48, +56 and +64, group records (32 bytes) at the pointer +68, entry records (28 bytes: halfword at 0, point
  count as a halfword at 2, a box in the floats at 4..24) at the pointer +72, vertex data at the pointer +76, a byte at +81 set by
  `FloraType`, and the allocated block base at +88 (Init adds it to the three section pointers). Group records: entry count at 0,
  a running total at 4, box at 8..28 (min xyz, then max xyz). `Begin` zeroes the entry's halfword at 2, `End` adds it to the group
  total and counts the entry; `CalcBVolumes` fills the boxes from three vertex types (plain floats at stride 12, packed halfwords at
  stride 6 times the scale, packed halfwords with a signed byte table index at stride 8).
* `VIPrimBuffer`: type at +8 (accepted by Begin, End and CalcBVolumes: 0, 2, 4, 5, 8), lock flag at +12, own box at +20/+32,
  four floats at +48 (the first is the scale used by `CalcBVolumes`; the draw routines copy all four to scratchpad offset 0x210),
  material index at +64 (BeginMaterial/EndMaterial; the draw routines read it as the material count), entry counter at +72,
  cursor at +80 (index into the qword stream), 16-byte material records at the pointer +88 (material id at 0, running counter at 4,
  stream offset and length in qwords at 8 and 12), 28-byte run records at the pointer +92 (point count at word 0, box at 4..24),
  qword stream at the pointer +96, allocated block base at +104. `Begin` reserves two header qwords at the cursor and remembers
  the cursor in the static word `_12VIPrimBuffer$PrimVQStart`; `End` writes a tag word and the count there, appends a pointer-like
  word that comes from one of five static words (`_8VIRaster$RStripOffset`, 2, 3, 4, 6), and counts the primitive in the current
  material. `Normal` and `Color` store into the statics `_12VIPrimBuffer$PrimNormal` (3 floats) and `$PrimColor` (4 bytes).
* Draw routines: the raster keeps a table of primitive buffers (12-byte entries) at +19328, material-set pointers (12-byte entries)
  at +19436, another table at +19364, a DMA tag write cursor at +19492 and a matrix-dirty word at +17888. For each material of a
  buffer, a material pointer is looked up (material set word 12, indexed by the record's id); a nonzero word at material +60 sets the
  routine's result to 1 and selects flag bit 2, a zero word selects flag bit 1; a drawn material calls `AddMaterial` then writes a
  16-byte EE DMA tag at the cursor (ID byte 0x30, or 0x50 for the run-list form) pointing at the stream. Inferred: the tag layout
  is the EE DMA tag (halfword count, ID byte 3, address word 1). Unknown: the meaning of the flags and of the material word 60.
* Collision probes: `VICollRay`, `VICollSphere` and `VIPickSegment` initialisers move two end points by the matrix with
  `__ml__FRC7VIVect3RC10VIMatrix44` (result four lanes, fourth dropped), transform a direction as a w = 0 vector (ray and sphere,
  `Normalize__7VIVect4` after) or with `SubMatrixMul` (segment, `Normalize__7VIVect3` after), then call a `Precalc` routine. The
  sphere's radius at +88 is scaled by the length of the matrix's first row.
* `Collide(VICollSphere, VICapsule)` is described in the source: outside the grown capsule it runs the side-wall test and both cap
  spheres; inside it picks the nearest cap or the side wall.

## Measured effect (private plan copy, outside git)

`docs/agents/w16-prim-a-plan-entries.py --private-root DIR` writes a private plan with the 36 entries appended. One full partial
build of that plan (7,363 selected inputs, one link) completed: 5,870 function bodies, 840 unresolved imports. The older shared
integration build `work/plan16-build` has 841. All 36 assigned names are present in that list and absent from the new one;
35 new names appear (listed below), so the net change is 841 to 840. The count is not an official post-merge figure: other
packages of this wave merge in parallel.

## Imports closed

`Begin__12VICollBufferi`, `BeginPrimGroup__12VICollBuffer`, `CalcBVolumes__12VICollBufferiP7VIVect3`, `End__12VICollBuffer`, `EndPrimGroup__12VICollBuffer`, `FloraType__12VICollBufferSc`, `Init__12VICollBuffer16VICollBufferTypeiii`, `Lock__12VICollBuffer`, `CollBuffer__9VICollidei`, `CreateCollBuffer__9VICollidei`, `Collide__9VICollideR9VICollRayR12VICollBuffer`, `Collide__9VICollideR12VICollSphereR12VICollBuffer`, `Collide__9VICollideR12VICollSphereR9VICapsule`, `Init__9VICollRayRC9VICollRayRC10VIMatrix44`, `Init__12VICollSphereRC12VICollSphereRC10VIMatrix44`, `CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44`, `Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44`, `Begin__12VIPrimBufferi`, `BeginMaterial__12VIPrimBufferi`, `CalcBVolumes__12VIPrimBufferiP7VIVect3`, `Color__12VIPrimBufferR9VIColor32`, `End__12VIPrimBuffer`, `EndMaterial__12VIPrimBuffer`, `Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii`, `Lock__12VIPrimBuffer`, `Normal__12VIPrimBufferR7VIVect3`, `CreatePrimBuffer__8VIRasteri`, `Cull__8VIRasterR13VIPickSegmentR6VIBBox`, `DrawPrimBufferPackVU__8VIRasteriifUi`, `DrawPrimBufferPackVUNBW__8VIRasteriiUi`, `DrawPrimBufferPackVUNC__8VIRasteriiUi`, `DrawPrimBufferPackVUNC__8VIRasteriiiUi`, `DrawPrimBufferPackVUNCG__8VIRasteriiUi`, `DrawPrimBufferVUNC__8VIRasteriiUi`, `Collide__6VIZoneR8VIEnvRayR7VIVect3T2`, `Collide__6VIZoneR18VISceneCollRayVars`

## Imports opened

Functions, called from the new units and still undefined: `Add__t6VIPool1ZP12VICollBufferRCP12VICollBuffer`,
`Add__t6VIPool1ZP12VIPrimBufferRCP12VIPrimBuffer`, `Allocate__12VICollBuffer`, `Allocate__12VIPrimBuffer`,
`CalcDataSize__12VICollBuffer`, `CalcDataSize__12VIPrimBufferiii`, `CollideBufferV__9VICollideR12VICollSphereR12VICollBuffer`,
`CollideBufferPackV__9VICollideR12VICollSphereR12VICollBuffer`, `CollideBufferPackVGF__9VICollideR12VICollSphereR12VICollBuffer`,
`CollideBufferV__9VICollideR9VICollRayR12VICollBuffer`, `CollideBufferPackV__9VICollideR9VICollRayR12VICollBuffer`,
`CollideBufferPackVGF__9VICollideR9VICollRayR12VICollBuffer`, `CollideNoCaps__9VICollideR9VICollRayRC10VICylinder`,
`Collide__9VICollideR9VICollRayRC8VISphere`, `Collide__6VIZoneiR18VISceneCollRayVars`, `Collide__6VIZoneiR8VIEnvRayR7VIVect3T3`,
`DrawPrimBufferTessPackVUNCG__8VIRasteriiUi`, `Intersects__C6VIBBoxRC8VISphere`, `PointInSolid__10VICylinderRC7VIVect3`,
`PointInSolid__9VICapsuleRC7VIVect3`, `Precalc__12VICollSphere`, `Precalc__13VIPickSegment`,
`SetPreTranslations__9VICollideiP7VIVect3`, `Union__6VIBBoxRC6VIBBoxT1`, `UploadPreTrans__8VIRasterPP10_sceDmaTag`,
`__12VICollBuffer` (constructor), `__12VIPrimBuffer` (constructor).
Data: `_12VIPrimBuffer$PrimColor`, `_12VIPrimBuffer$PrimNormal`, `_12VIPrimBuffer$PrimVQStart`, `_8VIRaster$RStripOffset`,
`_8VIRaster$RStrip2Offset`, `_8VIRaster$RStrip3Offset`, `_8VIRaster$RStrip4Offset`, `_8VIRaster$RStrip6Offset`.
Referenced and already defined by other packages in the shared plan (not opened): `Bound__6VIBBoxiRC7VIVect3`,
`Transform__10VIMatrix44RC7VIVect3fT1`, `__ml__FRC7VIVect3RC10VIMatrix44`, `__ml__FRC7VIVect4RC10VIMatrix44`,
`Normalize__7VIVect3`, `Normalize__7VIVect4`, `Precalc__9VICollRay`, `SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3`,
`FlushDMABufferIfFull__8VIRaster`, `CalcMatrices__8VIRaster`, `UploadMatrices__8VIRasterPP10_sceDmaTag`,
`AddMaterial__8VIRasteriPP10_sceDmaTag`, `__builtin_new`. Several of these (the Collide buffer walkers, Allocate/CalcDataSize, the
pool Add, the Precalc routines, the tessellated draw) are sizeable functions that belong to a sibling prim/collision package.

## Build-plan entries needed

Run `python3 docs/agents/w16-prim-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` to regenerate them and to
write a private plan/snapshot copy. The 36 entries (append in this order; `input_order`, the file digests and the
`plan`/`snapshot` hashes are for the integrator to assign; the `sha256` values are of the final committed source files):

```json
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "Begin__12VICollBufferi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_begin.o", "profile": "authored_cpp", "sha256": "354b7a439399080c00605db2241786d38cc4365e6a676d1ca450f36a83aa3908", "source": "sources/w16-prim-a-collbuffer-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "BeginPrimGroup__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_begin_prim_group.o", "profile": "authored_cpp", "sha256": "53296530a5d7a29b6f8590a79d0618efb74bbfa2e1f25aab605ed410bb0d249b", "source": "sources/w16-prim-a-collbuffer-begin-prim-group/begin_prim_group.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 940, "name": "CalcBVolumes__12VICollBufferiP7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_calc_bvolumes.o", "profile": "authored_cpp", "sha256": "bc30aade706b0b2eed05b1c96f6daa21913d97d0087a2b41b61f82b1cb1cf56a", "source": "sources/w16-prim-a-collbuffer-calc-bvolumes/calc_bvolumes.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "End__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_end.o", "profile": "authored_cpp", "sha256": "cbcca3e2950245c6fb91102b4d0b35039ee55b675f0a6edf7015b83dc38d8927", "source": "sources/w16-prim-a-collbuffer-end/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "EndPrimGroup__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_end_prim_group.o", "profile": "authored_cpp", "sha256": "dbe728f87120fd84c5d4876751e242c7f710057d8c4f05b61bdb531f0bfafe00", "source": "sources/w16-prim-a-collbuffer-end-prim-group/end_prim_group.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "FloraType__12VICollBufferSc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_flora_type.o", "profile": "authored_cpp", "sha256": "30f100763dac4719c06cb9d56c35bf0adfa96b083f10844e5214b77f429e6550", "source": "sources/w16-prim-a-collbuffer-flora-type/flora_type.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "Init__12VICollBuffer16VICollBufferTypeiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_init.o", "profile": "authored_cpp", "sha256": "4455df2ca28e7bee198ab1d148452349b9df2b98dabf0deb703e250b7367b7cd", "source": "sources/w16-prim-a-collbuffer-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Lock__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collbuffer_lock.o", "profile": "authored_cpp", "sha256": "140febe13d8e165fdc9d85e5035283b10383e8b6ee708ac59d5963f09f061857", "source": "sources/w16-prim-a-collbuffer-lock/lock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "CollBuffer__9VICollidei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collide_coll_buffer.o", "profile": "authored_cpp", "sha256": "8ae068f630be57d711989f7296f06f5235789bd06ef605b1dc20c909319e8d4c", "source": "sources/w16-prim-a-collide-coll-buffer/coll_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "CreateCollBuffer__9VICollidei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collide_create_coll_buffer.o", "profile": "authored_cpp", "sha256": "631a65c81d037232ffe4bb2091588c642150341ba9cdba7e3c9e485d2b612a6a", "source": "sources/w16-prim-a-collide-create-coll-buffer/create_coll_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Collide__9VICollideR9VICollRayR12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collide_ray_buffer.o", "profile": "authored_cpp", "sha256": "d1a9b78291d6e39a494484c704eee4976c4397dd6e30c1af2a96dcb89be8aa11", "source": "sources/w16-prim-a-collide-ray-buffer/ray_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Collide__9VICollideR12VICollSphereR12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collide_sphere_buffer.o", "profile": "authored_cpp", "sha256": "c11b83c63f2ce02ca10499a97d4e36ccf20806acba6d30246a0a5c44bd6313b4", "source": "sources/w16-prim-a-collide-sphere-buffer/sphere_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 900, "name": "Collide__9VICollideR12VICollSphereR9VICapsule"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collide_sphere_capsule.o", "profile": "authored_cpp", "sha256": "ab984166b65b728cd426ab53f1ab35ad3d561efef519e0f0b7fa6619c8b5f029", "source": "sources/w16-prim-a-collide-sphere-capsule/sphere_capsule.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "Init__9VICollRayRC9VICollRayRC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collray_init.o", "profile": "authored_cpp", "sha256": "14871732065a74712daafb5c8111e0686d4677ba7d4110471edf62825e4879ba", "source": "sources/w16-prim-a-collray-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 372, "name": "Init__12VICollSphereRC12VICollSphereRC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_collsphere_init.o", "profile": "authored_cpp", "sha256": "3f1d99dc96babc04c126a68b60b8cd3a0b92e17575687428f2f200b92cde305d", "source": "sources/w16-prim-a-collsphere-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_groupsprite_calc_member_transform.o", "profile": "authored_cpp", "sha256": "48dadb015d6abaa29e7197d253a6d3d7047c027602e1a4928da3f69d9dbec7ff", "source": "sources/w16-prim-a-groupsprite-calc-member-transform/calc_member_transform.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 284, "name": "Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_pickseg_init.o", "profile": "authored_cpp", "sha256": "1362fb6ed417f99157318b154e0ffc195177427b3a728b3a3106682857d4fb43", "source": "sources/w16-prim-a-pickseg-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 264, "name": "Begin__12VIPrimBufferi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_begin.o", "profile": "authored_cpp", "sha256": "4b66b8e7a96cd0521220d46d5b5c499868f83e660b994cd9a87ca1c865204dea", "source": "sources/w16-prim-a-primbuffer-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "BeginMaterial__12VIPrimBufferi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_begin_material.o", "profile": "authored_cpp", "sha256": "10548a44fe913d7490b25883032b42f23f9dc6ec625492b532f10f975c014f8a", "source": "sources/w16-prim-a-primbuffer-begin-material/begin_material.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1020, "name": "CalcBVolumes__12VIPrimBufferiP7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_calc_bvolumes.o", "profile": "authored_cpp", "sha256": "50c5d4ca57bffec956bbb626f5aceae5ce4288e87475eccfca0982e95429fedc", "source": "sources/w16-prim-a-primbuffer-calc-bvolumes/calc_bvolumes.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "Color__12VIPrimBufferR9VIColor32"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_color.o", "profile": "authored_cpp", "sha256": "ef0cd44b058274e98679323ce070c70f02165cd93f3bfae651e0f4b6c71da856", "source": "sources/w16-prim-a-primbuffer-color/color.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 488, "name": "End__12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_end.o", "profile": "authored_cpp", "sha256": "1e8a4524e149316d94bde52220f13509196517e63d9e1b0b3f147c3c00b8ebb1", "source": "sources/w16-prim-a-primbuffer-end/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "EndMaterial__12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_end_material.o", "profile": "authored_cpp", "sha256": "de04dc3aaf337738e6de9375c30db67bb5a3e41d92ec30c1715e217b93e171c0", "source": "sources/w16-prim-a-primbuffer-end-material/end_material.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_init.o", "profile": "authored_cpp", "sha256": "aa8f24b41e386a229f2d73aec6ba6b65ac27475e7d0827e4eb26a3534705ed29", "source": "sources/w16-prim-a-primbuffer-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Lock__12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_lock.o", "profile": "authored_cpp", "sha256": "f9aeee36b2aea37ebf8c5c5fbd434e9fddf92b51cdc40bff21d40777a73b15ca", "source": "sources/w16-prim-a-primbuffer-lock/lock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "Normal__12VIPrimBufferR7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_primbuffer_normal.o", "profile": "authored_cpp", "sha256": "1dcd180f627a188fb4476bb7b06c63e06a6935f8e9f1ed2e2797814714bc04fd", "source": "sources/w16-prim-a-primbuffer-normal/normal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "CreatePrimBuffer__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_create_prim_buffer.o", "profile": "authored_cpp", "sha256": "cd813379d7ae4e8aab1e70835b8fb345d895f5e90e045fc5cb6fdecdb2a33db4", "source": "sources/w16-prim-a-raster-create-prim-buffer/create_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "Cull__8VIRasterR13VIPickSegmentR6VIBBox"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_cull.o", "profile": "authored_cpp", "sha256": "84d605cabcccdc4979e73fd87bd420d43634c2121fa380603a273da7b9dfafe9", "source": "sources/w16-prim-a-raster-cull/cull.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 464, "name": "DrawPrimBufferPackVU__8VIRasteriifUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_packvu.o", "profile": "authored_cpp", "sha256": "c2f2bdf43c4a4e1905cd401000c5f324ecd8d9adef93a8c6216740ecc7c59627", "source": "sources/w16-prim-a-raster-draw-primbuffer-packvu/draw_packvu.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 448, "name": "DrawPrimBufferPackVUNBW__8VIRasteriiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_packvunbw.o", "profile": "authored_cpp", "sha256": "489396f207302872f595795085f6db5e0732001edbbfa0d5264aa1a8eed79efc", "source": "sources/w16-prim-a-raster-draw-primbuffer-packvunbw/draw_packvunbw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 592, "name": "DrawPrimBufferPackVUNC__8VIRasteriiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_packvunc.o", "profile": "authored_cpp", "sha256": "cd1947b495e0aa9f41f4948ec615bf3450607c2b2f154dfff584678c5212aedf", "source": "sources/w16-prim-a-raster-draw-primbuffer-packvunc/draw_packvunc.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 640, "name": "DrawPrimBufferPackVUNC__8VIRasteriiiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_packvunc3.o", "profile": "authored_cpp", "sha256": "b37f5688d9a189a9a75cdd9c12b8a5623819bc9f545dc1dcfd5a418c3bf7a6c1", "source": "sources/w16-prim-a-raster-draw-primbuffer-packvunc3/draw_packvunc3.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 528, "name": "DrawPrimBufferPackVUNCG__8VIRasteriiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_packvuncg.o", "profile": "authored_cpp", "sha256": "746324103d7d2e3b95851becbc4be440c267d9cc9aebed2ad6e6b935e460e843", "source": "sources/w16-prim-a-raster-draw-primbuffer-packvuncg/draw_packvuncg.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 420, "name": "DrawPrimBufferVUNC__8VIRasteriiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_raster_draw_primbuffer_vunc.o", "profile": "authored_cpp", "sha256": "bea6a6c4db108dc012e1b692c8973dc11684409362d1706b5bc77c32b9bd7fc4", "source": "sources/w16-prim-a-raster-draw-primbuffer-vunc/draw_vunc.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "Collide__6VIZoneR8VIEnvRayR7VIVect3T2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_zone_collide_env_ray.o", "profile": "authored_cpp", "sha256": "725bf4e39dfe0915a5b4ce14f2792467114a3e45c807ba76fd3eec27be34caca", "source": "sources/w16-prim-a-zone-collide-env-ray/collide_env_ray.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "Collide__6VIZoneR18VISceneCollRayVars"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16pa_zone_collide_scene_vars.o", "profile": "authored_cpp", "sha256": "10fa093e100822b1d006dc95459c6c35267d9653caa937e3032daf13264a04e0", "source": "sources/w16-prim-a-zone-collide-scene-vars/collide_scene_vars.cpp"}
```

## Authored but never executed

All 36. They ran only in the private interpreter against logging stubs and in native host checks with substituted callees; none
ran on the EE or in gameplay. In particular: the draw routines were exercised with a random memory image and stubbed matrix,
DMA and material calls; scratchpad contents were invented. The sphere visibility test uses `rsqrt.s` and the sphere
initialiser uses `sqrt.s`; the matrix products around them were stubbed, so last-bit equivalence with VU0 results is not
established, and the interpreter rounds each float operation to nearest. The two CalcBVolumes routines were run with at most three groups and eight
entries each.

## Unknown

* Real member and field names of every class touched, and the meaning of the type values (which of 0..8 is which vertex format).
* What the word at material +60 flags, the scratchpad words at 0x100/0x190/0x1cc/0x222 (inferred only as a frustum or visibility
  bound), and the two tag word values 0x6c008000 and 0x6c018000 in `VIPrimBuffer::End`.
* Whether the original `Begin`/`End` for the unaccepted prim-buffer types are ever reached (types 1, 3, 6, 7 return -1).
* `Lock` has no matching unlock among the assigned names; who clears the flag is unknown.
* Return types of `Bound`, `Union`, `Add`, `Precalc` and the `Collide...` walkers beyond what the callers use (zero/nonzero or
  negative). The stack layouts of the original (vector temporaries, whether they are default-constructed) are not reproduced.
* `VIPrimBuffer::Init` ignores its raster argument in the original; why is unknown.
