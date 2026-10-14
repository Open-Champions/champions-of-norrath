# Inventory and items

Package `inventory-items`, branch `agents/inventory-items`, shared base `20a5902`.
This is a takeover: the previous worker left no commits and only an uncommitted
`queryInventoryItem__FPci` draft (a bytes helper header plus a host test). That
draft was reviewed against the image, rewritten on the shared player-list layout
(`inventory-items-record/record.hpp`), and committed first.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses
below are that file's symbol values and instruction operands. No game bytes or
disassembly listings are committed; all text is my own description.

Every function was compiled with the reviewed EE g++ `authored_cpp` flags and has
a host check under its own directory (`host_check.cpp`, `host_test.cpp` or
`host/check.cpp`; these are not plan inputs). The checks return non-zero when a
field, call order, argument or result is wrong, and the larger ones (removal, the shop
start, Gold draw, ItemProp draw, query) were also run against mutated copies of the
source to confirm they fail. The 27 units were also built in
a private copy of the plan and snapshot kept outside git (see the last sections).
Compiled bodies differ in size from the image extents. These are behavior
reconstructions, not original-byte matches. Nothing here ran on the EE or as
gameplay.

## Functions

| source (under `sources/`) | symbol | image | compiled bytes | sha256 of source |
|---|---|---|---|---|
| `inventory-items-clone-gold/clone.cpp` | `clone__4Gold` | `0x115368`, 368 bytes | 680 | `68abcad1e87f325a8b471a360133d7a12438ba678648bde163d14ca7069dde7f` |
| `inventory-items-clone-itemprop/clone.cpp` | `clone__8ItemProp` | `0x1148f0`, 680 bytes | 984 | `62236c1bdb03a89adb0b4b054d5a90fcbaa685163a17e401bc28f6501559ee28` |
| `inventory-items-dtor-gold/dtor.cpp` | `_$_4Gold` | `0x30dee8`, 116 bytes | 144 | `6199e67ebfdf962a7f4f111a9d53e8b22f02996bb2f481daeec3e0fce82dbd6b` |
| `inventory-items-dtor-itemprop/dtor.cpp` | `_$_8ItemProp` | `0x30dbf0`, 100 bytes | 112 | `0de709b41b0b791ca09dec638d3bd1505612d5f1068989fb31f7ac26073565a5` |
| `inventory-items-get-index/get_index.cpp` | `inventoryGetItemIndex__FiPcPiT2` | `0x1b1d78`, 212 bytes | 224 | `25c64329f474da9313507ca3386d3c77cbdb3256227de4340040447f358deb4b` |
| `inventory-items-msg-callback/msg_callback.cpp` | `invMsgCallback__FPUci` | `0x1ac6c8`, 476 bytes | 320 | `0c7ef3b87ef22406e6523965e07fea8c8d610366b1824b93556b73e06810bc70` |
| `inventory-items-msg-draw-gold/msg_draw.cpp` | `msg_draw__4Gold` | `0x14ad38`, 1160 bytes | 1292 | `28a9bd693da1d7079411da3cddbd1f679cb8339a999ba941dc842897a1399a09` |
| `inventory-items-msg-draw-itemprop/msg_draw.cpp` | `msg_draw__8ItemProp` | `0x14c1f0`, 544 bytes | 640 | `2006c0f1f74f8b7f254af01e8d4feb119395058c551a69b6ae4bf6ed0b56ba6a` |
| `inventory-items-msg-load-gold/msg_load.cpp` | `msg_load__4GoldPc` | `0x154d98`, 24 bytes | 44 | `b247a6c116d8386c93bb0ce8d09afbc366a30674e2ea4b0391c973582aa28010` |
| `inventory-items-msg-load-itemprop/msg_load.cpp` | `msg_load__8ItemPropPc` | `0x155808`, 280 bytes | 296 | `09362d02797cca1f50867b39fc663b7d3c5746f0e45c901d4101a9188e06db57` |
| `inventory-items-msg-poke-itemprop/msg_poke.cpp` | `msg_poke__8ItemPropi` | `0x1559c8`, 240 bytes | 252 | `31f923d4aaddd58545c3852b6c9f70f9747483e9c0b2995a5445e27d712d238c` |
| `inventory-items-msg-run-gold/msg_run.cpp` | `msg_run__4Gold` | `0x154e68`, 152 bytes | 200 | `406e8c78c995df96a4ce0d589d7c4be2a677b4b4c94ef889c882eeea862f0b7c` |
| `inventory-items-msg-save-gold/msg_save.cpp` | `msg_save__4GoldPc` | `0x154d80`, 24 bytes | 44 | `e252f2bc99320aa8d8e6b56a512827d9b190d97277598e6c56dfc6697f8bf7eb` |
| `inventory-items-msg-save-itemprop/msg_save.cpp` | `msg_save__8ItemPropPc` | `0x155750`, 180 bytes | 124 | `0192f17c54d91f1f136779c1599eb3c584631cbd5ca2e7f7c75f374abc5d365f` |
| `inventory-items-poke-name-itemprop/poke_name.cpp` | `msg_getPokeName__8ItemPropPUsi` | `0x155920`, 168 bytes | 212 | `7dc80d00cf2748c27776722f359e54124d84de390e9e3ccdb131b25739bac294` |
| `inventory-items-query/query.cpp` | `queryInventoryItem__FPci` | `0x1b1b68`, 184 bytes | 196 | `1d6310d6ea6ab705b73d8d00889f33a82d1563b2a081175a621042de868c8841` |
| `inventory-items-remove/remove.cpp` | `removeItemFromInventory__Fiiii` | `0x1ae290`, 1096 bytes | 964 | `e304c2fe4137030b231f20f888709a29de83871c612d282c5aa0e69c06aafaa9` |
| `inventory-items-shop-msg-callback/shop_msg_callback.cpp` | `shopMsgCallback__FPUci` | `0x1b2028`, 544 bytes | 532 | `e70f0fcc81337154b365cf1271b983b5be283427509e3e3403a51a257a93622d` |
| `inventory-items-shop-start/shop_start.cpp` | `shopStart__Fiii` | `0x1b3b20`, 2944 bytes | 2356 | `5fd2678841c4057852e90541bb10b799a8df9f443fa8cae3507ed096978ed20d` |
| `inventory-items-tf-gold/typeinfo.cpp` | `__tf4Gold` | `0x30df60`, 76 bytes | 92 | `6e99df7c3609ac0c0bb5d7cbbd0ad4aaa805b19aef08725b4922142790565cea` |
| `inventory-items-tf-itemprop/typeinfo.cpp` | `__tf8ItemProp` | `0x30dc58`, 132 bytes | 192 | `2c3f9aff30809d54cffd62d4ae7c77f835f0560dc86ae07da3186194713b76b0` |
| `inventory-items-tf-ptr-itemprop/typeinfo.cpp` | `__tfP8ItemProp` | `0x312d80`, 80 bytes | 92 | `f3565bd21533c125b83dee2ce800621abceea0559dd9d25cf26f1eb65e407d4d` |

