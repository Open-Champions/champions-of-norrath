# Level and animation load

Package notes for `agents/level-anim-load`. Sources are under `experimental/sources/level-anim-load-*`. Every unit was compiled with the reviewed EE `authored_cpp` flags and linked into a private copy of the partial build; the shared `experimental/build-plan.json` and `experimental/snapshot.json` were not edited. Each function has a `host_contract.cpp` beside it and each storage unit a `storage_contract.cpp`; these are host tests, not plan inputs. Nothing here ran on the EE or in the game, and none of the function bodies is an original-byte match (the compiled bodies are larger than the originals).

Original addresses below are from the pinned private executable `SLUS_205.65` (read locally; no listing is published). Where this file says "observed" it describes instructions or bytes at the cited address; "inferred" and "unknown" are stated separately in each source header.

## Functions

| Authored symbol | Original | Address | Original size | Compiled size |
|---|---|---|---|---|
| `animInitState__FP14AnimationState` | same | `0x001637a8` | 8 | 8 |
| `animAdd__FP14AnimationStateP15AnimationHeaderi` | same | `0x00163830` | 132 | 312 |
| `animResetFramestates__FP14AnimationState` | same | `0x001640d0` | 96 | 140 |
| `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi` | same | `0x00160128` | 208 | 552 |
| `animSetToEnd__FP14AnimationState` | same | `0x00163ba0` | 184 | 440 |
| `cutMsgHandlerFunction__FPUci` | same | `0x0018e470` | 16 | 20 |
| `cutStartScene__FP9_cutscenePc` | same | `0x0018e1c0` | 320 | 384 |
| `lumpFind__FPvPc` | same | `0x00166580` | 204 | 260 |
| `norrath_route_unroll_vnode` | `routeUnrollVnode__Fi` (weak) | `0x0031a2d8` | 236 | 444 |
| `norrath_swap_int` | `swap__H1Zi_RX01T0_v` (weak) | `0x003131a0` | 20 | 20 |
| `norrath_archive_sound_free_ram` | `soundGetFreeRam__Fv` | `0x0024f588` | 48 | 64 |
| `norrath_monster_spawn` | `monsterSpawn__FPPcG6Point3sb` | `0x0026aa80` | 2112 | 2812 |

The animation state is addressed by byte offsets, with no class declaration claimed. Observed: signed byte 4 is a count, signed byte 5 a capacity, halfword 6 a flag union, word 0 a pointer to a record whose word at +1552 points back, and components of 32 bytes start at +8 (header pointer at +0, link pointer at +4, floats at +8, +12, +16, +20 and +24, halfwords at +28 and +30). `animInit` links `animResults` in 1568-byte records with the list words at +1556 and +1560, which is why word 0 is read as a result-record pointer; that is inferred.

`animAddTransitionTo` forces the rate to 0.0 when the slot index is zero, and uses the step 1/(rate*60) only for a rate at or above the 435.0 literal at `0x0047fe50`; below it the step is 1.0. The NPC and prop natives pass rates near 0.3, so the division has not been seen taken and its purpose is unknown. `animSetToEnd` calls `updateAnimation__FP13animComponent` (`0x00161528`, a local function of the animation module) and then adjusts the linked record with the exact float order of the image.

`lumpFind` is the same binary search the older `norrath_lump_find` performs; it is added under the original name because the loader imports that name. The older function is untouched.

`norrath_monster_spawn` is the long one. Observed behaviour: option strings are matched by prefix (`type=`, `line=`, `chance=`, `radius=`, `countmin=`, `countmax=`, `multitype=`, `clearradius=`); `eRandState` advances by `x * 0x19660d + 0x3c6ef35f` and a draw is `(x >> 16) / 65536`; a missing `chance=` (0.0) never spawns; the count is `(int)(min + draw * (max - min + 1))`; the type text is `<n>,` followed by entries with `-` flags (`A`/`a` always, `O`/`o` once) or parenthesised groups, and `spawn__FG6Point3iisPcbPPci` receives the first option string containing the entry's text taken from its start to the end of the type text. That last detail is what the instructions do; its intent is not claimed. The names "always" and "once" are inferred from how the flag tables are used. The host test checks the draw sequence against an independent description of the generator and checks the always, once, group, nested-paren and multitype paths with entry pools; every deliberate edit tried (about two dozen, covering the generator constants, the gate, the count formula, the option prefixes, the flag letters, the group and depth parsing, the pool removal and the selection) was caught by the test except one that cannot change behaviour. Where the image would read past a 16-entry table or an unterminated type text, this version stops at the terminator and treats out-of-range table reads as nonzero; those cases are not tested.

## Data

| Authored symbol | Original | Address | Bytes | Initial |
|---|---|---|---|---|
| `norrath_factory_world_pointer_word` | `world` | `0x004844a0` | 4 | 0 |
| `norrath_factory_world_flags_word` | `levelFlags` | `0x004844c0` | 4 | 0 |
| `norrath_find_height_orientation_scale_bytes` | literal | `0x00480024` | 4 | `00 01 00 38` |
| `norrath_forbidden_orientation_scale_bytes` | literal | `0x00480028` | 4 | `00 01 00 38` |
| `cd` | `cd` (net module) | `0x004f3e90` | 64 | 0 |
| `scene` | `scene` | `0x004f4060` | 56 | 0 |
| `frameStates` | `frameStates` | `0x006162c0` | 580224 | 0 |
| `animResults` | `animResults` | `0x006a3d40` | 156800 | 0 |
| `cutMsgHandler` | same | `0x00486a28` | 4 | 0 |
| `netNmReady`, `netWeSentReady`, `cutCurrentScene`, `cutInit`, `cutPlayMode`, `displayActionPos`, `displayCutPos`, `displayFrame`, `cutTime`, `cutSkipMode`, `dialogActor`, `musicSize` | same | `0x004843a4`, `0x004843a8`, `0x0048432c`, `0x0048431a`, `0x00484319`, `0x00484344`, `0x00484348`, `0x00484328`, `0x00486a20`, `0x00484334`, `0x00486a14`, `0x00484330` | 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 4, 4 | 0 |
| `cutSceneName` | same | `0x00790f10` | 80 | 0 |

