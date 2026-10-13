# w10-world-a: world, level and game-loop part 1 of 4

Package branch: `agents/w10-world-a`. Ten functions and 26 data objects. Every source has a `host_check.cpp`
next to it (not an EE plan input). Every source also compiles with the reviewed `authored_cpp` profile. No compiled
body is claimed to match the original bytes, and no byte match is claimed. The pushes were made in two batches for the
26 data commits (one push after the last), not after each one.

## How each function was checked (private, outside git)

Each function was compiled with the reviewed flags, linked at a fixed address with every undefined symbol bound to the
address of the original in the private image, and run side by side with the original in a private R5900 interpreter from
identical memory snapshots. Callees were replaced by identical recording stubs. The comparison is: the ordered call log
(arguments, with pointed-to data logged by content), the return value where the function returns one, and every byte
that changed outside the stack. Each check was shown to fail on at least one deliberate mutation of the source.

| Function | Original address, size | Paired scenarios | Original words reached |
| --- | --- | --- | --- |
| `engineGetField__Fv` | 0x00191300, 16 | 5 | not measured |
| `dumpMem__FPvi` | 0x00144730, 68 | 6 (no state changes to compare; loop count is only checked on the host) | not measured |
| `blockFree__FPv` | 0x001447d8, 88 | 7 | not measured |
| `findTagInt__10GameObjectPci` | 0x00140f60, 316 | 15 | not measured |
| `addpmlist__FPc` | 0x002686a8, 240 | 68 | not measured |
| `addpmlistptrscap__Fv` | 0x00268798, 396 | 120 | not measured |
| `findCutDialog__Fv` | 0x00187a58, 220 | 6 | not measured |
| `convertIndiciesToPointers__Fi` | 0x0024ffd8, 760 | 30 | not measured |
| `executeAction__FP6Action` | 0x00187b38, 2484 | 1200 | 600 of 621 (the rest are delay-slot nops and the unreachable null-object throw call) |
| `doJumpEffect__Fi` | 0x00283b00, 2040 | 300 | 509 of 510 |

## Functions (observed behavior; each source header has the details)

- `engineGetField__Fv`: returns the word 8 bytes into `draw` (0x00790ff0).
- `dumpMem__FPvi`: a busy loop over `(pointer rounded down to 16) .. pointer + length` with four no-ops per step; no
  output call is present in the image body. The compiler here lays the loop out differently.
- `blockFree__FPv`: pushes a block on the free list of the size class found by comparing the address with the pool
  start words of the 4-row table `region` (0x00333d58). Authored under its original name; the existing
  `norrath_block_free` unit is a different symbol and is untouched.
- `findTagInt__10GameObjectPci`: lower-cases the key in place, scans the object's `name=value` string list at +44 for the
  n-th entry whose key matches and is followed by `=`, returns `atoi` of the value, else 0.
- `addpmlist__FPc` and `addpmlistptrscap__Fv`: interning of a name in `pmlistchars` and de-duplication of the last
  zero-terminated run of `pmlistptrs`. Authored under their original names as readable code; the existing
  `norrath_megatile_*` units are the byte-view versions and are untouched. The aliasing-sensitive rereads those units
  keep are not reproduced where the paired runs cannot reach them (out-of-bounds pool/counter overlap).
- `findCutDialog__Fv`: builds `<level>.lmp` and `<cut scene name without extension>.dbin`, looks the entry up and sets
  `nmDialogNodes` to its size divided by 1096.
- `convertIndiciesToPointers__Fi`: inverse of `convertPointersToIndicies__Fi`: unpacks the three item lists of
  `player[n]` and turns the 19 index words at +21896..+21968 back into item addresses (index = list * 1000 + slot,
  -1 = none).
- `executeAction__FP6Action`: the cut-scene action interpreter, 20-byte records `{float time; short type; short kind;
  int a, b, c}`; `type` -100 (far clip), -99 (dialog line, music line, text switch) or an actor-table index with 13
  object kinds including the animation kind that selects an animation by index or by a name hash.
- `doJumpEffect__Fi`: the per-frame jump gate effect: letter-boxed HUD, blur/yaw/zoom driven by `jumpGateTime`, lightning
  and creature shock effects, and particle bursts, using the shared generator `eRandState`.
