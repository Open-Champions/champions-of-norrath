# w9-hud-inventory-items-a: HUD, inventory, shop, item and fonts (a)

Branch `agents/w9-hud-inventory-items-a`. Private reference: `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). All
addresses are that image's symbol values. No game bytes or disassembly are committed;
descriptions are my own. The earlier interrupted run's commits were reviewed and kept; its
uncommitted `drinkPotion` draft was verified and committed.

How each function was checked: it compiles with the reviewed EE `authored_cpp` flags and
each has a `host_check.cpp` (not a plan input) that returns non-zero on a wrong result.
In addition each function was run against the original in a private R5900 interpreter
(kept outside git, one scenario set per function), from identical memory snapshots with
identical stub callees, comparing the call log (arguments and order), every changed byte
outside the stack, and instruction coverage of the original. Final scenario counts
and results: `drinkPotion` 2500 scenarios, 0 differences, 515 of 522 instructions covered
(the rest are branches the image has for a case that cannot occur); `getHairAndSkin` 1200
scenarios, 0 differences, 277 of 280 instructions (the rest are padding no-ops).
Earlier functions in the list were compared the same way before this session. These are
behavior reconstructions, not byte matches.

## Functions authored (sources under `experimental/sources/`)

| directory | symbol | image |
|---|---|---|
| `w9-hud-inventory-items-a-game-difficulty-scale` | `gameDifficultyScale__Fv` | |
| `w9-hud-inventory-items-a-game-treasure-ramp` | `gameTreasureRamp__Fv` | |
| `w9-hud-inventory-items-a-compare-to-item` | `compareToItem__Q24item11descriptionPQ24item11description` | |
| `w9-hud-inventory-items-a-game-button-to-icon` | `gameButtonToIcon__Fi` | |
| `w9-hud-inventory-items-a-change-hair-style` | `changeHairStyle__Fii` | 0x27ec78 |
| `w9-hud-inventory-items-a-change-hair-color` | `changeHairColor__Fii` | |
| `w9-hud-inventory-items-a-change-beard` | `changeBeard__Fii` | |
| `w9-hud-inventory-items-a-change-skin` | `changeSkin__Fii` | 0x27eaf0 |
| `w9-hud-inventory-items-a-font-print-wrap-centeredu` | `fontPrintWrapCenteredu__FiiPUsiiii` | |
| `w9-hud-inventory-items-a-font-print-wrap-centered` | `fontPrintWrapCentered__FiiPciiii` | |
| `w9-hud-inventory-items-a-cache-equipment` | `cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture` | 0x1959a0 (local symbol) |
| `w9-hud-inventory-items-a-get-best-slot-name` | `getBestSlotName__FPc` | 0x25ba20 |
| `w9-hud-inventory-items-a-drink-potion` | `drinkPotion__6Playeriiii` | 0x1a8358, 2088 bytes |
| `w9-hud-inventory-items-a-get-hair-and-skin` | `getHairAndSkin__Fi` | 0x27ed50, 1120 bytes |
| `w9-hud-inventory-items-a-create-random` | `createRandom__4itemPQ24item11descriptioniiii` | 0x104440, 4220 bytes |

## Data objects defined

| directory | symbols |
|---|---|
| `w9-hud-inventory-items-a-skill-trees` | `WarriorSkillTree`, `ClericSkillTree`, `WizardSkillTree`, `RangerSkillTree`, `ShadowKnightSkillTree`, `gSkillTree` (byte-identical to the image) |
| `w9-hud-inventory-items-a-appearance-settings` | `hairStyleSetting`, `hairColorSetting`, `beardSetting`, `skinSetting` (local BSS, 16 bytes apart) |
| `w9-hud-inventory-items-a-hud-words` | `fontCharAlpha`, `cacheLoadCache`, `finishFrameCount`, `currScorePos`, `currActivityScore`, `curFont`, `coins`, `countUp` |
| `w9-hud-inventory-items-a-boss-meter-words` | `bossMeterCurrent`, `bossMeterMax` (floats), `bossMeterAlpha` (int); initial value 0 |

## Observed, inferred, unknown for the last two functions

`drinkPotion`: observed the control flow, the two potion name lists (nine names each),
the 72-byte inventory rows at record+9560, the 23984-byte player records, the
5400/72-byte grid addressing and the callee order. Inferred that the record words at
+172/176/180 and +196/200/204 are current/maximum/pending of two stats (health and a
second one). Unknown: what kinds above 1 mean, and names of the row sizes.

`getHairAndSkin`: observed the 636-byte record layout used (offsets in the source
header), the file-name patterns, and that no loop is bounded (a record with more than
eight hair or beard entries would write past its arrays, as the image would). Inferred
the +17 byte value 4 selects a cloth armor set. Unknown: meaning of that value, what
the +20 name is, and the text buffer size (128 bytes here).

## Not authored, with reasons

- `create__4itemPcPQ24item11descriptionii` (0x105f60, 5428 bytes) is already authored as
  `norrath_player_create_item` in `game-item-create-build-v2` (see
  `docs/agents/w8-hud-text-items-a.md`). It is the same function under its mangled name;
  I did not write a duplicate. The integrator should alias the two names, or
  that package should export the mangled name.
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` (0x1ce218, 9184
  bytes): not attempted; it is the largest function in the package and its callers are
  outside it (Trap run and the auto prop run units).

