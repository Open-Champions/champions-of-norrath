# particles

Package work is on `agents/particles`. New source is under `experimental/sources/particles-*`. Host checks sit beside that source and are not EE plan inputs: they include host headers. The reviewed profile is `authored_cpp`. Each unit was compiled with those flags using the reviewed EE g++. The official driver rejects a plan whose inputs differ from `snapshot.json`, so that check was a direct compile kept outside git. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

Addresses are from the pinned SLUS-20565 symbol table. Extents for size-0 BSS symbols are the bytes to the next symbol. Fresh EE objects are not original-byte matches. No particle system, trail, noise table, list, or object draw has run on the EE. The reviewed `-G0` profile places zero objects in BSS, including words whose file section is `.sdata`, `.sbss`, or `.data`.

`listDestroy__FP8ListHead` is 144 bytes at `0x00194300`. It loads the node at the head and the `freeNodes` word. An empty head returns without storing `freeNodes`. Each node unlinks `next` at +0 and `prev` at +4, reloading those words after a store. A nonzero twin at +8 is unlinked the same way and pushed onto the free list, then the node is pushed. The walk continues from the next word captured before those stores.

`lightResetOptionalParams__Fv` is 316 bytes at `0x0013cb88`. It scales `scene` bytes 48, 49, and 50 by the `.lit4` bits `0x3f4ccccc` and stores the low 16 bits of the `cvt.w.s` results. `charAmbient` receives scene bytes 40 through 43. `charDirect` receives bytes 36 through 39. `charDirect2` receives the three scaled values and scene byte 39. `charScaleR`, `charScaleG`, and `charScaleB` receive 1.0f. A signed `interfaceMode` of 10 or more is reduced by 10. Twelve bytes at scene+24 go to `charDirectDir2`, and twelve bytes at scene+12 go to `charDirectDir`. `ambientOnly` and `disableVolumetricLighting` are stored as zero. The host contract rounds ties to even so it matches `cvt.w.s`. The EE source uses a C++ cast, which the reviewed g++ lowers to `cvt.w.s`.

`initParticleSystems__Fv` is 56 bytes at `0x00180228`. It clears the word at offset 304 of each of the 16 `PS` records, last record first, then stores 1 in `psSequenceNm`.

`initTrails__Fv` is 112 bytes at `0x00184248`. It clears the halfword at the start of each of the 64 `trail` records and byte 1836 of each of the 20 `bolt` records, last record first, then stores 1 in `trailSequenceNm`.

`allocDescriptor__FP11ParticleDef` is 384 bytes at `0x001872c0`. Slot 0 is taken when `descListMark` word 0 is `0xffffffff`. Otherwise the scan starts at slot 1 and stops at the first word that is `0xffffffff` or at index 32. Index 32 returns 0 and writes nothing. A free slot stores 1 in the mark, stores the index at descriptor+4, copies 64 bytes from descriptor+16 into `descList + index * 64`, and returns 1.

`initNoiseTable__Fv` is 452 bytes at `0x001581b8`. It calls `powf(8, -1)`, `powf(4, -1)`, `powf(2, -1)`, and `powf(1, -1)`. The seed is `eRandState`. Each generated sample steps `seed * 0x19660d + 0x3c6ef35f`, then uses `(seed >> 16) * 2^-16 - 0.5`, except outputs 241 through 255 use 0. Octave n is skipped when the output index's low n bits are not all zero, and a skipped octave adds its previous sample again. The dot of the four accumulators with the four powers is multiplied by bits `0x471c4000` and stored with `cvt.w.s` as 256 halfwords. The seed is written back. From seed 0 and zero accumulator and sample words, the host contract's final seed is `0x1adddd60` and the first halfwords are 10, 2080, 6031, 8885, 10139, 13204, 14827, 14034.

`drawParticlesTask__Fv` is 32 bytes at `0x00180018`. It calls `P_DrawParticles__FP8Matrix34` with `worldCameraMatrix`.