The two scale literals are adjacent `.lit4` words (the single float `0x38000100`) that `worldFindHeight` (`0x00176378`) and `worldTestForbidden` (`0x001766c8`) load through gp. `frameStates` is `150 * 3868` bytes plus a 24-byte tail that `animInit` does not write; `animResults` is exactly 100 records of 1568. `cd` is the network client block `netFree` hands to `sceInetClose`, `sceInetCtlDownInterface` and `sceLibnetTerminate`; of the four same-named symbols in the image it is the one at `0x004f3e90`, selected by the `lui`/`addiu` pair in `netFree`. `cutTime` and `dialogActor` have ELF size 0, so only the stored word is claimed. `cutPlayMode` is the byte between `cutMode` and `cutInit`.

`norrath_loader_failure_word_view` is not an object. The loader stores to absolute address 0 (`sw zero,0(zero)` at `0x00165d04`, 2 from `cdLoad`, 0 from `memAlloc`). It is satisfied by `level-anim-load-loader-failure-word/failure_word.cpp`, which defines a global absolute symbol of value 0 through a basic `asm` block, so stores resolve to address 0 exactly as in the image. Checked with the EE toolchain: `nm` reports `00000000 A norrath_loader_failure_word_view` in the partial object and the dependent relocations resolve to it. This is the one unit that goes beyond a section object, and the integrator should decide whether an absolute symbol at zero is acceptable under the no-absolute-addresses rule; if not, drop that file and the import stays open.

## Same-address definitions the integrator should know about

`norrath_factory_world_pointer_word` is the word `world` at `0x004844a0`, which `world-state-route-world` already defines as `norrath_route_world_word_bytes`. `norrath_factory_world_flags_word` is `levelFlags` at `0x004844c0`, which natives-c defines as `levelFlags`. The brief required the factory-facing names, so each is a separate 4-byte object here; as two link symbols they are two different words, not one. Pick one owner before any run that needs them to agree. Likewise `cutMode` stays an unresolved import under its original name; world-state defines the same byte as `norrath_game_cut_mode_byte_view`. This package defines `cutPlayMode` but not `cutMode`.

## Checks performed

- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.
- Each function's host contract was compiled with the host C++ compiler and run; the field accessors take a hook pair (`NORRATH_HOST_ARENA`) that maps 32-bit target words to arena pointers, since the host cannot hold a target pointer in a record word.
- Mutation checks: each function was edited in several places (a constant, a comparison, a store offset, a branch) and the contract failed in each case, except edits that cannot change observable behaviour (for example `count <= 0` against `count < 0` in `monsterSpawn`, whose loop does nothing at zero).
- Private build: `python3 tools/build_partial.py` with a private copy of the plan and snapshot containing the 35 entries below. All 35 units compiled; the link completed with 943 inputs; the unresolved count went from 950 to 937. All 22 package names left the unresolved list; 9 names were opened.
- Nothing was executed on the EE and no original code was run.

## Imports closed

`animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animInitState__FP14AnimationState`, `animResetFramestates__FP14AnimationState`, `animResults`, `animSetToEnd__FP14AnimationState`, `cd`, `cutMsgHandler`, `cutMsgHandlerFunction__FPUci`, `cutStartScene__FP9_cutscenePc`, `frameStates`, `lumpFind__FPvPc`, `norrath_archive_sound_free_ram`, `norrath_factory_world_flags_word`, `norrath_factory_world_pointer_word`, `norrath_find_height_orientation_scale_bytes`, `norrath_forbidden_orientation_scale_bytes`, `norrath_loader_failure_word_view`, `norrath_monster_spawn`, `norrath_route_unroll_vnode`, `norrath_swap_int`, `scene`.

## Imports opened

`SetMode__6Camerai`, `coreStartLoading__Fv`, `cutDrawFrame__Fv`, `cutRunFrameTask__Fv` (all from `cutStartScene`); `spawn__FG6Point3iisPcbPPci` (from `monsterSpawn`; `0x0026a738`); `updateAnimation__FP13animComponent` (from `animSetToEnd`; a local function, `0x00161528`); `effectsBufferSize`, `nmSounds`, `soundList` (from the sound free-RAM query; audio owns them). `cutStartScene` also references `cutMode`, `gameControlPaused`, `hudNoDraw`, `frontEndRunning`, `gameNetKick__Fi`, `worldSetFarClip__Ff`, `worldSetFrustum__Fffff` and `strcpy`, all already unresolved imports of other packages. The 13 cutscene state words it needs were defined here rather than left open.

## Build-plan entries needed

Append these 35 plan inputs after the current last input, renumbering `input_order` at merge, and add the matching snapshot inputs and file hashes. The object names are private to this package and unique. The host contracts are not inputs. Header closure is `sources/level-anim-load-common/mem.hpp` for the function units that include it.

Plan inputs (JSON, one per line):

