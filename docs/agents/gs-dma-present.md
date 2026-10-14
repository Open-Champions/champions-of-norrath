# GS DMA frame present

Work stays on `agents/gs-dma-present`. Takeover found a clean worktree at
`e49d0b4`, with 24 committed package definitions and no handoff note. No
uncommitted predecessor edits needed retention or removal. The pinned image
SHA-256 was rechecked: `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Original edition remains unresolved. All addresses below refer to that image.

The inherited 25 C++ host contracts passed again with ASan/UBSan; all 26
source units compiled with the reviewed EE profiles. This is not execution
on an EE. Source comments retain per-symbol observed addresses and behavior.

## frameStartPacket__FUibi

Observed at `0x0018fac0`, 832 bytes. Constructs DMA/GIF headers and the two
frame/depth, scissor, offset and control contexts. The old engineYOffset is
packed before its new value is stored. Color zero skips the clear sprites;
nonzero emits 20 pairs, including the final strip. Full mode skips the last
two scissor writes. The packet is written through the uncached alias and
sent using the ordinary frameStatePacket address, followed by sync-path.
The fixed construction makes the original alternate end-tag branch unreachable.
Host cases cover both modes, both fields, both color paths, every output word,
the first untouched word, and call order. Fresh EE compilation passed.

Inferred: the register-number pairs are GS commands. Unknown: hardware timing,
valid screen-slide range, interrupts, and actual frame presentation.

## lightVU0StoppedProcessing__Fv

Observed at `0x0013ce70`, 648 bytes. Calls `lightResetOptionalParams__Fv`
on entry and exit. Stores 8388608.0 to the four VU words at `0x1100ffd0`
through `0x1100ffdc`, the unnamed `.lit4` 8388736.0 to `0x1100ffcc`, the
float of the scene offset-52 word biased by `0x80000000` to the three words
at `0x1100ffc0` through `0x1100ffc8`, and `worldCameraPos[2] * 0.25` to
`0x1100ffac`, in that store order. Four `worldCharViewMatrix` column lengths
are square-rooted into dead stack slots. The eight scene bytes at offsets
44..51 are scaled by 1.0, or by the `globalCharLightScale` word when
`interfaceMode` is nonzero, rounded half-to-even, clamped to 0..255, and
stored at offsets 36..43. Host cases cover both scale paths, half-even ties,
both clamp edges, both bias wraps, all nine VU stores with order, and the
double reset call. Fresh EE compilation passed with no new imports.

Inferred: round-to-nearest-even matches `cvt.w.s` under the default FCSR
mode. Unknown: FCSR state, VU/converter side effects, the scene word
meanings, and why the matrix lengths are discarded.

## doSuperSample__Fiib

Observed at `0x0018fe00`, 992 bytes. A changed mode word rebuilds the
sample packet with `(mode != 0)` and updates `madePacket.769`, defined here
with its `-1` file value. Gray 128 is used unless `useComposite` selects the
composite words. A nonzero `trippleBufferMode` with alpha below 128 emits
the fixed 12-quadword `frameStatePacket` (tag `0x70000005`, DMA/GIF header,
one buffer word mixing both display bases, and the constant tail) and sends
it before the sync-path wait; the alternate tag arm is dead. `copyScreenAlpha`
takes alpha clamped to 0..128, `copyScreenColor` takes the doubled scaled
color packed R/G/B with round-half-even and 0..255 clamps, and the three
composite words step toward the 124..133 band (`>= 133` drops 4, 124..132
becomes 128, below grows 4). Brightness at or above 0.2 (NaN included) bumps
`compositeAlpha` by 3 and pins the scale to 0.2. `copyScreenFrame` takes the
shifted selected base with `0xa0000`, then `FlushCache(0)`, the `copyScreen`
send, and a final sync-path wait. Host cases cover mode caching, both buffer
selects with exact packet words, all three skip paths, call order, both
brightness paths plus NaN, alpha clamps, saturation via out-of-band words,
and every stepper edge. Fresh EE compilation passed with no new imports.

Inferred: the packet words are DMA/GIF descriptors. Unknown: hardware
packet effects, FCSR mode, caller argument meanings, and valid color ranges.

## reloadDMA__Fi

Observed at `0x0015ed18`, 2488 bytes. Always returns 1. An uninitialized
engine with any cause but -1 runs sync/ei and returns. Cause -2 bumps
`donesGot.753` when `channelDone.747` has bit 1 (else clears
`flushing.755`) and stores 2 at `0x12001000`. Cause -1 flips
`readBuffer.745`, marks initialized, clears the bucket, `texTypes.758`,
`sentDone.752`, both done counters, points all four texture queues at the
draw-record head, and sets `channelDone.747` to 0 (2 with an empty head).
A 2-valued `channelDone.747` with matching counters scans bucket slots for
a new head, locking and skipping nodes. The drain masks `channelBusy.746`
with `~(1u << (cause & 31))`, walks the tex/vif queues against DMA
counters, sweeps on cause 1, repoints on cause 2, and honors `flushing.755`.
Uploads build GIF tags at computed tex/vif list addresses (lock/cacheIn
selected by halfwords, s3 budget vs `drawSizeOverEnable`), close with
`0x70000000`, kick lengths at `0x1000a000`/`0x10009000`, and flip the lists.
The tail signals `endDrawSemaphore` (EIntr/SignalSema/DIntr for cause -1,
iSignalSema otherwise) once the bucket reaches 8 with idle channels. All
21 state words are defined here with file values (zeros, `drawSizeOverEnable`
1); the zero-initialized ones land in BSS rather than `.sdata` under the
reviewed profile. Host cases cover entry gating, both -2 arms, reframe,
tex/vif drains, the cause-1 sweep, cause-2 repoint, flushing return, both
semaphore tails, upload/cachein/small paths with exact store words, the s3
budget trip, the vif 6-node batch close, and the refill lock walk. Fresh EE
compilation passed with no new imports.

Inferred: computed addresses are uncached DMA-list aliases; the dead tag
accumulator and dead stack slots carry no behavior. Unknown: hardware DMA
effects, texture heap layout, valid counter ranges, and the texture
halfword meanings.

## makeSamplePacket__Fb

Observed at `0x0018f1f8`, 1492 bytes. Builds 577 quadwords, including two
70-tile passes and the end controls. Full mode changes vertical destination
scale and scissor height. The texture base is reloaded between passes.
Publishes copyScreenFrame at byte 48, copyScreenColor at 144, and
copyScreenAlpha at 180, then FlushCache(0). Every packet word and the first
untouched word pass the two-mode host contract; reviewed EE compilation passed.
No packet was submitted to hardware. Original alternate end-tag path is
unreachable for these fixed loop bounds.

## Package completion

All 29 assigned names are now reconstructed on this branch: the 26
inherited units plus `lightVU0StoppedProcessing__Fv`,
`doSuperSample__Fiib`, and `reloadDMA__Fi`. Each unit's source comment
records its observed address and behavior; per-symbol detail for the
inherited 26 lives there. Takeover verification: all 27 inherited host
checks pass again here (24 C++ contracts with ASan/UBSan plus the
`AddDmacHandler` 16-byte body match against the pinned image), all 29
units compile with the reviewed EE profiles, and `python3 -m unittest
discover -s tools -p 'test_*.py'` passes (214 tests).

### Imports closed

Package-defined names (29 assigned plus in-package state): `AddDmacHandler`,
`EnableDmac`, `checkModelVersion`, `coreLoadingScreenRunning`, `draw`,
`drawFrameNm`, `drawRemoveInterrupts__Fv`, `drawRestoreInterrupts__Fv`,
`drawStart__Fv`, `drawWaitFinished__Fv`, `engineSetVideoMode__Fb`,
`frameStartPacket__FUibi`, `gsIntHandler__Fi`, `lastFrameVBlankCount`,
`lightVU0StoppedProcessing__Fv`, `makeSamplePacket__Fb`,
`doSuperSample__Fiib`, `madePacket.769`, `powerOffMode`, `reloadDMA__Fi`
with its 21 state words (`initialized.744`, `readBuffer.745`,
`channelBusy.746`, `channelDone.747`, `drTexLoad.748`, `drTexLoaded.749`,
`drDraw.750`, `drDone.751`, `sentDone.752`, `donesGot.753`,
`donesNeeded.754`, `flushing.755`, `preTex.756`, `preVif.757`,
`texTypes.758`, `drawSizeOverEnable`, `bucket`, `texDmaListFlip`,
`vifDmaListFlip`, `texDmaListPos`, `vifDmaListPos`), `sceDmaPause`,
`sceDmaPutEnv`, `sceDmaSend`, `sceGsSyncV`, `specialMpegMode`,
`switchTo30Counter.798`, `switchToHalfCounter.797`,
`textureDrawColor__FP8_textureiiiUi`, `textureInternalInit__Fv`,
`texturePackDown__Fv`, plus `gsHID`. Internal call edges now linked inside
the package: `reloadDMA__Fi` (used by `drawRestoreInterrupts__Fv`,
`drawStart__Fv`, `gsIntHandler__Fi`), `gsIntHandler__Fi`, `AddDmacHandler`,
and `EnableDmac` (used by `drawRestoreInterrupts__Fv`), and
`makeSamplePacket__Fb` (used by `doSuperSample__Fiib`).

### Imports opened (still unresolved)

`drawHead`, `drawRecords`, `nmDrawUsed`, `writeBuffer`, `endDrawSemaphore`,
`displayBuffer1StartAddress`, `displayBuffer2StartAddress`,
`engineDrawBufferStartAddress`, `engineYOffset`, `engineScreenSlide`,
`engineScissory1`, `engineScissory2`, `dmaGif`, `fullFrameMode`, `vif0HID`,
`vif1HID`, `gifHID` (draw-texture-init per its notes); `DIntr`, `EIntr`,
`WaitSema` (initsys callees); `iSignalSema`, `AddIntcHandler`,
`EnableIntc`, `DisableIntc`, `RemoveIntcHandler`, `sceDmaGetChan`,
`CheckAddress`, `WaitDma`, `sceGsSyncPath`, `SignalSema` (gs-dma-kernel
owns most; `SignalSema` is not in its closed list); `DisableDmac`,
`RemoveDmacHandler`, `RemoveIntcHandler`, `FlushCache`, `VSync`, `VSync2`,
`sceGsGetGParam` (kernel/syscall side); `compositeR`, `compositeG`,
`compositeB` (render-lights screen color); `memset` (runtime-memory);
`atoi`, `sceOpen`, `sceRead`, `sceClose` (libc/syscall side);
`CreateThread`, `StartThread` (thread services);
`lightResetOptionalParams__Fv`, `scene`, `worldCharViewMatrix`,
`worldCameraPos`, `interfaceMode`, `globalCharLightScale` (light/world
side, unowned); `textureLock__FP8_texture`,
`textureUnlock__FP8_texture`, `textureCacheIn__FP8_texture`,
`textureUncompBufferLocked`, `cacheClear__Fv`,
`_textureDrawColor__FP8_textureiiiUi`, `cache`, `dummyTex`, `uncompMap`,
`texturePageTable`, `texturePageSegLoc`, `segmentUsed`, `texturePageBlock`,
`texturePageLoc`, `texdecompress`, `decompressStack`, `cacheHead`,
`cacheTail`, `uncompSpin`, `decompressThreadid`,
`texDecompressThread__FPv`, `makeUnscrambleTable__Fv` (texture subsystem,
unowned); `trippleBufferMode`, `engineDisable30fps`, `engineFullFrameMode`,
`compositeAlpha`, `copyScreen`, `copyScreenFrame`, `copyScreenColor`,
`copyScreenAlpha`, `frameStatePacket`, `engineBrightnessCorrection`,
`sceDmaCurrentEnv` (owner not established; no package defines the
copy-screen/frame-state packets yet). `reloadDMA__Fi` is declared `void`
by the already-committed `gsIntHandler__Fi`; the observed body returns 1
and this unit declares it `int`, which links identically.

### Build-plan entries needed

Do not add any `host_contract.cpp`/`host_check.cpp`. The integrator assigns
`input_order`. Profiles are `authored_cpp` except the noted assembly unit.
Emitted body sizes were measured from fresh EE objects; data-only units
list no functions. The `do_super_sample` local helper is listed because it
emits a local text symbol.

```json
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_add_dmac_handler.o","profile":"authored_entry_asm","sha256":"b8aaf3926799a893db0720f673f2b7e3d935a6d1bf4af18d4d97fb7904c8fecc","source":"sources/gs-dma-present-add-dmac-handler/add_dmac_handler.S","emitted_functions":[{"name":"AddDmacHandler","compiled_body_bytes":16}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_check_model_version.o","profile":"authored_cpp","sha256":"4baf5dfba2f98bf12c58f6e9541cf5bdc29bdac9845d88c9f396ac0e3647c769","source":"sources/gs-dma-present-check-model-version/check_model_version.cpp","emitted_functions":[{"name":"checkModelVersion","compiled_body_bytes":216}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_core_loading.o","profile":"authored_cpp","sha256":"d120a6c55a0537b809269a16caf0f2b0d86508f2524d608469dfebdc1fa221f0","source":"sources/gs-dma-present-core-loading/core_loading.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_do_super_sample.o","profile":"authored_cpp","sha256":"ac011ff6b83bd50350db910c9881069ae83123148d03df5ad3e0c2622da1dec3","source":"sources/gs-dma-present-do-super-sample/do_super_sample.cpp","emitted_functions":[{"name":"doSuperSample__Fiib","compiled_body_bytes":984}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw.o","profile":"authored_cpp","sha256":"51db443cac0c97aef2d2e4cc8a11684bbf8eeb1c8aedca9987f3cb414f96a92f","source":"sources/gs-dma-present-draw/draw.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw_frame_nm.o","profile":"authored_cpp","sha256":"185857e1747a18524123235e70b32dce31e86b021612946e93f64e2ac5b5d921","source":"sources/gs-dma-present-draw-frame-nm/draw_frame_nm.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw_remove_interrupts.o","profile":"authored_cpp","sha256":"12f3acc7b5502d740b05f7a042fe0bda60918945feea0907d6f45d50f280ce86","source":"sources/gs-dma-present-draw-remove-interrupts/draw_remove_interrupts.cpp","emitted_functions":[{"name":"drawRemoveInterrupts__Fv","compiled_body_bytes":112}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw_restore_interrupts.o","profile":"authored_cpp","sha256":"49a0999147be294630e4276efadd65e388bc28331c228d6b621ff51dd6f59e87","source":"sources/gs-dma-present-draw-restore-interrupts/draw_restore_interrupts.cpp","emitted_functions":[{"name":"drawRestoreInterrupts__Fv","compiled_body_bytes":268}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw_start.o","profile":"authored_cpp","sha256":"a0c67725e3467f975d475d2cd5938e1e3f4c2e96e212b950096fca8420733198","source":"sources/gs-dma-present-draw-start/draw_start.cpp","emitted_functions":[{"name":"drawStart__Fv","compiled_body_bytes":156}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_draw_wait_finished.o","profile":"authored_cpp","sha256":"9a2dd240b30b52ce1b504eb9169313bde0189daaa348877918485e197d811fa0","source":"sources/gs-dma-present-draw-wait-finished/draw_wait_finished.cpp","emitted_functions":[{"name":"drawWaitFinished__Fv","compiled_body_bytes":12}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_enable_dmac.o","profile":"authored_cpp","sha256":"c370de245064ca2eee5403c8cf020f8939ff559446a18ff425726023b3d0785d","source":"sources/gs-dma-present-enable-dmac/enable_dmac.cpp","emitted_functions":[{"name":"EnableDmac","compiled_body_bytes":112}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_engine_set_video_mode.o","profile":"authored_cpp","sha256":"6ff65e50d230bbc0a0300d112b30583871abb9b1126552505332577cfbaccb08","source":"sources/gs-dma-present-engine-set-video-mode/engine_set_video_mode.cpp","emitted_functions":[{"name":"engineSetVideoMode__Fb","compiled_body_bytes":84}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_frame_start_packet.o","profile":"authored_cpp","sha256":"7782a5dee73f9c4661ed7038735def0243f05b13ffbbfb895382c93ad20c39f9","source":"sources/gs-dma-present-frame-start-packet/frame_start_packet.cpp","emitted_functions":[{"name":"frameStartPacket__FUibi","compiled_body_bytes":584}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_gs_int_handler.o","profile":"authored_cpp","sha256":"d6685c05967839284a2f87c7f14d6e70531fd561756b9ff40401e4d997f4e7ea","source":"sources/gs-dma-present-gs-int-handler/gs_int_handler.cpp","emitted_functions":[{"name":"gsIntHandler__Fi","compiled_body_bytes":40}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_last_frame_vblank.o","profile":"authored_cpp","sha256":"0c219e014b3dfb83fb423d937fb69cc06fd1ce9de400cc3883b91d6c4dac0610","source":"sources/gs-dma-present-last-frame-vblank/last_frame_vblank.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_light_vu0_stopped.o","profile":"authored_cpp","sha256":"eb2762591ffbadefd7632107b295c782ac3cf284cbd42ca97dbe54168f94b551","source":"sources/gs-dma-present-light-vu0-stopped/light_vu0_stopped.cpp","emitted_functions":[{"name":"lightVU0StoppedProcessing__Fv","compiled_body_bytes":588}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_make_sample_packet.o","profile":"authored_cpp","sha256":"c7d143f95c0fc3b5672864bb7be790a00d0f24647492cae44edfe8425dcb819e","source":"sources/gs-dma-present-make-sample-packet/make_sample_packet.cpp","emitted_functions":[{"name":"makeSamplePacket__Fb","compiled_body_bytes":804}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_power_off_mode.o","profile":"authored_cpp","sha256":"315d62e4f66abbf1ec418a19cc6c68d8f72d5c113d868d3f015d164b6f887958","source":"sources/gs-dma-present-power-off-mode/power_off_mode.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_reload_dma.o","profile":"authored_cpp","sha256":"b58c26a2084a2aeaf15ee9e5296561013562b84598113953984421f55c38262d","source":"sources/gs-dma-present-reload-dma/reload_dma.cpp","emitted_functions":[{"name":"reloadDMA__Fi","compiled_body_bytes":2928}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_sce_dma_pause.o","profile":"authored_cpp","sha256":"4a112182f8927e0577a34b041b7ff78f4b0f879ee86e096828909964d2e750e9","source":"sources/gs-dma-present-sce-dma-pause/sce_dma_pause.cpp","emitted_functions":[{"name":"sceDmaPause","compiled_body_bytes":140}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_sce_dma_put_env.o","profile":"authored_cpp","sha256":"495586867b373c4d1b6167445ddc59e9d504704dfb5c1300cb44787c77ddc097","source":"sources/gs-dma-present-sce-dma-put-env/sce_dma_put_env.cpp","emitted_functions":[{"name":"sceDmaPutEnv","compiled_body_bytes":436}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_sce_dma_send.o","profile":"authored_cpp","sha256":"83bb4794a79041d4595ae1f1c86b85d56e2bc9beff2e39f1120fa41324fe2afc","source":"sources/gs-dma-present-sce-dma-send/sce_dma_send.cpp","emitted_functions":[{"name":"sceDmaSend","compiled_body_bytes":100}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_sce_gs_sync_v.o","profile":"authored_cpp","sha256":"af8491514220e2a182bd42ff25a972389dbf6040a8b71e02a69cb5f98aa7a8c4","source":"sources/gs-dma-present-sce-gs-sync-v/sce_gs_sync_v.cpp","emitted_functions":[{"name":"sceGsSyncV","compiled_body_bytes":168}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_special_mpeg_mode.o","profile":"authored_cpp","sha256":"41ad56c97116f350ece731b7089cda80ffe1fbd166239b06d9bdc76da3153478","source":"sources/gs-dma-present-special-mpeg-mode/special_mpeg_mode.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_switch_30_counter.o","profile":"authored_cpp","sha256":"77aa9b2b06308cfd3aa72049c0a9c978994eda5658f2773779a7b688db41c6ae0639af8ae27b8a6fa61eadb","source":"sources/gs-dma-present-switch-30-counter/switch_30_counter.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_switch_half_counter.o","profile":"authored_cpp","sha256":"d7b4822084729f4aa72049c0a9c978994eda5658f2773779a7b8861e521c1930","source":"sources/gs-dma-present-switch-half-counter/switch_half_counter.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_texture_draw_color.o","profile":"authored_cpp","sha256":"cc7b0ee6b1c3f5545b09f2cbddd9b91db9b4ce011d171b7319066d01c0ad7586","source":"sources/gs-dma-present-texture-draw-color/texture_draw_color.cpp","emitted_functions":[{"name":"textureDrawColor__FP8_textureiiiUi","compiled_body_bytes":8}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_texture_internal_init.o","profile":"authored_cpp","sha256":"30d2519185f096188416633faf1ac09a344cbccc6fd36d87aeebdf6d4e9dc1c2","source":"sources/gs-dma-present-texture-internal-init/texture_internal_init.cpp","emitted_functions":[{"name":"textureInternalInit__Fv","compiled_body_bytes":564}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"present_texture_pack_down.o","profile":"authored_cpp","sha256":"804f3476906aacf4f523185af9f240a6a934567e35a70edc423839c8d6dfedaa","source":"sources/gs-dma-present-texture-pack-down/texture_pack_down.cpp","emitted_functions":[{"name":"texturePackDown__Fv","compiled_body_bytes":44}]}
```

A private link of these objects was not run here; the shared plan was not
edited. The object names are proposals; the integrator renumbers and may
rename on collision.

### Authored but never executed

Every function above was compiled with the reviewed EE toolchain and, except
the data-only words and the byte-matched `AddDmacHandler`, checked by a host
contract or body comparison on this Mac. No GS, DMA, VU, semaphore, timer,
cache, thread, ROM, or interrupt call has run on the EE. No packet was sent
to hardware, no frame was presented, and no texture bytes were uploaded.
The zero-initialized words land in BSS under the reviewed profile rather
than the original `.sdata`/`.sbss` homes.

### Unknown

Hardware effects of every DMA/GIF/VIF packet, register store, counter
read, and kick; valid ranges for colors, counters, brightness, bucket
positions, and s3 budgets; FCSR rounding state behind the float-to-int
conversions; the texture struct layout beyond the touched offsets and the
meanings of its halfwords; what the ROM version path, `0x10000000` timer
word, and scene words select; caller argument meanings for the sample and
frame packets; and which package will own the copy-screen/frame-state
packets, `trippleBufferMode`, `compositeAlpha`, and the texture callees.
No startup stage or gameplay has run.

### Verification log (takeover)

`git status` was clean at takeover; the branch tip was `d1f9d67` with no
handoff note and no uncommitted changes, so nothing was kept or discarded.
`lightVU0StoppedProcessing__Fv`, `doSuperSample__Fiib`, and `reloadDMA__Fi`
were reconstructed one per commit, each with an ASan/UBSan host contract
and a fresh reviewed-EE compile with no new imports, and each commit was
pushed to `origin/agents/gs-dma-present` individually. During
`reloadDMA__Fi` work the host contract caught two transcription bugs (a
skipped queue advance in the cause-1 sweep and a misplaced LINK on the
lock path) plus three wrong test literals; all were fixed against the
pinned bytes. Final state: 29 host checks pass (25 inherited contracts,
the 3 new ones, and the `AddDmacHandler` body match), all 29 sources
compile, `python3 -m unittest discover -s tools -p 'test_*.py'` passes
(214 tests), and the worktree is clean.
