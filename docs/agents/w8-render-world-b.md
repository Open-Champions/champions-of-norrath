# w8-render-world-b: textures, models, world and lumps (2/2)

Branch `agents/w8-render-world-b`. Private executable `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`
from the megatile notes; GP is `0x004879f0`). Symbol names, addresses and sizes
below come from that executable's own symbol table. Nothing here was executed on
the game or on hardware.

## Method and checks performed

- Each function was disassembled privately (nothing derived was committed), written
  in readable C++ and compiled alone with the reviewed `authored_cpp` flags
  (EE GCC 15.2.0, `-G0 -O2 ... -march=r5900 -std=c++17`). Every unit compiled.
- Every unit has a `host_check.cpp` built with `c++ -std=c++17 -Wall -Wextra -Werror`
  (units that read headers by address also use their `NORRATH_HOST_*` macro, which
  swaps a stored 32-bit address for an offset into a test arena). All exited 0.
- All 21 authored units were linked together with `ld -m elf32lr5900 -r --warn-common`
  with no duplicate definitions among them.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passed (214 tests).
- I did not run `tools/build_partial.py`: it refuses plans whose sources are not in
  the shared `snapshot.json`, and the plan and snapshot are not mine to edit.
- No byte match to the original is claimed for any function. Compiled sizes differ
  from the original sizes.

## Observed

Each source header records the address and size of the function it implements, the
fields it reads and writes (by decimal byte offset), and where a constant lives.
Highlights that are easy to miss:

- `worldFindWaterPatches` and `worldDrawWorld` walk the same half-word stream tree
  (split nodes have bits 0x8000|0x4000 in the first case and just 0x8000 in the second).
  Keys are built as `sign_extended_low | (sign_extended_high << 16)`, shifted by 2.
- `texProcessLoad` computes the squared distance of a candidate block using the x cell
  index for both axes. This looks unintended but is what the code does; it is reproduced
  and covered by a host check.
- `worldDrawWorld` clears `modelClippingOn` at its end except when the display list has
  fewer than 3000 entries free, in which case it returns early without clearing.
- `modelDraw` builds two GS state packets around the model's reference tags and rewrites
  the last DMA tag's id; the GS register numbers written are 0x40/0x41 (SCISSOR), 0x44,
  0x47/0x48 (TEST), 0x4e/0x4f (ZBUF), 0x1a/0x1b (PRMODECONT/PRMODE).
- The constants read by `worldMoveDoor` are 32767.0f and 1/32767 (bits `0x38000100`),
  those of `worldDrawWorld` are +-1,000,000.0f (x/y) and -60.0f..780.0f (z), and
  `texProcessLoad` starts its search distance at 1.0e10f.

## Inferred

- `halos`, `nmHalos`, `modelOption` and similar values are declared with widths taken from
  how the code stores them; their real types are not known.
- The zero-initialised tables (`texmap`, `texdecompress`, and so on) are placed in
  `.data` / `.sdata` by section attribute to match the original image. `uncompMap`
  (0x2800 bytes) and `segmentUsed` (0x100 bytes) are `.bss` with sizes taken from the next
  symbol and the sizes cleared by another package's initialiser.
- In `texProcessLoad` the state byte meanings (0 idle, 1 waiting on the stream, 2 releasing,
  3 one-frame delay) are inferred from the transitions, not from any name.

## Unknown

- What the world header, the 60-byte polyset records, the 64-byte textures and the model
  header fields mean beyond the offsets read.
