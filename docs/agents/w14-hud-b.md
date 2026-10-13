# w14-hud-b: in-game HUD, shop, item display, save info and map helpers (part b)

Branch `agents/w14-hud-b`, based on `a7bb4e51`. The package was 22 unresolved imports of the shared partial build (re-run
here on that revision: 957 unresolved imports, all 22 names present). Addresses, sizes and constants come from the pinned
private `SLUS_205.65` symbol table and its instruction bytes. Nothing here ran on the console. Each authored function was
compared with the original in the private R5900 interpreter (outside git, same harness as earlier packages), and each has
a committed host check (`host_check.cpp`, not a plan input). No compiled body is claimed to match the original bytes.

## What was found about the list

Of the 22 names, 16 are closed by sources in this branch (15 new, one earlier unit that only needs selecting), five are names
that cannot be closed by a source file, and `routePlot__FR10RouteQueue` was not authored.

- `norrath_item_create_random` is the alias name that `ctors-lava-wraith` uses for `createRandom__4itemPQ24item11descriptioniiii`,
  which is already in the plan from `w9-hud-inventory-items-a-create-random`. The unit `w9-hud-inventory-items-c-create-random`
  defines the same ELF name again plus the alias, so selecting it would add a duplicate definition. It stays open.
- `norrath_item_create` is the same function as `create__4itemPcPQ24item11descriptionii` (`0x105f60`; in the plan from
  `w10-item-a-item-create`). Same situation; a forwarding wrapper would not be the original body, so none was written.
- `norrath_inventory_message_handler_word_view` is the word `invMsgHandle` at `0x00486a88` (symbol size 0 in the image, 4 bytes
  by the gap); `norrath_ctors_ant_lantern_view_34b474` is `skills + 52` (`skills` is 252 bytes at `0x0034b440`) and
  `norrath_ctors_ant_lantern_view_34b6b1` is `levelName + 81` (`levelName` is 322 bytes at `0x0034b660`). All three are
  views into objects other units define under their real names; the build driver applies no symbol aliases
  (`symbol_aliases` must stay empty), so they need either a link-level alias entry or callers switched to the ELF names.
- `norrath_inventory_exit_sound_resource_view` already has a unit, `w9-hud-inventory-items-b-view-exit-sound-resource`
  (the 21-byte string `game_lvl_complet.anm` at `0x00466348`), that was never selected. It is in the plan entries below.

## Functions (observed / inferred / unknown is in the header comment of every source)

| source (under `sources/`) | symbol | image | compiled bytes | paired scenarios (original words covered) |
|---|---|---|---|---|
| `w14-hud-b-msg-hud-assign-quick-spell/msg_hud_assign_quick_spell.cpp` | `msgHudAssignQuickSpell__Fiii` | `0x1c7858`, 52 | 68 | 40 |
| `w14-hud-b-msg-hud-assign-spell-book-spell/msg_hud_assign_spell_book_spell.cpp` | `msgHudAssignSpellBookSpell__Fiiil` | `0x1c7ab8`, 64 | 80 | 40 |
| `w14-hud-b-shop-clip-grid-coordinates/shop_clip_grid_coordinates.cpp` | `shopClipGridCoordinates__FPiN30` | `0x1bc270`, 100 | 156 | 60 (aliased pointers included) |
| `w14-hud-b-shop-mouse-on-hot-spot/shop_mouse_on_hot_spot.cpp` | `shopMouseOnHotSpot__FP12screenStruct` | `0x1bc400`, 132 | 248 | 300 |
| `w14-hud-b-pic-add/pic_add.cpp` | `picAdd__4itemPc` | `0x100948`, 216 | 192 | 120 |
| `w14-hud-b-shop-draw-item-bitmap-glow/shop_draw_item_bitmap_glow.cpp` | `shopDrawItemBitmapGlow__FP8_textureii` | `0x1bc488`, 344 | 368 | 5 (100 draw calls each, with the original soft-float routines) |
| `w14-hud-b-read-map/read_map.cpp` | `readMap__3mapPUc` | `0x15c038`, 416 | 368 | 4 (whole 93,312-byte tile array compared) |
| `w14-hud-b-object-put-save-data/object_put_save_data.cpp` | `objectPutSaveData__FPci` | `0x142ee8`, 632 | 720 | 150 (158 of 158) |
| `w14-hud-b-player-init-armor-cache/player_init_armor_cache.cpp` | `playerInitArmorCache__Fv` | `0x195358`, 516 | 556 | 60 (129 of 130; one nop) |
| `w14-hud-b-persue2/persue2.cpp` | `persue2__8CreatureP10GameObjectfff` | `0x110bc8`, 1216 | 1280 | 312 (301 of 304; three nops) |
| `w14-hud-b-run-stats/run_stats.cpp` | `runStats__Fii` | `0x1bcb38`, 4508 | 4300 | 316 (1114 of 1127; 13 nops) |
| `w14-hud-b-player-selection-task/player_selection_task.cpp` | `playerSelectionTask__Fv` | `0x27a658`, 12200 | 12576 (plus two file-local helpers, 52 and 152) | 2296 (2920 of 3050 words) |
| `w14-hud-b-data-player-gold-icon-x/data_player_gold_icon_x.cpp` | `playerGoldIconx` | `0x4282c0`, 16 bytes `.data` | | values checked |
| `w14-hud-b-data-titleload-tex-copy/data_titleload_tex_copy.cpp` | `titleloadTexCopy` | `0x8dd280`, 64 bytes `.bss` | | |
| `w14-hud-b-data-scrollmap-tex-copy/data_scrollmap_tex_copy.cpp` | `scrollmapTexCopy` | `0x8dd2c0`, 128 bytes `.bss` | | |

