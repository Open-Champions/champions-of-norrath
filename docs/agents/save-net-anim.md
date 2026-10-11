# save, network, and animation state

Package notes for `agents/save-net-anim`. These objects were compiled with the reviewed `authored_cpp` flags and checked by `host-tests/save-net-anim/`. They are not in `experimental/build-plan.json` or `experimental/snapshot.json`. `tools/check_source_snapshot.py` rejects every unrecorded file under `experimental/sources/` until the integrator adds the file hashes and keeps `input_order` contiguous. The tools unit tests do not run that checker.

`norrath_archive_map_animation_sounds` walks one animation header. A null sound-list word returns before the length call. Sound records are 8 bytes and stop on a null name. The mapped id is stored after the list pointer is reloaded. Event records are 28 bytes and stop on `0x7fffffff`. `*bodyfall*` stores half-index -3 and `*blank*` stores -4. Mode 0 clears event bytes 10 through 14. Mode below 2 rewrites indexes that are below -2 or past the sound count, to 0 when a sound exists and to -1 when it does not, so -3 and -4 remain only when the mode is at least 2. The length result times 60 is truncated toward zero and stored at header offset 64. The host test uses an address arena because this machine cannot map the EE pointer range. The EE object was compiled and its defined and undefined symbols were checked; the routine was not executed by the EE.

## Layout notes

The network mode byte, the HUD no-controller byte, and the memory-card startup-script flag are one-byte zeros. The mode byte is the same address as the still-unresolved `norrath_creature_view_483829`. This package defines `norrath_network_mode_byte_view` and aliases `netNetworkMode` to that same byte. A second definition of the creature name would be a different byte.

String objects include the terminating NUL and stop there. `%s.lmp` is 7 bytes. `start` is 6. `script.scr` is 11. `preStart` is 9. `objects.ob` is 11. `anm` is 4. The padding before the next literal is not part of the symbol.

The atan2 scale word is the four bytes `83 f9 22 46`. The transition threshold is `88 88 88 3c`. The inventory transition rate is `99 99 99 3e`. These are pool slots, not ELF object symbols of their own. The animation math coefficient views are the math package.

The lump-name table is ten 16-byte slots, 160 bytes. The resource-name table is seven 16-byte slots, 112 bytes.

The never-save table is 19 words, 76 bytes: eighteen names and a null word. Seventeen entries are the existing `norrath_factory_text_*` objects at the original addresses. The two clearColor slots are the same symbol, `norrath_factory_text_462058`. The faytree slot is a string in this object because that address has no factory-text import. The filter compares names with `norrath_factory_strcasecmp`, so the table does not need pointer identity with a second copy of the text. Whether a later store replaces a slot is unknown.

Message callbacks are 64 zero bytes, sixteen pointer slots, ending where `pktHandler` begins. A handler index is a byte and is not bounds-checked. An index past 15 is outside this object.

The partner views are separate arrays, not one overlapping object. Each array is the contiguous span that makes a 17632-byte stride land on the next player's slot. Bytes between slots are inside that span, and another view may cover some of the same addresses as its own object. Four records come from the partners symbol running exactly four strides before the next symbol. Local flags and the connected-status byte are each 52897 bytes. Cursors are 52900 bytes. Rings are 61088 bytes because a 13-bit mask reaches 8192 bytes from each player's ring base. Indexes past the fourth player are outside these objects.

Inventory animation state is 1056 bytes, four records of 264, from that `animState` base through the byte before the next symbol. Saved map tiles are 93312 bytes, all initially zero. The original object is file-backed zero storage. Under `-G0` this zero definition is not promised to stay in that original section.

The saved-level index is 648 bytes, 27 records of 24. That is the gap from the index base to the payload base. The loader reads the first byte of record 26. No ELF symbol records a size. The payload is 618800 bytes, from its base through the byte before `slotInfoHeaders`, which is also the save routine's limit of `ramSlot + 0xb29c8`. The load buffer is 60032 bytes, the gap from `loadBuffer` to `tempBuffer`. All three are initially zero. Interior address calculations that looked like extra objects were not separate symbols.

