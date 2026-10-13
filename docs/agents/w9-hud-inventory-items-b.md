# w9-hud-inventory-items-b

Package: HUD, inventory, shop, item and fonts (b). Branch `agents/w9-hud-inventory-items-b`. New source is under `experimental/sources/w9-hud-inventory-items-b-*`; every directory holds one implementation file and a `host_check.cpp` that is not a build input. This run continued an earlier interrupted run of the same package that had already committed six functions (`hudInfoMessageShowing__Fv`, `getProxyMeshId__4itemPQ24item11description`, `lerpShortRot__Fssfff`, `inventoryFrameRun__Fv`, `inventoryAdjustAbility__Fii`, `inventoryUnload__Fv`); those were reviewed again and are included in the checks below.

Private reference: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that image's symbol values and instruction operands. No game bytes, disassembly or decompiler output are committed; all text is my own description.

## Checks performed

- Every implementation compiles with the reviewed EE g++ `authored_cpp` flags (taken from the plan profile), and every host check, built with the host C++ compiler (units that need it with `-fno-builtin`, and `-DNORRATH_INVENTORY_HOST=1` for `inventoryGridDropItem__Fiiciiii`, as the sibling inventory-items checks do), returns 0.
- Paired runs (private, outside git): each function was also run against the original in a small R5900 interpreter. The original and the authored object (linked at a separate address with every undefined symbol bound to the original's address) start from identical memory, call the same Python stub callees, and are compared on the return value (where the function returns one), the ordered callee log with arguments, and every byte changed outside the stack. Scenario counts per function are in the table. Mutated copies of the source were confirmed to fail for `lerpShortRot__Fssfff`, `hudGivePlayerGold__FiG6Point3i`, `getPicName__4itemPciPQ24item11description` and `getName__4itemPQ24item11descriptioni`, so the comparison can fail. The interpreter, harness and scenario scripts load the private image, so they are not published. This is a comparison on an interpreter with stubbed callees, not a run on the EE or in the game.
- The paired runs found one real divergence: `inventoryGridDropItem__Fiiciiii` first passed the drop position by value as a plain struct and expected it returned in registers, but the original passes it by invisible reference and receives the result through a hidden pointer (as for a class with a non-trivial copy constructor). The type was fixed and the run repeated (700 scenarios, 0 differences).
- A private copy of the plan and snapshot (outside git, in a scratch directory) with the 34 units appended passed `--check-plan` and built with `build_partial.py`: 2350 inputs, 1 link, 0 COMMON symbols, 1896 function symbols, 1507 unresolved imports. 33 of the 34 package names are absent from its `unresolved-imports.json`. The source files were hashed again after the last comment edit (`--check-plan` passes; the full build predates a comment-only edit that does not change the compiled code).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

A fresh EE compile is not an original-byte match; compiled body sizes differ from the image extents. These are behaviour reconstructions.

## Functions

| symbol | image | source (under `sources/`) | compiled bytes | private paired scenarios |
|---|---|---|---|---|
| `givePlayerFeat__Fxii` | `0x001be250`, 996 | `w9-hud-inventory-items-b-give-player-feat/give_player_feat.cpp` | 608 | 500 |
| `hudGivePlayerGold__FiG6Point3i` | `0x001c4190`, 868 | `w9-hud-inventory-items-b-hud-give-player-gold/hud_give_player_gold.cpp` | 632 | 60 |
| `hudInfoMessageShowing__Fv` | `0x001c7690`, 12 | `w9-hud-inventory-items-b-hud-info-message-showing/hud_info_message_showing.cpp` | 16 | 6 states |
| `inventoryAdjustAbility__Fii` | `0x001b1938`, 176 | `w9-hud-inventory-items-b-inventory-adjust-ability/inventory_adjust_ability.cpp` | 184 | 120 |
| `inventoryAdjustSkill__Ficx` | `0x001aeb00`, 376 | `w9-hud-inventory-items-b-inventory-adjust-skill/inventory_adjust_skill.cpp` | 392 | 300 |
| `inventoryCheckWieldingStatus__FiPQ24item11description` | `0x001b0c18`, 1228 | `w9-hud-inventory-items-b-inventory-check-wielding-status/inventory_check_wielding_status.cpp` | 1048 | 600 |
| `inventoryExit__Fiii` | `0x001acc20`, 356 | `w9-hud-inventory-items-b-inventory-exit/inventory_exit.cpp` | 376 | 160 |
| `inventoryFrameDraw__Fv` | `0x001ad870`, 1152 | `w9-hud-inventory-items-b-inventory-frame-draw/inventory_frame_draw.cpp` | 1160 | 200 |
| `inventoryFrameRun__Fv` | `0x001b1848`, 120 | `w9-hud-inventory-items-b-inventory-frame-run/inventory_frame_run.cpp` | 160 | 80 |
| `inventoryGridDropItem__Fiiciiii` | `0x001b0578`, 1696 | `w9-hud-inventory-items-b-inventory-grid-drop-item/inventory_grid_drop_item.cpp` | 1820 | 700 |
| `inventoryLoad__Fv` | `0x001ac510`, 440 | `w9-hud-inventory-items-b-inventory-load/inventory_load.cpp` | 428 | 20 |
| `inventorySetAnim__FiP14AnimationStatei` | `0x001b01b0`, 652 | `w9-hud-inventory-items-b-inventory-set-anim/inventory_set_anim.cpp` | 372 | 300 |
| `inventoryUnload__Fv` | `0x001b1698`, 132 | `w9-hud-inventory-items-b-inventory-unload/inventory_unload.cpp` | 140 | 30 |
| `getName__4itemPQ24item11descriptioni` | `0x00101c30`, 1348 | `w9-hud-inventory-items-b-item-get-name/item_get_name.cpp` | 1132 | 400 |
| `getPicName__4itemPciPQ24item11description` | `0x001075f0`, 1164 | `w9-hud-inventory-items-b-item-get-pic-name/item_get_pic_name.cpp` | 1320 | 400 |
| `getProxyMeshId__4itemPQ24item11description` | `0x00108498`, 68 | `w9-hud-inventory-items-b-item-get-proxy-mesh-id/item_get_proxy_mesh_id.cpp` | 48 | 300 |
| `getValue__4itemPQ24item11description` | `0x00102178`, 6048 | `w9-hud-inventory-items-b-item-get-value/item_get_value.cpp` | 5972 | 1500 |
| `makeRandomPotion__4itemPQ24item11description` | `0x00107ce8`, 632 | `w9-hud-inventory-items-b-item-make-random-potion/item_make_random_potion.cpp` | 404 | 300 |
| `lerpColor__FUiUif` | `0x00193e20`, 704 | `w9-hud-inventory-items-b-lerp-color/lerp_color.cpp` | 572 | 600 |
| `lerpShortRot__Fssfff` | `0x0015a610`, 168 | `w9-hud-inventory-items-b-lerp-short-rot/lerp_short_rot.cpp` | 176 | 600 |

`hudInfoMessageShowing__Fv`'s count is its six timer values.

## Data objects

Each is a single unit under `sources/w9-hud-inventory-items-b-data-<name>/` that defines the original ELF name. Where the symbol has declared size 0 the extent is the gap to the next symbol (inferred); the run-time contents are unknown and the object is zero-initialised.

| symbol | address | bytes | evidence |
|---|---|---|---|
| `goldNumber` | `0x007d0588` | 288 | gap to `hudBox`; `hudGivePlayerGold__FiG6Point3i` walks eight 36-byte entries |
| `goldAlpha` | `0x007d08e0` | 16 | gap to `playerGettinIt` |
| `hermiteTime` | `0x00486d34` | 4 | gap to `delta` |
| `hermDeltaScale` | `0x00486d40` | 4 | gap to `flareDelta` |
| `lastFontColor` | `0x00486988` | 4 | gap to `curFont` |
| `msgTimeout` | `0x008d8d28` | 32 | gap to `sQueue` |
| `nameTags` | `0x008d5dc0` | 128 | gap to `lightInfo` |
| `hudHeaderUni` | `0x004282e0` | 256 | nm size; image bytes all zero |
| `inventory` | `0x00427288` | 464 | nm size (four 116-byte screen records); image bytes all zero |
| `invGrid` | `0x00484544` | 4 | nm size; image word zero |
| `inventoryLump` | `0x00484820` | 4 | nm size; image word zero |
| `inventoryLoadState` | `0x00484824` | 4 | nm size; image word zero |
| `lastFontCharAlpha` | `0x0048411c` | 4 | nm size; image word 0xffffffff |

One further data unit, `w9-hud-inventory-items-b-view-exit-sound-resource`, defines `norrath_inventory_exit_sound_resource_view` (21 bytes: the string `game_lvl_complet.anm` and its terminator at `0x00466348`), which `inventoryExit__Fiii` passes to `lumpFindResource__FPcT0`. It is a view name, like the existing `norrath_inventory_ability_sound_*` units, not an original ELF name.

## Observed

- Layouts, offsets, constants and call orders in the source comments were read from the instructions and the rodata/data the instructions reference. The float constants are the image's literal-pool words, written as hexadecimal float literals in `getValue__4itemPQ24item11description` because some are not the nearest float to their short decimal form (for example the exponent 1.95 is stored as 1.9499999, and the first paired run of that function failed on exactly this until the literals were read from the image).
- Player records are 23984 bytes apart at `player`; the inventory lists, 72-byte item slots and 104-byte feat/skill slots at `+21976` are consistent across `inventoryGridDropItem__Fiiciiii`, `givePlayerFeat__Fxii` and `inventorySetAnim__FiP14AnimationStatei`.
- `inventory` is an array of 116-byte records, `statsScreen` (0x004281a8, 232 bytes) likewise.
- `hudGivePlayerGold__FiG6Point3i` takes the first of eight 36-byte `goldNumber` entries whose word at +28 is zero, projects the world position with `matTransform` and `worldViewMatrix`, and gives the entry a drift vector perpendicular to the direction to `(playerGoldx[p], playerGoldy[p])`; with all eight in use it adds the amount to the player record word at +232.
- `getValue__4itemPQ24item11description` sums a weapon (or armor) base and per-tag terms through `powf`, with the first matching effect entry supplying the values.

## Inferred

- Field roles (screen record words, item effect entries `{tag, v1, v2, v3}`, the goldNumber entry layout), that `getValue` is a price, that `typeScaleTab` (7 floats at 0x0031f510) is a per-subtype damage scale, that the two accumulators in `getValue` are damage low/high, that `eRandState` is a cosmetic generator and `syncRandState` the synchronised one. Sizes of the gap-sized data objects.
- Mapping the string tables to meanings only from the strings (lump names of the playable models, `*_idleb.anm`, `inv_drop_*.anm`, the race prefixes, `circlet`, `spear`).

## Unknown

- What the screen-record mode words, flag bits, kinds, variants, effect tags (14..19, 20..38, 0..13, 37) and item shapes mean; message type 9 and the other bytes sent by `inventoryExit__Fiii`; the meaning of the value 29 in the feat table; what `_shopCheck__Fi` does.
- Whether `gameDifficulty` can be 0 (`makeRandomPotion__4itemPQ24item11description` then indexes past its 9-entry tables in the original and in this reconstruction; the paired run therefore uses difficulties 1..3), and whether `typeScaleTab[sub]` is ever read out of range.
- The real extent and contents of the gap-sized data objects.

## Authored but never executed

All of it. No function ran on the EE, in the game or on a runtime stack of authored units. They were only run against stubs: the host checks and the private interpreter pairing described above. In particular:

- `inventoryLoad__Fv` contains the original's store of a zero word through a null pointer when `textureLockCacheBuffer__Fv` returns null in the loading state. It is reproduced as a volatile write to address 0 and is not exercised by any check (it would fault on the host).
- The data definitions are zero-filled (or hold the observed initial word) and were only checked for size and initial contents. No authored code in this package reads `hermiteTime`, `hermDeltaScale`, `lastFontColor`, `msgTimeout`, `nameTags`, `goldAlpha`, `hudHeaderUni` or `lastFontCharAlpha`.
- Behaviour that depends on callees (`getAttributeName`, `findAndStripGender`, `usprintf`, `getDamageRangeAll`, `getArmorBonus`, `create`, ...) was only checked against stub callees.

## Imports closed

`getName__4itemPQ24item11descriptioni`, `getPicName__4itemPciPQ24item11description`, `getProxyMeshId__4itemPQ24item11description`, `getValue__4itemPQ24item11description`, `givePlayerFeat__Fxii`, `goldAlpha`, `goldNumber`, `hermDeltaScale`, `hermiteTime`, `hudGivePlayerGold__FiG6Point3i`, `hudHeaderUni`, `hudInfoMessageShowing__Fv`, `invGrid`, `inventory`, `inventoryAdjustAbility__Fii`, `inventoryAdjustSkill__Ficx`, `inventoryCheckWieldingStatus__FiPQ24item11description`, `inventoryExit__Fiii`, `inventoryFrameDraw__Fv`, `inventoryFrameRun__Fv`, `inventoryGridDropItem__Fiiciiii`, `inventoryLoadState`, `inventoryLoad__Fv`, `inventoryLump`, `inventorySetAnim__FiP14AnimationStatei`, `inventoryUnload__Fv`, `lastFontCharAlpha`, `lastFontColor`, `lerpColor__FUiUif`, `lerpShortRot__Fssfff`, `makeRandomPotion__4itemPQ24item11description`, `msgTimeout`, `nameTags`

That is 33 of the 34 package names. All 33 are absent from `unresolved-imports.json` of the private build above.

Left open: `norrath_inventory_message_handler_word_view`. It is the word `invMsgHandle` (`0x486a88`), which `inventoryInitNet__Fv` already defines in `items-player-inventory-init-net/inventory_init_net.cpp`; a second definition would be a different object and the plan forbids aliases across units. It needs the one-line alias (pattern from `items-player-active-count/active_count.cpp`) added to that file, which belongs to items-player:

```cpp
#if defined(__APPLE__)
asm(".globl _norrath_inventory_message_handler_word_view\n"
    ".set _norrath_inventory_message_handler_word_view, _invMsgHandle\n");
#else
extern "C" int norrath_inventory_message_handler_word_view __attribute__((alias("invMsgHandle")));
#endif
```

## Imports opened

Names these units reference that no unit in the private build defines and that were not in the earlier unresolved list (left unresolved on purpose; none were authored or stubbed): `_4item$prefixTable`, `_4item$suffixTable`, `_shopCheck__Fi`, `animGetAnimComponent__FP14AnimationStateP15AnimationHeader`, `bevel`, `buySellVertSpacing`, `findAndStripGender__4itemPUs`, `getArmorBonus__4itemPQ24item11description`, `getAttributeName__4itemiPc`, `getDamageRangeAll__4itemPQ24item11descriptionPiT2i`, `givePlayerSkill__Fii`, `invHeader`, `invInitSharedInterfaceTextures__Fv`, `inventoryDrawDelay`, `inventoryDrawPlayerColor__Fiii`, `inventoryDrawPlayer__FiiP14AnimationStatei`, `inventoryDraw__Fi`, `inventoryRun__Fi`, `inventoryUnpack__Fiib`, `lightSetSceneLights__FG6Point3iiiiiiiii`, `marble`, `playerGoldx`, `playerGoldy`, `playerSetFeatRank__Fixi`, `playerSetSkillRank__Fiii`, `resetMenu__FP12screenStruct`, `shopUnpackToGrid__Fiii`, `stage`, `statsScreen`.

Names these units also reference that were already open and stay open: `_4item$globalItem`, `__8ItemPropG6Point3PQ24item11descriptioni`, `animState`, `blockAlloc__Fi`, `cdIsBusy__Fv`, `cdLoadAsync__FPcPvT0ii`, `compareToItem__Q24item11descriptionPQ24item11description`, `coreStartLoading__Fv`, `create__4itemPcPQ24item11descriptionii`, `lumpAdd__FPvPCc`, `lumpUnload__FPv`, `matIdent__FP8Matrix34`, `matRot__FP8Matrix34is`, `matTransform__FP6Point3P8Matrix44T0`, `objectFindEmptySpot__FG6Point3iiPc`, `player`, `playerGetSkillRank__Fii`, `resetPlayerAnimations__Fi`, `shopGrid`, `skills`, `strchr`, `syncRandState`, `textureDrawSuperColor__FP8_textureiiiUiiiiicc`, `textureLockCacheBuffer__Fv`, `textureReleaseCacheBuffer__Fv`, `ustrcat__FPUsPc`, `worldSetViewMatricies__FP6Point3P8Matrix34`, `worldViewMatrix`.

`animState` is ambiguous in the image: four file-local objects carry that name (0x007cc5c8, 0x007ccad8, 0x007d0440, 0x007d0910). The inventory code uses the one at 0x007cc5c8 (an array of 264-byte `AnimationState` records indexed by player), so whoever defines the import must pick that object. `invHeader`, `marble`, `stage` and `bevel` are word pointers in the small-data area read by `inventoryFrameDraw__Fv`; `playerGoldx` and `playerGoldy` are 16-byte int arrays at `0x004282d0` and `0x004282b0`.

## Build-plan entries needed

Append these as authored inputs with fresh `input_order` values after the current last entry. `profile` is `authored_cpp` and `kind` is `independently_authored_source`; `header_closure` is empty unless shown. Do not add any `host_check.cpp`. `compiled_body_bytes` are the reviewed EE g++ sizes (every function symbol the object defines). The data units emit no functions. The matching `snapshot.json` `inputs` (with `component`, `private_object_sha256` and the same `emitted_functions`) and `files` additions follow from these; the source hashes are of the files at this branch's last commit.

```json
{"source": "sources/w9-hud-inventory-items-b-data-goldalpha/data_goldAlpha.cpp", "object_name": "data_goldAlpha.o", "sha256": "347160415eb40ee0ff12a4890e53351f056f5bab41b3e06588b7adb2e71207ae"}
{"source": "sources/w9-hud-inventory-items-b-data-goldnumber/data_goldNumber.cpp", "object_name": "data_goldNumber.o", "sha256": "d90af906f6961d97a9a7ad72f10c38759b8e2dbeec2820e8e4dccce88b4d8020"}
{"source": "sources/w9-hud-inventory-items-b-data-hermdeltascale/data_hermDeltaScale.cpp", "object_name": "data_hermDeltaScale.o", "sha256": "8d58f6ba1e456551e60fd0c619e830d924dce1b8f505b99bbab5b8ac2f548159"}
{"source": "sources/w9-hud-inventory-items-b-data-hermitetime/data_hermiteTime.cpp", "object_name": "data_hermiteTime.o", "sha256": "d317b79603ec81d34d44fcfad2f746faeaecc0ad3407c8824b8fcae1ac689458"}
{"source": "sources/w9-hud-inventory-items-b-data-hudheaderuni/data_hudHeaderUni.cpp", "object_name": "data_hudHeaderUni.o", "sha256": "e27910ac0c1bfc1acef13fc3fb178f4d47e03d11611bed5867d48bc3bd9bfaec"}
{"source": "sources/w9-hud-inventory-items-b-data-inventory/data_inventory.cpp", "object_name": "data_inventory.o", "sha256": "c4863e6c756c0cd67dccc34b46db37fc240aecdc030d4f1851705ec5cc4d6038"}
{"source": "sources/w9-hud-inventory-items-b-data-inventoryloadstate/data_inventoryLoadState.cpp", "object_name": "data_inventoryLoadState.o", "sha256": "b1030c5bac5994034b788e58a7631e8c84c0dc731d9d68141ef41ed69eb18fe7"}
{"source": "sources/w9-hud-inventory-items-b-data-inventorylump/data_inventoryLump.cpp", "object_name": "data_inventoryLump.o", "sha256": "7348a970f4313549e9e7e552ce0db47b3a5086f2fcea3a5e488c33353350450f"}
{"source": "sources/w9-hud-inventory-items-b-data-invgrid/data_invGrid.cpp", "object_name": "data_invGrid.o", "sha256": "6265af52a10b452af392e676b75c90a6659bff1a5bb3d9a851e9ccef7b69a946"}
{"source": "sources/w9-hud-inventory-items-b-data-lastfontcharalpha/data_lastFontCharAlpha.cpp", "object_name": "data_lastFontCharAlpha.o", "sha256": "21ef13b908ea198ac0acfbfc59822705e01eeda31a8773d3b5443722f4e46a83"}
{"source": "sources/w9-hud-inventory-items-b-data-lastfontcolor/data_lastFontColor.cpp", "object_name": "data_lastFontColor.o", "sha256": "01214354bbe9f9e1cc3a3503e4c4f1f36f604d4f4657571ca404159b56add472"}
{"source": "sources/w9-hud-inventory-items-b-data-msgtimeout/data_msgTimeout.cpp", "object_name": "data_msgTimeout.o", "sha256": "b765d242c224b9f05fc29b807666cd7885233a5fb423665aaa0e011c31761b34"}
{"source": "sources/w9-hud-inventory-items-b-data-nametags/data_nameTags.cpp", "object_name": "data_nameTags.o", "sha256": "6b165424eb750b3b713947e418f96fa4f82c6f951318a84cd04a7385349d4435"}
{"source": "sources/w9-hud-inventory-items-b-give-player-feat/give_player_feat.cpp", "object_name": "give_player_feat.o", "sha256": "8fd26c18a31da662d61e2f518be33e79c754b804d9828409d88c667d96e2df63", "emitted_functions": [{"compiled_body_bytes": 608, "name": "givePlayerFeat__Fxii"}]}
{"source": "sources/w9-hud-inventory-items-b-hud-give-player-gold/hud_give_player_gold.cpp", "object_name": "hud_give_player_gold.o", "sha256": "6f479c4aa531d7bfffd347d353d2788d4163bf43733176a7f3c8a89e4220592d", "emitted_functions": [{"compiled_body_bytes": 632, "name": "hudGivePlayerGold__FiG6Point3i"}]}
{"source": "sources/w9-hud-inventory-items-b-hud-info-message-showing/hud_info_message_showing.cpp", "object_name": "hud_info_message_showing.o", "sha256": "c8710d4fea96b16b34c55ee8fb575992d889e55ebb09823af8cc413cc73b2b6a", "emitted_functions": [{"compiled_body_bytes": 16, "name": "hudInfoMessageShowing__Fv"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-adjust-ability/inventory_adjust_ability.cpp", "object_name": "inventory_adjust_ability.o", "sha256": "52666e3abd3337c5a4c881d5762af3cddcab39a53a782dbc5bc985e785df2b80", "emitted_functions": [{"compiled_body_bytes": 184, "name": "inventoryAdjustAbility__Fii"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-adjust-skill/inventory_adjust_skill.cpp", "object_name": "inventory_adjust_skill.o", "sha256": "80b7413b37dfc9192c414309f2ddc357dda06e2cdc5b62bd48b1143b70e4fcf0", "emitted_functions": [{"compiled_body_bytes": 392, "name": "inventoryAdjustSkill__Ficx"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-check-wielding-status/inventory_check_wielding_status.cpp", "object_name": "inventory_check_wielding_status.o", "sha256": "4ad51223bba5cf307fd92c44106023a3742539f9dc6e405620823680327ad0d9", "emitted_functions": [{"compiled_body_bytes": 1048, "name": "inventoryCheckWieldingStatus__FiPQ24item11description"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-exit/inventory_exit.cpp", "object_name": "inventory_exit.o", "sha256": "a0820983a4fe49a66fa2af535da45778d337c2ecc1a73ff0a7c8f3753a58e181", "emitted_functions": [{"compiled_body_bytes": 376, "name": "inventoryExit__Fiii"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-frame-draw/inventory_frame_draw.cpp", "object_name": "inventory_frame_draw.o", "sha256": "5e3d37beec55dfd2c9bf931ddc6d0d832bb3a2241c7bfbeb53f95ed0ca1dc28c", "emitted_functions": [{"compiled_body_bytes": 1160, "name": "inventoryFrameDraw__Fv"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-frame-run/inventory_frame_run.cpp", "object_name": "inventory_frame_run.o", "sha256": "204834522868159c534a4fc3525b60f1a603c5414095274e13b941bcc4b165ea", "emitted_functions": [{"compiled_body_bytes": 160, "name": "inventoryFrameRun__Fv"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-grid-drop-item/inventory_grid_drop_item.cpp", "object_name": "inventory_grid_drop_item.o", "sha256": "fa9b6dec668ecdf724b4220638a603b61bfba6400130c8d4d2071c6ecb0c32f0", "header_closure": ["sources/inventory-items-record/record.hpp"], "emitted_functions": [{"compiled_body_bytes": 1820, "name": "inventoryGridDropItem__Fiiciiii"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-load/inventory_load.cpp", "object_name": "inventory_load.o", "sha256": "80306037a91566a6295a2fc2dbfcf0e609740766281205e95ed36bbc4b325cb9", "emitted_functions": [{"compiled_body_bytes": 428, "name": "inventoryLoad__Fv"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-set-anim/inventory_set_anim.cpp", "object_name": "inventory_set_anim.o", "sha256": "33e2fe4f833586aa402e2ce046d282fa3402597fb4fa64aff5d706b556e053ba", "emitted_functions": [{"compiled_body_bytes": 372, "name": "inventorySetAnim__FiP14AnimationStatei"}]}
{"source": "sources/w9-hud-inventory-items-b-inventory-unload/inventory_unload.cpp", "object_name": "inventory_unload.o", "sha256": "175ec2c3b50b64fc278e3a3b8991668ad3a7d3e600ec9bce79a78533d95053cc", "emitted_functions": [{"compiled_body_bytes": 140, "name": "inventoryUnload__Fv"}]}
{"source": "sources/w9-hud-inventory-items-b-item-get-name/item_get_name.cpp", "object_name": "item_get_name.o", "sha256": "d9352d381000d9a05b0b3df1f90b6b552099eebcfc116ca39475ab509f3af3eb", "emitted_functions": [{"compiled_body_bytes": 1132, "name": "getName__4itemPQ24item11descriptioni"}]}
{"source": "sources/w9-hud-inventory-items-b-item-get-pic-name/item_get_pic_name.cpp", "object_name": "item_get_pic_name.o", "sha256": "37eb46050a766a56147b2c33e5cd1b4900eb402a8afcb365365e981d681f8bd9", "emitted_functions": [{"compiled_body_bytes": 1320, "name": "getPicName__4itemPciPQ24item11description"}]}
{"source": "sources/w9-hud-inventory-items-b-item-get-proxy-mesh-id/item_get_proxy_mesh_id.cpp", "object_name": "item_get_proxy_mesh_id.o", "sha256": "2a355ca0cab0111d238a4f17c263094cff4eb8067722f810d8bb6a6c6f1bfbae", "emitted_functions": [{"compiled_body_bytes": 48, "name": "getProxyMeshId__4itemPQ24item11description"}]}
{"source": "sources/w9-hud-inventory-items-b-item-get-value/item_get_value.cpp", "object_name": "item_get_value.o", "sha256": "2bba4715f766995ed4fd522fe1e1eb5d69c3595f5dc80e51b310521f5d9bf60c", "emitted_functions": [{"compiled_body_bytes": 5972, "name": "getValue__4itemPQ24item11description"}]}
{"source": "sources/w9-hud-inventory-items-b-item-make-random-potion/item_make_random_potion.cpp", "object_name": "item_make_random_potion.o", "sha256": "d84df70a62f21d96d1a26c6d260b038537b50471a143eddea9817c0efe0ab46a", "emitted_functions": [{"compiled_body_bytes": 404, "name": "makeRandomPotion__4itemPQ24item11description"}]}
{"source": "sources/w9-hud-inventory-items-b-lerp-color/lerp_color.cpp", "object_name": "lerp_color.o", "sha256": "2de9977e22f10e41590414d967faffd48d3a02eb1e0c886d464d781f09b5d419", "emitted_functions": [{"compiled_body_bytes": 572, "name": "lerpColor__FUiUif"}]}
{"source": "sources/w9-hud-inventory-items-b-lerp-short-rot/lerp_short_rot.cpp", "object_name": "lerp_short_rot.o", "sha256": "b133e967a234cafc8ef6ad71afc88c3df596b15cb4b3d1b927122bd0a3271017", "emitted_functions": [{"compiled_body_bytes": 176, "name": "lerpShortRot__Fssfff"}]}
{"source": "sources/w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp", "object_name": "norrath_inventory_exit_sound_resource_view.o", "sha256": "a690c5595078ef6f4b3bd0078aff01b135f41e6d2776bb01a988bb30dbc0c800"}
```
