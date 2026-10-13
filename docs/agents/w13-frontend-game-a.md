# Front end, loading, player selection, shop, memory card and inventory/game-loop support (part 1 of 2)

Package `w13-frontend-game-a`, branch `agents/w13-frontend-game-a`. Original names, offsets and
behaviour were read from the pinned SLUS-20565 image with the EE binutils; nothing here has run on
the EE or as gameplay. Compiled sizes are not image sizes and no function is claimed as a byte
match. Every unit below compiles with the reviewed toolchain and has a committed host check that
builds and passes natively on arm64 macOS (command lines in the table).

## How each unit was checked

* **Host check** (committed, repeatable): `c++ -std=c++17 -Wall -Wextra -Werror <flags> host_check.cpp`, where
  the callees are recording stand-ins. Each check was mutation-tested by hand: single-line source
  edits were applied to a scratch copy and the check had to fail (equivalent edits and edits that
  only matched header comments are not counted). Where 32-bit pointer words sit in memory, the
  check uses either the host arena convention (`NORRATH_HOST_ARENA`, `norrath_host_base`) or a
  handle table (`W13_FRONT_HOST`, `hostWordAddress`); the EE build of the same source is unchanged.
* **Paired run** (private, outside git): the authored function and the original function were run
  side by side from identical memory snapshots on a private R5900 interpreter with identical stub
  callees; call logs, every changed byte outside the stack and (where meaningful) the return value
  had to match. This is local evidence only and cannot be repeated from the repository. Scenario
  counts and coverage are in the commit messages. The interpreter needed the second-pipeline
  `mult1`/`div1`/`mflo1` family added for `shopDrawItemBitmap`; halts count as failures.

Observed / inferred / unknown are kept separate inside each source file header; the short form is
in the last three sections of this file.

## Units, build-plan entries and checks

All entries use profile `authored_cpp`, an empty header closure and `kind` independently_authored_source.
Add them to `snapshot.json` as well; the integrator assigns `input_order`. The `Object` column is the
suggested `object_name`. "check" is the committed host check; "flags" are its extra compiler flags.

