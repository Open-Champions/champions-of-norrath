# World, level and game-loop part 4 (w10-world-d)

Package `agents/w10-world-d`. Scope: the unresolved imports named in the task (route, world, selection and resource words). Addresses, sizes and sections come from the hash-pinned private `SLUS_205.65` symbol table and the reviewed EE decoder; they are local evidence, not values the relocatable build reproduces. Nothing here ran on an EE or as gameplay.

## Method

Each function was read from the private image, rewritten in readable C++ in the repository style, compiled with the reviewed EE `authored_cpp` flags, and compared against the original with a private R5900 interpreter (outside git). The comparison runs the original and the authored function from identical memory snapshots, with every callee replaced by the same recorded stub, and compares the call log (arguments and pointed-to bytes), the return value or float result, and every changed byte outside the stack. Original-instruction coverage is reported per function. Each comparison was also run against a deliberately wrong variant of the source to confirm that it fails; that is noted per function. Every unit also has a host check (built with the host compiler, stubs for all callees) that is not an EE plan input.

The interpreter needed one local change (a discarded `mfc0` cycle-counter read returns zero) for routeMakeLattice. No original bytes, disassembly or interpreter output are in the repository.

## Functions

| Symbol | Address | Original bytes | Compiled bytes | Paired check |
|---|---|---|---|---|
| `worldPlotRouteSetEnable__Fi` | `0x0017d5b8` | 8 | 12 | 7 argument values |
| `showHiddenObjects__FP10GameObject` | `0x0018e308` | 64 | 48 | 300 runs |
| `unremoveMonster__FP10GameObject` | `0x0018e3d8` | 80 | 64 | 300 runs; mask mutation caught |
| `worldDeactivateUntakenDoors__FP12_worldHeader` | `0x0017d298` | 124 | 148 | 300 runs incl. count changed by the callee |
| `worldFreeWorld__FP12_worldHeader` | `0x001752f8` | 328 | 328 | 400 runs |
| `worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2` | `0x001755f0` | 480 | 428 | 400 runs |
| `routeClear__Fv` | `0x002bfbd8` | 220 | 180 | 2 prefilled states (a first draft wrote the cache marker at offset 0; the check caught it, it is at offset 24) |
| `worldFindPointHeight__FP12_worldHeaderG6Point3` | `0x00175a60` | 804 | 832 | 600 runs, 200/201 words; orientation mutation caught |
| `worldTestForbidden__FP12_worldHeaderG6Point3f` | `0x001766c8` | 788 | 728 | 500 runs, 195/197 words; rotation mutation caught |
| `worldPrecacheWorld__FP12_worldHeaderP8Matrix34` | `0x0017b080` | 1120 | 1032 | 300 random trees, 280/281 words; flag-mask mutation caught |
| `worldAllocateSegments__FP12_worldHeader` | `0x00174cb0` | 1600 | 488 | 24 runs, 397/401 words (each run is slow in the interpreter); rounding mutation caught |
| `wallAndObjectSteering__FP8CreatureffP10GameObject` | `0x00108618` | 1360 | 1524 | 300 runs, 339/340 words; two mutations caught |
| `routeMakeLattice__Fv` | `0x002bfef8` | 2276 | 1600 | 90 random graphs, 568/569 words; threshold mutation caught |
| `swap__H1Zf_RX01T0_v` | `0x00316478` | 20 | 20 | 6 cases |
| `swap__H1Zs_RX01T0_v` | `0x00316460` | 20 | 20 | 6 cases |

None of these is a byte match; byte sizes differ because the compilers differ. Uncovered words are annulled delay slots and one unreachable path (`routeMakeLattice` with a non-positive node count, which the unit does handle).

### Observed behaviour worth recording

- **Hidden return slot.** `wallAndObjectSteering` returns a 12-byte Point3 through a hidden first argument in the image; the reviewed compiler returns such a struct in registers, so the source writes the slot as an explicit first parameter. The register layout is the same.
- **Tree walk.** `worldPrecacheWorld` reuses the half-word tree format, 32-byte stack entries and clip-mask handling that `worldDrawWorld` (package `w8-render-world-b`) documents, with a root box of +/-100000 on x and y rather than +/-1000000 and a 16-entry stack; it only requests texture data.
- **Segment sizing.** `worldAllocateSegments` loads each flagged record's lump (`"%s.lmp"`), then sizes a cache as the largest sum of nine neighbouring cells of a 100-wide map, each cell worth ceil(entry / 0x20000), times 0x20000 bytes.
- **Route lattice.** `routeMakeLattice` links each node to up to 12 nearest nodes that pass `worldCanPass`, drops candidates that lie roughly along an existing link (detour factor 1.1), then removes a link `i->B` when `i->A->B` is within a factor of 1.25. Link halfwords added this way have bit 15 set. It also fills `route_dirs` with eight directions.
- **Quirk kept:** in `routeMakeLattice` the outer index steps back after a removal when the position is at or past the removed slot; the variant "strictly past" differs only if a node linked to itself, which the paired runs never produced.

