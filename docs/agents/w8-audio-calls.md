# Sound and music calls (w8-audio-calls)

Addresses, sizes, and instruction behavior below were read from the pinned
SLUS-20565 image on this machine. Observed means read from those bytes.
Inferred means deduced from callers. Unknown means not established. Nothing
here ran on the EE. Compiled bodies are not claimed as original-byte matches.

Compile check used the reviewed `authored_cpp` flags with the toolchain
at `work/toolchains/ee`. Data units are verified against the executable with
`docs/agents/w8-audio-calls-data-check.py`.

## Sound state data

Unit `sources/w8-audio-calls-sound-state/sound_state.cpp`. Defines
`streamVolume`, `streaming`, `streamOutput`, `streamSizeLeft`,
`musicPlaying`, `musicStart`, `emergencyMusicWaitAbort`, `musicDuckAmount`,
`soundFrameNm`, `sequenceInProgress`, `sequenceVolume`, `musicTimer`,
`disableSoundPlaying`, `sequence`, `delaySoundList`, `stream`. Sizes and
initial bytes of all sixteen match the image (checker output: sixteen `ok`).
The same storage also has `norrath_sound_*_view` names in the audio package;
both sets cannot be linked into one image without choosing one name per object.

## Functions

Each function is its own commit. "Contract" is a host test in the same
directory (`host_contract.cpp`, not a plan input) that builds the source
for the host with stub callees and fails on a wrong result, order, argument
or neighboring write. Expected values come from reading the original
instruction bytes, not from running them. Compiled sizes are from the
reviewed profile; none is an original-byte match.

| Original name | Address | Orig. bytes | Notes |
|---|---|---|---|
| `soundGetNmQueued__Fi` | 0x0024f388 | 40 | word at +448 of a 452-byte stream record |
| `soundSetMusicDuckAmount__Ff` | 0x0024f4a0 | 20 | `desiredMusicDuckAmount = 1 - x` |
| `soundLock__Fi` | 0x0024f540 | 68 | raises signed lock byte (+12) of the first `nmSounds` list records |
| `soundGetFreeRam__Fv` | 0x0024f588 | 44 | 0x200000 - `effectsBufferSize` - last record size word |
| `getStockPitch__FP10_vagHeader` | 0x0024f8e0 | 44 | `(convertEnd(+16) << 12) / 48000` |
| `findChannel__Fv` | 0x0024f910 | 100 | round-robin over 43 records, tests halfword +4 against 0x5 |
| `soundFindDialog__FPc` | 0x0024a120 | 172 | last language match, else last untagged |
| `soundGetDialogSize__FPc` | 0x0024ede8 | 48 | next record start word minus this one |
| `soundGetDialogFilename__Fv` | 0x0024f688 | 64 | `"%s.vat"` of the level name into a static 80-byte buffer |
| `soundClearStreamQue__Fi` | 0x0024f0a8 | 140 | clears counted 48-byte blocks, then +36 and +448 |
| `soundContinueStream__Fi` | 0x0024f218 | 108 | clears bit 0x8 of the first flagged counted block |
| `soundGetStreamPreloadDone__Fi` | 0x0024aeb0 | 208 | first block flagged 0x8, +36 word vs 0xc800, field +20 |
| `soundGetStreamAmountPlayed__Fi` | 0x0024f3b0 | 148 | command 80 RPC, receive word at 16 * index + 12 |
| `soundStartGlobalMusic__FPci` | 0x0024a390 | 156 | stream 0 volume, then `mouth.vat` from the global dialog record |
| `soundChangeVolume__Fiff` | 0x0024cc10 | 292 | scales, optional mono mix, clamps 0..16383, two batch entries |
| `soundStartStream__FiPciiiiii` | 0x0024a660 | 380 | fills the next 48-byte queue block after a directory lookup |
| `soundFlushCommandBuffer__Fv` | 0x00249fb0 | 364 | masks key words, appends tagged entries, sends the batch |
| `transferVAG__FP10_vagHeaderi` | 0x0024d0c8 | 568 | staged copy, pieces of up to 4096 bytes to sound memory |
| `soundClear__Fi` | 0x0024e3c8 | 696 | resets delayed sounds, channels, list, sequences, RPC 112 x3 |
| `soundPlaySequence__FP15AnimationHeaderG6Point3f` | 0x0024d4f8 | 424 | position overload of the authored object overload |
| `_GLOBAL_$I$soundSfxVolume` | 0x0024fac8 | 32 | calls the authored static initializer, phase 1, 0xffff |
| `_GLOBAL_$D$soundSfxVolume` | 0x0024fae8 | 32 | same, phase 0 |
| `soundInit__Ff` | 0x0024c010 | 428 | forwarding entry to `norrath_sound_initialize` |
| `soundRestore__Fb` | 0x0024bc38 | 984 | forwarding entry to `norrath_sound_restore` |
| `soundUpdateAdjustments__Fv` | 0x0024c1c0 | 228 | forwarding entry to `norrath_sound_update_adjustments` |
| `soundGetDialogDir__Fv` | 0x0024f6c8 | 132 | opened callee: `"%s.lmp"` lump query, resource `dialog.bin` |
| `soundFindGlobalDialog__FPc` | 0x0024a1f0 | 212 | opened callee: same search over the global dialog map |
| `soundPrintStream__Fi` | 0x0024f448 | 84 | opened callee: empty countdown of the queue count |
| `soundError__FPc` | 0x0024f9a0 | 64 | opened callee: prints, then asserts at line 1904 |
| `soundStopSound__Fi` | 0x0024cf80 | 328 | opened callee: channel key off, or sequence id stop |