| Source (under `experimental/sources/`) | SHA-256 | Emitted functions (compiled bytes) | Host check | Flags |
|---|---|---|---|---|
| `w13-frontend-game-a-msg-equip-item/msg_equip_item.cpp` | `5c25284e419bfe5c8480390b2b602d0008bea0a2308df3b91df148e535e7f804` | `msgEquipItem__FiPQ24item11descriptioni` 72 | `w13-frontend-game-a-msg-equip-item/host_check.cpp` | `-` |
| `w13-frontend-game-a-msg-unequip-item/msg_unequip_item.cpp` | `a5a53469e4f13843cc35db92c36dfae44a88ace146a54a426d3da698ed2145ab` | `msgUnequipItem__FiPQ24item11description` 64 | `w13-frontend-game-a-msg-unequip-item/host_check.cpp` | `-` |
| `w13-frontend-game-a-msg-inv-item-set-attributes/msg_inv_item_set_attributes.cpp` | `7a25d80f271aba006623a812bc253801a99f79a6f4e24b3e733e95017372968d` | `msgInvItemSetAttributes__FiiPcPQ24item11description` 88 | `w13-frontend-game-a-msg-inv-item-set-attributes/host_check.cpp` | `-` |
| `w13-frontend-game-a-mc-err-wnd/mc_err_wnd.cpp` | `8c42a7f9ed47df74208bc5ceab29d22c35cba7ca939821990f2d3d58bbeded0f` | `McErrWnd__Fv` 12 | `w13-frontend-game-a-mc-err-wnd/host_check.cpp` | `-` |
| `w12-actor-spell-c-msg-run-7weather/msg_run.cpp` | `fb04c25e63b82c008efbd3fc4227d21afba0dc1b9a4cd35764f18ffe0c3d56c6` | `msg_run__7Weather` 8 | `w13-frontend-game-a-msg-run-7weather/host_check.cpp` | `-` |
| `w10-front-a-mc-convert-temp-player-to-real/mc_convert_temp_player_to_real.cpp` | `eaa32379cec41b516348f17f2b4c0ce05ca48c965e2f7cda07d18eb07f33d55f` | `MC_ConvertTempPlayerToReal__Fi` 100 | `w13-frontend-game-a-mc-convert-temp-player-to-real/host_check.cpp` | `-` |
| `w10-front-a-mc-set-ram-nm-players/mc_set_ram_nm_players.cpp` | `f74276bd8ee26825c1122d5f4c6d0dd71d2405a2b4566273d26dfdd82e9045e6` | `MC_SetRamNmPlayers__Fi` 108 | `w13-frontend-game-a-mc-set-ram-nm-players/host_check.cpp` | `-fno-builtin` |
| `w10-front-a-mc-build-packed-player/mc_build_packed_player.cpp` | `b76eda68f2819d15051a1e41b9d551ee332e7a57bbb41d5e21218832befcdf57` | `MC_BuildPackedPlayer__FP11playerEntryi` 84 | `w13-frontend-game-a-mc-build-packed-player/host_check.cpp` | `-fno-builtin` |
| `w10-front-a-mc-net-pack/mc_net_pack.cpp` | `648cd7f877ccd63e4c377227ad5f386c5bd7b9b8c617cd21b7d84b298f993878` | `MC_NetPack__Fv` 524 | `w13-frontend-game-a-mc-net-pack/host_check.cpp` | `-fno-builtin` |
| `w10-front-a-mc-net-un-pack/mc_net_un_pack.cpp` | `29c9db525a6091d36e4bdfcd5320ee0fbc4da05ecc8e80e1b5daf0e07a0ac7ef` | `MC_NetUnPack__Fv` 808 | `w13-frontend-game-a-mc-net-un-pack/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-core-start-loading-screen/core_start_loading_screen.cpp` | `7d25bb812d6abc1710a824538c735847dc604293e630f820ceec37eb58ce8d64` | `coreStartLoadingScreen__Fv` 1504 | `w13-frontend-game-a-core-start-loading-screen/host_check.cpp` | `-DNORRATH_HOST_CONTRACT -fno-builtin` |
| `w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp` | `232ae541045e730b711e7bcaa8e8459ac700829ca57e3c9de780394b28ce2b33` | `MC_CheckPlayerImportOrChange__Fv` 2248; `_ZN12_GLOBAL__N_1L14remove_flaggedEi` 192 | `w13-frontend-game-a-mc-check-player-import-or-change/host_check.cpp` | `-fno-builtin` |
| `w11-front-a-mc-load-level-entities/mc_load_level_entities.cpp` | `54c5a5cdffa7e72aa22166eb19faae0b11234abb0b85d36efb7f84d41d6987da` | `MC_LoadLevelEntities__FPcT0` 812 | `w13-frontend-game-a-mc-load-level-entities/host_check.cpp` | `-Wno-deprecated-declarations -fno-builtin` |
| `w12-hud-loop-a-draw-gold/draw_gold.cpp` | `507a3e3eca845c2dc765e85f35123d6bef6d93adf48d8493253eecca8d30fd90` | `drawGold__Fv` 1388 | `w13-frontend-game-a-draw-gold/host_check.cpp` | `-Wno-deprecated-declarations -ffp-contract=off -fno-builtin` |
| `w13-frontend-game-a-lump-rename/lump_rename.cpp` | `9e718548a43178d156ecd7ac0680437205c9ee4aac1f9c3f6283ab30e11bf000` | `lumpRename__FPcT0` 188 | `w13-frontend-game-a-lump-rename/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-get-pic-4item/get_pic.cpp` | `2bb2e382888fdd8c233d7371fe5d6e0e25b100ae5e0770781187d1de56e18980` | `getPic__4itemiPQ24item11description` 184 | `w13-frontend-game-a-get-pic-4item/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-change-net-cam-6camera/change_net_cam.cpp` | `daaa890b0f04aec4e384f1e819ff2730362b4902625f81a2500d6f622e0cbaf9` | `ChangeNetCam__6Camerai` 192 | `w13-frontend-game-a-change-net-cam-6camera/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-list-insert/list_insert.cpp` | `7eef904556a482bcf3d082383376356f9aa3a41789bf22af7982562d037294a2` | `listInsert__FP8ListHeadP10__listNodePv` 100 | `w13-frontend-game-a-list-insert/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-count-items/count_items.cpp` | `e76b6cc38bf9a0539ca443eb418bd4da876898394033e4d1667a7076afd80959` | `countItems__FP10GameObject` 612 | `w13-frontend-game-a-count-items/host_check.cpp` | `-DW13_FRONT_HOST -ffp-contract=off -fno-builtin` |
| `w13-frontend-game-a-obj-get/obj_get.cpp` | `c751474e1be433f60ecb0462e096b8973b8819e825b1bf5233a369e75882f350` | `objGet__FP10GameObjectPc` 1672 | `w13-frontend-game-a-obj-get/host_check.cpp` | `-DW13_FRONT_HOST -fno-builtin` |
| `w13-frontend-game-a-get-move-dir/get_move_dir.cpp` | `a69d6e57ec7bd447b5f4c8b1eab85f1be5acdf232bfe2da35c97ccc0b607d018` | `getMoveDir__FiiiP12screenStruct` 284 | `w13-frontend-game-a-get-move-dir/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-shop-run-mouse-logic/shop_run_mouse_logic.cpp` | `c1df9949d33554d8d1f4aeb3b8ce63d722a7725e65c61ec6ecfeb1d054abf42c` | `shopRunMouseLogic__FiiP12screenStructi` 512 | `w13-frontend-game-a-shop-run-mouse-logic/host_check.cpp` | `-ffp-contract=off -fno-builtin` |
| `w13-frontend-game-a-lump-partial-load/lump_partial_load.cpp` | `8b32c367740bbc9098db1321ee3da238a55e94af177c8c4006adbb3564e80273` | `lumpPartialLoad__FPcT0` 1024 | `w13-frontend-game-a-lump-partial-load/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-armor-cache-locals/armor_cache_locals.cpp` | `14587c98e44cab4fb7d4d7e2415a107daea0f0a9d7658c28185b66cd3eb61286` | (data only: `armorName.1138`, `equipmentSlot2Cache.1137`) | `w13-frontend-game-a-armor-cache-locals/host_check.cpp` | `-` |
| `w13-frontend-game-a-player-allocate-armor-cache/player_allocate_armor_cache.cpp` | `49cb807da285272867ec1c36a3f1f6340311cf2c0eb96ce162d254d81c460c81` | `playerAllocateArmorCache__Fii` 956 | `w13-frontend-game-a-player-allocate-armor-cache/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-update-task-bar/update_task_bar.cpp` | `da5a72b87eaf904876cf95413a2bcf269f78af44c8b4e0e818c2bc70cc8f79a5` | `_ZN12_GLOBAL__N_1L12clearAndPlayEiPiPc` 200; `updateTaskBar__FiP12screenStruct` 380 | `w13-frontend-game-a-update-task-bar/host_check.cpp` | `-fno-builtin` |
| `w13-frontend-game-a-shop-draw-info-box/shop_draw_info_box.cpp` | `216d62baf7a4eb3b0c7b368e09602fff9951703f48b151f0daa60c363411a67f` | `shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii` 2088 | `w13-frontend-game-a-shop-draw-info-box/host_check.cpp` | `-ffp-contract=off -fno-builtin` |
| `w13-frontend-game-a-shop-draw-item-bitmap/shop_draw_item_bitmap.cpp` | `bc320649e137a1e916f56b530b69eca7786054b9dd37d3a71d3a7c3cf1d05df1` | `shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii` 2512 | `w13-frontend-game-a-shop-draw-item-bitmap/host_check.cpp` | `-ffp-contract=off -fno-builtin` |
| `w13-frontend-game-a-draw-newest-quick-spell-menu/draw_newest_quick_spell_menu.cpp` | `eab0b19db3ca2a3567a21fff8e3d5922222852466ebc796f562381966fb6ce65` | `drawNewestQuickSpellMenu__Fiiicl` 4352 | `w13-frontend-game-a-draw-newest-quick-spell-menu/host_check.cpp` | `-ffp-contract=off -fno-builtin` |

