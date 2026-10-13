# HUD, inventory, shop, item and fonts (c)

Package `w9-hud-inventory-items-c`, branch `agents/w9-hud-inventory-items-c`, shared base `ad8cd7c5`.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` (the same image the other
packages use). Addresses below are that file's symbol values and instruction operands. No game
bytes or disassembly listings are committed; every comment is my own description. Item, lump and
animation names that appear in source are short identifier strings the code passes around (the
same kind `inventory-items-shop-start` already carries), not dialogue.

## How each function was checked

Every function was compiled with the reviewed EE g++ `authored_cpp` flags. Each has a host check
in its own directory (`host_check.cpp`; not a plan input) that returns non-zero when a field, call
order, argument or result is wrong. Units that use record words as pointers include the source and
define `NORRATH_INVENTORY_HOST` so the check can map the 32-bit words to host objects; build those
files alone (`c++ -std=c++17 -I. host_check.cpp`; the ones that provide their own `sprintf` or string
routines also need `-fno-builtin`).

In addition each function was run side by side with the original in a private R5900 interpreter
(outside git): both versions start from identical memory, every callee is replaced by the same logging
stub, and the call logs (arguments, order), return value and every changed byte outside the stack must
be equal. The scenario counts and the share of original instructions reached are in the table below.
This is a behavior comparison, not a byte match: compiled bodies differ in size from the image extents.
Nothing here ran on the EE or as gameplay. The EE float unit has no NaN or infinity encodings while the
interpreter follows IEEE; scenarios that reach an ordered float comparison therefore avoid those bit patterns, and the few
scenarios that feed them through plain multiplications or additions (the halo scale, the record floats) give identical results in both versions.

## Functions

| source (under `sources/`) | symbol | image | compiled bytes | paired runs | original instructions reached |
|---|---|---|---|---|---|
| `w9-hud-inventory-items-c-pack/pack.cpp` | `pack__4itemPQ24item11description` | `0x1083e8`, 28 bytes | 28 | 33 | 7/7 |
| `w9-hud-inventory-items-c-unpack/unpack.cpp` | `unpack__4itemPQ24item11description` | `0x108408`, 28 bytes | 28 | 33 | 7/7 |
| `w9-hud-inventory-items-c-setup-halo/setup_halo.cpp` | `setupHalo__4itemPQ24item11descriptionf` | `0x108308`, 220 bytes | 104 | 60 | 54/55 |
| `w9-hud-inventory-items-c-player-get-skill-rank/player_get_skill_rank.cpp` | `playerGetSkillRank__Fii` | `0x1ac260`, 92 bytes | 88 | 2880 | 23/23 |
| `w9-hud-inventory-items-c-player-get-marker-color/player_get_marker_color.cpp` | `playerGetMarkerColor__Fii` | `0x1ac1d0`, 136 bytes | 68 | 63 | 32/34 |
| `w9-hud-inventory-items-c-player-log-inventory/player_log_inventory.cpp` | `playerLogInventory__Fi` | `0x1ac490`, 96 bytes | 8 | 30 | 24/24 |
| `w9-hud-inventory-items-c-shop-buy/shop_buy.cpp` | `shopBuy__FcPQ24item11description` | `0x1bbe58`, 136 bytes | 168 | 216 | 34/34 |
| `w9-hud-inventory-items-c-player-is-anim-playing/player_is_anim_playing.cpp` | `playerIsAnimPlaying__6PlayerPc` | `0x1ac160`, 112 bytes | 116 | 600 | 28/28 |
| `w9-hud-inventory-items-c-player-get-potions-left/player_get_potions_left.cpp` | `playerGetPotionsLeft__Fii` | `0x1abd68`, 312 bytes | 400 | 480 | 77/78 |
| `w9-hud-inventory-items-c-pic-find/pic_find.cpp` | `picFind__4itemPc` | `0x100258`, 364 bytes | 472 | 3000 | 91/91 |
| `w9-hud-inventory-items-c-pic-kick-one-out/pic_kick_one_out.cpp` | `picKickOneOut__4itemv` | `0x1004a8`, 256 bytes | 212 | 401 | 63/64 |
| `w9-hud-inventory-items-c-player-check-recall-hot-key/player_check_recall_hot_key.cpp` | `playerCheckRecallHotKey__Fv` | `0x1ab1e8`, 416 bytes | 452 | 1500 | 104/104 |
| `w9-hud-inventory-items-c-reset-player-animations/reset_player_animations.cpp` | `resetPlayerAnimations__Fi` | `0x1a1c98`, 448 bytes | 344 | 1500 | 109/112 |
| `w9-hud-inventory-items-c-recompute-paper-doll-modifiers/recompute_paper_doll_modifiers.cpp` | `recomputePaperDollModifiers__Fi` | `0x1a4a90`, 700 bytes | 944 | 1500 | 171/175 |
| `w9-hud-inventory-items-c-unequip-item/unequip_item.cpp` | `unequipItem__FiPQ24item11description` | `0x1af3a8`, 912 bytes | 896 | 2500 | 219/228 |
| `w9-hud-inventory-items-c-player-maybe-get-item/player_maybe_get_item.cpp` | `playerMaybeGetItem__FiPQ24item11description` | `0x148480`, 696 bytes | 708 | 3000 | 168/174 |
| `w9-hud-inventory-items-c-shop-frame/shop_frame.cpp` | `shopFrame__Fv` | `0x1b2248`, 876 bytes | 1060 | 6000 | 216/219 |
| `w9-hud-inventory-items-c-setup-default-equipment/setup_default_equipment.cpp` | `setupDefaultEquipment__Fi` | `0x19e780`, 11948 bytes | 8396 | 1200 | 2978/2987 |
| `w9-hud-inventory-items-c-shop-item-set-attributes/shop_item_set_attributes.cpp` | `shopItemSetAttributes__FiiPcPQ24item11description` | `0x1b9fd8`, 7020 bytes | 1308 | 6000 | 1706/1755 |
| `w9-hud-inventory-items-c-create-random/create_random.cpp` | `createRandom__4itemPQ24item11descriptioniiii` | `0x104440`, 4220 bytes | 3892 | 500 | 1044/1055 |

Notes on the instructions not reached:

- `setupHalo__4itemPQ24item11descriptionf`: the one miss is the clamp of the scale to 1.0, which the only level (7) never reaches.
- `playerGetMarkerColor__Fii`: two nops.
- `playerLogInventory__Fi`: empty loop body.
- `shopBuy__FcPQ24item11description`: includes char arguments that are not sign-extended by the caller.
- `playerGetPotionsLeft__Fii`: one nop.
- `picKickOneOut__4itemv`: one nop.
- `resetPlayerAnimations__Fi`: three nops.
- `recomputePaperDollModifiers__Fi`: four nops.
- `unequipItem__FiPQ24item11description`: nine nops.
- `playerMaybeGetItem__FiPQ24item11description`: six nops.
- `shopFrame__Fv`: two nops and one delay slot that cannot run (a branch-likely whose condition was just excluded).
- `setupDefaultEquipment__Fi`: nine nops.
- `shopItemSetAttributes__FiiPcPQ24item11description`: every uncovered instruction is a nop.
- `createRandom__4itemPQ24item11descriptioniiii`: five nops; the rest are annulled delay slots and the potion/no-prefix paths of rarely drawn inputs.

## Data objects

| source | symbol | what it is |
|---|---|---|
| `shop-data/shop_data.cpp` | `shop` | 464 bytes of `.bss` at `0x7d01f8` (the gap to `sellMode`). |
| | `sellMode`, `playerGettinIt` | four words each (16 bytes) at `0x7d03c8` and `0x7d08f0`. |
| | `shopIntro`, `shopTick`, `shopGrid` | one word each at `0x486aa0`, `0x486aac`, `0x48491c` (`shopTick` is typed as a pointer). |
| | `shopDisableMouse`, `shopMouseDecay` | two words each at `0x484a10` and `0x484a18`. |
| `text-storage/text_storage.cpp` | `norrath_text_hash_slots` | 8192 pointer-sized slots at `0x6d8fa0`. |
| | `norrath_text_missing_entries` | 16 entries of 20 halfwords at `0x6e0fa0`. |
| | `norrath_text_missing_cursor` | one byte at `0x484038` (the original local is called `unknownSpinner`). |
| `armor-cache-storage/armor_cache_storage.cpp` | `norrath_player_armor_cache_view_7cb770` | 3520 bytes at `0x7cb770` (the gap to `cacheLoadName`). |

All of them are zero in the image (the `.data` and `.sdata` ones have all-zero file bytes) and are built as plain zeroed C++
objects, so they land in `.bss` here. Sizes were checked against the compiled object (`nm -S`) and the image gaps. The three
`norrath_*` names are authored names (the image has no symbol or only a local one); they are the names the earlier units import.

## Observed

- Player record fields used by this package (record is 23984 bytes, see `inventory-items-record/record.hpp`): +80 and +84 words
  (class-like and a mode-like value; the starting kit switches on +80 with values 0 to 4), +152 a word that feeds the weight/ability
  total, +172 a float tested against 0, +176 and +200 floats that attribute ids 5 and 6 add to, +232 the gold-like purse used by shopBuy,
  +304/+308/+312 floats read by the item pickup test (current load, limit), +328 the three inventory lists, +21872 a pointer word to the
  player object, +21896..+21968 the equipped-slot words, +23572 and +23582 two signed bytes, +23712 the skill pairs.
- Item slots (72 bytes): +0 pointer (word) to an item-table row; the first word of a row is its name pointer; row bytes +28 type, +29
  list number (also the kind byte used by `unequipItem`), +30; +4 quantity halfword of the slot; +34 slot index; +36 four attribute
  entries of 8 bytes (id halfword, three value halfwords). Attribute ids come with their parse formats in
  `shop-item-set-attributes` (for example 13 is armorbonus, 14 requiredlevel, 5/6 healthbonus/manabonus which
  `recomputePaperDollModifiers` adds to the floats at record+176/+200).
- `pack`/`unpack` convert the slot's row pointer to and from a row index using the item table at `0x32b320` (64-byte rows).
- The item table, prefix table (`_4item$prefixTable`, 58 records of 40 bytes) and suffix table (`_4item$suffixTable`, 91 records of 20
  bytes) feed `createRandom`; its random draws all go through `syncRandState` with the usual 1664525 / 1013904223 step.
- The shop screen keeps two 116-byte per-player records inside `shop`: a player number word at +52 and a state word at +76 (value 2
  is tested by `shopFrame`).
- `addItemToInventory`'s third argument is an announce flag in the other package that uses it; here it is 1 for the normal gives and 0
  for the recall potion of the starting kit. The recall potion, healing and rejuvenation potions are added twice from one create
  (the image's loop body runs twice).
- The sound and animation names in `unequipItem` and `playerMaybeGetItem` (`inv_drop_*`, `inv_equip_*`, `game_butn_cantdo.anm`) are
  chosen by the same item flag bits; `unequipItem` has a dead test (bits 0x4000000/0x2000000) that leads to the same jewelry name as
  the default.
- The image's `playerLogInventory` walks the three lists with an empty body.
- Float behavior: the EE FPU has no NaN or infinity encodings; the private interpreter does model them, so comparisons that end in
  an ordered float test avoid them and no comment claims a NaN behavior.

## Inferred

- The 72-byte "slot-shaped" record of the mangled `item::description` is the inventory slot, so `pack`/`unpack` take a slot.
- `createRandom`'s first number argument is a value ceiling and the third a category (0, 1, 2 are special; -1 means any); the
  prefix/suffix table names come from the image's symbols. What the categories are is not established.
- `setupDefaultEquipment` is a starting kit by class plus a debug "give everything" list; the class numbers 0..4 and the names
  `Warrior/Cleric/ShadowKnight/Wizard` come from the skill-tree symbols the function picks by class.
- `shopItemSetAttributes` parses item attribute text ("name+attribute+...") the way the item creation code does; the ids are tied to
  the parse formats, not to any named table.

## Checks performed

- Each unit compiles with the reviewed toolchain (`authored_cpp` flags) and has a host check that returns non-zero on a wrong field,
  call order, argument or result. Checks of the larger units were also run against mutated copies of the source and fail
  (examples: round count, boundary of a limit comparison, a class threshold, an off-by-one in a loop count).
- Every function above was run side by side with the original in the private interpreter with identical memory and identical logging
  stubs for every callee; all runs matched on call logs (arguments and order), return value and every changed byte outside the stack.
  The counts and coverage are in the table.
- Private partial build: all units were added to a private copy of the plan and snapshot kept outside git; `build_partial.py`
  completed. The unresolved-import diff against the shared base is below. The `unittest` suite passes.

## Imports closed

`createRandom__4itemPQ24item11descriptioniiii`, `norrath_item_create_random`, `norrath_player_armor_cache_view_7cb770`, `norrath_text_hash_slots`, `norrath_text_missing_cursor`, `norrath_text_missing_entries`, `pack__4itemPQ24item11description`, `picFind__4itemPc`, `picKickOneOut__4itemv`, `playerCheckRecallHotKey__Fv`, `playerGetMarkerColor__Fii`, `playerGetPotionsLeft__Fii`, `playerGetSkillRank__Fii`, `playerGettinIt`, `playerIsAnimPlaying__6PlayerPc`, `playerLogInventory__Fi`, `playerMaybeGetItem__FiPQ24item11description`, `recomputePaperDollModifiers__Fi`, `resetPlayerAnimations__Fi`, `sellMode`, `setupDefaultEquipment__Fi`, `setupHalo__4itemPQ24item11descriptionf`, `shop`, `shopBuy__FcPQ24item11description`, `shopDisableMouse`, `shopFrame__Fv`, `shopGrid`, `shopIntro`, `shopItemSetAttributes__FiiPcPQ24item11description`, `shopMouseDecay`, `shopTick`, `unequipItem__FiPQ24item11description`, `unpack__4itemPQ24item11description`.

32 of the 33 assigned names.

## Imports not closed from the assigned list

`norrath_item_create` is the one assigned name left open. Observed: it is the same function as `create__4itemPcPQ24item11descriptionii` (0x105f60, 5428 bytes) and as `norrath_player_create_item` already authored in `game-item-create-build-v2`. Closing it needs a one-line alias in that unit (`__attribute__((alias("norrath_player_create_item")))` on the target), which belongs to another package, so it was not touched. `norrath_item_create_random` is closed: `create-random` authors `createRandom__4itemPQ24item11descriptioniiii` and defines that name as an alias of it.

## Imports opened

Original ELF names, left unresolved on purpose (none authored or stubbed):

`_4item$prefixTable`, `_4item$suffixTable`, `armorDebug`, `bevel`, `getArmorBonus__4itemPQ24item11description`, `givePlayerSkill__Fii`, `healthPotionsLeft`, `inventoryDrawPlayerColor__Fiii`, `inventorySwitchDualWield__FiPQ24item11description`, `manaPotionsLeft`, `marble`, `netGetCameraCustomControl__Fi`, `playerSetFeatRank__Fixi`, `playerSetSkillRank__Fiii`, `shopRun__Fi`, `shopUnpackToGrid__Fiii`.

## Build-plan entries needed

Append these as `independently_authored_source` inputs (profile `authored_cpp`); `input_order` is assigned by the integrator. Put the
`emitted_functions` into `snapshot.json` and every file hash below into `files` in both the plan and the snapshot. Do not add any
`host_*` file. `default_authored_inputs` grows by the number of lines.

```json
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_armor_cache_storage_armor_cache_storage.o", "profile": "authored_cpp", "sha256": "4ec29b6959b42abb9607a69f7aadef8a07ce4c2a9cfb5e401194e594519ab2b1", "source": "sources/w9-hud-inventory-items-c-armor-cache-storage/armor_cache_storage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 252, "name": "_ZN12_GLOBAL__N_1L21add_level_requirementEPhi"}, {"compiled_body_bytes": 3892, "name": "createRandom__4itemPQ24item11descriptioniiii"}, {"compiled_body_bytes": 3892, "name": "norrath_item_create_random"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_create_random_create_random.o", "profile": "authored_cpp", "sha256": "1aa0a71dc51773b5544ada38f4f289aa20ffc49f7ce029c0e54e9c6b50ea9bae", "source": "sources/w9-hud-inventory-items-c-create-random/create_random.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "pack__4itemPQ24item11description"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_pack_pack.o", "profile": "authored_cpp", "sha256": "5f40ad870ca7a96de8215f78b3f1688e9ecea07556d601edbf0b9d4dad159737", "source": "sources/w9-hud-inventory-items-c-pack/pack.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 472, "name": "picFind__4itemPc"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_pic_find_pic_find.o", "profile": "authored_cpp", "sha256": "7e5e75f8a84458206ef0372d9bf0d283ce489960401353b5e34d4c4aced690a0", "source": "sources/w9-hud-inventory-items-c-pic-find/pic_find.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 212, "name": "picKickOneOut__4itemv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_pic_kick_one_out_pic_kick_one_out.o", "profile": "authored_cpp", "sha256": "9624b3d83ac6d8357002e2d652a4db164950287748fca69fe15bc4bb2c1df897", "source": "sources/w9-hud-inventory-items-c-pic-kick-one-out/pic_kick_one_out.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 452, "name": "playerCheckRecallHotKey__Fv"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_check_recall_hot_key_player_check_recall_hot_key.o", "profile": "authored_cpp", "sha256": "d20abca43d33dc3464fcc671228157ce119dd05f39653cd4cde838a39443ac51", "source": "sources/w9-hud-inventory-items-c-player-check-recall-hot-key/player_check_recall_hot_key.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "playerGetMarkerColor__Fii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_get_marker_color_player_get_marker_color.o", "profile": "authored_cpp", "sha256": "97c058a235e4be5f2e25a63305669ef0e5d7839345a851975171c740971d34ca", "source": "sources/w9-hud-inventory-items-c-player-get-marker-color/player_get_marker_color.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 400, "name": "playerGetPotionsLeft__Fii"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_get_potions_left_player_get_potions_left.o", "profile": "authored_cpp", "sha256": "d273b4e3eb7c4fe3bd3d989867edbe5768593cafafba780706b228497a5b47c1", "source": "sources/w9-hud-inventory-items-c-player-get-potions-left/player_get_potions_left.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "playerGetSkillRank__Fii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_get_skill_rank_player_get_skill_rank.o", "profile": "authored_cpp", "sha256": "df9d3acdff3f923c833174f36f6965533a0dede1c8a62442743cac48e3127dd7", "source": "sources/w9-hud-inventory-items-c-player-get-skill-rank/player_get_skill_rank.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "playerIsAnimPlaying__6PlayerPc"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_is_anim_playing_player_is_anim_playing.o", "profile": "authored_cpp", "sha256": "7d92d7b75acee94dae8fa3eede1f1d20afe3a4d5769715cbffa8ea0105e61ca3", "source": "sources/w9-hud-inventory-items-c-player-is-anim-playing/player_is_anim_playing.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "playerLogInventory__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_log_inventory_player_log_inventory.o", "profile": "authored_cpp", "sha256": "567027509c72c350a6cbf7041b063eef4b3200b10999bb7352b76ed81b6f523c", "source": "sources/w9-hud-inventory-items-c-player-log-inventory/player_log_inventory.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 708, "name": "playerMaybeGetItem__FiPQ24item11description"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_player_maybe_get_item_player_maybe_get_item.o", "profile": "authored_cpp", "sha256": "9436e9e520d1774acdfaa08e1d76d7217028ad9f72c04ce9e19cb50cbea16ec2", "source": "sources/w9-hud-inventory-items-c-player-maybe-get-item/player_maybe_get_item.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 944, "name": "recomputePaperDollModifiers__Fi"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_recompute_paper_doll_modifiers_recompute_paper_doll_modifiers.o", "profile": "authored_cpp", "sha256": "24e66fb5b3ca3c9d6bafea811a4b6b3dfc7a0187fad8b22cc609f06145e7dcfb", "source": "sources/w9-hud-inventory-items-c-recompute-paper-doll-modifiers/recompute_paper_doll_modifiers.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 344, "name": "resetPlayerAnimations__Fi"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_reset_player_animations_reset_player_animations.o", "profile": "authored_cpp", "sha256": "375fbeb2757d1a295b485c93a75b79e68fd1d769ec4986c1afb9d37049a83b83", "source": "sources/w9-hud-inventory-items-c-reset-player-animations/reset_player_animations.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "_ZN12_GLOBAL__N_1L9armor_setERNS_3KitEPKPKciii"}, {"compiled_body_bytes": 8396, "name": "setupDefaultEquipment__Fi"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_setup_default_equipment_setup_default_equipment.o", "profile": "authored_cpp", "sha256": "1b61c223556d3eb7e3fe01a9170d3b90c34d63b76e631be10567f51fa64d49ed", "source": "sources/w9-hud-inventory-items-c-setup-default-equipment/setup_default_equipment.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "setupHalo__4itemPQ24item11descriptionf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_setup_halo_setup_halo.o", "profile": "authored_cpp", "sha256": "f7b0f605bd06bba3c7e36361dfdd7568def59ba01019e79009d315b5478876bc", "source": "sources/w9-hud-inventory-items-c-setup-halo/setup_halo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "shopBuy__FcPQ24item11description"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_shop_buy_shop_buy.o", "profile": "authored_cpp", "sha256": "253a1336173c513ec80561a13911b724cadf346dbf693566831bf4c9ee242413", "source": "sources/w9-hud-inventory-items-c-shop-buy/shop_buy.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_shop_data_shop_data.o", "profile": "authored_cpp", "sha256": "f134c68a0a4c1c9a74d13cb1f9eb5fcb086af0cf790433360a640351b8bc798b", "source": "sources/w9-hud-inventory-items-c-shop-data/shop_data.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1060, "name": "shopFrame__Fv"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_shop_frame_shop_frame.o", "profile": "authored_cpp", "sha256": "21f2d0c81ddc6254fdea642f02bce2d26f5dace80469fdc05b433e9d054748a3", "source": "sources/w9-hud-inventory-items-c-shop-frame/shop_frame.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "_ZN12_GLOBAL__N_1L6refuseEiPKc.isra.0"}, {"compiled_body_bytes": 1308, "name": "shopItemSetAttributes__FiiPcPQ24item11description"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_shop_item_set_attributes_shop_item_set_attributes.o", "profile": "authored_cpp", "sha256": "ff4a23235d157165d845772bdb7f9d4221ffc5044d1db29f8c4ac9af1134f10c", "source": "sources/w9-hud-inventory-items-c-shop-item-set-attributes/shop_item_set_attributes.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_text_storage_text_storage.o", "profile": "authored_cpp", "sha256": "1b671b43000229ff6a63cf812f053ce54bdb6808f0bdac5438e4007a6ce522a0", "source": "sources/w9-hud-inventory-items-c-text-storage/text_storage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 896, "name": "unequipItem__FiPQ24item11description"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_unequip_item_unequip_item.o", "profile": "authored_cpp", "sha256": "ea8996b295e500357114e4d18420a7421adb2e7faa5fb1062faf20b45c2872dd", "source": "sources/w9-hud-inventory-items-c-unequip-item/unequip_item.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "unpack__4itemPQ24item11description"}], "header_closure": ["sources/inventory-items-record/record.hpp"], "kind": "independently_authored_source", "object_name": "w9_hud_inventory_items_c_unpack_unpack.o", "profile": "authored_cpp", "sha256": "f97b8db96656614b2a5460ca5a84b91e34c92b6a44b3dab856eab8c4433be8c1", "source": "sources/w9-hud-inventory-items-c-unpack/unpack.cpp"}
```

File hashes:

```json
{
 "sources/inventory-items-record/record.hpp": "40adb98656b4ba29dd1dd1e2262463311649c7ff19b55302a50544c1d24d0dc8",
 "sources/w9-hud-inventory-items-c-armor-cache-storage/armor_cache_storage.cpp": "4ec29b6959b42abb9607a69f7aadef8a07ce4c2a9cfb5e401194e594519ab2b1",
 "sources/w9-hud-inventory-items-c-create-random/create_random.cpp": "1aa0a71dc51773b5544ada38f4f289aa20ffc49f7ce029c0e54e9c6b50ea9bae",
 "sources/w9-hud-inventory-items-c-pack/pack.cpp": "5f40ad870ca7a96de8215f78b3f1688e9ecea07556d601edbf0b9d4dad159737",
 "sources/w9-hud-inventory-items-c-pic-find/pic_find.cpp": "7e5e75f8a84458206ef0372d9bf0d283ce489960401353b5e34d4c4aced690a0",
 "sources/w9-hud-inventory-items-c-pic-kick-one-out/pic_kick_one_out.cpp": "9624b3d83ac6d8357002e2d652a4db164950287748fca69fe15bc4bb2c1df897",
 "sources/w9-hud-inventory-items-c-player-check-recall-hot-key/player_check_recall_hot_key.cpp": "d20abca43d33dc3464fcc671228157ce119dd05f39653cd4cde838a39443ac51",
 "sources/w9-hud-inventory-items-c-player-get-marker-color/player_get_marker_color.cpp": "97c058a235e4be5f2e25a63305669ef0e5d7839345a851975171c740971d34ca",
 "sources/w9-hud-inventory-items-c-player-get-potions-left/player_get_potions_left.cpp": "d273b4e3eb7c4fe3bd3d989867edbe5768593cafafba780706b228497a5b47c1",
 "sources/w9-hud-inventory-items-c-player-get-skill-rank/player_get_skill_rank.cpp": "df9d3acdff3f923c833174f36f6965533a0dede1c8a62442743cac48e3127dd7",
 "sources/w9-hud-inventory-items-c-player-is-anim-playing/player_is_anim_playing.cpp": "7d92d7b75acee94dae8fa3eede1f1d20afe3a4d5769715cbffa8ea0105e61ca3",
 "sources/w9-hud-inventory-items-c-player-log-inventory/player_log_inventory.cpp": "567027509c72c350a6cbf7041b063eef4b3200b10999bb7352b76ed81b6f523c",
 "sources/w9-hud-inventory-items-c-player-maybe-get-item/player_maybe_get_item.cpp": "9436e9e520d1774acdfaa08e1d76d7217028ad9f72c04ce9e19cb50cbea16ec2",
 "sources/w9-hud-inventory-items-c-recompute-paper-doll-modifiers/recompute_paper_doll_modifiers.cpp": "24e66fb5b3ca3c9d6bafea811a4b6b3dfc7a0187fad8b22cc609f06145e7dcfb",
 "sources/w9-hud-inventory-items-c-reset-player-animations/reset_player_animations.cpp": "375fbeb2757d1a295b485c93a75b79e68fd1d769ec4986c1afb9d37049a83b83",
 "sources/w9-hud-inventory-items-c-setup-default-equipment/setup_default_equipment.cpp": "1b61c223556d3eb7e3fe01a9170d3b90c34d63b76e631be10567f51fa64d49ed",
 "sources/w9-hud-inventory-items-c-setup-halo/setup_halo.cpp": "f7b0f605bd06bba3c7e36361dfdd7568def59ba01019e79009d315b5478876bc",
 "sources/w9-hud-inventory-items-c-shop-buy/shop_buy.cpp": "253a1336173c513ec80561a13911b724cadf346dbf693566831bf4c9ee242413",
 "sources/w9-hud-inventory-items-c-shop-data/shop_data.cpp": "f134c68a0a4c1c9a74d13cb1f9eb5fcb086af0cf790433360a640351b8bc798b",
 "sources/w9-hud-inventory-items-c-shop-frame/shop_frame.cpp": "21f2d0c81ddc6254fdea642f02bce2d26f5dace80469fdc05b433e9d054748a3",
 "sources/w9-hud-inventory-items-c-shop-item-set-attributes/shop_item_set_attributes.cpp": "ff4a23235d157165d845772bdb7f9d4221ffc5044d1db29f8c4ac9af1134f10c",
 "sources/w9-hud-inventory-items-c-text-storage/text_storage.cpp": "1b671b43000229ff6a63cf812f053ce54bdb6808f0bdac5438e4007a6ce522a0",
 "sources/w9-hud-inventory-items-c-unequip-item/unequip_item.cpp": "ea8996b295e500357114e4d18420a7421adb2e7faa5fb1062faf20b45c2872dd",
 "sources/w9-hud-inventory-items-c-unpack/unpack.cpp": "f97b8db96656614b2a5460ca5a84b91e34c92b6a44b3dab856eab8c4433be8c1"
}
```

## Authored but never executed

All functions and data above were compiled and checked only on the host and in the private interpreter. None has run on the EE, as
part of the shared link as an executable, or in gameplay. The tables used by `createRandom`, `picFind` and the feat walk of
`setupDefaultEquipment` were exercised against the image's own data in the interpreter, not against a loaded game. The
`create`-family, `getValue`, `equipItem`, `addItemToInventory` and text functions were replaced by logging stubs in every comparison.
Float arithmetic and `powf` results in `createRandom`, the halo and the paper-doll code were compared on the interpreter's
single-precision unit with a stand-in `powf`, not on the EE.

## Unknown

- What the item categories, the flag bits (0x10000, 0x80000, bit 35, 0x800000, 0x200000 and the others) and the +28/+29/+30 row bytes
  mean; the meaning of the prefix/suffix record words and masks.
- The meaning of player record words +80 and +84, +21872 object fields, the equipped-word table, and the shop record fields beyond
  the offsets read.
- Whether `playerLogInventory`'s empty loop hid a log call; why `shopBuy` reads its gold word before the second price query.
- The sizes of the local text buffers (`unequipItem` 64+, `shopItemSetAttributes` 208, `createRandom` buffer, `setupDefaultEquipment`
  name buffer 104) beyond what the frame layouts allow.
- Whether the five `norrath_*` names should be aliases: `norrath_item_create` is the same function as
  `norrath_player_create_item` (`game-item-create-build-v2`) and as `create__4itemPcPQ24item11descriptionii` (0x105f60); this
  package did not author it (see above).

Note: `unequipItem` imports `animState`, which the image defines as several local statics (0x7cc5c8, 0x7ccad8, 0x7d0440, 0x7d0910);
the function uses the first (stride 264 bytes per player), and the existing unresolved `animState` import stands for it.