`shopStart__Fiii` also emits one local helper (`fill`, 648 bytes) that is not a
plan symbol.

## Data objects

| source | symbol | what it is |
|---|---|---|
| `inventory-items-views/norrath_inventory_ability_sound_lump_view.cpp` | `norrath_inventory_ability_sound_lump_view` | 8 bytes at `0x484828`: the string `fx.lmp` and a zero pad. `inventoryAdjustAbility__Fii` passes it as the lump name. |
| `inventory-items-views/norrath_inventory_ability_sound_resource_view.cpp` | `norrath_inventory_ability_sound_resource_view` | 19 bytes at `0x466548`: `menu_char_incr.anm` and its terminator. |
| `inventory-items-views/norrath_inventory_reset_resource_names_view.cpp` | `norrath_inventory_reset_resource_names_view` | 96 bytes at `0x464b48`: six 16-byte name slots (`dhs_idled.anm`, `dha_idled.anm`, `l_idled.anm`, `b_idled.anm`, `dw_idled.anm`, `ohw_idled.anm`). `resetPlayerAnimations__Fi` loads the six slot addresses in steps of 16. |
| `inventory-items-shop-running/shop_running.cpp` | `shopRunning` | the 4-byte word at `0x483f2c`, initial value 0. |
| `inventory-items-gold-resources/gold_resources.cpp` | `goldModel`, `goldTex` | 16 zero bytes each at `0x600d30` and `0x600d40` (the labels around them are 16 bytes apart). Defined so the Gold draw does not open them. |

