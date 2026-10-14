# w10-render-d: animation, model, light and render (AddParticle to map)

Branch `agents/w10-render-d`. Private executable `work/private/SLUS_205.65` (the Champions of Norrath
image the other notes name; GP `0x004879f0`). Addresses, sizes and names are the image's own symbol table.
Nothing here ran on hardware or inside the game. The sources of `usprintf`, `texCacheData` and `textureFreeTexture`
come from an interrupted earlier run of this package (the last one was uncommitted; I re-ran its host check, fixed a
non-terminating paired scenario that made cyclic chains, and committed it). The paired runs of `texCacheData` and `usprintf`
were repeated at the start of this run and still matched.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and
  compiled alone with the reviewed `authored_cpp` flags (EE GCC, `-G0 -O2 ... -march=r5900 -std=c++17`).
  Every unit compiled and is listed below with its compiled body size. Fresh compilation is not an
  original-byte match and no match is claimed.
- Every unit has a `host_check.cpp` built with the host C++ compiler and run on this Mac (all exit 0). Units that store
  32-bit addresses inside game structures use a `NORRATH_HOST_*` macro (the check is built with
  `-DNORRATH_HOST_WATER`, `-DNORRATH_HOST_WATERRUN`, `-DNORRATH_HOST_TEXTURE`, `-DNORRATH_HOST_SUBFONT` or
  `-DNORRATH_HOST_LIGHTEFFECT`) that turns a stored address into an offset in a test arena. Collaborators are recorded stubs.
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my freshly
  compiled one from the same memory snapshot with identical stub callees, then compared the call log
  (argument registers including float registers, the bytes behind pointer arguments), the return value where
  it is meaningful, and every changed byte outside the stack. Scenario counts per function are in the commit
  messages. For `draw__11AddParticle...`, `worldRunWater` and `subfontPrintu` the
  original's instruction coverage was measured: only annulled branch-delay slots and padding nops were never
  executed (3 words, 1 word and 4 words respectively).
- A mutated copy of most units was run through the same comparison and was detected. Not mutated: the two empty or
  dead-copy bodies, `_$_11AddParticle`'s siblings other than the flag test, the render data, and `texCacheData`,
  `textureFreeTexture` and `usprintf` (a mutation script for `usprintf` exists from the earlier run; I did not repeat it). For `textRemoveText` the hash start has no visible effect unless a decoy copy of the
  address sits elsewhere in the table, so the scenarios add decoys.
- The private interpreter needed `mult1`, `mflo1` and `mfhi1` (MMI) for `worldRunWater`; they were added to my private
  copy only. Earlier notes already warn about `sqrt.s`, `rsqrt.s`, saturating `cvt.w.s` and the halt-counts-as-pass
  defect; every run here was checked for `HALT` lines.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).
- I did not run `tools/build_partial.py` (the plan and snapshot are not mine to edit; one partial build at a
  time was asked for). Each unit was compiled alone and its undefined symbols listed instead.

## Functions authored (one verified function per commit)

| Function | Address | Original size | Compiled bytes | Source directory |
| --- | --- | ---: | ---: | --- |
| `clone__11LightEffect` | `0x00126f70` | 524 | 368 | `w10-render-d-light-effect-clone` |
| `_$_5Light` | `0x0013eac8` | 92 | 72 | `w10-render-d-light-dtor` |
| `textRemoveText__FPv` | `0x00166f98` | 300 | 288 | `w10-render-d-text-remove-text` |
| `textureInitTexture__FP12_worldHeaderP8_texturePUcPc` | `0x0016d278` | 404 | 428 | `w10-render-d-texture-init-texture` |
| `texCacheData__FP8_texturei` | `0x001727e0` | 84 | 68 | `w10-render-d-tex-cache-data` |
| `textureFreeTexture__FP8_texture` | `0x001729c8` | 92 | 88 | `w10-render-d-texture-free-texture` |
| `subfontPrintu__FiiPUsi` | `0x00173408` | 1356 | 1468 | `w10-render-d-subfont-printu` |
| `waterInit__Fv` | `0x00178188` | 444 | 424 | `w10-render-d-water-init` |
| `worldRunWater__FP12_worldHeader` | `0x00178348` | 1272 | 1456 | `w10-render-d-world-run-water` |
| `worldSetWaterProperties__FP8_texture` | `0x0017daa8` | 208 | 288 | `w10-render-d-world-set-water-properties` |
| `usprintf__FPUsPcffffffff` | `0x001919d0` | 76 | 184 | `w10-render-d-usprintf-char` |
| `msg_run__11LightEffect` | `0x001d45a0` | 488 | 436 | `w10-render-d-light-effect-msg-run` |
| `draw__11AddParticleG6Point3P6Point3i` | `0x00223768` | 1136 | 1232 | `w10-render-d-add-particle-draw` |
| `run__11AddParticleG6Point3` | `0x00225150` | 36 | 8 | `w10-render-d-add-particle-run` |
| `debugReport__11AddParticle` | `0x00225230` | 8 | 8 | `w10-render-d-add-particle-debug-report` |
| `_$_11LightEffect` | `0x003117e0` | 124 | 128 | `w10-render-d-light-effect-dtor` |
| `__tf11LightEffect` | `0x00311860` | 76 | 92 | `w10-render-d-light-effect-typeinfo` |
| `__tf11AddParticle` | `0x00315000` | 76 | 92 | `w10-render-d-add-particle-typeinfo` |
| `_$_11AddParticle` | `0x00315050` | 116 | 120 | `w10-render-d-add-particle-dtor` |

