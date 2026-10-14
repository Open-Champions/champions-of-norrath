# dialog-hud-controls

Package work is on `agents/dialog-hud-controls`. New source is under `experimental/sources/dialog-hud-controls-*`. Host checks sit beside that source and are not EE build inputs. Each implementation file compiled with the reviewed EE g++ `authored_cpp` profile. Host checks were compiled with `-O2 -fsanitize=address,undefined` and returned 0. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests. Fresh EE compilation is not an original-byte match.

The shared tip `70bd0fb` linked 622 authored inputs, 529 function bodies, 781 unresolved imports, and 0 COMMON. `DLG_Init__Fv` is still unresolved there, so the 105 names opened by game-boot are not in that list yet. `MC_Debug` is already unresolved. None of the names this package defines were referenced by that link. They close the game-boot imports when those callers are linked with these units.

`nmPlayers` is already defined by items-player. It is not defined again. The 95,936 zero bytes at `player` (`0x00408600`) are already published as `norrath_player_progression_records_view`. This package does not create a second copy. The ELF name `player` still needs an alias if a caller imports that name.

## Evidence

Addresses are from the pinned `SLUS_205.65` symbol table. Observed, inferred, and unknown are separated in the source comments.

Call counts below are `jal` sites in the pinned image. `drawDialog__Fv` and `runDialog__Fv` are registered by address, so they have no `jal`.

- `netPlayerIsLocal__Fi` at `0x001353a8` (44 bytes, 93 calls) returns 1 when the signed index is not below `nmPartners`. Otherwise it returns the sign-extended byte at `partners + 0x44d4 + index * 17632`.
- `netGetLocalPlayerNm__Fi` at `0x00135438` (108 bytes, 27 calls) loads `nmPartners` on both sides of the `netNetworkMode` test. Mode 0 returns the argument. A nonpositive count returns -1. Otherwise it returns the partner index of the requested nonzero local-flag byte, or -1.
- `DLG_Active__Fv` at `0x001d38b0` (12 bytes, 26 calls) returns 1 when the `curDlg` word is nonzero. The compare is unsigned.
- `soundFadeStream__Fi` at `0x0024a5d0` (144 bytes, 23 calls) uses `stream` at `0x007dd290`. `gCd` begins 1360 bytes later. The ELF size of `stream` is 0. Count 8 or more at `index * 452 + 0x1c0` returns. A smaller count `n` clears and stores at `n * 48 + index * 452`, including the word 2 at offset 108, then increments the counter.
- `msgHudSetPlayerBusy__Fcc` at `0x001c7890` (108 bytes, 14 calls) sign-extends both low bytes. Mode 0 returns. Any other mode prints `MSG SEND:  Set player %d to busy=%d\n` (two spaces) and sends the three bytes `2`, player, busy on `hudMsgHandle`.
- `menuSetPos__FP4Menui` at `0x001940e0` (108 bytes, 7 calls) walks 16-byte records to the first zero word. An empty menu stores -1 at byte 4. Otherwise the position is clamped to `0 .. last` and stored as `position | (position << 8)` at byte 4 of the terminating record.
- `soundUpdateMusicVolume__Fv` at `0x0024ee18` (68 bytes, 4 calls) returns unless stream 0 status has bit `0x20`. It then multiplies `musicDuckAmount` by `soundMusicVolume` and by the float bits `0x3ef00000`, and calls `soundSetStreamVolume` for stream 0.
- `helpMessageInit__Fv` at `0x001c7828` (48 bytes, 1 call) registers `helpMessageFrame__Fv` at priority 50 with the bytes `HelpMessage` and flag 1.
- `worldSetLightHaloTexture__FP8_texture` at `0x0017dd68` (8 bytes, 1 call) stores the argument in `lightHaloTexture`. `menuPlayerNm` begins four bytes later.
- `specialSoundHandler__FiG6Point3` at `0x001d0d80` (72 bytes, address taken) copies 12 bytes from the second argument. Code -2 passes that copy to `gameFootstepSound__FG6Point3iii` with three zero integers.

Button words, font characters, the 16-byte button records, HUD and comm flags, `boxTex`, `DLG_ShowDialogText`, `curDlg`, the pause-menu words, the sound volume words, and the 80-byte `superMainMenu` and `raceMainMenu` objects are the observed initial bytes. Zero objects were placed in BSS by this compiler. Nonzero words were placed in `.data`. `superMainMenu` word `0x004841cc` is the address of `cheatInvulnerability`. The other nonzero menu words are addresses of text with no ELF symbol. Those arrays keep the observed words. They are not relocated pointers.

`curDlg` is also published by world-state as `norrath_dialog_active_pointer_word_view`. This package defines the ELF name. Those two objects are the same original four bytes and should not both remain separate storage.

## Imports closed

These names are defined here. They were not unresolved on tip `70bd0fb` because game-boot is not in that plan.