The three forwarding entries are not reconstructions of the original bodies.
The audio package already authored those bodies under `norrath_sound_*` names
and published original-name aliases, but the selected build keeps the frozen
sources without the alias lines, and an alias cannot cross translation units.
Each entry is an 8-byte forward; linking it fills the import and nothing more.

Data (sixteen objects in `sound_state.cpp`, three in `sound_state_extra.cpp`)
was checked with `docs/agents/w8-audio-calls-data-check.py` against the image:
size and initial bytes match for all nineteen.

## Corrections made while working

`soundStartGlobalMusic__FPci` first compared the music volume with 0.0f. The
compared word at 0x0048200c is in the `.lit4` float pool and holds the bits
0x3d4ccccc (one unit under the nearest 0.05f). A symbol label at a nearby
address (`zeroes.1`) was misleading; the corrective commit follows the bytes.

## Imports closed

`_GLOBAL_$D$soundSfxVolume`, `_GLOBAL_$I$soundSfxVolume`, `delaySoundList`,
`disableSoundPlaying`, `emergencyMusicWaitAbort`, `findChannel__Fv`,
`getStockPitch__FP10_vagHeader`, `musicDuckAmount`, `musicPlaying`,
`musicStart`, `musicTimer`, `sequence`, `sequenceInProgress`,
`sequenceVolume`, `soundChangeVolume__Fiff`, `soundClearStreamQue__Fi`,
`soundClear__Fi`, `soundContinueStream__Fi`, `soundFindDialog__FPc`,
`soundFlushCommandBuffer__Fv`, `soundFrameNm`, `soundGetDialogFilename__Fv`,
`soundGetDialogSize__FPc`, `soundGetFreeRam__Fv`, `soundGetNmQueued__Fi`,
`soundGetStreamAmountPlayed__Fi`, `soundGetStreamPreloadDone__Fi`,
`soundInit__Ff`, `soundLock__Fi`,
`soundPlaySequence__FP15AnimationHeaderG6Point3f`, `soundRestore__Fb`,
`soundSetMusicDuckAmount__Ff`, `soundStartGlobalMusic__FPci`,
`soundStartStream__FiPciiiiii`, `soundUpdateAdjustments__Fv`, `stream`,
`streamOutput`, `streamSizeLeft`, `streamVolume`, `streaming`,
`transferVAG__FP10_vagHeaderi`. That is all 39 names in the package.

Extra names defined that nothing imported yet: `nextChannel`,
`desiredMusicDuckAmount`, `paused`, and the functions `soundGetDialogDir__Fv`,
`soundFindGlobalDialog__FPc`, `soundPrintStream__Fi`, `soundError__FPc`,
`soundStopSound__Fi` (all opened as imports by the 39 and then closed).

