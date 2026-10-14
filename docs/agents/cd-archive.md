# CD archive and file system

Package work is on `agents/cd-archive`. Sources are under `experimental/sources/cd-archive-*`. Each function was compiled with the reviewed EE g++ `authored_cpp` flags and checked by the host contract beside it. `host_contract.cpp` is a host test only. It is not a build-plan input.

The loader in `experimental/sources/game-archive-services-build/archive_services.cpp` is the caller. These functions are the CD and file-init callees it was waiting on. `norrath_archive_map_animation_sounds` and `norrath_archive_sound_free_ram` stay unresolved; they are the audio side of the lump walk.

Observed originals, GP `0x004879f0`:

| Authored function | Original | Address | Size |
| --- | --- | --- | --- |
| `norrath_archive_engine_push_border` | `enginePushBorderColor__FUi` | `0x00191ba8` | 8 |
| `norrath_archive_engine_pop_border` | `enginePopBorderColor__Fv` | `0x00191bb0` | 8 |
| `norrath_archive_engine_sync_vblank` | `engineSyncVBlank__Fv` | `0x00191180` | 72 |
| `norrath_archive_last_character` | `strrchr` | `0x003052d8` | 80 |
| `norrath_archive_cd_sync` | `sceCdSync` | `0x002e2ed8` | 160 |
| `norrath_archive_cd_error` | `sceCdGetError` | `0x002e3c20` | 152 |
| `norrath_archive_cd_read` | `sceCdRead` | `0x002e38e8` | 480 |
| `norrath_archive_cd_read_iop` | `sceCdReadIOPm` | `0x002e3ac8` | 340 |
| `norrath_archive_cd_stream_pause` | `sceCdStPause` | `0x002e4368` | 80 |
| `norrath_archive_cd_stream_read` | `sceCdStRead` | `0x002e41e0` | 392 |
| `norrath_archive_cd_layer_search` | `sceCdLayerSearchFile` | `0x002e2998` | 788 |
| `norrath_archive_initialize_file` | `initFile__FPcPvi` | `0x001641a0` | 408 |

Push and pop compile to `jr ra; nop`. The color argument is unused. Sync-vblank returns the sign-extended byte at `vblankField` (`0x0048443c`). The loader prototype is `void`; the return is still produced. The two `vblankCount` loads (`0x00484438`) skip the wait when they differ. The wait uses `vblankSemaphore2` (`0x00486a60`) through the existing `norrath_player_wait_semaphore`.

`norrath_archive_last_character` is the signed `strrchr` used by init. A zero search byte returns the terminator. `initFile` compares the text after the last `.` with the eight 8-byte slots at `0x00483f98` (`anm`, `vif`, `tex`, `fnt`, `world`, `vag`, `uni`, `etex`). `world` subtracts a nonzero result from the size with unsigned wrap. `vag` passes `noNewVags` (`0x00483f90`) and returns `size - 64`. The compare is `norrath_string_compare`, the published stand-in for `strcmp` at `0x003044fc`. The extension inits keep their original mangled names.

CD sync mode 0 prints `N cmd wait\n` once when `SCE_CD_debug` (`0x00446090`) is positive, then `DelayThread(4000)` until `_sceCd_c_cb_sem` (`0x004460b4`) and `sceSifCheckStatRpc` on `_sceCd_cd_ncmd` (`0x00447250`) are both clear. Any other mode returns 1 when either is busy. Layer search calls this authored sync.

CD error uses `_sceCd_scmd_prechk(3)`, then RPC function 4 on `_sceCd_cd_scmd` (`0x00447c00`) with a 4-byte receive at `_sceCd_scmdrdata` (`0x00447280`). The result word is loaded through the uncached alias `0x20000000` before `SignalSema` on `_sceCd_scmd_semid` (`0x004460ac`).

