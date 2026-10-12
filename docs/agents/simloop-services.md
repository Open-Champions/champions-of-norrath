# Logo-loop pad, cd, and sound (simloop-services)

Takeover note: the first agent ran out of budget after 11 of the package
functions. This pass reviewed the clean worktree, kept those commits, and
finished the remaining symbols. Addresses are from the pinned SLUS-20565
symbol table and instruction bytes, used only as local reference. No game
binary, disassembly, or decompiler output is committed. Host checks are not
plan inputs; they use host headers and `NORRATH_HOST_CONTRACT` where a source
turns a 32-bit address word into a pointer.

Every function below is an independent reconstruction of the observed
behavior. None is claimed to be an original-byte match. Each has a host
contract that fails when the observed branch, store, or call order is wrong
(several were checked by mutating the source and confirming the contract
failed). Build a host check, for example:

```sh
c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/check \
  experimental/sources/<unit>/host_check.cpp experimental/sources/<unit>/<source>.cpp
/tmp/check
```

(`NORRATH_HOST_CONTRACT` is only needed for `sound-sequence-length` and
`sound-play-sequence`.)

## Functions

| Symbol | Address | Bytes | Unit |
|---|---|---|---|
| `padProcess__Fv` | 0x00167e68 | 708 | `simloop-services-pad-process` |
| `padInitPadIndexMap__Fv` | 0x00168228 | 44 | `simloop-services-pad-index-map` |
| `cdDirectoryLookup__FPcPiT1i` | 0x00164f00 | 540 | `simloop-services-cd-directory-lookup` |
| `cdProcess__Fv` | 0x00165238 | 436 | `simloop-services-cd-process` |
| `soundMakeSequenceUnpausable__Fi` | 0x0024f9e0 | 64 | `simloop-services-sound-make-unpausable` |
| `soundSetStreamVolume__Fif` | 0x0024f5b8 | 80 | `simloop-services-sound-set-stream-volume` |
| `soundMapSound__FPci` | 0x0024f608 | 124 | `simloop-services-sound-map` |
| `soundStartDialogStream__FiPci` | 0x0024ef40 | 136 | `simloop-services-sound-start-dialog` |
| `soundStartMusic__FPci` | 0x0024eed0 | 108 | `simloop-services-sound-start-music` |
| `soundGetStreamPlayStatus__Fi` | 0x0024adb8 | 248 | `simloop-services-sound-stream-status` |
| `soundResetStream__Fi` | 0x0024af80 | 272 | `simloop-services-sound-reset-stream` |
| `soundComputeSequenceLength__FP15AnimationHeader` | 0x0024c4c0 | 308 | `simloop-services-sound-sequence-length` |
| `soundPlaySequence__FP15AnimationHeaderP10GameObjectf` | 0x0024d308 | 492 | `simloop-services-sound-play-sequence` |
| `soundPlaySound__FP10_vagHeaderP6Point3fff` | 0x0024c6a8 | 1380 | `simloop-services-sound-play-sound` |
| `soundFrame__Fv` | 0x0024b7f8 | 492 | `simloop-services-sound-frame` |
| `scePadPortOpen` | 0x002d3710 | 484 | `simloop-services-sce-pad-port-open` |
| `scePadInit` | 0x002d34b0 | 320 | `simloop-services-sce-pad-init` |
| `sceMtapPortOpen` | 0x002d4a80 | 108 | `simloop-services-sce-mtap-port-open` |
| `sceMtapInit` | 0x002d4840 | 576 | `simloop-services-sce-mtap-init` |

Data: `musicPlaying` and `emergencyMusicWaitAbort` (4-byte `.sdata` words,
zero) and `musicDuckAmount` (1.0f) in `simloop-services-sound-music-state`;
`pad_dma_buf` (2560 bytes, the span to `stateData`, in
`simloop-services-pad-dma-buffer`); `stream` (1356 bytes, three 452-byte
records, in `simloop-services-sound-stream-records`); and the file-local
pad and multitap library state in `simloop-services-pad-mtap-state` (below).

### Observed

- `soundPlaySequence`: the last free sequence record wins, not the first;
  group-limit rejection can happen even when a free record exists. The
  authored `norrath_sound_play_sequence` in the frozen prefix does the same
  thing; this unit is an independent body under the original name because
  aliases in frozen-prefix sources are not kept.
