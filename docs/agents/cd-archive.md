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

## Imports closed

`norrath_archive_engine_push_border`, `norrath_archive_engine_pop_border`, `norrath_archive_engine_sync_vblank`, `norrath_archive_last_character`, `norrath_archive_cd_sync`, `norrath_archive_cd_error`, `norrath_archive_cd_read`, `norrath_archive_cd_read_iop`, `norrath_archive_cd_stream_pause`, `norrath_archive_cd_stream_read`, `norrath_archive_cd_layer_search`, `norrath_archive_initialize_file`, `norrath_archive_animation_extension_view`, `norrath_archive_check_file_view`, `norrath_archive_command_view`, `norrath_archive_completion_pointer_view`, `norrath_archive_directory_a_view`, `norrath_archive_directory_b_view`, `norrath_archive_directory_name_view`, `norrath_archive_directory_pointer_view`, `norrath_archive_directory_switch_view`, `norrath_archive_layer_bias_view`, `norrath_archive_load_failure_format_view`, `norrath_archive_lookup_format_view`, `norrath_archive_no_new_vags_view`, `norrath_archive_permanent_halt_flag_view`, `norrath_archive_prefix_five_view`, `norrath_archive_prefix_four_view`, `norrath_archive_read_active_view`, `norrath_archive_read_mode_view`, `norrath_archive_stream_active_view`, `norrath_archive_stream_destination_view`, `norrath_archive_stream_remaining_view`, `norrath_archive_stream_restart_view`, `norrath_archive_zero_record_format_view`, `norrath_archive_cd_command_count_view`.

## Imports opened

Left unresolved on purpose. Vblank words are the scheduler's (`vblankCount`, `vblankField`, `vblankSemaphore2`). The extension inits belong to their own packages. `norrath_player_wait_semaphore`, `norrath_player_signal_semaphore`, and `norrath_string_compare` already have authored definitions. `sceCdStream` is the unread body under pause and stream read.

`norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view`, `animInitAnimation__FP15AnimationHeader`, `modelInitModel__FPv`, `textureInitTexture__FP8_texture`, `fontInitFont__FP4Font`, `worldInitWorld__FP12_worldHeader`, `soundAddSound__FPcP10_vagHeaderb`, `textAddText__FPv`, `norrath_sce_printf`, `sceCdStream`, `sceCdNcmdDiskReady`, `_sceCd_ncmd_prechk`, `_sceCd_scmd_prechk`, `cmd_sem_init`, `sceSifInitRpc`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifCheckStatRpc`, `sceSifWriteBackDCache`, `_sceCd_cd_read_intr`, `_sceCd_cd_callback`, `DelayThread`, `PollSema`, `ReferThreadStatus`, `SCE_CD_debug`, `_sceCd_c_cb_sem`, `_sceCd_ee_read_mode`, `_sceCd_cd_ncmd`, `_sceCd_cd_scmd`, `_sceCd_ncmd_semid`, `_sceCd_scmd_semid`, `_sceCd_ncmdsdata`, `_sceCd_scmdrdata`, `_sceCd_rd_intr_data`, `_sceCd_Read_cur_pos`, `stm_status`, `dum_mode`, `ncmd_sema_keep_cmd`, `_sf_bind`, `_sf_data`, `sfrdata`, `cdrdsf`, `my_thid`, `my_th_info`.

Still open from the loader, and not part of this package: `norrath_archive_map_animation_sounds`, `norrath_archive_sound_free_ram`.

## Build-plan entries needed

Append these to `experimental/build-plan.json`. Do not add the host contracts. `header_closure` is empty and `profile` is `authored_cpp`. Suggested `input_order` continues from 297.

```json
{"header_closure":[],"input_order":298,"kind":"independently_authored_source","object_name":"push_border.o","profile":"authored_cpp","sha256":"99acb0d995a077701076b9be82752d53992ae07247ecb7c2ae0534ad42adddbc","source":"sources/cd-archive-engine-push-border/push_border.cpp"}
{"header_closure":[],"input_order":299,"kind":"independently_authored_source","object_name":"pop_border.o","profile":"authored_cpp","sha256":"87f792d65f5ee685174fcb6a9f5c76669f90cf4aa13d5c2d0d364895311cd0b5","source":"sources/cd-archive-engine-pop-border/pop_border.cpp"}
{"header_closure":[],"input_order":300,"kind":"independently_authored_source","object_name":"sync_vblank.o","profile":"authored_cpp","sha256":"d19be1827dc4766828cc2f11f5fea7bf211902fb0a103631492edecd5e7c4847","source":"sources/cd-archive-engine-sync-vblank/sync_vblank.cpp"}
{"header_closure":[],"input_order":301,"kind":"independently_authored_source","object_name":"last_character.o","profile":"authored_cpp","sha256":"c492e58d834bbe286530b268a1fde01ba0c6a9ff3f822f635a87c3efe10cedb4","source":"sources/cd-archive-last-character/last_character.cpp"}
{"header_closure":[],"input_order":302,"kind":"independently_authored_source","object_name":"cd_sync.o","profile":"authored_cpp","sha256":"a31efbf3fc295278eac763c3d1e325a0f7a9d86acd098e35631d2151bebcf461","source":"sources/cd-archive-cd-sync/cd_sync.cpp"}
{"header_closure":[],"input_order":303,"kind":"independently_authored_source","object_name":"cd_error.o","profile":"authored_cpp","sha256":"ab69929ea1bd158e0f809df3f1a62da7c64d37b649570bca24c4a7bc8f07bbfa","source":"sources/cd-archive-cd-error/cd_error.cpp"}
{"header_closure":[],"input_order":304,"kind":"independently_authored_source","object_name":"cd_read.o","profile":"authored_cpp","sha256":"3d93d71856070c752421d212d1c31b12b83e7c0ebcdc609e9e398d5ad2e29ddb","source":"sources/cd-archive-cd-read/cd_read.cpp"}
{"header_closure":[],"input_order":305,"kind":"independently_authored_source","object_name":"cd_read_iop.o","profile":"authored_cpp","sha256":"921145b15cb13ef54b012c6dcc6d78ab05ef57f050105159d7a124afde6623ad","source":"sources/cd-archive-cd-read-iop/cd_read_iop.cpp"}
{"header_closure":[],"input_order":306,"kind":"independently_authored_source","object_name":"stream_pause.o","profile":"authored_cpp","sha256":"7fb8fb925e6c226ce2c8609a3cafe88367a16278cbf5b330f3bbe9c88d44f31f","source":"sources/cd-archive-cd-stream-pause/stream_pause.cpp"}
{"header_closure":[],"input_order":307,"kind":"independently_authored_source","object_name":"stream_read.o","profile":"authored_cpp","sha256":"bae947f305238ecb2793fdb3140d83e5d48b6733bf9655e5306a52b517928eff","source":"sources/cd-archive-cd-stream-read/stream_read.cpp"}
{"header_closure":[],"input_order":308,"kind":"independently_authored_source","object_name":"layer_search.o","profile":"authored_cpp","sha256":"e0f6e6b18dfc16ad6ab0f860ffd7f72eddfe67c9ef528ddfd0135694bb1cf146","source":"sources/cd-archive-cd-layer-search/layer_search.cpp"}
{"header_closure":[],"input_order":309,"kind":"independently_authored_source","object_name":"initialize_file.o","profile":"authored_cpp","sha256":"6e43324b2fffce1224a740c9b91e92ee1b675329eba1cbe6b33d5d454a4c327c","source":"sources/cd-archive-initialize-file/initialize_file.cpp"}
{"header_closure":[],"input_order":310,"kind":"independently_authored_source","object_name":"views.o","profile":"authored_cpp","sha256":"f6d12b11b095fab59c774dd9269090c89af94b6924dea771e93ff9cdc952d099","source":"sources/cd-archive-views/views.cpp"}
```

## Authored but never executed

The functions were compiled for the EE and exercised only by host contracts with stubbed callees. They were not linked into a partial image, not run on the EE, and not compared byte for byte with the original functions. The bind-retry spin and an interrupt landing between the two vblank-count loads were not executed. `sceCdStream` has no body here, so the pause and stream-read calls stop at that import.

## Unknown

`sceCdStream` at `0x002e4458` (360 bytes) is not reconstructed. The Sony precheck, semaphore, thread-status, and SIF RPC helpers are unresolved, so their failure behavior inside those callees is not re-derived here. `norrath_string_compare` is the published `strcmp` stand-in; this package only depends on a zero result for an equal extension and a nonzero result otherwise. The retail disc revision is still unknown. `norrath_archive_map_animation_sounds` and `norrath_archive_sound_free_ram` were left for the audio package.
