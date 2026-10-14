# w16-prim-b: prim and collision buffers, collision queries, vector/matrix/quaternion (part 2)

Branch `agents/w16-prim-b`. One commit per function, each pushed. Source is under `experimental/sources/w16-prim-b-*`; every directory
holds the authored source and a `host_check.cpp` (host headers, not a plan input). Addresses and sizes come from the pinned private
`SLUS_205.65` read locally with the reviewed EE binutils; no executable bytes, disassembly listing or decompiler output is committed.
Every source opens with an Observed / Inferred / Unknown comment. Compiled sizes are from the reviewed EE g++ with the `authored_cpp`
flags and are not claims of byte identity.

## Result

Of the 35 assigned names, **34 are authored** here. The 35th, `RotateHPR__10VIMatrix44RC7VIVect3`, already exists as
`experimental/sources/w12-vimath-b-vimatrix44-rotate-hpr` but is not in this branch's `build-plan.json`; it is left to the integrator
(nothing was re-authored). One full private partial build was run with these 34 units appended to the branch plan (see "Build check").

## How each function was checked

- Compiled with the reviewed `authored_cpp` flags (EE g++) and inspected for undefined symbols; no `madd`/`msub` contraction appears.
- Compared against the original function with the private R5900 interpreter harness (outside git): same memory snapshot and same
  stub callees feed the original and the authored object, and the call logs, return value (where the routine returns one) and every
  changed byte outside the stack must match. Unreachable remainders are padding or annulled words. Scenario counts are 20 to 80 each.
- Hand-made mutations of the authored source (constants, offsets, operand order, branch conditions, argument order) were applied one
  at a time and caught by the paired runs, with the exceptions listed under "Unknown".
- A native host check per unit (host headers, `-ffp-contract=off`). Where a unit reads pointer words, the host check maps them through
  an arena (`NORRATH_HOST_ARENA`).
- Interpreter additions made privately for this package: the VU0 macro-mode model (`lqc2`, `sqc2`, `qmtc2`, `vmulx`, `vsub`) for
  `Normalize__7VIVect4` and the local-matrix zero fill; the harness copy carries the earlier `rsqrt.s` fix.

## Units