The selection task also needed twenty-one data objects that nobody defined. All are in the same unit of the original
(`0x00486cf4..0x00486d7c` bss, plus one table); each is its own `w14-hud-b-data-*` directory with a size and zero-value host check
(`statTable` compares its four strings): `charDelta`, `newMode`, `statAllocation`, `msgAlpha`, `msgAlphaDelta`, `curStat`,
`statAllocWarn`, `curSlider`, `lastSlider`, `maxSlider`, `bodyAlpha`, `otherHermiteTime`, `throb`, `throbR`, `throbG`, `throbB`,
`throbA`, `statTable` (16 bytes `.data`, four string pointers to the stat-row text keys) and three address-named views
`norrath_player_select_view_486d3c` (the `flareAlpha` at that address; the image has three objects of that name),
`norrath_player_select_view_486d44` (`flareDelta`, two of that name) and `norrath_player_select_view_486d88` (`firstTime`, two of
that name). The task binds those three by address so that it cannot be tied to a different same-named object (for example the
`int flareAlpha` that `w9-frontend-d-run-browser` declares).

Per-function notes worth keeping:

- `shopDrawItemBitmapGlow`: the two constants come from the image; the double 13.723 is bit pattern `0x402b722d0e560418` (not the
  literal 13.723, which is one ulp different) and the scale is the float word `0x3d83126e` (gp-relative, unnamed). It calls the
  original's soft-float `fptodp`, `dpmul` and `dptoli`, and `expf`; the paired runs execute those original routines.
- `objectPutSaveData`: reads `userFilterFunction`, `norrath_create_by_name_function_word` (the existing view of
  `userCreateByNameFunction`, `0x486894`) and `poolOffset`; writes the nine `lightStart` defaults (32,32,32,68,68,68,0,-1.0,1.0).
  Pointers inside the saved data are 32-bit words, so the source uses unsigned words and a `fromWord` cast, which also lets
  the host check run as x86_64 with low addresses.
- `persue2`: the first float parameter is not read. The result of the move through the creature table (entry at +176) is cut to
  a short; on the one route where the move result is dropped (blocked line, route point reports -1.0) the result stays 0.
- `playerSelectionTask`: a nine-way switch on `selectMode` (0 start, 1 character choice, 2 custom look sliders, 3 stat
  allocation, 4 name pad, 5 leave fade, 6 next-player prompt, 7 advance player, 8 return to the name pad) after the throb colours, the
  idle-animation blends of the ten targets, the camera calls and the fade arithmetic, then a backdrop and the prompt line. The
  character index is read once at entry (`selected`) and used by the paths that the image also takes it from a register; the
  other paths re-read `playerSelectInfo[curPlayer][32]`. Pad masks, colours and 0.077-style floats are the image's values
  (the unnamed gp floats are named `g<offset>` in the source). A detail the paired runs caught: the 10-entry sound-name table
  holds `erm017_altc` and `esm017_alta` (longer than the 8 bytes that looked like the whole string).
