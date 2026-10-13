# SCE library and audio-codec calls (w10-sce-a)

Branch `agents/w10-sce-a`. Sources under `experimental/sources/w10-sce-a-*`, plus host contracts added next to
earlier packages' sources that had none. Addresses, sizes and instruction behaviour come from the pinned
SLUS-20565 image on this machine and its symbol table, used only as local reference. Nothing here ran on an EE.
No game binary, disassembly or decompiler output is committed. Compiled sizes below are from the reviewed
`authored_cpp` profile and are not original-byte matches (except the two assembler routines, which are
byte-compared).

## What the package found first

Most of the symbols in the scope list were *not* missing sources. Ten already had an authored source written
by an earlier package (`w8-startup-sce-d-sif-get-sreg`, `w9-startup-crt-f-{mc-read, mc-write, pad-read,
pad-set-act-direct, tty-init, tty-read, tty-write, tty-state}`, `simloop-services-pad-dma-buffer`) that the
integrator had left out of the plan because no host check was committed ("checked only in a private paired run").
For those the work here is a committed host contract that fails when the behaviour is wrong, plus a fresh private
paired run against the image. The rest (SIF multi-client RPC, pad library, GIF packet, libnet inet calls, the
audio-device and codec calls, three data objects) are new sources.

## How each function was checked

* Every function is its own commit and compiles with the reviewed `authored_cpp` (or, for `.S`,
  `authored_entry_asm`) flags.
* Every unit has `host_contract.cpp` or `host_check.cpp` (not a plan input). It supplies stand-in callees and
  fails when the order of calls, arguments, stored words or result is wrong. Each one passes on the committed
  source and was also run against hand-made single-edit mutants of that source (not kept in git); at least one
  mutant per function failed with a nonzero exit, and two weak spots found that way were closed with a stronger
  check (`sceSifMCallRpc` alignment boundary, `mceStorePwd` empty text). The one surviving mutant,
  `mceStorePwd` with `< 1024` changed to `< 1023`, is equivalent (both give 1023 for a length of 1023). Build lines
  are the first comment of each file; most need `-fno-builtin` when they define `printf`/`memset`, and
  `-DNORRATH_HOST_CONTRACT` where noted.
* Paired runs (private, outside git, `priv-w10-sce-a`): the original function and the authored one run on a
  small R5900 interpreter from identical memory snapshots with identical stub callees. Call logs (callee,
  arguments, the memory the callee sees), return value and every changed byte outside the stack must be equal.
  Every emitted function passed with 0 mismatches (a few dozen to ~1800 scenarios each; `sceCdDiskReady_old` has
  100 because its retry delay is 1M iterations per spin), except the two assembler routines (byte-compared
  instead). `sceSifGetSreg` was paired as well. Two interpreter gaps were fixed privately (`mult1`/`multu1`/
  `mflo1`/`mfhi1` were unsupported, which made the original halt identically with the authored side
  uncontested; the pad runs would have been vacuous). Paired runs are not repository evidence.
* The two assembler routines (`sceSifDmaStat`, `sceGifPkOpenGifTag`) are compared byte for byte with the image
  by `host_check.cpp` (takes the private executable path, address, object and symbol as arguments).

## Functions


