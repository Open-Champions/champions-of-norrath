# w12-hud-loop-b

Package: game loop, HUD, shop, inventory and player-selection functions (B). Branch `agents/w12-hud-loop-b`.
Every function below is new source under `experimental/sources/w12-hud-loop-b-*`. Each was compiled with the
reviewed EE g++ `authored_cpp` profile and has a `host_check.cpp` (not a plan input) that exits non-zero on failure.
In addition each function was compared against the pinned `SLUS_205.65` in a private R5900 interpreter (outside git):
original and authored function run from identical memory with identical stub callees, and call logs plus every
changed byte outside the stack were compared. Those private comparisons cannot be repeated from the repository.
Byte-exact code matches are not claimed; compiled sizes differ from image extents.

## Functions (one commit each)

Observed in the image, per function: addresses and sizes are in the source header comments.
`saveActivate__9Savepoint`, `inventoryItemCheckSlot__FiPQ24item11description`, `itemEquipped__FiPQ24item11description`,
`updatePlayersReady__9Savepoint`, `playerSelectionNPCVisible__FP3NPC`, `playerSelectionGetNPCActiveChunks__FP3NPC`,
`playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord`, `runGold__Fv`, `loadPlayerLumps__Fv`,
`objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`, `inventoryStart__Fii`, `shopDrawTaskBar__FP12screenStruct`,
`maybeDrawMultiPlayerMarkers__6Player`, `updateCursor__FiiiiiP12screenStructi`, `resurectEffect__6Player`,
`playerBlockSparks__6Playeri`, `shopInventoryGridDraw__FiiiP12screenStruct`, `playerItemScan__6PlayerP3padi`,
`runHUD__Fi`, `shopInventoryGridRun__FiiiP12screenStruct`.
Data: `levelUpFade`, `multiBody`, `multiHelpMessageOn`, `shipOn`, `shopPromptOK` (one unit), `sprGrid`, `spriteCount`
(one unit), and the file-local `pokeMessageAlpha` / `pokeItemMagic`.

Earlier unplanned sources for `loadPlayerLumps`, `updatePlayersReady` and `saveActivate` exist from older waves
(rejected for missing or contradicting host checks); these are fresh authorings, not copies.

## Method notes
- A long decimal float literal is truncated by the EE compiler, which lands one unit below the observed float. Constants that
  are not short decimals are written as hexadecimal float literals (see `w12-hud-loop-b-resurect-effect`).
