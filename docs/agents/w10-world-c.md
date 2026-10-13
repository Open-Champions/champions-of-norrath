# w10-world-c: world, level and game-loop part 3 of 4

Branch `agents/w10-world-c`. Sources are under `experimental/sources/w10-world-c-*`. Original names, addresses and
sizes come from the pinned private `SLUS_205.65` (read locally; no listing is published). Each source opens with its
own observed / inferred / unknown comment. Host checks sit beside each source as `host_check.cpp`; they are not plan
inputs. Every unit compiled with the reviewed EE `authored_cpp` flags. No compiled body is claimed to match the
original bytes, and nothing here ran on the EE or in the game.

## Functions (one commit each)

| Authored symbol | Original | Address | Original size | Compiled size |
|---|---|---|---|---|
| `norrath_game_object_e_rand` | `eRand__10GameObject` (own body, not an alias) | `0x00153fa8` | 40 | 56 |
| `norrath_game_object_find_tag_float` | `findTagFloat__10GameObjectPci` (own body) | `0x001410a0` | 332 | 376 |
| `objectMoveWithWorldCollision__FP6Point3G6Point3fiT0` | same | `0x00141900` | 624 | 520 |
| `objectAddToMediumRunList__FP10GameObject` | same | `0x00143908` | 100 | 216 |
| `objectCreateByName__FPcG6Point3sPPc` | same | `0x00143d38` | 96 | 96 |
| `removeMonster__FP10GameObject` | same (local in the image) | `0x0018e388` | 80 | 68 |
| `mtcInitialize__Fv` | same | `0x00266ee8` | 316 | 316 |
| `patchSortedListWithLocalTime__Fv` | same | `0x002564c8` | 480 | 148 (plus a 572-byte local helper, 720 in all) |
| `makeCollisionList__FP12_worldHeaderffffPP13collisionParti` | same (local) | `0x00175d88` | 596 | 688 |
| `objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc` | same | `0x00269b80` | 1088 | 896 |
| `lumpLoadGlob__FPc` | same | `0x00165f50` | 792 | 784 |

The two `norrath_game_object_*` names are separate complete routines because the existing authored versions
(`w9-animation-object-b-e-rand`, `-find-tag-float`, `w9-animation-object-a-find-tag-float`) define the original names
and were left unselected by the integrator for duplicate definitions; an alias cannot be taken across translation
units, so these two define the import names directly (the same approach the player package used for
`norrath_player_find_tag_float`).

### Observed, per function (short form; the long form is in each source header)

- `objectMoveWithWorldCollision`: up to 11 passes (counter from 10 down to -1). Each pass calls
  `worldClipMotion(world, position, motion * remaining, radius, mask, normal)`, subtracts the returned fraction from
  `remaining`, advances the position, and, while `remaining` is above the float at `0x004802d0` (0.1), turns the motion by
  `normal * (0.1 - dot(normal, motion))` when that is positive. Writes the position back and copies the normal out if a
  pointer was given; returns 1 if any pass turned the motion.
- `objectAddToMediumRunList`: sibling of the slow-run-list routine; 15 heads, cycle word `mediumListCycle`
  (`0x00483ad4`), wraps to 0 when the signed cycle reaches 15.
- `objectCreateByName`: calls the hook word `userCreateByNameFunction` (`0x00486894`) with a fresh copy of the position
  and stores the name pointer at object+52 when the hook returns an object. Returns 0 with no call when the hook is null.
- `removeMonster`: if flag bit 0x20 is set and the signed byte at object+570 is zero, sets bit 4 of that byte, ORs
  0x88000 into the flag word, then calls `objectRemoveFromGrid`.
- `mtcInitialize`: walks up to 8 sixteen-byte records at `mgrg + 784`, picking the first whose kind passes a test
  (kind 1: string equal to `levelName + 161`; kind 3: `atoi(levelName + 161) % modulus == remainder`; other kinds
  chosen outright) and copies two of its words to `mtcExit`/`mtcStart`.
- `patchSortedListWithLocalTime`: for each of a counted run of records in `sortedDir` (64-byte stride) builds two
  8-byte clock images from BCD fields (record bytes 10..14 and 2..6), passes each to `sceScfGetLocalTimefromRTC`, and
  stores the result in `sortedTimes` (16-byte stride; image from bytes 10..14 first).
