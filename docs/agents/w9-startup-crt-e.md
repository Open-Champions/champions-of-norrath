# w9-startup-crt-e: startup and runtime support (e)

All sources are independently authored C++ under `experimental/sources/w9-startup-crt-e-*`. Each
function unit has a `host_contract.cpp` that fails on a wrong result. Nothing was run on an EE or
in-game. Addresses refer to the private reference executable.

## Observed

- Function extents and the storage addresses/sizes in each unit header were read from the private
  executable's symbol table and image bytes. The soft-double names are the four original entry points
  at 0x002fb5c0, 0x002fb680, 0x002fba48 and 0x002fbc20.
- `padFilter` 0x00484074 (4 bytes, image 0), `reglistLoopCount` 0x00486994 (4 bytes), pad `rdata`
  0x006e1cc0 (128 bytes), `sceDmaCurrentEnv` 0x008dd830 (80 bytes), the ten-pointer table at 0x004293e8,
  and the word at 0x004852d8 inside the `.sdata` literal "Color".
- The ten strings of the 0x004293e8 table are `RaceBarbarianMale/Female`, `RaceWoodElfMale/Female`,
  `RaceHighElfMale/Female`, `RaceEruditeMale/Female`, `RaceDarkElfMale/Female`. The host contract in
  `w8-startup-sce-d-set-srs-info` stubs the table with other text ("Elf", "Dwarf", ...); that stub does
  not match the image and is not evidence.

## Inferred

- The soft-double entry points forward to the independently authored bodies under the
  `norrath_player_double_*` / `norrath_factory_double_compare` names, because the original bodies are
  the same routines. They are pass-throughs here, not a second copy.
- The race table is indexed by a race/gender selector (from the strings and the caller only).
- Names ending `_view` or carrying an address suffix are mine, because the originals are file-local or
  carry a dot suffix.

## Unknown

- Meaning of the pad `rdata` bytes and of the `sceDmaCurrentEnv` layout beyond what `sceDmaPutEnv` copies.
- What the word at 0x004852d8 was meant to hold (see `w8-startup-sce-d.md`); defined as 0x6f6c6f43, the
  first four bytes of the literal, with no ownership of the literal.
- Whether callers ever pass soft-double operands in other registers.
- Contents of the file-system receive buffers and of the Cd state objects.

## Imports closed (by this package's sources, once in the plan)

Functions: `sceCdDiskReady`, `sceCdPowerOff`, `sceCdRead`, `sceCdReadIOPm`, `sceCdStPause`,
`sceCdStStop`, `sceDevctl`, `sceFsInit`, `sceGifPkEnd`, `sceGifPkRefLoadImage`, `sceMcChdir`,
`sceMcClose`, `sceMcDelete`, `sceMcFormat`, `sceMcGetDir`, `sceMcMkdir`, `raise`, `printfloat`,
`norrath_strticmp`, `norrath_softdouble_add_2fb5c0`, `norrath_softdouble_compare_2fba48`,
`norrath_softdouble_multiply_2fb680`, `norrath_softdouble_to_float_2fbc20`.

Data: `padFilter`, `reglistLoopCount`, `rdata`, `sceDmaCurrentEnv`,
`norrath_sce_d_race_strings_view_4293e8`, `norrath_sce_d_unknown_sdata_word_view_4852d8`.

## Imports not closed here, and why

- `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`, `norrath_libpad_port_info_view`,
  `norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view`: already defined in
  `simloop-services-pad-mtap-state/pad_mtap_state.cpp`, which is not yet in the plan. A second definition
  would duplicate storage.
- `pad_dma_buf`: defined in `simloop-services-pad-dma-buffer/pad_dma_buffer.cpp`, not yet in the plan.
- `norrath_loader_failure_word_view`: defined by `level-anim-load-loader-failure-word`.
- `norrath_restore_broadcast_mask_word`: same word as `norrath_grid_ignore_flags_view` (0x00483ac0,
  `objectIgnoreFlags`). A second definition would give one game word two locations and an assembler alias
  to an undefined symbol emits nothing. Needs an integrator decision.
- `osdsrc` (0x00444a38, 1960 bytes): original kernel machine code copied to 0x80074000. Republishing its
  bytes would republish original code. Needs an owner decision (private extraction at link time, or a
  reconstruction written from understanding).

## Imports opened

None new from the data unit. Function units open only the Sony/SIF/libc calls they name
(for example `sceSifCallRpc`, `sceSifBindRpc`, `_raise_r`, `_impure_ptr`, `DPRINT`-style debug prints and the
`norrath_player_double_*` bodies); these belong to other packages.

## Build-plan entries needed

Profile `authored_cpp`, empty `header_closure`, source paths relative to `experimental/`. Do not add any
`host_contract.cpp`. Body sizes are from my private `-O2` trial compile, not from the plan's profile flags;
re-measure with the plan build.