Notes on the entries:

* Nine entries point at sources written by earlier waves that were left out of the plan for lack of a
  committed check: `w10-front-a-mc-convert-temp-player-to-real`, `-mc-set-ram-nm-players`, `-mc-build-packed-player`,
  `-mc-net-pack`, `-mc-net-un-pack`, `w9-frontend-a-mc-check-player-import-or-change`, `w11-front-a-mc-load-level-entities`,
  `w12-hud-loop-a-draw-gold` and `w12-actor-spell-c-msg-run-7weather`. This package adds only the check (a
  `w13-frontend-game-a-*` directory with a new check that includes the source; for `mc-set-ram-nm-players` and
  `mc-build-packed-player` the directory only points at the w11 check that already existed and passes).
  The sources are unchanged, so the plan entry names the original file.
* `w13-frontend-game-a-core-start-loading-screen` is a v2 copy of `w10-front-a-core-start-loading-screen` with the
  global-pointer read behind `NORRATH_HOST_CONTRACT` so a host check can build it. **Plan only one of the two.**
* `w11-front-a-mc-load-level-entities` is the planned source: the w10 copy cannot be checked on a 64-bit host.
  Its check needs `-DW11_FRONT_A_HOST`; `w13-frontend-game-a-mc-load-level-entities/host_check.cpp` defines it.
