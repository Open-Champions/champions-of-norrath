# w15-startup-a: CD stream, EE/IRX, memory-card read, network thread and link-symbol imports

Branch `agents/w15-startup-a`. One commit per function; data units are separate commits. Source is under
`experimental/sources/w15-startup-a-*`; every directory also holds a `host_contract.cpp` (host check, not a plan input).
Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here ran on the EE hardware, with the SDK, a disc,
a memory card or a network.

## How each function was checked

* Each unit compiles with the reviewed EE g++ and the `authored_cpp` flags (single-unit compile).
* Each unit has a native host contract (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0) with scripted callees.
* Each function was compared with the image in a private paired run (an R5900 interpreter outside git): the original and the
  freshly compiled authored function run from identical memory snapshots with identical stubbed callees; call logs
  (including argument contents and the memory the callee would read), return values and every changed byte outside the stack
  must be equal. Every function also had at least two deliberately wrong variants run; each was caught except where stated.
* A full partial build was **not** completed: it was started and stopped because the machine was heavily loaded. The closure
  below was measured instead by compiling all 22 planned units with the plan's flags, listing their defined and undefined
  symbols, and comparing with `work/plan15-build/unresolved-imports.json`. A private plan copy that holds the entries below
  passes `python3 tools/build_partial.py --check-plan` (generator: `docs/agents/w15-startup-a-plan-entries.py`).

## Functions

| Function | Image address | Image bytes | Authored bytes | Paired scenarios |
|---|---|---|---|---|
| `VICdStreamStatus__Fv` | 0x01000b58 | 12 | 12 | 6 |
| `VICdStreamSync__Fi` | 0x01000b08 | 76 | 88 | 24 |
| `VICdStreamDiskReady__Fv` | 0x01000a90 | 120 | 120 | 60 |
| `VICdStreamRead__FUiUiPvi` | 0x01000958 | 168 | 168 | 27 |
| `FlushIop__Fi` (opened by the above; local in the image) | 0x01000b68 | 120 | 136 | 60 |
| `VIEEDMASend__FiPv` | 0x010f9a40 | 116 | 72 | 12 |
| `VIEEFileDelay__Fi` | 0x010cb4f8 | 104 | 104 | 7 |
| `VIEEFileDelayTh__FiUsPv` (opened by the above) | 0x010cb560 | 36 | 36 | 16 |
| `VIEELoadIrx__FPCciT0` | 0x010f9be8 | 116 | 116 | 30 |
| `VIEEUnloadIrx__Fi` | 0x010f9cd8 | 56 | 48 | 4 |
| `VIMemCardReadFileAsync__FPCcPvi` | 0x010d3920 | 284 | 288 | 54 |
| `unet_startup_thread__FPv` | 0x0101c448 | 516 | 420 | 162 |

Compiled sizes are not byte matches. `VIEEFileDelayTh__FiUsPv` disassembles to the same call, `sync`, `ei` sequence.

## Observed

* The CD-stream functions share five words/blocks in the CD-stream file: a request block (0x011e25c0, filled by Read and
  DiskReady, sent by `FlushIop__Fi`), a reply block (0x011e2600), a pending word (0x011e2640), a status word (0x011e2644) and a
  RPC client record (0x011e2580). `VICdStreamRead` fills words 4, a, b, d, buffer, sends, then sets pending; `DiskReady` sends
  command 5 and copies reply word 0 into status; `Sync(0)` yields the thread while pending is set; `Status` reads status.
  `FlushIop__Fi(flag)` returns -1 if the client is busy, else issues RPC 0x80f0 with 64-byte blocks and returns 0.
* `VIEEDMASend` writes 2 to 0x1000e010 only for channel 1, then sends. `VIEEFileDelay` is a semaphore plus alarm sleep;
  its callback signals the semaphore and ends with `sync; ei`. `VIEELoadIrx` prints, builds `cdrom0:\MODULES\NAME;1` and
  retries only the load call until it is non-negative. `VIEEUnloadIrx` stops then unloads.