## Data objects

Sizes and sections come from the private symbol table; extents of `.bss` objects with a zero ELF size are inferred from the gap to the next symbol and say so in the source header.

| Symbol | Address | Bytes | Section | Notes |
|---|---|---|---|---|
| `routeEnable` | `0x0048418c` | 1 | .sdata | initial 1 |
| `rqHead`, `rqTail` | `0x00484194`, `0x00484198` | 4 each | .sdata | zero |
| `routeQueue` | `0x006ea1a0` | 6144 (inferred, 128 x 48) | .bss | |
| `nodecount` | `0x0048655c` | 4 | .sdata | zero; opened by `routeClear`, `routeMakeLattice`, defined here |
| `worldLastDrawSucceded` | `0x0048412d` | 1 | .sdata | initial 1 |
| `worldWaterRainTimer` | `0x004841c0` | 4 | .sdata | zero |
| `worldActivityLevel` | `0x00429440` | 32 | .data | zero; element type unknown |
| `worldAnimation` | `0x006ebb40` | 3072 (inferred, 64 x 48) | .bss | |
| `worldBoundingBox` | `0x007d0b08` | 24 (inferred) | .bss | |
| `selectLumpName` | `0x00484548` | 4 | .sdata | pointer to the literal `select.lmp` |
| `selectMode`, `thisLevel`, `wasFormatting` | `0x00486cf8`, `0x00486d94`, `0x00486c98` | 4 each (inferred) | .bss | |
| `stage` | `0x004848f4` | 4 | .sdata | zero |
| `userFilterFunction` | `0x00483b00` | 4 | .sdata | same address as `norrath_restore_filter_word`; defined under the original name because callers use it |
| `scale` | `0x0046ccc0` | 12 | .rodata | floats 0.9, 0.6, 0.0 |
| `skeletonResMap` | `0x0042d8d0` | 800 | .data | 99 file names plus a null entry |
| `typeToResource` | `0x00422a10` | 40 | .data | ten pointers to per-race resource maps |

## Not authored

- `routePlot__FR10RouteQueue` (`0x002c0a10`, 19,812 bytes, about 4,950 instructions). It inlines the route-cache reclaim of `routeAddNode`, builds its own heap and buffer templates (`swap` for `RouteHeap`, `RouteBuffer`, int, float), calls `qsort`, `worldHitscan`, `worldHitscanWithObjects`, `routeUnrollVnode__Fi`, `listRemove__FP10__listNode` and the soft-double routines (`fptodp`, `dpadd`, `dpsub`, `dpmul`, `dpcmp`, `dptofp`) twelve-plus times each. That is more than one bounded task here, and a partial reconstruction would earn no credit. It stays an unresolved import of `worldPlotRouteProcess__Fi`. A dedicated agent should take it with its own plan and the node/route layouts recorded above.
- `tlbsrc` (`0x004459d0`, 0x330 bytes, `.data`). The bytes are machine code for the kernel TLB handlers that `_InitSys` copies; authoring them here would only reproduce original instructions as data. It needs an independently written handler and the kernel-patches owner. Left unresolved.

## Imports closed (31)

routeClear__Fv, routeEnable, routeMakeLattice__Fv, routeQueue, rqHead, rqTail, scale, selectLumpName, selectMode, showHiddenObjects__FP10GameObject, skeletonResMap, stage, thisLevel, typeToResource, unremoveMonster__FP10GameObject, userFilterFunction, wallAndObjectSteering__FP8CreatureffP10GameObject, wasFormatting, worldActivityLevel, worldAllocateSegments__FP12_worldHeader, worldAnimation, worldBoundingBox, worldDeactivateUntakenDoors__FP12_worldHeader, worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2, worldFindPointHeight__FP12_worldHeaderG6Point3, worldFreeWorld__FP12_worldHeader, worldLastDrawSucceded, worldPlotRouteSetEnable__Fi, worldPrecacheWorld__FP12_worldHeaderP8Matrix34, worldTestForbidden__FP12_worldHeaderG6Point3f, worldWaterRainTimer.