Check performed: all 32 compiled objects of this package were linked with
`ld -r` onto the object from a fresh partial build of the shared plan
(`game-components-partial.o`, 1195 unresolved). The link reported no
multiple definition and no COMMON. All 39 names left the undefined list; the
list went from 1195 to 1157; the only additions are the three names above.

## Imports opened

Three symbols are called by this package's sources and still defined by
nothing in the link: `sceSdTransToIOP` (called by `transferVAG__FP10_vagHeaderi`
and `soundFlushCommandBuffer__Fv`; an SDK wrapper, not read),
`soundSetAudioEffect__Fiiiii` (0x0024e680, 1060 bytes, called by
`soundClear__Fi`; not attempted, a large body of SPU register values), and
`get__11ResourceMap` (called by `soundFindGlobalDialog__FPc`; the ResourceMap
method that returns the table of a map).

Five more names the first link showed as opened (`soundError__FPc`,
`soundFindGlobalDialog__FPc`, `soundGetDialogDir__Fv`, `soundPrintStream__Fi`,
`soundStopSound__Fi`) were then authored here and are listed above.

Imported by these sources and already unresolved or owned by other packages:
`cdDirectoryLookup__FPcPiT1i`, `convertEnd__Fi`, `lumpQuery__FPc`,
`lumpFindResource__FPcT0`, `scePrintf`, `sprintf`, `strcmp`, `strcpy`,
`memset`, `memcpy`, `FlushCache`, `sceSdRemote`, `memAlloc__FiPc`,
`memFree__FPv`, `norrath_sound_assert`, `norrath_sound_call_rpc`,
`batchBuffer`, `nmBatchCommands`, `soundFadeStream__Fi`,
`soundSetStreamVolume__Fif`, `listDestroy__FP8ListHead`,
`animAdd__FP14AnimationStateP15AnimationHeaderi`, `gameIsPaused__Fv`,
`language`, `engineCurrentLevelName`, `soundMusicVolume`, `soundSfxVolume`,
`soundStereoEnable` and the `norrath_sound_*_view` objects from the audio
packages. `soundStereoEnable` is the audio package's
`norrath_sound_stereo_enable_word_view` by address; I did not check each of
these names against the current link beyond the check below.

## Storage that exists twice

The original-name objects defined here (`sequence`, `stream`,
`delaySoundList`, `streamVolume`) are the same bytes as the audio package's
`norrath_sound_sequence_records_view`, `norrath_sound_stream_records_view`,
`norrath_sound_stream_volumes_view` and the dialog-maps unit. The authored
callers split: the simloop sources use `sequence`, `stream` and
`delaySoundList`; the play-sound and stop/clear code use the `norrath_*`
views for the channel, key, voice and batch tables. In one image `sequence`
and `norrath_sound_sequence_records_view` would be two separate 2432-byte
objects, and `soundClear__Fi` here clears one while `soundPlaySound` reads
the other. The integrator must pick one name per object (link-time alias or
renaming the callers) before this storage is treated as correct. I did not
change other packages' sources.

## Build-plan entries needed

Append these after the current last input. Profile `authored_cpp`, header
closure empty for every entry. `host_contract.cpp` files are not plan inputs.
`snapshot.json` must record all these files and the host contracts before
`tools/check_source_snapshot.py` accepts the tree; this branch edits neither
the plan nor the snapshot. Hashes are of the files at the last commit of this
branch.