* `VIMemCardReadFileAsync` zeroes `sAsyncMode`, opens `/<product id>/<name>` on port 0 slot 0 (mode 1), syncs once, maps -4 to -2
  and any other negative to -1, otherwise records the handle in `sAsyncFd`, starts the read and returns 0.
* `unet_startup_thread__FPv` brings libnet up (details in the unit header) and loops forever on four error conditions.
* Section table: `.eh_frame` starts at 0x00453970 where `__EH_FRAME_BEGIN__` (size 0) sits; `.sbss` starts at 0x00486800
  (`_fbss`); `_gp` is 0x004879f0, which is the start of `.lit4` (0x0047fa00) plus 0x7ff0; `.bss`/`.vubss` end at 0x008eab1c
  (`_bssend`, also `_srsctorsstart`); `.srsctors` is 0x3c bytes there and `.srsdtors` 0x1c bytes at 0x008eab58 (`_srsctorsend`);
  `COMMON` ends at 0x008ec91c (`_end`).
* `mt_ebx` and `mt_eby` are 1024 bytes each in `.data` and all zero in the file.

## Inferred

* Command 4 is a sector read and 5 a disc-ready query for the IOP CD server; status 1 means "not sent" and 2 "request outstanding".
* The product id string is the memory-card directory name; `sAsyncMode` records an in-flight read.
* `_gp`, `_fbss`, `_bssend`, `_end`, `_srsctorsstart`, `_srsctorsend` are section-boundary symbols of the SDK link script, not objects.

## Unknown

* IOP-side meaning of the stream command words and reply values; whether `sSifClient` is exactly 64 bytes.
* Return types of `VIEEUnloadIrx__Fi` and the alarm callback in the original source (the register values were reproduced).
* The libnet/inet call signatures beyond observed register values; the original nop count in the hang loops.
* Writers of `mt_ebx`/`mt_eby`, `net_ready` (other than the status function) and `sAsyncMode`.
* Final link order for `__EH_FRAME_BEGIN__`.

## Imports closed (names)

By authored source: `VICdStreamDiskReady__Fv`, `VICdStreamRead__FUiUiPvi`, `VICdStreamStatus__Fv`, `VICdStreamSync__Fi`,
`VIEEFileDelay__Fi`, `VIEEDMASend__FiPv`, `VIEELoadIrx__FPCciT0`, `VIEEUnloadIrx__Fi`, `VIMemCardReadFileAsync__FPCcPvi`,
`unet_startup_thread__FPv`, `unet_startup_thread_id`, `unet_startup_thread_stack`, `net_ready`, `mcerror`, `mt_ebx`, `mt_eby`,
`sAsyncFd`, `norrath_overlay_sRecv_11edd00`, `__EH_FRAME_BEGIN__`.

By adding plan entries for sources written in earlier waves (no new code from me): `norrath_archive_vblank_count_view`,
`norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view` (`sony-cd-vblank-views`),
`norrath_loader_failure_word_view` (`w14-startup-io-b-loader-failure-word`; needs the owner's choice noted in `w14-startup-io-b.md`),
and `norrath_net_flag_bytes_48681c` (replace the plan entry `w11-game-a-receive-control-packet` by
`w14-startup-io-b-receive-control-packet`, which indexes `kickState + 4`; the new unit defines `receiveControlPacket__FPiib` again, so
it is an in-place swap, not an addition).

## Imports not closed

* `osdsrc` (1960 bytes) and `tlbsrc` (816 bytes) are original EE machine code copied to 0x80074000/0x80075000. Publishing their
  bytes would publish original code; this needs the owner's decision (link a private extraction at integration time). Unchanged
  from `kernel-patches.md`.
* `_gp`, `_fbss`, `_bssend`, `_end`, `_srsctorsstart`, `_srsctorsend`: link-script symbols; no C++ unit can define them in a
  relocatable partial link, and the recipe imports no absolute addresses. Evidence for a future script is in "Observed". The
  `.srsctors` table (15 words, 0x3c bytes) holds addresses of SRS constructors in the overlay, so its contents cannot be authored yet.

## Imports opened

Names my units reference that were not in the 915-name list and are not defined in the current plan: `SetAlarm`,
`VIInitUSocketExApi__Fv`, `pf`, `sceInetAddress2Name`, `sceInetAddress2String`, `sceLibnetRegisterHandler`,
`sceLibnetWaitGetAddress`. Everything else my units reference (SDK wrappers such as `CreateSema`, `sceMcOpen`, `sceSifCallRpc`,
and the data `cd`, `hostname`, `net_buf`, `sLibNetInitialized`, `kickState`) is already defined by the plan or in the 915-name list.
Names defined by my units that no earlier unit referenced: `FlushIop__Fi`, `VIEEFileDelayTh__FiUsPv`, `last_buffer`,
`norrath_cdstream_*`, `sAsyncMode`, `sProductId`. Still imported from the memory-card window package (listed in the wave file):
`SetIcon__16VIIconDefinitionPvi`, `SetTitle__16VIIconDefinitionPCc`, `sIconDefinition`, `sListIcon`, `sMemCardInitialized`,
`sProductTitle`; and `snprintf__8VIStringPciPCce` (VIString package).

## Build-plan entries needed

Plan only one copy of each. The first 19 lines are this package's units; the last three are earlier-wave units (the last replaces
the plan entry for `sources/w11-game-a-receive-control-packet/receive_control_packet.cpp`). Regenerate with
`python3 docs/agents/w15-startup-a-plan-entries.py --toolchain <ee> [--with-others --swap-receive --private-root DIR]`.