CD read and the IOP read share the disk-ready gate: bit 0 of `_sceCd_ee_read_mode` (`0x004460b8`) skips `sceCdNcmdDiskReady`, and ready result 6 returns 0. The 24-byte packet is at `_sceCd_ncmdsdata` (`0x00446180`). Mode byte 2 selects 2048, 2328, or 2340 bytes per sector for the EE read only. The EE read writebacks the destination unless read-mode bit 1 is set, and it clears the first word of `_sceCd_Read_cur_pos` (`0x00447240`). RPC function 1 ends at `_sceCd_cd_read_intr`. The IOP read is prechk 5, RPC function 13, ends at `_sceCd_cd_callback`, and writebacks only the 24-byte packet. Its end parameter is `c_cnt.66` (`0x004460d4`, 4 file-backed zero bytes). That symbol is not a C identifier; the authored name is `norrath_archive_cd_command_count_view`.

Stream pause stores 0 to `stm_status` (`0x00447c34`) before the optional print, then calls `sceCdStream(0, 0, 0, 7, dum_mode)`. Stream read returns 0 without writing `*error` when `stm_status` is 0. Mode 0 returns one transfer, low half as the count and high half through `*error`. A nonzero mode repeats until the block count is met, or a call reports an error and transfers nothing. The entry printf format names a mode argument, but only the block count is passed.

Layer search requires `PollSema(_sceCd_ncmd_semid)` to return that same id. A 256-byte name forces a NUL at `_sf_data + 291`. RPC function 0 sends 300 bytes and receives 4 at `sfrdata`. Success copies 36 bytes back through the uncached alias. `_sf_bind` (`0x004460c4`) is file-initialized to -1. The bind server is `0x80000597`. A failed bind spins a volatile countdown from `0x00100000` to `0xffffffff` and retries.

Data views are the file bytes the loader names. Directory A is `layer0Dir` at `0x006cc6a0` and directory B is `layer1Dir` at `0x006d26a8`: 768 rows of 32 bytes plus the count word, 24580 bytes each, both zero in the file image. `currentLayerDir` at `0x00483ff0` points at directory A. On EE that pointer is 4 bytes with an `R_MIPS_32` relocation to directory A. `command` at `0x006d86b0` is 16 bytes. The extension slots are followed by `lump.cc`, `ntsc_`, and `pal_`. The lookup format is `%s;1`. The directory name is `\BG\DATA`. The load-failure format is `'%s' not found\n`. The zero-record format at `0x004633c0` is `!!!!!!!!!! %s is fucked\n`. Zero-initialized objects land in `.bss` under this profile. The values match the file image. The integrator's link assigns addresses.

`tools/check_source_snapshot.py` will reject these new source paths until `experimental/snapshot.json` records them. That snapshot was not edited.

`main` and `launchSRS` import five loader functions under their original names. None of them is an already authored body. `norrath_cd_load` stores its failure through a named word; `cdLoad__FPcPvii` at `0x00165120` (276 bytes) stores 2 at address 0. `lumpLoad__FPc` is `0x00165b30` (652 bytes), `lumpFindResource__FPcT0` is `0x00165dc0` (396 bytes), `lumpLock__Fi` is `0x001662f0` (88 bytes), and `lumpClear__Fi` is `0x001663f0` (188 bytes). The three engine hooks and `cdBusy`, `command`, `cdResult`, and `noNewVags` are the same objects already in this package, so those translation units also export the original names. `lumps`, `nmLumps`, and `resourceFrame` are the object-infra views `norrath_constructor_lump_cache_view`, `norrath_constructor_lump_count_view`, and `norrath_constructor_resource_frame_view`. A `.set` in another object does not publish those names, so they are not declared again here.

`cdLoad` waits, looks the name up, and on a miss prints `'%s' not found\n` then stores 2 at address 0. A hit uses the file size when the requested length is 0, adds the arithmetic sector shift of the byte offset, and fills `command`. `issueCommand` runs with mode 1 already stored, `cdBusy` becomes 1, and both paths hold border color `0x00ffffff` while `cdProcess` and `engineSyncVBlank` run.

