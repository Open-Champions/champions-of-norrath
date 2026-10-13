# w12-startup-rt-b: startup and runtime support (libc, EE/SCE kernel, device and TLB glue, linker symbols) (B)

Branch `agents/w12-startup-rt-b`. Addresses and instruction behaviour come from the pinned `SLUS_205.65` on this
machine, used only as local reference; no game bytes, disassembly or decompiler output is committed. Nothing here
ran on an EE.

## What the package found first

A fresh partial build of the shared branch (5113 sources) reproduced 827 unresolved imports, all 28 names of the
scope among them. 19 of the 28 already had an authored source that the build plan does not select (written by
`simloop-services`, `sony-cd` and `w9-hud-inventory-items-b`), 3 more are aliases of objects defined elsewhere, and
`norrath_loader_failure_word_view` is authored but unselected. Only 9 needed new authoring, plus one helper (`min`)
that the new code opened. The measured effect is at the end.

## How each unit was checked

* One function (or one data object) per commit, compiled alone with the reviewed `authored_cpp` flags.
* Every unit has a committed `host_check.cpp` (not a plan input) with stand-in callees. It fails when a stored
  word, branch, call order, argument or result is wrong. I ran hand-made one-edit mutants of `lgFrameEncode`
  (frame-count rounding), `lgFrameDecode` (capacity accounting, boundary compare), `mark_device_unplugged` (three:
  already-6 test, table-two stride, table-two flag word) and `kdm_rpccall_end` (semaphore offset); all were caught.
  The mutants are not kept.
* The three non-trivial functions (`lgFrameEncode`, `lgFrameDecode`, `mark_device_unplugged`) were also run against the
  original in a private R5900 interpreter (outside git): original and authored function on identical memory
  snapshots with identical stub callees; call logs, return value (where defined) and every changed byte outside
  the stack are compared. 600, 600 and 500 scenarios, zero differences, every original instruction word reached except
  padding nops and annulled delay slots. Eleven-argument calls needed the harness to place stack arguments; I added that
  to my private copy. The trivial units (the two `gsm_frame_*` adapters, `initialise_event_list`, `kdm_rpccall_end`, `min`,
  the two data objects) rely on the host check alone.

## Functions

| Symbol | Address | Orig. bytes | Compiled bytes | Unit |
|---|---|---|---|---|
| `gsm_frame_encode` | 0x002eb740 | 36 | 20 | `w12-startup-rt-b-gsm-frame-encode` |
| `gsm_frame_decode` | 0x002eb768 | 36 | 20 | `w12-startup-rt-b-gsm-frame-decode` |
| `lgFrameEncode` | 0x002eb2b8 | 484 | 528 | `w12-startup-rt-b-lg-frame-encode` |
| `lgFrameDecode` | 0x002eb4a0 | 464 | 484 | `w12-startup-rt-b-lg-frame-decode` |
| `initialise_event_list` | 0x002e8dd0 | 12 | 12 | `w12-startup-rt-b-initialise-event-list` |
| `kdm_rpccall_end` | 0x002e9068 | 48 | 40 | `w12-startup-rt-b-kdm-rpccall-end` |
| `mark_device_unplugged` | 0x002e7f58 | 380 | 300 | `w12-startup-rt-b-mark-device-unplugged` |
| `min` | 0x002eb670 | 16 | 16 | `w12-startup-rt-b-min` |

Data: `mcErrOn` (0x01190c30, 4 bytes) and `norrath_overlay_sFunctionIndex_11edf00` (0x011edf00, first word of an
8-byte slot). Compiled sizes are from the reviewed profile and are not original-byte matches.

## Observed

* `gsm_frame_encode` / `gsm_frame_decode` end with a jump to `gsm_encode` (0x002ec2c0) / `gsm_decode` (0x002eb7e0). The
  word at object+20 (encode) or object+24 (decode) becomes the callee's first argument; their own first two
  arguments become its second and third. They are the per-frame callbacks of `lgFrameEncode` / `lgFrameDecode`.
* `lgFrameEncode` slices 16-bit samples into frames of `samplesPerFrame`, fills a caller-owned frame buffer, calls the
  callback `fn(frame, outputPosition, object)` per full frame, and reports the sum of the callback results in
  the capacity word. `lgFrameDecode` does the reverse over a byte stream and reports twice the samples produced. The
  errors are 0x80000004 (null buffers, odd count or capacity) and, for encode only, a printed
  "bytes provided / Need" diagnostic. Both routines contain behaviour that looks unintended and is kept as observed:
  encode adds a frame when the fill is non-zero and the capacity is not a multiple of the output frame size (testing
  the capacity, not the sample count); decode mixes output and input units in its input limit and subtracts the
  running total, not the frame size, from the remaining capacity. Details are in the unit comments.