- Data (size, image value): `_3map$iconList` (1344, zero; 28-byte records), `_3map$iconListCount`, `_3map$revealed`,
  `action` and `actor` (table base addresses, 4), `alwaysLoadedPage` (64), `asyncRandState`, `blocksFree`,
  `bossMeterName` (128), `buffr` (1280), `buffrfull`, `buffrmode`, `buffrout`, `cacheLoadDelayTime`, `cacheLoadName`
  (64), `cacheLoadValid`, `clearing` (1 byte), `collisionFrame`, `curDest` (1 byte), `currentDir` (4096),
  `currentTime`, `cursorTime`, `decodeQ` (512, 64 entries of 8 bytes), `delta`, `derivedFrameNm` (image value -10), `file`.
  Section placement (data versus BSS) is not reproduced by the reviewed compile profile; the sizes and zero image
  values are.

## Imports closed

_3map$iconList, _3map$iconListCount, _3map$revealed, action, actor, addpmlist__FPc, addpmlistptrscap__Fv, alwaysLoadedPage, asyncRandState, blockFree__FPv, blocksFree, bossMeterName, buffr, buffrfull, buffrmode, buffrout, cacheLoadDelayTime, cacheLoadName, cacheLoadValid, clearing, collisionFrame, convertIndiciesToPointers__Fi, curDest, currentDir, currentTime, cursorTime, decodeQ, delta, derivedFrameNm, doJumpEffect__Fi, dumpMem__FPvi, engineGetField__Fv, executeAction__FP6Action, file, findCutDialog__Fv, findTagInt__10GameObjectPci (36 names).

Checked without the full build (the machine was busy): the 36 objects were compiled with the reviewed flags and merged with the
partial object of the fresh baseline build (`ld -r`); the 36 names left the undefined list (1332 -> 1306 undefined symbols
after ten new ones, below). A full partial build with these entries was started and stopped unfinished.

## Imports opened (left unresolved, owned elsewhere or not in this package)

- GetToggle__6Camera, SetNoTargetUpdates__6Camerai (doJumpEffect__Fi), P_StartLightning__FG6Point3T0iiffUi (doJumpEffect__Fi),
  animRemove__FP14AnimationStateP15AnimationHeaderf (executeAction__FP6Action), soundGetDialogSize__Fi (executeAction__FP6Action).
- Data: jumpGatePos (12 bytes at 0x008d5fe0), jumpHidden (byte 0x00486da8), jumpParticleJump (0x008d6040), pmlistchars (the 0x1000-byte
  pool at 0x00435f68), pmlistcharsused (0x00485e20).
- Already unresolved before, still unresolved and used by these sources: pmlistptrs, pmlistptrsused (not in this package).
- Symbols that are defined elsewhere and used here: strlen, strcpy, strncmp, strrchr, strstr, sprintf, atoi, _ctype_, draw,
  engineCurrentLevelName, cutSceneName, nmDialogNodes, dialogActor, dlgDir, dialogPacket, anim, and the camera, sound,
  animation, list, lump, object and model functions called by executeAction__FP6Action and doJumpEffect__Fi.
- The image has two symbols named `anim` (0x00790f60 and 0x007d0dc8). executeAction__FP6Action uses the first (it indexes
  0x00790f60 + 4 + 8 * row). Whether the authored `anim` definition is that object was not checked.

## Build-plan entries needed (path under `experimental/`, sha-256)

All 36 sources are single files without local includes. `python3 docs/agents/w10-world-a-plan-entries.py --toolchain <ee>`
prints the full JSON entries (it also writes a private plan with `--private-root`).