`drawObjectsTask__Fv` is 1852 bytes at `0x0013f930`. It is registered as a task. A zero `objectDrawEnable` byte returns immediately; any other byte, including `0x80`, enters. The walk starts at mode `0x8000` and corners (0, 0) and (119, 119). A mode other than `0x803f` is replaced by `worldClipCubeToFrustum` of the cell cube `(x0<<8)-15456`, `(x1<<8)-15008`, `(y0<<8)-15456`, `(y1<<8)-15008`, `cameraZ-840`, `cameraZ`. A zero result discards the cell. A cell that still spans more than one column or row splits on the longer axis, or on Y when the spans are equal. The high half is visited first. A one-cell result walks `objectGrid`, then `ghostObjectGrid`, through the safe-list iterator with `objectIgnoreFlags`. Bit 2 of the word at object+12 skips that object. The object cube is center floats +16, +20, +24, radius byte +40, and z extra byte +41. A display-list remainder below 3000 destroys the iterator and returns. The remainder is `GLISTSIZE` minus the arithmetic shift of `glistp - gstart`. Mode `0x803f` skips the cubes and clears `modelClippingOn` before the object grid. Other modes call `worldClipCubeToClipFrustum` with the frustum call's return and store 0 or 1 in `modelClippingOn`. The draw adds the signed halfword at vtable+40 to the object and calls the word at vtable+44. `alwaysDrawList` is walked last with `modelClippingOn` set to 1, and that byte is cleared when the walk finishes. This source keeps pending cells in a 256-entry array. That bound holds the host contract, which discards every cell that does not contain (119, 119).

## Data

Initial bytes are the file or BSS value. Later init functions write these objects; those writes are not part of the definitions.

| Name | Address | Extent | Initial |
|---|---|---|---|
| `freeNodes` | `0x00486a6c` | 4 | zero pointer |
| `nodes` | `0x007ad000` | 121920 | zero, 6096 records of 20 |
| `objectGrid` | `0x004fc0c0` | 57600 | zero |
| `ghostObjectGrid` | `0x0050a1c0` | 57600 | zero |
| `scene` | `0x004f4060` | 56 | zero |
| `farClip` | `0x004869d8` | 4 | zero |
| `frameStates` | `0x006162c0` | 580224 | zero, 150 records of 3868 plus 24 |
| `animResults` | `0x006a3d40` | 156800 | zero, 100 records of 1568 |
| `identityResult` | `0x006ca1c0` | 1600 | zero |
| `fsList` | `0x0048690c` | 4 | zero pointer |
| `arList` | `0x00486914` | 4 | zero pointer |
| `arListBack` | `0x00486918` | 4 | zero pointer |
| `table` | `0x006d8fa0` | 32768 | zero |
| `stringHash` | `0x005182c0` | 2048 | zero |
| `objectHash` | `0x00518ac0` | 2048 | zero |
| `offTheGrid` | `0x00486890` | 4 | zero |
| `objectStrings` | `0x00486898` | 4 | zero |
| `descListMark` | `0x00780280` | 128 | zero |
| `descList` | `0x00780300` | 2048 | zero |
| `invisible` | `0x006f5a80` | 80 | zero |
| `block` | `0x00700080` | 524288 | zero |
| `wigglyTab` | `0x006ffe60` | 544 | zero |
| `nmSingle` | `0x004869f8` | 4 | zero |
| `nmLines` | `0x004869fc` | 4 | zero |
| `P_FrameNum` | `0x004869f4` | 2 | zero |
| `PS` | `0x006f5ad0` | 4992 | zero |
| `psSequenceNm` | `0x00484308` | 4 | 1 |
| `trail` | `0x00780b00` | 66560 | zero |
| `bolt` | `0x006f6e50` | 36880 | zero |
| `trailSequenceNm` | `0x00484310` | 4 | 1 |
| `noiseTable` | `0x006021c8` | 512 | zero |
| `worldCameraMatrix` | `0x0037ef00` | 48 | zero |
| `worldCameraPos` | `0x0037ee80` | 12 | zero |
| `objectDrawEnable` | `0x00483ac8` | 1 | 1 |
| `objectIgnoreFlags` | `0x00483ac0` | 4 | zero |
| `modelClippingOn` | `0x0048407c` | 1 | zero |
| `alwaysDrawList` | `0x00486888` | 8 | zero |

