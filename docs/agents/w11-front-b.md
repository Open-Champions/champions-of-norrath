# w11-front-b: front-end, HUD and inventory (2 of 3)

Package `w11-front-b`, branch `agents/w11-front-b`. Private reference: the pinned
SLUS-20565 image at `work/private/SLUS_205.65`; addresses below are its symbol values and
instruction operands. No game bytes or listings are committed; all text is my own description.
Every function was compiled with the reviewed EE g++ `authored_cpp` flags and has a host
check (`host_check.cpp`, not a plan input) that exits non-zero on a wrong result. The larger
ones (`objectGetSaveData__FPci`, `mainMenu__Fii`) were also run against mutated copies of
the source and the checks failed for each mutation tried. Compiled bodies differ in size from
the image extents; these are behaviour reconstructions, not original-byte matches. Nothing
here ran on the EE or as gameplay.

## Baseline

A default authored partial build of this branch's base (plan of 4454 authored inputs) gave
957 unresolved imports; the 26 names of this package were all in that list. `loadPlayerLumps__Fv`,
`hairColorSetting`, `hairStyleSetting`, `invHeader`, `lockAlpha`, `multiBody` and
`multiHelpMessageOn` were unresolved only because their authored sources (earlier waves) are
not in the plan yet; see "Authored by others, not yet planned".

## Functions

| symbol | image | compiled | what it does |
|---|---|---|---|
| `getDamageMultiplier__4itemPQ24item11description` | `0x108230`, 16 bytes | 12 | returns 1.0f, argument unused |
| `msgInventoryExit__Fiii` | `0x1b17d0`, 56 | 72 | sends `4, a, b, c` on `invMsgHandle` |
| `msgHudSavePoint__FP9Savepoint` | `0x1c7930`, 60 | 60 | sends `4,4,4,4` + the pointer on `hudMsgHandle` (8 bytes) |
| `writeMap__3mapPUc` | `0x15e010`, 88 | 96 | packs the high nibble of every second tile (even rows, every fourth tile and the one two columns on) of `_3map$maptiles` into 5832 bytes |
| `getPicSize__4itemiPQ24item11descriptionPiT3` | `0x108428`, 112 | 116 | `picFind(getPicName(..))`, halfwords at +6 and +8 of the record, zeros when missing |
| `inventoryPadFindButton__Fi` | `0x1b1e50`, 180 | 244 | first local player with float at record+172 above zero whose pad word at +12 has a bit of the mask; -1 if none |
| `objectGetSaveData__FPci` | `0x142ce8`, 508 | 488 | tries passes 0,1,2.. of the local data collector until the object data and string pool fit; writes the 8-byte header, data, pool; returns the aligned length, -1 or -2 |
| `mainMenu__Fii` | `0x1b2cf0`, 1296 | 1428 | the shop Buy/Sell menu (shop mode 3): button plates, two labels with highlight, accept/back handling, and the bobbing arrow pair using soft doubles |

`getDamageMultiplier` and the data objects below are single-purpose; the checks are described
in each file header and its `host_check.cpp`.

## Data objects

| symbol | image | what it is |
|---|---|---|
| `iconName` | `iconName.1132` at `0x4271b8`, 36 bytes | nine string pointers: ab, critical, damage, eletric, flaming, ice, weight, gp, blunt. Defined under the plain name the authored consumer imports; the image name has the compiler's local suffix |
| `moneyTex`, `lcursor`, `iconLump`, `hudLump` | `0x427268` (28), `0x4848e4`, `0x486a80`, `0x486a84` | texture and lump pointer words, zero at start |
| `globalIcons` | `0x42a418`, 152 bytes | zeroed object |
| `hudBustCovered` | `0x484af9`, 1 byte | zero |
| `p1`, `p2` | `0x8d5938`, `0x8d5948` | 16-byte point slots (alignment inferred from the 16-byte spacing) |
| `partlist.1103` | `0x7d8010`, extent to next symbol 176 | zeroed object; only an 8-byte zero store was seen |
| `list.1073` | `0x4294b0`, 3780 bytes | 63 level records of 60 bytes (62 names and a null-named default record) with the fields `gameLoadWorld__FPc` reads, copied from the image; the object file is 3780 bytes and every non-pointer byte equals the image (private comparison) |

## Checks performed

- Each unit: compiled alone with the plan flags (sizes above); host check built with
  `c++ -std=c++17 -Wall -Wextra -Werror` and run, exit code 0.
- `list.1073`: compiled `.data` compared byte by byte (privately) with the image bytes; all 3780
  agree except the 62 name-pointer words, which the linker fills.
- `objectGetSaveData__FPci`: mutated `eligible >= pass` and `!(pass < eligible)`; the host check
  failed (exit 18 and 17).
- `mainMenu__Fii`: mutated the language-4 test, the left arrow offset, the back-button mask, the
  Sell mode store and the 1.5 factor; the host check failed each time (exit 21, 1, 3, 10, 18).