```json
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 910, "kind": "independently_authored_source", "object_name": "lal_anim_add_anim_add.o", "profile": "authored_cpp", "sha256": "654541b71ca4137fd386ab76dadd7db55343ddce8dedb0225092f05bb97b3f05", "source": "sources/level-anim-load-anim-add/anim_add.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 911, "kind": "independently_authored_source", "object_name": "lal_anim_add_transition_to_anim_add_transition_to.o", "profile": "authored_cpp", "sha256": "5dc5b43adfe5e49d8b9b9f6ad5f868210c8d937433b63511b344c44db24f4efd", "source": "sources/level-anim-load-anim-add-transition-to/anim_add_transition_to.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 912, "kind": "independently_authored_source", "object_name": "lal_anim_init_state_anim_init_state.o", "profile": "authored_cpp", "sha256": "83f29cbdd836db25fb046d29cc16161f51c45629acb78cd23d9ac7237afa523e", "source": "sources/level-anim-load-anim-init-state/anim_init_state.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 913, "kind": "independently_authored_source", "object_name": "lal_anim_reset_framestates_anim_reset_framestates.o", "profile": "authored_cpp", "sha256": "992d4622a56944824d3a84c3161384c7634edfb4d7ac067d8ecbccaed49d480b", "source": "sources/level-anim-load-anim-reset-framestates/anim_reset_framestates.cpp"}
{"header_closure": ["sources/level-anim-load-anim-results/storage.hpp"], "input_order": 914, "kind": "independently_authored_source", "object_name": "lal_anim_results_storage.o", "profile": "authored_cpp", "sha256": "5c83389567f4e2cccbe0be8512d7304aa74e6413efc0ad158f5af13ca408878a", "source": "sources/level-anim-load-anim-results/storage.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 915, "kind": "independently_authored_source", "object_name": "lal_anim_set_to_end_anim_set_to_end.o", "profile": "authored_cpp", "sha256": "44deed07186ee948d4d1454e8269325ef70c1b73b5a68f8cb4f78d47bb31fec8", "source": "sources/level-anim-load-anim-set-to-end/anim_set_to_end.cpp"}
{"header_closure": ["sources/level-anim-load-cd-state/storage.hpp"], "input_order": 916, "kind": "independently_authored_source", "object_name": "lal_cd_state_storage.o", "profile": "authored_cpp", "sha256": "240d053ce394835f28d1b5ef86d5c20dd5fddfd80bcaa1a88e99e74e2c7e552e", "source": "sources/level-anim-load-cd-state/storage.cpp"}
{"header_closure": ["sources/level-anim-load-cut-current-scene/storage.hpp"], "input_order": 917, "kind": "independently_authored_source", "object_name": "lal_cut_current_scene_storage.o", "profile": "authored_cpp", "sha256": "798f5c6e580a045e2b54734d49d930223a40eb17daead378d6f3bbffc34bcefb", "source": "sources/level-anim-load-cut-current-scene/storage.cpp"}
{"header_closure": ["sources/level-anim-load-cut-init/storage.hpp"], "input_order": 918, "kind": "independently_authored_source", "object_name": "lal_cut_init_storage.o", "profile": "authored_cpp", "sha256": "3ced145469e4411fe6cf62e69aa7ea8ec59f44b1d6f9ce1aa6660b79f454f5ef", "source": "sources/level-anim-load-cut-init/storage.cpp"}
{"header_closure": ["sources/level-anim-load-cut-msg-handler/storage.hpp"], "input_order": 919, "kind": "independently_authored_source", "object_name": "lal_cut_msg_handler_storage.o", "profile": "authored_cpp", "sha256": "a213e85b14233592a58bb73a59e4f7b1f8e3956edc041fc4a774a98969a603ce", "source": "sources/level-anim-load-cut-msg-handler/storage.cpp"}
{"header_closure": [], "input_order": 920, "kind": "independently_authored_source", "object_name": "lal_cut_msg_handler_function_cut_msg_handler_function.o", "profile": "authored_cpp", "sha256": "5a133c7f44752db70eec15352e215370c60674bdc6832c16d3fc13f5fc65cbe9", "source": "sources/level-anim-load-cut-msg-handler-function/cut_msg_handler_function.cpp"}
{"header_closure": ["sources/level-anim-load-cut-play-mode/storage.hpp"], "input_order": 921, "kind": "independently_authored_source", "object_name": "lal_cut_play_mode_storage.o", "profile": "authored_cpp", "sha256": "c89f09d3be338f7bc09ee1ec61a0d9544605981c03458a70bfd1a135c677cc8b", "source": "sources/level-anim-load-cut-play-mode/storage.cpp"}
{"header_closure": ["sources/level-anim-load-cut-scene-name/storage.hpp"], "input_order": 922, "kind": "independently_authored_source", "object_name": "lal_cut_scene_name_storage.o", "profile": "authored_cpp", "sha256": "308c4b3358fb0e9e21b1d90c83c96584a96ec1ad1e3b22f7609f543ef09d00f0", "source": "sources/level-anim-load-cut-scene-name/storage.cpp"}
{"header_closure": ["sources/level-anim-load-cut-skip-mode/storage.hpp"], "input_order": 923, "kind": "independently_authored_source", "object_name": "lal_cut_skip_mode_storage.o", "profile": "authored_cpp", "sha256": "1ab972e9887a1afa7d8b2093455be769357abd6981bf7a69751ab67473836b7b", "source": "sources/level-anim-load-cut-skip-mode/storage.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 924, "kind": "independently_authored_source", "object_name": "lal_cut_start_scene_cut_start_scene.o", "profile": "authored_cpp", "sha256": "3f9f277aa9832699b304aa5d11e4aa3e35060b08acef880e5ab8a5b1f510975e", "source": "sources/level-anim-load-cut-start-scene/cut_start_scene.cpp"}
{"header_closure": ["sources/level-anim-load-cut-time/storage.hpp"], "input_order": 925, "kind": "independently_authored_source", "object_name": "lal_cut_time_storage.o", "profile": "authored_cpp", "sha256": "868f7c1e263d877da6d0249a0b474255384310160af188c10d902733837d6a02", "source": "sources/level-anim-load-cut-time/storage.cpp"}
{"header_closure": ["sources/level-anim-load-dialog-actor/storage.hpp"], "input_order": 926, "kind": "independently_authored_source", "object_name": "lal_dialog_actor_storage.o", "profile": "authored_cpp", "sha256": "0e670425ded373d122992eea5983715656c28d53007286a6a3408ec9c8880d4f", "source": "sources/level-anim-load-dialog-actor/storage.cpp"}
{"header_closure": ["sources/level-anim-load-display-action-pos/storage.hpp"], "input_order": 927, "kind": "independently_authored_source", "object_name": "lal_display_action_pos_storage.o", "profile": "authored_cpp", "sha256": "71d66951ee6f8f430b45a427d22d91df1019f3e276967853cdb66a6ef7d7c556", "source": "sources/level-anim-load-display-action-pos/storage.cpp"}
{"header_closure": ["sources/level-anim-load-display-cut-pos/storage.hpp"], "input_order": 928, "kind": "independently_authored_source", "object_name": "lal_display_cut_pos_storage.o", "profile": "authored_cpp", "sha256": "b4c001b57fa73fade9eb84186643db35d17db8a2332a7c93f3d55aae8dfa8757", "source": "sources/level-anim-load-display-cut-pos/storage.cpp"}
{"header_closure": ["sources/level-anim-load-display-frame/storage.hpp"], "input_order": 929, "kind": "independently_authored_source", "object_name": "lal_display_frame_storage.o", "profile": "authored_cpp", "sha256": "8150f5cf26f99db57d80f789ba8e34b0ae9f5accef0bc1b62ab1060d4af62f72", "source": "sources/level-anim-load-display-frame/storage.cpp"}
{"header_closure": ["sources/level-anim-load-find-height-scale/storage.hpp"], "input_order": 930, "kind": "independently_authored_source", "object_name": "lal_find_height_scale_storage.o", "profile": "authored_cpp", "sha256": "f7c368856c061553679db40b481351de203e7a69e5f4f366797345fae31eb1f5", "source": "sources/level-anim-load-find-height-scale/storage.cpp"}
{"header_closure": ["sources/level-anim-load-forbidden-scale/storage.hpp"], "input_order": 931, "kind": "independently_authored_source", "object_name": "lal_forbidden_scale_storage.o", "profile": "authored_cpp", "sha256": "fe8ad9d6a5365ef61f417da97aadddd1ef9df862188f8c81d159c48ad5ba4972", "source": "sources/level-anim-load-forbidden-scale/storage.cpp"}
{"header_closure": ["sources/level-anim-load-frame-states/storage.hpp"], "input_order": 932, "kind": "independently_authored_source", "object_name": "lal_frame_states_storage.o", "profile": "authored_cpp", "sha256": "bf0eb334aea12fec3bf5d729a4473db8aa689cb0b665c6ac00833d1f0e800e78", "source": "sources/level-anim-load-frame-states/storage.cpp"}
{"header_closure": [], "input_order": 933, "kind": "independently_authored_source", "object_name": "lal_loader_failure_word_failure_word.o", "profile": "authored_cpp", "sha256": "f8f47f4028b1ca99e6bceb0a9f9b8fca00dff4554b2b1ffde7c9a18896ad4791", "source": "sources/level-anim-load-loader-failure-word/failure_word.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 934, "kind": "independently_authored_source", "object_name": "lal_lump_find_lump_find.o", "profile": "authored_cpp", "sha256": "c42ad46490d371bd75ecf661ce013afb7a600f348be1eb290a8af6bfaeb1dac3", "source": "sources/level-anim-load-lump-find/lump_find.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 935, "kind": "independently_authored_source", "object_name": "lal_monster_spawn_monster_spawn.o", "profile": "authored_cpp", "sha256": "41852c6fca73652d77e6e436e8fd4e694af4c7d81395103f26d3ca6d5d2ff635", "source": "sources/level-anim-load-monster-spawn/monster_spawn.cpp"}
{"header_closure": ["sources/level-anim-load-music-size/storage.hpp"], "input_order": 936, "kind": "independently_authored_source", "object_name": "lal_music_size_storage.o", "profile": "authored_cpp", "sha256": "338a56b5c62a5eaee000e70050708e5dd11c8cd3f65bc71d7c0608bfe2e5b6cf", "source": "sources/level-anim-load-music-size/storage.cpp"}
{"header_closure": ["sources/level-anim-load-net-nm-ready/storage.hpp"], "input_order": 937, "kind": "independently_authored_source", "object_name": "lal_net_nm_ready_storage.o", "profile": "authored_cpp", "sha256": "035a1c16e1a850633b12abb725365691fb0004ca6640c9e3ff06eab5d38f47d6", "source": "sources/level-anim-load-net-nm-ready/storage.cpp"}
{"header_closure": ["sources/level-anim-load-net-we-sent-ready/storage.hpp"], "input_order": 938, "kind": "independently_authored_source", "object_name": "lal_net_we_sent_ready_storage.o", "profile": "authored_cpp", "sha256": "b7bf9a067326876e20bbda29c026042b22d783d2e801e4c1ffb07932886b9159", "source": "sources/level-anim-load-net-we-sent-ready/storage.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 939, "kind": "independently_authored_source", "object_name": "lal_route_unroll_vnode_route_unroll_vnode.o", "profile": "authored_cpp", "sha256": "a38d38a778df2af3aece41012c154e25cc6e568a34ffff9936c1de33f25f0170", "source": "sources/level-anim-load-route-unroll-vnode/route_unroll_vnode.cpp"}
{"header_closure": ["sources/level-anim-load-scene-state/storage.hpp"], "input_order": 940, "kind": "independently_authored_source", "object_name": "lal_scene_state_storage.o", "profile": "authored_cpp", "sha256": "8e60f5381b4f6c157145d06d19ee5006a8f0c50fc1cff412b82810351def5bfb", "source": "sources/level-anim-load-scene-state/storage.cpp"}
{"header_closure": ["sources/level-anim-load-common/mem.hpp"], "input_order": 941, "kind": "independently_authored_source", "object_name": "lal_sound_free_ram_sound_free_ram.o", "profile": "authored_cpp", "sha256": "127210bc2d028fd50c2badee83fbecc32c8ff93e666bbdb50e7d33efef5993d4", "source": "sources/level-anim-load-sound-free-ram/sound_free_ram.cpp"}
{"header_closure": [], "input_order": 942, "kind": "independently_authored_source", "object_name": "lal_swap_int_swap_int.o", "profile": "authored_cpp", "sha256": "8088ab150d71b03a34b39071a50c16fd1c7ddfdb9eb1778f3c7994d967441ff3", "source": "sources/level-anim-load-swap-int/swap_int.cpp"}
{"header_closure": ["sources/level-anim-load-world-flags-word/storage.hpp"], "input_order": 943, "kind": "independently_authored_source", "object_name": "lal_world_flags_word_storage.o", "profile": "authored_cpp", "sha256": "71713aa33efb3534b88beb6a27505de5e382feb9a65edaaded4a66b2b1270e4d", "source": "sources/level-anim-load-world-flags-word/storage.cpp"}
{"header_closure": ["sources/level-anim-load-world-pointer-word/storage.hpp"], "input_order": 944, "kind": "independently_authored_source", "object_name": "lal_world_pointer_word_storage.o", "profile": "authored_cpp", "sha256": "e2db2cdd572be421e16f843c5ad70ce7e5c58b09c1f158a57df1d520208faf5e", "source": "sources/level-anim-load-world-pointer-word/storage.cpp"}
```