- `makeCollisionList`: scales the box by the float word `0x3bcccccc`, floors, clamps to the grid in the world header
  (words +8..+20), and walks cells (rows outer) listing parts (36-byte records at header +32) not yet stamped with
  `collisionFrame`, not flagged (halfword +16 bit 2), and overlapping the box; stamps them; null-terminates the list.
- `objectFindEmptySpotMonsterspawn`: 50 attempts; a shift from `eRandState` along `(isin(angle), icos(angle))`, plus a
  unit-sphere point from `syncRandState` scaled by an int; forbidden test, ground-height test (< 6.0 above the centre,
  height not stored), and a box scan of listed objects (reach = radius + float at object+28, squared compare, `<=`).
  The result slot holds the last candidate tried, even after total failure.
- `lumpLoadGlob`: reads a 2048-byte directory of 40-byte records into `globDir`, then for each record allocates, loads
  from successive sector-aligned offsets, runs `initLump`, shrinks the block, and fills a `lumps` entry (renaming
  `gaydark1.lmp`/`gaydark2.lmp` to `faydark1.lmp`/`faydark2.lmp`).

### Inferred

- `mtcInitialize`: the compiled image keeps a first read of `mgrg` in a register across the `strcmp` call and uses it
  afterwards; this version reloads the global instead. They differ only if `strcmp` changes that register or the global.
- `patchSortedListWithLocalTime`: the field mapping is the SCE memory-card directory time stamp and the CD clock image
  by offset; "converts RTC time to local time" is inferred from the callee name only. The original leaves byte 4 of the
  local clock as stack garbage on the first pass; this version starts it at zero and keeps it across passes.
- `objectMoveWithWorldCollision`: the local Point3 `worldClipMotion` writes is read as a surface normal. The original
  copies it out even if `worldClipMotion` never wrote it (stack garbage); this version starts it at zero.
- `objectFindEmptySpotMonsterspawn`: placing a monster; the angle int is read as an angle from the `isin__Fi`/`icos__Fi`
  callee names.

### Unknown

- The meaning of the mask int passed to `worldClipMotion`; the kinds of the `mtcInitialize` records and what `mtcStart`
  and `mtcExit` are for (nothing authored reads them); the identity of the count word at `0x004861e8` (below); why the
  two `gaydark` lumps are renamed; the layout of the objects used by `removeMonster`, `objectCreateByName` and the box
  scan beyond the words named in each header.

## Data (storage units, one commit per group)

| Symbols | Address | Bytes | Initial |
|---|---|---|---|
| `lastkey`, `lasttime` | `0x004865c8`, `0x004865cc` | 4 each | 0 |
| `lastlastCE` | `0x0048662c` | 4 | 0 |
| `loadCompleteFlag`, `loadPage` | `0x00486981`, `0x00486982` | 1, 2 | 0 |
| `loadKillListSize`, `loadSegmentsLoaded` | `0x004840f4`, `0x004840f8` | 4 each | 0 |
| `needToClearCacheBuffer` | `0x00484108` | 1 | 1 |
| `reallyLoad.873` | `0x00484107` | 1 | 0 |
| `loadKillList` | `0x006e9380` | 16 (gap to the next symbol; symbol size is 0) | 0 |
| `nmActors`, `nmAlwaysPages`, `nmChars`, `nmHalos`, `nmWaterPatches` | `0x00486a10`, `0x00486984`, `0x00486998`, `0x00484088`, `0x00484130` | 4 each | 0 |
| `noAdvanceTime`, `resourceFrame`, `routeClearStart` | `0x00483868`, `0x00483f88`, `0x00484190` | 4 each | 0, 1, 0 |
| `progress` | `0x00486ca0` | 4 (a float: `drawProgress` loads it with `lwc1`) | 0 |
| `nullp` | `0x008d5798` | 12 used (16-byte gap) | 0 |
| `pmlistptrs`, `pmlistptrsused` | `0x00435b68`, `0x00485e1c` | 1024, 4 | 0 |
| `loc` | `0x0046cc90` (read-only) | 48 (twelve floats: 0, 0, 200, -120, 180, -120, 0, 0, 0, -120, 0, -140) | as listed |
| `globDir` (opened by `lumpLoadGlob`, defined here) | `0x006d8780` | 2080 (gap to `table`) | 0 |
| `mtcStart`, `mtcExit` (opened by `mtcInitialize`, defined here) | `0x00485c78`, `0x00485c7c` | 4 each | 0 |