* `initialise_event_list` clears the halfwords at record+36 and record+38.
* `kdm_rpccall_end` signals the semaphore in the word at `GLibData+40` with `iSignalSema` unless the word at +56 is
  non-zero.
* `mark_device_unplugged` searches two `GLibData` device tables (272-byte records from +128, count at +64; 104-byte
  records from +1216, count at +68) for the first record whose signed halfword at +12 equals the argument, and under
  the semaphore at +44 stores 0x80000006 at record+24, -1 at +12, 6 at +36 and 1 in the word at +100 (first table) or
  +104 (second), unless the record's word at +36 is already 6.
* `mcErrOn` and `sFunctionIndex` both lie inside the loaded `.srs` section of the executable and their file bytes are
  zero. This corrects the earlier note in `w11-crt-b.md` that `mcErrOn` lies outside every loaded section.
* `norrath_net_flag_bytes_48681c` is not a separate object: 0x0048681c is `kickState` (0x00486818, 8 bytes,
  authored in `w10-net-a-frame-state-data`) plus 4. `net_game_step` already reads "the kickState byte (4 + index)".

## Inferred

* The two codec routines are a GSM-style frame codec (160 samples, 33 bytes) from their only caller's constants;
  no field names exist in the image. The two device tables are the keyboard and mouse lists (the reader thread
  initialises both `+12` halfwords to -1); the value 6 is an "unplugged" state. The word at `GLibData+56` is a
  "do not signal" switch.

## Unknown

* Whether the encode/decode quirks above are bugs or deliberate, the layout of the codec object beyond +20/+24/+28/+348,
  what the `+24` and `+36` fields of a device record mean beyond these stores, who writes `mcErrOn`, what the four
  bytes after `sFunctionIndex` hold, and where in the `.srs` layout these two objects belong.
* Whether `gsm_frame_decode`'s result is used (its only caller ignores it; it is declared `int`).

## Imports closed (by this package, or closed once the existing units below are selected)

Authored here: `gsm_frame_decode`, `gsm_frame_encode`, `initialise_event_list`, `kdm_rpccall_end`, `lgFrameDecode`,
`lgFrameEncode`, `mark_device_unplugged`, `mcErrOn`, `norrath_overlay_sFunctionIndex_11edf00`.

Already authored by other packages, not in the plan; they close once the integrator selects these (I included them
in a private plan to confirm, nothing else changed):
* `sony-cd-vblank-views/views.cpp`: `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`,
  `norrath_archive_vblank_semaphore_view`.
* `w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp`:
  `norrath_inventory_exit_sound_resource_view`.
* `simloop-services-pad-mtap-state/pad_mtap_state.cpp`: `norrath_libmtap_change_priority_client_view`,
  `norrath_libmtap_get_connection_client_view`, `norrath_libmtap_module_version_client_view`,
  `norrath_libmtap_port_close_client_view`, `norrath_libmtap_port_open_client_view`, `norrath_libmtap_rpc_buffer_view`,
  `norrath_libpad_dma_info_view`, `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`,
  `norrath_libpad_port_info_view`, `norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view`.

## Names in scope that stay open, and why

* `norrath_net_flag_bytes_48681c`: an alias of `kickState + 4`, which `w10-net-a-frame-state-data` defines. The fix
  belongs in `w11-game-a-receive-control-packet` (not mine): index `kickState[4 + packet[9]]` instead of the invented
  name. The build forbids symbol aliases, so I did not add one.
* `norrath_inventory_message_handler_word_view`: the word `invMsgHandle` (0x00486a88), already defined by
  `items-player-inventory-init-net`; the users (`game-player-*/inventory_actions.hpp`) should refer to `invMsgHandle`.
  Same reasoning, no alias.
* `norrath_loader_failure_word_view`: `level-anim-load-loader-failure-word` defines it (an absolute symbol of value 0,
  by `asm`); it is not in the plan, and I did not test its selection in my private build. The integrator needs to
  select it.

## Imports opened

`gsm_encode` (0x002ec2c0, 2316 bytes) and `gsm_decode` (0x002eb7e0, 2324 bytes): the two callees of the new adapters,
large codec routines nobody has authored. `min` was opened by the lg routines and is closed here. Other imports used by
the new code were already unresolved or defined: `GLibData`, `WaitSema`, `SignalSema`, `iSignalSema`, `memcpy`, `memset`,
`printf`. `max` (0x002eb680) has no importer and was not authored.