- A private plan (kept outside git) with these 15 units appended passes `--check-plan` and a full
  private build was started; its result is recorded at the end of this file.

## Build-plan entries needed

Append in this order, profile `authored_cpp`, empty header closure; every file must also be
recorded in `snapshot.json`. Do not plan the `host_check.cpp` files.

| object | source (under `sources/`) | SHA-256 |
|---|---|---|
| `w11_front_b_get_damage_multiplier.o` | `w11-front-b-get-damage-multiplier/get_damage_multiplier.cpp` | `de7509124c526d2cc941eaf02f268b4f1c1857af6c926299503ad4aa732872f5` |
| `w11_front_b_msg_inventory_exit.o` | `w11-front-b-msg-inventory-exit/msg_inventory_exit.cpp` | `308765d5eb395c0843855fc7596cf63fee147ed0f25fc6ce4ca38875bda12d71` |
| `w11_front_b_msg_hud_save_point.o` | `w11-front-b-msg-hud-save-point/msg_hud_save_point.cpp` | `6a5f347ad1c286a531c563aa254a7967a94a7224bbb83a22f97d4e8689df0d1f` |
| `w11_front_b_write_map.o` | `w11-front-b-write-map/write_map.cpp` | `7f389daa1b01c10e8732797cf1db7fa26f2d1270c5e3f75f2bcb0f57e618cda2` |
| `w11_front_b_get_pic_size.o` | `w11-front-b-get-pic-size/get_pic_size.cpp` | `62c0bdaf6752ad4fe8d6856a8d49e9bf2c1828b1f8ad1f1917e504a1a1b2673b` |
| `w11_front_b_inventory_pad_find_button.o` | `w11-front-b-inventory-pad-find-button/inventory_pad_find_button.cpp` | `c8a5c11a2ad19361777c1bb940559f08a7b1d3019a657f958ea0e2fb82f32c5c` |
| `w11_front_b_object_get_save_data.o` | `w11-front-b-object-get-save-data/object_get_save_data.cpp` | `372e7fd2756dce6181fca7b4d59a05a72756ebdf41c0508b64e9afb5554653c2` |
| `w11_front_b_main_menu.o` | `w11-front-b-main-menu/main_menu.cpp` | `32cf696400ec18dd38a5ddf209e3d4b1cbba727b36f88b92d3db53d1f810f46b` |
| `w11_front_b_icon_name.o` | `w11-front-b-icon-name/icon_name.cpp` | `c070b2ba4da98825fffe14ec1ceb795b5bb8dd5574bef6f55ee6e5f735ab6f74` |
| `w11_front_b_inventory_texture_words.o` | `w11-front-b-inventory-texture-words/inventory_texture_words.cpp` | `6da00de7a5b2a68fbe2cbd4cff5e35653af8cead343d2ff335078780bdd8f2e3` |
| `w11_front_b_global_icons.o` | `w11-front-b-global-icons/global_icons.cpp` | `fbf1717302cc55047b3d3c6ce6f4b55de47e5d53470005cfc0afa192c1fc64c8` |
| `w11_front_b_hud_bust_covered.o` | `w11-front-b-hud-bust-covered/hud_bust_covered.cpp` | `2cea23a69f1e9681917303f2c3f6ec3d7ca06381a53f5490312aa27ab3d6d559` |
| `w11_front_b_hermite_points.o` | `w11-front-b-hermite-points/hermite_points.cpp` | `ddbfc147c46157e36a8ceedffbc335f38445fdbe1706c245c29f79c0408807ea` |
| `w11_front_b_skelight_partlist.o` | `w11-front-b-skelight-partlist/skelight_partlist.cpp` | `f02be8f598b198668970e75a16afd88c3edc23c7408f76de153d663d02030d7b` |
| `w11_front_b_level_list.o` | `w11-front-b-level-list/level_list.cpp` | `3fabd025beab93e19bdcdd741fcf9c9ebb42b07fe3476e8451efa9351e830565` |

Emitted functions (the other seven inputs emit none):

| object | function | compiled bytes |
|---|---|---|
| `w11_front_b_get_damage_multiplier.o` | getDamageMultiplier__4itemPQ24item11description | 12 |
| `w11_front_b_msg_inventory_exit.o` | msgInventoryExit__Fiii | 72 |
| `w11_front_b_msg_hud_save_point.o` | msgHudSavePoint__FP9Savepoint | 60 |
| `w11_front_b_write_map.o` | writeMap__3mapPUc | 96 |
| `w11_front_b_get_pic_size.o` | getPicSize__4itemiPQ24item11descriptionPiT3 | 116 |
| `w11_front_b_inventory_pad_find_button.o` | inventoryPadFindButton__Fi | 244 |
| `w11_front_b_object_get_save_data.o` | objectGetSaveData__FPci | 488 |
| `w11_front_b_main_menu.o` | mainMenu__Fii | 1428 |

## Authored by others, not yet planned