- `sources/w10-world-a-add-pm-list/add_pm_list.cpp` `ee96a947587731a5644f7e4d84e1edfe3578a0799e149f85d45e1196d36b00b8`
- `sources/w10-world-a-add-pm-list-ptrs-cap/add_pm_list_ptrs_cap.cpp` `d2d5c7f1d0d16ac7135e5fcf55c39b6b27f83e7616a66f87cb7b5438de691dfe`
- `sources/w10-world-a-block-free/block_free.cpp` `fe209f9a991db4c94e5ac459d2855ed4ed6c586e8b28627e0aaa24d2f025f79a`
- `sources/w10-world-a-convert-indicies-to-pointers/convert_indicies_to_pointers.cpp` `2d27866c9a0808979ed1a6cb6ec27914be2240cb39b45d58a8ca0e24474126eb`
- `sources/w10-world-a-data-3map-icon-list/3map_icon_list.cpp` `1386fa9e6bc6681e1f2aac4ce63c0694f75814cd0b806e53157923ccc4f03ef6`
- `sources/w10-world-a-data-3map-icon-list-count/3map_icon_list_count.cpp` `5e25019d4d8d06799bee1eda1f4c7740adf50e87e97f7904418855c15a733035`
- `sources/w10-world-a-data-3map-revealed/3map_revealed.cpp` `f32463684f0b1d84be86ccd7b979f436b975e359d76b259a0834f92d5fcfd6e6`
- `sources/w10-world-a-data-action/action.cpp` `76a37ebf5042ac61777729cf8e35ba9cfbf673da924ff8404e43575cfda184f0`
- `sources/w10-world-a-data-actor/actor.cpp` `fc0f1d6e2ee8b50f0be44556abf5876f54aae30694f60ed2833ec396a8412c79`
- `sources/w10-world-a-data-always-loaded-page/always_loaded_page.cpp` `72adb3c0100def5331ee0e0265c4010d09d987ff835753890a2dfb35625c3bce`
- `sources/w10-world-a-data-async-rand-state/async_rand_state.cpp` `2f7be51959ddc24ba211d15ac62b33f577cb5f993a3005feba1405ca795b2a17`
- `sources/w10-world-a-data-blocks-free/blocks_free.cpp` `29e3dd7394fd20861cda4ed01cd66af160df57b728cdda153d755452508e3540`
- `sources/w10-world-a-data-boss-meter-name/boss_meter_name.cpp` `8406b7a95ebe55f460762e62307d0cbdf9a2c80e5717b008a87b76191b4c3ee8`
- `sources/w10-world-a-data-buffr/buffr.cpp` `eedab93511af6dddbf36ffb244f349e5df115948dd0c0c2f18132190a90c8205`
- `sources/w10-world-a-data-buffr-full/buffr_full.cpp` `5d8eabf0e7ac7b5ac7f81f67f90fe31a693588019b88eaaa9e1e8f29bf269e32`
- `sources/w10-world-a-data-buffr-mode/buffr_mode.cpp` `09fc6a455d08fc04e21b789786a6ef6ed9e2d456dfb2cb47b113d6c2f947d3ee`
- `sources/w10-world-a-data-buffr-out/buffr_out.cpp` `29dee465e6eefed0b1082bcf203fc60c9255bf02d61f850a0b2cfd77e256f620`
- `sources/w10-world-a-data-cache-load-delay-time/cache_load_delay_time.cpp` `b168adf7f0c9bc16274f9a0f158c677f000db4542f5fd8bcc1550864cfce3367`
- `sources/w10-world-a-data-cache-load-name/cache_load_name.cpp` `1409ad535c588f907e8ee90a8206f93dac8ba67466eb19d268b0fac7413e9eea`
- `sources/w10-world-a-data-cache-load-valid/cache_load_valid.cpp` `cd7bcbb3a7927e892e8c03fb72d82117da8673601ddeaf8bc07d19d39fa01e32`
- `sources/w10-world-a-data-clearing/clearing.cpp` `0b21103f1efecc533a00361c7aa56f35a41dfdcb78e114450bac43fb802034f5`
- `sources/w10-world-a-data-collision-frame/collision_frame.cpp` `0768b6ce5bcacfe7b3b0f9cb77abb9e1c3e382bdbd8c74231806deab2109d220`
- `sources/w10-world-a-data-cur-dest/cur_dest.cpp` `ba22a59b3cce4e9c4dfcda0a5655e741881e6b4d263db866126171ad63732739`
- `sources/w10-world-a-data-current-dir/current_dir.cpp` `5dcc469988a6ba4e69b6c66a05e866e59c434b6448fd99cadad098c1d35d36ca`
- `sources/w10-world-a-data-current-time/current_time.cpp` `a1801ee6fcbb8b9dafceab7375d5798702beee08d0129ad8f2cd68cfd0180eb4`
- `sources/w10-world-a-data-cursor-time/cursor_time.cpp` `644d5b309f5390e485d6a63319df8781bcb252277a035bdf5dd943e791f6a08f`
- `sources/w10-world-a-data-decode-q/decode_q.cpp` `f1467e00c5f876a48986c383914796369d4c7306254351e3ed5fba0c191ff0bb`
- `sources/w10-world-a-data-delta/delta.cpp` `29678f491d82c22be1fc44c873ef14e9f2b396970bc65fe25822bb3f0259712e`
- `sources/w10-world-a-data-derived-frame-nm/derived_frame_nm.cpp` `8f93258e00c91cb3fdfc44e43ba8d69367306fe3adce0254366f7154b5ab9a12`
- `sources/w10-world-a-data-file/file.cpp` `94fb5bd995475d654bb9b82c1c0293af8f9f853c32acff2535c4f36d0b0e1580`
- `sources/w10-world-a-do-jump-effect/do_jump_effect.cpp` `078c3eb35c11bd3d7fff854a6d339d5cbb9fd133a3cd7720b655597307c53a1f`
- `sources/w10-world-a-dump-mem/dump_mem.cpp` `c9f45ac756f5341281e86fabe85d631fce9546c6d3c7e00a3dbfcd72561a8e10`
- `sources/w10-world-a-engine-get-field/engine_get_field.cpp` `de590fd414bc4f662f7d59382139339e4952e6c0ec0568a8de92ca3fea94d533`
- `sources/w10-world-a-execute-action/execute_action.cpp` `7c374302c76ed8b0e2667b1c2ee5a71ed861a9b0b628b0ee30d4cb4b2f03a323`
- `sources/w10-world-a-find-cut-dialog/find_cut_dialog.cpp` `a449a42ae45dbe63679d5abaac4a948bded2a55d32f1c8ce3066fab87534f011`
- `sources/w10-world-a-find-tag-int/find_tag_int.cpp` `a4afcf0ac9f76d6d7ebbc79bda2976e83138e1df385b7291d4c47e8210a9421f`

