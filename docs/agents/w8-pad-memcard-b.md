# Pad, multitap and memory-card services (2/2)

Package notes for `agents/w8-pad-memcard-b`. Sources are under `experimental/sources/w8-pad-memcard-b-*/`, one directory per symbol, each with the authored `.cpp` and a `host_contract.cpp`. Per-symbol observations (addresses, sizes, store order) are recorded as Observed/Inferred/Unknown comments at the top of each source file. Addresses come from the private SLUS_205.65 image's symbol table and from reading its code; nothing from the image is committed.

## Checks performed

- Each of the 11 units compiles alone with the reviewed EE toolchain under the unchanged `authored_cpp` flags.
- Each `host_contract.cpp` builds with the host C++ compiler and exits 0 (11 of 11). The `updateRamSlot` contract also passes under AddressSanitizer and UBSan.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- No full partial build was run (no private plan was assembled), and the shared plan and snapshot were not touched.
- Nothing was compared byte for byte against the image, and nothing was executed on the EE. The function host contracts use scripted stand-ins for the libpad, camera and save callees, so they test the authored control flow and the byte layouts written, not the Sony library or the game.

## Functions

- `resetQuickLoad__Fv` at `0x0025bd20`: zeroes the 64 bytes at `quickLoadStamp`.
- `padClear__Fv` at `0x00167660`: clears four 24-byte `rumphole` records (word +12 is left alone), clears two `actSendData` bytes, and sends it to each pad that exists via `scePadSetActDirect`.
- `padBigRumble__Fiff` at `0x001682d0`: arms a record from the first float times 60 and a strength clamped to 0..1.
- `padGetUnprocessedData__FiP18padUnprocessedData` at `0x00167708`: reads one pad, requests mode/alignment/pressure changes when needed, and fills an 8-byte output.
- `padProcessData__FP3padP18padUnprocessedDataP12padStateData` at `0x001678f0`: held/pressed/repeat button words and float flags from that output.
- `updateRamSlot__Fii` at `0x002591d8`: writes the RAM save slot (title lines, camera, jump gates, level seeds, per-player records, script entries). The compiled body is 1,325 bytes against the original 1,824, because the original expands its block copies inline and this source calls `memcpy`.

## Data

`tapCount` (2 bytes, both 1), `padFrame.756` (4 bytes, 0, defined through an asm label so the spelling carries the numeric suffix), `padIndexMap` (4 bytes, 0), `whichPad` (1 byte, 0), `saveConfigEndGame` (1 byte, 0). Initial values are the image bytes; extents are the symbol sizes or the gap to the next symbol.

## Observed and inferred, summarized

Observed: all addresses and sizes above; the `_gp` value `0x004879f0` used to resolve every gp-relative load; that `norrath_restore_broadcast_mask_word` is the word at `0x00483ac0` (`objectIgnoreFlags`), checked from `objectProcessDeleteList__Fv`'s load; `norrath_got_player_start_this_level_bytes` is `gotPlayerStartThisLevel` at `0x00484ca8`; `norrath_keep_player_positions_bytes` is `keepPlayerPositions` at `0x00486cc6`.
Inferred: the meaning of the record fields (held, pressed, repeat words; duration and strength for rumble) and of the player-object word at `player + 21872`.
Unknown: the remaining fields of the 88-byte pad record, who reads the 24-byte rumble records, what the slot-header bytes `+13..+19` hold, and the behaviour of every Sony library call (not executed).

## Imports closed

`padBigRumble__Fiff`, `padClear__Fv`, `padFrame.756`, `padGetUnprocessedData__FiP18padUnprocessedData`, `padIndexMap`, `padProcessData__FP3padP18padUnprocessedDataP12padStateData`, `resetQuickLoad__Fv`, `saveConfigEndGame`, `tapCount`, `updateRamSlot__Fii`, `whichPad`.

## Names in the package list that I did not define, and why

- Already authored by other packages but not yet in `build-plan.json`: `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`, `norrath_libpad_rpc_clients_view`, `norrath_libpad_port_info_view`, `norrath_libpad_dma_info_view`, `norrath_libpad_rpc_buffer_view` and the five `norrath_libmtap_*_client_view` plus `norrath_libmtap_rpc_buffer_view` are all defined in `experimental/sources/simloop-services-pad-mtap-state/pad_mtap_state.cpp`; `norrath_archive_vblank_semaphore_view` is defined in `experimental/sources/sony-cd-vblank-views/views.cpp`. Defining them again would duplicate storage, so this package does not. The integrator needs those two source units in the plan.
- Aliases of storage that another package already defines: `norrath_restore_broadcast_mask_word` (same word as `norrath_grid_ignore_flags_view`, world-state), `norrath_got_player_start_this_level_bytes` (same byte as `norrath_player_view_484ca8`, items-player) and `norrath_keep_player_positions_bytes` (same byte as `keepPlayerPositions`, card-progress). I tried a `.set` alias to an undefined symbol with the reviewed assembler; it emits no symbol, so it cannot close the import. A second definition would give the same game byte two storage locations, which would be wrong. These three stay open until the integrator renames the callers or links with an alias/defsym.