`lumpLoad` returns a 40-byte cache record whose flag bit is clear and whose pointer is nonzero. Otherwise `vamplord.lmp` sets `noNewVags` unless `engineCurrentLevelName` is `gothic2`, and `superorc.lmp` sets it unless that name is `ruinint1`. The allocation is `(size + 2047)` masked to 2048, plus 256, labeled with the next record. `cdLoad` is called with a zero length and offset. A miss prints `!!!!!!lump not found\n` and stores 0 at address 0. Both paths scan the cache again. `lumpFindResource` returns `data + 256` when the flag bit is set and that text matches. A clear bit keeps the pointer and stops the scan. The search uses 12-byte records and returns the word at `record + 4`. `lumpLock` raises a signed-smaller lock at offset 32, then always calls `soundLock`. `lumpClear` clears `discardable[0]` before `soundClear`, unloads trailing records whose lock is signed-less-or-equal to the argument, and increments `resourceFrame` on every path.

`engineInit__Ff` imports `cdInit__Fv` at `0x00164e58` (164 bytes). It is not one of the bodies above. It stores `0xC8` in `CDmode` byte 0 and clears bytes 1 and 2 before `sceCdReadDvdDualInfo`. Byte 1 is cleared again when the `isDual` word is nonzero. `sceCdSearchFile` looks up `\BG\DATA`. A hit calls `loadDirectory` on directory A with the file's first word and `(size + 2047) >> 11`. `sceSifAllocIopHeap(0x14800)` is aligned up to 2048 bytes and passed to `sceCdStInit(40, 3, aligned)`. `isDual`, `dualLayer2Start`, and `CDmode` are the existing four-byte views at `0x00483ff4`, `0x00483ff8`, and `0x00486920`.

## Imports closed

`norrath_archive_engine_push_border`, `norrath_archive_engine_pop_border`, `norrath_archive_engine_sync_vblank`, `norrath_archive_last_character`, `norrath_archive_cd_sync`, `norrath_archive_cd_error`, `norrath_archive_cd_read`, `norrath_archive_cd_read_iop`, `norrath_archive_cd_stream_pause`, `norrath_archive_cd_stream_read`, `norrath_archive_cd_layer_search`, `norrath_archive_initialize_file`, `norrath_archive_animation_extension_view`, `norrath_archive_check_file_view`, `norrath_archive_command_view`, `norrath_archive_completion_pointer_view`, `norrath_archive_directory_a_view`, `norrath_archive_directory_b_view`, `norrath_archive_directory_name_view`, `norrath_archive_directory_pointer_view`, `norrath_archive_directory_switch_view`, `norrath_archive_layer_bias_view`, `norrath_archive_load_failure_format_view`, `norrath_archive_lookup_format_view`, `norrath_archive_no_new_vags_view`, `norrath_archive_permanent_halt_flag_view`, `norrath_archive_prefix_five_view`, `norrath_archive_prefix_four_view`, `norrath_archive_read_active_view`, `norrath_archive_read_mode_view`, `norrath_archive_stream_active_view`, `norrath_archive_stream_destination_view`, `norrath_archive_stream_remaining_view`, `norrath_archive_stream_restart_view`, `norrath_archive_zero_record_format_view`, `norrath_archive_cd_command_count_view`. Also the original names of those same objects: `enginePushBorderColor__FUi`, `enginePopBorderColor__Fv`, `engineSyncVBlank__Fv`, `cdBusy`, `command`, `cdResult`, `noNewVags`, `isDual`, `dualLayer2Start`, `CDmode`. And `cdInit__Fv`. And the loader functions and words reconstructed for `main` and `launchSRS`: `cdLoad__FPcPvii`, `lumpLoad__FPc`, `lumpFindResource__FPcT0`, `lumpLock__Fi`, `lumpClear__Fi`, `lumpLoadLump`, `lumpLoadSize`, `lumpLoadCompletionFlag`, `lumpLoadName`, `discardable`.

## Imports opened