Snapshot inputs (JSON, one per line; `emitted_functions` carries the compiled body bytes):

```json
{"component": "level-anim-load-anim-add", "emitted_functions": [{"compiled_body_bytes": 312, "name": "animAdd__FP14AnimationStateP15AnimationHeaderi"}], "input_order": 910, "kind": "independently_authored_source", "object_name": "lal_anim_add_anim_add.o", "source": "sources/level-anim-load-anim-add/anim_add.cpp"}
{"component": "level-anim-load-anim-add-transition-to", "emitted_functions": [{"compiled_body_bytes": 552, "name": "animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi"}], "input_order": 911, "kind": "independently_authored_source", "object_name": "lal_anim_add_transition_to_anim_add_transition_to.o", "source": "sources/level-anim-load-anim-add-transition-to/anim_add_transition_to.cpp"}
{"component": "level-anim-load-anim-init-state", "emitted_functions": [{"compiled_body_bytes": 8, "name": "animInitState__FP14AnimationState"}], "input_order": 912, "kind": "independently_authored_source", "object_name": "lal_anim_init_state_anim_init_state.o", "source": "sources/level-anim-load-anim-init-state/anim_init_state.cpp"}
{"component": "level-anim-load-anim-reset-framestates", "emitted_functions": [{"compiled_body_bytes": 140, "name": "animResetFramestates__FP14AnimationState"}], "input_order": 913, "kind": "independently_authored_source", "object_name": "lal_anim_reset_framestates_anim_reset_framestates.o", "source": "sources/level-anim-load-anim-reset-framestates/anim_reset_framestates.cpp"}
{"component": "level-anim-load-anim-results", "emitted_functions": [], "input_order": 914, "kind": "independently_authored_source", "object_name": "lal_anim_results_storage.o", "source": "sources/level-anim-load-anim-results/storage.cpp"}
{"component": "level-anim-load-anim-set-to-end", "emitted_functions": [{"compiled_body_bytes": 440, "name": "animSetToEnd__FP14AnimationState"}], "input_order": 915, "kind": "independently_authored_source", "object_name": "lal_anim_set_to_end_anim_set_to_end.o", "source": "sources/level-anim-load-anim-set-to-end/anim_set_to_end.cpp"}
{"component": "level-anim-load-cd-state", "emitted_functions": [], "input_order": 916, "kind": "independently_authored_source", "object_name": "lal_cd_state_storage.o", "source": "sources/level-anim-load-cd-state/storage.cpp"}
{"component": "level-anim-load-cut-current-scene", "emitted_functions": [], "input_order": 917, "kind": "independently_authored_source", "object_name": "lal_cut_current_scene_storage.o", "source": "sources/level-anim-load-cut-current-scene/storage.cpp"}
{"component": "level-anim-load-cut-init", "emitted_functions": [], "input_order": 918, "kind": "independently_authored_source", "object_name": "lal_cut_init_storage.o", "source": "sources/level-anim-load-cut-init/storage.cpp"}
{"component": "level-anim-load-cut-msg-handler", "emitted_functions": [], "input_order": 919, "kind": "independently_authored_source", "object_name": "lal_cut_msg_handler_storage.o", "source": "sources/level-anim-load-cut-msg-handler/storage.cpp"}
{"component": "level-anim-load-cut-msg-handler-function", "emitted_functions": [{"compiled_body_bytes": 20, "name": "cutMsgHandlerFunction__FPUci"}], "input_order": 920, "kind": "independently_authored_source", "object_name": "lal_cut_msg_handler_function_cut_msg_handler_function.o", "source": "sources/level-anim-load-cut-msg-handler-function/cut_msg_handler_function.cpp"}
{"component": "level-anim-load-cut-play-mode", "emitted_functions": [], "input_order": 921, "kind": "independently_authored_source", "object_name": "lal_cut_play_mode_storage.o", "source": "sources/level-anim-load-cut-play-mode/storage.cpp"}
{"component": "level-anim-load-cut-scene-name", "emitted_functions": [], "input_order": 922, "kind": "independently_authored_source", "object_name": "lal_cut_scene_name_storage.o", "source": "sources/level-anim-load-cut-scene-name/storage.cpp"}
{"component": "level-anim-load-cut-skip-mode", "emitted_functions": [], "input_order": 923, "kind": "independently_authored_source", "object_name": "lal_cut_skip_mode_storage.o", "source": "sources/level-anim-load-cut-skip-mode/storage.cpp"}
{"component": "level-anim-load-cut-start-scene", "emitted_functions": [{"compiled_body_bytes": 384, "name": "cutStartScene__FP9_cutscenePc"}], "input_order": 924, "kind": "independently_authored_source", "object_name": "lal_cut_start_scene_cut_start_scene.o", "source": "sources/level-anim-load-cut-start-scene/cut_start_scene.cpp"}
{"component": "level-anim-load-cut-time", "emitted_functions": [], "input_order": 925, "kind": "independently_authored_source", "object_name": "lal_cut_time_storage.o", "source": "sources/level-anim-load-cut-time/storage.cpp"}
{"component": "level-anim-load-dialog-actor", "emitted_functions": [], "input_order": 926, "kind": "independently_authored_source", "object_name": "lal_dialog_actor_storage.o", "source": "sources/level-anim-load-dialog-actor/storage.cpp"}
{"component": "level-anim-load-display-action-pos", "emitted_functions": [], "input_order": 927, "kind": "independently_authored_source", "object_name": "lal_display_action_pos_storage.o", "source": "sources/level-anim-load-display-action-pos/storage.cpp"}
{"component": "level-anim-load-display-cut-pos", "emitted_functions": [], "input_order": 928, "kind": "independently_authored_source", "object_name": "lal_display_cut_pos_storage.o", "source": "sources/level-anim-load-display-cut-pos/storage.cpp"}
{"component": "level-anim-load-display-frame", "emitted_functions": [], "input_order": 929, "kind": "independently_authored_source", "object_name": "lal_display_frame_storage.o", "source": "sources/level-anim-load-display-frame/storage.cpp"}
{"component": "level-anim-load-find-height-scale", "emitted_functions": [], "input_order": 930, "kind": "independently_authored_source", "object_name": "lal_find_height_scale_storage.o", "source": "sources/level-anim-load-find-height-scale/storage.cpp"}
{"component": "level-anim-load-forbidden-scale", "emitted_functions": [], "input_order": 931, "kind": "independently_authored_source", "object_name": "lal_forbidden_scale_storage.o", "source": "sources/level-anim-load-forbidden-scale/storage.cpp"}
{"component": "level-anim-load-frame-states", "emitted_functions": [], "input_order": 932, "kind": "independently_authored_source", "object_name": "lal_frame_states_storage.o", "source": "sources/level-anim-load-frame-states/storage.cpp"}
{"component": "level-anim-load-loader-failure-word", "emitted_functions": [], "input_order": 933, "kind": "independently_authored_source", "object_name": "lal_loader_failure_word_failure_word.o", "source": "sources/level-anim-load-loader-failure-word/failure_word.cpp"}
{"component": "level-anim-load-lump-find", "emitted_functions": [{"compiled_body_bytes": 260, "name": "lumpFind__FPvPc"}], "input_order": 934, "kind": "independently_authored_source", "object_name": "lal_lump_find_lump_find.o", "source": "sources/level-anim-load-lump-find/lump_find.cpp"}
{"component": "level-anim-load-monster-spawn", "emitted_functions": [{"compiled_body_bytes": 2812, "name": "norrath_monster_spawn"}], "input_order": 935, "kind": "independently_authored_source", "object_name": "lal_monster_spawn_monster_spawn.o", "source": "sources/level-anim-load-monster-spawn/monster_spawn.cpp"}
{"component": "level-anim-load-music-size", "emitted_functions": [], "input_order": 936, "kind": "independently_authored_source", "object_name": "lal_music_size_storage.o", "source": "sources/level-anim-load-music-size/storage.cpp"}
{"component": "level-anim-load-net-nm-ready", "emitted_functions": [], "input_order": 937, "kind": "independently_authored_source", "object_name": "lal_net_nm_ready_storage.o", "source": "sources/level-anim-load-net-nm-ready/storage.cpp"}
{"component": "level-anim-load-net-we-sent-ready", "emitted_functions": [], "input_order": 938, "kind": "independently_authored_source", "object_name": "lal_net_we_sent_ready_storage.o", "source": "sources/level-anim-load-net-we-sent-ready/storage.cpp"}
{"component": "level-anim-load-route-unroll-vnode", "emitted_functions": [{"compiled_body_bytes": 444, "name": "norrath_route_unroll_vnode"}], "input_order": 939, "kind": "independently_authored_source", "object_name": "lal_route_unroll_vnode_route_unroll_vnode.o", "source": "sources/level-anim-load-route-unroll-vnode/route_unroll_vnode.cpp"}
{"component": "level-anim-load-scene-state", "emitted_functions": [], "input_order": 940, "kind": "independently_authored_source", "object_name": "lal_scene_state_storage.o", "source": "sources/level-anim-load-scene-state/storage.cpp"}
{"component": "level-anim-load-sound-free-ram", "emitted_functions": [{"compiled_body_bytes": 64, "name": "norrath_archive_sound_free_ram"}], "input_order": 941, "kind": "independently_authored_source", "object_name": "lal_sound_free_ram_sound_free_ram.o", "source": "sources/level-anim-load-sound-free-ram/sound_free_ram.cpp"}
{"component": "level-anim-load-swap-int", "emitted_functions": [{"compiled_body_bytes": 20, "name": "norrath_swap_int"}], "input_order": 942, "kind": "independently_authored_source", "object_name": "lal_swap_int_swap_int.o", "source": "sources/level-anim-load-swap-int/swap_int.cpp"}
{"component": "level-anim-load-world-flags-word", "emitted_functions": [], "input_order": 943, "kind": "independently_authored_source", "object_name": "lal_world_flags_word_storage.o", "source": "sources/level-anim-load-world-flags-word/storage.cpp"}
{"component": "level-anim-load-world-pointer-word", "emitted_functions": [], "input_order": 944, "kind": "independently_authored_source", "object_name": "lal_world_pointer_word_storage.o", "source": "sources/level-anim-load-world-pointer-word/storage.cpp"}
```

