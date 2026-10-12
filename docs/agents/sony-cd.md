# Sony CD and SIF

Branch `agents/sony-cd`. These units publish the CD/SIF names the archive layer calls. Host contracts compile with the system C++ compiler and fail on a wrong result. `PollSema`, `ReferThreadStatus`, and `iSignalSema` are the 16-byte syscall bodies at 0x002d5130 (service 69), 0x002d4fe0 (service 48), and 0x002d5110 (service -67); each matches the pinned ELF. The other functions are C reconstructions of the observed control flow. Their compiled sizes are not original-byte matches. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

`cdLoad__FPcPvii` is already on `origin/agents/cd-archive` and is not defined here. The sif-keyboard queries and module calls are not defined here. `SignalSema` and `WaitSema` are called through the published `norrath_player_signal_semaphore` and `norrath_player_wait_semaphore`. `sceCdSync` is called through `norrath_archive_cd_sync`. `CreateSema`, `DeleteSema`, `DIntr`, `EIntr`, and `scePrintf` are the existing symbols.

## Observed

`sceSifCallRpc` is 0x002d8270, 500 bytes. A missing packet returns -1. Mode bit 2 skips the writeback. Mode bit 1 sends command 0x8000000a and does not wait. The waiting path stores the semaphore, waits, then deletes it. A zero send frees the packet and returns -2. The ninth argument is the end parameter.

`sceSifBindRpc` is 0x002d80a0, 320 bytes. It clears client words 16 and 36 before requesting a packet. Command 0x80000009. On a zero send in the waiting path it frees the packet before deleting the semaphore.

`sceSifInitRpc` is 0x002d7908, 412 bytes. A nonzero `_sceSifInitCheck` returns after restoring interrupts. Otherwise it fills the 64-byte `_data_table` at 0x008e0440 and registers handlers 0x80000008, 09, 0a, and 0c. A zero `sceSifGetReg(0x80000002)` sends 16 bytes from `_packet_buffer+64` and waits until `sceSifGetSreg(0)` is nonzero. Other ELF objects also named `_data_table` and `_packet_buffer` are not these objects.

`sceCdNcmdDiskReady` calls `_sceCd_ncmd_prechk(2)` and RPC function 14. `sceCdStream` calls `_sceCd_ncmd_prechk(15)`, writes a 20-byte command at `_sceCd_ncmdsdata`, and uses RPC function 9. Both load the result word through the uncached alias before signaling `_sceCd_ncmd_semid`.

`_sceCd_ncmd_prechk` and `_sceCd_scmd_prechk` are the 368-byte checks at 0x002e2cd0 and 0x002e2fe8. The S check uses `sceCdSyncS` and server 0x80000593. The N check uses `sceCdSync` and server 0x80000595. `cmd_sem_init` at 0x002e26f0 creates four semaphores when any of the three command ids is -1. The fourth id is `cb_semid`, with maximum count 0.

The extension initializers are the seven functions `initFile` calls. `fontInitFont__FP4Font` relocates three words and calls `textureInitTexture__FP8_texture`. `textAddText__FPv` stores strings in `table` at 0x006d8fa0, 0x2000 words, the span to `unknownBuffer`. `soundAddSound__FPcP10_vagHeaderb` uses `nmSounds`, `effectsBufferSize`, and `soundList` at 0x007d9280. The ELF size of `soundList` is 0; the span to `batchBuffer` is 0x3000. `worldInitWorld__FP12_worldHeader` is 0x001745d8, 1748 bytes. It returns the word at +120 when the word at +112 is nonzero, and 0 otherwise.

The three vblank imports are the words at 0x00484438 (4 bytes), 0x0048443c (1 byte), and 0x00486a60 (4 bytes). They are published only under the archive view names.

## Imports closed