`reallyLoad.873` is a function-local static in the image; the dotted name is the image's own symbol and is reproduced
with an `__asm__` label. `resourceFrame` is also viewed at the same address by the constructor packages' view units
(`norrath_ctors_*_view_483f88`); those are separate symbols and are untouched.

## Skipped, with reasons

- `list.1073` (`0x004294b0`, 3780 bytes, writable initialized data): 63 sixty-byte level records holding floats, ints
  and absolute pointers into read-only strings. Reproducing it means publishing a block of extracted game data plus
  relocations to string targets this package does not own. Left unresolved. `game_load_world` imports it as
  `levelList` through an `__asm__("list.1073")` label and `run_music` zeroes 32 bytes inside it.
- `osdsrc` (`0x00444a38`, 1960 bytes): the bytes are EE machine code (an embedded program), so publishing them would be
  publishing a binary blob. Left unresolved. Needs an owner decision on how embedded code images are represented.

## Checks performed

- Each function: host contract compiled with `c++ -std=c++17 -Wall` (`-ffp-contract=off -DNORRATH_HOST_ARENA` for the
  monster-spawn spot check, `-DNORRATH_HOST_CONTRACT` for `lumpLoadGlob`) and run to exit code 0.
- Mutation checks, each of which failed at least one numbered check: for every function, edits to the constants,
  comparison directions, store offsets, loop bounds or call arguments (about 8 per function). Two survivors were
  found and closed by adding cases (the scale constant and the low-edge clamps in `makeCollisionList`; the year
  modulus in `patchSortedListWithLocalTime`).
- Each source compiled with the reviewed EE g++ and the `authored_cpp` flag set (copied from `build-plan.json`);
  storage units checked with `nm -S` for symbol, size and section (`D` for the two nonzero initial values, `R` for `loc`).
- Quoted-include closure and file hashes checked with `build_partial.py --check-plan` on a private copy of the plan and
  snapshot (kept outside git, with the 22 entries below appended): `source_plan_valid`, 3496 selected inputs.
- Link check: the existing `plan10-build` partial object (1332 unresolved) plus these 22 objects, linked with
  `ld -m elf32lr5900 -r --warn-common`: no duplicate definitions; unresolved count 1332 -> 1300. A full 3496-unit
  rebuild was not run (one build at a time on a shared machine).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Imports closed

`lastkey`, `lastlastCE`, `lasttime`, `loadCompleteFlag`, `loadKillList`, `loadKillListSize`, `loadPage`,
`loadSegmentsLoaded`, `loc`, `lumpLoadGlob__FPc`,
`makeCollisionList__FP12_worldHeaderffffPP13collisionParti`, `mtcInitialize__Fv`, `needToClearCacheBuffer`, `nmActors`,
`nmAlwaysPages`, `nmChars`, `nmHalos`, `nmWaterPatches`, `noAdvanceTime`, `norrath_game_object_e_rand`,
`norrath_game_object_find_tag_float`, `nullp`, `objectAddToMediumRunList__FP10GameObject`,
`objectCreateByName__FPcG6Point3sPPc`, `objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc`,
`objectMoveWithWorldCollision__FP6Point3G6Point3fiT0`, `patchSortedListWithLocalTime__Fv`, `pmlistptrs`,
`pmlistptrsused`, `progress`, `reallyLoad.873`, `removeMonster__FP10GameObject`, `resourceFrame`, `routeClearStart`
(34 of the 36 assigned).

Still open from the assignment: `list.1073`, `osdsrc` (see above).

## Imports opened

- `norrath_world_c_view_4861e8`: the count word at `0x004861e8` (a 4-byte `.sbss` word with no symbol of its own)
  that `patchSortedListWithLocalTime` loops to. It is written at the end of `generateSortedListAndInfoHeaders__Fv`
  (`0x002562a8`, assigned to `w10-world-b`) and read by several memory-card routines (`0x00251600`..`0x0025ba00`). It
  is not `sortedDirCount` (`0x00485be8`). The name is address-based on purpose. Whoever owns the card browser should
  define one symbol for it; do not define a second.