| unit | symbol | image address | image bytes | compiled bytes |
|---|---|---|---|---|
| `w16-prim-b-bbox-store-outward` | `StoreOutward__C6VIBBoxP7VIPlane` | `0x01071808` | 180 | 156 |
| `w16-prim-b-collbuffer-set-packing` | `SetPacking__12VICollBufferi` | `0x0108baa0` | 80 | 68 |
| `w16-prim-b-collbuffer-unlock` | `Unlock__12VICollBuffer` | `0x0108ba80` | 28 | 32 |
| `w16-prim-b-collbuffer-vertex-float` | `Vertex__12VICollBufferR7VIVect3` | `0x0108bbb0` | 124 | 120 |
| `w16-prim-b-collbuffer-vertex-group` | `VertexGroup__12VICollBufferSc` | `0x0108bc30` | 12 | 12 |
| `w16-prim-b-collbuffer-vertex-s16` | `Vertex__12VICollBufferR10VIVect3S16` | `0x0108b1e8` | 248 | 264 |
| `w16-prim-b-collide-query` | `Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer` | `0x010971e0` | 104 | 72 |
| `w16-prim-b-collsphere-ctor` | `__12VICollSphere` | `0x010a0878` | 20 | 8 |
| `w16-prim-b-effectvolume-render-dust-storm` | `RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44` | `0x01149800` | 1432 | 1040 |
| `w16-prim-b-effectvolume-render-falling-leaves` | `RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44` | `0x01147e70` | 968 | 680 |
| `w16-prim-b-effectvolume-render-ground-fog` | `RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44` | `0x01148f30` | 1260 | 1000 |
| `w16-prim-b-effectvolume-render-light-shafts` | `RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44` | `0x011484d8` | 1992 | 1528 |
| `w16-prim-b-hsprite-set-node-bind-pose` | `SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3` | `0x0105fa90` | 528 | 480 |
| `w16-prim-b-matrix44-copy-ctor` | `__10VIMatrix44RC10VIMatrix44` | `0x010d07a0` | 84 | 72 |
| `w16-prim-b-matrix44-transform-scale3` | `Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2` | `0x01099e78` | 300 | 276 |
| `w16-prim-b-matrix44-transform-uniform` | `Transform__10VIMatrix44RC6VIQuatfRC7VIVect3` | `0x01099d88` | 240 | 240 |
| `w16-prim-b-pickseg-ctor` | `__13VIPickSegment` | `0x010bf470` | 112 | 92 |
| `w16-prim-b-primbuffer-normal-s8` | `Normal__12VIPrimBufferR9VIVect3S8` | `0x01102d78` | 40 | 40 |
| `w16-prim-b-primbuffer-set-packing` | `SetPacking__12VIPrimBufferiii` | `0x01102b88` | 188 | 156 |
| `w16-prim-b-primbuffer-unlock` | `Unlock__12VIPrimBuffer` | `0x011012c0` | 204 | 184 |
| `w16-prim-b-primbuffer-uv0` | `UV0__12VIPrimBufferR7VIVect2` | `0x01102cd0` | 32 | 32 |
| `w16-prim-b-primbuffer-uv0-s16` | `UV0__12VIPrimBufferR10VIVect2S16` | `0x01102cf0` | 32 | 32 |
| `w16-prim-b-primbuffer-vertex-float` | `Vertex__12VIPrimBufferR7VIVect3` | `0x011014b0` | 616 | 304 |
| `w16-prim-b-primbuffer-vertex-group` | `VertexGroup__12VIPrimBuffers` | `0x01102e50` | 16 | 16 |
| `w16-prim-b-primbuffer-vertex-s16` | `Vertex__12VIPrimBufferR10VIVect3S16` | `0x01101718` | 1412 | 720 |
| `w16-prim-b-quat-slerp` | `Slerp__6VIQuatRC6VIQuatT1f` | `0x010cfdc0` | 492 | 508 |
| `w16-prim-b-raster-pick-prim-buffer-pack-vunbw` | `PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal` | `0x0111eee0` | 8 | 8 |
| `w16-prim-b-raster-pick-prim-buffer-pack-vunc` | `PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal` | `0x0111de58` | 1272 | 1176 |
| `w16-prim-b-raster-pick-prim-buffer-pack-vunc-colors` | `PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal` | `0x0111e938` | 1448 | 1272 |
| `w16-prim-b-raster-pick-prim-buffer-pack-vuncg` | `PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal` | `0x0111e350` | 1512 | 1304 |
| `w16-prim-b-raster-pick-prim-buffer-vunc` | `PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal` | `0x0111d740` | 1816 | 2088 |
| `w16-prim-b-raster-prim-buffer` | `PrimBuffer__8VIRasteri` | `0x01113370` | 28 | 28 |
| `w16-prim-b-raster-set-object-blends` | `SetObjectBlends__8VIRasteriP10VIMatrix44` | `0x0110abf8` | 372 | 488 |
| `w16-prim-b-vect4-normalize` | `Normalize__7VIVect4` | `0x01066378` | 120 | 120 |

## Evidence summary

Observed (all from the pinned image; details are in each source header):

- `VICollBuffer`: type word at +8 (0 float vertices, 1 and 3 packed 16-bit vertices), lock word at +12, vertex count at +60, entry
  index at +52, entry array pointer at +72 (28-byte records whose halfword at +2 counts vertices), vertex array pointer at +76,
  group byte at +80/+81, reciprocal packing scale at +16. `Vertex` appends and bumps the entry's count; `Unlock` clears the lock word.