`DLG_Active__Fv`, `DLG_ShowDialogText`, `actionButton`, `attackButton`, `automapButton`, `blockButton`, `boxTex`, `chatOn`, `commEcho`, `commEnable`, `commPlayback`, `commRecord`, `commThresh`, `controlStyle`, `curDlg`, `damageDisplayOn`, `debugPauseMenuOn`, `dropButton`, `dropButtonFontChar`, `energyPotionButton`, `equipButton`, `equipButtonFontChar`, `g_BloodLevel`, `healthPotionButton`, `helpMessageInit__Fv`, `hudNetWaiting`, `infoButton`, `infoButtonFontChar`, `menu`, `menuNm`, `menuPlayerNm`, `menuSetPos__FP4Menui`, `msgHudSetPlayerBusy__Fcc`, `netGetLocalPlayerNm__Fi`, `netPlayerIsLocal__Fi`, `padAcceptButton`, `padAcceptButtonFontChar`, `padBackButton`, `padBackButtonFontChar`, `padVibrationEnable`, `particlesPaused`, `raceMainMenu`, `recallOn`, `soundFadeStream__Fi`, `soundMusicVolume`, `soundSfxVolume`, `soundStereoEnable`, `soundUpdateMusicVolume__Fv`, `specialSoundHandler__FiG6Point3`, `spellButton1`, `spellButton2`, `startMenuState`, `subtitlesOn`, `superMainMenu`, `supersampleMode`, `useRaceMenu`, `waitNetExitCount`, `weaponToggleButton`, `worldSetLightHaloTexture__FP8_texture`.

## Imports you opened

Left unresolved on purpose: `cdProcess__Fv`, `MC_Busy__Fv`, `MC_Debug`, `MC_GameConfig`, `animSetSpecialSoundHandler__FPFiG6Point3_v`.

Referenced by these units and not defined here: `engineAddTask__FPFv_viPci`, `gameFootstepSound__FG6Point3iii`, `helpMessageFrame__Fv`, `hudMsgHandle`, `lightHaloTexture`, `musicDuckAmount`, `netSendMessage__FiPUci`, `nmPartners`, `partners`, `soundGetStreamPlayStatus__Fi`, `soundSetStreamVolume__Fif`, `stream`.

`netNetworkMode` and `scePrintf` are already defined on the tip. `soundMusicVolume` is defined by the volume unit in this package.

Not defined, and not given a second object: `nmPlayers`, `player`.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. `header_closure` is empty. `profile` is `authored_cpp`. `kind` is `independently_authored_source`. Do not add any `host_check.cpp` file. Body sizes are the reviewed EE g++ sizes, not the original extents. Data units emit no functions. Zero-filled symbols landed in BSS.