- `sceScfGetLocalTimefromRTC` (`0x002d16a0`, an SCE library call; belongs to the SCE package).

Everything else the new sources reference was already defined in the baseline link (`world`, `worldClipMotion`,
`worldTestForbidden`, `worldFindHeight`, `objectFindInBox`, `isin__Fi`, `icos__Fi`, `eRandState`, `syncRandState`,
`collisionFrame`, `lumps`, `nmLumps`, `noNewVags`, `engineCurrentLevelName`, `sortedDir`, `sortedTimes`, `mgrg`,
`levelName`, `strcmp`, `atoi`, `strcpy`, `cdLoad`, `memAlloc`, `memRealloc`, `initLump`,
`soundGetFreeRam`, `FlushCache`, `listOnewayLink`, `norrath_list_remove`, `floorf`, `objectRemoveFromGrid`,
`mediumRunList`, `mediumListCycle`, `norrath_create_by_name_function_word`), except `_ctype_`, which `mtcInitialize`
reads and which is already an unresolved import owned by a startup package (not new).

## Build-plan entries needed

Append after the current last input, renumbering `input_order` at merge (object names are unique and private to this
package; `kind` is `independently_authored_source`, `profile` is `authored_cpp`). Add each source and header closure to
`files` with the hashes below and the matching snapshot inputs (`component` = the directory name, `emitted_functions` as
listed). The host checks are not inputs. `sources/w8-object-list-math-common/*` and
`sources/w9-animation-object-b-lumps/lumps.hpp` are already in `files`.