These closed-by-name imports of this package already have sources in the tree from earlier
waves that are not in `build-plan.json`; I did not duplicate them. They compile (checked alone).

| symbol | source |
|---|---|
| `loadPlayerLumps__Fv` | `sources/w10-front-a-load-player-lumps/load_player_lumps.cpp` |
| `hairStyleSetting`, `hairColorSetting` | `sources/w9-hud-inventory-items-a-appearance-settings/appearance_settings.cpp` |
| `invHeader` | `sources/w10-render-b-data-inventory-textures/data_inventory_textures.cpp` |
| `lockAlpha` | `sources/w10-render-b-data-lock-alpha/data_lock_alpha.cpp` |
| `multiBody`, `multiHelpMessageOn` | `sources/w8-net-play-b-launch-state/launch_state.cpp` |

## Findings worth passing on

- Observed: `runMusic__Fv` zeroes 32 bytes at `0x429440`, which is the 32-byte object
  `worldActivityLevel`, not `0x4296c0`. `w10-audio-a-music-state` names `0x4296c0` (inside
  `list.1073`) and ships `norrath_music_activity_ring_view` with the level-table bytes as its
  initial value; `w8-core-loop-e-run-music` repeats that address. Those two notes look wrong; the
  ring is `worldActivityLevel` (initial bytes all zero). I did not change their files.
- Observed: `getDataForAllObjects__FPciPiiT2T2` (`0x142880`, a local symbol) is the callee of
  `objectGetSaveData__FPci`; no source defines it.

## Imports closed (19)

`getDamageMultiplier__4itemPQ24item11description`, `getPicSize__4itemiPQ24item11descriptionPiT3`,
`globalIcons`, `hudBustCovered`, `hudLump`, `iconLump`, `iconName`, `inventoryPadFindButton__Fi`,
`lcursor`, `list.1073`, `mainMenu__Fii`, `moneyTex`, `msgHudSavePoint__FP9Savepoint`,
`msgInventoryExit__Fiii`, `objectGetSaveData__FPci`, `p1`, `p2`, `partlist.1103`,
`writeMap__3mapPUc`.

The other seven names of the package (`hairColorSetting`, `hairStyleSetting`, `invHeader`,
`loadPlayerLumps__Fv`, `lockAlpha`, `multiBody`, `multiHelpMessageOn`) close once the sources in
the table above are planned.

## Imports opened

`getDataForAllObjects__FPciPiiT2T2`, `shopDrawTaskBar__FP12screenStruct`,
`updateCursor__FiiiiiP12screenStructi` and `sin` (the soft-double sine, called with and returning a
64-bit word). `mainMenu__Fii` also still needs `rcursor` (owned by `w11-front-c`), `litodp` and
`dptoli` (already unresolved).

## Authored but never executed

All eight functions and seven data objects were compiled and exercised only by the host
checks, with mocked callees; none ran on the EE, in a linked executable, or in gameplay.
`mainMenu__Fii` evaluates its bobbing offsets with the host's double sine through mocked
`litodp`, `dpmul`, `fptodp`, `dpadd`, `sin` and `dptoli`; the real soft-double routines were not
involved. The float step constant is the image word `0x3dcccccc`, written as a bit pattern.
`objectGetSaveData__FPci` was checked against a scripted collector, not the real one.

## Unknown

- What `updateCursor__FiiiiiP12screenStructi`'s arguments (the 2, the 39 and the trailing 1) mean,
  what `S+84` is for, and why `language` values 1, 3 and 4 shift the arrows.
- What the "eligible" count of `getDataForAllObjects` means (inferred as objects with flag bits
  `0x3000`), what the "omitted" count is (only written), and what the halfword 1 in the save header
  stands for. A zero eligible count lets an oversized result be written without a room check; this
  is the observed behaviour, not an endorsement.
- The meaning of the float at player record offset 172 (it acts as a liveness test), and the
  name and layout of the pad record beyond the word at +12.
- The meaning of the `list.1073` flag bits, audio halfwords and environment count; the real extent
  and layout of `partlist.1103`; the type of `p1`/`p2` in the original source; what `globalIcons`
  words other than the ones written by `gameInitPostFrontend__Fv` hold.
- The data objects here are `.bss` in the compiled object (zero-initialised); the image keeps
  `moneyTex`, `lcursor`, `globalIcons` and `hudBustCovered` in file-backed `.data`. Section
  placement follows the build profile, as in earlier packages.

## Private build result

With the 15 entries above appended to a private plan and snapshot kept outside git, the default
authored partial build compiled 4469 units and linked once: 3409 function bodies, 942 unresolved
imports (957 before), no COMMON symbols. Closed 19, opened 4 (`getDataForAllObjects__FPciPiiT2T2`,
`shopDrawTaskBar__FP12screenStruct`, `sin`, `updateCursor__FiiiiiP12screenStructi`). This is a
partial relocatable link, not an executable. `python3 -m unittest discover -s tools -p 'test_*.py'`
passes (214 tests).