File hashes to add to both `files` maps:

```json
{
 "sources/level-anim-load-anim-add-transition-to/anim_add_transition_to.cpp": "5dc5b43adfe5e49d8b9b9f6ad5f868210c8d937433b63511b344c44db24f4efd",
 "sources/level-anim-load-anim-add/anim_add.cpp": "654541b71ca4137fd386ab76dadd7db55343ddce8dedb0225092f05bb97b3f05",
 "sources/level-anim-load-anim-init-state/anim_init_state.cpp": "83f29cbdd836db25fb046d29cc16161f51c45629acb78cd23d9ac7237afa523e",
 "sources/level-anim-load-anim-reset-framestates/anim_reset_framestates.cpp": "992d4622a56944824d3a84c3161384c7634edfb4d7ac067d8ecbccaed49d480b",
 "sources/level-anim-load-anim-results/storage.cpp": "5c83389567f4e2cccbe0be8512d7304aa74e6413efc0ad158f5af13ca408878a",
 "sources/level-anim-load-anim-results/storage.hpp": "d5248cd1d114f4807a037c37afb489d1d9caa7d85aabbd00251aec4585a88677",
 "sources/level-anim-load-anim-set-to-end/anim_set_to_end.cpp": "44deed07186ee948d4d1454e8269325ef70c1b73b5a68f8cb4f78d47bb31fec8",
 "sources/level-anim-load-cd-state/storage.cpp": "240d053ce394835f28d1b5ef86d5c20dd5fddfd80bcaa1a88e99e74e2c7e552e",
 "sources/level-anim-load-cd-state/storage.hpp": "2d2b4ba2d61f5212a6b350827bf99615b1d5babbbe19b9cc623153912e075f6a",
 "sources/level-anim-load-common/mem.hpp": "387f4fd04cd4045a881f8e6b4072af795a917855c23ce1dc37ec5d3b5f2a3e31",
 "sources/level-anim-load-cut-current-scene/storage.cpp": "798f5c6e580a045e2b54734d49d930223a40eb17daead378d6f3bbffc34bcefb",
 "sources/level-anim-load-cut-current-scene/storage.hpp": "eb70e07c2f69ddd6c5bf0b49463aeab59a9f198452393ca25be7dd477d2b54c8",
 "sources/level-anim-load-cut-init/storage.cpp": "3ced145469e4411fe6cf62e69aa7ea8ec59f44b1d6f9ce1aa6660b79f454f5ef",
 "sources/level-anim-load-cut-init/storage.hpp": "fb5c66206524fe28de04bd7eba334ffb278ad73e26eb5ddffb2776b647bab466",
 "sources/level-anim-load-cut-msg-handler-function/cut_msg_handler_function.cpp": "5a133c7f44752db70eec15352e215370c60674bdc6832c16d3fc13f5fc65cbe9",
 "sources/level-anim-load-cut-msg-handler/storage.cpp": "a213e85b14233592a58bb73a59e4f7b1f8e3956edc041fc4a774a98969a603ce",
 "sources/level-anim-load-cut-msg-handler/storage.hpp": "6fae00157c28b20b19d7433c61c471a4293678ea918bfbcb05c549ab80d496fe",
 "sources/level-anim-load-cut-play-mode/storage.cpp": "c89f09d3be338f7bc09ee1ec61a0d9544605981c03458a70bfd1a135c677cc8b",
 "sources/level-anim-load-cut-play-mode/storage.hpp": "4cd9c3fea5a0a2a6aa10fc79e1abbff48871b9b1a3d3297c4d4ddd086ef72a24",
 "sources/level-anim-load-cut-scene-name/storage.cpp": "308c4b3358fb0e9e21b1d90c83c96584a96ec1ad1e3b22f7609f543ef09d00f0",
 "sources/level-anim-load-cut-scene-name/storage.hpp": "8953fb96afab10e5acbdbd446ee95a5577cd101e84408ede9fe5e50d8f4e3176",
 "sources/level-anim-load-cut-skip-mode/storage.cpp": "1ab972e9887a1afa7d8b2093455be769357abd6981bf7a69751ab67473836b7b",
 "sources/level-anim-load-cut-skip-mode/storage.hpp": "fa1dfa60280bd7654fc31f84acd98f331c5c6d61bfb243aaec61ac8366ea4aa6",
 "sources/level-anim-load-cut-start-scene/cut_start_scene.cpp": "3f9f277aa9832699b304aa5d11e4aa3e35060b08acef880e5ab8a5b1f510975e",
 "sources/level-anim-load-cut-time/storage.cpp": "868f7c1e263d877da6d0249a0b474255384310160af188c10d902733837d6a02",
 "sources/level-anim-load-cut-time/storage.hpp": "ceda0c3cea11a5ce02efd5aa28b8a9453e932a09e97e1249f1649f72c9c20e13",
 "sources/level-anim-load-dialog-actor/storage.cpp": "0e670425ded373d122992eea5983715656c28d53007286a6a3408ec9c8880d4f",
 "sources/level-anim-load-dialog-actor/storage.hpp": "5826553953ecb9be86c8fab99631d5f3e64ef5709b93251a160858d86d640478",
 "sources/level-anim-load-display-action-pos/storage.cpp": "71d66951ee6f8f430b45a427d22d91df1019f3e276967853cdb66a6ef7d7c556",
 "sources/level-anim-load-display-action-pos/storage.hpp": "c843fd31dba4e3187cd52297b5e7ad953ab1356311bd16dc6a26eab16f68ab72",
 "sources/level-anim-load-display-cut-pos/storage.cpp": "b4c001b57fa73fade9eb84186643db35d17db8a2332a7c93f3d55aae8dfa8757",
 "sources/level-anim-load-display-cut-pos/storage.hpp": "4a6ae77cda114247d6b4362ebcdb50e75933db50fa409b07d3ab23c0ecbc095a",
 "sources/level-anim-load-display-frame/storage.cpp": "8150f5cf26f99db57d80f789ba8e34b0ae9f5accef0bc1b62ab1060d4af62f72",
 "sources/level-anim-load-display-frame/storage.hpp": "13dd92e267d9d92161c198c6f357770c6f634ad177ebff00b967bdf7f1bea079",
 "sources/level-anim-load-find-height-scale/storage.cpp": "f7c368856c061553679db40b481351de203e7a69e5f4f366797345fae31eb1f5",
 "sources/level-anim-load-find-height-scale/storage.hpp": "3070e769f32123312ef9d1d768736613ae32fd41315fb7f09d8af8e98635f7e0",
 "sources/level-anim-load-forbidden-scale/storage.cpp": "fe8ad9d6a5365ef61f417da97aadddd1ef9df862188f8c81d159c48ad5ba4972",
 "sources/level-anim-load-forbidden-scale/storage.hpp": "fbc870dbb1333e34da32f09221cfb9726abcfcec28e1bcde66acab2f95010bf6",
 "sources/level-anim-load-frame-states/storage.cpp": "bf0eb334aea12fec3bf5d729a4473db8aa689cb0b665c6ac00833d1f0e800e78",
 "sources/level-anim-load-frame-states/storage.hpp": "576cc78e5ae49c2444fadfb1fd5e856ee150291e3971ab068bbd2b8328c76da3",
 "sources/level-anim-load-loader-failure-word/failure_word.cpp": "f8f47f4028b1ca99e6bceb0a9f9b8fca00dff4554b2b1ffde7c9a18896ad4791",
 "sources/level-anim-load-lump-find/lump_find.cpp": "c42ad46490d371bd75ecf661ce013afb7a600f348be1eb290a8af6bfaeb1dac3",
 "sources/level-anim-load-monster-spawn/monster_spawn.cpp": "41852c6fca73652d77e6e436e8fd4e694af4c7d81395103f26d3ca6d5d2ff635",
 "sources/level-anim-load-music-size/storage.cpp": "338a56b5c62a5eaee000e70050708e5dd11c8cd3f65bc71d7c0608bfe2e5b6cf",
 "sources/level-anim-load-music-size/storage.hpp": "d928af1bdfe29d783ee0091c2746778d597cc2eea2ecb36f725ac37685e4140c",
 "sources/level-anim-load-net-nm-ready/storage.cpp": "035a1c16e1a850633b12abb725365691fb0004ca6640c9e3ff06eab5d38f47d6",
 "sources/level-anim-load-net-nm-ready/storage.hpp": "4e3249194e09b0ec328f96538a278bdd23eb2ad82cc07d2576831b7a6eec01d9",
 "sources/level-anim-load-net-we-sent-ready/storage.cpp": "b7bf9a067326876e20bbda29c026042b22d783d2e801e4c1ffb07932886b9159",
 "sources/level-anim-load-net-we-sent-ready/storage.hpp": "e4be98540c128f9c3787fb31d9a2e956db8b8c2b86f98a8880d65db656038ca0",
 "sources/level-anim-load-route-unroll-vnode/route_unroll_vnode.cpp": "a38d38a778df2af3aece41012c154e25cc6e568a34ffff9936c1de33f25f0170",
 "sources/level-anim-load-scene-state/storage.cpp": "8e60f5381b4f6c157145d06d19ee5006a8f0c50fc1cff412b82810351def5bfb",
 "sources/level-anim-load-scene-state/storage.hpp": "00aba2b6a9a2119826570f8b2b519cac1e40fe57efcc556e0301fbc3b3912bb7",
 "sources/level-anim-load-sound-free-ram/sound_free_ram.cpp": "127210bc2d028fd50c2badee83fbecc32c8ff93e666bbdb50e7d33efef5993d4",
 "sources/level-anim-load-swap-int/swap_int.cpp": "8088ab150d71b03a34b39071a50c16fd1c7ddfdb9eb1778f3c7994d967441ff3",
 "sources/level-anim-load-world-flags-word/storage.cpp": "71713aa33efb3534b88beb6a27505de5e382feb9a65edaaded4a66b2b1270e4d",
 "sources/level-anim-load-world-flags-word/storage.hpp": "d03b361b4224c4073d84b68b95bd929135262b29a62c1d45b90257ab47b94efa",
 "sources/level-anim-load-world-pointer-word/storage.cpp": "e2db2cdd572be421e16f843c5ad70ce7e5c58b09c1f158a57df1d520208faf5e",
 "sources/level-anim-load-world-pointer-word/storage.hpp": "2cade5859b88f4436d8b092d388a8eb7f813138bb940baa1540674a0917b8308"
}
```

## Authored but never executed

All of it. The function bodies ran only on the host, against hand-derived expectations and an independent description of the random generator, with the storage reached through an address arena. The data units were compared byte for byte on the host. The EE compiled every unit and the partial link accepted them; no EE or game path ran, so the animation timing, the cutscene start, the route unroll on real node data and the spawn selection are untested in the sense of gameplay.

## Unknown

- What `updateAnimation` does to the link record, and what the link fields at +28 and +32 through +48 mean beyond the arithmetic `animSetToEnd` applies.
- Why `animAddTransitionTo` compares the rate against 435.0 before dividing.
- The meaning of the first `_cutscene` word's bit 1 (it skips `coreStartLoading`), and the types of `cutTime`, `dialogActor` and the display words.
- The behaviour of `spawn__FG6Point3iisPcbPPci`, the intent of passing the tail of the type text to `strstr`, and the image's behaviour when the type text is missing, unterminated, or has more than 16 entries.
- Whether anything reads `norrath_loader_failure_word_view` back.
- Whether the four same-named `cd` symbols in the image, other than the network one, belong to this package; only the one at `0x004f3e90` was claimed.
