# w9-audio notes

Package: sound, music and dialog streaming. Every function below was compiled
with the reviewed `authored_cpp` profile, run against the original function in
a private paired-run interpreter (identical snapshots and callee stubs, call
logs and every changed byte outside the stack compared, mutations of the
authored source detected), and has a `host_contract.cpp` whose standalone run
passes. The paired-run harness and its output live outside git.

## Functions committed (one per commit)

- ac5ed215 Add sceCdSync with a host contract.
- 8ae97356 Add sceCdSearchFile with a host contract.
- e37fd161 Add sceCdGetError with a host contract.
- 6bf88acd Add sceCdStInit with a host contract.
- f0d0087a Add sceCdPOffCallback with a host contract.
- 76673301 Add sceGifPkInit with a host contract.
- 0a3f24ad Add sceGifPkReset with a host contract.
- 8944d6d1 Add sceGifPkSize with a host contract.
- 4a61dbe5 Add sceGifPkAddGsData with a host contract.
- 33fca4d8 Add sceGifPkTerminate with a host contract.
- b2553fa6 Add sceGszbufaddr with a host contract.
- 1d0e8efd Add sceInetClose with a host contract.
- fb593746 Add sceInetCtlDownInterface with a host contract.
- 64358184 Add sceInetCtlGetState with a host contract.
- a2824e56 Add sceInetGetInterfaceList with a host contract.
- 534814a7 Add sceLibnetInitialize with a host contract.
- 78ca2ba3 Add sceCdReadDvdDualInfo with a host contract.
- 69b4965d Add sceCdReadClock with a host contract.
- 425e8c29 Add sceCdStRead with a host contract.
- b25e9361 Add sceCdLayerSearchFile with a host contract.
- 82ead29b Add lgKbdClose with a host contract.
- 90b2f69a Add lgCodecOpen with a host contract.
- 7baf527e Add sceClose with a host contract.
- 8d2cf0cb Add soundGetGlobalDialogDir__Fv with a host contract.
- bbbde8fa Add soundDisable__Ff with a host contract.
- 1ac1ef8c Add soundQuiet__Fv with a host contract.
- 29de4a3f Add soundAllFlags__Fi with a host contract.
- adbc372e Add soundReallyAllFlags__Fi with a host contract.
- cad5ecdf Add soundStartGlobalDialogStream__FiPci with a host contract.
- a4133aed Add soundStartDialogStream__Fiii with a host contract.
- c74fcbec Add startDlgRandom__FP9dialogDirPciT1i with a host contract.
- 89314e09 Add soundStartGlobalDialogStreamRandom__FiPci with a host contract.
- 7e9b15ae Add animGetVolumeScale__Fii with a host contract.
- 4484f9a5 Add soundChangeSequenceVolume__Fif with a host contract.
- 2d5c1fa4 Add soundMakePersistant__Fi with a host contract.
- 4900a6cf Add soundSetAudioEffect__Fiiiii with a host contract.
- 21415539 Add runStreamQueing__Fv with a host contract.
- c44a02ff Add playSoundForDlg__FP9DlgPacket with a host contract.

## Imports closed

animGetVolumeScale__Fii, playSoundForDlg__FP9DlgPacket, runStreamQueing__Fv,
soundAllFlags__Fi, soundChangeSequenceVolume__Fif, soundDisable__Ff,
soundGetGlobalDialogDir__Fv, soundMakePersistant__Fi, soundQuiet__Fv,
soundReallyAllFlags__Fi, soundSetAudioEffect__Fiiiii,
soundStartDialogStream__Fiii, soundStartGlobalDialogStreamRandom__FiPci,
soundStartGlobalDialogStream__FiPci. Also closed as a helper:
startDlgRandom__FP9dialogDirPciT1i.

## Imports opened

From the new sources, left unresolved: sprintf, strcpy, strlen, strncmp,
cdProcess__Fv, cdLoadAsync__FPcPvT0ii, soundStartDialogStream__FiPci,
soundGetDialogSize__FPc, soundFindDialog__FPc, soundFindGlobalDialog__FPc,
soundGetDialogFilename__Fv, soundGetDialogDir__Fv, soundChangeVolume__Fiff,
pan__FG6Point3ffPfT3, memAlloc__FiPc, memFree__FPv, sceSdRemote,
get__11ResourceMap, soundStartStream__FiPciiiiii, and the data symbols
animLoadingComplete, animLoadBuffer, animUseBuffer, animStarted, waitForSound,
anim (the 32-byte one at 0x007d0dc8, authored in w8-core-loop-a-anim-slots),
language, eRandState, effectsBufferSize, effectModeSize, nmSounds, soundList,
pitchTab, groupVolumeScale, disableSoundPlaying, stream, plus the
`norrath_sound_*_view` imports already published.