| source | sha256 | functions (bytes) | data |
|---|---|---|---|
| `sources/w9-startup-crt-e-cd-disk-ready/disk_ready.cpp` | `b0ab9798683c459a913a11c7b9b1bd0fa955fb0698270d5f44f45f6f22518873` | sceCdDiskReady=688 | - |
| `sources/w9-startup-crt-e-cd-disk-ready-state/disk_ready_state.cpp` | `b5dc7c01084c611751efa51b1f80de38c004fa55c01bda421580efad8f11e0a7` | - | _dr_mode, _sceCd_rcmdrdata, cddr |
| `sources/w9-startup-crt-e-cd-power-off/power_off.cpp` | `84d86de6aab160b0c79f2bd4766da0eef414ab6f11d22706b5c01a64f3ef01fe` | sceCdPowerOff=212 | - |
| `sources/w9-startup-crt-e-cd-read/cd_read.cpp` | `e079b14838b45498aacfa4478af933b031a44a41e0d6e61a3fb0085f1254675f` | sceCdRead=716 | - |
| `sources/w9-startup-crt-e-cd-read-iopm/cd_read_iopm.cpp` | `7088c1eefada1c5fb26e92e6edd34938d255cf0514dbd103fd2ab0a68c6dd728` | sceCdReadIOPm=508 | - |
| `sources/w9-startup-crt-e-cd-st-pause/st_pause.cpp` | `5d564dea9b3d1d6788d012e796cd2a30a03094552aa9703049be0833d5512617` | sceCdStPause=104 | - |
| `sources/w9-startup-crt-e-cd-st-stop/st_stop.cpp` | `5f6668dad6dc64234015b653e678173a80859b2384a40c87c93036e683aa9b45` | sceCdStStop=36 | - |
| `sources/w9-startup-crt-e-data-views/data_views.cpp` | `f85676b499e30a6d6c2eeaa4e11cd64f7a103215ce0581dea61cdfc45990e7de` | - | norrath_sce_d_race_strings_view_4293e8, norrath_sce_d_unknown_sdata_word_view_4852d8, padFilter, rdata, reglistLoopCount, sceDmaCurrentEnv |
| `sources/w9-startup-crt-e-devctl/devctl.cpp` | `751cc6425310e257a057a8d7900ebf04d494ca7c26fbca3c9b7a50748236a8a1` | sceDevctl=680 | - |
| `sources/w9-startup-crt-e-fs-init/fs_init.cpp` | `d11ca962cc3fb5299d3a7d9fb853d7460a9ecd3f48b982333e1399b428b522b5` | sceFsInit=568 | - |
| `sources/w9-startup-crt-e-fs-init-state/fs_init_state.cpp` | `e47fd1343b34b0fb22d0a8340e161fe95a1e94a0c27964c5fb7f0ffeb8423d30` | - | _fs_rcv_bufdbl, _rcv_data_cmd, _sif_FsPoff_Data, _sif_FsRcv_Data, norrath_fs_rcv_adr_view |
| `sources/w9-startup-crt-e-gif-pk-end/pk_end.cpp` | `795ee99af37dec576974fce37e8b12692d613d9a17a70460c8b781288f9c1888` | sceGifPkEnd=96 | - |
| `sources/w9-startup-crt-e-gif-pk-ref-load-image/pk_ref_load_image.cpp` | `7583155ecb450c6cb617191fac5fa7ec3ce10e463511f40afbe5042f0d0f510a` | sceGifPkRefLoadImage=480 | _ZZ20sceGifPkRefLoadImageE3tag |
| `sources/w9-startup-crt-e-mc-chdir/mc_chdir.cpp` | `4fe2c769859431a14e40ab2429669497407c1c5882ef488d1a3fc217074103d8` | sceMcChdir=392 | - |
| `sources/w9-startup-crt-e-mc-close/mc_close.cpp` | `8b4f3e7d9f33887b81d94bdc16a440dbd46d21173761229150af88e38262f252` | sceMcClose=224 | - |
| `sources/w9-startup-crt-e-mc-delete/mc_delete.cpp` | `35686ab41f6df4808d3e6d340ef76199bda5581fe07589f0e97bf2be22e36ba9` | sceMcDelete=328 | - |
| `sources/w9-startup-crt-e-mc-format/mc_format.cpp` | `ef12bc01e8ec4f9d49f5a048667c48c24a70ce71da2ede679f922193069d0fb3` | sceMcFormat=240 | - |
| `sources/w9-startup-crt-e-mc-get-dir/mc_get_dir.cpp` | `e3c42eb4e4cc89826a46d5d3c553d9b5c3306218c62d586442304890830f5080` | sceMcGetDir=424 | - |
| `sources/w9-startup-crt-e-mc-mkdir/mc_mkdir.cpp` | `7b08b41a08adfd25592979714f6e3d6ea1aa51ee98f536d4c9e5da778d727417` | sceMcMkdir=48 | - |
| `sources/w9-startup-crt-e-printfloat/printfloat.cpp` | `cfa17b1012187eb3abaed862f2bab5d9f485d903414c909a06d107ba4f3de6a8` | printfloat=516 | - |
| `sources/w9-startup-crt-e-raise/raise.cpp` | `b0816a17d1f506cdc28b9ed4c5a75eec8231882a376953063b1e10e745162b21` | raise=16 | - |
| `sources/w9-startup-crt-e-softdouble-names/softdouble_names.cpp` | `a01c29e00b104a75b802ae8749b42d3b7cca0bb6452a918dc383e2287dde53eb` | norrath_softdouble_add_2fb5c0=8, norrath_softdouble_compare_2fba48=8, norrath_softdouble_multiply_2fb680=8, norrath_softdouble_to_float_2fbc20=8 | - |
| `sources/w9-startup-crt-e-strticmp/strticmp.cpp` | `d5a4e6d2a98aa70209d5a2d6927bea32629a03e1a9e0663853f95f1c1ae5ec51` | norrath_strticmp=104 | - |

## Authored but never executed

Everything above. The host contracts run the authored C++ against stub callees on the build machine only.
No unit has run on an EE, in an emulator or in the game, and none has been compared against the original
instructions in a paired run.