The cosmetic animation table is 144 bytes, eighteen slots. Each slot is a zero word followed by a pointer. The last pointer is null. The other seventeen targets are unresolved object methods. Factory code that exists today only stores the address of the table.

`norrath_construct_Savepoint` is still unresolved. It is the factory constructor used by object creation, so it stays with object-infra. `norrath_network_partner_count_word_view` is already defined by game-network-partner-count. `netFree` stores the original name `nmPartners`, which is that same word, and this package does not define it again.

## Original names

`main` and `launchSRS` import the original names listed under Imports closed. `netNetworkMode` is not a new object. It is an alias of `norrath_network_mode_byte_view`: both symbols are size 1, the same address, and the same section. The image byte is `0x483829`. The other names were not the same bytes as anything this package had already defined, so each one is a new source file under `experimental/sources/save-net-anim-startup/`.

`netInterfaceIsUp` is the zero byte at `0x483828`, immediately before the mode byte. `netInitState` is the zero byte at `0x4838ed`. `MC_ProtectRamSlot` is `0x485be0`. `coreLaunchSRS` is `0x486679`. `fromSRS`, `fromSRSButEnteringGame`, and `coreBeenInSRS` are the three adjacent zero bytes at `0x4867ad` through `0x4867af`, immediately before `srs.bin`. The first two are local in the image. They are defined as globals here because `main` references them from another file. `netGameLaunchInfo` is 328 zero bytes at `0x333130`.

`cutInitNet__Fv` (`0x18e448`, 36 bytes) registers `cutMsgHandlerFunction__FPUci` and stores the id in `cutMsgHandler`. `commInitNet__Fv` (`0x2cd1a8`, 52) registers `commHandler__FPUci` into `commHandle`, then `commStatusHandler__FPUci` into `commStatusHandle`. `DLG_InitNet__Fv` (`0x1d3808`, 36) registers `dialogMsgCallback__FPUci` into `dialogMsgHandle`. `hudInitNet__Fv` (`0x1c77f0`, 52) registers `hudMsgCallback__FPUci` into `hudMsgHandle`, then `netWaitCallBack__FPUci` into `netWaitMsgHandler`. The first store happens before the second call. `textInitNet__Fv` (`0x2cb410`, 36) registers `textHandler__FPUci` into `dataHandle`.

`MC_Configure__Fv` (`0x25ad10`, 40) calls `stopCardQuery__Fv` and returns `newMode__Fii(0, 0)`. `MC_Reset__Fv` (`0x25ac40`, 40) calls `MCA_ClearInfo__Fv`, stores -1 into `MCA_Slot` and `MCA_Port`, and returns -1. `mcProcess__Fv` (`0x2d0ed0`, 36) calls `mcHeartBeat__Fv` and returns 1 when the unsigned `gameBailFromGameLoop` word is nonzero.

`MC_Init__Fi` (`0x2502c8`, 1004) stores its argument in `disableMC` before `gameSetDefaultGameConfig__Fi(-1)`, then reloads that byte. A nonzero `sceMcInit` result stores 1 into `disableMC` and returns 1. A zero result, or a nonzero argument that skips `sceMcInit`, takes the long path and returns -1. That path calls `MC_NewGame__Fi(1)`, sizes each level as `(size + _3map$autoMapBitsSize + 1000)` aligned down to a multiple of 128, keeps the running best across acts, then replaces `slotFootPrint` with `0xb29c8` after `bareSlotFootPrint` has taken the computed sum plus 648. The icon header then receives the observed `PS2D` bytes, halfword 26, the six pool floats plus 1.0, 0.5, and -0.5, the SJIS title, and three `icon.ico` copies. `MCA_Present` and `MCA_Formatted` are nonzero addresses, so the debug command always runs.