- `runStats`: the quest page indexes `statsScreen` by the player index word while the other pages index by slot; the quest page
  reads the quest count and records from the first player record. `fontPrintWrapu__FiiPUsiiii` must return an int (the quest
  loop adds it to the running height; `fontEnd__Fv` is called in the next instruction's delay slot and its result is not used).
  The libc and soft-float symbols (`sprintf`, `strlen`, `strcat`, `sin`) are declared with `__asm__` labels under plain C++
  names so the host check can supply them.

## Verification run

- Each source compiled with the reviewed EE g++ and `authored_cpp` flags (`cc.sh` in the private harness is the same command line
  as the plan profile); each host check was compiled and run (`c++ -std=c++17 -Wall -Wextra -Werror`; the four that hold 32-bit
  pointer words use `-arch x86_64 -Wl,-no_pie -Wl,-pagezero_size,0x1000`, flags in each file).
- Paired runs: original and authored function from identical memory with identical stand-in callees, comparing every changed
  byte outside the stack and the call logs (call name, arguments, string contents). Counts above. All reported zero mismatches.
  Hooks that return a short or a pointer return 32-bit values zero-extended as well as sign-extended in some scenarios, so the
  source's own truncation is exercised (this caught the `persue2` move result).
- Mutation checks: each function's paired script was re-run against hand-made mutants (constants, offsets, comparison
  boundaries, argument order, loop bounds, a dropped result); every mutant listed was caught. `shopDrawItemBitmapGlow` is
  weaker: a one-ulp change of the float scale is not caught (the integer alpha does not change for the 5 scenarios).
- `playerSelectionTask`: the final run is 2296 scenarios: 1,500 random state draws and 796 forced ones (a 6,000-draw run before
  the last fix found the two `esm017_alta` mismatches); the forced ones cover every `requestToExit`/`msgAlpha`/`msgAlphaDelta` combination,
  every pad mask on every mode, fades with `flareDelta` of either sign and `flareAlpha` 0, 0.5, 1, each prompt line by mode and row, the
  slider and stat edge cases; 2920 of the original's 3050 words were executed; the rest are delay-slot nops and the unreachable
  rounding / unsigned-range float branches (`throb*46` is never negative; alphas never reach 2^31). Fourteen mutants were
  caught; two survivors are equivalent (`g22324` and `g22316` hold the same word, and `0.0f <= top` versus `<` differ only at exactly
  zero). The host check (x86_64, low addresses) runs about a hundred hand-written frames with expected traces.