| Source | Object | sha256 | Header closure | Emitted functions (bytes) |
|---|---|---|---|---|
| `sources/w10-world-c-game-object-e-rand/e_rand.cpp` | `w10wc_e_rand.o` | `04de79f3b9b951f1ec88d5d65fd0d90202b3aff77e561f4aac416c49f73e1b43` | - | `norrath_game_object_e_rand` (56) |
| `sources/w10-world-c-game-object-find-tag-float/find_tag_float.cpp` | `w10wc_find_tag_float.o` | `bc87d16cd4673bc2f4e6f304803c1f55191e8f6761e5b5b4f1987514cac7cf17` | - | `norrath_game_object_find_tag_float` (376) |
| `sources/w10-world-c-object-move-with-world-collision/object_move.cpp` | `w10wc_object_move.o` | `b47b8e3908714c216b62bbfe3f37a6888507b61bbb9ed510418e2c80f5986f35` | - | `objectMoveWithWorldCollision__FP6Point3G6Point3fiT0` (520) |
| `sources/w10-world-c-add-to-medium-run-list/add_to_medium_run_list.cpp` | `w10wc_add_to_medium_run_list.o` | `0c9a17e9d052e34c5dddbb39fd761f04cca5f137522d57bbee11aa3c0157a67e` | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` | `objectAddToMediumRunList__FP10GameObject` (216) |
| `sources/w10-world-c-create-by-name/create_by_name.cpp` | `w10wc_create_by_name.o` | `112ee04007652405407638afedf02086e87ff38802dc8d405b7f55fe3344669f` | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` | `objectCreateByName__FPcG6Point3sPPc` (96) |
| `sources/w10-world-c-remove-monster/remove_monster.cpp` | `w10wc_remove_monster.o` | `a071a40b273cf6c50f8ebce39e33e169ffdaa416e1a966d574f3f0cf6a829932` | - | `removeMonster__FP10GameObject` (68) |
| `sources/w10-world-c-mtc-initialize/mtc_initialize.cpp` | `w10wc_mtc_initialize.o` | `8458b6d473cfe6091b701b4bd12217c5eb27dac687daca8e0dd57aa61c0efa2c` | - | `mtcInitialize__Fv` (316) |
| `sources/w10-world-c-patch-sorted-list/patch_sorted_list.cpp` | `w10wc_patch_sorted_list.o` | `ecc86934e19a304310c93b740f2d52882be2f73fdb588d58dc6f371aff195b03` | - | `_ZN12_GLOBAL__N_1L7convertEPhPKhS0_` (572), `patchSortedListWithLocalTime__Fv` (148) |
| `sources/w10-world-c-keyboard-repeat-words/keyboard_repeat_words.cpp` | `w10wc_keyboard_repeat_words.o` | `8503ceb4ddc3090a10a3d55c71962bdfe6fdf3cc5a60bb69b242431dc7350b90` | - | - |
| `sources/w10-world-c-comm-last-words/comm_last_words.cpp` | `w10wc_comm_last_words.o` | `2b40e0e605cacbacec8d9ed87cda72be768ec5c3d7110ad825175b73b1ca8045` | - | - |
| `sources/w10-world-c-texture-load-state/texture_load_state.cpp` | `w10wc_texture_load_state.o` | `933a318823aacf14919ed520f140df2af281660ad8e1de5e7ebf42ee95e0aa4a` | - | - |
| `sources/w10-world-c-count-words/count_words.cpp` | `w10wc_count_words.o` | `ca373fffa709f9deda19c3e7abd367e47ab3a888e1175fe6e5bd6b35d5e75220` | - | - |
| `sources/w10-world-c-frame-words/frame_words.cpp` | `w10wc_frame_words.o` | `0490a7c36a92d4dd446d6595df203e54a01553328d9c8a419bd0155cf05029d7` | - | - |
| `sources/w10-world-c-progress-word/progress_word.cpp` | `w10wc_progress_word.o` | `13ad97bf56b6009e5e8f096b18656c07d9a3b0033b89ab9d6afc19bec61a6aef` | - | - |
| `sources/w10-world-c-null-point/null_point.cpp` | `w10wc_null_point.o` | `d52f351f690c4db95cc0cc271291fb34d7e3da9f3609598857ac15f3cbdb4da4` | - | - |
| `sources/w10-world-c-pm-list-pointers/pm_list_pointers.cpp` | `w10wc_pm_list_pointers.o` | `2680cb8ca5bdbd5a066b4f688688f10d6a1e643e10612682b01d31e36158c226` | - | - |
| `sources/w10-world-c-auto-map-locations/auto_map_locations.cpp` | `w10wc_auto_map_locations.o` | `f0d5c00840e972fee93684ffe4d8f395a6e756bb623fcf6e5ff2ccc778c7f0da` | - | - |
| `sources/w10-world-c-make-collision-list/make_collision_list.cpp` | `w10wc_make_collision_list.o` | `fbf83a302edaadadc238c5059335428d22ec6624eb2d4dbb0f3afdb88280ba11` | - | `makeCollisionList__FP12_worldHeaderffffPP13collisionParti` (688) |
| `sources/w10-world-c-find-empty-spot-monsterspawn/find_empty_spot_monsterspawn.cpp` | `w10wc_find_empty_spot_monsterspawn.o` | `73bd91c5346b5a0febad1bb9db50b2b33077ce181ecccae8200b773ce4c6d6d4` | - | `objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc` (896) |
| `sources/w10-world-c-glob-dir/glob_dir.cpp` | `w10wc_glob_dir.o` | `77e7a9e901c29016951e184d1453507a529bebe593fd9b0b8f005880a9e461f0` | - | - |
| `sources/w10-world-c-lump-load-glob/lump_load_glob.cpp` | `w10wc_lump_load_glob.o` | `9d9f73f1ea5cd34d73108aeb385f2cf930acb682c9c4def76afb0d8cbdf36e06` | `sources/w9-animation-object-b-lumps/lumps.hpp` | `lumpLoadGlob__FPc` (784) |
| `sources/w10-world-c-mtc-words/mtc_words.cpp` | `w10wc_mtc_words.o` | `612ddb6c5b47c7d99da10bf04070fbbf303dc4e83f36f5ae8ca855acd22a9d00` | - | - |

## Authored but never executed

All eleven functions and all storage units: compiled and exercised only by host contracts with call mocks. None ran on
the EE or in the game. `lumpLoadGlob`'s null-allocation store is mocked on the host; on the target it is a volatile
store to address 0, as `lumpAdd` does. The `mtcInitialize` kind-1/3 tests have not been seen against real records.

## What is unknown

See the per-function lists above, plus: the content of `list.1073` as a published structure; what writes `nmChars` and
`nmAlwaysPages`; whether `progress`, `nullp` and the `nm*` counters are written by code outside this package's reach;
whether the compiled-size differences to the original (for instance `objectAddToMediumRunList`, 216 against 100 bytes)
come from compiler versions or from differing source shapes.