## Build-plan entries needed

Append these `files` entries (one JSON object per line, `input_order` assigned by the integrator; none needs a header
closure). The three existing units listed above need entries too (see `simloop-services.md`, `sony-cd.md` and
`w9-hud-inventory-items-b.md`).

```
{"emitted_functions":[{"compiled_body_bytes":20,"name":"gsm_frame_decode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"gsm_frame_decode.o","profile":"authored_cpp","sha256":"9cd4f005032f1a5da1da1684478b7f7e55efaee68ac7ad508ef0db72f7fddd7b","source":"sources/w12-startup-rt-b-gsm-frame-decode/gsm_frame_decode.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"gsm_frame_encode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"gsm_frame_encode.o","profile":"authored_cpp","sha256":"ffa0c69069dd4e5867ccad38a1a24c8e5bbabc83b68e5d46d2d79d0bc6a5d9ad","source":"sources/w12-startup-rt-b-gsm-frame-encode/gsm_frame_encode.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"initialise_event_list"}],"header_closure":[],"kind":"independently_authored_source","object_name":"initialise_event_list.o","profile":"authored_cpp","sha256":"c24a18d1de9ecc95c3ee485c348618f1f67d799f3f96297152b922bf3bb68311","source":"sources/w12-startup-rt-b-initialise-event-list/initialise_event_list.cpp"}
{"emitted_functions":[{"compiled_body_bytes":40,"name":"kdm_rpccall_end"}],"header_closure":[],"kind":"independently_authored_source","object_name":"kdm_rpccall_end.o","profile":"authored_cpp","sha256":"c88086bdb4357ea4fae7b9b3f730279d5950a0d993ab65814ee439ac939a94ef","source":"sources/w12-startup-rt-b-kdm-rpccall-end/kdm_rpccall_end.cpp"}
{"emitted_functions":[{"compiled_body_bytes":484,"name":"lgFrameDecode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"lg_frame_decode.o","profile":"authored_cpp","sha256":"8240adba55bf3d7297a6de4bc3bf37cfbea068acff298bed307b4263a15a6daf","source":"sources/w12-startup-rt-b-lg-frame-decode/lg_frame_decode.cpp"}
{"emitted_functions":[{"compiled_body_bytes":528,"name":"lgFrameEncode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"lg_frame_encode.o","profile":"authored_cpp","sha256":"a0e2b69e0f884c9d00916839ad2efa042bdf3f47515e3b26508dae113c23cad4","source":"sources/w12-startup-rt-b-lg-frame-encode/lg_frame_encode.cpp"}
{"emitted_functions":[{"compiled_body_bytes":300,"name":"mark_device_unplugged"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mark_device_unplugged.o","profile":"authored_cpp","sha256":"10fdc312a3433677cdac0a54e21a61d56e181197c01cbe3dc945d6e6a5c1fd6c","source":"sources/w12-startup-rt-b-mark-device-unplugged/mark_device_unplugged.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"mc_err_on.o","profile":"authored_cpp","sha256":"7ee26a38019ffada791ccb795cc579389f8cface82e09a525a6e4cea5c66a796","source":"sources/w12-startup-rt-b-mc-err-on/mc_err_on.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"min"}],"header_closure":[],"kind":"independently_authored_source","object_name":"min.o","profile":"authored_cpp","sha256":"55a74383d84033cbee1275efaf13d983525dd4a2913b36e9ad15bdc17e3820aa","source":"sources/w12-startup-rt-b-min/min.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"overlay_function_index.o","profile":"authored_cpp","sha256":"8b1e54c8cc97c24645dbd3459e213684012753208606cec158a227cba53fad62","source":"sources/w12-startup-rt-b-overlay-function-index/overlay_function_index.cpp"}
```

## Authored but never executed

All of it: nothing ran on an EE. The three large routines were compared with the original only in a private
interpreter with stand-in callees. `gsm_frame_*`, `initialise_event_list`, `kdm_rpccall_end`, `min` and the two data
objects were checked by host contract only. No physical device tables, semaphores, IOP replies or real codec state
were involved.

## Measured effect

Partial build of the shared branch at `c7243a6d` with the repo EE toolchain: 5113 compilations, 827 unresolved imports.
With a private plan adding my nine units, `min`, and the three existing units above: 5126 compilations, 804 unresolved
imports. 25 of the 28 scope names closed; `gsm_decode` and `gsm_encode` opened. `python3 -m unittest discover -s tools
-p 'test_*.py'` passes (214 tests). `check_source_snapshot.py` reports unrecorded files, as it does for any unmerged package.