| Source | Object name | SHA-256 |
|---|---|---|
| `sources/w8-audio-calls-change-volume/change_volume.cpp` | `change_volume.o` | `c1cf64a4376287f01ffe759d3c9efec3618a4231e9baaad95c1c461d87d31137` |
| `sources/w8-audio-calls-clear-stream-que/clear_stream_que.cpp` | `clear_stream_que.o` | `889dc490b50b46f39d0d1a3641ee598dd3ea007d89ce0e74e9d851a133a1631a` |
| `sources/w8-audio-calls-continue-stream/continue_stream.cpp` | `continue_stream.o` | `120a49465725110aa9a2dc1ae6780a399a264004f5fec7d4e2bf85458e452f29` |
| `sources/w8-audio-calls-find-channel/find_channel.cpp` | `find_channel.o` | `a030ce921fff0c4939ef6fdc7b902e6718e1ebbd5b51f880b541d1b15f97e5e7` |
| `sources/w8-audio-calls-find-dialog/find_dialog.cpp` | `find_dialog.o` | `6505a9213080a3d716279fa166595dc63a297210d51746b51d3afe7e9f18f008` |
| `sources/w8-audio-calls-find-global-dialog/find_global_dialog.cpp` | `find_global_dialog.o` | `d42f4c04cd0f8360c17321d8518d1ca30eb156d6f2ca8ff103e0ced4103824c9` |
| `sources/w8-audio-calls-flush-command-buffer/flush_command_buffer.cpp` | `flush_command_buffer.o` | `00f8b4c1621c1a1c7c18ef79b883668e4fb9a771d81f0b388a125a9700fe7592` |
| `sources/w8-audio-calls-get-dialog-dir/get_dialog_dir.cpp` | `get_dialog_dir.o` | `8bab24fbee2b770adc3cd78b32f41d9b9124486ff6f119d22515092d7a4cb778` |
| `sources/w8-audio-calls-get-dialog-filename/get_dialog_filename.cpp` | `get_dialog_filename.o` | `e505838b6187b755a06171571fd6ff0ba2815745e6bed78a6291b4fd3b5fb171` |
| `sources/w8-audio-calls-get-dialog-size/get_dialog_size.cpp` | `get_dialog_size.o` | `e24461691176a12230f41f2fd096d67a21d80603cf566bc60cefb203c2a77350` |
| `sources/w8-audio-calls-get-free-ram/get_free_ram.cpp` | `get_free_ram.o` | `4fdc8514039e5c2f8d9da0b1b58eb0bf5085bc4bddb8e25ea70fc2bd6c6dfe47` |
| `sources/w8-audio-calls-get-nm-queued/get_nm_queued.cpp` | `get_nm_queued.o` | `7bad58dcccda49e84e4cddc103298599dd1c816789981e91d0988e2205c2c739` |
| `sources/w8-audio-calls-get-stock-pitch/get_stock_pitch.cpp` | `get_stock_pitch.o` | `3205dd448245c0ce9ef5bf17ba60b7216cbcca7629f254ce9f4142f1473643a2` |
| `sources/w8-audio-calls-get-stream-amount-played/get_stream_amount_played.cpp` | `get_stream_amount_played.o` | `34c525006fe951f6d7e5289969c7ce860e379118de242e67cc878453e71b6a1e` |
| `sources/w8-audio-calls-get-stream-preload-done/get_stream_preload_done.cpp` | `get_stream_preload_done.o` | `30668b7256952e256e7b82e185ed6acb1723c5bb0f980dbd0af14253967ff399` |
| `sources/w8-audio-calls-global-destroy/global_destroy.cpp` | `global_destroy.o` | `f979d02715e9ca8a305df5293c38e486c26bb215fc46847297a25caf22544925` |
| `sources/w8-audio-calls-global-init/global_init.cpp` | `global_init.o` | `87397e2cb1d23589d275bde2306aadc1fdf58a70acbf8a745a35af220b84c6c7` |
| `sources/w8-audio-calls-lock/sound_lock.cpp` | `sound_lock.o` | `24d481b57b81b9de41c82c2183a1e77162e7943b8c272c11a770265b1a71148b` |
| `sources/w8-audio-calls-play-sequence-at-point/play_sequence_at_point.cpp` | `play_sequence_at_point.o` | `04e352573c32dce7e0d124e006dc8f4eda910420ad8db0464335aaadd5583c61` |
| `sources/w8-audio-calls-print-stream/print_stream.cpp` | `print_stream.o` | `98c3c7856b6773eae6079d59cd68c625a9b569fe18f54a18b3ce7321ddd44f99` |
| `sources/w8-audio-calls-set-music-duck-amount/set_music_duck_amount.cpp` | `set_music_duck_amount.o` | `0cae69c1896fad7b8d0b173c4e4a7c5d97b188a3b1017797f9a11ad7f89e8de6` |
| `sources/w8-audio-calls-sound-clear/sound_clear.cpp` | `sound_clear.o` | `36f19e89fc2cad72b7c01bb0ae84e89a533cb122d7113144dfb1b7089230bd0b` |
| `sources/w8-audio-calls-sound-error/sound_error.cpp` | `sound_error.o` | `0909bd279bf9cb76e06970185206ffc18ed4d95c665bc39737a69a3f75be6a36` |
| `sources/w8-audio-calls-sound-init-entry/sound_init.cpp` | `sound_init.o` | `41e46eadaacd8881d11b1dd0ebdf8fc4fa29e093d13ed60dda7afac9a8f63194` |
| `sources/w8-audio-calls-sound-restore-entry/sound_restore.cpp` | `sound_restore.o` | `bb9e8d5b94816938396f3ee6ccad54f7ff477260c9b68eb672df2abd16d659d3` |
| `sources/w8-audio-calls-sound-state/sound_state.cpp` | `sound_state.o` | `50dad4c2b1a4a8babc1d1f990d5003c4e1ffa3fa1afa1739a758829598d67e6b` |
| `sources/w8-audio-calls-sound-state/sound_state_extra.cpp` | `sound_state_extra.o` | `05132840f02ffe353b0fafb284913b5bd1deeee975c6ca1431dac1ecce2bc1f0` |
| `sources/w8-audio-calls-start-global-music/start_global_music.cpp` | `start_global_music.o` | `4f37e78460fc4530a32f76f878de0f90b994a315606ebda1b0d9ab6941dbd152` |
| `sources/w8-audio-calls-start-stream/start_stream.cpp` | `start_stream.o` | `09c7eba1e007c89097b4368ddb7ee96f8ee318d9819a5922479f546c5b303fb1` |
| `sources/w8-audio-calls-stop-sound/stop_sound.cpp` | `stop_sound.o` | `d197d37ffebe5b46871b195e977a4688b6f30c02aa1b34ce1f593ddc376ce2a7` |
| `sources/w8-audio-calls-transfer-vag/transfer_vag.cpp` | `transfer_vag.o` | `8062d2f9da167bca3a04001333df0eadeb7eff4ab0166a8f4c2fdb8953a7ffc7` |
| `sources/w8-audio-calls-update-adjustments-entry/update_adjustments.cpp` | `update_adjustments.o` | `d8fe163ccfd5d72bdd08c0c74d5e019ba2a77760bebf8fe6793543ee50119728` |