- `VIPrimBuffer`: type word at +8 (0 float layout; 2, 4, 5, 8 packed layouts), lock word at +12, packing scales at +48/52/56,
  an unlock-time scale at +56/+60 chosen by the type, entry index +72, vertex-slot index +80, entry pointer +92 (28-byte records, word 0
  counts logical vertices), slot pointer +96 (16-byte slots). The static "current" values `PrimUV0`, `PrimUV0S16`, `PrimNormal`,
  `PrimNormalS8`, `PrimColor`, `PrimBones`, `PrimWeightsS8` and `PrimVertexGroup` are read by `Vertex` and written by the setters.
- The five `PickPrimBuffer*` variants walk group records (`pb+88`), 28-byte entries (`pb+92`) and vertex slots (`pb+96`), cull each
  entry, then run a triangle strip with a flip flag through `Pick` and `CalcHitColor`; the float variant reads positions and colours
  as floats, the packed variants read signed halfwords times `pb+48` and bytes shifted left by one, one adds a per-group offset vector
  from the raster at +18936, one takes colours from a separate colour buffer, and one only returns 0.
- Four effect-volume render routines share one shape (guards on floats 92 and 96, null particle array returns -1, material select,
  zeroed locals, a particle loop that builds a quad and emits a six-vertex fan); they differ in particle stride (36, 36, 60, 32 bytes),
  table (`sFallEffect`, `sFogCard`, `sDustCard`) and motion terms.
- `SetObjectBlends` builds a DMA chain tag plus per-32-matrix blocks around `memcpy`; `SetNodeBindPose` stores a bind pose and
  derives the inverse and parent-relative pose through `Transform`, `Invert`, `Mul` and `ConvertTo`.

Inferred (marked as such in the headers): the roles of the buffer fields (lock flag, per-type scale), that the strip pick keeps
winding consistent with the flip flag, that the DMA words in `SetObjectBlends` are tag/VIF words, that the light-shaft envelope is a
scene clock in hours, and the argument roles of callees taken from their mangled names.

## Imports closed (34)

`Normal__12VIPrimBufferR9VIVect3S8`, `Normalize__7VIVect4`,
`PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`,
`PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`,
`PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal`,
`PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`,
`PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`, `PrimBuffer__8VIRasteri`,
`Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer`,
`RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44`, `RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44`,
`RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44`, `RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44`,
`SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3`, `SetObjectBlends__8VIRasteriP10VIMatrix44`, `SetPacking__12VICollBufferi`,
`SetPacking__12VIPrimBufferiii`, `Slerp__6VIQuatRC6VIQuatT1f`, `StoreOutward__C6VIBBoxP7VIPlane`,
`Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2`, `Transform__10VIMatrix44RC6VIQuatfRC7VIVect3`, `UV0__12VIPrimBufferR10VIVect2S16`,
`UV0__12VIPrimBufferR7VIVect2`, `Unlock__12VICollBuffer`, `Unlock__12VIPrimBuffer`, `VertexGroup__12VICollBufferSc`,
`VertexGroup__12VIPrimBuffers`, `Vertex__12VICollBufferR10VIVect3S16`, `Vertex__12VICollBufferR7VIVect3`,
`Vertex__12VIPrimBufferR10VIVect3S16`, `Vertex__12VIPrimBufferR7VIVect3`, `__10VIMatrix44RC10VIMatrix44`, `__12VICollSphere`,
`__13VIPickSegment`.