The three string views are byte-compared with the image in the commit that added
them. `norrath_ti_4Gold`, `norrath_ti_8ItemProp` and `norrath_ti_P8ItemProp` (12
zero bytes each, originally COMMON at `0x8eaeb8`, `0x8eade8`, `0x8eb2a8`) are
defined by the three typeinfo units, following the existing `norrath_ti_*`
naming. `halo` in the ItemProp draw unit is a file-local 8-byte record
(`ff ff ff ff` then the float 409.0, matching the data at `0x483c30`).

## Observed

- Player records are 23984 bytes apart in `player`. At record+328 there are three
  lists of 4616 bytes: 64 slots of 72 bytes, a signed count at list+4608, and four
  unread bytes. Slot+0 points to a description whose first word points to the item
  name and whose signed byte at +28 is a type; slot+4 is a signed halfword
  quantity; slot+34 is another signed halfword. All of this is in `record.hpp`.
- `queryInventoryItem` sums the +4 halfword over every slot of all three lists
  whose name equals the argument (`strcmp`). `inventoryGetItemIndex` runs `strstr`
  with the caller's text as the haystack and the item name as the needle; it
  stores the slot number and list number of the last match (the slot output is
  first set to -1; the list output is untouched without a match).
- `removeItemFromInventory` clears the equipped-word table entries that hold the
  slot address (words at record+21896..21968), shifts later slots down one place
  while rewriting those words, adjusts the +34 halfword of every shifted slot,
  scans the shop table at `invGrid` for local players, then zeroes the unused
  tail, drops the count, and calls `playerRecomputeAbilityModifiers`. Details are
  in the unit's header comment.
- `invMsgCallback` is a switch over 11 message types with a code-address table at
  `0x466300`; all cases tail call a handler except types 3 (nothing) and 9 (one
  byte store). `shopMsgCallback` has two types: 0 buys, 1 sells. The sell path
  computes a gold gain with the float `0x3e4ccccc` (not 0.2f) and multiplies by 20
  for type 8 or a name containing `spear`.
- `shopStart__Fiii` with a zero second argument opens the screen; non-zero builds
  three stock lists (22, 17 and 10 random items plus fixed items; random draws use
  the LCG step `seed*1664525+1013904223` on `syncRandState`) and insertion-sorts
  list 0. The duplicate walk for lists 1 and 2 starts at `shopList + 9232*L`, not
  at the list being built (observed addressing; it reads other memory).
- Gold: `msg_draw__4Gold` draws a model, a main sprite and five sparkles that are
  respawned inside the unit ball using the same LCG on the word at object+68.
  `msg_run__4Gold` plays `inv_drop_coin.anm` once (when the stamp at +80 equals
  `frameNm - 18`) and removes the object when the animation flag bit is set.
- ItemProp: `msg_poke__8ItemPropi` calls the vtable slot at +8 with flag 3 (a
  deleting-destructor call) once the item is picked up; the destructor
  `_$_8ItemProp` stores the Prop vtable, not the ItemProp one.
- `__tf8ItemProp` initializes the `Prop` typeinfo inline (it does not call a
  `__tf4Prop`).

## Inferred

- The three lists in a player record are item lists of some kind; their meaning
  and the Gold/ItemProp roles are not established, so no field was named.
- `compareToItem` takes two 72-byte slot-shaped records (the mangled name says
  `item::description`), and the +34 halfword is a slot index maintained for the
  shop grid.
- The tail-called handlers in `invMsgCallback` receive packet fields at natural
  alignment; the layouts in the unit are inferred from the load offsets.
- The potion table index is `11 - 3*d - group` for gameDifficulty `d`; the image
  reads `(base - 12d)/4` words from the table, so only `d` of 1..3 stay inside
  the 9-entry tables.

## Checks performed

- Each unit compiles with the reviewed toolchain (`authored_cpp` flags).
- Host checks, one `c++ -std=c++17 -ffp-contract=off <host file> [source.cpp]`
  build and run each (units whose host file includes the source are built alone);
  all pass at `-O0` and `-O2`.
- Private partial build: the 27 units were added to a private copy of
  `build-plan.json` and `snapshot.json` outside git. `build_partial.py` completed
  (935 inputs, 788 function bodies, 976 unresolved) and the unresolved-import list
  diff against the shared base (950) closed 26 of the 27 package names and opened the
  names listed below.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.