```jsonl
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "VICdStreamDiskReady__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_cdstream_disk_ready.o", "profile": "authored_cpp", "sha256": "ddbff11e6560be461ecb106ceb080602c24442368b999fa3883c5832ff30f746", "source": "sources/w15-startup-a-cdstream-disk-ready/disk_ready.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "FlushIop__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_cdstream_flush_iop.o", "profile": "authored_cpp", "sha256": "35f359a98454cb3ff2491ae6aab0c872587df13db6c6f51500a6cc4857d0c7c5", "source": "sources/w15-startup-a-cdstream-flush-iop/flush_iop.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "VICdStreamRead__FUiUiPvi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_cdstream_read.o", "profile": "authored_cpp", "sha256": "11aeabaccaabd73000efe1153bd23b623f4d339016758b38e9f053528c86e0e4", "source": "sources/w15-startup-a-cdstream-read/read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "VICdStreamStatus__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_cdstream_status.o", "profile": "authored_cpp", "sha256": "fa134de38ebf512c52edf8e52f1f31ece7845e07564a279a3dee3e8fb26c69f5", "source": "sources/w15-startup-a-cdstream-status/status.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "VICdStreamSync__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_cdstream_sync.o", "profile": "authored_cpp", "sha256": "c894cc395057825a5e30af0f54b0626754eabf398b68be83fc16c78e274f28c4", "source": "sources/w15-startup-a-cdstream-sync/sync.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_cdstream_state.o", "profile": "authored_cpp", "sha256": "300e50734143b4dfdae0db6f13a48d3cad572e683adcf128a419617544577dcc", "source": "sources/w15-startup-a-data-cdstream-state/cdstream_state.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_mcerror.o", "profile": "authored_cpp", "sha256": "950757fda5ba79783bc7fc0020d314ac2591c137540ac735c0efc7f7729a182e", "source": "sources/w15-startup-a-data-mcerror/mcerror.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_megatile_edge_lists.o", "profile": "authored_cpp", "sha256": "2a68b97ea306ae594a3c49ea07226b63c1c6ebce5e595181bbee4396c5832ae8", "source": "sources/w15-startup-a-data-megatile-edge-lists/edge_lists.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_memcard_async_state.o", "profile": "authored_cpp", "sha256": "0099ac709a762525f1410c7c0f4566735be084855d27537cab351f5d3fc3e5dd", "source": "sources/w15-startup-a-data-memcard-async-state/async_state.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_net_thread_state.o", "profile": "authored_cpp", "sha256": "1c89fb55761a49667f77d36a0f2998effd86ca190521126a30ff00d6c5e5686e", "source": "sources/w15-startup-a-data-net-thread-state/net_thread_state.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_data_overlay_srecv_view.o", "profile": "authored_cpp", "sha256": "41b93374de29c7a2c62229f9b92d94960dd9adb409639caeb331a2fde1306c46", "source": "sources/w15-startup-a-data-overlay-srecv-view/srecv_view.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_link_eh_frame_begin.o", "profile": "authored_cpp", "sha256": "9b7a8de5e09db9fcc05fc3e0006a2fbad0785f03d1a4305d061aa23d28a4e026", "source": "sources/w15-startup-a-link-eh-frame-begin/eh_frame_begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 288, "name": "VIMemCardReadFileAsync__FPCcPvi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_memcard_read_file_async.o", "profile": "authored_cpp", "sha256": "21b5dc7b6ce4f72aa67ec9afb396509636ef3ae1732e3158a5b0245c5a3a61e9", "source": "sources/w15-startup-a-memcard-read-file-async/read_file_async.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 420, "name": "unet_startup_thread__FPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_unet_startup_thread.o", "profile": "authored_cpp", "sha256": "21c449889bf9bf9b1c1c9fc7087c8d356577372bb94891031da0319dae2795f6", "source": "sources/w15-startup-a-unet-startup-thread/unet_startup_thread.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "VIEEDMASend__FiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_vieedmasend.o", "profile": "authored_cpp", "sha256": "47da3942ea37aff6aea58cb1d78e304822f51b3780586009502935bc067f4759", "source": "sources/w15-startup-a-vieedmasend/dma_send.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "VIEEFileDelay__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_vieefiledelay.o", "profile": "authored_cpp", "sha256": "a91be190a32db9321b0565456af2b79c535a52d8132a97ad01ded3aeea7fba02", "source": "sources/w15-startup-a-vieefiledelay/file_delay.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "VIEEFileDelayTh__FiUsPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_vieefiledelay_alarm.o", "profile": "authored_cpp", "sha256": "5646fa6a6391d64af3d0733b770f53111b30ae4b174117c30d693b6e9fed25f8", "source": "sources/w15-startup-a-vieefiledelay-alarm/alarm_handler.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "VIEELoadIrx__FPCciT0"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_vieeloadirx.o", "profile": "authored_cpp", "sha256": "494fbc63fbd4b1786aceb126e5a2a5dc907a130db3f0cff1db5fd79235f0130e", "source": "sources/w15-startup-a-vieeloadirx/load_irx.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "VIEEUnloadIrx__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15_startup_a_vieeunloadirx.o", "profile": "authored_cpp", "sha256": "cfd1b6dbb686b1aec716239f14c5b68a7ff2587627052b03eaf1dc40bb71fffb", "source": "sources/w15-startup-a-vieeunloadirx/unload_irx.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "sony_cd_vblank_views.o", "profile": "authored_cpp", "sha256": "2735a9c09cb5afcffd3881673c54a30b2da2862a2278c3da24b7ef2325bc30d7", "source": "sources/sony-cd-vblank-views/views.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14_startup_io_b_loader_failure_word.o", "profile": "authored_cpp", "sha256": "ba7ecffd1799968d4f3b21c1a70154192ba536e4aecb512ef332ece22f7ab8d7", "source": "sources/w14-startup-io-b-loader-failure-word/failure_word.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2796, "name": "receiveControlPacket__FPiib"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14_startup_io_b_receive_control_packet.o", "profile": "authored_cpp", "sha256": "83bb2905c392be1568dd1a0b48224fe3898677fe3bdb23bfba63d679fd9f75ca", "source": "sources/w14-startup-io-b-receive-control-packet/receive_control_packet.cpp"}
```

## Authored but never executed

All twelve functions and the eight data units: run only on the host against scripted callees and in the private interpreter
against the image with stubbed callees. The interpreter treats `sync` and `ei` as no-ops, so dropping them is not caught by the paired
run. `sProductId` carries the 20-character product id text from the image; the other data is zero storage. The `.eh_frame`
marker has not been through a final link.

## Not done

The two machine-code blobs and the six link-script symbols above. A completed partial build with the new plan (it was stopped under
load); the closure was measured from symbol lists instead.