## Build-plan entries needed

Profile `authored_cpp`, header closure empty, `independently_authored_source`.
Source (relative to `experimental/`) and SHA-256; object name is the file stem
with `.o`. Do not plan `host_contract.cpp`.

- `sources/w9-audio-anim-get-volume-scale/anim_get_volume_scale.cpp` `a8b3c7efde4da9958a94b287c8bb1c6ae7691d95bc0f0bf55d817709bd323da2`
- `sources/w9-audio-get-global-dialog-dir/get_global_dialog_dir.cpp` `65b84cebc00c2f2587408d9fec181757443e7e7295581691e6445ed05bf16be8`
- `sources/w9-audio-play-sound-for-dlg/play_sound_for_dlg.cpp` `c0e2bf6cbb6cd53d1f5ba54374e421e5bdc581cb7dfb0564c1cd0350e5061559`
- `sources/w9-audio-run-stream-queing/run_stream_queing.cpp` `5f6fe3b660546aeb7f84a87f0763aebc05bd8d6cb846692b5e25260a97b84572`
- `sources/w9-audio-sound-all-flags/sound_all_flags.cpp` `514faf7bfcb37868c16218db38e8e522fb033ba2b34ef84b6d55fd4c9c19883c`
- `sources/w9-audio-sound-change-sequence-volume/sound_change_sequence_volume.cpp` `77c8ce11c484c2052a3baad15051224db1e5802d49bb7cb4a48b56f0b226e39d`
- `sources/w9-audio-sound-disable/sound_disable.cpp` `4449053753eb387880a66565780715aed0b9abc5a4627b6f79649f4ecdb1b0d7`
- `sources/w9-audio-sound-make-persistant/sound_make_persistant.cpp` `ae97d0f7df890c38822c881b662a5d52a2ab0c9d5f75a1ac6c089862bf643697`
- `sources/w9-audio-sound-quiet/sound_quiet.cpp` `79183b4c21d4bdc366c8ed4e79b3245a95a4197e6d6edfc434dd673535174477`
- `sources/w9-audio-sound-really-all-flags/sound_really_all_flags.cpp` `01e3c730fa634458eac564be45db5110425b4c653f3b6a1978a9d29c78e70e22`
- `sources/w9-audio-sound-set-audio-effect/sound_set_audio_effect.cpp` `8c445323ee37a97daa2913ce5a885d552ec19a31d565c0cf4fb9a8092e38b703`
- `sources/w9-audio-start-dialog-stream/start_dialog_stream.cpp` `e11ceaf9023339762e6f157b70ecf7e38e3e043a5d8a277bc74af8e71d58d530`
- `sources/w9-audio-start-dlg-random/start_dlg_random.cpp` `0300322a3e8055f3fd324b2173f7fa4e9dec91fc9cd1e59b0f45488c82587670`
- `sources/w9-audio-start-global-dialog-stream-random/start_global_dialog_stream_random.cpp` `cd43d1b63d79082e9330119f7af6a0ed6590a0f70c36181e06d1ca6f7e7f08f2`
- `sources/w9-audio-start-global-dialog-stream/start_global_dialog_stream.cpp` `6fdc0c7b10f419cb31bf1e23da865b8cbf2ab63b464edfffaf83c286f8c5e54a`

## Authored but never executed on the real target

Everything above is only compiled and run on the private interpreter and the
host; nothing has run on hardware or an emulator. Stack garbage that the
original copies into RPC send buffers (runStreamQueing, see its source comment)
is not reproduced.

## Not done / unknown

- Data symbols left unauthored (no initial values or element types
  established; each needs the private image bytes read and a layout decision):
  alwaysLoadedPage, ambientGroup, delaySoundList, desiredMusicDuckAmount,
  disableSoundPlaying (extern only), groupVolumeScale (0x4b0 bytes, extern
  only), lastMusicState, musicDuckAmount, musicPlaying, musicStart, musicTimer
  (two local symbols share this name), nextChannel, nextIndex, nmAlwaysPages,
  nmAmbientGroups, nmChars, norrath_music_activity_ring_view, soundFrameNm,
  streamOutput, streamPaused, streamSizeLeft, streamVolume (12 bytes),
  streaming, and `stream` itself (three 452-byte records observed).
- Meaning of RPC commands 64, 80, 96, 112 and of stream 2 / flags 42 in
  playSoundForDlg is not established.
- The current unresolved-import list was not regenerated (no full partial
  build was run, to keep the machine responsive).