## Imports closed

`_$_4Gold`, `_$_8ItemProp`, `__tf4Gold`, `__tf8ItemProp`, `__tfP8ItemProp`,
`clone__4Gold`, `clone__8ItemProp`, `invMsgCallback__FPUci`,
`inventoryGetItemIndex__FiPcPiT2`, `msg_draw__4Gold`, `msg_draw__8ItemProp`,
`msg_getPokeName__8ItemPropPUsi`, `msg_load__4GoldPc`, `msg_load__8ItemPropPc`,
`msg_poke__8ItemPropi`, `msg_run__4Gold`, `msg_save__4GoldPc`,
`msg_save__8ItemPropPc`, `norrath_inventory_ability_sound_lump_view`,
`norrath_inventory_ability_sound_resource_view`,
`norrath_inventory_reset_resource_names_view`, `queryInventoryItem__FPci`,
`removeItemFromInventory__Fiiii`, `shopMsgCallback__FPUci`, `shopRunning`,
`shopStart__Fiii`. That is 26 of the 27 package names.

Left open: `norrath_inventory_message_handler_word_view`. Observed: it is the
word `invMsgHandle` (`0x486a88`), which `inventoryInitNet__Fv` already defines in
`items-player-inventory-init-net/inventory_init_net.cpp`. A second definition
would be a different object, and the build forbids symbol aliases across units.
It needs a one-line alias in that same translation unit, using the pattern in
`items-player-active-count/active_count.cpp`:

```cpp
#if defined(__APPLE__)
asm(".globl _norrath_inventory_message_handler_word_view\n"
    ".set _norrath_inventory_message_handler_word_view, _invMsgHandle\n");
#else
extern "C" int norrath_inventory_message_handler_word_view __attribute__((alias("invMsgHandle")));
#endif
```

That file belongs to items-player, so I did not change it; the integrator can
apply this (and re-hash the plan entry) or have items-player do it.

## Imports opened

