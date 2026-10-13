# w9-other-a: game-loop plumbing, net command queue and misc state (a)

Package branch: `agents/w9-other-a`. Every function source sits beside a `host_check.cpp` (not an EE plan input,
compiled and run on the host with call mocks). Every source also compiles with the reviewed `authored_cpp` profile.
No compiled body is claimed to match the original bytes. The data units use the form `extern "C" T name = 0;`, which
the compiler warns about ("initialized and declared extern"); the size and zero image value come from the symbol table
and the file bytes.

## Functions (observed behavior, each header holds the details)

- `commProcessReal__Fv` (0x002cc4a8, 1992 bytes): the voice-chat pump. Checks: host check drives the early returns,
  the device walk and open, a full read/encode/send/mix/write pass, short reads and writes, busy and failing syncs.
  The 133-byte packet layout (132 encoded + local player byte) and the 4 senders x 16 slots in `packetbuffer` are
  observed; the field names for those are not claimed.
- `advanceFrame__Ff`, `commandQueueEmpty__Fv`, `lineToY__Fi`, `DLG_GetDialogCreature__Fv`, `get__11ResourceMap`:
  see the source headers.
- `initLightInfo__Fv` (0x00280510): copies ten rows of eight floats into the first 80 floats of `lightInfo`
  (0x008d5e40, 384 bytes). The original goes through a stack copy of a 96-float table first; that copy is not
  reproduced. Host check: values, untouched tail, idempotence.
- Data (size, zero image): `cheatInvulnerability`, `cheatMode`, `compImmediateCompressor`,
  `compImmediateDecompressor`, `compLocalCompressor`, `compLocalDecompressor` (80 bytes each), `curDlgStart`,
  `curPlayer`, `decodeQHead`, `decodeQTail`, `deferedCall` (10240), `displayDialogPos`, `dlgDir`, `flags`,
  `GLibData` (1640), `headerAlpha`, `hpos` (12 x Point3), `lastYAnchor`, `nextFrameDrop`.

## Imports closed

DLG_GetDialogCreature__Fv, GLibData, advanceFrame__Ff, cheatInvulnerability, cheatMode, commProcessReal__Fv,
commandQueueEmpty__Fv, compImmediateCompressor, compImmediateDecompressor, compLocalCompressor,
compLocalDecompressor, curDlgStart, curPlayer, decodeQHead, decodeQTail, deferedCall, displayDialogPos, dlgDir, flags,
get__11ResourceMap, headerAlpha, hpos, initLightInfo__Fv, lastYAnchor, lineToY__Fi, nextFrameDrop.

## Imports opened (left unresolved, owned elsewhere or not in this package)

- advanceFrame__Ff: action, actor, animFindComponentWithFlags__FP14AnimationStatei, cutMsgHandler, cutPlayMode,
  cutSkipMode, cutTime, dialogActor, displayActionPos, displayFrame, eRandState, executeAction__FP6Action,
  netGetNmConnectedPartners__Fv, netNmReady, netSendMessage__FiPUci, netWeSentReady, nmActions, nmActors,
  soundGetStreamAmountPlayed__Fi, soundGetStreamPlayStatus__Fi, worldPlotRouteProcess__Fi.
- commProcessReal__Fv: buffr, buffrfull, buffrmode, buffrout, codec, commCVol, commEnable, commHandle, commNameMask,
  commPV, commRVInternal, commRVInternalLast, commRVThresh, commSendmask, commStatusHandle, comm_handle, drawFrameNm,
  framestart, frontEndRunning, lastCE, lastlastCE, lgAudARead, lgAudASync, lgAudAWrite, lgAudClose, lgAudEnumHint,
  lgAudEnumerate, lgAudOpen, lgAudSetRecordingVolume, lgAudStartPlayback, lgAudStartRecording, lgCodecDecode,
  lgCodecEncode, memset, netGetLocalPlayerNmSafe__Fi, netIsModem__Fv, netNetworkMode, netSendMessage__FiPUci,
  netSendPacket__FiiPUci, packetbuffer, packetshift, readcount, timeout, writecount.
- commandQueueEmpty__Fv: queueEnd, queueStart. DLG_GetDialogCreature__Fv: curDlg, dialogCreature.
  get__11ResourceMap: lumpFindResource__FPcT0, resourceFrame. initLightInfo__Fv: lightInfo (384-byte zero image at
  0x008d5e40; not defined here because it was not in the package list).

## Build-plan entries needed (path under `experimental/`, sha-256)