Data (not a function), one commit: `spellAlpha`, `spellAlphaInc`, `texLastDecompressionTime`,
`worldTileEntityLightScale` (1.0f) and `waterPatch` in `w10-render-d-render-data`. Sizes and initial bytes
were read from the image; the host check asserts the same. The image keeps the zero ones in an initialised-data
section; the reviewed profile places a zero definition in BSS, so section placement is the linker's matter.

Observed: `run__11AddParticleG6Point3` copies its point to a dead local and does nothing else, and
`debugReport__11AddParticle` is a bare return; both are authored as they are, not as a guess at a fuller
implementation.

## Imports closed (names)

`_$_11AddParticle`, `_$_5Light`, `_$_11LightEffect`, `__tf11AddParticle`, `__tf11LightEffect`,
`clone__11LightEffect`, `debugReport__11AddParticle`, `draw__11AddParticleG6Point3P6Point3i`,
`msg_run__11LightEffect`, `run__11AddParticleG6Point3`, `spellAlpha`, `spellAlphaInc`,
`subfontPrintu__FiiPUsi`, `texCacheData__FP8_texturei`, `texLastDecompressionTime`,
`textRemoveText__FPv`, `textureFreeTexture__FP8_texture`,
`textureInitTexture__FP12_worldHeaderP8_texturePUcPc`, `usprintf__FPUsPcffffffff`, `waterInit__Fv`,
`waterPatch`, `worldRunWater__FP12_worldHeader`, `worldSetWaterProperties__FP8_texture`,
`worldTileEntityLightScale`.

## Not closed (and why)

- `vqDecompress__FP8_texturei` (0x001712f8, 4676 bytes): reviewed, not authored. It builds DMA/GIF packets,
  polls the DMA channel registers at 0x10000000 with `SleepThread` waits, runs a Huffman bit reader with EE
  multimedia instructions (`pextlw`, `pcgtw`, `ppach`, `ppacb`, `plzcw`, `lq`) and expands vector-quantised
  blocks into a 16 by 16 packet layout. The private interpreter has no model for those instructions or the DMA
  registers, so a paired run could not distinguish a right reconstruction from a wrong one; I did not want to commit
  an unverified body. Needs interpreter MMI support and a DMA register model first.
- `draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is` (0x0015c628, 5936 bytes): reviewed, not
  authored. It writes several GS packet sequences directly into the display list, calls a caller-supplied
  iterator callback through `jalr`, uses the old soft-double helpers (`fptodp`, `dptoli`) with `floor` and `ceil`,
  and reads script state; the recognisable pieces are a viewport box from the camera, an icon list at
  `_3map$iconList` (0x006023c8) and a swoosh vertex table (`vu1swooshend`). Too large to author honestly in one sitting.
  Because the double conversions are called by their old names, the authored body would also have to declare
  those helpers explicitly rather than rely on the compiler's conversions.
- `trailParticle` (0x007d6060 and 0x007d7540), `trailParticleSpark` (four addresses), `soulParticle` (0x007d6d80)
  and `slideDustParticleDark` (0x008d5608): 80-byte ParticleDef objects initialised by static constructors. The
  image's symbol table lists several unrelated objects under two of the names, and `soulParticle` is already a
  file-local `static` in `w8-fx-particles-init-soul`, so a second definition here would be a different object from
  the one the constructor fills. The owners of those initialisers need to give them external linkage; I did not
  define them.

## Imports opened

