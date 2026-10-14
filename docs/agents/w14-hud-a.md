# w14-hud-a: in-game HUD, shop, item display, save info and map helpers (part a)

Package `w14-hud-a`, branch `agents/w14-hud-a`, shared base `a7bb4e51`.
Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses
below are that file's symbol values. No game bytes, listings or extracted data
are committed; everything is my own description.

Method: every function was written from the instruction semantics, compiled with
the reviewed EE g++ `authored_cpp` flags, then run side by side with the original
function in a private R5900 interpreter (outside git), from identical memory
snapshots with identical stand-ins for every callee. A run passes only when the
call logs (arguments and, for text, the string contents), every changed byte
outside the stack, and the checked registers agree. Sources are behaviour
reconstructions; compiled sizes differ from the image extents. Nothing here ran
on the EE or in gameplay.

## Functions

| source (under `sources/`) | symbol | image | compiled bytes |
|---|---|---|---|
| `w14-hud-a-comma-string/comma_string.cpp` | `commaString__FPc` | `0x1bbf48`, 208 | 296 |
| `w14-hud-a-get-enemy-word/get_enemy_word.cpp` | `getEnemyWord__Fi` | `0x15e908`, 60 | 36 |
| `w14-hud-a-remapped-button-icon/remapped_button_icon.cpp` | `hudGetRemappedButtonIcon__Fii` | `0x1c7b30`, 200 | 180 |
| `w14-hud-a-attack-bonus/attack_bonus.cpp` | `getAttackBonus__4itemPQ24item11description` | `0x1039f0`, 212 | 132 |
| `w14-hud-a-floor-put-save-info/floor_put_save_info.cpp` | `floorPutSaveInfo__FPc` | `0x192e18`, 232 | 304 |
| `w14-hud-a-texture-draw-float-inner/texture_draw_float_inner.cpp` | `_textureDrawFloat__FP8_textureffiUiff` | `0x16d978`, 744 | 472 |
| `w14-hud-a-model-draw-effect/model_draw_effect.cpp` | `modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl` | `0x16bb10`, 1360 | 1012 |
| `w14-hud-a-megatile-entitize/megatile_entitize.cpp` | `megatileEntitize__FPci` | `0x268f58`, 1668 | 1924 |
| `w14-hud-a-display-item-stats/display_item_stats.cpp` | `displayItemStats__FiiiPQ24item11descriptioniii` | `0x1b49c8`, 7524 | 6368 |