- 32-bit pointer words in the image are read through a `W12_HUD_HOST` widening hook so the host checks can run on a 64-bit host.
- `playerItemScan` publishes its function-local message buffer as `norrath_player_item_scan_message` (inferred name for
  the image's local `msg.1197`).
- Mutation sweeps (changed constants, boundaries, offsets) were run for every function except `shopInventoryGridRun`
  (each run takes minutes). Some boundary mutants are unreachable by any input (the right-edge clip of the translucent
  bars in `runHUD`, the 1e7 start distance in `playerItemScan`) and survive by design.

## Imports closed (names)
`inventoryItemCheckSlot__FiPQ24item11description`, `inventoryStart__Fii`, `itemEquipped__FiPQ24item11description`,
`levelUpFade`, `loadPlayerLumps__Fv`, `maybeDrawMultiPlayerMarkers__6Player`, `multiBody`, `multiHelpMessageOn`,
`objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`, `playerBlockSparks__6Playeri`,
`playerItemScan__6PlayerP3padi`, `playerSelectionGetNPCActiveChunks__FP3NPC`,
`playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord`, `playerSelectionNPCVisible__FP3NPC`,
`resurectEffect__6Player`, `runGold__Fv`, `runHUD__Fi`, `saveActivate__9Savepoint`, `shipOn`,
`shopDrawTaskBar__FP12screenStruct`, `shopInventoryGridDraw__FiiiP12screenStruct`,
`shopInventoryGridRun__FiiiP12screenStruct`, `shopPromptOK`, `sprGrid`, `spriteCount`,
`updateCursor__FiiiiiP12screenStructi`, `updatePlayersReady__9Savepoint`.
`sprGrid` and `spriteCount` duplicate the `norrath_floor_*_view` objects of `floor-storage` (same addresses, different
names); drop one pair when plans merge.

## Not closed
- `norrath_item_create` and `norrath_item_create_random`: invented names for `create__4itemPcPQ24item11descriptionii`
  and `createRandom__4itemPQ24item11descriptioniiii`, both already in the plan under their ELF names. An assembler
  alias to a symbol defined in another unit emits nothing (tested), and a forwarding wrapper would not be the original
  body. They need either a link-level alias entry or callers (`ctors-lava-wraith`) switched to the ELF names.
- `msg_run__6Player` (27,496 bytes), `routePlot__FR10RouteQueue` (19,812 bytes) and `playerSelectionTask__Fv`
  (12,200 bytes) were not attempted: each is several times larger than anything finished here and could not be done
  and verified in one sitting. Nothing partial was committed.

## Imports opened
References that these units make and no unit in this package defines (many are certainly defined by other packages or
the plan already; the exact net-new set needs a build): DLG_Active__Fv GLISTSIZE P_FaceSprite__FG6Point3ffiiiiP8_texturesi P_Push__Fv actionButton activePlayer beardSetting blueSpark buySellVertSpacing combineTable commNameMask coreForceSmallCharacters countItems__FP10GameObject countUp crystalTex curPlayer delta distanceToPlane2__FP6Point3P6Point2T0 drawFrameNm drawNewestQuickSpellMenu__Fiiicl dropButton dst engineAddTask__FPFv_viPci equipButton experienceTable fadeIconY fadeTimer floorDrawEnable fontStringSize__FP4FontPci frameNm fullCount gameCam gameControlPaused gameFlags gameMultiPlayerMarkersOn gameNoDraw gamePause getMoveDir__FiiiP12screenStruct getPic__4itemiPQ24item11description glistp globalIcons goldNumber gstart hairColorSetting hairStyleSetting haloTexture5 healthPotionsLeft helpMessageOn hideAbilityAdjustMouse hudNoDraw iRandState iconAlpha infoButton interfaceOn invGrid invHeader invMsgHandle inventory inventoryDrawDelay itemInfoHeight itemToDestroy levelFlags listInsert__FP8ListHeadP10__listNodePv localRandState lumpClear__Fi lumpFindResource__FPcT0 lumpLoad__FPc lumpLock__Fi lumpPartialLoad__FPcT0 lumpRename__FPcT0 manaPotionsLeft maxDistance mediumFont msgEquipItem__FiPQ24item11descriptioni msgHudSetPlayerBusy__Fcc msgInvItemSetAttributes__FiiPcPQ24item11description msgUnequipItem__FiPQ24item11description netLocalPad netNetworkMode netPlayerIsLocal__Fi nmPlayers nmPlayersAlive objectDrawEnable padAcceptButton padBackButton player playerAllocateArmorCache__Fii playerCheckArmorCacheRaces__Fv playerInitArmorCache__Fv playerSelectInfo playersAlive racePrefix rcursor recallOn redSpark runStats__Fii selectMode sellMode shop shopDisableMouse shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii shopGrid shopList shopMouseDecay shopMsgHandle shopRunMouseLogic__FiiP12screenStructi shopRunning smallFont sprintf src statsScreen strcmp syncRandState targets taskBar textureDrawColor__FP8_textureiiiUi textureDrawFloat__FP8_textureffiUiff trailParticleSpark updateTaskBar__FiP12screenStruct zeroPad.
Most likely net-new: `blueSpark`, `redSpark` (local particle objects used by `playerBlockSparks`; `trailParticleSpark`
is ambiguous because the image has four local objects of that name, this unit needs the one at 0x007cb550),
`combineTable`, `shopList`, `zeroPad` (the image has two; this package needs 0x007d03e8), `taskBar`, `crystalTex`,
`globalIcons`, `experienceTable`, `iconAlpha`, `fadeTimer`, `fadeIconY`, `gameFlags`.

## Build-plan entries needed
Append with fresh `input_order` values; `profile` is `authored_cpp`, `kind` is `independently_authored_source`,
`header_closure` is empty. Do not add `host_check.cpp`. `compiled_body_bytes` are reviewed-toolchain sizes, not image extents.

```json
{"source": "sources/w12-hud-loop-b-hud-words/hud_words.cpp", "object_name": "hud_words.o", "sha256": "036c25b26fdaea399c06df7f140067a7f4efd7aac4c59bfd4d539245862293fc"}
{"source": "sources/w12-hud-loop-b-inventory-item-check-slot/inventory_item_check_slot.cpp", "object_name": "inventory_item_check_slot.o", "sha256": "661ba00c3263a704bdebc4ceff84b1fdd3c3412a62faa707aa627acfd700b769", "emitted_functions": [{"name": "inventoryItemCheckSlot__FiPQ24item11description", "compiled_body_bytes": 108}]}
{"source": "sources/w12-hud-loop-b-inventory-start/inventory_start.cpp", "object_name": "inventory_start.o", "sha256": "fcb264d293d60e94aaff516a0acca794a6e90b2801fee35f5e10b51dad382b98", "emitted_functions": [{"name": "inventoryStart__Fii", "compiled_body_bytes": 772}]}
{"source": "sources/w12-hud-loop-b-item-equipped/item_equipped.cpp", "object_name": "item_equipped.o", "sha256": "39b4a419f26ca628e4fb9697626ee1c9ae0913e271ca0e5e56e32af4a78c441c", "emitted_functions": [{"name": "itemEquipped__FiPQ24item11description", "compiled_body_bytes": 280}]}
{"source": "sources/w12-hud-loop-b-load-player-lumps/load_player_lumps.cpp", "object_name": "load_player_lumps.o", "sha256": "3ccfc5903ebbd8779cf0af368526b550b3e654bc302a677108f824cd2baceddc", "emitted_functions": [{"name": "loadPlayerLumps__Fv", "compiled_body_bytes": 864}]}
{"source": "sources/w12-hud-loop-b-multi-player-markers/multi_player_markers.cpp", "object_name": "multi_player_markers.o", "sha256": "9b806caeb7bf9e0ff6c0117cc4552b2dae84362c11c071856d429856b1e2520c", "emitted_functions": [{"name": "maybeDrawMultiPlayerMarkers__6Player", "compiled_body_bytes": 736}]}
{"source": "sources/w12-hud-loop-b-player-block-sparks/player_block_sparks.cpp", "object_name": "player_block_sparks.o", "sha256": "079542622688711e01891db6c39e24f248b6ba1d8e06d009c37db0bd6bd78782", "emitted_functions": [{"name": "playerBlockSparks__6Playeri", "compiled_body_bytes": 1416}]}
{"source": "sources/w12-hud-loop-b-player-item-scan/player_item_scan.cpp", "object_name": "player_item_scan.o", "sha256": "5e20b634e4f1c8a8142a300e15312aebe192b352e92e44e6292679e61fcd3263", "emitted_functions": [{"name": "playerItemScan__6PlayerP3padi", "compiled_body_bytes": 3004}]}
{"source": "sources/w12-hud-loop-b-player-selection-npc-active-chunks/npc_active_chunks.cpp", "object_name": "npc_active_chunks.o", "sha256": "2e78659ae38562c6d491e773a739e0a09171c8506f55c9479a5775d89dba3677", "emitted_functions": [{"name": "playerSelectionGetNPCActiveChunks__FP3NPC", "compiled_body_bytes": 772}]}
{"source": "sources/w12-hud-loop-b-player-selection-npc-draw/npc_draw.cpp", "object_name": "npc_draw.o", "sha256": "655a26b0ad0f841122ccd7d72ede9e2990d0142f2a8b17ebd9be60898d0495be", "emitted_functions": [{"name": "playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord", "compiled_body_bytes": 1164}]}
{"source": "sources/w12-hud-loop-b-player-selection-npc-visible/npc_visible.cpp", "object_name": "npc_visible.o", "sha256": "d71de0134d9a0cf0379edfa47a94bd09fc01e1dd94fa43b6b6f97039c01d0ec2", "emitted_functions": [{"name": "playerSelectionNPCVisible__FP3NPC", "compiled_body_bytes": 616}]}
{"source": "sources/w12-hud-loop-b-poke-state/poke_state.cpp", "object_name": "poke_state.o", "sha256": "d1a3ac0f6764f92925431e3bf0d25ab2f0164f3ba0ba11ce3762eb4f462b05cc"}
{"source": "sources/w12-hud-loop-b-resurect-effect/resurect_effect.cpp", "object_name": "resurect_effect.o", "sha256": "eaac5b134d71cf667040d3311a3491b19fc73937cd34b940d1ab36e6edf64002", "emitted_functions": [{"name": "resurectEffect__6Player", "compiled_body_bytes": 1316}]}
{"source": "sources/w12-hud-loop-b-run-gold/run_gold.cpp", "object_name": "run_gold.o", "sha256": "1299c073b1f9d0aaffd1dda733e7da50146ef70dd394e34608fa170e81e2a08f", "emitted_functions": [{"name": "runGold__Fv", "compiled_body_bytes": 192}]}
{"source": "sources/w12-hud-loop-b-run-hud/run_hud.cpp", "object_name": "run_hud.o", "sha256": "d84b8cf9da8f261619b11ad1e58aa757160626e21a679f15974118f3c652f8f1", "emitted_functions": [{"name": "runHUD__Fi", "compiled_body_bytes": 4764}]}
{"source": "sources/w12-hud-loop-b-save-activate/save_activate.cpp", "object_name": "save_activate.o", "sha256": "5aeac765f040805fdc67eafed4aaf099910bb673e83bc946deab290fb929986b", "emitted_functions": [{"name": "saveActivate__9Savepoint", "compiled_body_bytes": 12}]}
{"source": "sources/w12-hud-loop-b-shop-draw-task-bar/shop_draw_task_bar.cpp", "object_name": "shop_draw_task_bar.o", "sha256": "eff7c436984a63fd047377fc512e1ed6f03453347dea4376f0bb1cc5e62d2bdf", "emitted_functions": [{"name": "shopDrawTaskBar__FP12screenStruct", "compiled_body_bytes": 492}]}
{"source": "sources/w12-hud-loop-b-shop-inventory-grid-draw/shop_inventory_grid_draw.cpp", "object_name": "shop_inventory_grid_draw.o", "sha256": "0e095f4e79468f45e3aec72328955ae1e4f00d457ae8ed78aa891eff408ee904", "emitted_functions": [{"name": "shopInventoryGridDraw__FiiiP12screenStruct", "compiled_body_bytes": 1880}]}
{"source": "sources/w12-hud-loop-b-shop-inventory-grid-run/shop_inventory_grid_run.cpp", "object_name": "shop_inventory_grid_run.o", "sha256": "fbf77398db776b43a24d9da4758eefeb126c8d953f639d96b41629af4d5eba51", "emitted_functions": [{"name": "shopInventoryGridRun__FiiiP12screenStruct", "compiled_body_bytes": 8240}]}
{"source": "sources/w12-hud-loop-b-sorted-creature-list/sorted_creature_list.cpp", "object_name": "sorted_creature_list.o", "sha256": "f2f407fefd232420af743dff656ddd656792a174581d4f5bd7bdd9fd1761fbd3", "emitted_functions": [{"name": "objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature", "compiled_body_bytes": 944}]}
{"source": "sources/w12-hud-loop-b-sprite-grid/sprite_grid.cpp", "object_name": "sprite_grid.o", "sha256": "54a4129f9b03597994a01ecaf62ae71cb9b980ff17e6c9e044f3ad9e882acf5e"}
{"source": "sources/w12-hud-loop-b-update-cursor/update_cursor.cpp", "object_name": "update_cursor.o", "sha256": "528f8989a49833c54ecd6eada67f34116643be98870fc6159023cfe369dc0667", "emitted_functions": [{"name": "updateCursor__FiiiiiP12screenStructi", "compiled_body_bytes": 1028}]}
{"source": "sources/w12-hud-loop-b-update-players-ready/update_players_ready.cpp", "object_name": "update_players_ready.o", "sha256": "3b4ce073a29288fcd3142c0d72107a8a4dede4daca659ce9148bbc3fb10a179d", "emitted_functions": [{"name": "updatePlayersReady__9Savepoint", "compiled_body_bytes": 576}]}
```

## Authored but never executed
All of it. Host checks ran on this Mac; the paired comparisons ran in a private interpreter, not on the EE and not in
the game. No startup stage has run.

## Unknown
- Meaning of most record fields (player record floats at +172/+176/+180/+196/+200/+204, item flag bits 2^24, 2^35,
  2^38, screen words [12] and [28], bytes 28/29 of the item description) beyond their observed use.
- Names of the vtable entries called by `playerItemScan`, `objectGetSortedCreatureList` and the shop grid.
- Whether `pokeMessageAlpha` / `pokeItemMagic` or the `norrath_player_item_scan_message` buffer are used by other code.
- The combine path and the quest-drop path of `shopInventoryGridRun` have no host-check case; the paired run covers them.