| Symbol | Address | Orig. bytes | Compiled bytes | Unit |
|---|---|---|---|---|
| `sceCdDiskReady_old` | 0x002e34e8 | 504 | 672 | `w10-sce-a-cd-disk-ready-old` |
| `connect_to_iop_server` | 0x002e8fa0 | 196 | 192 | `w10-sce-a-connect-to-iop-server` |
| `sceGifPkAddGsAD` | 0x0133f2f8 | 36 | 32 | `w10-sce-a-gif-pk-add-gs-ad` |
| `sceGifPkCloseGifTag` | 0x0133f228 | 164 | 148 | `w10-sce-a-gif-pk-close-gif-tag` |
| `sceGifPkCnt` | 0x0133f070 | 116 | 100 | `w10-sce-a-gif-pk-cnt` |
| `sceGifPkOpenGifTag` | 0x0133f210 | 24 | 24 | `w10-sce-a-gif-pk-open-gif-tag` |
| `sceGifPkRef` | 0x0133f0e8 | 152 | 124 | `w10-sce-a-gif-pk-ref` |
| `sceGifPkReserve` | 0x0133f1f8 | 20 | 20 | `w10-sce-a-gif-pk-reserve` |
| `sceInetCreate` | 0x002e5348 | 112 | 120 | `w10-sce-a-inet-create` |
| `sceInetInterfaceControl` | 0x002e5930 | 168 | 180 | `w10-sce-a-inet-interface-control` |
| `sceInetOpen` | 0x002e53b8 | 72 | 80 | `w10-sce-a-inet-open` |
| `sceInetRecvFrom` | 0x002e5510 | 272 | 264 | `w10-sce-a-inet-recv-from` |
| `sceInetSendTo` | 0x002e56d0 | 228 | 240 | `w10-sce-a-inet-send-to` |
| `read_done` | file-local | - | 100 | `w10-sce-a-lgaud-a-read` |
| `lgAudARead` | 0x002e9bf8 | 372 | 408 | `w10-sce-a-lgaud-a-read` |
| `lgAudASync` | 0x002eae08 | 156 | 172 | `w10-sce-a-lgaud-a-sync` |
| `write_done` | file-local | - | 40 | `w10-sce-a-lgaud-a-write` |
| `lgAudAWrite` | 0x002e9d70 | 388 | 440 | `w10-sce-a-lgaud-a-write` |
| `lgAudEnumHint` | 0x002ea150 | 112 | 112 | `w10-sce-a-lgaud-enum-hint` |
| `lgAudEnumerate` | 0x002ea1c0 | 444 | 376 | `w10-sce-a-lgaud-enumerate` |
| `lgAudOpen` | 0x002e9550 | 356 | 424 | `w10-sce-a-lgaud-open` |
| `lgAudSetRecordingVolume` | 0x002ead10 | 244 | 244 | `w10-sce-a-lgaud-set-recording-volume` |
| `lgAudStartPlayback` | 0x002ea9c0 | 196 | 220 | `w10-sce-a-lgaud-start-playback` |
| `lgAudStartRecording` | 0x002ea830 | 196 | 220 | `w10-sce-a-lgaud-start-recording` |
| `lgCodecDecode` | 0x002eb028 | 208 | 156 | `w10-sce-a-lgcodec-decode` |
| `lgCodecEncode` | 0x002eaf58 | 208 | 156 | `w10-sce-a-lgcodec-encode` |
| `mceSifRpcEndFunc` | 0x002e0de0 | 12 | 12 | `w10-sce-a-mce-sif-rpc-end-func` |
| `mceStorePwd` | 0x002e1a00 | 140 | 204 | `w10-sce-a-mce-store-pwd` |
| `scePadGetButtonMask` | 0x002d4330 | 180 | 168 | `w10-sce-a-pad-get-button-mask` |
| `scePadSetButtonInfo` | 0x002d43e8 | 176 | 236 | `w10-sce-a-pad-set-button-info` |
| `scePadSetReqState` | 0x002d3c78 | 100 | 104 | `w10-sce-a-pad-set-req-state` |
| `sceSifDmaStat` | 0x002d5480 | 16 | 16 | `w10-sce-a-sif-dma-stat` |
| `sceSifMCallRpc` | 0x002e4be0 | 616 | 568 | `w10-sce-a-sif-m-call-rpc` |
| `sceSifMInitRpc` | 0x002e45c0 | 232 | 324 | `w10-sce-a-sif-m-init-rpc` |
| `sceSifRemoveRebootNotifyHandler` | 0x002df758 | 84 | 96 | `w10-sce-a-sif-remove-reboot-notify-handler` |
| `sceSifGetSreg` | 0x002d7160 | 24 | 24 | `w8-startup-sce-d-sif-get-sreg` |
| `sceMcRead` | 0x002e1310 | 276 | 312 | `w9-startup-crt-f-mc-read` |
| `sceMcWrite` | 0x002e1428 | 376 | 396 | `w9-startup-crt-f-mc-write` |
| `scePadRead` | 0x002d3a60 | 356 | 216 | `w9-startup-crt-f-pad-read` |
| `scePadSetActDirect` | 0x002d4198 | 192 | 200 | `w9-startup-crt-f-pad-set-act-direct` |
| `sceTtyInit` | 0x002d66b8 | 188 | 180 | `w9-startup-crt-f-tty-init` |
| `sceTtyRead` | 0x002d65e8 | 208 | 176 | `w9-startup-crt-f-tty-read` |
| `sceTtyWrite` | 0x002d6470 | 372 | 412 | `w9-startup-crt-f-tty-write` |