- `sources/w9-other-a-advance-frame/advance_frame.cpp` `48d1aae68216e413ff3c6b2ee774aae40feb766bacd618b8a4488c440e7f47b9`
- `sources/w9-other-a-cheat-invulnerability/cheat_invulnerability.cpp` `05c6ea0f32215bce6a8fe8dc61a9fc818c8f2125aafecc4c6015eca916032664`
- `sources/w9-other-a-cheat-mode/cheat_mode.cpp` `52fe889b16e3b5d64d3c5046ae4b24a22969336a0b824da8d1caa0943be77444`
- `sources/w9-other-a-comm-process-real/comm_process_real.cpp` `47e43e097ab5876beac756442750cef57c055c351f172d8488f2cb3bb2479193`
- `sources/w9-other-a-command-queue-empty/command_queue_empty.cpp` `0c74c3d1b757a0c903681612e0c2b4b13aac65cb885475eb186084605a710c96`
- `sources/w9-other-a-comp-immediate-compressor/comp_immediate_compressor.cpp` `413e264191e59a319475de24871bddad843afd96899a065ef0c6ed0eb4591a50`
- `sources/w9-other-a-comp-immediate-decompressor/comp_immediate_decompressor.cpp` `2f1b9c3aedd061861c7debe657053bf40f9d3c9c8cbaa81b83732ea99663449f`
- `sources/w9-other-a-comp-local-compressor/comp_local_compressor.cpp` `7d00e3f4124dc7e687f08001faa163ab456773c4f11c1acfd206653d2b988164`
- `sources/w9-other-a-comp-local-decompressor/comp_local_decompressor.cpp` `bf28770c68206c10bae04c4c554c8ffa501fcca887689acff2e37a0c5b2c665a`
- `sources/w9-other-a-cur-dlg-start/cur_dlg_start.cpp` `93b4222783be6c2134b04840f57984b0c80e99627636fa298f98773ff2be346b`
- `sources/w9-other-a-cur-player/cur_player.cpp` `06858fb866b65d534bb710e0c752346a6edf38bfaedf6c1a4f11a638d72131ff`
- `sources/w9-other-a-decode-q-head/decode_q_head.cpp` `7778081e58902509dcd4442dc191526408c930b2d2462e0054a3b5aeca51aac3`
- `sources/w9-other-a-decode-q-tail/decode_q_tail.cpp` `7643bae4de964f9b6eda4e5f28ebd110441a1a25ead560fc3b636441e91f9c69`
- `sources/w9-other-a-defered-call/defered_call.cpp` `3f243ab794a35e5420f3fda9b27d97c6005efd40b89643632c21234c81c73ca0`
- `sources/w9-other-a-display-dialog-pos/display_dialog_pos.cpp` `e631afde374f645a9de89ce530e524bdc7ab9452fd754e45c8c03a6c54d163c3`
- `sources/w9-other-a-dlg-dir/dlg_dir.cpp` `176f5685175363b05818fdb445604152844da3c84d92563374eaddb367a33d15`
- `sources/w9-other-a-dlg-get-creature/dlg_get_creature.cpp` `045f74b4cb3e9114b344e2e87a225b1fa5290d3de669c2dc4979c964204bc7d2`
- `sources/w9-other-a-flags/flags.cpp` `ef2e3e0ea110711c250131773cc078a875e678862fca9cbe3a0f82908bd939eb`
- `sources/w9-other-a-g-lib-data/g_lib_data.cpp` `e20474db3b0ec50923e14121c7ea2dbbaac9465c0a0fe6aa228207c3a32fa2de`
- `sources/w9-other-a-header-alpha/header_alpha.cpp` `c65bd164247e908ef29569abcb6e5957dad4c877ad1a4cbca47473090ab5757f`
- `sources/w9-other-a-hpos/hpos.cpp` `7a7dcc2f375d3276698234ed55a306cc3a521493d22c83b1c8ff88fa3429758c`
- `sources/w9-other-a-init-light-info/init_light_info.cpp` `e93318cd62e36627388333f0537b37e110e3af9d9857be24ce1d3fa0d1588736`
- `sources/w9-other-a-last-y-anchor/last_y_anchor.cpp` `2d915598adb666c249add56e647f9f83a4f33fefcdef8056ebf9549fb68d5451`
- `sources/w9-other-a-line-to-y/line_to_y.cpp` `640197bc899e8c699e6350a4303e155451c70f364916a71cff4b73fa7cbb674b`
- `sources/w9-other-a-next-frame-drop/next_frame_drop.cpp` `ae90dc7574098a3376fe0b64fe2f1856f9147d904b9faec5a954852cf48455ed`
- `sources/w9-other-a-resource-map-get/resource_map_get.cpp` `dc4da2f4188ef62be99a95c5d8f0c24d8ee5f7796b9f11edc59b001c19c6ad2a`

## Authored but never executed

Everything above has only run in host checks with mocked callees. None of it has run against the original image or in
a game run; no byte comparison with the original was made. `commProcessReal__Fv` was not run through the private
paired harness.

## Unknown

- Names of the voice-chat record fields and what `timeout` means beyond the 5-pass hold; whether the 0x80 bit in the
  first status message is a flag or an index.
- What the `lightInfo` values mean, and what the 16 floats after the first 80 hold or who writes them.
- The first line of `commProcessReal__Fv` reads `netNetworkMode` as a signed byte and `frontEndRunning` as a word;
  other readers of these may use other widths.