## Imports opened

Names these units reference that are not defined by this package: `quickLoadStamp`, `rumphole`, `actSendData`, `rdata`, `needActAlign`, `actAlignData`, `padVibrationEnable`, `startInhibit`, `padFilter`, `padBackButton`, `padAcceptButton`, `processAnalogInput__FiiPfT2` (a local `t` symbol in the image, so whichever unit defines it must give it external linkage), `scePadSetActDirect`, `scePadRead`, `scePadSetMainMode`, `scePadSetActAlign`, `scePadInfoPressMode`, `scePadEnterPressMode`, `memset`, `memcpy`, `strlen`, `strncmp`, `strcpy`, `gameGetFullLevelName__FPc`, `Save__6CameraPc`, `jumpGateSave__FPc`, `netPlayerIsLocal__Fi`, `convertPointersToIndicies__Fi`, `scriptGameStateEntry__FiPi`, `MC_SaveLevelEntities__FPc`, `gameOver`, `nmPlayers`, `curPlaySeconds`, `gameDifficulty`, `gameCam`, `levelSeeds`, `gameAutoMapMode`, `player`, `tempPlayerStruct`, `validLocalPlayer`, `actCount`, `actLevelCount`, `allActs`, `levelName`, `ramSlot`. Several are defined by other packages; the list overstates what is open once those land.

## Build-plan entries needed

Append to `inputs` in `experimental/build-plan.json` (and the matching `files` hashes and snapshot records), `header_closure` empty, `profile` `authored_cpp`, continuing `input_order` from the integrator's current last order. The hashes are of the sources at this branch's tip.

| object_name | source | sha256 |
|---|---|---|
| reset_quick_load.o | sources/w8-pad-memcard-b-reset-quick-load/reset_quick_load.cpp | 345b1599fb2d6a284345a5d26eed57dcb8faef16788993a82225593179260536 |
| pad_clear.o | sources/w8-pad-memcard-b-pad-clear/pad_clear.cpp | 2fa5722e8a3a0a18d2ddd054c7b2f555a524205d7177cdf7247287aeba3531ba |
| pad_big_rumble.o | sources/w8-pad-memcard-b-pad-big-rumble/pad_big_rumble.cpp | b24d0d584957915e5379dc5691a5c88c409b3866986d4af283c2bf698e0a09ad |
| pad_get_unprocessed.o | sources/w8-pad-memcard-b-pad-get-unprocessed/pad_get_unprocessed.cpp | bd2f4ffdd00703fb8bb202ddf94facae6b3b4ff1d27770e85e9271232bac5a11 |
| pad_process_data.o | sources/w8-pad-memcard-b-pad-process-data/pad_process_data.cpp | bb2a67bf488369b8e85bc805878baf5e30fb2402f3b20ae86ca2905138650fd4 |
| update_ram_slot.o | sources/w8-pad-memcard-b-update-ram-slot/update_ram_slot.cpp | 83d3f6dfd3f221413008ee2453d4dcd748fdcafde3b5d8b54ded85b98e4ec93b |
| tap_count.o | sources/w8-pad-memcard-b-tap-count/tap_count.cpp | f16c9846ee0623218071e98d738d9e952eb160b0fda40732e408b0721620e84e |
| pad_frame.o | sources/w8-pad-memcard-b-pad-frame/pad_frame.cpp | db2d845c49303dcb50ee96b1cb02efdb486a1cc469da622e22ed64202535707d |
| pad_index_map.o | sources/w8-pad-memcard-b-pad-index-map/pad_index_map.cpp | c1764a9120c28221db40f5dd31af378b6f70a42af180154f3309fbf498dd8e49 |
| which_pad.o | sources/w8-pad-memcard-b-which-pad/which_pad.cpp | ca1688d752a45d6239b39ebe724de1981fcd88113b995169a309f8b686237ded |
| save_config_end_game.o | sources/w8-pad-memcard-b-save-config-end-game/save_config_end_game.cpp | bd6679fd5149088fe1294347dddf9dd3593992f256939ac36081bdfd2ec154bf |

Also needed from other packages (not mine): `sources/simloop-services-pad-mtap-state/pad_mtap_state.cpp` and `sources/sony-cd-vblank-views/views.cpp`. Do not add the host contracts. The snapshot needs an `emitted_functions` record per unit (empty for the five data units). `padClear__Fv` is also defined by game-state's callers only as an import, so there is no duplicate.

## Authored but never executed

All eleven units. None ran on the EE or in the game. The host contracts check the authored logic against scripted stand-ins only. In particular `padGetUnprocessedData` and `padProcessData` are untested against real libpad packets, and `updateRamSlot` is untested against a real save-slot reader.

## Unknown

- Whether the original `updateRamSlot` block copies and this source's `memcpy` calls leave identical bytes in the slot (they should; not compared).
- The fields of the 88-byte pad record not named in `padProcessData`'s comment, and the layout of `padUnprocessedData` beyond the eight bytes written.
- The result of `updateRamSlot` when the fallback player's object word is zero (the original reads through it; this source does too).
- Who writes `quickLoadStamp`.