Left unresolved on purpose. Vblank words are the scheduler's (`vblankCount`, `vblankField`, `vblankSemaphore2`). The extension inits belong to their own packages. `norrath_player_wait_semaphore`, `norrath_player_signal_semaphore`, and `norrath_string_compare` already have authored definitions. `sceCdStream` is the unread body under pause and stream read.

`norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view`, `animInitAnimation__FP15AnimationHeader`, `modelInitModel__FPv`, `textureInitTexture__FP8_texture`, `fontInitFont__FP4Font`, `worldInitWorld__FP12_worldHeader`, `soundAddSound__FPcP10_vagHeaderb`, `textAddText__FPv`, `norrath_sce_printf`, `sceCdStream`, `sceCdNcmdDiskReady`, `_sceCd_ncmd_prechk`, `_sceCd_scmd_prechk`, `cmd_sem_init`, `sceSifInitRpc`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifCheckStatRpc`, `sceSifWriteBackDCache`, `_sceCd_cd_read_intr`, `_sceCd_cd_callback`, `DelayThread`, `PollSema`, `ReferThreadStatus`, `SCE_CD_debug`, `_sceCd_c_cb_sem`, `_sceCd_ee_read_mode`, `_sceCd_cd_ncmd`, `_sceCd_cd_scmd`, `_sceCd_ncmd_semid`, `_sceCd_scmd_semid`, `_sceCd_ncmdsdata`, `_sceCd_scmdrdata`, `_sceCd_rd_intr_data`, `_sceCd_Read_cur_pos`, `stm_status`, `dum_mode`, `ncmd_sema_keep_cmd`, `_sf_bind`, `_sf_data`, `sfrdata`, `cdrdsf`, `my_thid`, `my_th_info`.

Still open from the loader, and not part of this package: `norrath_archive_map_animation_sounds`, `norrath_archive_sound_free_ram`.

Opened by the original-named loader functions: `cdWait__Fv`, `cdDirectoryLookup__FPcPiT1i`, `issueCommand__Fv`, `cdProcess__Fv`, `lumpUnload__FPv`, `soundLock__Fi`, `soundClear__Fi`, `memAlloc__FiPc`, `strcmp`, `strcpy`, `scePrintf`, `engineCurrentLevelName`. `cdInit` also leaves `sceCdReadDvdDualInfo`, `sceCdSearchFile`, `loadDirectory__FP9directoryii`, `sceSifAllocIopHeap`, and `sceCdStInit` unresolved. `lumps`, `nmLumps`, and `resourceFrame` stay the object-infra views named above.

## Build-plan entries needed

The merged plan already contains the first thirteen sources at orders 358 through 370. Replace the hashes of the four files that have since gained original-name aliases. Orders 358, 359, 360, and 370 keep their object names. Append the remaining sources at 423. Do not add the host contracts. `header_closure` is empty and `profile` is `authored_cpp`. `lump_load.o` and `lump_find_resource.o` are already used by older drafts, so the new objects use a `cd_archive_` prefix.

```json
{"header_closure":[],"input_order":358,"kind":"independently_authored_source","object_name":"push_border.o","profile":"authored_cpp","sha256":"ee21dba1b2cb7a01568a812cf92a839bb041d3c9d9c45e1286c8701b0e8d4250","source":"sources/cd-archive-engine-push-border/push_border.cpp"}
{"header_closure":[],"input_order":359,"kind":"independently_authored_source","object_name":"pop_border.o","profile":"authored_cpp","sha256":"e24d39ab05e9117b631e6380fd174a6f3ccb5af02bd48aa58fa05864c8766d4f","source":"sources/cd-archive-engine-pop-border/pop_border.cpp"}
{"header_closure":[],"input_order":360,"kind":"independently_authored_source","object_name":"sync_vblank.o","profile":"authored_cpp","sha256":"e5dd4270ab8378d4ca05e59cd2c3ca466f0f433ac9588fc69697cfdbf943a802","source":"sources/cd-archive-engine-sync-vblank/sync_vblank.cpp"}
{"header_closure":[],"input_order":370,"kind":"independently_authored_source","object_name":"views.o","profile":"authored_cpp","sha256":"7f0d88f9d9e74c570acad9e7bff8160aafc88ab00f4459316e907fbc24dc0a5d","source":"sources/cd-archive-views/views.cpp"}
{"header_closure":[],"input_order":423,"kind":"independently_authored_source","object_name":"cd_archive_lump_state.o","profile":"authored_cpp","sha256":"8589286206bb2d9b457ee6fea49da133c00922f88a3a4f58170bbf1ad7fd50b3","source":"sources/cd-archive-lump-state/lump_state.cpp"}
{"header_closure":[],"input_order":424,"kind":"independently_authored_source","object_name":"cd_archive_cd_load.o","profile":"authored_cpp","sha256":"f5a574ff616d2f62b77264241baf54a970c13c6f4c4ad7836aa973db2e3fe512","source":"sources/cd-archive-cd-load/cd_load.cpp"}
{"header_closure":[],"input_order":425,"kind":"independently_authored_source","object_name":"cd_archive_lump_load.o","profile":"authored_cpp","sha256":"229b7b45fe1227f8e9ad19ac7897061513aa623d9f82670c9510663199765542","source":"sources/cd-archive-lump-load/lump_load.cpp"}
{"header_closure":[],"input_order":426,"kind":"independently_authored_source","object_name":"cd_archive_lump_find_resource.o","profile":"authored_cpp","sha256":"986371d17ea310fa063fbc329a1309fd480aba93c08e2346755855283cfde8a7","source":"sources/cd-archive-lump-find-resource/lump_find_resource.cpp"}
{"header_closure":[],"input_order":427,"kind":"independently_authored_source","object_name":"cd_archive_lump_lock.o","profile":"authored_cpp","sha256":"155cf72c0377200930ea6fbb1483a320e67176834f3f06e651cef02778bf0eef","source":"sources/cd-archive-lump-lock/lump_lock.cpp"}
{"header_closure":[],"input_order":428,"kind":"independently_authored_source","object_name":"cd_archive_lump_clear.o","profile":"authored_cpp","sha256":"ef56c4bb8d75ebd5c41c558313ec0cc40a2b1203991d325d4ae2d723353c2f7b","source":"sources/cd-archive-lump-clear/lump_clear.cpp"}
{"header_closure":[],"input_order":429,"kind":"independently_authored_source","object_name":"cd_archive_cd_init.o","profile":"authored_cpp","sha256":"536d2bbe76656a934c6525fdcedd6addd8a2bd337408d09d98266c3a7e5a1d6b","source":"sources/cd-archive-cd-init/cd_init.cpp"}
```

## Authored but never executed

The functions were compiled for the EE and exercised only by host contracts with stubbed callees. They were not linked into a partial image, not run on the EE, and not compared byte for byte with the original functions. The bind-retry spin and an interrupt landing between the two vblank-count loads were not executed. `sceCdStream` has no body here, so the pause and stream-read calls stop at that import. `cdLoad`, `lumpLoad`, `lumpFindResource`, `lumpLock`, `lumpClear`, and `cdInit` were likewise compiled and checked only against host stubs.

## Unknown

`sceCdStream` at `0x002e4458` (360 bytes) is not reconstructed. The Sony precheck, semaphore, thread-status, and SIF RPC helpers are unresolved, so their failure behavior inside those callees is not re-derived here. `norrath_string_compare` is the published `strcmp` stand-in; this package only depends on a zero result for an equal extension and a nonzero result otherwise. The retail disc revision is still unknown. `norrath_archive_map_animation_sounds` and `norrath_archive_sound_free_ram` were left for the audio package. `cdWait`, `cdDirectoryLookup`, `issueCommand`, `cdProcess`, `lumpUnload`, and `loadDirectory` are not reconstructed. `lumpClear`'s unload loop ends only when `lumpUnload` changes `nmLumps` or the last lock. `sceCdReadDvdDualInfo`, `sceCdSearchFile`, `sceSifAllocIopHeap`, and `sceCdStInit` are unresolved Sony calls inside `cdInit`.