The unit objects leave these undefined (names as the sources use them). Most belong to other packages; the ones
marked (mine?) are things this package's sources reference that no authored unit I found defines:
`P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_10GameObject`, `__8ListHeadRC8ListHead`, `__builtin_delete`,
`__rtti_si`, `__tf10GameObject`, `__ti10GameObject`, `_vt$10GameObject`, `_vt$11AddParticle`, `_vt$11LightEffect`,
`blockAlloc__Fi`, `blockFree__FPv`, `curFont`, `decodeQ`, `decodeQHead`, `decodeQTail` (mine?),
`drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25`, `drawTexture__FR6Packetiiiiiiiii`,
`eRandState`, `eRand__10GameObject`, `ef1Noise__Fi`, `engineFullFrameMode`, `engineGetField__Fv`, `fontCharAlpha`,
`fontColor__FUiii`, `frameNm`, `gameIsPaused__Fv`, `haloTexture2`, `iatan2__Fff`, `lastFontCharAlpha`,
`lastFontColor`, `levelFlags`, `lightAdd__FP5Light`, `listDestroy__FP8ListHead`, `memset`, `nmChars`,
`nmWaterPatches`, `norrath_text_hash_slots` (the alias `textAddText` already uses for the table at 0x006d8fa0),
`p` (the packet object at 0x006e93d0, referenced through an assembler label), `strlen`,
`supersampleMode`, `uncompCache`, `uncompMap` (mine?), `usprintf__FPUsT0ffffffff`, `waterList`, `waterListBack`,
`waterStep`, `waterTexture`, `world`, `worldWaterAlpha`, `worldWaterBumpAmount`, `worldWaterBumpRadius`,
`worldWaterBumpTimer`, `worldWaterColor`, `worldWaterRainTimer`, `worldWaterWavyness`.
`decodeQ`, `decodeQHead`, `decodeQTail`, `uncompCache`, `uncompMap` and `nmWaterPatches`/`waterList`/`waterListBack`
are data the ring, cache and water code use; none is in my import list.

## Build-plan entries needed

Append these (JSON lines; the integrator assigns `input_order` and the snapshot digest). Each source is
compiled with profile `authored_cpp`, no header closure. They define the symbols above, so the matching names leave
the unresolved list. If `waterPatch`, `spellAlpha` and the others are also defined by another unit when the
plans are merged, one of the two definitions must go.