Not authored here: `RotateHPR__10VIMatrix44RC7VIVect3` (already authored by `w12-vimath-b-vimatrix44-rotate-hpr`, 296 compiled
bytes, not in this branch's plan).

## Imports opened

Callees and storage these units leave unresolved after the private build (none is defined here; owners are other packages or
nobody yet): `CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25`, `Color__8VIRasterRC10VIColor32F`,
`ConvertTo__C10VIMatrix44R6VIQuatRfR7VIVect3`, `Cull__8VIRasterR13VIPickSegmentR6VIBBox`,
`Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23`,
`PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44`,
`QueryBufferPackVGF__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer`,
`QueryBufferPackV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer`,
`QueryBufferV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer`, `SetMaterial__8VIRasteri`,
`SetSolidFillUIAVGMaterial__8VIRaster`, and the data symbols `_12VIPrimBuffer$PrimBones`, `_12VIPrimBuffer$PrimColor`,
`_12VIPrimBuffer$PrimNormal`, `_12VIPrimBuffer$PrimNormalS8`, `_12VIPrimBuffer$PrimUV0`, `_12VIPrimBuffer$PrimUV0S16`,
`_12VIPrimBuffer$PrimVertexGroup`, `_12VIPrimBuffer$PrimWeightsS8`, `_8VIRaster$ObjectBlendsBuffer`. All other callees and data the
units name (`BeginTriFan__8VIRaster`, `EndTriFan__8VIRaster`, `UV__8VIRasterRC7VIVect2`, `Vertex__8VIRasterRC7VIVect3`,
`Identity__10VIMatrix44`, `Invert__10VIMatrix44RC10VIMatrix44`, `Mul__10VIMatrix44RC10VIMatrix44T1`,
`Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `RotateY__10VIMatrix44f`, `_$_10VIMatrix44`, `__9VICollRay`, `_14VIEffectVolume$mScene`,
`sFallEffect`, `sFogCard`, `sDustCard`, `acosf`, `atanf`, `sinf`, `powf`, `memcpy`) are defined elsewhere in the shared plan.

## Build check

One private partial build was run (outside git) with the shared branch plan plus these 34 units appended: 7,361 translation units
compiled with implicit assembly and one relocatable link, no failure; 5,868 function bodies and 824 unresolved imports. Every
assigned name except `RotateHPR__10VIMatrix44RC7VIVect3` was absent from `unresolved-imports.json` (that one is authored by
`w12-vimath-b-vimatrix44-rotate-hpr`, which this branch's plan does not select). The link was not executed and is not an
original-byte match; it only shows symbol closure and that the 34 definitions do not collide with the rest of the plan.


## Build-plan entries needed

Append these 34 authored inputs (one JSON object per line, `profile` `authored_cpp`, `header_closure` empty, no host-test file) and
assign fresh `input_order` values; `default_authored_inputs` increases by 34. `docs/agents/w16-prim-b-plan-entries.py --toolchain
/path/to/ee [--private-root DIR]` regenerates them and writes a private plan copy outside git. No `private_object_sha256` is claimed.
Also append `w12-vimath-b-vimatrix44-rotate-hpr` if it is not already merged.

```
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "StoreOutward__C6VIBBoxP7VIPlane"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_bbox_store_outward.o", "profile": "authored_cpp", "sha256": "603544a970bbe66aa75f64213435174a945f558d4a47955041dc19b0056bdba3", "source": "sources/w16-prim-b-bbox-store-outward/store_outward.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "SetPacking__12VICollBufferi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collbuffer_set_packing.o", "profile": "authored_cpp", "sha256": "c21f01f3477ac88ebc9a78a9afc1f2125598744f0e03e3564f6283f1789a78b0", "source": "sources/w16-prim-b-collbuffer-set-packing/set_packing.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Unlock__12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collbuffer_unlock.o", "profile": "authored_cpp", "sha256": "1535209e46efdc4bda672ea7f69e43b51fcae428c054c4ba7bfc18ddff5c02ed", "source": "sources/w16-prim-b-collbuffer-unlock/unlock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Vertex__12VICollBufferR7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collbuffer_vertex_float.o", "profile": "authored_cpp", "sha256": "e564ef7724883f8f66b62f0b8783a202730336f9bb1ea7c08f016685855292c9", "source": "sources/w16-prim-b-collbuffer-vertex-float/vertex.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "VertexGroup__12VICollBufferSc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collbuffer_vertex_group.o", "profile": "authored_cpp", "sha256": "964d866f8b7e848ad0fb0d91a453a4cd4ae6a615917fdf4af85685170fea771a", "source": "sources/w16-prim-b-collbuffer-vertex-group/vertex_group.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 264, "name": "Vertex__12VICollBufferR10VIVect3S16"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collbuffer_vertex_s16.o", "profile": "authored_cpp", "sha256": "9d17b4659f619a56882b230be6250fcb8545bf03b7ada0b25f5fab2cfff83847", "source": "sources/w16-prim-b-collbuffer-vertex-s16/vertex.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collide_query.o", "profile": "authored_cpp", "sha256": "6fbcf8e75fa9bcab5e24909ec0e9b463146150fde155bfe7db633f6d60efc5c3", "source": "sources/w16-prim-b-collide-query/query.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "__12VICollSphere"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_collsphere_ctor.o", "profile": "authored_cpp", "sha256": "6a670b6a44cd4562d55551baa7e36804a1f57853eef4c09d4dd1c2c7aedb8639", "source": "sources/w16-prim-b-collsphere-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1040, "name": "RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_effectvolume_render_dust_storm.o", "profile": "authored_cpp", "sha256": "4bea0d35a0c090ebc6f1f16c0e7eaf1e33e667c469985c00b4d16f8dae5c1c94", "source": "sources/w16-prim-b-effectvolume-render-dust-storm/render_dust_storm.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 680, "name": "RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_effectvolume_render_falling_leaves.o", "profile": "authored_cpp", "sha256": "738c7b73ff9adff9f46067d49782e3d5bf6e5744da38af5b69814c104eeb3ca5", "source": "sources/w16-prim-b-effectvolume-render-falling-leaves/render_falling_leaves.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1000, "name": "RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_effectvolume_render_ground_fog.o", "profile": "authored_cpp", "sha256": "19665867e195e496fa9f343929485d703498c2e72344135dd2b70ee287e82cd9", "source": "sources/w16-prim-b-effectvolume-render-ground-fog/render_ground_fog.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1528, "name": "RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_effectvolume_render_light_shafts.o", "profile": "authored_cpp", "sha256": "3a277ef9063ee948ccbe9f1795fb7990063cce8c2c818554f3b1ebb88ca1b4eb", "source": "sources/w16-prim-b-effectvolume-render-light-shafts/render_light_shafts.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 480, "name": "SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_hsprite_set_node_bind_pose.o", "profile": "authored_cpp", "sha256": "5893652fcad154b32751c58184dc20588514db4197c4a646fd55790d13405b5f", "source": "sources/w16-prim-b-hsprite-set-node-bind-pose/set_node_bind_pose.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "__10VIMatrix44RC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_matrix44_copy_ctor.o", "profile": "authored_cpp", "sha256": "a860c1dd78b907c2b48a4eadf67b3c0d7199fd092fc4b78f926505f71459222b", "source": "sources/w16-prim-b-matrix44-copy-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_matrix44_transform_scale3.o", "profile": "authored_cpp", "sha256": "721c7ed5e825919efea242b541bf6609de2cbea84004c9459ca4bfc247b6c1a6", "source": "sources/w16-prim-b-matrix44-transform-scale3/transform.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 240, "name": "Transform__10VIMatrix44RC6VIQuatfRC7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_matrix44_transform_uniform.o", "profile": "authored_cpp", "sha256": "0f80da72922736813e944181a6b232d511dbf8b9fe2f24f4398e525a1aeb8fc6", "source": "sources/w16-prim-b-matrix44-transform-uniform/transform.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__13VIPickSegment"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_pickseg_ctor.o", "profile": "authored_cpp", "sha256": "24138e658077eb2ab6ffaf6f559e19315d606f40d77e5574b30d26385eb9a2a8", "source": "sources/w16-prim-b-pickseg-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "Normal__12VIPrimBufferR9VIVect3S8"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_normal_s8.o", "profile": "authored_cpp", "sha256": "0401610d7b53b58b65132c620e24401fe04ec7238e8943a26e56b7a864bf75e6", "source": "sources/w16-prim-b-primbuffer-normal-s8/normal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "SetPacking__12VIPrimBufferiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_set_packing.o", "profile": "authored_cpp", "sha256": "4cb75872ccf9afdb185dc20608c6718a1c66cf062256ee125cac7ecc8e7cd7b3", "source": "sources/w16-prim-b-primbuffer-set-packing/set_packing.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Unlock__12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_unlock.o", "profile": "authored_cpp", "sha256": "1b0a5e577ada50adcfdf0bcb0bd3969c11f1e8383c4892550e9ee0c4c2f1b3fc", "source": "sources/w16-prim-b-primbuffer-unlock/unlock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "UV0__12VIPrimBufferR7VIVect2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_uv0.o", "profile": "authored_cpp", "sha256": "db6e41fe4e9338119e182226f5068372bd23e830c45aef3b28a40605016bd027", "source": "sources/w16-prim-b-primbuffer-uv0/uv0.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "UV0__12VIPrimBufferR10VIVect2S16"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_uv0_s16.o", "profile": "authored_cpp", "sha256": "79bc4fa2106d90f13f46f43135d902eaf0427ed3e9e76239dde51c4e43453140", "source": "sources/w16-prim-b-primbuffer-uv0-s16/uv0.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 304, "name": "Vertex__12VIPrimBufferR7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_vertex_float.o", "profile": "authored_cpp", "sha256": "3d6ffe89481382e6e381fbe594a46e90ca836336ec3d5e16d555c31b168e088e", "source": "sources/w16-prim-b-primbuffer-vertex-float/vertex.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "VertexGroup__12VIPrimBuffers"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_vertex_group.o", "profile": "authored_cpp", "sha256": "f41a94c3132e8e72e3a249528f62d7b74404ec03bb091d2f979c2970f29fa202", "source": "sources/w16-prim-b-primbuffer-vertex-group/vertex_group.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 720, "name": "Vertex__12VIPrimBufferR10VIVect3S16"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_primbuffer_vertex_s16.o", "profile": "authored_cpp", "sha256": "4ee6350b81e5dc3cf705c845cff0b212d66ca456f9044a92a0db8ef9bcb94010", "source": "sources/w16-prim-b-primbuffer-vertex-s16/vertex.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 508, "name": "Slerp__6VIQuatRC6VIQuatT1f"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_quat_slerp.o", "profile": "authored_cpp", "sha256": "9c1a3ae968f74f3dbaafd51caeb2e102ce6d7b90382349b11d8c5097843395e5", "source": "sources/w16-prim-b-quat-slerp/slerp.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_pick_prim_buffer_pack_vunbw.o", "profile": "authored_cpp", "sha256": "81be4b5ccd1f90ce3b6ebdf063963b63865371505ed11461b012d313240576a6", "source": "sources/w16-prim-b-raster-pick-prim-buffer-pack-vunbw/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1176, "name": "PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_pick_prim_buffer_pack_vunc.o", "profile": "authored_cpp", "sha256": "35afd8fbe47b09dddc502ad3e793d7104b75d99191dec1ecf5b259d3114f7fe6", "source": "sources/w16-prim-b-raster-pick-prim-buffer-pack-vunc/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1272, "name": "PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_pick_prim_buffer_pack_vunc_colors.o", "profile": "authored_cpp", "sha256": "ec8d7375a4c5d05fcab621ca0771af8c8d1a7e028787dfb1759fa8e33608dd57", "source": "sources/w16-prim-b-raster-pick-prim-buffer-pack-vunc-colors/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1304, "name": "PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_pick_prim_buffer_pack_vuncg.o", "profile": "authored_cpp", "sha256": "f0aa85f270151b684bb723f6ce4db79b6eaf033c9bb9c81ba5937c2358691997", "source": "sources/w16-prim-b-raster-pick-prim-buffer-pack-vuncg/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2088, "name": "PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_pick_prim_buffer_vunc.o", "profile": "authored_cpp", "sha256": "f016e96977fa7619c9083e1cc6a081a69a7369a18d4a161739c9904699cccc45", "source": "sources/w16-prim-b-raster-pick-prim-buffer-vunc/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "PrimBuffer__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_prim_buffer.o", "profile": "authored_cpp", "sha256": "de661f6088d81455bf68c006bfd7f7283c54c080249a0c4a1481aa7f4d636773", "source": "sources/w16-prim-b-raster-prim-buffer/prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 488, "name": "SetObjectBlends__8VIRasteriP10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_raster_set_object_blends.o", "profile": "authored_cpp", "sha256": "d5335e2f9d4e8ecbfc49ee1c4f414258c53e31e316928a53eb2ba05903cf06c3", "source": "sources/w16-prim-b-raster-set-object-blends/set_object_blends.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Normalize__7VIVect4"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16primb_vect4_normalize.o", "profile": "authored_cpp", "sha256": "a41897c84e76c2a29afbf25040fde241ba0e418a42024658022c6035a2cc835f", "source": "sources/w16-prim-b-vect4-normalize/normalize.cpp"}
```

## Authored but never executed

All 34 functions. They ran only inside the private interpreter harness (with stubbed callees) and on the host in the host checks;
none has run on EE hardware or in the game, and the partial object has not been linked into a runnable image. Data layouts come
from the original code's accesses only. Specific gaps:

- The interpreter's divide-by-zero result differs from hardware, so `RenderLightShafts` (camera x of exactly 0, where `atanf` sees
  an infinity or NaN) cannot distinguish `0 < x` from `0 <= x`; a mutation of that comparison survives.
- A dead repeated test (`now < 9.0` a second time) in `RenderLightShafts` is unreachable and kept for fidelity.
- Entry vertex counts below 2 in the `PickPrimBuffer*` strips make the original loop for about four billion iterations; the
  paired scenarios use counts of at least 2 (the source does not guard against it either).
- The VU0 sequences (`Normalize__7VIVect4` multiply, the local-matrix zero fill) use the interpreter's round-to-nearest model, not
  VU0 hardware rounding; the host builds substitute plain float arithmetic.
- `float`-to-byte conversions in `PickPrimBufferVUNC` match the interpreter's saturating `cvt.w.s`; hardware behaviour for NaN
  and huge values was not sampled.
- EE g++ rounds decimal float constants toward zero, so the two pi-like constants are written as hexadecimal floats
  (`0x1.921fb4p+1f` is bits 0x40490fda, `0x1.921fb4p+0f` is 0x3fc90fda, `0x1.555554p-2f` is 0x3eaaaaaa) to keep the intended bits.

## Unknown

- The names and meaning of most buffer fields (offset 16 of `VICollBuffer`, offsets 48..60 of `VIPrimBuffer`), the type numbers
  2, 4, 5 and 8, the high words written by `Vertex__12VIPrimBufferR10VIVect3S16` (`0x69028000`, `0x6d028000`, `0x6e028000`,
  `0x6e038000`) and `SetObjectBlends` (`0x10000000`, `0x6c800000`, `0x6c000000`).
- What the second `PrimBuffer` table words (12-byte elements) hold besides the first, and who writes them.
- The first two colours of each colour-buffer block, the group record fields beyond material index and count, and the palette
  fields beyond word 12.
- The argument roles of `Pick`, `CalcHitColor`, `ConvertTo`, `Invert`, `Mul` and `PrepareBillboardTransform` are taken from
  their mangled names and register use; none of those callees was read here.
- The meaning of particle words not read by the render routines, the units of the scene clock in `RenderLightShafts`, and
  whether the four static card tables are initialised at run time (they read as zero in the image file).
- `VIPickSegment` fields at offsets 48..59 and 92 onward are not written by its constructor; their use is unknown.