- Whether `worldFindWaterPatches` is really only used for water (the name is the
  executable's).
- Types of several flags and of the halos table.
- `texDecompressThread__FPv` (0x0016f8b8, 2,788 bytes) is not authored; see below.

## Imports closed (30 of the 31 in my list)

`megatile_ramkmax`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`,
`modelGetActiveChunks__FP12_modelHeaderi`, `modelResetOptionalParams__Fv`,
`modelSetShadowBrightness__Ff`, `nmLumps`, `segmentUsed`, `texIsDecompressing__Fv`,
`texIsLoading__Fv`, `texProcessDecompress__Fv`, `texProcessLoad__Fi`, `texdecompress`,
`texmap`, `texmapend`, `texmapstart`, `uncompMap`, `uncompSpin`, `world`,
`worldAllocateWaterPatches__Fi`, `worldClear__Fv`, `worldClipCubeToClipFrustum__FPA1_fi`,
`worldDrawEnable`, `worldDrawWorld__FP12_worldHeader`,
`worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi`,
`worldMoveDoor__FP12_worldHeaderiG6Point3s`, `worldReset__FP12_worldHeader`,
`worldRunWaterTask__Fv`, `worldSetDoorCollision__FP12_worldHeaderii`,
`worldSetDoorHeight__FP12_worldHeaderif`, `worldSetViewCenter__Fv`, `worldViewCenter`.

## Imports not closed

`texDecompressThread__FPv` at `0x0016f8b8`, 2,788 bytes. Not authored yet; it is a long Huffman-style texture decoder thread with scratchpad tables and GS packet building, left for last.

## Imports opened

Symbols my sources reference but do not define (unresolved in a link of only my units):
`DynamicLightArray`, `GLISTSIZE`, `StartThread`, `TerminateThread`, `WakeupThread`,
`animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps`,
`animLoadBoneMatricies__FP6PacketP14AnimationState`, `cdEndStream__Fv`,
`cdIsBusyStreamingNonwithstanding__Fv`, `cdIsBusy__Fv`, `cdStartStream__FPci`,
`cdStreamLoadAsync__FPcT0i`, `clipFrustumNormalSign`, `decodeQHead`, `decodeQTail`,
`decompressThreadid`, `defaultHalos`, `drawAddToList__FPviP8_textureP11_drawRecordi`,
`drawFrameNm`, `drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi`, `dumpMem__FPvi`,
`engineCurrentLevelName`, `engineScissory1`, `engineScissory2`, `floorf` (external Newlib),
`gameIsPaused__Fv`, `glistp`, `globalShadowBrightness`, `gstart`, `halos`, `icos__Fi`,
`isin__Fi`, `lightDeactivate__FP5Light`, `loadCompleteFlag`, `loadInProgress`,
`loadKillList`, `loadKillListSize`, `loadPage`, `loadSegmentsLoaded`, `loadSlot`,
`matMul__FP8Matrix44P8Matrix34T0`, `memAlloc__FiPc`, `memFree__FPv`, `memset`,
`modelClippingOn`, `modelDrawFade`, `modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi`,
`modelOption`, `modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34`,
`modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc`, `modelShadowScale`,
`nmHalos`, `nmWaterPatches`, `reallyLoad.873` (a function-local static in the original,
referenced here through an assembler label), `routeClearStart`, `routeEnable`, `rqHead`,
`rqTail`, `scx1`, `scx2`, `scy1`, `scy2`, `shadowBrightness`, `sprintf`,
`texCacheData__FP8_texturei`, `texDecodeStarted`, `texLastDecompressionTime`,
`textureFreeTexture__FP8_texture`,
`textureInitTexture__FP12_worldHeaderP8_texturePUcPc`, `texturePageBlock`,
`texturePageLoc`, `texturePageSegLoc`, `texturePageTable`, `texturesAreDecompressing`,
`texturesAreLoading`, `waterInit__Fv`, `waterPatch`, `worldAnimation`,
`worldCameraMatrix`, `worldCameraPos`, `worldCharViewMatrix`,
`worldClipCubeToFrustum__FPA1_fi`, `worldClipFrustum`, `worldLastDrawSucceded`,
`worldRunWater__FP12_worldHeader`, the water tuning globals
(`worldWaterAlpha`, `worldWaterAlpha2`, `worldWaterBumpAmount`, `worldWaterBumpRadius`,
`worldWaterBumpTimer`, `worldWaterColor`, `worldWaterColor2`, `worldWaterRainTimer`,
`worldWaterWavyness`, `worldWaterWavyness2`), `zwritesOff`, `zwritesOn`.
Many of these are authored by other packages under their real names (`worldWater*` by
world-state, `worldClipCubeToFrustum__FPA1_fi` and `worldCameraPos` by floor-world-view,
`DynamicLightArray` is used by sony-cd-world-init). `clipFrustumNormalSign` and
`worldClipFrustum` (0x006ec7a8, 0x006eb9f8) have no definer that I found.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, `input_order` assigned at integration;
`emitted_functions` from the integrator's build. Do not add any `host_check.cpp`.
The `world_draw_world.cpp` source is named that way because `draw_world.o` already
exists in the plan (game-state-draw-world).

```json
[
  {
    "source": "sources/w8-render-world-b-model-active-chunks/active_chunks.cpp",
    "object_name": "active_chunks.o",
    "sha256": "971eeaa27603ad1ddd71eb137a0c0c0b0fe144a14dc016310180d77561971d7a"
  },
  {
    "source": "sources/w8-render-world-b-model-draw/model_draw.cpp",
    "object_name": "model_draw.o",
    "sha256": "2e2ccf4580456013811fdcb99a2a3e20e6b1b177e3d02aa581cd5d284fb8bada"
  },
  {
    "source": "sources/w8-render-world-b-model-reset-optional/reset_optional.cpp",
    "object_name": "reset_optional.o",
    "sha256": "7d0c0e8a7409fd29c516ad1d245f4767cbf9cb1066781609ef5c3b977465d10b"
  },
  {
    "source": "sources/w8-render-world-b-model-shadow/shadow_brightness.cpp",
    "object_name": "shadow_brightness.o",
    "sha256": "5ce4cbf654a89cb72db8d4b3ec67971e4d5a0443c539c698b2c9e2bd2187cdc9"
  },
  {
    "source": "sources/w8-render-world-b-tex-is-decompressing/is_decompressing.cpp",
    "object_name": "is_decompressing.o",
    "sha256": "f55bdcedf2483ab705edd52901a06c3fc01f60c5277ab4a23bb86baaf9d20f79"
  },
  {
    "source": "sources/w8-render-world-b-tex-is-loading/is_loading.cpp",
    "object_name": "is_loading.o",
    "sha256": "60d8146c76e7bbbb493bbfce6ac2157948817e17d421f432d9056a518d65cb5a"
  },
  {
    "source": "sources/w8-render-world-b-tex-process-decompress/process_decompress.cpp",
    "object_name": "process_decompress.o",
    "sha256": "648dcc0e391831a061b74a2c7637ba0cba79de4272e41598523b380222d6803d"
  },
  {
    "source": "sources/w8-render-world-b-tex-process-load/process_load.cpp",
    "object_name": "process_load.o",
    "sha256": "5f68f4a087594170df321f3e18fab71ea3e55b0dbf5c7611ed8d2430e4fca09a"
  },
  {
    "source": "sources/w8-render-world-b-texture-data/texture_data.cpp",
    "object_name": "texture_data.o",
    "sha256": "585ae32f18bc8936610c52066b4b75372ccb2639a93877cb03bf803d9f8e0783"
  },
  {
    "source": "sources/w8-render-world-b-world-allocate-water-patches/allocate_water_patches.cpp",
    "object_name": "allocate_water_patches.o",
    "sha256": "9c24e3d7308b9c91642357ef88886b4ef61567e088032d1ed443253056d6fe2f"
  },
  {
    "source": "sources/w8-render-world-b-world-clear/world_clear.cpp",
    "object_name": "world_clear.o",
    "sha256": "e4aefd057b1a8db83220e426fd062124e234ea0293079d99487de8a252b70920"
  },
  {
    "source": "sources/w8-render-world-b-world-clip-cube-to-clip-frustum/clip_cube_to_clip_frustum.cpp",
    "object_name": "clip_cube_to_clip_frustum.o",
    "sha256": "cc64db84fb7ccaea3b50b8215f0e4166f8dee8453b3cbd4069cd3a4e932112f6"
  },
  {
    "source": "sources/w8-render-world-b-world-data/world_data.cpp",
    "object_name": "world_data.o",
    "sha256": "01ed314fa24ffb252f9801fb36edb7b06e565bd864b6ac607353246ad9e32ab2"
  },
  {
    "source": "sources/w8-render-world-b-world-draw-world/world_draw_world.cpp",
    "object_name": "world_draw_world.o",
    "sha256": "373c7ee201d6abb2737b97ce4e5a49b5cfc3eafdf5140d45b5898ec0e5027298"
  },
  {
    "source": "sources/w8-render-world-b-world-find-water-patches/find_water_patches.cpp",
    "object_name": "find_water_patches.o",
    "sha256": "1cb566a4b4756814206859d832f5e737bd5a09536894cc7aa7e54f84425d1dc9"
  },
  {
    "source": "sources/w8-render-world-b-world-move-door/move_door.cpp",
    "object_name": "move_door.o",
    "sha256": "4706272e36de46737e6c835ebc15d84e535d868eeb78c50d47d8177235d6143d"
  },
  {
    "source": "sources/w8-render-world-b-world-reset/world_reset.cpp",
    "object_name": "world_reset.o",
    "sha256": "d7381df13c422a423c09365358f20b007171dc6df2d3b22a25eca3fb2274c5e8"
  },
  {
    "source": "sources/w8-render-world-b-world-run-water-task/run_water_task.cpp",
    "object_name": "run_water_task.o",
    "sha256": "c47a9e0439acc4ab92abc9271dc62f22e13337ba7c8e520ec551f9db5b998ca4"
  },
  {
    "source": "sources/w8-render-world-b-world-set-door-collision/set_door_collision.cpp",
    "object_name": "set_door_collision.o",
    "sha256": "bf124942f0380a28c531c31e2fd2d072e6541c0c89d63023adba8cdcc75350e6"
  },
  {
    "source": "sources/w8-render-world-b-world-set-door-height/set_door_height.cpp",
    "object_name": "set_door_height.o",
    "sha256": "5bfe0af3e7a727650c95b545c117aaf903b5bc8ce466a6c5db953d2667507acc"
  },
  {
    "source": "sources/w8-render-world-b-world-set-view-center/set_view_center.cpp",
    "object_name": "set_view_center.o",
    "sha256": "7563832bbec751d7c5dbf28b6f23794d1b7eb4389303b8d8f4e5eda51d035329"
  }
]
```

## Authored but never executed

All of it. Every function was checked only by compiling it and running my own host
checks against stubs, which prove what I understood and wrote, not that the original
behaves that way. `worldDrawWorld`, `worldFindWaterPatches`, `worldReset`, `worldMoveDoor`,
`texProcessLoad` and `modelDraw` use arena offsets on the host in place of 32-bit
addresses; the EE objects use real addresses. `modelDraw` writes the second word of its
local packet (the last tag address) but the host checks do not read it.
No data was cross-checked against a running game, and the section placement of the data
units was checked only with `nm`/`readelf` on the compiled object.

## Blockers

- None needing the owner. The remaining symbol is blocked by size and effort, not by
  missing information.