```
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-add-particle-debug-report/debug_report.cpp","object_name":"debug_report.o","sha256":"3db064b764399ee8412aeb133c1f77b7b060f37c1af0f25a37440aa0fe84f479","emitted_functions":[{"name":"debugReport__11AddParticle","compiled_body_bytes":8}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-add-particle-draw/draw.cpp","object_name":"draw.o","sha256":"4dfe8243912317333f725ee1a5e5dff12717fd9ce90c1eb918330a990660438e","emitted_functions":[{"name":"draw__11AddParticleG6Point3P6Point3i","compiled_body_bytes":1232}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-add-particle-dtor/dtor.cpp","object_name":"dtor.o","sha256":"68491f132092140b3d6b3a3bc09c3e69ca911673b7e2b4251b662d5749b70870","emitted_functions":[{"name":"_$_11AddParticle","compiled_body_bytes":120}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-add-particle-run/run.cpp","object_name":"run.o","sha256":"384dc271dcfe502f9d1c36aa4a8e5187923a01e5923190e0e66602d93f9897b9","emitted_functions":[{"name":"run__11AddParticleG6Point3","compiled_body_bytes":8}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-add-particle-typeinfo/typeinfo.cpp","object_name":"typeinfo.o","sha256":"7536bffddccc6ed039e447483fccf94c4c944d6733e85e49e177c84bf36920d9","emitted_functions":[{"name":"__tf11AddParticle","compiled_body_bytes":92}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-light-dtor/light_dtor.cpp","object_name":"light_dtor.o","sha256":"c88e1deb739981d0f3a0bf5706493059c5030b16da28f87c6c3a56dfbcfa7779","emitted_functions":[{"name":"_$_5Light","compiled_body_bytes":72}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-light-effect-clone/clone.cpp","object_name":"clone.o","sha256":"72b6e37c22d68bac78332aa07840f1f886f3532eb97fb3d80bf192358b002063","emitted_functions":[{"name":"clone__11LightEffect","compiled_body_bytes":368}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-light-effect-dtor/dtor.cpp","object_name":"dtor.o","sha256":"3b7b378532bc3ab8afb8163c69e5c324852086f2ab433bc2b66afff2b2efebb8","emitted_functions":[{"name":"_$_11LightEffect","compiled_body_bytes":128}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-light-effect-msg-run/msg_run.cpp","object_name":"msg_run.o","sha256":"25d0247748f04336338d4709bf387fdd47691ce935f34df2fb937c5980543a69","emitted_functions":[{"name":"msg_run__11LightEffect","compiled_body_bytes":436}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-light-effect-typeinfo/typeinfo.cpp","object_name":"typeinfo.o","sha256":"d68cd4031e725571b6a14b089a6345757dfeae4bb86997a78d1968289cd76b45","emitted_functions":[{"name":"__tf11LightEffect","compiled_body_bytes":92}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-render-data/render_data.cpp","object_name":"render_data.o","sha256":"4bb4ad5140c2b6c776fbede2ca223f7f2f51fd45e217c4277ed98f10ebb8af36","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-subfont-printu/subfont_printu.cpp","object_name":"subfont_printu.o","sha256":"ede1e8dff12f6591b4da85d42b90e26bf2e65e6183f008d91b7c804b60133672","emitted_functions":[{"name":"subfontPrintu__FiiPUsi","compiled_body_bytes":1468}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-tex-cache-data/tex_cache_data.cpp","object_name":"tex_cache_data.o","sha256":"08e0b1619ced485de6ad9e43ac67832b190e7949faf1f3b5646e10c3c239cdb0","emitted_functions":[{"name":"texCacheData__FP8_texturei","compiled_body_bytes":68}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-text-remove-text/text_remove_text.cpp","object_name":"text_remove_text.o","sha256":"ea1a776508c7c58f26c1115c65333f00dec370a339e61a7a19c6db793dd109b1","emitted_functions":[{"name":"textRemoveText__FPv","compiled_body_bytes":288}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-texture-free-texture/texture_free_texture.cpp","object_name":"texture_free_texture.o","sha256":"cd9821b4afebba6055e89ca6e537e1089caefe30bab273e5763759946e535572","emitted_functions":[{"name":"textureFreeTexture__FP8_texture","compiled_body_bytes":88}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-texture-init-texture/texture_init_texture.cpp","object_name":"texture_init_texture.o","sha256":"388cf776d4466477265b33b3ac7c04351e7048566c07c9132d4811c081d1dc36","emitted_functions":[{"name":"textureInitTexture__FP12_worldHeaderP8_texturePUcPc","compiled_body_bytes":428}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-usprintf-char/usprintf_char.cpp","object_name":"usprintf_char.o","sha256":"212e97e0970e20913e3d6ca2774d1897556f24c0a6ad448f303b6a776ad635a5","emitted_functions":[{"name":"usprintf__FPUsPcffffffff","compiled_body_bytes":184}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-water-init/water_init.cpp","object_name":"water_init.o","sha256":"a4303e894d6e73f9d359d7219b8d1f8d0e42ce1f886e3d1d5511f6759836e62a","emitted_functions":[{"name":"waterInit__Fv","compiled_body_bytes":424}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-world-run-water/world_run_water.cpp","object_name":"world_run_water.o","sha256":"6eda96016e8250d9f4bfcb07433700089fd364204750df68b779050e895fb0fb","emitted_functions":[{"name":"worldRunWater__FP12_worldHeader","compiled_body_bytes":1456}]}
{"header_closure":[],"kind":"independently_authored_source","profile":"authored_cpp","source":"sources/w10-render-d-world-set-water-properties/world_set_water_properties.cpp","object_name":"world_set_water_properties.o","sha256":"16698ef8aa84c8d069165431c7d374b4d5606452914a6529150fd0d801336ce5","emitted_functions":[{"name":"worldSetWaterProperties__FP8_texture","compiled_body_bytes":288}]}
```

## Authored but never executed

All of it. Host checks ran on this Mac; the paired comparisons ran inside a private interpreter, not on the EE
and not in the game, with every collaborator replaced by a stub that logs its arguments. In particular none of
these ran together with the real `waterStep`, `drawTexture`, `fontColor`, `lightAdd` or the allocator. Float
behaviour was compared under the interpreter's single-precision model; NaN and infinity inputs were not sampled
(`msg_run__11LightEffect` and `draw__11AddParticle...` use plain compares for their clamps, the original uses
`min.s`/`max.s` in places, so NaN behaviour is not established). A zero `worldWaterBumpTimer` in `worldRunWater`
(a signed remainder) and a zero ripple radius were not run: the original divides without a trap check and a fresh
compile may trap or behave differently.

## Unknown

- The meaning of most fields in the light-effect object (offsets 140 to 184), in the AddParticle object
  (+80, +84, +88, +92, +96, +252, +316 to +336, +368 to +384) and in the water patch record (+4752 to +4768);
  the sources describe what is read and written, not what it means.
- Whether index 91 to 104 of the font are really a grey range and what the offset `-4` in supersample text
  does; whether `lastFontColor`'s top byte is an alpha.
- What the second owner halfword in a water patch's link block indexes, and what the `0xa7fb` stored at +10
  of a texture record means.
- The vtable slot called by `msg_run__11LightEffect` with the value 3 is inferred to be the destructor; not
  confirmed.