- `soundPlaySound` with a positive delay never plays: it only queues into
  `delaySoundList` and returns -1. `soundFrame` plays due entries with
  `sequenceInProgress` restored from the entry.
- `soundFrame` reads two SPU words through `sceSdRemote(1, 0x8040, 0x1700/
  0x1701, 0)` every eighth frame and clears bit 4 of a channel's flags if
  its voice bit is set and bit 0x10 is clear. What these registers mean in
  hardware is not established.
- The pad and multitap bind loops wait 0x10001 passes of four nops after a
  failed bind. The first attempt has no wait. Pad ignores the bind return;
  multitap treats a negative return as failure.
- `scePadPortOpen` prepares two 128-byte halves of the caller buffer and
  keeps the reply words at the `PadInfo` slot.
- `sceMtapInit` prints the version mismatch unconditionally; `scePadInit`
  prints only while the isWarning word is nonzero.

## Data layout from evidence (pad-mtap-state)

All are file-local symbols in the executable; the published names are views.
`isInit` 0x004448f8 (0), `isWarning` 0x004448fc (1); `padsif` 0x008dd880
(two 40-byte clients); `PadInfo` 0x008dd8d0 (240 bytes, 28 per slot, 112 per
port); `dma_iop_data` 0x008dd9c0 (256); `buffer` 0x008ddac0 (128);
`port_open`, `port_close`, `get_connection`, `change_priority`,
`module_version` at 0x008ddb40 + 40n; `sif_buffer` 0x008ddc40 (128).
Views: `norrath_libpad_{is_init_word,is_warning_word,rpc_clients_view,
port_info_view,dma_info_view,rpc_buffer_view}` and
`norrath_libmtap_{port_open,port_close,get_connection,change_priority,
module_version}_client_view`, `norrath_libmtap_rpc_buffer_view`.

## Build-plan entries needed

Append after the current last input, in this order. Profile `authored_cpp`,
header closure empty. `snapshot.json` must also record each source (and the
host checks as files) and the emitted function name and compiled body bytes
for each unit. Do not plan `host_check.cpp`. This branch does not edit the
shared plan or snapshot; the build below used a private copy.