## createRandom

Compared against the image over 300 scenarios (before a readability refactor) and 120 (after),
0 differences in call log (create, getValue, powf, strstr, strcpy, strcat with arguments),
changed bytes and result; 1043 of 1055 instructions covered. Scenarios mutate the real item,
prefix and suffix tables (flags, categories, masks, null texts, levels). Uncovered: the empty
prefix-table case, the path where mode 2 meets a non-"potion" row with a set suffix text
(the tables rarely allow it; covered only by the mutated runs partly), and padding.
Observed: table layouts (item rows 64 bytes, prefix 40, suffix 20), random step formula,
all branches. Unknown: meaning of modes, categories, flag bits, kinds.

## End summary

- Imports closed: ClericSkillTree, RangerSkillTree, ShadowKnightSkillTree, WarriorSkillTree,
  WizardSkillTree, gSkillTree, bossMeterAlpha, bossMeterCurrent, bossMeterMax,
  cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture, cacheLoadCache,
  changeBeard__Fii, changeHairColor__Fii, changeHairStyle__Fii, changeSkin__Fii, coins,
  compareToItem__Q24item11descriptionPQ24item11description, countUp,
  createRandom__4itemPQ24item11descriptioniiii, curFont, currActivityScore, currScorePos,
  drinkPotion__6Playeriiii, finishFrameCount, fontCharAlpha,
  fontPrintWrapCentered__FiiPciiii, fontPrintWrapCenteredu__FiiPUsiiii, gameButtonToIcon__Fi,
  gameDifficultyScale__Fv, gameTreasureRamp__Fv, getBestSlotName__FPc, getHairAndSkin__Fi.
- Still open: `create__4itemPcPQ24item11descriptionii` (same function as `norrath_player_create_item`;
  needs an alias) and `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`
  (0x1ce218, 9184 bytes, not attempted: it is by far the largest function and I judged it
  would not get a verified result in this sitting).
- Imports opened: `_4item$itemTable` (authored by items-player-item-table as
  `norrath_item_table_view`; needs alias), `_4item$prefixTable`, `_4item$suffixTable`,
  `getValue__4itemPQ24item11description`, `powf`, `strstr`, `strcpy`, `strcat`,
  `syncRandState`, `nmPlayers`, `player`, `getVifAndTex__FPcT0PP12_modelHeaderPP8_texture`,
  `lumpFindResource__FPcT0`, `npcLumpName` (local symbol), `targets`, `targetObjs`,
  `sortedDir`, `sortedDirCount`, `scriptGetGameState__FPc`, `scriptSetGameState__FPci`,
  `textGetString__FPc`, `multiHudAddHelpMessage__FiPUsT1i`, `hudGetFadeIconPos__Fii`,
  `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `netPlayerIsLocal__Fi`,
  `msgInventoryGridDropItem__Fiiciiii`, `removeItemFromInventory__Fiiii`,
  `playerGetPotionsLeft__Fii`, `playerRecomputeWeight__Fi`, `fadeTimer`, `fadeIconY`,
  `invGrid`, `interfaceOn`, plus those of the earlier font/cache units.
- Build-plan entries needed: one `authored_cpp` entry per directory listed above (sources
  `<dir>/<main>.cpp`; `host_check.cpp` files are not inputs). I did not edit the plan.
- Authored but never executed on hardware: all of it. Only host checks and the private
  paired interpreter runs were done.
- Unknown: meanings noted in each source header.