`sceMcInit` (`0x2e0b90`, 460) creates two semaphores only when `semaidRegFunc` is negative. The first parameter words are 1, 1, and 0 at offsets 4, 8, and 20. The second are 1, 0, and 0. It then calls `sceMcSync(0, 0, 0)`, `WaitSema` on the first id, and `sceSifInitRpc(0)`. The countdown is `0x100000`. `sceSifBindRpc` uses server `0x80000400`. A negative bind prints `bind error libmc \n` and does not return. A zero word at `mcClientID+36` repeats the countdown and the bind. Command 254 is sent with 48 bytes from `sifParamOrd` and 12 bytes into `retval`. A negative RPC result clears that client word and returns the result minus 100. A service word below 522 returns -120. A manager word below 526 returns -121. Otherwise the first `retval` word is returned.

`netFree__Fb` (`0x130de0`, 912) returns without stores while `netInitState` is zero. Otherwise it clears that byte, closes a non-negative `broadcastConnection` with timeout 1000, and stores -1 there. A nonzero argument calls `sceInetCtlDownInterface` and clears `netInterfaceIsUp`. It always terminates `cd`, clears `netNetworkMode`, sets `netNmPartners` to 1, blanks 70528 partner bytes, and writes 128.0f at record offsets 24, 40, 56, 72, 104, 120, 136, and 152 of four stride-17632 records. It then stores 0 through `nmPartners`.


## Imports closed

`norrath_archive_map_animation_sounds`, `norrath_network_mode_byte_view`, `netNetworkMode`, `norrath_game_hud_no_controller_byte_view`, `norrath_mc_disable_startup_script_bytes`, `norrath_mc_lump_format_bytes`, `norrath_mc_start_key_bytes`, `norrath_mc_script_section_key_bytes`, `norrath_mc_pre_start_key_bytes`, `norrath_mc_objects_section_key_bytes`, `norrath_archive_animation_extension_view`, `norrath_animation_dependency_view_47ff1c`, `norrath_animation_transition_threshold_view`, `norrath_inventory_animation_transition_rate_view`, `norrath_inventory_animation_lump_names_view`, `norrath_inventory_animation_resource_names_view`, `norrath_network_message_callbacks_view`, `norrath_network_player_local_flags_view`, `norrath_animation_dependency_view_4e6755`, `norrath_network_send_cursor_words_view`, `norrath_network_send_ring_bytes_view`, `norrath_inventory_animation_state_base_view`, `norrath_saved_map_tiles_bytes`, `norrath_mc_saved_level_index_bytes`, `norrath_mc_saved_level_payload_bytes`, `norrath_mc_load_buffer_bytes`, `norrath_game_never_save_table_view`, `norrath_factory_animation_vtable`.

`commInitNet__Fv`, `coreBeenInSRS`, `coreLaunchSRS`, `cutInitNet__Fv`, `DLG_InitNet__Fv`, `fromSRS`, `fromSRSButEnteringGame`, `hudInitNet__Fv`, `MC_Configure__Fv`, `MC_Init__Fi`, `MC_ProtectRamSlot`, `MC_Reset__Fv`, `mcProcess__Fv`, `netFree__Fb`, `netGameLaunchInfo`, `netInitState`, `netInterfaceIsUp`, `sceMcInit`, `textInitNet__Fv`.

## Imports opened

The sound mapper references `norrath_string_compare`, which `sources/lump-find-build/string_compare.cpp` already defines, plus `soundMapSound__FPci` and `soundComputeSequenceLength__FP15AnimationHeader`, which stay unresolved for audio.

The never-save table references these existing factory imports: `norrath_factory_text_483b38`, `norrath_factory_text_483b40`, `norrath_factory_text_483b48`, `norrath_factory_text_461fc0`, `norrath_factory_text_483b50`, `norrath_factory_text_461fd0`, `norrath_factory_text_461fe0`, `norrath_factory_text_483b60`, `norrath_factory_text_483b68`, `norrath_factory_text_461ff0`, `norrath_factory_text_462000`, `norrath_factory_text_462010`, `norrath_factory_text_462028`, `norrath_factory_text_462040`, `norrath_factory_text_462058`, `norrath_factory_text_462068`.