`DelayThread`, `PollSema`, `ReferThreadStatus`, `iSignalSema`, `SCE_CD_debug`, `_sceCd_Read_cur_pos`, `_sceCd_c_cb_sem`, `_sceCd_cd_callback`, `_sceCd_cd_ncmd`, `_sceCd_cd_read_intr`, `_sceCd_cd_scmd`, `_sceCd_ee_read_mode`, `_sceCd_ncmd_prechk`, `_sceCd_ncmd_semid`, `_sceCd_ncmdsdata`, `_sceCd_rd_intr_data`, `_sceCd_scmd_prechk`, `_sceCd_scmd_semid`, `_sceCd_scmdrdata`, `_sf_bind`, `_sf_data`, `animInitAnimation__FP15AnimationHeader`, `modelInitModel__FPv`, `textureInitTexture__FP8_texture`, `fontInitFont__FP4Font`, `worldInitWorld__FP12_worldHeader`, `soundAddSound__FPcP10_vagHeaderb`, `textAddText__FPv`, `cdrdsf`, `cmd_sem_init`, `dum_mode`, `my_th_info`, `my_thid`, `ncmd_sema_keep_cmd`, `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view`, `sceCdNcmdDiskReady`, `sceCdStream`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifCheckStatRpc`, `sceSifInitRpc`, `sceSifWriteBackDCache`, `sfrdata`, `stm_status`.

Also defined, because these functions use them: `_sceCd_ncmdrdata`, `_sceCd_rcmd_semid`, `_ncmd_bind`, `_scmd_bind`, `scmd_sema_keep_cmd`, `cb_thid`, `cb_semid`, `sceCdCbfunc`, `sceCdCbfunc_num`, `sceCdCbfunc_number`, `_sceSifInitCheck`, `_data_table`, `_packet_buffer`, `_free_buffer`, `_free_buffer2`, `table`, `nmSounds`, `effectsBufferSize`, `soundList`, `sceCdSyncS`.

## Imports opened

`_sceRpcGetPacket`, `_sceRpcFreePacket`, `sceSifSendCmd`, `sceSifInitCmd`, `sceSifAddCmdHandler`, `sceSifGetReg`, `sceSifGetSreg`, `sceSifSetReg`, `_request_end`, `_request_bind`, `_request_call`, `_request_rdata`, `TimerUSec2BusClock`, `SetTimerAlarm`, `strlen`, `strcasecmp`, `convertEnd__Fi`, `getStockPitch__FP10_vagHeader`, `transferVAG__FP10_vagHeaderi`, `sprintf`, `cdLoad__FPcPvii`, `lightDeactivate__FP5Light`, `__eq__19ParticleEmitterDataRC19ParticleEmitterData`, `P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `megatileInit__FP12MegatileRootPv`, `megatileGenerate__FP12_worldHeader`, `_engineCheckStack__FPCci`, `earlyInit__3mapP12_worldHeader`, `megatile_ramkmax`, `nmParticleEmitterRecs`, `texmapstart`, `texmapend`, `texmap`, `engineCurrentLevelName`, `DynamicLightArray`, `ParticleEmitterArray`.

`cdLoad__FPcPvii` belongs to the cd-archive package. The megatile, light, particle, and map names belong to those packages. They are left unresolved here.

## Build-plan entries needed

Append these to `experimental/build-plan.json` inputs. Do not add the host contracts. `snapshot.json` must record every new source and host contract before `tools/check_source_snapshot.py` accepts them. This branch does not edit the snapshot or the shared plan. A private copy that appended these entries was rejected with `Input coverage mismatch` for that reason. No receipt was written.

Assign `input_order` from the next free orders on the integration tip, in the order below. This branch's plan already ends at 389, and the shared tip has moved, so do not reuse a guessed range.