Also defined because my sources would otherwise open them: `nodecount`, `swap__H1Zf_RX01T0_v`, `swap__H1Zs_RX01T0_v`.

Confirmed by a private partial build (below): of the 33 names in the task, 31 left the unresolved list; `routePlot__FR10RouteQueue` and `tlbsrc` remain.

## Imports opened

New symbols my sources reference that no unit of mine defines (the build lists them as unresolved):

- `route_dirs` (`0x008d8230`, 96 bytes in `.bss`): written by `routeMakeLattice`.
- The ten per-race maps behind `typeToResource`: `maleBarbResourceMap`, `femBarbResourceMap`, `maleWoodElfResourceMap`, `femaleWoodElfResourceMap`, `maleHighElfResourceMap`, `femaleHighElfResourceMap`, `maleEruditeResourceMap`, `femaleEruditeResourceMap`, `maleDarkElfResourceMap`, `femaleDarkElfResourceMap` (each a dynamically initialised resource-map object).
- Callees owned by other packages and already unresolved at the shared tip: `collisionFrame`, `mt_dyndat`, `mt_throwaway`, `texLoadAlwaysLoadedPages__Fv`, `texClearAlwaysLoadedPages__Fv`, `textureFreeTexture__FP8_texture`, `worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii`, `worldCanPass__FP12_worldHeaderG6Point3T1fi`, `makeCollisionList__FP12_worldHeaderffffPP13collisionParti`, `worldTransformFrustum__FP8Matrix34`, `texCacheData__FP8_texturei`.

## Build-plan entries needed