| Source | Object | SHA-256 |
|---|---|---|
| `sources/simloop-services-cd-directory-lookup/cd_directory_lookup.cpp` | `cd_directory_lookup.o` | `5358457ae15bb843df0d2483c7eddef648ce5fba284af510301a02f0913ce9c8` |
| `sources/simloop-services-cd-process/cd_process.cpp` | `cd_process.o` | `b8df8d554014274b732a3cbb8ccd2a3c4da06301967aa231eb8a7d3b52c5ac89` |
| `sources/simloop-services-pad-dma-buffer/pad_dma_buffer.cpp` | `pad_dma_buffer.o` | `5de751d33077b2f59d498c66f38b809407b16293c09dbab0a6e0bd1026726e4c` |
| `sources/simloop-services-pad-index-map/pad_index_map.cpp` | `pad_index_map.o` | `e5ac920eef39151e112bbed2727a4937034a5bf8d9983d601ca06e90f493efec` |
| `sources/simloop-services-pad-mtap-state/pad_mtap_state.cpp` | `pad_mtap_state.o` | `cd4cc30389e9190b46da94d0d284631c1242bd84a22fbb94ca8b37b17cb74bab` |
| `sources/simloop-services-pad-process/pad_process.cpp` | `pad_process.o` | `19ab5944dfc99bd31e8cbc3abb235714c8e08bb3f7c11cb08d0c3a8184022680` |
| `sources/simloop-services-sce-mtap-init/sce_mtap_init.cpp` | `sce_mtap_init.o` | `21bf63ca98618315b048c45575dfb4e140356564da1466e2934268b5911d3a20` |
| `sources/simloop-services-sce-mtap-port-open/sce_mtap_port_open.cpp` | `sce_mtap_port_open.o` | `d6863328f9d91cd6c173f1ecf42eee3e86c7dac4a0c23257a394ef35648e9fad` |
| `sources/simloop-services-sce-pad-init/sce_pad_init.cpp` | `sce_pad_init.o` | `c6dd241b30ad9e3f87be7eb6d4701ef61293d1f60ab98cda857985ba49d06bfb` |
| `sources/simloop-services-sce-pad-port-open/sce_pad_port_open.cpp` | `sce_pad_port_open.o` | `7ba53de6656b151f6e0ece8f18032106834459a5133d61de0672c4063034b143` |
| `sources/simloop-services-sound-frame/sound_frame.cpp` | `sound_frame.o` | `ebea775b668fd8b7daeb82d757f67f526e3c5b539d00a0afd0a9d9d5b26c86fc` |
| `sources/simloop-services-sound-make-unpausable/sound_make_unpausable.cpp` | `sound_make_unpausable.o` | `e11ac197628d956c5f12c16b5bba7bbcef00b21cad7042fccd8ad5a13ed06876` |
| `sources/simloop-services-sound-map/sound_map.cpp` | `sound_map.o` | `721c89c46c3a108dee2cb2f2686604a8e025fc00f3610d8a39491bc9c5af2985` |
| `sources/simloop-services-sound-music-state/music_state.cpp` | `music_state.o` | `53b20fbb954b254ad2af3dc0f65867999b2a1585582a446e9e246f9b3b6c9e38` |
| `sources/simloop-services-sound-play-sequence/sound_play_sequence.cpp` | `sound_play_sequence.o` | `bd01476c475778e2ea9cfe90c2c9fd7d3a80ee03ee41ba5761d4568221589884` |
| `sources/simloop-services-sound-play-sound/sound_play_sound.cpp` | `sound_play_sound.o` | `5eb8c22443bc42bcf4f50399edce820013e02125e1f618f1b8acf69a908ad101` |
| `sources/simloop-services-sound-reset-stream/sound_reset_stream.cpp` | `sound_reset_stream.o` | `97570ee826cc9b7780d28c2ce8c805a542a7446569893b1311b9626b92edb677` |
| `sources/simloop-services-sound-sequence-length/sound_compute_sequence_length.cpp` | `sound_compute_sequence_length.o` | `6f514598a587cfc83a9f0fe7bead4ebfd3a9ccddbde4e5d8ba4e1f2c4cd9a264` |
| `sources/simloop-services-sound-set-stream-volume/sound_set_stream_volume.cpp` | `sound_set_stream_volume.o` | `0296ae6b5f122ac2bb209bb8157b4fb628e202ace2c5a7ab09e2e918c469d1ad` |
| `sources/simloop-services-sound-start-dialog/sound_start_dialog.cpp` | `sound_start_dialog.o` | `0ed0e9d812e9e4e51d97be9660ea48b228e918e0e3de09ec2260660778831def` |
| `sources/simloop-services-sound-start-music/sound_start_music.cpp` | `sound_start_music.o` | `b783a6a0ab6c3cb65e707eb3bf2a6b4a70830842b22c0ce95d2ee3dc67f6ed42` |
| `sources/simloop-services-sound-stream-records/stream_records.cpp` | `stream_records.o` | `4876fd300e86a8fde7f3513926fb081f5d5e7cfa95680829240c4c56b3dd02c5` |
| `sources/simloop-services-sound-stream-status/sound_stream_status.cpp` | `sound_stream_status.o` | `481779478de69cb6214757aab9010c03df91ab88a8fff1829a2d2c0b33bf99de` |

A private plan with these 21 inputs appended to the current 845 built all 868
units (868 compiles, one relocatable link, 746 function bodies) with the
reviewed toolchain. `python3 -m unittest discover -s tools -p 'test_*.py'`:
214 tests passed.

## Imports closed

All 24 names in the package: `cdDirectoryLookup__FPcPiT1i`, `cdProcess__Fv`,
`emergencyMusicWaitAbort`, `musicDuckAmount`, `musicPlaying`,
`padInitPadIndexMap__Fv`, `padProcess__Fv`, `pad_dma_buf`, `sceMtapInit`,
`sceMtapPortOpen`, `scePadInit`, `scePadPortOpen`,
`soundComputeSequenceLength__FP15AnimationHeader`, `soundFrame__Fv`,
`soundGetStreamPlayStatus__Fi`, `soundMakeSequenceUnpausable__Fi`,
`soundMapSound__FPci`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`soundPlaySound__FP10_vagHeaderP6Point3fff`, `soundResetStream__Fi`,
`soundSetStreamVolume__Fif`, `soundStartDialogStream__FiPci`,
`soundStartMusic__FPci`, `stream`. The private build's unresolved list went
from 879 to 912 names: 24 closed, 57 opened.