## Authored but never executed

Everything above ran only in host checks with mocked callees and, for the ten functions, in the private paired runs
against the original image described above. None of it has run in a game run or on hardware. The paired runs replace
every callee with a recording stub, so the real callees (camera, sound, animation, lump, particle and model routines)
are not exercised. The data objects have only a host check of size and initial value.

Specific gaps:
- `dumpMem__FPvi`: the loop is not instruction-identical (the image uses branch-likely forms); only termination and
  absence of memory changes are compared in the paired run.
- `doJumpEffect__Fi`: the compiler adds a divide-by-zero trap (`teq`) on the modulus by the shadow-point count; the image
  has none. Behavior differs only when `modelGetNmShadowPoints` returns 0 (the image's result there is undefined).
- `executeAction__FP6Action`, kind 2 with c == 0x0badfeed: the dynamic-type test is written as the observed calls (the
  type word function through the object's table at +4, `__tf3NPC`, `__eq__C9type_infoRC9type_info`); the null-object
  `__throw_bad_typeid` call is unreachable after the earlier null check and was never run.
- `executeAction__FP6Action` max.s (clamps of the dialog actor's animation components) is written as a compare; NaN inputs
  were not exercised.
- `findTagInt__10GameObjectPci`, `addpmlist__FPc`: library string routines were Python stubs in the paired runs.

## Unknown

- Field names and types of everything behind the object offsets used (+12 flags, +36 tag list, +72 table, +160 64-bit flags,
  +288 animation state, +570 flag byte, +824 effect state, and the others in the source headers).
- The meaning of the Action record's `a`, `b`, `c` words per kind beyond what the code does with them, of the constant
  0x0badfeed, and of the frame figure 0x10bda.
- The sizes of `currentDir` (4096 is the gap to the next symbol; 1024 is the length the card code is seen to use) and of
  `cacheLoadName` (64 is the gap).
- Who writes `clearing` (the image text never stores to it), `_3map$revealed`, `buffrmode` and the other state words.
- Section placement of the data (initialized data versus BSS) in a final link.
- Why `dumpMem__FPvi` has no output call; whether `playerPrintPaperDoll__Fi` does anything (the sibling notes say it is a bare
  return).
