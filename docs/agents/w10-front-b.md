# w10-front-b: front end, save and player select, part 2

Package notes. All addresses are from the pinned SLUS-20565 image read with the EE binutils.
Nothing here has run on an EE or as gameplay. Functions were compared against the image on a
private R5900 interpreter (outside git); host checks cover each unit on the build machine.

## Functions

| Function | Image | Authored | Check |
|---|---|---|---|
| `playerCountChanged__6Camera` | 0x0026fde0, 16 B | 16 B | host check; paired run, 5 scenarios, memory and registers equal |
| `resetMenu__FP12screenStruct` | 0x001bbf28, 32 B | 32 B | host check; paired run, 5 scenarios |
| `playerPrintPaperDoll__Fi` | 0x001ac258, 8 B | 8 B | host check; paired run |
| `skirmishInit__Fv` | 0x002bfbc0, 8 B | 8 B | host check; paired run |
| `saveLevelDataASync__FPci` | 0x0025b5a8, 160 B | 164 B | host check; paired run, 5 scenarios |

Observed: the first four are as the comments in each source say (a float 1.0 store at +200; seven
zero stores; two bare returns). `saveLevelDataASync__FPci` is file-local in the image. It gets the
level name (unused afterwards), has `objectGetSaveData__FPci` fill `destination + 8`, writes that
byte count unaligned at `destination`, has `floorGetSaveInfo__FPci` fill the region after it,
writes that count unaligned 8 bytes later and ends with `map::writeMap`. The file-local word
`bufferBytes` (0x00486c7c) is written once and read back; no other function touches it. The
authored version keeps that word and the stack word `volatile` so the stores and reloads remain;
that is why it is 4 bytes longer. Only the address of the function-local static differs from the
original in the paired run (the original's is at 0x00486c7c). None of these are byte matches.

Inferred: the first word and the later count are byte lengths, from their use as pointer advances.
Unknown: what the 8 bytes at `destination + 4` hold (never written), and what is in the 8 bytes
the layout skips after the first block.

## Data

Sizes are ELF symbol sizes where the image gives them, else the gap to the next symbol or the
indexing stride; each choice is stated in the source comment. `chk_data` (private) compared every
compiled size with the image symbol size where one exists; all agree.

| Unit | Names |
|---|---|
| `w10-front-b-multi-help-words` | `multiRelease` (4 B), `multiTitle` (4 pointers), `multiSide` (4 words) |
| `w10-front-b-name-grid-words` | `nameGridW`, `nameGridCount` |
| `w10-front-b-gold-targets` | `playerGoldx` {50, 590, 50, 590}, `playerGoldy` {200, 200, 250, 250} |
| `w10-front-b-race-prefix` | `racePrefix`, ten short resource-name prefixes |
| `w10-front-b-stats-screen` | `statsScreen`, 232 zero bytes (two 116-byte records) |
| `w10-front-b-player-start-words` | `playerStartYaw` (8 B), `playerSpecStorage` (164 B = 4 x 41) |
| `w10-front-b-browser-words` | `slotsCorrupted` (64), `undoQuickLoadStamp` (64, 8-aligned), `sortedPlayerRace` (160 = 10 x 16) |
| `w10-front-b-skin-setting` | `skinSetting` (16 B) |
| `w10-front-b-player-select-words` | `playerMenu` (5 x 16 B), `playerSelectNextName`, `playerSelectPrevName`, `playerRecalled`, `playerSelectCount`, `playerSelectStartPlayer`, `playerSelectEnforce`, `playerSelectExitMessage` |
| `w10-front-b-norrath-views` | `norrath_got_player_start_this_level_bytes`, `norrath_keep_player_positions_bytes`, `norrath_restore_broadcast_mask_word`, `norrath_menu_local_t_004844e8` (1.0f), `norrath_menu_word_00484514` (116) |

Section placement follows the build profile (`-G0`): objects that are initialised zero in the
image's data are zero-initialised here and land in .bss.

### Overlap with earlier, unselected units (integrator decision)

These units were authored before and left out of the plan; my units replace them. Selecting both
sets causes multiple-definition errors.
- `w9-hud-inventory-items-a-appearance-settings` defines `skinSetting` (with `hairStyleSetting`,
  `hairColorSetting`, `beardSetting`, which are not in this package). Select either that unit or
  `w10-front-b-skin-setting` plus an equivalent for the other three.
- `w8-net-play-c-player-select-state` defines the player-select names, but also `worldCharViewMatrix`,
  `recallEnter`, `playerSelectSuccessful`, `syncState` which other units own; `w8-net-play-b-launch-state`
  defines `multiRelease`, `multiTitle`, `multiSide` (as byte arrays) beside names others own;
  `w8-frontend-menu-b-menu-words` defines the two menu placeholders; `data-views-symbols/norrath_player_view_484f38`
  defines a differently named view of `playerStartYaw`.
- `norrath_got_player_start_this_level_bytes`, `norrath_keep_player_positions_bytes` and
  `norrath_restore_broadcast_mask_word` are placeholder names for objects that already have
  original-named definitions (`gotPlayerStartThisLevel`, `keepPlayerPositions`, `objectIgnoreFlags`).
  My unit defines them as separate objects; a working executable needs one object under both
  names. This is unresolved here.

## Not done

- `playerSelectionTask__Fv` (0x0027a658, 12200 bytes): read but not authored. It is the whole
  character-creation screen: 3051 instructions, 218 branches, about 165 calls to 40 callees, 60 text
  keys and format strings, and 73 distinct gp-relative state words (most of them file-local statics
  with no other user), plus a sine-driven pulse, Hermite slides and slider handling that call
  `changeSkin`, `changeHairStyle`, `changeHairColor`, `changeBeard`, `doNamePad` and `drawNamePad`.
  A faithful complete function would need dozens of new data definitions whose layout is not
  established and a paired run covering most of the 218 branches; a partial function is not accepted
  under the repo rules. No unit was written.
- `norrath_loader_failure_word_view`: already authored by `level-anim-load-loader-failure-word` as an
  absolute symbol at zero (the loader stores to address 0); the integrator has left it out pending a
  decision about absolute symbols. Not duplicated.

## Build-plan entries needed

Append in this order, profile `authored_cpp`, empty header closure. Each source and its
`host_check.cpp` must also be recorded in `snapshot.json`. Do not plan the host files. Checked with
a relocatable link of the 15 objects against the unresolved-import baseline (`plan10-build`): no
multiple definitions.

| Object | Source | SHA-256 |
|---|---|---|
| `browser_words.o` | `sources/w10-front-b-browser-words/browser_words.cpp` | `f32735b8a175d269a681852b9a22ba93a236abd694da5fc0273665b81a0682cc` |
| `player_count_changed.o` | `sources/w10-front-b-camera-player-count-changed/player_count_changed.cpp` | `4c764382dfe7effe4845f2d8e21663469cee797b4f5dbf2f40053b884d7ed42e` |
| `gold_targets.o` | `sources/w10-front-b-gold-targets/gold_targets.cpp` | `95da0b0785d63a561b1e44666b52c6ccef067271138ea31a276323c45cc2252c` |
| `multi_help_words.o` | `sources/w10-front-b-multi-help-words/multi_help_words.cpp` | `e7888ea73a0c6a56bbf12edf367f46eb5500f424937d8c344d40549ee14f6cde` |
| `name_grid_words.o` | `sources/w10-front-b-name-grid-words/name_grid_words.cpp` | `c7c1d835c45d7c62b68e2fb1bfe2e3df3e3998f8628aee6208f424b611f78abb` |
| `norrath_views.o` | `sources/w10-front-b-norrath-views/norrath_views.cpp` | `44fcb58a911457905943da6d53b402bc2994b94239b2bee1f42f6112dfc2d3fe` |
| `player_print_paper_doll.o` | `sources/w10-front-b-player-print-paper-doll/player_print_paper_doll.cpp` | `de58835120816a521aed7052322cc9e32c0978976bfd96d59b1aa34f20920bf5` |
| `player_select_words.o` | `sources/w10-front-b-player-select-words/player_select_words.cpp` | `a7f8ad764940fd10dd5d6d5a8b220b86fac026ba89e567015cf0832b3df99f6e` |
| `player_start_words.o` | `sources/w10-front-b-player-start-words/player_start_words.cpp` | `acd4c62e48b4f91ece902b38b3725687cd48d2c0f343df769dae5ec2d658ea5a` |
| `race_prefix.o` | `sources/w10-front-b-race-prefix/race_prefix.cpp` | `45a3c2b28af81900148b24db4648f812380077d0091fb9de0162cf5624f604e4` |
| `reset_menu.o` | `sources/w10-front-b-reset-menu/reset_menu.cpp` | `924b765a6a68aece3095d4d2bdcc0ba16b03cede7c4627cf33cf79a6bd7a3703` |
| `save_level_data_async.o` | `sources/w10-front-b-save-level-data-async/save_level_data_async.cpp` | `761b87ada870cb142b2c207d932a0246cfa6ba16da5167cd2184d3d5e425a018` |
| `skin_setting.o` | `sources/w10-front-b-skin-setting/skin_setting.cpp` | `324bcc83989c3120efa2c9fcf838f9a8db84cde518df484016aacee836467bcc` |
| `skirmish_init.o` | `sources/w10-front-b-skirmish-init/skirmish_init.cpp` | `b84b064222edf1e8b398f8fa01a1b632271b40cd47e2731db71e6ae1e3bedd73` |
| `stats_screen.o` | `sources/w10-front-b-stats-screen/stats_screen.cpp` | `33e1789bef8708babd7d80bd7787666e251f3abe28d2d3733a44a29bc88c731e` |

Emitted functions: `playerCountChanged__6Camera` 16, `resetMenu__FP12screenStruct` 32,
`playerPrintPaperDoll__Fi` 8, `skirmishInit__Fv` 8, `saveLevelDataASync__FPci` 164 bytes; the other
ten inputs emit none.

## Summary

Imports closed (33): `multiRelease`, `multiSide`, `multiTitle`, `nameGridCount`, `nameGridW`,
`norrath_got_player_start_this_level_bytes`, `norrath_keep_player_positions_bytes`,
`norrath_menu_local_t_004844e8`, `norrath_menu_word_00484514`, `norrath_restore_broadcast_mask_word`,
`playerCountChanged__6Camera`, `playerGoldx`, `playerGoldy`, `playerMenu`,
`playerPrintPaperDoll__Fi`, `playerRecalled`, `playerSelectCount`, `playerSelectEnforce`,
`playerSelectExitMessage`, `playerSelectNextName`, `playerSelectPrevName`,
`playerSelectStartPlayer`, `playerSpecStorage`, `playerStartYaw`, `racePrefix`,
`resetMenu__FP12screenStruct`, `saveLevelDataASync__FPci`, `skinSetting`, `skirmishInit__Fv`,
`slotsCorrupted`, `sortedPlayerRace`, `statsScreen`, `undoQuickLoadStamp`.
Unresolved count against the baseline: 1332 to 1302 in a private relocatable link.

Imports still open from the scope: `playerSelectionTask__Fv`, `norrath_loader_failure_word_view`.

Imports opened (3): `floorGetSaveInfo__FPci`, `objectGetSaveData__FPci`, `writeMap__3mapPUc`.

Authored but never executed: all of it. Functions ran only on the private interpreter and host
checks with stubbed callees; data was only compiled and compared by size and value.

Unknown: the meaning of every data object's contents beyond the access widths; the 8 bytes skipped
in the save block; whether `bufferBytes` is read by anything not in the image's symbol table; the
element counts beyond the observed indexing for `multi*`, `skinSetting`, `slotsCorrupted`; how the
placeholder-named byte flags and the original-named ones become one object.