Order matters only for the object list; no entry includes another.

## Authored but never executed

Everything above. The host contracts ran on this machine's compiler, with
stub callees, never the target code. Nothing ran on the EE: no RPC, DMA,
SPU2 transfer, key-on or key-off, no sound hardware, no timer. The assertion
return register in `transferVAG__FP10_vagHeaderi` and `soundError__FPc` is
read with inline assembly on the target and replaced by a constant in the
host contracts, so that one instruction was compiled but never tested.

## Unknown

- Meaning of the RPC commands 80, 112, 176, 192, the batch command numbers
  1 and 2, tags 0x1500/0x1600, 0x80b0, 0x80d0, 0x80f0 and the 0x100 flag.
- Names of the stream queue block fields (+16/+24, +20/+28, +32, +36, +40,
  +44) and of the stream record words at +36, +448.
- The two bits tested by `findChannel__Fv` (0x5 of the halfword at +4).
- What fills the 16-byte send block's words 2 and 3 in `soundClear__Fi` and
  both blocks in `transferVAG__FP10_vagHeaderi`: the original copies stale
  stack bytes; this source does not write them.
- The unrelated second `musicTimer` at 0x00484f0c (read by
  `gameSetMusicTimer__Fi`) is not defined here.
- Whether `soundGetDialogDir__Fv`'s table walk has any observable effect
  (it reads memory only) and whether a null resource is ever returned.
- Behavior of floats outside the tested ranges when converted to integers
  (the target saturates; the host rounds toward zero and may wrap).
- `soundSetAudioEffect__Fiiiii` and `sceSdTransToIOP` bodies.