```json
[
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-anim-init.o",
    "profile": "authored_cpp",
    "sha256": "6cf85231fad3f9b3e18773ddd14ec56b6c95d4bbfb80a1b43300b7febf0aac1e",
    "source": "sources/sony-cd-anim-init/anim_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-callback.o",
    "profile": "authored_cpp",
    "sha256": "ab3c3fa289624b0187c3a50a607aa7c20af40ed65cf6dde411bcb9802a08467b",
    "source": "sources/sony-cd-callback/callback.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-cmd-sem-init.o",
    "profile": "authored_cpp",
    "sha256": "19cff86bb1ef99946aff7f5479dee350f7a846f7cd7e51d9d5da73b1c53f9204",
    "source": "sources/sony-cd-cmd-sem-init/cmd_sem_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-delay-thread.o",
    "profile": "authored_cpp",
    "sha256": "8df1b54be44203319b9d335a133fb3a549d31710c8508e87a2ab46df0d43f861",
    "source": "sources/sony-cd-delay-thread/delay_thread.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-disk-ready.o",
    "profile": "authored_cpp",
    "sha256": "23959d23a8e8195cec7f22a75b4fa6662cd95fb8fb7de24d99f1f43ea6c12fc7",
    "source": "sources/sony-cd-disk-ready/disk_ready.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-font-init.o",
    "profile": "authored_cpp",
    "sha256": "afab351d5ea2ee839cf3564849f0cb5f88b3c08c6bfdbe354fc00172aaa8c383",
    "source": "sources/sony-cd-font-init/font_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-isignal-sema.o",
    "profile": "authored_entry_asm",
    "sha256": "9b4e2b1f158c6fa23541ebe9819501a1cfa7acdeaef6897de7968818b571cf95",
    "source": "sources/sony-cd-isignal-sema/isignal_sema.S"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-model-init.o",
    "profile": "authored_cpp",
    "sha256": "72ffc461f5008c85192cef5d9f4987dfecf8b8784d1b115d81710e5701157102",
    "source": "sources/sony-cd-model-init/model_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-ncmd-prechk.o",
    "profile": "authored_cpp",
    "sha256": "f171c5452e915b1d1b194d81466180d8e44e6022038c6f33656fd4e495939382",
    "source": "sources/sony-cd-ncmd-prechk/ncmd_prechk.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-poll-sema.o",
    "profile": "authored_entry_asm",
    "sha256": "89dd3caef456cfa130f9f6eb19c83a2a007fe2377e4005ec40c30fc76958f6cd",
    "source": "sources/sony-cd-poll-sema/poll_sema.S"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-read-intr.o",
    "profile": "authored_cpp",
    "sha256": "375e3a18d490572c4a433a48c26ee762abffda5b6211a8047b79f46e3d9c1b81",
    "source": "sources/sony-cd-read-intr/read_intr.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-refer-thread.o",
    "profile": "authored_entry_asm",
    "sha256": "d70e2a0289f0828152c415140cd46aa973932648bf3b9e870c6bd2530ff63854",
    "source": "sources/sony-cd-refer-thread/refer_thread.S"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-scmd-prechk.o",
    "profile": "authored_cpp",
    "sha256": "2e3977400f0292288bed1f9cb0561495e643096e86a80f1282459e425fc90ac0",
    "source": "sources/sony-cd-scmd-prechk/scmd_prechk.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sif-bind.o",
    "profile": "authored_cpp",
    "sha256": "78efaa78ba007d8f433ad97b24d9170e3401e75b43bd463c0c8bb56ded2d886a",
    "source": "sources/sony-cd-sif-bind/bind_rpc.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sif-call.o",
    "profile": "authored_cpp",
    "sha256": "9860d0ab6a0499ea90c63752883c8aca0b93f50b1b2de7f8d3215be4d1002594",
    "source": "sources/sony-cd-sif-call/call_rpc.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sif-check-stat.o",
    "profile": "authored_cpp",
    "sha256": "232ad5d0f5c7e7e5c0f9b14c981e425c11074cb19575dc3de6522393c8a74536",
    "source": "sources/sony-cd-sif-check-stat/check_stat.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sif-init.o",
    "profile": "authored_cpp",
    "sha256": "c2b7bfb447d861fe6040304027ec44f55ea076ffe8bb1d32ad1bbabff7db97d7",
    "source": "sources/sony-cd-sif-init/init_rpc.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sif-writeback.o",
    "profile": "authored_cpp",
    "sha256": "6447db92fda8e72ae72e9f3ccbabb58f02243b59c1ef8d958cdc243507f2b161",
    "source": "sources/sony-cd-sif-writeback/writeback.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sound-add.o",
    "profile": "authored_cpp",
    "sha256": "6cf2c4d779c1bfcc5339f492b384ad2b8ba491b37c4b7fe61fd6a86be6b768c5",
    "source": "sources/sony-cd-sound-add/sound_add.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-stream.o",
    "profile": "authored_cpp",
    "sha256": "c9188e19ea098edfa09d0999200e0ce89376b1ad922b96c6b2210564806cb8dc",
    "source": "sources/sony-cd-stream/stream.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-sync-s.o",
    "profile": "authored_cpp",
    "sha256": "be35df53ca1289dbef8ac6f2c716a3e1a25b972596928d7465a46257740d3f7d",
    "source": "sources/sony-cd-sync-s/sync_s.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-text-add.o",
    "profile": "authored_cpp",
    "sha256": "f12d983c6875ce872e1d0a7ee7bc998b07d500746a2821aca4fbf1d761ba0e92",
    "source": "sources/sony-cd-text-add/text_add.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-texture-init.o",
    "profile": "authored_cpp",
    "sha256": "e79eefdf7ecf61a8a5b902a51c1a5ed3916e8c3426691dee8fc1e95a84c169d2",
    "source": "sources/sony-cd-texture-init/texture_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-vblank-views.o",
    "profile": "authored_cpp",
    "sha256": "2735a9c09cb5afcffd3881673c54a30b2da2862a2278c3da24b7ef2325bc30d7",
    "source": "sources/sony-cd-vblank-views/views.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-words.o",
    "profile": "authored_cpp",
    "sha256": "ad3df13998e90d08c7e7c4b8e56e7887efbf9a641f60af34a3909505ee288664",
    "source": "sources/sony-cd-words/words.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-world-init.o",
    "profile": "authored_cpp",
    "sha256": "54605a2528399cdcc5cceabb080dc243b8a085ac00f9218b15be62dc63be6f4a",
    "source": "sources/sony-cd-world-init/world_init.cpp"
  }
]
```

## Authored but never executed

Every function and data object above. The host contracts ran on this machine, and the three syscall stubs matched the pinned ELF. Nothing was executed on the EE. No CD command, SIF DMA, timer alarm, cache operation, or game path has run. The bind retry spin and the `sceSifGetSreg` wait are in the source and were not taken by the host contracts. The local `DelayThread` handler was called directly by the host contract.

## Unknown

Compiled C bodies are larger than the original functions and were not compared byte for byte. `sceSifWriteBackDCache` does not reproduce the original's unreachable trailing return. Whether the kernel passes `SetTimerAlarm`'s third argument to the handler as its fourth argument was not established; the handler signals the fourth argument it receives. The world walk over the word at header +84, the emitter flags 0x8000, and a `cdLoad` record list other than the -2 terminator are translated from the listing and are not separate host cases. `soundList`'s ELF size is 0. Hardware timing, interrupts, and gameplay remain unverified.