* `w13-frontend-game-a-draw-gold/host_check.cpp` is the w12 check with its last case corrected (the old one
  searched for a call order the source never produces, so it exited 10 on every host).
* The compiler emits no helper symbols for these units except where the table shows more than one function.

## Imports closed

28 names, all with a committed host check:

`ChangeNetCam__6Camerai`, `MC_BuildPackedPlayer__FP11playerEntryi`, `MC_CheckPlayerImportOrChange__Fv`, `MC_ConvertTempPlayerToReal__Fi`, `MC_LoadLevelEntities__FPcT0`, `MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `MC_SetRamNmPlayers__Fi`, `McErrWnd__Fv`, `coreStartLoadingScreen__Fv`, `countItems__FP10GameObject`, `drawGold__Fv`, `drawNewestQuickSpellMenu__Fiiicl`, `getMoveDir__FiiiP12screenStruct`, `getPic__4itemiPQ24item11description`, `listInsert__FP8ListHeadP10__listNodePv`, `lumpPartialLoad__FPcT0`, `lumpRename__FPcT0`, `msgEquipItem__FiPQ24item11descriptioni`, `msgInvItemSetAttributes__FiiPcPQ24item11description`, `msgUnequipItem__FiPQ24item11description`, `msg_run__7Weather`, `objGet__FP10GameObjectPc`, `playerAllocateArmorCache__Fii`, `shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii`, `shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii`, `shopRunMouseLogic__FiiP12screenStructi`, `updateTaskBar__FiP12screenStruct`.

Not closed from the assigned list (not attempted or not finished, see the end of this file):
`msg_run__6Player`, `megatileGenerate__FP12_worldHeader`, `routePlot__FR10RouteQueue`, `runStats__Fii`.

## Imports opened

Symbols the new sources reference that neither the shared partial build defines nor its unresolved list
already named (computed from `nm` of each unit against the integrator's `game-components-partial.o`):

`a.1122`, `blackoutTexCopy`, `commaString__FPc`, `curBoxOld`, `displayItemStats__FiiiPQ24item11descriptioniii`, `drewBitmap`, `floorPutSaveInfo__FPc`, `foregrndTexCopy`, `getPackChecksum__Fv`, `hudGetRemappedButtonIcon__Fii`, `hudQuickSpellOn`, `loadBuffer`, `loadFrames`, `loadStack`, `loadThreadMain__FPv`, `loadThreadParam`, `mcDisableStartupScript`, `megatileEntitize__FPci`, `msgHudAssignQuickSpell__Fiii`, `msgHudAssignSpellBookSpell__Fiiil`, `objectPutSaveData__FPci`, `padChangeIndex__Fii`, `picAdd__4itemPc`, `playerGoldIconx`, `readMap__3mapPUc`, `scrollmapTexCopy`, `shopClipGridCoordinates__FPiN30`, `shopDrawItemBitmapGlow__FP8_textureii`, `shopMouseOnHotSpot__FP12screenStruct`, `titleloadTexCopy`.

`a.1122` and `curBoxOld` are file-local variables of the quick-spell menu in the image (`a.1122` at 0x007d0568, four
ints, per-player fade alpha; `curBoxOld` at 0x00428290, four ints); `titleloadTexCopy`, `scrollmapTexCopy`,
`blackoutTexCopy`, `foregrndTexCopy` are the 64-byte texture copies of the loading screen (defined in the sources
as `Texture64` objects of those names but not in any data unit). `commaString__FPc` is called by `drawGold`.
Names already open before this package that these sources also need (so no new import): 
`fullCount`, `hideAbilityAdjustMouse`, `iRandState`, `itemInfoHeight`, `itemToDestroy`, `maxDistance`, `zeroPad`.

`armorName.1138` and `equipmentSlot2Cache.1137` are defined by `w13-frontend-game-a-armor-cache-locals` (not on the
original unresolved list because they were only reachable through the not-yet-authored allocator).

`zeroPad` is ambiguous in the image (three local objects at 0x007cc570, 0x007d03e8 and 0x007d04e0): the
functions here that read it (`shopRunMouseLogic`) use the one at 0x007d03e8; the integrator must keep that
object under the name `zeroPad` for them (earlier-wave sources use the same name).

## Authored but never executed

Everything. No function here has run on the EE, in an emulator, or inside the game. The only execution evidence is
the host checks (committed) and the private paired runs on a hand-written interpreter (not repeatable from the
repository, and the interpreter is the author's own model of the R5900; its float rounding is round-to-nearest,
which is not the console's truncating behaviour). Specifically not exercised: the null-object paths of `objGet` and
`countItems` (the original calls `__throw_bad_typeid`), the empty free list of `listInsert` (the original faults),
`getPicName`/`picAdd` and other callees, which are stand-ins everywhere.

## Unknown

* The meaning of most record fields touched here: screen record words (see `shop_inventory_grid_run` for the
  numbering), the item cell halfword at +34, the lump record words at +24, +28, +32 and +36, the directory entry
  words beyond the name, the flag at pad +16, and the roles of `flag`, `kind` and `index` in `drawNewestQuickSpellMenu`.
* Original names for the unnamed constants: the float and bit patterns are given as bit patterns in the sources.
* `countItems`: the initial value of `maxDistance` (a .data float) and what bit 39 of the flags means.
* `shopDrawInfoBox` computes a shortened item name that it never uses afterwards; why is unknown.
* The original code's inlined thousands-separator routine puts the first comma at `len % 3`, or `len / 2` when that is
  zero, so a nine-digit price prints wrongly; the authored code reproduces that (observed, probably a defect).
* Section placement of the two local tables follows the build profile rather than the original `.data` split.

## Not closed, and why

* `runStats__Fii` (0x001bcb38, 4508 bytes): read in full, not authored. Observed outline: arguments are a player and a
  mode (1, 2 or 3); the screen record is `statsScreen + 116 * player`, whose word 13 names the player record shown;
  mode 2 handles the level-up point allocation (text keys for the stat names, `printStat__FiPUsiiii`,
  `msgInventoryAdjustAbility__Fii`, `updateCursor__FiiiiiP12screenStructi`, `engineRemoveTask__FPFv_v(statsFrame__Fv)`,
  `givePlayerExperience__Fii`), modes 2 and 3 end in `statsRunSkillTree__Fi`, and mode 1 draws the statistics page
  (health, energy, armour, damage and the four stats, `displayResistances__Fi`, `playerRecomputeAbilityModifiers__Fv`).
  The cursor bob uses the soft-double routines (`litodp`, `dpmul`, `fptodp`, `dpadd`, `sin`, `dptoli`) through 64-bit
  register values, and the page uses `usprintf` with eight float arguments and about forty text keys. Authoring it
  needs the same treatment as `do_driveline` (declare the soft-double routines as `unsigned long long` functions) and
  every key string read from the image. Nothing was written because of its size next to the rest of the package.
* `megatileGenerate__FP12_worldHeader` (19,680 bytes), `routePlot__FR10RouteQueue` (19,812 bytes) and
  `msg_run__6Player` (27,496 bytes): not started. Each is several times larger than anything authored here and needs
  its own bounded task; none has an authored partial source in the snapshot. They remain unresolved imports.

## Honest corrections to commit messages

Four commit messages quote mutation counts that were loosely tallied while the checks were still being extended:
`Add getPic`, `Add lumpPartialLoad`, `Add drawNewestQuickSpellMenu` and `Add host check for MC_CheckPlayerImportOrChange`.
The accurate statement is the one in this file: every single-line source mutation that was not equivalent (or a
match inside a header comment) was caught by the final committed checks; the checks were extended once after a first
round of mutations left a few cases undetected (boundary values in the quick-spell fade and the item-prefix digit
trimming, among others). `Add lumpPartialLoad` was followed by a fix commit because its first check failed in one
case (stale lump records left by an earlier case); every later commit was gated on its check passing.