`nodes` at `0x007ad000` is the list pool. The route table at `0x00438f08` remains `norrath_route_nodes_bytes`. `offTheGrid` here is the word at `0x00486890`. A second local `offTheGrid` at `0x00486848` is a different object. `trail` here is `0x00780b00`. Two other local `trail` symbols, at `0x008d7040` and `0x008d7090`, are different objects. `block` here is the BSS pool at `0x00700080`. The `.srs` symbols named `block` are different objects. `table` is the animation table. `noiseTable` is separate. `alwaysDrawList` has a zero ELF size field; eight bytes is the gap to `offTheGrid`. `trailSequenceNm` is four bytes; `cutMode` is at `0x00484318`. `worldCameraPos` is 12 bytes. `worldViewCenter` follows at `0x0037ee90` and `worldFrustum` at `0x0037eea0`.

`freeNodes`, `objectGrid`, `ghostObjectGrid`, `stringHash`, `objectHash`, `offTheGrid`, and `objectStrings` are the original names of bytes object-infra published as `norrath_constructor_list_pool_view`, `norrath_constructor_grid_a_view`, `norrath_constructor_grid_b_view`, `norrath_constructor_string_names_view`, `norrath_constructor_string_objects_view`, `norrath_constructor_default_head_view`, and `norrath_constructor_string_count_view`. The plan rejects aliases, so these units are a second link object at a different address from those views. `objectInit` indexes the grids and hashes as words. `drawObjectsTask` indexes the grids as bytes. Both views are 57600 and 2048 bytes.

`glistp`, `gstart`, and `GLISTSIZE` stay the frame-loop objects. `eRandState` stays the subsystem-inits object. `pad`, `pad_dma_buf`, and `stateData` stay with the pad package.

## Imports closed

`listDestroy__FP8ListHead`, `lightResetOptionalParams__Fv`, `initParticleSystems__Fv`, `initTrails__Fv`, `allocDescriptor__FP11ParticleDef`, `initNoiseTable__Fv`, `drawParticlesTask__Fv`, `drawObjectsTask__Fv`, `freeNodes`, `nodes`, `objectGrid`, `ghostObjectGrid`, `scene`, `farClip`, `frameStates`, `animResults`, `identityResult`, `fsList`, `arList`, `arListBack`, `table`, `stringHash`, `objectHash`, `offTheGrid`, `objectStrings`, `descListMark`, `descList`, `invisible`, `block`, `wigglyTab`, `nmSingle`, `nmLines`, `P_FrameNum`, `PS`, `psSequenceNm`, `trail`, `bolt`, `trailSequenceNm`, `noiseTable`, `worldCameraMatrix`, `worldCameraPos`, `objectDrawEnable`, `objectIgnoreFlags`, `modelClippingOn`, `alwaysDrawList`.

## Imports opened

`P_DrawParticles__FP8Matrix34`, `powf`, `worldClipCubeToFrustum__FPA1_fi`, `worldClipCubeToClipFrustum__FPA1_fi`, `__16ListSafeIteratorP8ListHeadi`, `next__16ListSafeIterator`, `_$_16ListSafeIterator`, `ambientOnly`, `charAmbient`, `charDirect`, `charDirect2`, `charDirectDir`, `charDirectDir2`, `charScaleB`, `charScaleG`, `charScaleR`, `disableVolumetricLighting`, `interfaceMode`.

`glistp`, `gstart`, and `GLISTSIZE` are referenced and already defined by frame-loop. `eRandState` is referenced and already defined by subsystem-inits. These subsystem-inits imports stay with their packages: `engineAddTask__FPFv_viPci`, `icos__Fi`, `isin__Fi`, `memset`, `P_Clear__Fv`, `P_Update__Fv`, `pad`, `pad_dma_buf`, `padInitPadIndexMap__Fv`, `runObjectsTask__Fv`, `sceMtapInit`, `sceMtapPortOpen`, `scePadInit`, `scePadPortOpen`, `stateData`, `transformSceneLightTask__Fv`, `worldSetFrustum__Fffff`.