- Private partial builds: a copy of the plan and snapshot kept outside git plus the entries below. The first (without
  `playerSelectionTask` and its data) compiled 6,250 inputs and linked once: 4,932 function bodies, 950 unresolved imports, 0 COMMON, against the
  base 957 (the base was built from `a7bb4e51`). It closed 15 of the package's names (listed below) and opened the eight imports
  `displayResistances__Fi`, `expf`, `msgInventoryAdjustAbility__Fii`, `playerRecomputeDamage__6PlayerPQ24item11descriptioni`,
  `printStat__FiPUsiiii`, `statsFrame__Fv`, `statsRunSkillTree__Fi` and `textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii`.
  A second build with all 37 entries was started and stopped at about a quarter of the way (the machine was
  loaded by other builds); the 21 selection-task data objects and `playerSelectionTask__Fv` were instead checked at object level:
  every unit was compiled with the plan's `authored_cpp` command, the entries script compiled each with the driver's own
  `elf_inventory` (so the recorded function names and sizes are the driver's), and the undefined symbols of all objects were compared
  with the symbols defined by the first build's linked object and by these units; that comparison gives the 20 opened imports
  listed below and no unresolved reference to a name that a unit of this package should have defined.

## Imports closed (names)

`msgHudAssignQuickSpell__Fiii`, `msgHudAssignSpellBookSpell__Fiiil`, `norrath_inventory_exit_sound_resource_view` (earlier unit,
needs only selecting), `objectPutSaveData__FPci`, `persue2__8CreatureP10GameObjectfff`, `picAdd__4itemPc`, `playerGoldIconx`,
`playerInitArmorCache__Fv`, `playerSelectionTask__Fv`, `readMap__3mapPUc`, `runStats__Fii`, `scrollmapTexCopy`,
`shopClipGridCoordinates__FPiN30`, `shopDrawItemBitmapGlow__FP8_textureii`, `shopMouseOnHotSpot__FP12screenStruct`,
`titleloadTexCopy`. Also closed, though not in the assignment: `bodyAlpha` (unresolved in the base list; `w8-core-loop-d-select-init`
imports it).

Not closed: `norrath_ctors_ant_lantern_view_34b474`, `norrath_ctors_ant_lantern_view_34b6b1`,
`norrath_inventory_message_handler_word_view`, `norrath_item_create`, `norrath_item_create_random` (alias or view names of objects
that other units define under their real names; see the first section) and `routePlot__FR10RouteQueue`.

## Imports opened

Function imports that the new sources call and that nothing in the plan defines: `GetMode__6Camera`, `allDone__Fi`,
`boxDraw__FiiiiiUii`, `displayResistances__Fi`, `doNamePad__Fv`, `drawNamePad__Fv`, `expf`, `getHairStyle__Fi`,
`hudRunSlider__FiiiiiUifPii`, `initPostSelect__Fv`, `moveCamera__Fv`, `msgInventoryAdjustAbility__Fii`, `npcLookAtCamera__Fv`,
`playerRecomputeDamage__6PlayerPQ24item11descriptioni`, `printStat__FiPUsiiii`, `resetName__Fv`, `statsFrame__Fv`,
`statsRunSkillTree__Fi`, `switchToNamePad__Fv`, `textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii`. Their owners are the HUD,
player-selection, camera, math-library and stat-page code of other packages (`w14-hud-a` and the player-selection and name-pad
functions are the likely owners). `playerSelectionTask__Fv` also reads globals that other units define under their image names and
that are therefore not new: `curPlayer`, `selectMode`, `lockAlpha`, `requestToExit`, `hermiteTime`, `delta`, `hermDeltaScale`,
`headerAlpha`, `npcLumpName`, `startExitFade`, `playerSelectCount`, `playerSelectStartPlayer`, `playerSelectExitMessage`,
`coreLoading`, `gameCam`, `pad`, `targets`, `targetObjs`, `playerSelectInfo`, `abilityTable`, `hairStyleSetting` and friends,
`playerSelectSoundLump` and `hudFooterUni`. Same-name hazard: the image has three `flareAlpha`, two `flareDelta` and two
`firstTime` objects; `w9-frontend-d-run-browser` already imports an `int flareAlpha` and `int flareDelta` under the plain names.

## Build-plan entries needed

37 entries, profile `authored_cpp`, `input_order` left to the integrator, generated in this order by
`docs/agents/w14-hud-b-plan-entries.py` (gitignored directory; added with `git add -f`):
`python3 docs/agents/w14-hud-b-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee [--private-root DIR]`.
Thirty-six are sources of this package (12 functions, 24 data objects) and one is the earlier-wave unit
`w9-hud-inventory-items-b-view-exit-sound-resource`. The `sha256` values are of the committed sources at the last
source commit. Do not also select `w9-hud-inventory-items-c-create-random` (it defines `createRandom__4itemPQ24item11descriptioniiii`
again) or `w14-hud-b-*` units together with any other definition of the same data names.

```json
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_body_alpha.o", "profile": "authored_cpp", "sha256": "a427bf429fe179aa32c90585529d9c7aa3586075e1143921238614a40c73dd6a", "source": "sources/w14-hud-b-data-body-alpha/data_body_alpha.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_char_delta.o", "profile": "authored_cpp", "sha256": "84a86307487cb6590f72fed3ace11a724ac82303373c42a80cb91058ea7d9b8e", "source": "sources/w14-hud-b-data-char-delta/data_char_delta.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_cur_slider.o", "profile": "authored_cpp", "sha256": "98e6ce763016cf0cd27afea14a89428d37e5961b47bc2cc3ecf6fe6364dc7ef3", "source": "sources/w14-hud-b-data-cur-slider/data_cur_slider.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_cur_stat.o", "profile": "authored_cpp", "sha256": "239e722bf01884a1558e7bcc76e74d85cf5791625a836f7ffd7bd401b00e6135", "source": "sources/w14-hud-b-data-cur-stat/data_cur_stat.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_last_slider.o", "profile": "authored_cpp", "sha256": "d1baea7ad742f8b9bc40d4862dfed483d115b949c580c01cdf30a4bc5bb3bfee", "source": "sources/w14-hud-b-data-last-slider/data_last_slider.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_max_slider.o", "profile": "authored_cpp", "sha256": "419647e016d7450e0294acede9807a920a72e6a0c04303335dd98ed0203fa814", "source": "sources/w14-hud-b-data-max-slider/data_max_slider.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_msg_alpha.o", "profile": "authored_cpp", "sha256": "86f45597c0ba304a9f1f0dd93ed0c05aaf00964b318e5cfad4ce0bfef04806bd", "source": "sources/w14-hud-b-data-msg-alpha/data_msg_alpha.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_msg_alpha_delta.o", "profile": "authored_cpp", "sha256": "9f4534fdee5f13af13625bb9d7b6590dde917c0b7e0a4e807e7d646e4fd5ce34", "source": "sources/w14-hud-b-data-msg-alpha-delta/data_msg_alpha_delta.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_new_mode.o", "profile": "authored_cpp", "sha256": "670e602b3026bf10303ffa1999ad487e0651f2a1562e9dc0c8bcc831b95e0f18", "source": "sources/w14-hud-b-data-new-mode/data_new_mode.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_other_hermite_time.o", "profile": "authored_cpp", "sha256": "bf57392e40b0952f1c4bdfc2f2811ba568cf094db4b0b00bbee7cfcf96ad1fe3", "source": "sources/w14-hud-b-data-other-hermite-time/data_other_hermite_time.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_player_gold_icon_x.o", "profile": "authored_cpp", "sha256": "7b609873a8dc0069aa2f4169ee0ca27786cc325110e925684eabf624db0c915a", "source": "sources/w14-hud-b-data-player-gold-icon-x/data_player_gold_icon_x.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_player_select_view_486d3c.o", "profile": "authored_cpp", "sha256": "1e16532d731e9572c70fa0212d7ece4e05033371966a65817748fb2a728a7954", "source": "sources/w14-hud-b-data-player-select-view-486d3c/data_norrath_player_select_view_486d3c.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_player_select_view_486d44.o", "profile": "authored_cpp", "sha256": "f01637144fa840e91ca37dda9d1ba7a7895a513ca88ece4bc43be59ad9a011e5", "source": "sources/w14-hud-b-data-player-select-view-486d44/data_norrath_player_select_view_486d44.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_player_select_view_486d88.o", "profile": "authored_cpp", "sha256": "d33d8d7dd3fa4d265f1bfcb1321d2263fef971ab8a90e67bf2e32cf64afcf0d7", "source": "sources/w14-hud-b-data-player-select-view-486d88/data_norrath_player_select_view_486d88.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_scrollmap_tex_copy.o", "profile": "authored_cpp", "sha256": "eed6065c70540a536b8e1e4936b31d23731c2b158e15de2617b17026135a659d", "source": "sources/w14-hud-b-data-scrollmap-tex-copy/data_scrollmap_tex_copy.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_stat_alloc_warn.o", "profile": "authored_cpp", "sha256": "d362ae73c65671e93b4905ab1c643eb00e18e80e795d7f90746b88bc69e21d9c", "source": "sources/w14-hud-b-data-stat-alloc-warn/data_stat_alloc_warn.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_stat_allocation.o", "profile": "authored_cpp", "sha256": "423d294f8ef928c56231e3c9f770736364588fd26edc32820fd8254314626519", "source": "sources/w14-hud-b-data-stat-allocation/data_stat_allocation.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_stat_table.o", "profile": "authored_cpp", "sha256": "296e9af137de28e04a23f8fe9419082391bd8648e69e98ee2a8f02d0387b4ba3", "source": "sources/w14-hud-b-data-stat-table/data_stat_table.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_throb.o", "profile": "authored_cpp", "sha256": "bb058da2a19268f34dc8248fcbb8d10b8151642e0b5c71762b2efe47bd8b5264", "source": "sources/w14-hud-b-data-throb/data_throb.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_throb_a.o", "profile": "authored_cpp", "sha256": "6edb8e0b51986c174cda0ed5be3be48068e3662fd87090832c604222c8058266", "source": "sources/w14-hud-b-data-throb-a/data_throb_a.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_throb_b.o", "profile": "authored_cpp", "sha256": "76062a75417379d3b52ffdf1f63c5710565fcbc254392fe8b00c331c68ac693d", "source": "sources/w14-hud-b-data-throb-b/data_throb_b.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_throb_g.o", "profile": "authored_cpp", "sha256": "e8393f5d66cbbcaefaf067bc50fe27088621e481393299ad7097ebb910ad1f91", "source": "sources/w14-hud-b-data-throb-g/data_throb_g.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_throb_r.o", "profile": "authored_cpp", "sha256": "997344d5cecc4d9c8defaa59ca717e81990e694629a1c1555aeb1064c50b403b", "source": "sources/w14-hud-b-data-throb-r/data_throb_r.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_data_titleload_tex_copy.o", "profile": "authored_cpp", "sha256": "fdedbe5b01258fb2f548d9992dff72b3cc45806509654e4622d7fc0d985b1b33", "source": "sources/w14-hud-b-data-titleload-tex-copy/data_titleload_tex_copy.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "msgHudAssignQuickSpell__Fiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_msg_hud_assign_quick_spell.o", "profile": "authored_cpp", "sha256": "036eb2fcd0fe64ebb0744c68393e6e0670d0d5fec9d54b5d3b24a7ef658d0c70", "source": "sources/w14-hud-b-msg-hud-assign-quick-spell/msg_hud_assign_quick_spell.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "msgHudAssignSpellBookSpell__Fiiil"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_msg_hud_assign_spell_book_spell.o", "profile": "authored_cpp", "sha256": "9e2f65d9776bc9be6a98d66b8bcb48895772571468f7cec94f0ac2a4fc91be9a", "source": "sources/w14-hud-b-msg-hud-assign-spell-book-spell/msg_hud_assign_spell_book_spell.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 720, "name": "objectPutSaveData__FPci"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_object_put_save_data.o", "profile": "authored_cpp", "sha256": "e52f54a505f4e2db77fbd2afcc8275b0d28a23421649b4614980ba2754a30884", "source": "sources/w14-hud-b-object-put-save-data/object_put_save_data.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1280, "name": "persue2__8CreatureP10GameObjectfff"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_persue2.o", "profile": "authored_cpp", "sha256": "1651b39cffb1ad3e78c5c0ab34b3cbd73db2de069149d2c30fc7be2dce1107b8", "source": "sources/w14-hud-b-persue2/persue2.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "picAdd__4itemPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_pic_add.o", "profile": "authored_cpp", "sha256": "ebbb17daf2276fbc5837861f97911a11b8d730084491434d2d852c8a32e0d458", "source": "sources/w14-hud-b-pic-add/pic_add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 556, "name": "playerInitArmorCache__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_player_init_armor_cache.o", "profile": "authored_cpp", "sha256": "3dd7c15d1e6a94ee291a24752dcc982ed4d866fa619f1ba86cad836fbc474be1", "source": "sources/w14-hud-b-player-init-armor-cache/player_init_armor_cache.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "_ZN12_GLOBAL__N_1L13playMenuSoundEPKc"}, {"compiled_body_bytes": 152, "name": "_ZN12_GLOBAL__N_1L12transitionToEiPKcfi"}, {"compiled_body_bytes": 12576, "name": "playerSelectionTask__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_player_selection_task.o", "profile": "authored_cpp", "sha256": "e140b976310d14ee44eca433c99d21dbd488fbb9833d488f47fa2a1367a5d031", "source": "sources/w14-hud-b-player-selection-task/player_selection_task.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 368, "name": "readMap__3mapPUc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_read_map.o", "profile": "authored_cpp", "sha256": "937d0846e3cd64ed23f5a87e7c4bb0ef1ca858816e45aa4796ecc31d0eab5b29", "source": "sources/w14-hud-b-read-map/read_map.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 4300, "name": "runStats__Fii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_run_stats.o", "profile": "authored_cpp", "sha256": "597df628d458f21feb0788dc6abae7beeccf4764b567107d93f43dcecf2916a6", "source": "sources/w14-hud-b-run-stats/run_stats.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "shopClipGridCoordinates__FPiN30"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_shop_clip_grid_coordinates.o", "profile": "authored_cpp", "sha256": "1ae7bc4bcc15104c5ef0640abb828779ba1c60c8138d36e65002e7ed178b8809", "source": "sources/w14-hud-b-shop-clip-grid-coordinates/shop_clip_grid_coordinates.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 368, "name": "shopDrawItemBitmapGlow__FP8_textureii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_shop_draw_item_bitmap_glow.o", "profile": "authored_cpp", "sha256": "de1fe55e75f6445115f3ba573f3cd88e10f855e8acc939bbb4ee659178f37eac", "source": "sources/w14-hud-b-shop-draw-item-bitmap-glow/shop_draw_item_bitmap_glow.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "shopMouseOnHotSpot__FP12screenStruct"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_shop_mouse_on_hot_spot.o", "profile": "authored_cpp", "sha256": "a52ec29d60129da0390498d87082afe8df8329528b3401e5346fc24ea5e6f378", "source": "sources/w14-hud-b-shop-mouse-on-hot-spot/shop_mouse_on_hot_spot.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14hb_w9_hud_inventory_items_b_view_exit_sound_resource.o", "profile": "authored_cpp", "sha256": "a690c5595078ef6f4b3bd0078aff01b135f41e6d2776bb01a988bb30dbc0c800", "source": "sources/w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp"}
```

## Authored but never executed

Everything. Nothing ran on the console or as gameplay. All functions ran only in the private R5900 interpreter against the original
code with stand-in callees (so none of the callees' real behavior was exercised: `hudRunSlider__FiiiiiUifPii` and its out-pointer,
`doNamePad__Fv`, the soft-float and `expf`/`sinf` library routines other than the ones named above, the vtable entries of
`objectPutSaveData` and `persue2`, `textGetString__FPc`, the fade and font functions), and in the x86_64 host checks. Layout facts
that only the callees would confirm (the `screenStruct` fields, the creature table slot at +176, the `deleteList` node
layout, 12-byte equipment group records) were taken from the original's reads and writes, not from their owners. The
data objects were never compared with a running image; their extents are the gaps to the next symbol.

## Unknown

- Field names and meanings: `screenStruct`, the player record fields used by `runStats`, the 636-byte `targets` records and 41-byte
  `playerSelectInfo` records, the creature fields used by `persue2` (+176 slot, +260 route point, +712 stamp, +760 steering, +800),
  the 88-byte armour cache record byte at offset 84, the object-save record's tail, and the `lightStart` defaults.
- The numeric constants of the UI code (positions, colours, 0.077-style gp floats, 13.723, 0.064, the 1000000 / 100000 / 10000 text widths).
- Whether the `-100, -100` clip arguments of `fontPrintWrapu` really make the first three calls of the character page a measuring pass.
- Why `playerSelectionTask`'s mode 7 and 8 do not draw (the tail returns for modes 6 and 7).
- `routePlot__FR10RouteQueue` (0x002c0a10, 19,812 bytes, about 4,950 instructions): inspected, not authored. It walks a table of 16
  route links (20-byte records), an 8-entry cluster cache of 32-byte records and a pool of 64-byte graph nodes (`freeroot`, `nodeused`,
  `nodecount`), with the page-eviction and free-node block inlined four times, `qsort(rlcompare)`, three `swap<>` template
  instances, hit-scan ray tests and soft-double distance sums, and an A*-style open list. A paired run needs a consistent
  node/cluster/link graph, because inputs that are not self-consistent make the original loop or walk off the pools; building
  that environment model is a project of its own and it is the one name left from the list for that reason.