Notes: `read_done` and `write_done` are the nameless completion routines of the asynchronous read and write
(0x002eaea8 and 0x002eaf10, inside and just after `lgAudASync`'s extent in the symbol table); they are file-local
statics of `lgAudARead` and `lgAudAWrite` and are exercised by their contracts through the end-function pointer.
The symbol table gives local routines such as `scePadGetButtonMask` a size; sizes for `sceGifPk*` are from the
module image at 0x01000000.

## Observed

* `sceMcRead`/`sceMcWrite`/`scePadRead`/`scePadSetActDirect`/`sceTty*` agree with the earlier packages' sources
  (paired run, 0 mismatches). `sceMcWrite` splits a write at the next 16-byte boundary of the source (head copied
  into the parameter block, body sent from the source). `sceTtyWrite` with a negative count never reaches its
  loop's exit test and copies until 256 bytes are stored; this is the original's behaviour and is not tested
  on the host (it would read past the caller's buffer).
* Pad library: `scePadSetReqState` stores a byte at +113 of the 128-byte block of the port/slot;
  `scePadGetButtonMask` combines bytes 121..124 when byte 114 is 1 and bytes 100 and 102 are at least 2;
  `scePadSetButtonInfo` fills the shared RPC buffer (word 0 = 10) and, when the reply word is 1, calls
  `scePadSetReqState(port, slot, 2)` and returns the reply word read again afterwards.
* Multi-client SIF module (`sceSifMInitRpc`, `sceSifMCallRpc`): pool header at 0x008e8c00, packet buffer 0x008e8400,
  command handler 0x80000018, reboot notification 0x80000014, call command 0x8000001a. `sceSifMInitRpc` returns the
  flag value when already initialised and otherwise the result of `sceSifAddRebootNotifyHandler` (tail call).
  `sceSifMCallRpc` stores the caller's `$gp` in client word 6. Its mode argument must be 0 (a non-zero mode
  prints and returns -1 before anything else); the `mode & 1` path that follows exists in the original and is
  authored here in the original's order, but it cannot be reached and is not tested. The lock semaphore is left
  taken on every early exit and on the two failure exits of `lgAudARead`/`lgAudAWrite` (as observed).
* `sceSifRemoveRebootNotifyHandler`: top bit selects the 512-byte system table (`g_SysRebootBuf`, 32 entries of
  16 bytes) or the user table (`g_pUserRebootBuf`, `g_iUserRebootSize` entries); only the first word is cleared.
* GIF packet routines (`sceGifPk*`, module image): state is `{cursor, start, dma_tag, gif_tag}`. `sceGifPkCloseGifTag`
  derives the count from bits 58..59 and 60..63 of the open tag exactly as the contract documents; field names are
  descriptive only.
* Libnet inet calls: all are thin wrappers over `CallRpc`, `MemcpyD` and `MemcpyS` (function numbers 1, 2, 9, 13, 14;
  error values -542 for a failed call, -540 for a zero create reply, -543 from the copy helpers). The original
  stores the arguments in the order the sources do; see each header.
* Audio device and codec (`lgAud*`, `lgCodec*`): every call returns 0x8000000a when the library lock word
  (0x00447f7c) is -1, 0x80000004 for a bad argument, 0x80000008 when an asynchronous call is still running and
  0x80000000 when the RPC fails. The failure text of `lgAudSetRecordingVolume` is "lgAudStopPlayback: rpc call
  failed" in the image, and `lgAudOpen` names "device" for a null handle pointer; both are kept.
  `lgCodecEncode`/`lgCodecDecode`: kind 10 is a plain copy of the smaller size, kind 300 is a tail call to
  `lgCodecGSMEncode`/`lgCodecGSMDecode`.
* Data: `pad_dma_buf` is 2560 bytes (distance to `stateData`), `sifParamFname` is the 1104-byte span to
  `sifParamNext` (1044 bytes used by the callers), `sPad` is one pointer word, and four initialised words of the
  audio-device library sit at 0x00447f6c, 0x00447f74, 0x00447f78 and 0x00447f7c (`-1` is the "not initialised"
  semaphore id). `sceCdDiskReady_old`'s data (`cddr`, `_dr_mode`, `_scmd_dr_bind`, ...) is imported.

## Inferred

* The audio-device library is the USB headset library behind the voice pump in `commProcessReal`; the request
  buffer at 0x00447f74 is shared by all calls under the lock semaphore.
* `connect_to_iop_server` binds `GLibData`'s first 40 bytes as an RPC client (service number 0x61766973).
* The spin loops (`connect_to_iop_server`, `sceCdDiskReady_old`) are timing delays; their instruction counts are
  not reproduced and no timing was measured.

## Source changes in other packages' files

To make host contracts possible on a 64-bit host the following earlier sources were edited without changing
their EE code (object code of `pad_set_act_direct`, `tty_init`, `tty_write`, `tty_read` compared before and after):

* `w9-startup-crt-f-pad-set-act-direct/pad_set_act_direct.cpp`: the request-object pointer comes from a hook
  under `NORRATH_HOST_CONTRACT`.
* `w9-startup-crt-f-tty-state/tty_state.hpp`, `tty-init/tty_init.cpp`, `tty-write/tty_write.cpp`: the OR with
  0x20000000 moved into `ttyUncached()` (host: plain pointer).

## Blocked / not done

* `amx_Exec` (0x00135de8, 9176 bytes, direct-threaded interpreter): not attempted. The repository keeps the
  interpreter drafts held (`game-amx-exec-native-transport-source-v4` still stops on its `#error`s) and the
  script-amx package's `norrath_amx_exec` is a different executor, not the original body. A forwarding entry under
  the original name would be a placeholder, not a reconstruction, so none was written. Owner decision needed.
* `MemcpyD` (0x002e4f00, the sibling of the authored `MemcpyS`) is an import of five new functions and has no
  source. It is a 12-instruction function; I left it because it is not in this package's scope list.

## Private partial build

One partial build was run on a private plan copy (the shared selection at tip `cc78a45e` plus the 46 inputs):
3520 inputs compiled, one relocatable link, no duplicate definition, 2527 function bodies, 0 COMMON, 1306
unresolved imports (the shared tip's list had 1332). No original executable or object was read by the driver.
`python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.

## Imports closed (names)


`connect_to_iop_server`, `lgAudARead`, `lgAudASync`, `lgAudAWrite`, `lgAudEnumHint`, `lgAudEnumerate`, `lgAudOpen`, `lgAudSetRecordingVolume`, `lgAudStartPlayback`, `lgAudStartRecording`, `lgCodecDecode`, `lgCodecEncode`, `mceSifRpcEndFunc`, `mceStorePwd`, `pad_dma_buf`, `sPad`, `sceCdDiskReady_old`, `sceGifPkAddGsAD`, `sceGifPkCloseGifTag`, `sceGifPkCnt`, `sceGifPkOpenGifTag`, `sceGifPkRef`, `sceGifPkReserve`, `sceInetCreate`, `sceInetInterfaceControl`, `sceInetOpen`, `sceInetRecvFrom`, `sceInetSendTo`, `sceMcRead`, `sceMcWrite`, `scePadGetButtonMask`, `scePadRead`, `scePadSetActDirect`, `scePadSetButtonInfo`, `scePadSetReqState`, `sceSifDmaStat`, `sceSifGetSreg`, `sceSifMCallRpc`, `sceSifMInitRpc`, `sceSifRemoveRebootNotifyHandler`, `sceTtyInit`, `sceTtyRead`, `sceTtyWrite`, `sifParamFname`.

That is 44 of the 45 names in the scope list; `amx_Exec` is the one left open (see above).

## Imports opened

New unresolved imports of the units added here (18):


* `MemcpyD`: 0x002e4f00, local helper of the libnet module (copies into the request buffer); used by `sceInetCreate`, `sceInetSendTo`, `sceInetInterfaceControl`
* `QueueInit`: 0x002d6230, local routine of the tty library (from `sceTtyInit`)
* `QueuePeekReadDone`: 0x002d6298, local routine of the tty library (from `sceTtyRead`)
* `_iopnotify`: 0x002e46a8, local reboot-notification handler (from `sceSifMInitRpc`)
* `_send_to_iop`: 0x002d3398, local pad-library routine (from `scePadSetActDirect`)
* `lgCodecGSMDecode`: 0x002eb6e8 (tail call target of `lgCodecDecode`)
* `lgCodecGSMEncode`: 0x002eb690 (tail call target of `lgCodecEncode`)
* `mceIntrReadFixAlign`: 0x002e1278, completion routine of `sceMcRead`
* `norrath_lgaud_async_record_view`: 0x008ea930, the asynchronous record (3 words used; extent not established); descriptive name
* `norrath_lgaud_rpc_client_view`: 0x008ea780, the audio library RPC client (the word at +36 is not read here; extent not established); descriptive name
* `norrath_sif_m_request_end`: 0x002e47c8, the multi-client module's own copy of `_request_end` (its symbol name is shared with the copy at 0x002d7c08); descriptive name
* `norrath_sif_m_rpc_packet_buffer_view`: 0x008e8400, the multi-client packet buffer (extent not established); descriptive name
* `sceDeci2Open`: tty library
* `sceDeci2Poll`: tty library
* `sceDeci2ReqSend`: tty library
* `scePadGetDmaStr`: 0x002d39b0, local pad-library routine
* `sceSifAddRebootNotifyHandler`: 0x002df6a0 (from `sceSifMInitRpc`)
* `sceTtyHandler`: 0x002d62d8, the tty deci2 handler (from `sceTtyInit`)

The `norrath_lgaud_*` descriptive names are mine, not the original's: the words they stand for have no symbol-table
names. The four initialised words are defined in `w10-sce-a-lgaud-state` (under `norrath_lgaud_size_limit_view`,
`..._request_buffer_view`, `..._reply_words_view`, `..._semaphore_view`), so those names are not opened; in the EE
object the three zero words land in BSS and the semaphore word in `.data`, where the original keeps all four in
`.data` (values equal).

## Build-plan entries needed

Not added to the shared plan (and `snapshot.json` is untouched). The 43 emitted functions come from 46 inputs:
the new `w10-sce-a-*` units plus ten earlier-package sources listed in the first block of
`docs/agents/w10-sce-a-plan-entries.py`. Run

```sh
python3 docs/agents/w10-sce-a-plan-entries.py --toolchain /path/to/reviewed/ee [--private-root DIR]
```

for the entries with the current hashes (optionally with a private plan/snapshot copy). The two `.S` inputs use
`authored_entry_asm`; everything else `authored_cpp`. Header closure: the four tty units
(`tty-state`, `tty-init`, `tty-read`, `tty-write`) list `sources/w9-startup-crt-f-tty-state/tty_state.hpp`.
`input_order` is the integrator's. Object names are unique against the shared plan. Entries at this branch's
last source commit (one JSON object per line, `input_order` omitted):

```json

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

### snapshot.json file changes

`tools/check_source_snapshot.py` wants every file under `experimental/sources/` recorded. Hashes of four
existing files changed; 82 files are new (sources, `.S`, host contracts and checks). Because host files are not plan
inputs they only need `files` entries.

Changed (path, old sha256, new sha256):

```

sources/w9-startup-crt-f-pad-set-act-direct/pad_set_act_direct.cpp f2a4fb941a0df20c8ee7336bef060de1a32a939a8ff9e7c55bff8652708ede6b 1023e5e6a1697c3282f2a7c68f0844939a49f641f86379d926deabbaa1757cd2
sources/w9-startup-crt-f-tty-init/tty_init.cpp 010a504470d95b6ad12ed8d532ee554ec39faab7d0dc0da92ef39a7da4c58463 14cd11107a15a079d377abf266ed375891dc2f9653d23a74abe928d19b4ae75c
sources/w9-startup-crt-f-tty-state/tty_state.hpp 4787f73b09dc923dcb5d2eff91c76bfac0f645189f7512db8aac9e243c1300b7 fc88d38617b25be5e9a0008346515f3c27980831c53005ca884072f52b687a23
sources/w9-startup-crt-f-tty-write/tty_write.cpp 5d07d63b2f168a09e28c30285f2bb04806c24b6dd3b9d582cc5749c2a5e9c0b5 d18ccd23ffab3b46ece8e2c3988b9c8586e819772366c881059cb92705e0fad8
```

New files (path, sha256):

```

sources/simloop-services-pad-dma-buffer/host_check.cpp 7470b99a9ad7bcb83acf4db3a22d0ceab692309e72d9a5b397c3f5c9107df397
sources/w10-sce-a-cd-disk-ready-old/disk_ready_old.cpp d7ab203ff55c0a73127139a47d346ba4d6118dc7f4293d57f345755663b1e518
sources/w10-sce-a-cd-disk-ready-old/host_contract.cpp ae50bc3f65d60aa66a1c1b442a0f0d8c8df069adf47692101e79249e4d75c1e5
sources/w10-sce-a-connect-to-iop-server/connect_to_iop_server.cpp 4b8f2f25370e3b900f29e96efd41ac21c5752be83a49d48da0288302a76cf959
sources/w10-sce-a-connect-to-iop-server/host_contract.cpp 3c47ade131e1e11ef3ed2c8b445b7e3beb4909d75cf6bfb4f8549fb123d9ee45
sources/w10-sce-a-gif-pk-add-gs-ad/host_contract.cpp 342cea0a3390eba0abe4e036d68b4a2f7a5108ec2ce13c0c6d50321dd5b1738b
sources/w10-sce-a-gif-pk-add-gs-ad/pk_add_gs_ad.cpp dde8e5f35b552e5c4a0b1d495087abfdb19cf780a141c294d6dab3bde4a0fa8e
sources/w10-sce-a-gif-pk-close-gif-tag/host_contract.cpp 96d4616c0760b57df2573b45c5b72d745d1367f3f264915b34eae50860e92c20
sources/w10-sce-a-gif-pk-close-gif-tag/pk_close_gif_tag.cpp 559240b3d41f2b5aba1b884b3f9ccd137a296d9dde8fadba1c996c9c5f421dee
sources/w10-sce-a-gif-pk-cnt/host_contract.cpp 0d7695f6fc6d1e04d3330f0befd2647c9189fd09893e101cd938827074803dfe
sources/w10-sce-a-gif-pk-cnt/pk_cnt.cpp 84da8143a924ee1ca53e5d1a4b69ea8ee48a3d8f21adf307cc9a60f8837125c6
sources/w10-sce-a-gif-pk-open-gif-tag/host_check.cpp 825c837a08a09a8ec171af1efce3bbf837f9a488c65043c643d92de6571fa763
sources/w10-sce-a-gif-pk-open-gif-tag/pk_open_gif_tag.S f5803c9a5f7bbf4d6b31ea12c979159ae04ecda9879c970fca17443ab679c166
sources/w10-sce-a-gif-pk-ref/host_contract.cpp ecce856123189cdb4ff7548f22d3f744640774272f37e465d77bd2cf3e2766e0
sources/w10-sce-a-gif-pk-ref/pk_ref.cpp e6073ba5c460be13a14d294b6759d4b429c5dba43f32e2c7f734edecb37c435c
sources/w10-sce-a-gif-pk-reserve/host_contract.cpp 9fc2499890adfa287c2b924b2fee394141fc730932cc2c52622e83bcfcd93552
sources/w10-sce-a-gif-pk-reserve/pk_reserve.cpp 9a42f078955fd354a5cb79a3e0510afe6b1710beaab25c9839a2d0905635ffe9
sources/w10-sce-a-inet-create/host_contract.cpp ec4f93cb291fe2ab442dddb2b577bd2be687b61f154c2e24d6043762ca2bfaee
sources/w10-sce-a-inet-create/inet_create.cpp c2f45b29d3011424a3b1de0a535de3c3f667ea8900c96271d936e073761a8b74
sources/w10-sce-a-inet-interface-control/host_contract.cpp 9b1fbd0f20e9be0ffe75b6ad843affca0614eb9e513277792daf5f430332fb49
sources/w10-sce-a-inet-interface-control/inet_interface_control.cpp 9450f73209fd2dbd35367e6d0c1d0588f1671190e6dbaad5fdc60cf0e5a523a9
sources/w10-sce-a-inet-open/host_contract.cpp 7e32d5e6de4db1d00d041f3507903ec34c80ec31c78a7ffc2ecd0c236be9fb47
sources/w10-sce-a-inet-open/inet_open.cpp 9ba779b54138f6ceef638e70bd491d63b4c0995fe5f88fc08bb78e2bfc01c62d
sources/w10-sce-a-inet-recv-from/host_contract.cpp 5fb5075a6ed91b61ce684346962005e9ab502bea1647d1b9858401ab1555dddd
sources/w10-sce-a-inet-recv-from/inet_recv_from.cpp 19d6c3016c0e57ee7fac0159a8443895774e6f82a4743a941c75e952b4e7e1d3
sources/w10-sce-a-inet-send-to/host_contract.cpp 8a9af4ff808a181ca0b0605da4280a9f28ed60f708d63b767021d037f653f918
sources/w10-sce-a-inet-send-to/inet_send_to.cpp 154ecc3ca5097d17658638972f689ab551f04e131c2ba430bc5e66a6394be41c
sources/w10-sce-a-lgaud-a-read/host_contract.cpp be862b17b7f524378584ea4cca06f3d77db84acb5c090c88709d58a710ce26f7
sources/w10-sce-a-lgaud-a-read/lgaud_a_read.cpp c808b73289fd2ae112c1c1f46bb4fa6ded7ae407a0fa4d86a43f0103b2548879
sources/w10-sce-a-lgaud-a-sync/host_contract.cpp 5916bf3c71aa05a7635ee3c0b2e3ef7f5ccc7fe5a905db39e507b71668daceee
sources/w10-sce-a-lgaud-a-sync/lgaud_a_sync.cpp 6aa79378643aa10895b8c08d251cef545eeb26a8d0cabc4e39ca0b7ae37da6af
sources/w10-sce-a-lgaud-a-write/host_contract.cpp eca1e1070bf4f591c9bf0de9f84a9e50bd7dffd7a84156cc6052f435fd5f0105
sources/w10-sce-a-lgaud-a-write/lgaud_a_write.cpp d84a67a2951a2e010c13e9fe91b783fa89413e65cebb27400ade5685fc585114
sources/w10-sce-a-lgaud-enum-hint/host_contract.cpp 4fbc1588cea6ee1356e12c94e63463b6456f1e8635c0ea57b4d50a4f70a2fc2f
sources/w10-sce-a-lgaud-enum-hint/lgaud_enum_hint.cpp abec6035475e0a6e62b6079b046597a9cdb6d8bdd8d8f587a295cb1ccd83ce8f
sources/w10-sce-a-lgaud-enumerate/host_contract.cpp 23e871811e8e9fef18f0dd0c9b8e5ed5d601917f35c268ca1f966eb43a29d26f
sources/w10-sce-a-lgaud-enumerate/lgaud_enumerate.cpp 27f0b52d7bb58daae76a797c3211f3f81b10af5acdee39c50b003c12e51b710d
sources/w10-sce-a-lgaud-open/host_contract.cpp 068ee914a3f9c8242956a449860e175f8c54d386e04efac19a74a321ce78cd4a
sources/w10-sce-a-lgaud-open/lgaud_open.cpp af3d661770f3bed02d37a15b2cb07be4981db97351f1d2f63905210e81b99ffb
sources/w10-sce-a-lgaud-set-recording-volume/host_contract.cpp 2aedb4a968c5f618fcfb9a34f32cab36b77bdc895296db3e3fcc86e86441ea12
sources/w10-sce-a-lgaud-set-recording-volume/lgaud_set_recording_volume.cpp 08adf2c13435d6aa8ea1ac7cf86d89cfb48523f49459aed8b729f3e60ff68e85
sources/w10-sce-a-lgaud-start-playback/host_contract.cpp ecf1b13e27d6fff79f465b0f6c51ca773e508108ff3647ddb85874a039a7ba22
sources/w10-sce-a-lgaud-start-playback/lgaud_start_playback.cpp 38fa11eb505a8df02c6ddb3eb6ea715d92e8d0fa6bdb73a43dd75f1886f00eb3
sources/w10-sce-a-lgaud-start-recording/host_contract.cpp 01f60c9b984a51f9f0aa4087524e936c2e22e5c4224078f96f5262151909e059
sources/w10-sce-a-lgaud-start-recording/lgaud_start_recording.cpp 50e545bccf771e37ddd4dbe69bb5f65ce66b2a5bb08c88e61e3ab81e04cbebfb
sources/w10-sce-a-lgaud-state/host_check.cpp 50a623699326bc93c7231c360b899171d51e89f0975332e1d10bfab8d3086ba8
sources/w10-sce-a-lgaud-state/lgaud_state.cpp 9cacdfc405ae1c13e4d0c66f11f7570079ad191eeb015234502f4f5058e66bd9
sources/w10-sce-a-lgcodec-decode/host_contract.cpp b1806b5293019ca283adb583877d4eab1796207db0fb0f2de4fc43aefc96fe51
sources/w10-sce-a-lgcodec-decode/lgcodec_decode.cpp 626a90ad9b71d9ce01de8840679cebc9c0adf99e2210fc128750e92c83ab234c
sources/w10-sce-a-lgcodec-encode/host_contract.cpp d22dcdcdf24f01f4d368e0eeca6fa064b6b2451fa56ee892eba072eb04381705
sources/w10-sce-a-lgcodec-encode/lgcodec_encode.cpp 299bf104d7f16beeb2c1abf3b6a660fc275a35f499957ad0119e12121f8da171
sources/w10-sce-a-mce-sif-rpc-end-func/host_contract.cpp 93dd972146dc3f441d76e0da1d1d2a1fd391f9a74899656cf9e7dc3f6c0dca20
sources/w10-sce-a-mce-sif-rpc-end-func/mce_sif_rpc_end_func.cpp 00e6439a0bea49d7465aea26a13c94cd835a2eaaa82ef8200b6a89dfbfc19dcb
sources/w10-sce-a-mce-store-pwd/host_contract.cpp 920ecd09513e2217343d45014ebeaf773d03516c901d49d73bcaa880b2082c28
sources/w10-sce-a-mce-store-pwd/mce_store_pwd.cpp 27d58a3c179d9fa91527d963cfdfbc398aa8a00c344ec500a832cab7b9045f1b
sources/w10-sce-a-pad-get-button-mask/host_contract.cpp 504292f266823546b2e931153c2191c88ea7d280575f2a520f4263d6e611f9da
sources/w10-sce-a-pad-get-button-mask/pad_get_button_mask.cpp b11305ab717a871fe0c8d201dea53a370b36263dc14e0ad15d2dfaa27f058d73
sources/w10-sce-a-pad-set-button-info/host_contract.cpp bb7fcc407acb80284d5c9418fc7ae5eff013ce7b9d543a29a797b73412931ab2
sources/w10-sce-a-pad-set-button-info/pad_set_button_info.cpp fe240bbed063502ce65c51a8d8fb4401b5d98d63814be0266217c03d5b41567b
sources/w10-sce-a-pad-set-req-state/host_contract.cpp 866d1e877d0fb1af28f3e316e35ddabf8695e22fc6dbd6e150110e9b097cdf6e
sources/w10-sce-a-pad-set-req-state/pad_set_req_state.cpp 0efcd58044ff81a0a6dcbe32995f565a94d69cc9e725f217ad80d588d500d77a
sources/w10-sce-a-s-pad/host_check.cpp eab7e19a41129c06707d1a7eac9fbea5c6580cba5ad75945926d8d150e8bcb72
sources/w10-sce-a-s-pad/s_pad.cpp 59dd360ededee6d17e92bd29105a1e94c6df3f27d7e800252760d028b9cf9359
sources/w10-sce-a-sif-dma-stat/host_check.cpp 825c837a08a09a8ec171af1efce3bbf837f9a488c65043c643d92de6571fa763
sources/w10-sce-a-sif-dma-stat/sif_dma_stat.S 7a9cf1cec54865bf6a04fffa9ecbc6fadc88e8cc00768d66a5f60aa62e79194b
sources/w10-sce-a-sif-m-call-rpc/host_contract.cpp 2cf1c56f5265a10de0f3b18adb59e5f1bf21ae09a2748a05e261aaefb33b08c4
sources/w10-sce-a-sif-m-call-rpc/sif_m_call_rpc.cpp 9735ad2eef67f1904deda01ce892a6d7cbe8077fb3b515986b6d6ddeeb1ac0f6
sources/w10-sce-a-sif-m-init-rpc/host_contract.cpp 1b7fedec6f1f661662f775e58f4e5af4616fd4070d0b20331a808ee2f67d588a
sources/w10-sce-a-sif-m-init-rpc/sif_m_init_rpc.cpp 5b1e358ceae3b2da30c7e1033e2fa187e08017094643ccbd8a4346b7015c4f77
sources/w10-sce-a-sif-param-fname/host_check.cpp 83301f9b181e4323e88a47bcc9be5a213e292ac85cfe03ff79a5d27f346167a0
sources/w10-sce-a-sif-param-fname/sif_param_fname.cpp 452a158661fc516a4c311283c4052e9bf86f7d2026706498132fcf63a44ba148
sources/w10-sce-a-sif-remove-reboot-notify-handler/host_contract.cpp e8190e8fdb3b4ec253d249c1456a68b6bb4c9b19517319854085a8313f8d8871
sources/w10-sce-a-sif-remove-reboot-notify-handler/sif_remove_reboot_notify_handler.cpp 071bbd9e31a108011e7b70c105b3d6202c05e5dd6303dd529c3b6f3654db6600
sources/w8-startup-sce-d-sif-get-sreg/host_contract.cpp 6064a3651f45569c3ee4c22a2f81b396aaf27792fa874c10fbd73f7e328e990d
sources/w9-startup-crt-f-mc-read/host_contract.cpp f900cf392dbdaeebb86455d0be05f45a9523171d8291e97f98f199bd022abfd3
sources/w9-startup-crt-f-mc-write/host_contract.cpp d109876b5cbc94b43457769d61f7661ce25f49fe2533274a45d5e065e714a1c4
sources/w9-startup-crt-f-pad-read/host_contract.cpp a7b5500ea70531d0c9fa89c18754fe6d21b4a4d48917a03b7fc0e27a608a4ca0
sources/w9-startup-crt-f-pad-set-act-direct/host_contract.cpp 94e10ade610f57abb478b99df69c2abc5fd7165d30850d14aa36c2861ecbc9d6
sources/w9-startup-crt-f-tty-init/host_contract.cpp e72c52dc5773cb724a66dcdc487faa1e85d2844b96ffcc25e2d31e2dcb84be88
sources/w9-startup-crt-f-tty-read/host_contract.cpp 63d5e67e58b86f72e050a98d3e6744c4b44c7b7eda933f8acd84da4dca2b68cf
sources/w9-startup-crt-f-tty-state/host_contract.cpp 2ac34981d289f74a1e7fdf2e5d64248895da05b342556d348b8696b8dc70c5a1
sources/w9-startup-crt-f-tty-write/host_contract.cpp 0ee196b419d9c63d6e32dd69a92f744490530b4d06445a00e3416c1af63d745c
```

## What is authored but never executed

All of it, on an EE. The paired runs execute original and authored code on a private interpreter, not on hardware,
with stub callees, so interrupts, cache write-backs, the IOP side and real timing were never exercised. Specific
gaps: the unreachable `mode & 1` branch of `sceSifMCallRpc` (compiled out); the endless wait of
`connect_to_iop_server` on a negative bind result (a host child process is checked to still be running after 300 ms,
nothing more); the 16384- and 1048577-iteration delays (`connect_to_iop_server`, `sceCdDiskReady_old`: not
timed, and the retry paths of `sceCdDiskReady_old` are paired in only four scenarios because of their cost);
`sceTtyWrite` and the codec copy with negative counts; the completion routines' behaviour when the IOP calls them
from interrupt context (`iSignalSema` is only stubbed); the `$gp` value stored by `sceSifMCallRpc` (the paired
run compares it, the host contract replaces it by a variable).

## What is unknown

* `amx_Exec`: see above.
* Meaning of nearly every word and field: the audio request buffer layout beyond the offsets the functions use,
  the 332-byte enumeration description, the RPC service numbers (0x61766973, 0x8000059a), the GIF tag fields
  beyond what `sceGifPkCloseGifTag` computes, the pad block bytes 100/102/113/114/121..124, the libnet request
  words.
* Extents of the bss objects I only import (`norrath_lgaud_rpc_client_view`, `norrath_lgaud_async_record_view`,
  `norrath_sif_m_rpc_packet_buffer_view`); the true size of `rbuf` (256 bytes is a lower bound); what the 60 bytes
  of `sifParamFname` after the 1044 used are.
* Who calls `lgAudInit` to set the three pointer words, and whether `lgAudInit`/`lgAudRead`/`lgAudWrite` and the
  other `lgAud*` calls (not in this package) agree with the descriptive names above.
* Whether the original types of the `sceInet*` and `lgAud*` parameters are the ones used here (they are the
  smallest types that reproduce the observed instructions; return types are `int`/`unsigned`).