Each directory also holds a host check (`host_check.cpp`; not a plan input). The
megatile and item-stats checks need an x86_64 build with statics below 4 GiB
(command in each file's first lines) because the game's records hold 32-bit
pointer words; the others build natively with `c++ -std=c++17`.

## Data objects

| source | symbol | image | what is known |
|---|---|---|---|
| `w14-hud-a-hide-ability-adjust-mouse` | `hideAbilityAdjustMouse` | bss `0x486a90`, 8 | two ints (the shop mouse logic indexes it by side); no code in the image stores to it |
| `w14-hud-a-icon-alpha` | `iconAlpha` | bss `0x7d0860`, 112 | 28 ints, seven slots for four players (inferred from the HUD run indexing) |
| `w14-hud-a-item-info-height` | `itemInfoHeight` | bss `0x7d03d8`, 16 | four ints, one per player: the text-line count of the item box |
| `w14-hud-a-item-to-destroy` | `itemToDestroy` | bss `0x486abc`, 4 | one pointer word set by item counting |
| `w14-hud-a-hud-quick-spell-on` | `hudQuickSpellOn` | data `0x34b7a8`, 16 | four ints, zero at load |
| `w14-hud-a-cur-box-old` | `curBoxOld` | data `0x428290`, 16 | four ints, each -2 at load |
| `w14-hud-a-drew-bitmap` | `drewBitmap` | data `0x427458`, 16 | four ints, zero at load |
| `w14-hud-a-tex-copies` | `blackoutTexCopy`, `foregrndTexCopy` | bss `0x8dd340`, 64 and `0x8dd380`, 128 | opaque 64-byte texture records (one and two) |
| `w14-hud-a-dwarf-reg-size-table` | `dwarf_reg_size_table` | bss `0x8eaa98`, 124 | byte table filled by an already authored routine |

Sizes come from the distance to the neighbouring symbol and from the authored
code that indexes each object; each unit is `static_assert`ed and its compiled
size matches.

## Observed

- `commaString__FPc` inserts commas in place with `k = (n - 1) / 3` passes. The
  leading group is `n % 3`, or half of the carried value when that is zero; this
  gives 3 for `n = 6` but 4 for `n = 9`, so nine-digit numbers come out wrong
  ("1234,,56789"). Reproduced, not corrected. It also writes one byte past the
  new terminator.
- `getAttackBonus` reads the first entry of kind 35 among four 8-byte entries
  at cell+36 and scales a base halfword by `value * 0.01`. The entries are
  kind, three value halves (the older reading of 4 pad bytes was wrong: the
  item-stats routine reads halves at +2, +4 and +6).
- `_textureDrawFloat` and `modelDrawEffect` write fixed display-list packets in
  the same style as `_textureDrawColor` and `modelDraw` (see their headers for
  the tag arithmetic). The y offset constant of the sprite is 29184.0, the x
  offset 22528.0 (an early decoding of 28672 failed the paired run).
- `megatileEntitize__FPci`: records relocate once (flag bit 2), special names
  create at once, others are placed by tile row via `doEntityDispatch`, delayed
  records replay through `specialparse`, and `mt_ebx`/`mt_eby` lists become
  "Line" objects. The 4-argument call to `objectCreateByName` for Line objects
  passes an uninitialised point (as the image does).
- `displayItemStats`: see the header of its source for the full layout. Two
  oddities are reproduced on purpose: kind 18 (a requirement) is never coloured
  because the block that would test it is unreachable in the image (a duplicated
  compare against 15), and the 42nd key slot is read from the stack in the image.

## Inferred

- Table names that look like GS register selectors (`3` UV, `5` XYZ2, `1`
  RGBAQ, `0x14` TEX1_1, `0x42` ALPHA_1, `0x47` TEST_1, `0x45` DTHE) follow the
  GS register map, not names from the image.
- `player + 80` is a class word, `+89` a level byte, `+144/+148/+152` three
  stats used by item requirements; kind numbers 14 to 19 are requirement kinds.
- The `720` divisor in `megatileEntitize` is the tile size.

## Unknown

- What the 32 bytes at cell+4 and the base word at +32 mean beyond what the
  code reads; what base kind byte 8 and slot 2 select; the unit of the
  `modelDrawEffect` scale (it multiplies matrix column 3 by 2^42 in the image).
- The meaning of the lightStart words, of mgrg's half at +50, and of the
  `hideAbilityAdjustMouse` writer (none found in the image).

## Checks performed

- Each unit compiles with the reviewed toolchain flags (private `cc.sh`).
- Paired runs: commaString 12 strings; getEnemyWord 6 values;
  hudGetRemappedButtonIcon 39 scenarios; getAttackBonus 40 random records;
  floorPutSaveInfo 10 blocks; _textureDrawFloat 29 scenarios (list bytes and
  drawAddToList arguments); modelDrawEffect 300 scenarios; megatileEntitize 288
  scenarios (416 of 417 original words covered; the missing word is padding);
  displayItemStats 700 randomized items/players/languages (1795 of 1881 words
  covered; the rest is padding, an unaligned-copy variant and the unreachable
  kind-18 block). No halt in any run.
- Source mutations (changing one constant or condition) made each paired run fail
  for commaString, _textureDrawFloat, modelDrawEffect (six of seven; the seventh
  is the else branch that cannot run), megatileEntitize (eight; two more were cut
  by a timeout and not run) and displayItemStats (ten). The small functions were
  validated by the paired run only.
- Host checks pass (`c++ -std=c++17`; the two arena checks as x86_64).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.
- Link check: the 18 units compiled and linked with `ld -m elf32lr5900 -r` into
  one object; the defined names are exactly the imports listed below. A full
  `tools/build_partial.py` run was not made (it needs the shared snapshot, which
  I may not edit).

## Imports closed

19 of the 22 package names: `_textureDrawFloat__FP8_textureffiUiff`,
`blackoutTexCopy`, `commaString__FPc`, `curBoxOld`,
`displayItemStats__FiiiPQ24item11descriptioniii`, `drewBitmap`,
`dwarf_reg_size_table`, `floorPutSaveInfo__FPc`, `foregrndTexCopy`,
`getAttackBonus__4itemPQ24item11description`, `getEnemyWord__Fi`,
`hideAbilityAdjustMouse`, `hudGetRemappedButtonIcon__Fii`, `hudQuickSpellOn`,
`iconAlpha`, `itemInfoHeight`, `itemToDestroy`, `megatileEntitize__FPci`,
`modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl`.

## Not closed

- `megatileGenerate__FP12_worldHeader` (`0x25e3a8`, 19680 bytes). Read for its
  call list only: it uses setjmp/longjmp, 26 stack checks and 12 heap checks, soft
  double arithmetic, instantiated `swap` templates, and the MGWorldmap, MGGlobals,
  PolysetHeader, EdgingGeneral and WaterPolysetData classes whose layouts nobody
  has established. Writing it before those layouts exist would be guesswork.
- `combineTable` (`0x4274b8`, 1020 bytes, initialised data). Observed: 255 words,
  almost all pointers to item and modifier-description strings in groups of
  three (name, two modifier specs), 17 families with five tiers each, and a
  zero tail. Reproducing it would republish the game's item-balance data as
  text, so it is held for an owner decision; the layout is described only.
- `creditsText` (`0x428a20`, 2408 bytes, initialised data). Observed: 602 words in
  records of six words (flag word, string pointers, zero words). The content is the
  game's credits text, so it is held for an owner decision.

## Imports opened

New names the authored sources reference and that are defined nowhere in the
current shared partial build (original ELF names, left unresolved on purpose):
`doEntityDispatch__FP8MTObjDefiP13delayedEntityPib`, `dynlevelcount`,
`instmap_bounds`, `kittencount`, `levsuffix`, `mt_ebx`, `mt_eby`,
`objectProcessDeleteList__Fv`, `specialparse__FPPciiG6Point3Pc`.
The megatile package defines `norrath_megatile_do_entity_dispatch` and
`norrath_megatile_specialparse` under its own names; the integrator can bind them
to these original names or have that package export them.

Referenced and already defined in the shared partial build (not opened by me):
`GLISTSIZE`, `MC_GameConfig`, `addpmlist__FPc`, `addpmlistptrscap__Fv`,
`animLoadBoneMatricies__FP6PacketP14AnimationState`,
`drawAddToList__FPviP8_textureP11_drawRecordi`, `floorAdd__FG6Point3sfii`,
`floorClear__Fv`, `floorf`, the `font*` routines, `fptodp`, `getArmorBonus`,
`getDamageRangeAll`, `getWeight`, `glistp`, `gstart`, `language`,
`lightSetupAmbientOnly__FP6Packet`, `lightStart`, `lumpFindResource__FPcT0`,
`matMul__FP8Matrix44P8Matrix34T0`, `memset`, `mgrg`, `mt_tiledata`,
`objectCreateByName__FPcG6Point3sPPc`, `player`, `playerGetFeatRank__Fix`,
`pmlist*`, `smallFont`, `sprintf`, `strcmp`, `strlen`, `strstr`,
`textGetString__FPc`, `textureDrawSuperColor__FP8_textureiiiUiiiiicc`,
`textureSmoothBorder__FiiiiiUiiii`, `tinyIcon`, `userFilterFunction`,
`usprintf__FPUsT0ffffffff`, `ustrcat__FPUsT0`, `worldCharViewMatrix`.

## Build-plan entries needed

Append these as `independently_authored_source` inputs (profile `authored_cpp`);
`input_order` is assigned by the integrator. `emitted_functions` is for the
snapshot only. `default_authored_inputs` grows by 18. No header files.

```json
{"emitted_functions":[{"compiled_body_bytes":132,"name":"getAttackBonus__4itemPQ24item11description"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_attack_bonus.o","profile":"authored_cpp","sha256":"5f9b86687c228dc9811711ed4f44921e92dc9f410cf234c4944ddac5b2ee1e93","source":"sources/w14-hud-a-attack-bonus/attack_bonus.cpp"}
{"emitted_functions":[{"compiled_body_bytes":296,"name":"commaString__FPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_comma_string.o","profile":"authored_cpp","sha256":"fb2b88e00836595bd31681f9e0dfcf31200730fd805adfcf434e105565ddd408","source":"sources/w14-hud-a-comma-string/comma_string.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_cur_box_old.o","profile":"authored_cpp","sha256":"53e4ad0b06ad64e90065f60843745ec2d0a18f793b2d5f4e69def13285578579","source":"sources/w14-hud-a-cur-box-old/cur_box_old.cpp"}
{"emitted_functions":[{"compiled_body_bytes":6368,"name":"displayItemStats__FiiiPQ24item11descriptioniii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_display_item_stats.o","profile":"authored_cpp","sha256":"27a51a781286d536e5200b2c0e5b835462eb4e4135516bdb396724602654a59a","source":"sources/w14-hud-a-display-item-stats/display_item_stats.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_drew_bitmap.o","profile":"authored_cpp","sha256":"b96b0c2a62158b987735e4275efd46e9c842079142f74dff64e8f4e8c5ac7910","source":"sources/w14-hud-a-drew-bitmap/drew_bitmap.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_dwarf_reg_size_table.o","profile":"authored_cpp","sha256":"f45c89d91769d7b2ea052d91e15457e29b50a871211ec8405dd60caa05248e34","source":"sources/w14-hud-a-dwarf-reg-size-table/dwarf_reg_size_table.cpp"}
{"emitted_functions":[{"compiled_body_bytes":304,"name":"floorPutSaveInfo__FPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_floor_put_save_info.o","profile":"authored_cpp","sha256":"e8355539127ec57c8428898fe6675ebe1688bb728fd7a09f57eb1705f08f7873","source":"sources/w14-hud-a-floor-put-save-info/floor_put_save_info.cpp"}
{"emitted_functions":[{"compiled_body_bytes":36,"name":"getEnemyWord__Fi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_get_enemy_word.o","profile":"authored_cpp","sha256":"2494357563f0542f1dd17e4907d93b7df4efefe7f5e999358d13bac82b670230","source":"sources/w14-hud-a-get-enemy-word/get_enemy_word.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_hide_ability_adjust_mouse.o","profile":"authored_cpp","sha256":"24be9b144c1f099df5fffe354a2afb894ed0f1a467144862dbcce6cfd94b786a","source":"sources/w14-hud-a-hide-ability-adjust-mouse/hide_ability_adjust_mouse.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_hud_quick_spell_on.o","profile":"authored_cpp","sha256":"dc21b24d2a11f4ce0924d2c51d2b1da523e68b5c8fcc9bd3be67f6accdf2ffd4","source":"sources/w14-hud-a-hud-quick-spell-on/hud_quick_spell_on.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_icon_alpha.o","profile":"authored_cpp","sha256":"f324c6f4eef615d603678b5ec223f0b3b55c05f24aa4ed97cef316d3c2413adf","source":"sources/w14-hud-a-icon-alpha/icon_alpha.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_item_info_height.o","profile":"authored_cpp","sha256":"7f7d3deb23579e46490d95d58cb645fbbcd906672e04386f33e34c8f379ec224","source":"sources/w14-hud-a-item-info-height/item_info_height.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_item_to_destroy.o","profile":"authored_cpp","sha256":"e3f6de6c4320716bc9326d623d19cd995313048071cd7d20c4dc51aea3c00c29","source":"sources/w14-hud-a-item-to-destroy/item_to_destroy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1924,"name":"megatileEntitize__FPci"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_megatile_entitize.o","profile":"authored_cpp","sha256":"bdfc109d35610ccfb8482a59a09f4a9a06cf89710a4ab942b62b6d96ad0f8144","source":"sources/w14-hud-a-megatile-entitize/megatile_entitize.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1012,"name":"modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_model_draw_effect.o","profile":"authored_cpp","sha256":"7212ceb4c385b8760f393eb21feaa57fb36592fa5048e7cac7ef63a95e087ee5","source":"sources/w14-hud-a-model-draw-effect/model_draw_effect.cpp"}
{"emitted_functions":[{"compiled_body_bytes":180,"name":"hudGetRemappedButtonIcon__Fii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_remapped_button_icon.o","profile":"authored_cpp","sha256":"f2fd9c4b33c35b183d2f448f20c57c4cbfe87c6bd6ec7510a4911bff17d3a6e2","source":"sources/w14-hud-a-remapped-button-icon/remapped_button_icon.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_tex_copies.o","profile":"authored_cpp","sha256":"911205c4893ccdec5bde9d1be509ff9a3920bc23bf006602748175cd467e2977","source":"sources/w14-hud-a-tex-copies/tex_copies.cpp"}
{"emitted_functions":[{"compiled_body_bytes":472,"name":"_textureDrawFloat__FP8_textureffiUiff"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_hud_a_texture_draw_float_inner.o","profile":"authored_cpp","sha256":"a3a92cba42163212d02778c6fba73e56ff6c920301fcbb0d2033ee2695dccbfb","source":"sources/w14-hud-a-texture-draw-float-inner/texture_draw_float_inner.cpp"}
```

File hashes (for `files` in both the plan and the snapshot):

```json
{
 "sources/w14-hud-a-attack-bonus/attack_bonus.cpp": "5f9b86687c228dc9811711ed4f44921e92dc9f410cf234c4944ddac5b2ee1e93",
 "sources/w14-hud-a-comma-string/comma_string.cpp": "fb2b88e00836595bd31681f9e0dfcf31200730fd805adfcf434e105565ddd408",
 "sources/w14-hud-a-cur-box-old/cur_box_old.cpp": "53e4ad0b06ad64e90065f60843745ec2d0a18f793b2d5f4e69def13285578579",
 "sources/w14-hud-a-display-item-stats/display_item_stats.cpp": "27a51a781286d536e5200b2c0e5b835462eb4e4135516bdb396724602654a59a",
 "sources/w14-hud-a-drew-bitmap/drew_bitmap.cpp": "b96b0c2a62158b987735e4275efd46e9c842079142f74dff64e8f4e8c5ac7910",
 "sources/w14-hud-a-dwarf-reg-size-table/dwarf_reg_size_table.cpp": "f45c89d91769d7b2ea052d91e15457e29b50a871211ec8405dd60caa05248e34",
 "sources/w14-hud-a-floor-put-save-info/floor_put_save_info.cpp": "e8355539127ec57c8428898fe6675ebe1688bb728fd7a09f57eb1705f08f7873",
 "sources/w14-hud-a-get-enemy-word/get_enemy_word.cpp": "2494357563f0542f1dd17e4907d93b7df4efefe7f5e999358d13bac82b670230",
 "sources/w14-hud-a-hide-ability-adjust-mouse/hide_ability_adjust_mouse.cpp": "24be9b144c1f099df5fffe354a2afb894ed0f1a467144862dbcce6cfd94b786a",
 "sources/w14-hud-a-hud-quick-spell-on/hud_quick_spell_on.cpp": "dc21b24d2a11f4ce0924d2c51d2b1da523e68b5c8fcc9bd3be67f6accdf2ffd4",
 "sources/w14-hud-a-icon-alpha/icon_alpha.cpp": "f324c6f4eef615d603678b5ec223f0b3b55c05f24aa4ed97cef316d3c2413adf",
 "sources/w14-hud-a-item-info-height/item_info_height.cpp": "7f7d3deb23579e46490d95d58cb645fbbcd906672e04386f33e34c8f379ec224",
 "sources/w14-hud-a-item-to-destroy/item_to_destroy.cpp": "e3f6de6c4320716bc9326d623d19cd995313048071cd7d20c4dc51aea3c00c29",
 "sources/w14-hud-a-megatile-entitize/megatile_entitize.cpp": "bdfc109d35610ccfb8482a59a09f4a9a06cf89710a4ab942b62b6d96ad0f8144",
 "sources/w14-hud-a-model-draw-effect/model_draw_effect.cpp": "7212ceb4c385b8760f393eb21feaa57fb36592fa5048e7cac7ef63a95e087ee5",
 "sources/w14-hud-a-remapped-button-icon/remapped_button_icon.cpp": "f2fd9c4b33c35b183d2f448f20c57c4cbfe87c6bd6ec7510a4911bff17d3a6e2",
 "sources/w14-hud-a-tex-copies/tex_copies.cpp": "911205c4893ccdec5bde9d1be509ff9a3920bc23bf006602748175cd467e2977",
 "sources/w14-hud-a-texture-draw-float-inner/texture_draw_float_inner.cpp": "a3a92cba42163212d02778c6fba73e56ff6c920301fcbb0d2033ee2695dccbfb"
}
```

## Authored but never executed

All nine functions and the data objects. They were compiled and compared in a
private interpreter against the original with stand-in callees; none has run on
the EE, as part of the linked executable, or in gameplay. The float code
(`_textureDrawFloat`, `modelDrawEffect`, `megatileEntitize` coordinates,
`getAttackBonus`, `displayItemStats` weights) was compared on the interpreter's
single-precision model, not the EE's. `fptodp` and `floorf` were stubbed in the
paired runs, so real soft-float results were not compared. In `displayItemStats`
the three variadic `sprintf` formats and every text routine were stand-ins.
`megatileEntitize` and `displayItemStats` read records holding 32-bit pointer
words; their host checks use an address arena.