The animation table adds these unresolved method symbols: `__tf16CosmeticPropAnim`, `_$_16CosmeticPropAnim`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__16CosmeticPropAnim`, `msg_draw__16CosmeticPropAnim`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__16CosmeticPropAnimPc`, `msg_save__16CosmeticPropAnimPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__16CosmeticPropAnim`, `getModel__10GameObject`.

Registrars: `netRegisterMessageHandler__FPFPUci_v`, `netRegisterPacketHandler__FPFPUci_v`, `cutMsgHandlerFunction__FPUci`, `cutMsgHandler`, `commHandler__FPUci`, `commStatusHandler__FPUci`, `commHandle`, `commStatusHandle`, `dialogMsgCallback__FPUci`, `dialogMsgHandle`, `hudMsgCallback__FPUci`, `netWaitCallBack__FPUci`, `hudMsgHandle`, `netWaitMsgHandler`, `textHandler__FPUci`, `dataHandle`.

Memory card: `stopCardQuery__Fv`, `newMode__Fii`, `MCA_ClearInfo__Fv`, `MCA_Slot`, `MCA_Port`, `mcHeartBeat__Fv`, `gameBailFromGameLoop`, `MCA_Init__Fv`, `gameSetDefaultGameConfig__Fi`, `MC_NewGame__Fi`, `sprintf`, `strcpy`, `memset`, `AsciiString2Sjis__FPUcPUs`, `engineAddTask__FPFv_viPci`, `MCA_Cmd__Fiiii`, `MCA_Sync__FPi`, `mcHeartBeatRun__Fv`, `disableMC`, `flareAlpha`, `frontEndWasRunning`, `curAct`, `asyncMode`, `oldAsyncMode`, `configState`, `lastCardPort`, `lastCardSlot`, `pauseStackPos`, `keepPlayerPositions`, `MC_SavingOrLoading`, `browserState`, `MC_LastModeSuccessful`, `saveCheckPointCount`, `flareDelta`, `killPlayer`, `MC_KickProtect`, `MC_CheckPointAge`, `slotFootPrint`, `actCount`, `actLevelCount`, `allActs`, `_3map$autoMapBitsSize`, `bareSlotFootPrint`, `totalClusters`, `errorCondition`, `slotClusters`, `totalFootPrint`, `iconHeader`, `sortedTimes`, `MC_Debug`, `MCA_LastDirCount`.

`sceMcInit` references `CreateSema`, `sceMcSync`, `WaitSema`, `sceSifInitRpc`, `sceSifBindRpc`, `scePrintf`, `sceSifCallRpc`, `SignalSema`, `semaidRegFunc`, `semaid_timerfunc`, `mcClientID`, `sifParamOrd`, `retval`.

`netFree` references `broadcastConnection`, `cd`, `g_net_buf`, `sceInetClose`, `sceInetCtlDownInterface`, `sceLibnetTerminate`, `netNmPartners`, `partners`, and `nmPartners`. `inventoryInitNet__Fv` and `shopInitNet__Fv` stay with items-player.

## Build-plan entries needed

Append these to `experimental/build-plan.json`. Orders 346 through 391 follow this worktree's current maximum of 345. Renumber them if another package takes those orders first. `snapshot.json` needs the same sources, the header, and a contiguous input list or `check_source_snapshot.py` fails.