A private plan copy was produced and the full partial build run once with it (3,493 selected inputs at this branch's shared tip, 2,499 function bodies, 1,318 unresolved symbols) outside the repository. To reproduce:

```
python3 docs/agents/w10-world-d-plan-entries.py --toolchain /path/to/reviewed/ee --private-root /path/outside/git/plan
```

This prints the entries below and writes a private plan and snapshot; the shared `experimental/build-plan.json` and `experimental/snapshot.json` are untouched. The lead integrator merges these 19 entries (no unit needs a header closure):

```
{"emitted_functions": [{"compiled_body_bytes": 672, "name": "sceCdDiskReady_old"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "disk_ready_old.o", "profile": "authored_cpp", "sha256": "d7ab203ff55c0a73127139a47d346ba4d6118dc7f4293d57f345755663b1e518", "source": "sources/w10-sce-a-cd-disk-ready-old/disk_ready_old.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "connect_to_iop_server"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "connect_to_iop_server.o", "profile": "authored_cpp", "sha256": "4b8f2f25370e3b900f29e96efd41ac21c5752be83a49d48da0288302a76cf959", "source": "sources/w10-sce-a-connect-to-iop-server/connect_to_iop_server.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "sceGifPkAddGsAD"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_add_gs_ad.o", "profile": "authored_cpp", "sha256": "dde8e5f35b552e5c4a0b1d495087abfdb19cf780a141c294d6dab3bde4a0fa8e", "source": "sources/w10-sce-a-gif-pk-add-gs-ad/pk_add_gs_ad.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "sceGifPkCloseGifTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_close_gif_tag.o", "profile": "authored_cpp", "sha256": "559240b3d41f2b5aba1b884b3f9ccd137a296d9dde8fadba1c996c9c5f421dee", "source": "sources/w10-sce-a-gif-pk-close-gif-tag/pk_close_gif_tag.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "sceGifPkCnt"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_cnt.o", "profile": "authored_cpp", "sha256": "84da8143a924ee1ca53e5d1a4b69ea8ee48a3d8f21adf307cc9a60f8837125c6", "source": "sources/w10-sce-a-gif-pk-cnt/pk_cnt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "sceGifPkOpenGifTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_open_gif_tag.o", "profile": "authored_entry_asm", "sha256": "f5803c9a5f7bbf4d6b31ea12c979159ae04ecda9879c970fca17443ab679c166", "source": "sources/w10-sce-a-gif-pk-open-gif-tag/pk_open_gif_tag.S"}
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "sceGifPkRef"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_ref.o", "profile": "authored_cpp", "sha256": "e6073ba5c460be13a14d294b6759d4b429c5dba43f32e2c7f734edecb37c435c", "source": "sources/w10-sce-a-gif-pk-ref/pk_ref.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "sceGifPkReserve"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pk_reserve.o", "profile": "authored_cpp", "sha256": "9a42f078955fd354a5cb79a3e0510afe6b1710beaab25c9839a2d0905635ffe9", "source": "sources/w10-sce-a-gif-pk-reserve/pk_reserve.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "sceInetCreate"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inet_create.o", "profile": "authored_cpp", "sha256": "c2f45b29d3011424a3b1de0a535de3c3f667ea8900c96271d936e073761a8b74", "source": "sources/w10-sce-a-inet-create/inet_create.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "sceInetInterfaceControl"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inet_interface_control.o", "profile": "authored_cpp", "sha256": "9450f73209fd2dbd35367e6d0c1d0588f1671190e6dbaad5fdc60cf0e5a523a9", "source": "sources/w10-sce-a-inet-interface-control/inet_interface_control.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "sceInetOpen"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inet_open.o", "profile": "authored_cpp", "sha256": "9ba779b54138f6ceef638e70bd491d63b4c0995fe5f88fc08bb78e2bfc01c62d", "source": "sources/w10-sce-a-inet-open/inet_open.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 264, "name": "sceInetRecvFrom"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inet_recv_from.o", "profile": "authored_cpp", "sha256": "19d6c3016c0e57ee7fac0159a8443895774e6f82a4743a941c75e952b4e7e1d3", "source": "sources/w10-sce-a-inet-recv-from/inet_recv_from.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 240, "name": "sceInetSendTo"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inet_send_to.o", "profile": "authored_cpp", "sha256": "154ecc3ca5097d17658638972f689ab551f04e131c2ba430bc5e66a6394be41c", "source": "sources/w10-sce-a-inet-send-to/inet_send_to.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "read_done"}, {"compiled_body_bytes": 408, "name": "lgAudARead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_a_read.o", "profile": "authored_cpp", "sha256": "c808b73289fd2ae112c1c1f46bb4fa6ded7ae407a0fa4d86a43f0103b2548879", "source": "sources/w10-sce-a-lgaud-a-read/lgaud_a_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "lgAudASync"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_a_sync.o", "profile": "authored_cpp", "sha256": "6aa79378643aa10895b8c08d251cef545eeb26a8d0cabc4e39ca0b7ae37da6af", "source": "sources/w10-sce-a-lgaud-a-sync/lgaud_a_sync.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "write_done"}, {"compiled_body_bytes": 440, "name": "lgAudAWrite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_a_write.o", "profile": "authored_cpp", "sha256": "d84a67a2951a2e010c13e9fe91b783fa89413e65cebb27400ade5685fc585114", "source": "sources/w10-sce-a-lgaud-a-write/lgaud_a_write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "lgAudEnumHint"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_enum_hint.o", "profile": "authored_cpp", "sha256": "abec6035475e0a6e62b6079b046597a9cdb6d8bdd8d8f587a295cb1ccd83ce8f", "source": "sources/w10-sce-a-lgaud-enum-hint/lgaud_enum_hint.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 376, "name": "lgAudEnumerate"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_enumerate.o", "profile": "authored_cpp", "sha256": "27f0b52d7bb58daae76a797c3211f3f81b10af5acdee39c50b003c12e51b710d", "source": "sources/w10-sce-a-lgaud-enumerate/lgaud_enumerate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 424, "name": "lgAudOpen"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_open.o", "profile": "authored_cpp", "sha256": "af3d661770f3bed02d37a15b2cb07be4981db97351f1d2f63905210e81b99ffb", "source": "sources/w10-sce-a-lgaud-open/lgaud_open.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 244, "name": "lgAudSetRecordingVolume"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_set_recording_volume.o", "profile": "authored_cpp", "sha256": "08adf2c13435d6aa8ea1ac7cf86d89cfb48523f49459aed8b729f3e60ff68e85", "source": "sources/w10-sce-a-lgaud-set-recording-volume/lgaud_set_recording_volume.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 220, "name": "lgAudStartPlayback"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_start_playback.o", "profile": "authored_cpp", "sha256": "38fa11eb505a8df02c6ddb3eb6ea715d92e8d0fa6bdb73a43dd75f1886f00eb3", "source": "sources/w10-sce-a-lgaud-start-playback/lgaud_start_playback.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 220, "name": "lgAudStartRecording"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_start_recording.o", "profile": "authored_cpp", "sha256": "50e545bccf771e37ddd4dbe69bb5f65ce66b2a5bb08c88e61e3ab81e04cbebfb", "source": "sources/w10-sce-a-lgaud-start-recording/lgaud_start_recording.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgaud_state.o", "profile": "authored_cpp", "sha256": "9cacdfc405ae1c13e4d0c66f11f7570079ad191eeb015234502f4f5058e66bd9", "source": "sources/w10-sce-a-lgaud-state/lgaud_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "lgCodecDecode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgcodec_decode.o", "profile": "authored_cpp", "sha256": "626a90ad9b71d9ce01de8840679cebc9c0adf99e2210fc128750e92c83ab234c", "source": "sources/w10-sce-a-lgcodec-decode/lgcodec_decode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "lgCodecEncode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "lgcodec_encode.o", "profile": "authored_cpp", "sha256": "299bf104d7f16beeb2c1abf3b6a660fc275a35f499957ad0119e12121f8da171", "source": "sources/w10-sce-a-lgcodec-encode/lgcodec_encode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "mceSifRpcEndFunc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "mce_sif_rpc_end_func.o", "profile": "authored_cpp", "sha256": "00e6439a0bea49d7465aea26a13c94cd835a2eaaa82ef8200b6a89dfbfc19dcb", "source": "sources/w10-sce-a-mce-sif-rpc-end-func/mce_sif_rpc_end_func.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "mceStorePwd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "mce_store_pwd.o", "profile": "authored_cpp", "sha256": "27d58a3c179d9fa91527d963cfdfbc398aa8a00c344ec500a832cab7b9045f1b", "source": "sources/w10-sce-a-mce-store-pwd/mce_store_pwd.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "scePadGetButtonMask"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_get_button_mask.o", "profile": "authored_cpp", "sha256": "b11305ab717a871fe0c8d201dea53a370b36263dc14e0ad15d2dfaa27f058d73", "source": "sources/w10-sce-a-pad-get-button-mask/pad_get_button_mask.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "scePadSetButtonInfo"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_set_button_info.o", "profile": "authored_cpp", "sha256": "fe240bbed063502ce65c51a8d8fb4401b5d98d63814be0266217c03d5b41567b", "source": "sources/w10-sce-a-pad-set-button-info/pad_set_button_info.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "scePadSetReqState"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_set_req_state.o", "profile": "authored_cpp", "sha256": "0efcd58044ff81a0a6dcbe32995f565a94d69cc9e725f217ad80d588d500d77a", "source": "sources/w10-sce-a-pad-set-req-state/pad_set_req_state.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "s_pad.o", "profile": "authored_cpp", "sha256": "59dd360ededee6d17e92bd29105a1e94c6df3f27d7e800252760d028b9cf9359", "source": "sources/w10-sce-a-s-pad/s_pad.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "sceSifDmaStat"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_dma_stat.o", "profile": "authored_entry_asm", "sha256": "7a9cf1cec54865bf6a04fffa9ecbc6fadc88e8cc00768d66a5f60aa62e79194b", "source": "sources/w10-sce-a-sif-dma-stat/sif_dma_stat.S"}
{"emitted_functions": [{"compiled_body_bytes": 568, "name": "sceSifMCallRpc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_m_call_rpc.o", "profile": "authored_cpp", "sha256": "9735ad2eef67f1904deda01ce892a6d7cbe8077fb3b515986b6d6ddeeb1ac0f6", "source": "sources/w10-sce-a-sif-m-call-rpc/sif_m_call_rpc.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "sceSifMInitRpc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_m_init_rpc.o", "profile": "authored_cpp", "sha256": "5b1e358ceae3b2da30c7e1033e2fa187e08017094643ccbd8a4346b7015c4f77", "source": "sources/w10-sce-a-sif-m-init-rpc/sif_m_init_rpc.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_param_fname.o", "profile": "authored_cpp", "sha256": "452a158661fc516a4c311283c4052e9bf86f7d2026706498132fcf63a44ba148", "source": "sources/w10-sce-a-sif-param-fname/sif_param_fname.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "sceSifRemoveRebootNotifyHandler"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_remove_reboot_notify_handler.o", "profile": "authored_cpp", "sha256": "071bbd9e31a108011e7b70c105b3d6202c05e5dd6303dd529c3b6f3654db6600", "source": "sources/w10-sce-a-sif-remove-reboot-notify-handler/sif_remove_reboot_notify_handler.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "sceSifGetSreg"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "sif_get_sreg.o", "profile": "authored_cpp", "sha256": "3d5a2dd076540dcd27ed979937aa0222cc09edd82a85a16c31126314c397a201", "source": "sources/w8-startup-sce-d-sif-get-sreg/sif_get_sreg.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 312, "name": "sceMcRead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "mc_read.o", "profile": "authored_cpp", "sha256": "ca8da0b1e9e2594887d6356ade7bb33daf267e5c2367633803add6b6c757e05a", "source": "sources/w9-startup-crt-f-mc-read/mc_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 396, "name": "sceMcWrite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "mc_write.o", "profile": "authored_cpp", "sha256": "6630fbf3a10aa09030708f2da5825c81d9e848f4a8e576e9036a9dbf55292aac", "source": "sources/w9-startup-crt-f-mc-write/mc_write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 216, "name": "scePadRead"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_read.o", "profile": "authored_cpp", "sha256": "70542f095450df1beb2ffa48f0cf169eec83c40d95f23417cf6ab6cd4d945266", "source": "sources/w9-startup-crt-f-pad-read/pad_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "scePadSetActDirect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_set_act_direct.o", "profile": "authored_cpp", "sha256": "1023e5e6a1697c3282f2a7c68f0844939a49f641f86379d926deabbaa1757cd2", "source": "sources/w9-startup-crt-f-pad-set-act-direct/pad_set_act_direct.cpp"}
{"emitted_functions": [], "header_closure": ["sources/w9-startup-crt-f-tty-state/tty_state.hpp"], "kind": "independently_authored_source", "object_name": "tty_state.o", "profile": "authored_cpp", "sha256": "22b10554112bda4eab039abfcf07a7bc4c82c491617eff55221e389b0543711b", "source": "sources/w9-startup-crt-f-tty-state/tty_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "sceTtyInit"}], "header_closure": ["sources/w9-startup-crt-f-tty-state/tty_state.hpp"], "kind": "independently_authored_source", "object_name": "tty_init.o", "profile": "authored_cpp", "sha256": "14cd11107a15a079d377abf266ed375891dc2f9653d23a74abe928d19b4ae75c", "source": "sources/w9-startup-crt-f-tty-init/tty_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "sceTtyRead"}], "header_closure": ["sources/w9-startup-crt-f-tty-state/tty_state.hpp"], "kind": "independently_authored_source", "object_name": "tty_read.o", "profile": "authored_cpp", "sha256": "12d0ca3da4b4b6fc2555b7cc33c621ab03bf55ff59c227591f2bbe9e288f0bb6", "source": "sources/w9-startup-crt-f-tty-read/tty_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 412, "name": "sceTtyWrite"}], "header_closure": ["sources/w9-startup-crt-f-tty-state/tty_state.hpp"], "kind": "independently_authored_source", "object_name": "tty_write.o", "profile": "authored_cpp", "sha256": "d18ccd23ffab3b46ece8e2c3988b9c8586e819772366c881059cb92705e0fad8", "source": "sources/w9-startup-crt-f-tty-write/tty_write.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "pad_dma_buffer.o", "profile": "authored_cpp", "sha256": "5de751d33077b2f59d498c66f38b809407b16293c09dbab0a6e0bd1026726e4c", "source": "sources/simloop-services-pad-dma-buffer/pad_dma_buffer.cpp"}
```

## Authored but never executed

All 15 functions and the 4 data units compile with the reviewed EE profile. They ran only in the private interpreter against recorded stubs and in host checks against stubs; none ran on EE hardware, an emulator or inside the original process. The interpreter does not model cache, interrupts, timing or faults. The cycle-counter reads in `routeMakeLattice` are discarded in the image and not reproduced. Hit-scan, passability, collision-list, texture and lump callees were stubbed, so the checks establish the argument and ordering contract only. The two `swap` functions and the data objects are exercised in the same way.

## Unknown

- The names of the world header, the 60-byte world record, the 28-byte door record, the 64-byte route node, the 36-byte part and the creature structure; the meaning of the flag bits used (`0x80`, `0x800`, `0xc880`, bit 15 of a link, bit 1 and bit 0 of a part's flag halfword).
- What the second word of a `skeletonResMap` entry is for (always zero in the image), and the layout of the race maps.
- The element type of `worldActivityLevel` and the real extents of the `.bss` objects with a zero ELF size.
- Whether the `texmap` array, declared 0x1900 bytes in the image, is indexed beyond that by `worldAllocateSegments` for 100 x 100 cells.
- Everything about `routePlot` and `tlbsrc` (above).
