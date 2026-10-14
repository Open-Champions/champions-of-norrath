# w10-audio-a: audio, music and streaming

Branch `agents/w10-audio-a`. Private executable `work/private/SLUS_205.65`; addresses and sizes
are the image's own symbol table, initial values were read from the image. Nothing here ran on
hardware or inside the game.

## Method and checks performed

- Functions were disassembled privately (nothing derived is committed) and rewritten as readable
  C++, then compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0). Every unit
  compiled. Each function unit has a `host_check.cpp` and each data unit a `host_contract.cpp`
  (host compiler); all exit 0.
- Private paired runs (outside git): a small R5900 interpreter ran the original function and the
  freshly compiled one from the same memory snapshot with identical hooked callees, comparing
  call logs, return value where relevant, and every changed byte outside the stack. The
  functions `gameSetMusicTimer__Fi`, `cdIsBusyStreamingNonwithstanding__Fv`, `cdEndStream__Fv`,
  `cdStreamLoadAsync__FPcT0i`, `cdStartStream__FPci` and `soundStartMusic__Fii` were checked this way
  in the earlier part of the package (counts are in their commit messages).
  `soundSetAmbientData__FPc` ran 113 texts (valid, malformed, CR/LF, comments, over-long names,
  random) with `memset`, `strchr`, `strstr` and `sscanf` hooked identically on both sides; 318 of
  its 330 instructions were executed (the rest are alignment nops and delay slots of branches never
  taken) and four deliberately broken variants were each caught.