## Imports opened

`DPRINT`, `batchBuffer`, `convertEnd__Fi`, `currentLayerDir`, `delaySoundList`, `disableSoundPlaying`, `findChannel__Fv`, `gCd`, `initLump__FPv`, `layer0Dir`, `layer1Dir`, `listOnewayLink__FP8ListHeadT0Pv`, `lumps`, `memRealloc__FPvi`, `netLocalPad`, `nmBatchCommands`, `nmLumps`, `nmSounds`, `padFrame.756`, `padGetUnprocessedData__FiP18padUnprocessedData`, `padIndexMap`, `padProcessData__FP3padP18padUnprocessedDataP12padStateData`, `pan__FG6Point3ffPfT3`, `pauseProcess__Fv`, `runAmbient__Fv`, `runMusicDucking__Fv`, `runScreaming__Fv`, `runSequences__Fv`, `sbuff`, `sbuffin`, `sceCdGetError`, `sceCdLayerSearchFile`, `sceCdStRead`, `sceCdSync`, `sceMtapGetModVersion`, `scePadGetModVersion`, `scePadGetSlotMax`, `scePadInit2`, `sceSdRemote`, `sequence`, `sequenceInProgress`, `sequenceVolume`, `soundChangeVolume__Fiff`, `soundFindDialog__FPc`, `soundFlushCommandBuffer__Fv`, `soundFrameNm`, `soundGetDialogFilename__Fv`, `soundGetFreeRam__Fv`, `soundList`, `soundStartStream__FiPciiiiii`, `soundUpdateAdjustments__Fv`, `strcasecmp`, `streamOutput`, `streamSizeLeft`, `streamVolume`, `streaming`, `tapCount`.

Of these, the original-named sound internals (`findChannel__Fv`,
`pan__FG6Point3ffPfT3`, `soundChangeVolume__Fiff`, `runMusicDucking__Fv`,
`runScreaming__Fv`, `runSequences__Fv`, `runAmbient__Fv`, `pauseProcess__Fv`,
`soundFlushCommandBuffer__Fv`, `soundUpdateAdjustments__Fv`, `convertEnd__Fi`)
and the data `delaySoundList`, `batchBuffer`, `nmBatchCommands`,
`sequenceInProgress`, `sequenceVolume`, `soundFrameNm`, `disableSoundPlaying`
belong to the sound code; no package defines them. `sceSdRemote`,
`scePadInit2`, `scePadGetModVersion`, `sceMtapGetModVersion`, `DPRINT`, and
`scePadGetSlotMax` are SDK functions. Remaining names come from the first
agent's earlier functions (cd, pad, dialog, stream). The views
`norrath_sound_sequence_id_spinner_view` (2 bytes at 0x00485940, zero) was
already an open import of the audio package and stays open.

## Overlaps for the integrator

- `stream` is defined here at 0x007dd290; `norrath_sound_stream_records_view`
  (audio) is the same address. `sequence` (0x007d8110) is imported by name by
  one earlier source and `norrath_sound_sequence_records_view` (audio) is the
  same object. Decide whether each pair is one symbol; the plan rejects
  symbol aliases.
- `soundPlaySequence__FP15AnimationHeaderP10GameObjectf` duplicates the logic
  of `norrath_sound_play_sequence` in a frozen-prefix source.

## Authored but never executed

Every function and data object above. Host contracts ran on this machine
against mocks; nothing ran on the EE. No RPC, SPU, pad, or CD hardware or IOP
module was exercised. The wait loops use an inline `nop` sequence and were not
timed.

## Unknown

- What the SPU words read through `sceSdRemote` at 0x1700 and 0x1701 are.
- The fourth field of the 36-byte `delaySoundList` entry beyond +32 and what
  `PadInfo` fields at +0 to +12 mean to the pad library beyond how they are
  stored here; 16 bytes after `module_version` are not owned.
- None of the functions is an original-byte match; instruction order, delay
  slots, and register use differ.
- `pad_dma_buf`'s alignment beyond the observed address, and whether only the
  first 1280 bytes per port are used.