```json
[
  {
    "header_closure": [
      "sources/save-net-anim-map-sounds/map_sounds.hpp"
    ],
    "input_order": 346,
    "kind": "independently_authored_source",
    "object_name": "map_sounds.o",
    "profile": "authored_cpp",
    "sha256": "d766e2f9bf1401ecc6173b5f3d7e66caac0cf88ee5d7368c2698e7dffed3b907",
    "source": "sources/save-net-anim-map-sounds/map_sounds.cpp"
  },
  {
    "header_closure": [],
    "input_order": 347,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_dependency_view_47ff1c.o",
    "profile": "authored_cpp",
    "sha256": "fb51ec35f1aee30590e49dfaded7ce009603e9b2ea6062855f3aede3ee1e7ba4",
    "source": "sources/save-net-anim-views/norrath_animation_dependency_view_47ff1c.cpp"
  },
  {
    "header_closure": [],
    "input_order": 348,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_dependency_view_4e6755.o",
    "profile": "authored_cpp",
    "sha256": "e4766f33ab0a1e538ce2bf8cd1789b1dea37048162258163df6ca7c70a63e068",
    "source": "sources/save-net-anim-views/norrath_animation_dependency_view_4e6755.cpp"
  },
  {
    "header_closure": [],
    "input_order": 349,
    "kind": "independently_authored_source",
    "object_name": "norrath_animation_transition_threshold_view.o",
    "profile": "authored_cpp",
    "sha256": "ae3c530a7510083304db10104dfa65d1d1f3da65f550ec9a6a3cc17762c97974",
    "source": "sources/save-net-anim-views/norrath_animation_transition_threshold_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 350,
    "kind": "independently_authored_source",
    "object_name": "norrath_archive_animation_extension_view.o",
    "profile": "authored_cpp",
    "sha256": "9846ebcf223ec7fe81671d64ee245cdfadaba5f3d1bfd77e69b8a13049a2006e",
    "source": "sources/save-net-anim-views/norrath_archive_animation_extension_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 351,
    "kind": "independently_authored_source",
    "object_name": "norrath_factory_animation_vtable.o",
    "profile": "authored_cpp",
    "sha256": "09471cd3f13abe4903de0b294e219ccb7c43fe89cf15e16eba4b60f11299814b",
    "source": "sources/save-net-anim-views/norrath_factory_animation_vtable.cpp"
  },
  {
    "header_closure": [],
    "input_order": 352,
    "kind": "independently_authored_source",
    "object_name": "norrath_game_hud_no_controller_byte_view.o",
    "profile": "authored_cpp",
    "sha256": "74f55f74946f74e8289fb9d78630e9f692b9fadc640cf39a89fd51bc609942d9",
    "source": "sources/save-net-anim-views/norrath_game_hud_no_controller_byte_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 353,
    "kind": "independently_authored_source",
    "object_name": "norrath_game_never_save_table_view.o",
    "profile": "authored_cpp",
    "sha256": "4fc926f1081fa12ebbe94ec78d18804973fa0cac301bcc25f630a29331646cca",
    "source": "sources/save-net-anim-views/norrath_game_never_save_table_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 354,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_lump_names_view.o",
    "profile": "authored_cpp",
    "sha256": "7b4d5a9c26f8d37f77e64a9bfa34be2a274dd6f7219f555b0a8d89a2d054260d",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_lump_names_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 355,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_resource_names_view.o",
    "profile": "authored_cpp",
    "sha256": "5c584f22809f4e06621023b6921ded82afa830f4df93a74f9939331fcd0b5b07",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_resource_names_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 356,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_state_base_view.o",
    "profile": "authored_cpp",
    "sha256": "c7eea382cb7ada206bd7c42e52fc70d1226cfd30ba9a01aa4937eb6ec5174a69",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_state_base_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 357,
    "kind": "independently_authored_source",
    "object_name": "norrath_inventory_animation_transition_rate_view.o",
    "profile": "authored_cpp",
    "sha256": "7c30f5131916e78969e9e11c3236a3d52e9b0ee42fe5738d0551fd7c70a908e0",
    "source": "sources/save-net-anim-views/norrath_inventory_animation_transition_rate_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 358,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_disable_startup_script_bytes.o",
    "profile": "authored_cpp",
    "sha256": "084bb3d5da86807db14f8790ce6c07817abc57038ba377e7f28e356ce71f3ab1",
    "source": "sources/save-net-anim-views/norrath_mc_disable_startup_script_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 359,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_load_buffer_bytes.o",
    "profile": "authored_cpp",
    "sha256": "e8e9249d39c4daeabb65d952ea03b89fb97c9d13f956c46876a8e4a4ec22156c",
    "source": "sources/save-net-anim-views/norrath_mc_load_buffer_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 360,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_lump_format_bytes.o",
    "profile": "authored_cpp",
    "sha256": "3184b41051fcf9fe7a152f80083bc45b6e2d8e42ca4ad75e89cd80731c4d385e",
    "source": "sources/save-net-anim-views/norrath_mc_lump_format_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 361,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_objects_section_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "86ba14a734ab660e9d8d15f0c0be755793dfba37f41a04940c9c598a78131c6d",
    "source": "sources/save-net-anim-views/norrath_mc_objects_section_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 362,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_pre_start_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "3de5197c45b266ad2cda194224297dea5ee57a52f45b9f9bbbd69825dc7af632",
    "source": "sources/save-net-anim-views/norrath_mc_pre_start_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 363,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_saved_level_index_bytes.o",
    "profile": "authored_cpp",
    "sha256": "887132f1508265499d1f5068eb27df5b1445c42cfd1ca8e9cdf2a67c611892e9",
    "source": "sources/save-net-anim-views/norrath_mc_saved_level_index_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 364,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_saved_level_payload_bytes.o",
    "profile": "authored_cpp",
    "sha256": "5afecb9c48d71a26db59177b7e32e7be22e445d3206857b71d65bc111e978816",
    "source": "sources/save-net-anim-views/norrath_mc_saved_level_payload_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 365,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_script_section_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "b977c790dfff058f3d0dd8e6fe080a69b294a78e6b849cbe022fa5a7f85ac33f",
    "source": "sources/save-net-anim-views/norrath_mc_script_section_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 366,
    "kind": "independently_authored_source",
    "object_name": "norrath_mc_start_key_bytes.o",
    "profile": "authored_cpp",
    "sha256": "e90b6b0422e61655552c71addab8f15bdf6690ccc65e2015c757b2c5b663bad1",
    "source": "sources/save-net-anim-views/norrath_mc_start_key_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 367,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_message_callbacks_view.o",
    "profile": "authored_cpp",
    "sha256": "ca276e8f9dcdade5231be42d44b7fbefe96db338f3867fe8f9c9f48794368529",
    "source": "sources/save-net-anim-views/norrath_network_message_callbacks_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 368,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_mode_byte_view.o",
    "profile": "authored_cpp",
    "sha256": "05dab96dc055d99e805a7f67ea0e3e042a2162dfcbbfc16a8cf82a9ebc6ddd62",
    "source": "sources/save-net-anim-views/norrath_network_mode_byte_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 369,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_player_local_flags_view.o",
    "profile": "authored_cpp",
    "sha256": "b9918825c8bd0fd70302d9f182f25f4e979f59b07d4bee6e737ea913f55e7862",
    "source": "sources/save-net-anim-views/norrath_network_player_local_flags_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 370,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_send_cursor_words_view.o",
    "profile": "authored_cpp",
    "sha256": "fcfd75408266ea715505f5894172bac0c9e34644e9fc96484e6dc19762fa0b4a",
    "source": "sources/save-net-anim-views/norrath_network_send_cursor_words_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 371,
    "kind": "independently_authored_source",
    "object_name": "norrath_network_send_ring_bytes_view.o",
    "profile": "authored_cpp",
    "sha256": "d28fa422afe7d50327f4790cfc743de815701b2e8aa78a273b4cc99dca157195",
    "source": "sources/save-net-anim-views/norrath_network_send_ring_bytes_view.cpp"
  },
  {
    "header_closure": [],
    "input_order": 372,
    "kind": "independently_authored_source",
    "object_name": "norrath_saved_map_tiles_bytes.o",
    "profile": "authored_cpp",
    "sha256": "0109d172252921f11bf417b5e87e41ec7a1a1fb42775b670de87981610161f0e",
    "source": "sources/save-net-anim-views/norrath_saved_map_tiles_bytes.cpp"
  },
  {
    "header_closure": [],
    "input_order": 373,
    "kind": "independently_authored_source",
    "object_name": "comm_init_net.o",
    "profile": "authored_cpp",
    "sha256": "7b7a854471737f6077d582ead8e2b5823462d0fb178727f28489605d98aee701",
    "source": "sources/save-net-anim-startup/comm_init_net.cpp"
  },
  {
    "header_closure": [],
    "input_order": 374,
    "kind": "independently_authored_source",
    "object_name": "core_been_in_srs.o",
    "profile": "authored_cpp",
    "sha256": "fdf79529f1c28d4e676ce6005611e51a98f4fd7fdcbc92024a2d49fd3e476b48",
    "source": "sources/save-net-anim-startup/core_been_in_srs.cpp"
  },
  {
    "header_closure": [],
    "input_order": 375,
    "kind": "independently_authored_source",
    "object_name": "core_launch_srs.o",
    "profile": "authored_cpp",
    "sha256": "560afa28fd8bc560f451571b5ee3b82ca84013bc2f02bfc671b4e7f1b1859ecf",
    "source": "sources/save-net-anim-startup/core_launch_srs.cpp"
  },
  {
    "header_closure": [],
    "input_order": 376,
    "kind": "independently_authored_source",
    "object_name": "cut_init_net.o",
    "profile": "authored_cpp",
    "sha256": "c2f9e9d4604522b02f307d5268f295810ababf951b287eb74d0f4d5204b4a864",
    "source": "sources/save-net-anim-startup/cut_init_net.cpp"
  },
  {
    "header_closure": [],
    "input_order": 377,
    "kind": "independently_authored_source",
    "object_name": "dlg_init_net.o",
    "profile": "authored_cpp",
    "sha256": "9b4eaa36ba5ccf4cbd274ac33fed138f32b1430e392a33e839e86e0ac59c5991",
    "source": "sources/save-net-anim-startup/dlg_init_net.cpp"
  },
  {
    "header_closure": [],
    "input_order": 378,
    "kind": "independently_authored_source",
    "object_name": "from_srs.o",
    "profile": "authored_cpp",
    "sha256": "b3235d22d9139a83ab0b45154c0017b934eb979a013e2e869779e39bcbf497a9",
    "source": "sources/save-net-anim-startup/from_srs.cpp"
  },
  {
    "header_closure": [],
    "input_order": 379,
    "kind": "independently_authored_source",
    "object_name": "from_srs_but_entering_game.o",
    "profile": "authored_cpp",
    "sha256": "1b2a9559447879c8714ec2508dbd54e1458a556526560f6026331849507fa2f6",
    "source": "sources/save-net-anim-startup/from_srs_but_entering_game.cpp"
  },
  {
    "header_closure": [],
    "input_order": 380,
    "kind": "independently_authored_source",
    "object_name": "hud_init_net.o",
    "profile": "authored_cpp",
    "sha256": "f85c6c8ae3b68db4464d203d578d8f7d938109d0afb63d135d8de4ccd763d9bd",
    "source": "sources/save-net-anim-startup/hud_init_net.cpp"
  },
  {
    "header_closure": [],
    "input_order": 381,
    "kind": "independently_authored_source",
    "object_name": "mc_configure.o",
    "profile": "authored_cpp",
    "sha256": "a484c0b4aa40b4a3203f339b5bdff82232cfe855261d8b050f66da0275ac70e9",
    "source": "sources/save-net-anim-startup/mc_configure.cpp"
  },
  {
    "header_closure": [],
    "input_order": 382,
    "kind": "independently_authored_source",
    "object_name": "mc_init.o",
    "profile": "authored_cpp",
    "sha256": "2d2462c633da604a596e3711efce69ecd37ee9eefe1505240c925fd3dee0ed90",
    "source": "sources/save-net-anim-startup/mc_init.cpp"
  },
  {
    "header_closure": [],
    "input_order": 383,
    "kind": "independently_authored_source",
    "object_name": "mc_process.o",
    "profile": "authored_cpp",
    "sha256": "c402d619a0e8c5826beeaa562354ae083dae28180ebdddc6ee0c9c149ae8461e",
    "source": "sources/save-net-anim-startup/mc_process.cpp"
  },
  {
    "header_closure": [],
    "input_order": 384,
    "kind": "independently_authored_source",
    "object_name": "mc_protect_ram_slot.o",
    "profile": "authored_cpp",
    "sha256": "7a9e193dbd47c02833c15c94c3d9bcc634fbfef1cc4053193be60c07b0b98631",
    "source": "sources/save-net-anim-startup/mc_protect_ram_slot.cpp"
  },
  {
    "header_closure": [],
    "input_order": 385,
    "kind": "independently_authored_source",
    "object_name": "mc_reset.o",
    "profile": "authored_cpp",
    "sha256": "254a20914ead29d38df2c8c01f11d4340b61d3a5d5c4bc8988a4c3071fb7d9cc",
    "source": "sources/save-net-anim-startup/mc_reset.cpp"
  },
  {
    "header_closure": [],
    "input_order": 386,
    "kind": "independently_authored_source",
    "object_name": "net_free.o",
    "profile": "authored_cpp",
    "sha256": "dd546c285c770f4ae2447333ab43b93689b2ceed9ba7585954722950c9bcf835",
    "source": "sources/save-net-anim-startup/net_free.cpp"
  },
  {
    "header_closure": [],
    "input_order": 387,
    "kind": "independently_authored_source",
    "object_name": "net_game_launch_info.o",
    "profile": "authored_cpp",
    "sha256": "95b5c6ec383e960284ee8469affba996bc7c6f32a857fd793ed679d7fe066e58",
    "source": "sources/save-net-anim-startup/net_game_launch_info.cpp"
  },
  {
    "header_closure": [],
    "input_order": 388,
    "kind": "independently_authored_source",
    "object_name": "net_init_state.o",
    "profile": "authored_cpp",
    "sha256": "e54747f76d2f5ee788533b486ca7a74a5033a185897cb57da692b324d4c6e9cb",
    "source": "sources/save-net-anim-startup/net_init_state.cpp"
  },
  {
    "header_closure": [],
    "input_order": 389,
    "kind": "independently_authored_source",
    "object_name": "net_interface_is_up.o",
    "profile": "authored_cpp",
    "sha256": "1c319edf17b392e6a01dace391d1def032ddc868fdeca66fe9dc70fd40046b0d",
    "source": "sources/save-net-anim-startup/net_interface_is_up.cpp"
  },
  {
    "header_closure": [],
    "input_order": 390,
    "kind": "independently_authored_source",
    "object_name": "sce_mc_init.o",
    "profile": "authored_cpp",
    "sha256": "e68230438247235f76efee87da9ccefd15d50e6046c0d6df7f469145afb69722",
    "source": "sources/save-net-anim-startup/sce_mc_init.cpp"
  },
  {
    "header_closure": [],
    "input_order": 391,
    "kind": "independently_authored_source",
    "object_name": "text_init_net.o",
    "profile": "authored_cpp",
    "sha256": "54dbaede67f438a08f285738b293b54bf89cc4c9cd6baaab1af8a7b132c73150",
    "source": "sources/save-net-anim-startup/text_init_net.cpp"
  }
]
```

## Authored but never executed

`norrath_archive_map_animation_sounds` and the startup functions run in the host tests. The EE compile checks each defined symbol and its unresolved imports. None of them is linked into the partial image, and none was executed by the EE. The views and the vtable are data. Nothing in this branch calls the vtable slots. The `sceMcInit` bind-failure loop is not taken by the host test.

## Unknown

Which startup store first writes the zero views. Whether a later store replaces a never-save pointer. The original section of each `-G0` zero object. The bytes between partner slots, which sit inside these spans and also inside neighboring views that this package does not alias. Handler indexes past 15, and player indexes past 3. The animation math views, `norrath_construct_Savepoint`, and `norrath_creature_view_483829`. The semaphore-parameter bytes other than the three words stored by `sceMcInit` are not written in the image; this reconstruction clears them. The bodies of the registered handlers are not in this package.