- Data units: sizes are distances to the next symbol in the image (or the symbol's own size), and
  initial values are the image's bytes.

## Imports closed

`ambientGroup`, `cdEndStream__Fv`, `cdIsBusyStreamingNonwithstanding__Fv`, `cdStartStream__FPci`,
`cdStreamLoadAsync__FPcT0i`, `cutLoadedDialogStream`, `cutStartMusic`, `delaySoundList`,
`desiredMusicDuckAmount`, `disableSoundPlaying`, `displayMusicPos`, `gameSetMusicTimer__Fi`,
`groupVolumeScale`, `lastMusicState`, `musicDuckAmount`, `musicPlaying`, `musicStart`, `musicTimer`,
`nextChannel`, `nmAmbientGroups`, `nmBatchCommands`, `norrath_music_activity_ring_view`,
`playedStinger`, `sequence`, `sequenceInProgress`, `sequenceVolume`, `soundFrameNm`,
`soundSetAmbientData__FPc`, `soundStartMusic__Fii`, `stream`, `streamOutput`, `streamPaused`,
`streamSizeLeft`, `streamVolume`, `streaming`.

## Not closed

`norrath_inventory_exit_sound_resource_view`: its source already exists
(`w9-hud-inventory-items-b-view-exit-sound-resource`, owned by another package) but it was still in
the unresolved list; it likely needs a build-plan entry from that package, not new code here.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip. `profile` is
`authored_cpp`, `header_closure` is empty, and no `host_check.cpp`/`host_contract.cpp` is an input
(they include host headers). Body sizes are from the reviewed EE g++ `authored_cpp` profile, not the
original executable. No `private_object_sha256` is claimed. `snapshot.json` must list the files.

```json
{"source":"sources/w10-audio-a-ambient-state/ambient_state.cpp","object_name":"ambient_state.o","sha256":"244e49b2ae4687a81e109b23828eaafc434bd994568d7faf2c3ded7c3bce538d","emitted_functions":[]}
{"source":"sources/w10-audio-a-cd-end-stream/cd_end_stream.cpp","object_name":"cd_end_stream.o","sha256":"4aec806e0b9cf89e0b19cb905312089cf6eed66bc845047de880dee9e6113d4a","emitted_functions":[{"name":"cdEndStream__Fv","compiled_body_bytes":140}]}
{"source":"sources/w10-audio-a-cd-is-busy-streaming/cd_is_busy_streaming.cpp","object_name":"cd_is_busy_streaming.o","sha256":"d69dc4c620c9cc907e11efb7f1411f8a7350ceaa800c5684cbf83dd6dc30c43a","emitted_functions":[{"name":"cdIsBusyStreamingNonwithstanding__Fv","compiled_body_bytes":12}]}
{"source":"sources/w10-audio-a-cd-start-stream/cd_start_stream.cpp","object_name":"cd_start_stream.o","sha256":"a756091d4fbb823866d0f7fbe4606670189e678f3691cf97d602ac7bcfb62762","emitted_functions":[{"name":"cdStartStream__FPci","compiled_body_bytes":272}]}
{"source":"sources/w10-audio-a-cd-stream-load-async/cd_stream_load_async.cpp","object_name":"cd_stream_load_async.o","sha256":"59a2726cac78184bea60d78e7599e936cb63ccb61c6d8c55664839824b35374f","emitted_functions":[{"name":"cdStreamLoadAsync__FPcT0i","compiled_body_bytes":176}]}
{"source":"sources/w10-audio-a-music-state/music_state.cpp","object_name":"music_state.o","sha256":"36f3fa5a8e80ba45387014da687188bf60c5ce87051b3c6929aafeef699fc98d","emitted_functions":[]}
{"source":"sources/w10-audio-a-sequence-state/sequence_state.cpp","object_name":"sequence_state.o","sha256":"58c0f5a2ed0fe9aef85c70abfde476299b0c35dc638b31429ede28eafbd4efe0","emitted_functions":[]}
{"source":"sources/w10-audio-a-set-ambient-data/set_ambient_data.cpp","object_name":"set_ambient_data.o","sha256":"9cd24b1e55cb55a5e138df3a057f33d9bb847a7e15cd2675cbb90ca4ecb42451","emitted_functions":[{"name":"soundSetAmbientData__FPc","compiled_body_bytes":1176}]}
{"source":"sources/w10-audio-a-set-music-timer/set_music_timer.cpp","object_name":"set_music_timer.o","sha256":"e13a5e3a3f25f077e6a578a481adb4c63c1bac80a33d28c5e099ccdfede0e6ac","emitted_functions":[{"name":"gameSetMusicTimer__Fi","compiled_body_bytes":12}]}
{"source":"sources/w10-audio-a-sound-start-music-id/sound_start_music_id.cpp","object_name":"sound_start_music_id.o","sha256":"a9e38b8b6e2eaa053e1f91f8071043657dca4da5292cddafc22c6b8d8272f447","emitted_functions":[{"name":"soundStartMusic__Fii","compiled_body_bytes":116}]}
{"source":"sources/w10-audio-a-stream-state/stream_state.cpp","object_name":"stream_state.o","sha256":"84e53f96c731446c5b2b68d6f6b4102321db138a974c07ca539e1cd50065be09","emitted_functions":[]}
```

## Imports opened

Symbols my sources reference but do not define (from an `ld -r` of only my objects; others own them
under these names): `CDmode`, `_ctype_`, `cdBusy`, `cdDirectoryLookup__FPcPiT1i`, `cdResult`, `eRandState`, `enginePopBorderColor__Fv`, `enginePushBorderColor__FUi`, `engineSyncVBlank__Fv`, `memset`, `powerOffMode`, `sceCdStResume`, `sceCdStStart`, `sceCdStStop`, `scePrintf`, `soundMusicVolume`, `soundSetStreamVolume__Fif`, `soundStartDialogStream__Fiii`, `sscanf`, `strchr`, `strstr`.

## Authored but never executed

All of it. The host checks and the interpreter comparisons ran on this Mac only. In the paired runs
the libc callees of `soundSetAmbientData__FPc` and the sound/CD collaborators of the stream functions
were hooked, so the real `sscanf`, the CD library and the IOP sound client never ran together with these
functions. The interpreter does not model EE-specific float corner cases (denormals, saturation, NaN);
the float paths in `soundStartMusic__Fii` and the ambient start-time computation were sampled with
ordinary values only. The data units were never loaded as part of a linked image.

## Unknown

- The meaning of the two numbers at `ambientGroup` +16/+20 (they feed `animGetVolumeScale__Fii`) and of
  the countdown at +12 beyond "60 times a random time between min and max".
- Whether `soundSetAmbientData__FPc` was meant to reject more than sixteen groups or entries; nothing
  bounds them, and its line buffer is 160 bytes.
- Element types of `sequence` and `delaySoundList`, and whether the symbols are padded.
- The length of `cutStartMusic` beyond the 48 bytes to the next symbol, and the pointee type of
  `streamOutput`.
- Flag bits 0x2 and 0x20 and the 0.46875 scale in `soundStartMusic__Fii`.
- Why `norrath_music_activity_ring_view` holds non-zero bytes in the image before `runMusic__Fv` zeroes it.