```json
{"source":"sources/dialog-hud-controls-net-player-is-local/net_player_is_local.cpp","object_name":"net_player_is_local.o","sha256":"a711bcdf640b44f206060ea5c1f13a0cee8557bf11798facaa2e18cff53c69a4","emitted_functions":[{"name":"netPlayerIsLocal__Fi","compiled_body_bytes":56}]}
{"source":"sources/dialog-hud-controls-net-get-local-player/net_get_local_player.cpp","object_name":"net_get_local_player.o","sha256":"d1e8361cb9e6f7290a4bc73d7d85210aa462133d2c0f6fee10e045c0a0df73b1","emitted_functions":[{"name":"netGetLocalPlayerNm__Fi","compiled_body_bytes":104}]}
{"source":"sources/dialog-hud-controls-dlg-active/dlg_active.cpp","object_name":"dlg_active.o","sha256":"555d329dd44d37b004519469c768107911e618acd7e1247a79212d0855e78bd4","emitted_functions":[{"name":"DLG_Active__Fv","compiled_body_bytes":16}]}
{"source":"sources/dialog-hud-controls-sound-fade/sound_fade.cpp","object_name":"sound_fade.o","sha256":"36ae84aa94f2b08e50e5fbad80129397f24a26e22a510d5f1a7d400969d5eca6","emitted_functions":[{"name":"soundFadeStream__Fi","compiled_body_bytes":244}]}
{"source":"sources/dialog-hud-controls-msg-hud-busy/msg_hud_busy.cpp","object_name":"msg_hud_busy.o","sha256":"9fbe3ae057c4c3fc617b201803b33a3664df15d17dbc80a492aa4dca26b910c7","emitted_functions":[{"name":"msgHudSetPlayerBusy__Fcc","compiled_body_bytes":124}]}
{"source":"sources/dialog-hud-controls-menu-set-pos/menu_set_pos.cpp","object_name":"menu_set_pos.o","sha256":"95a9149e27d36e91e1e9aabccb95ba82bd603b5a203149a3fce0c11d5e3eaf9d","emitted_functions":[{"name":"menuSetPos__FP4Menui","compiled_body_bytes":148}]}
{"source":"sources/dialog-hud-controls-sound-update-music/sound_update_music.cpp","object_name":"sound_update_music.o","sha256":"309ff791d99c6d9e585d0d97a260ed48f474733277f7a08c34adaffff43d1337","emitted_functions":[{"name":"soundUpdateMusicVolume__Fv","compiled_body_bytes":88}]}
{"source":"sources/dialog-hud-controls-help-message-init/help_message_init.cpp","object_name":"help_message_init.o","sha256":"83e4e0d2ebef1ff7db874d16319a1e464e0654088b5eaa734c73c3a2adc136fe","emitted_functions":[{"name":"helpMessageInit__Fv","compiled_body_bytes":28}]}
{"source":"sources/dialog-hud-controls-halo-texture/halo_texture.cpp","object_name":"halo_texture.o","sha256":"c1ee1f43c9aef18ee5ae3c5eed09dcfd475bfef63f7c6889cd85675e2b5dc963","emitted_functions":[{"name":"worldSetLightHaloTexture__FP8_texture","compiled_body_bytes":12}]}
{"source":"sources/dialog-hud-controls-special-sound/special_sound.cpp","object_name":"special_sound.o","sha256":"4ce3e12bb24e65dd46501d93bc7bb4e8421fb7e928656a2118c648e4d6d5d477","emitted_functions":[{"name":"specialSoundHandler__FiG6Point3","compiled_body_bytes":104}]}
{"source":"sources/dialog-hud-controls-button-words/button_words.cpp","object_name":"button_words.o","sha256":"59b9868b91c171626abb2983d2cce5c5f4ee262e6b53f39844937c0f6927706f"}
{"source":"sources/dialog-hud-controls-button-records/button_records.cpp","object_name":"button_records.o","sha256":"36e70cb6865fcfd97abe67a490c579f563da870fc27c946a26311f5afc2721b0"}
{"source":"sources/dialog-hud-controls-hud-comm-flags/hud_comm_flags.cpp","object_name":"hud_comm_flags.o","sha256":"d7919fcebadcd00736afd070a5c70b0940e99e20115484cc71cd510e4ba83027"}
{"source":"sources/dialog-hud-controls-dialog-words/dialog_words.cpp","object_name":"dialog_words.o","sha256":"e01d9981c224fa2c3462494044e2b7729f3b1cea60aa7948212c40467c8b283a"}
{"source":"sources/dialog-hud-controls-menu-state/menu_state.cpp","object_name":"menu_state.o","sha256":"22a13e578a6288262148e3e504b880b14b0d77cabccd8841b2567b4ad6481dc3"}
{"source":"sources/dialog-hud-controls-sound-volumes/sound_volumes.cpp","object_name":"sound_volumes.o","sha256":"362fd8cf125e763de8136fc90ad22937cb75a8eae844f1e017a565f2057bb7c9"}
{"source":"sources/dialog-hud-controls-main-menus/main_menus.cpp","object_name":"main_menus.o","sha256":"af0d3d1b420479b7e8d41b54c4c73e68ce792b27598a1604b63d2309ccfbbb69"}
```

## Authored but never executed

All ten functions and every data object above. Host checks ran on this Mac, and the reviewed EE compiler produced relocatable objects. No routine ran on the EE, under the original process, or as gameplay. `drawDialog__Fv` and `runDialog__Fv` were not called. No startup stage has run.

## Unknown

- `drawDialog__Fv` (`0x001d2f28`, 1448 bytes, 41 calls to 19 targets) and `runDialog__Fv` (`0x001d2940`, 1508 bytes) were not reconstructed.
- `padProcess__Fv` (`0x00167e68`, 708 bytes), `pauseMenuFrameRun__Fv` (`0x0017e738`, 4292 bytes), `soundFrame__Fv` (`0x0024b7f8`, 492 bytes), `soundGetStreamPlayStatus__Fi` (`0x0024adb8`, 248 bytes), and `soundResetStream__Fi` (`0x0024af80`, 272 bytes) were not reconstructed. The last two call `sceSifCallRpc`.
- `c.1089` at `0x007d0988` has ELF size 0. `worldBoundingBox` begins 384 bytes later. The constructor writes through offset 136. The size is not established, so this package does not define it.
- The second local `menu` at `0x00484ba4`, `menuNm` at `0x00484ba0`, and `startMenuState` at `0x00484bdc` are different objects with the same static names. The pause and debug routines use the first addresses. The second objects are not defined.
- `menuPlayerNm` is a word. Four unnamed bytes sit between it and `kickMenuList`.
- `stream` has ELF size 0. The 1360-byte gap to `gCd` is the bound used for the fade offsets. Bytes inside a record that `soundFadeStream` does not write are not described.
- The menu arrays keep absolute words. Whether each word is a pointer, and where those unnamed strings live in a relocatable link, is not settled by storing the original addresses.
- `curDlg` and `norrath_dialog_active_pointer_word_view` are two authored objects for one original word. Which one a final link should keep is an integrator choice.
- The ELF name `player` is not aliased to `norrath_player_progression_records_view` here.
- Exception flags, interrupts, and game runtime remain untested.