## Build-plan entries needed

Append these authored inputs. Empty `header_closure`. Profile `authored_cpp`. Kind `independently_authored_source`. The integrator assigns `input_order`. Host checks are not inputs.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "list_destroy.o", "profile": "authored_cpp", "sha256": "2e63e459b4df359ea141e67eba200b65471b7d2e8a2f79463683c4e574d6a6d5", "source": "sources/particles-list-destroy/list_destroy.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "light_reset.o", "profile": "authored_cpp", "sha256": "c9f8edb634246efd80cd8d2292a6f8cde95760ade977bf50b7293a8f2d545b8d", "source": "sources/particles-light-reset/light_reset.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "init_particle_systems.o", "profile": "authored_cpp", "sha256": "18409b7f1ee70b9e6455499bcf7cf7b0c54c3830ef7b3cecb8f55419de59fc08", "source": "sources/particles-init-particle-systems/init_particle_systems.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "init_trails.o", "profile": "authored_cpp", "sha256": "23ea1b6c5573dc4f0805066e713187f2ff5f4bd441ef79115baab28ba797e263", "source": "sources/particles-init-trails/init_trails.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "alloc_descriptor.o", "profile": "authored_cpp", "sha256": "c4af109e165dc7c94ab1c7532eae7ff6e71c72292b2079c489f18be43d03fa7e", "source": "sources/particles-alloc-descriptor/alloc_descriptor.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "init_noise.o", "profile": "authored_cpp", "sha256": "ac3110fdc5175b1cc7368c6d60d1492fa3d94d081fb1a652130eee88da9a1178", "source": "sources/particles-init-noise/init_noise.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "draw_particles_task.o", "profile": "authored_cpp", "sha256": "b90655ef205574d9a2eeb306b368095986e9657b9863c717b0573a6551252ba3", "source": "sources/particles-draw-particles-task/draw_particles_task.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "draw_objects_task.o", "profile": "authored_cpp", "sha256": "07c1a012c48f67378bc4587aa7a274553c10c809406c1e393aaf8272939bd6a5", "source": "sources/particles-draw-objects-task/draw_objects_task.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "free_nodes.o", "profile": "authored_cpp", "sha256": "fccdca0cb0aa100edf9b59c5a53b2347f9e1e1921172f13557c2f77e89cefbf5", "source": "sources/particles-free-nodes/free_nodes.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "nodes.o", "profile": "authored_cpp", "sha256": "32e3b886ed86bd49105b2d82f22aa5d4627bd25aad660d40a1395dba42455848", "source": "sources/particles-nodes/nodes.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_grid.o", "profile": "authored_cpp", "sha256": "4174591bb7d5c4c68298f03368b7e88002576803933aa99827ea09534851af29", "source": "sources/particles-object-grid/object_grid.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ghost_object_grid.o", "profile": "authored_cpp", "sha256": "720cd09eef65c08c060a280b516851c71e5fce2879afb0a902bb9de38be8166d", "source": "sources/particles-ghost-object-grid/ghost_object_grid.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "scene.o", "profile": "authored_cpp", "sha256": "62132d07f2f2fa6c3f3f1fed2bb6d26515692066a108ea4ecc9cab07567ee62d", "source": "sources/particles-scene/scene.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "far_clip.o", "profile": "authored_cpp", "sha256": "2fe5ceb3dd46ee3e0996bfe7e0cde84d9f444a986ede07ee7e044dd8caaa8af3", "source": "sources/particles-far-clip/far_clip.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "frame_states.o", "profile": "authored_cpp", "sha256": "a483415a541921d27e4afb1842ad122797f8168c012965d4152adc680876d9c8", "source": "sources/particles-frame-states/frame_states.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_results.o", "profile": "authored_cpp", "sha256": "cb4f251b2f310bc01208cf7c4c8e30ed85777e452cc50b3c202326d102244636", "source": "sources/particles-anim-results/anim_results.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "identity_result.o", "profile": "authored_cpp", "sha256": "a740c301093222828878e95eda5b014876eeab739fcf000d393cfa4809b35323", "source": "sources/particles-identity-result/identity_result.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "fs_list.o", "profile": "authored_cpp", "sha256": "6f04cd68b2d02fb500287658a0b33a0f46e6de7a6fa57a5081d350147ac07e09", "source": "sources/particles-fs-list/fs_list.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ar_list.o", "profile": "authored_cpp", "sha256": "8c1cc7145c84df2f807f4d2b92672c3437b1b32656df882f54a7cefff0306769", "source": "sources/particles-ar-list/ar_list.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ar_list_back.o", "profile": "authored_cpp", "sha256": "1345748936c07263dc0a93c05f29a94cd348acbef07d7d97026b9265d0ca97f2", "source": "sources/particles-ar-list-back/ar_list_back.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_table.o", "profile": "authored_cpp", "sha256": "b725f405ca3e76ab9ddbb3486944ccac456ecfa5432ed3c9e1734974aaf6213d", "source": "sources/particles-anim-table/anim_table.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "string_hash.o", "profile": "authored_cpp", "sha256": "e72b53cff5fbe27be0eb3783890fc5ac9b1e2aeb529928ac9e8916abff56fd54", "source": "sources/particles-string-hash/string_hash.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_hash.o", "profile": "authored_cpp", "sha256": "dbc1404bbf092369b590332961d1383892416934348587acc0304db303ed3cee", "source": "sources/particles-object-hash/object_hash.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "off_the_grid.o", "profile": "authored_cpp", "sha256": "963d6ef386aedc6dcc44efa33d192942d58a61649315ecc3cb2c3596c4eb505f", "source": "sources/particles-off-the-grid/off_the_grid.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_strings.o", "profile": "authored_cpp", "sha256": "e2a73c4417b6b9e5c43c4818aa603b996eeeedab27bb0568f3f2f715ad3ec95a", "source": "sources/particles-object-strings/object_strings.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "desc_list_mark.o", "profile": "authored_cpp", "sha256": "7d707abcc55f58727fa5f2c8b6172f12b8d8d59572f677c88c9c5074c5357045", "source": "sources/particles-desc-list-mark/desc_list_mark.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "desc_list.o", "profile": "authored_cpp", "sha256": "60dbfb1d58fb35eba4e58d4af258acebf82e0a7182b4a16553fb5acb60db73fb", "source": "sources/particles-desc-list/desc_list.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "invisible.o", "profile": "authored_cpp", "sha256": "44ae67d1c6722c6af0f0e806d0ce7c13b07c10fb3a14667f5b2c6d7a7b20e8db", "source": "sources/particles-invisible/invisible.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "particle_block.o", "profile": "authored_cpp", "sha256": "88b0cc1fad92b7a14b121e6f02f9b1e2c37bc4dc8588c1e818365ab65d3adf6f", "source": "sources/particles-particle-block/particle_block.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "wiggly_tab.o", "profile": "authored_cpp", "sha256": "a9a0ca7b59e1e002526ef069260203b5b75f3b99f107993fffd3eb2bdebcf5a9", "source": "sources/particles-wiggly-tab/wiggly_tab.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "nm_single.o", "profile": "authored_cpp", "sha256": "a238a9b241412fb99533537a4f2871fa2adf949947eefbb4037c61bee9cb5b50", "source": "sources/particles-nm-single/nm_single.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "nm_lines.o", "profile": "authored_cpp", "sha256": "93e284120e270d9a83ba87ae07054a5a688a20f31c11374e53eff641a9bddf0f", "source": "sources/particles-nm-lines/nm_lines.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "p_frame_num.o", "profile": "authored_cpp", "sha256": "37ba21f9b811e1a854c95d6d667810cd18969945386f5f404ee3882fedb9b821", "source": "sources/particles-p-frame-num/p_frame_num.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "particle_systems.o", "profile": "authored_cpp", "sha256": "b1582629d20cc18ed190ba138ab445bb6b21baf1c012d7ab9a7dd6a1a444a4df", "source": "sources/particles-particle-systems/particle_systems.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ps_sequence.o", "profile": "authored_cpp", "sha256": "cbfec073858ac9c18e3ccbbcbc287050197a0de0c91aecd5173426defbe3502b", "source": "sources/particles-ps-sequence/ps_sequence.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "trail.o", "profile": "authored_cpp", "sha256": "0c5069fc2bb4bb5aac2b43b7776d115e30a7c6f78db377e12a02f786c30bd4f7", "source": "sources/particles-trail/trail.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "bolt.o", "profile": "authored_cpp", "sha256": "75a50b88a2d236588ad30cf775ae5bafb4e036fe7ad5fc4e3450eda2128e06b2", "source": "sources/particles-bolt/bolt.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "trail_sequence.o", "profile": "authored_cpp", "sha256": "c9b6a6e99faaf466cb143a3d8a9fac41da2945dd67623d0144260337e4472ea8", "source": "sources/particles-trail-sequence/trail_sequence.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "noise_table.o", "profile": "authored_cpp", "sha256": "8d0adbcd5076b0bf113564da6962ec05fa4875f67ceb4776f86728e3dc187f28", "source": "sources/particles-noise-table/noise_table.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "world_camera_matrix.o", "profile": "authored_cpp", "sha256": "ce1f0b948a7ef3779695962eed3205ff663d456a835b349168f7994002bc732e", "source": "sources/particles-world-camera-matrix/world_camera_matrix.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "world_camera_pos.o", "profile": "authored_cpp", "sha256": "bd3734804cd76e30f6ceacf4928a66de4ac168ecde09fe5c17e230edc3cea279", "source": "sources/particles-world-camera-pos/world_camera_pos.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_draw_enable.o", "profile": "authored_cpp", "sha256": "70056c889f0cc3c45a54d2a76b1c4d5942d31bb0d4bc93cb60a9917162ce0d4c", "source": "sources/particles-object-draw-enable/object_draw_enable.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_ignore_flags.o", "profile": "authored_cpp", "sha256": "6088e77e54cd896db8830396cc72e741e9cda893a8ae7980005ed8b7883f4d00", "source": "sources/particles-object-ignore-flags/object_ignore_flags.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "model_clipping.o", "profile": "authored_cpp", "sha256": "13c11cb714b23505a58515207469766bcbf349f1b171baa3dc59542b6621036a", "source": "sources/particles-model-clipping/model_clipping.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "always_draw_list.o", "profile": "authored_cpp", "sha256": "996f30fa896ff79e1ef28f2f35509e7962628c9743fdd187671611940dc91715", "source": "sources/particles-always-draw-list/always_draw_list.cpp"}
```

## Authored but never executed

Every function and data object above was compiled with the reviewed EE g++ and checked by a host contract. Nothing in this package has run on the EE. Fresh objects are not original-byte matches. `initNoiseTable` was checked from seed 0 with accumulator and sample words started at zero. `drawObjectsTask` was checked with a clip stub that keeps only the path to cell (119, 119). The 256-entry cell array was not filled by an unpruned 120 by 120 walk.

## Unknown

The noise accumulator words above the first, and the sample words, are unread stack before the first generated sample. This source starts them at zero. That stand-in is what the host halfwords check. Bytes in a list node after the three link words are not read by `listDestroy`. The eight-byte `alwaysDrawList` extent is the gap to the next symbol. The four bytes between `trailSequenceNm` and `cutMode` have no symbol. `worldViewCenter` and `worldFrustum` sit between `worldCameraPos` and `worldCameraMatrix` and stay outside this package. `descListMark` is zero in the image; `P_Init` fills it with `0xffffffff` later. `farClip` is zero in the image; `worldInit` stores bits `0x44e10000` later. Whether `worldSetFrustum` reads that word is outside this package. The original binding of the local `.sdata` and `.sbss` symbols is global in these units because `-G0` does not recreate that binding. The pending-cell storage in the original `drawObjectsTask` was not recovered as a 256-entry array. `P_DrawParticles__FP8Matrix34` and the three list-iterator bodies are unresolved. The light words named in Imports opened are unresolved and belong with the light package.