Original ELF names, left unresolved on purpose (none were authored or stubbed):
`P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `Pop__6Camera`, `Push__6Camera`,
`SetMode__6Camerai`, `_$_14AnimationState`,
`__14AnimationStateRC14AnimationState`,
`animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`, `animState`,
`animStatePtr`, `coins`, `compareToItem__Q24item11descriptionPQ24item11description`,
`createRandom__4itemPQ24item11descriptioniiii`,
`create__4itemPcPQ24item11descriptionii`, `drinkPotion__6Playeriiii`,
`engineSetBlur__Fi`, `equipItem__FiPQ24item11descriptioni`,
`getName__4itemPQ24item11descriptioni`, `getValue__4itemPQ24item11description`,
`get__11ResourceMap`, `hudClearInfoMessage__Fv`, `interfaceOn`, `invGrid`,
`inventory`, `inventoryAdjustAbility__Fii`, `inventoryAdjustSkill__Ficx`,
`inventoryExit__Fiii`, `inventoryGridDropItem__Fiiciiii`,
`modelSetHalo__FP7HaloDefi`, `norrath_rtti_ptr` (the image's `__rtti_ptr`,
0x2fe658, 32 bytes), `norrath_ti_4Prop` (the image's `__ti4Prop`, 12 bytes at
0x8eadb8), `pack__4itemPQ24item11description`, `playerLogInventory__Fi`,
`playerMaybeGetItem__FiPQ24item11description`, `playerNetDead__Fv`,
`playerRecomputeAbilityModifiers__Fv`, `resetPlayerAnimations__Fi`,
`setupHalo__4itemPQ24item11descriptionf`, `shop`, `shopBuy__FcPQ24item11description`,
`shopDisableMouse`, `shopFrame__Fv`, `shopIntro`, `shopItemSetAttributes__FiiPcPQ24item11description`,
`shopList`, `shopMouseDecay`, `shopTick`, `startFrame`,
`unequipItem__FiPQ24item11description`, `unpack__4itemPQ24item11description`,
`ustrcat__FPUsPc`, `ustrcat__FPUsT0`, `waitingForStreamSpool`.
That is 52 names; the shared base had 950 unresolved and the private build 976.

Used by this package and not opened by it. Already unresolved in the shared base
(still unresolved): `drawFrameNm`, `gameNoDraw`, `gamePause`, `isin__Fi`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `modelResetOptionalParams__Fv`, `msg_load__4PropPc`, `msg_save__4PropPc`, `norrath_list_head_copy_construct`, `norrath_rtti_si`, `objectDrawEnable`, `objectRemoveFromRunList__FP10GameObject`, `player`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `syncRandState`, `textGetString__FPc`.
Defined elsewhere in the shared partial build (or by another unit of this package):
`engineAddTask__FPFv_viPci`, `engineTaskActive__FPc`, `floorDrawEnable`, `frameNm`, `gameCam`, `gameDifficulty`, `haloTexture1`, `haloTexture2`, `lumpFindResource__FPcT0`, `lumpLoad__FPc`, `msgHudSetPlayerBusy__Fcc`, `netGetLocalPlayerNm__Fi`, `netNetworkMode`, `netPlayerIsLocal__Fi`, `nmPlayers`, `norrath_block_alloc`, `norrath_block_free`, `norrath_factory_strstr`, `norrath_game_object_vtable_view`, `norrath_player_view_44cd48`, `norrath_player_view_44cef8`, `norrath_player_view_44d018`, `norrath_ti_10GameObject`, `norrath_vt_dtor_10GameObject`, `norrath_vt_tf_10GameObject`, `sprintf`, `strcmp`.

Not mine but seen: `msg_collision__4GoldP10GameObject` is an unresolved import
used by the Gold vtable view and belongs to nobody in this package list.

## Build-plan entries needed

Append these as `independently_authored_source` inputs (profile `authored_cpp`).
`input_order` is assigned by the integrator; the private build used 910 to 936.
`emitted_functions` goes in `snapshot.json`; the numbers are this machine's
compiles. Put every file hash below under `files` in both the plan and the
snapshot. `default_authored_inputs` grows by 27. Do not add any `host_*` file.

Inputs (one JSON object per line; `emitted_functions` is for the snapshot only):

```json
{"emitted_functions": [{"compiled_body_bytes": 680, "name": "clone__4Gold"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_clone_gold_clone.o", "profile": "authored_cpp", "sha256": "68abcad1e87f325a8b471a360133d7a12438ba678648bde163d14ca7069dde7f", "source": "sources/inventory-items-clone-gold/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 984, "name": "clone__8ItemProp"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_clone_itemprop_clone.o", "profile": "authored_cpp", "sha256": "62236c1bdb03a89adb0b4b054d5a90fcbaa685163a17e401bc28f6501559ee28", "source": "sources/inventory-items-clone-itemprop/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "_$_4Gold"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_dtor_gold_dtor.o", "profile": "authored_cpp", "sha256": "6199e67ebfdf962a7f4f111a9d53e8b22f02996bb2f481daeec3e0fce82dbd6b", "source": "sources/inventory-items-dtor-gold/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_8ItemProp"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_dtor_itemprop_dtor.o", "profile": "authored_cpp", "sha256": "0de709b41b0b791ca09dec638d3bd1505612d5f1068989fb31f7ac26073565a5", "source": "sources/inventory-items-dtor-itemprop/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 224, "name": "inventoryGetItemIndex__FiPcPiT2"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_get_index_get_index.o", "profile": "authored_cpp", "sha256": "25c64329f474da9313507ca3386d3c77cbdb3256227de4340040447f358deb4b", "source": "sources/inventory-items-get-index/get_index.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_gold_resources_gold_resources.o", "profile": "authored_cpp", "sha256": "f1d40629e10f49ec5adb57b4f5770458164889904aef8877fe9ccd524a548f79", "source": "sources/inventory-items-gold-resources/gold_resources.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "invMsgCallback__FPUci"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_callback_msg_callback.o", "profile": "authored_cpp", "sha256": "0c7ef3b87ef22406e6523965e07fea8c8d610366b1824b93556b73e06810bc70", "source": "sources/inventory-items-msg-callback/msg_callback.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1292, "name": "msg_draw__4Gold"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_draw_gold_msg_draw.o", "profile": "authored_cpp", "sha256": "28a9bd693da1d7079411da3cddbd1f679cb8339a999ba941dc842897a1399a09", "source": "sources/inventory-items-msg-draw-gold/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 640, "name": "msg_draw__8ItemProp"}], "header_closure": ["sources/inventory-items-object/object.hpp", "sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_draw_itemprop_msg_draw.o", "profile": "authored_cpp", "sha256": "2006c0f1f74f8b7f254af01e8d4feb119395058c551a69b6ae4bf6ed0b56ba6a", "source": "sources/inventory-items-msg-draw-itemprop/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "msg_load__4GoldPc"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_load_gold_msg_load.o", "profile": "authored_cpp", "sha256": "b247a6c116d8386c93bb0ce8d09afbc366a30674e2ea4b0391c973582aa28010", "source": "sources/inventory-items-msg-load-gold/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "msg_load__8ItemPropPc"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_load_itemprop_msg_load.o", "profile": "authored_cpp", "sha256": "09362d02797cca1f50867b39fc663b7d3c5746f0e45c901d4101a9188e06db57", "source": "sources/inventory-items-msg-load-itemprop/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 252, "name": "msg_poke__8ItemPropi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_msg_poke_itemprop_msg_poke.o", "profile": "authored_cpp", "sha256": "31f923d4aaddd58545c3852b6c9f70f9747483e9c0b2995a5445e27d712d238c", "source": "sources/inventory-items-msg-poke-itemprop/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "msg_run__4Gold"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_msg_run_gold_msg_run.o", "profile": "authored_cpp", "sha256": "406e8c78c995df96a4ce0d589d7c4be2a677b4b4c94ef889c882eeea862f0b7c", "source": "sources/inventory-items-msg-run-gold/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "msg_save__4GoldPc"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_save_gold_msg_save.o", "profile": "authored_cpp", "sha256": "e252f2bc99320aa8d8e6b56a512827d9b190d97277598e6c56dfc6697f8bf7eb", "source": "sources/inventory-items-msg-save-gold/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "msg_save__8ItemPropPc"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_msg_save_itemprop_msg_save.o", "profile": "authored_cpp", "sha256": "0192f17c54d91f1f136779c1599eb3c584631cbd5ca2e7f7c75f374abc5d365f", "source": "sources/inventory-items-msg-save-itemprop/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 212, "name": "msg_getPokeName__8ItemPropPUsi"}], "header_closure": ["sources/inventory-items-object/object.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_poke_name_itemprop_poke_name.o", "profile": "authored_cpp", "sha256": "7dc80d00cf2748c27776722f359e54124d84de390e9e3ccdb131b25739bac294", "source": "sources/inventory-items-poke-name-itemprop/poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 196, "name": "queryInventoryItem__FPci"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_query_query.o", "profile": "authored_cpp", "sha256": "1d6310d6ea6ab705b73d8d00889f33a82d1563b2a081175a621042de868c8841", "source": "sources/inventory-items-query/query.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 964, "name": "removeItemFromInventory__Fiiii"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_remove_remove.o", "profile": "authored_cpp", "sha256": "e304c2fe4137030b231f20f888709a29de83871c612d282c5aa0e69c06aafaa9", "source": "sources/inventory-items-remove/remove.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 532, "name": "shopMsgCallback__FPUci"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_shop_msg_callback_shop_msg_callback.o", "profile": "authored_cpp", "sha256": "e70f0fcc81337154b365cf1271b983b5be283427509e3e3403a51a257a93622d", "source": "sources/inventory-items-shop-msg-callback/shop_msg_callback.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_shop_running_shop_running.o", "profile": "authored_cpp", "sha256": "29c4c4442f260a2b841cc6a6cbd87374dc6f827261eaa52e336634f84463344f", "source": "sources/inventory-items-shop-running/shop_running.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 648, "name": "_ZN12_GLOBAL__N_1L4fillEPN15inventory_items4ListEPKNS0_4SlotERKNS_5StageEiff"}, {"compiled_body_bytes": 2356, "name": "shopStart__Fiii"}], "header_closure": ["sources/inventory-items-object/object.hpp", "sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "inventory_items_shop_start_shop_start.o", "profile": "authored_cpp", "sha256": "5fd2678841c4057852e90541bb10b799a8df9f443fa8cae3507ed096978ed20d", "source": "sources/inventory-items-shop-start/shop_start.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf4Gold"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_tf_gold_typeinfo.o", "profile": "authored_cpp", "sha256": "6e99df7c3609ac0c0bb5d7cbbd0ad4aaa805b19aef08725b4922142790565cea", "source": "sources/inventory-items-tf-gold/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "__tf8ItemProp"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_tf_itemprop_typeinfo.o", "profile": "authored_cpp", "sha256": "2c3f9aff30809d54cffd62d4ae7c77f835f0560dc86ae07da3186194713b76b0", "source": "sources/inventory-items-tf-itemprop/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tfP8ItemProp"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_tf_ptr_itemprop_typeinfo.o", "profile": "authored_cpp", "sha256": "f3565bd21533c125b83dee2ce800621abceea0559dd9d25cf26f1eb65e407d4d", "source": "sources/inventory-items-tf-ptr-itemprop/typeinfo.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_views_norrath_inventory_ability_sound_lump_view.o", "profile": "authored_cpp", "sha256": "180849e5b2655235861006fc88222c977c8c74ea80a475fa19a6409fc5a73e0b", "source": "sources/inventory-items-views/norrath_inventory_ability_sound_lump_view.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_views_norrath_inventory_ability_sound_resource_view.o", "profile": "authored_cpp", "sha256": "ada6a7a4162ad0a5bc5afe9cb9e39832133ce19c26acd5cf49d046058c4fe27b", "source": "sources/inventory-items-views/norrath_inventory_ability_sound_resource_view.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "inventory_items_views_norrath_inventory_reset_resource_names_view.o", "profile": "authored_cpp", "sha256": "b3f2c12f9987b8d91a0e76b4345ede12b1ba2a662697f04de7984e01e1de0ecb", "source": "sources/inventory-items-views/norrath_inventory_reset_resource_names_view.cpp"}
```

File hashes (headers and sources, for `files`):

```json
{
 "sources/inventory-items-clone-gold/clone.cpp": "68abcad1e87f325a8b471a360133d7a12438ba678648bde163d14ca7069dde7f",
 "sources/inventory-items-clone-itemprop/clone.cpp": "62236c1bdb03a89adb0b4b054d5a90fcbaa685163a17e401bc28f6501559ee28",
 "sources/inventory-items-dtor-gold/dtor.cpp": "6199e67ebfdf962a7f4f111a9d53e8b22f02996bb2f481daeec3e0fce82dbd6b",
 "sources/inventory-items-dtor-itemprop/dtor.cpp": "0de709b41b0b791ca09dec638d3bd1505612d5f1068989fb31f7ac26073565a5",
 "sources/inventory-items-get-index/get_index.cpp": "25c64329f474da9313507ca3386d3c77cbdb3256227de4340040447f358deb4b",
 "sources/inventory-items-gold-resources/gold_resources.cpp": "f1d40629e10f49ec5adb57b4f5770458164889904aef8877fe9ccd524a548f79",
 "sources/inventory-items-msg-callback/msg_callback.cpp": "0c7ef3b87ef22406e6523965e07fea8c8d610366b1824b93556b73e06810bc70",
 "sources/inventory-items-msg-draw-gold/msg_draw.cpp": "28a9bd693da1d7079411da3cddbd1f679cb8339a999ba941dc842897a1399a09",
 "sources/inventory-items-msg-draw-itemprop/msg_draw.cpp": "2006c0f1f74f8b7f254af01e8d4feb119395058c551a69b6ae4bf6ed0b56ba6a",
 "sources/inventory-items-msg-load-gold/msg_load.cpp": "b247a6c116d8386c93bb0ce8d09afbc366a30674e2ea4b0391c973582aa28010",
 "sources/inventory-items-msg-load-itemprop/msg_load.cpp": "09362d02797cca1f50867b39fc663b7d3c5746f0e45c901d4101a9188e06db57",
 "sources/inventory-items-msg-poke-itemprop/msg_poke.cpp": "31f923d4aaddd58545c3852b6c9f70f9747483e9c0b2995a5445e27d712d238c",
 "sources/inventory-items-msg-run-gold/msg_run.cpp": "406e8c78c995df96a4ce0d589d7c4be2a677b4b4c94ef889c882eeea862f0b7c",
 "sources/inventory-items-msg-save-gold/msg_save.cpp": "e252f2bc99320aa8d8e6b56a512827d9b190d97277598e6c56dfc6697f8bf7eb",
 "sources/inventory-items-msg-save-itemprop/msg_save.cpp": "0192f17c54d91f1f136779c1599eb3c584631cbd5ca2e7f7c75f374abc5d365f",
 "sources/inventory-items-object/object.hpp": "ea2808ea22197b6d9d415e821d0bcfa77d0ddc742eab8e9409e9bbef5b3f20cd",
 "sources/inventory-items-poke-name-itemprop/poke_name.cpp": "7dc80d00cf2748c27776722f359e54124d84de390e9e3ccdb131b25739bac294",
 "sources/inventory-items-query/query.cpp": "1d6310d6ea6ab705b73d8d00889f33a82d1563b2a081175a621042de868c8841",
 "sources/inventory-items-record/record.hpp": "40adb98656b4ba29dd1dd1e2262463311649c7ff19b55302a50544c1d24d0dc8",
 "sources/inventory-items-remove/remove.cpp": "e304c2fe4137030b231f20f888709a29de83871c612d282c5aa0e69c06aafaa9",
 "sources/inventory-items-shop-msg-callback/shop_msg_callback.cpp": "e70f0fcc81337154b365cf1271b983b5be283427509e3e3403a51a257a93622d",
 "sources/inventory-items-shop-running/shop_running.cpp": "29c4c4442f260a2b841cc6a6cbd87374dc6f827261eaa52e336634f84463344f",
 "sources/inventory-items-shop-start/shop_start.cpp": "5fd2678841c4057852e90541bb10b799a8df9f443fa8cae3507ed096978ed20d",
 "sources/inventory-items-tf-gold/typeinfo.cpp": "6e99df7c3609ac0c0bb5d7cbbd0ad4aaa805b19aef08725b4922142790565cea",
 "sources/inventory-items-tf-itemprop/typeinfo.cpp": "2c3f9aff30809d54cffd62d4ae7c77f835f0560dc86ae07da3186194713b76b0",
 "sources/inventory-items-tf-ptr-itemprop/typeinfo.cpp": "f3565bd21533c125b83dee2ce800621abceea0559dd9d25cf26f1eb65e407d4d",
 "sources/inventory-items-views/norrath_inventory_ability_sound_lump_view.cpp": "180849e5b2655235861006fc88222c977c8c74ea80a475fa19a6409fc5a73e0b",
 "sources/inventory-items-views/norrath_inventory_ability_sound_resource_view.cpp": "ada6a7a4162ad0a5bc5afe9cb9e39832133ce19c26acd5cf49d046058c4fe27b",
 "sources/inventory-items-views/norrath_inventory_reset_resource_names_view.cpp": "b3f2c12f9987b8d91a0e76b4345ede12b1ba2a662697f04de7984e01e1de0ecb"
}
```

## Authored but never executed

All 22 functions and the data objects above were compiled and exercised only by
the host checks. None has run on the EE, in the shared link as an executable, or
in gameplay. Float arithmetic in the sell callback, the Gold and ItemProp draws
and the stock builder is reproduced with the image's constants and operation
order but was checked on the host's single-precision unit, not the EE's. The
64-bit values the image loads whole (the flag words at ItemProp+336, +232 and the
Gold mask) are passed as `unsigned long long`; the callee was not seen.
`shopStart__Fiii`'s stock and screen paths, and every callee it reaches, were
never run against real data.

## Unknown

- What the three player lists are, what the equipped-word table at record+21896
  holds, and the meaning of the fields at slot+4, +34, +13 and +33.
- What `gameDifficulty` values are legal; values outside 1..3 make the potion
  index read outside the 9-entry tables (the image would read the following
  `combineTable` data; the reconstruction would index outside its own arrays).
- Why the stock builder's duplicate walk for lists 1 and 2 starts at
  `shopList + 9232*L`; the reconstruction keeps the observed addresses.
- The meaning of the `invGrid` table (9 columns by 75 rows of 72-byte records, per
  group and list), the shop record fields at `shop`, and the `inventory` array at
  `0x427288`.
- The sizes of the packets `invMsgCallback` and `shopMsgCallback` receive; only
  the offsets they read are known.
- The local buffer size in `msg_getPokeName` (at least 208 bytes of frame was
  used) and whether `modelDraw`'s `Ul` argument is 32 or 64 bits in its callee.
- Whether the Gold constants 0x3d088888 (about 1/30) and 2000.0f, and the ItemProp
  constants 0x3d888888 (about 1/15), 6000.0f and 64.0f, have names in the original
  source.
