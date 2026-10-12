# Integration log

Official unresolved-import counts come from the default authored partial link
(`python3 tools/build_partial.py` with the reviewed EE toolchain, no Newlib
root). COMMON stayed zero. Nothing here was executed on the EE or as gameplay.

## Baseline

Before these packages, revision `6ec25da` linked **1029** unresolved imports,
434 function bodies, and 310 authored inputs.

## agents/math

Merged. The notes separate observed bytes from inferred reconstruction names,
the sources are authored C++ rather than a disassembly dump, and each function
has a host test that returns failure. No game binary was committed.

The worker's private trial was not the official count. This link reproduces
the claimed delta exactly.

| | |
|---|---|
| Before | 1029 |
| After | 1009 |
| Closed | 20 |
| Opened | 0 |

Closed: `norrath_factory_double_isinf`, `norrath_factory_double_isnan`,
`norrath_entity_floorf`, `norrath_entity_fptodp`, `norrath_icos_scale`,
`norrath_icos_cubic`, `norrath_icos_quintic`, `norrath_icos_septic`,
`norrath_isin_scale`, `norrath_isin_cubic`, `norrath_isin_quintic`,
`norrath_isin_septic`, `norrath_animation_math_atan_high_view`,
`norrath_animation_math_atan_low_view`,
`norrath_animation_math_atan_coefficients_view`,
`norrath_animation_math_view_47d3b0`, `norrath_animation_math_view_47d3b8`,
`norrath_animation_math_view_47d3c0`, `norrath_animation_math_view_47d434`,
`norrath_player_softdouble_nan_parts_view`.

Opened: none.

Compiled bodies matched the notes: infinity 60, NaN 44, entity floor 232,
float-to-double 104. The four table units emit no functions. `floorf` and
`ceilf` stay optional external inputs. Host checks are recorded in the
snapshot and are not plan inputs. 214 tool tests passed, and the host checks
for all eight units passed on this machine. No EE or game execution.

## agents/audio

Merged. Original names are tied to addresses and separated from the published
import names. The two service requests are authored syscall sequences with host
checks that fail when the compiled body differs from the pinned executable.
DMA enable is explicitly not an original-byte match and has a host contract.
Storage units have host contracts. No game binary or disassembly listing was
committed.

| | |
|---|---|
| Before | 1009 |
| After | 978 |
| Closed | 31 |
| Opened | 0 |

That is the worker's claimed closure. The worker did not publish an official
unresolved total; 978 is the measured one.

Closed: `norrath_sound_add_dma_handler`, `norrath_sound_enable_dma`,
`norrath_sound_set_dma_chain`,
`norrath_sound_command_packet_buffer_view`,
`norrath_sound_command_send_buffer_view`,
`norrath_sound_command_initialization_packet_view`,
`norrath_sound_command_dma_handle_view`,
`norrath_sound_command_state_view`,
`norrath_sound_command_system_handlers_view`,
`norrath_sound_system_register_words_view`,
`norrath_sound_command_initialized_word_view`,
`norrath_sound_channel_records_view`,
`norrath_sound_channel_voice_map_view`,
`norrath_sound_key_on_words_view`,
`norrath_sound_key_off_words_view`,
`norrath_sound_group_limits_view`,
`norrath_sound_sequence_records_view`,
`norrath_sound_stream_buffer_sizes_view`,
`norrath_sound_stream_volumes_view`,
`norrath_sound_stream_records_view`,
`norrath_sound_master_volume_view`,
`norrath_sound_master_scale_view`,
`norrath_sound_delay_thread_name_view`,
`norrath_sound_global_dialog_map_view`,
`norrath_sound_global_dialog_name_view`,
`norrath_sound_global_dialog_extension_view`,
`norrath_sound_batch_iop_buffer_word_view`,
`norrath_sound_iop_buffer_word_view`,
`norrath_sound_startup_formats_view`,
`norrath_sound_startup_assert_file_view`,
`norrath_sound_startup_assert_conditions_view`.

Opened: none. `norrath_sound_enable_dma` calls the already defined
`norrath_sound_disable_interrupts` and `norrath_sound_enable_interrupts`.

Compiled bodies: DMA handler registration 16, DMA chain request 16, DMA
enable 112. The twelve storage units emit no functions. The host body
comparison matched both 16-byte requests against the pinned executable.
Thirteen host contracts passed. 214 tool tests passed. No EE, DMA, SPU, or
game path ran.

## agents/memory

Merged. Observed symbols, sizes, and initial bytes are separated from inferred
names. `memRealloc` has a host check that fails on the clamp, slack, split, and
growth paths. Data units have host checks. No game binary or disassembly
listing was committed.

The notes describe a private plan whose unresolved list still had 911 names.
That absolute count does not reproduce on the shared tip. The closed and
opened names do.

| | |
|---|---|
| Before | 978 |
| After | 950 |
| Closed | 29 |
| Opened | 1 (`memcpy`) |

Closed: `norrath_mem_realloc`, `norrath_player_malloc_bins_view`,
`norrath_player_malloc_trim_threshold_view`,
`norrath_player_malloc_top_pad_view`,
`norrath_player_malloc_break_base_view`,
`norrath_player_malloc_max_break_view`,
`norrath_player_malloc_max_total_view`,
`norrath_player_malloc_current_info_view`,
`norrath_player_malloc_lock_owner_view`,
`norrath_player_malloc_lock_count_view`,
`norrath_player_malloc_semaphore_view`, `norrath_heap_ring_word_view`,
`norrath_heap_next_fit_view`, `norrath_heap_fragment_threshold_view`,
`norrath_heap_free_bytes_view`, `norrath_heap_allocation_tag_view`,
`norrath_heap_total_bytes_view`, `norrath_heap_dump_count_view`,
`norrath_heap_check_file_view`, `norrath_heap_failure_format_view`,
`norrath_heap_dump_border_view`, `norrath_heap_dump_count_format_view`,
`norrath_heap_dump_free_format_view`, `norrath_heap_dump_used_format_view`,
`norrath_heap_dump_totals_format_view`,
`norrath_heap_dump_serial_format_view`, `norrath_block_data_pool_view`,
`norrath_block_region_view`, `norrath_enable_malloc_wrapper_byte_view`.

`norrath_mem_alloc` and `norrath_mem_free` were already defined, so the new
call does not open them. `__sce_sema_id` stays unresolved. The semaphore view
is a separate object; the notes say those two names are not yet one symbol.
`norrath_player_heap_pointer_word` stays unresolved. The realloc body is 556
bytes, matching the note. The other five units emit no functions. The
sanitized realloc check and the bin-object check passed. The heap text check
passes its byte comparisons; under AddressSanitizer it then reads one byte
past `mem.cc`. 214 tool tests passed. No EE or game path ran.

## agents/object-infra

Merged. Addresses are tied to observed symbols. The slow-run function, state,
vtable, and factory-text units each have a host contract that fails on a
mismatch. No game binary or disassembly listing was committed.

The notes' private link reported 646 unresolved imports from that branch's
own plan. That absolute count does not reproduce. The closed and opened names
do: every name under Imports closed left the unresolved list, and the
seventeen vtable slots arrived.

| | |
|---|---|
| Before | 950 |
| After | 685 |
| Closed | 282 |
| Opened | 17 |

Closed: the 282 names in `docs/agents/object-infra.md`, including
`norrath_object_add_to_slow_run_list`, the constructor and hash views, the
factory marker, and all 263 `norrath_factory_text_*` views. The hash-slot
names are assembler aliases of the constructor string tables, so each pair is
one object. `norrath_object_slow_run_list_view` is also defined. It was not a
previous import, so it does not change the count.

Opened: `norrath_vt_tf_10GameObject`, `norrath_vt_dtor_10GameObject`,
`norrath_vt_msg_hurt_10GameObject`, `norrath_vt_msg_trigger_10GameObject`,
`norrath_vt_msg_run_10GameObject`, `norrath_vt_msg_draw_10GameObject`,
`norrath_vt_msg_collision_10GameObject`, `norrath_vt_msg_route_10GameObject`,
`norrath_vt_msg_load_10GameObject`, `norrath_vt_msg_save_10GameObject`,
`norrath_vt_msg_collision_world_10GameObject`,
`norrath_vt_msg_move_collision_10GameObject`,
`norrath_vt_msg_alert_10GameObject`,
`norrath_vt_msg_get_poke_name_10GameObject`,
`norrath_vt_msg_poke_10GameObject`, `norrath_vt_clone_10GameObject`,
`norrath_vt_get_model_10GameObject`.

`norrath_list_remove` and `norrath_list_oneway_link` were already defined.
`norrath_actor_view_483ad0` was already unresolved. The slow-run body is 304
bytes. The other three units emit no functions. All four host contracts
passed. 214 tool tests passed. No PS2 or game execution. The vtable slots are
not called.

## Startup text from the shared branch

The startup lead's text unit landed on `origin/codex/norrath-first-reconstruction`
after object-infra was committed here and before that commit was pushed. It
is selected data, not one of the four packages. `main` and `launchSRS` now
reference the named strings. The unit defines no function.

The lead measured 912 on the memory tip (950 − 38). Measured from the
object-infra tip, the same 38 names close and nothing opens.

| | |
|---|---|
| Before | 685 |
| After | 647 |
| Closed | 38 |
| Opened | 0 |

Closed: the 24 `norrath_main_rodata_*` / `norrath_main_sdata_*` addresses and
the 14 `norrath_launchsrs_rodata_*` / `norrath_launchsrs_sdata_*` addresses
listed in `docs/STARTUP_TEXT.md`. Opened: none. 443 function bodies, 344
authored inputs, zero COMMON. 214 tool tests passed. No startup stage ran.

## agents/megatile

Merged. Observed original names stay tied to addresses. Each unit has a host
test that returns failure. No game binary or disassembly listing was committed.
Host checks for specialparse, entity dispatch, and the table bytes passed.

The notes list 47 defined names as closed. All 47 are defined. Only 23 of
them were unresolved imports on the shared link, so the official delta is
smaller than that list.

| | |
|---|---|
| Before | 647 |
| After | 626 |
| Closed | 23 |
| Opened | 2 |

Closed: `norrath_megatile_specialparse`, `norrath_megatile_do_entity_dispatch`,
`norrath_megatile_regions_view`, `norrath_megatile_x_view`,
`norrath_megatile_y_view`, `norrath_megatile_grid_origin_view`,
`norrath_megatile_pm_pointers_view`, `norrath_megatile_pm_characters_view`,
`norrath_megatile_counters_view`, `norrath_megatile_world_word`,
`norrath_megatile_scale_view`, `norrath_megatile_name_a_view`,
`norrath_megatile_name_b_view`, `norrath_megatile_name_c_view`,
`norrath_megatile_name_d_view`, `norrath_megatile_name_e_view`,
`norrath_megatile_name_f_view`, `norrath_megatile_pm_name_view`,
`norrath_megatile_pm_a_view`, `norrath_megatile_pm_b_view`,
`norrath_megatile_pm_c_view`, `norrath_megatile_count_format_view`,
`norrath_megatile_point_format_view`.

The other 24 names in the notes are defined and were not previous imports:
`norrath_megatile_dynlevels_view`, `norrath_megatile_level_name_view`,
`norrath_megatile_closest_limit_view`, and the trigger, format, kitten, and
spawn text objects.

Opened: `norrath_factory_sscanf`, `norrath_swap_int`. The other names the
notes leave open were already unresolved or already defined, including
`norrath_megatile_add_pm_list` and `norrath_megatile_cap_pm_pointers`.
Bodies: specialparse 2404, entity dispatch 1996. The table unit emits no
functions. 445 function bodies, 347 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

## agents/script-amx

Merged. The selected executor is `sources/script-amx-exec/exec.cpp`. The older
held draft `sources/game-amx-exec-native-transport-source-v4` is unchanged,
still stops on `#error`, and is not a plan input. Each new unit has a host
test that returns failure. No game binary or disassembly listing was committed.
All nine host checks passed.

| | |
|---|---|
| Before | 626 |
| After | 752 |
| Closed | 10 |
| Opened | 136 |

Closed: `norrath_custom_variables_process`,
`norrath_script_current_amx_word_view`,
`norrath_script_complain_frame_word_view`,
`norrath_amx_little_endian_word_view`,
`norrath_script_game_state_data_view`, `norrath_script_post_action_view`,
`norrath_mc_pre_start_key_bytes`, `norrath_mc_start_key_bytes`,
`norrath_amx_game_native_registry_view`, `norrath_amx_exec`.

Opened: the 120 `amx*__FP5__amxPi` natives named in `docs/agents/script-amx.md`
(`amxNewWorld__FP5__amxPi` once), plus `DisableZooming__6Camera`,
`SetDistanceAtLevel__6Cameraif`, `SetMaxHeight__6Cameraf`,
`SetPitchAtLevel__6Cameraii`, `fxUpdateDust__Fiiiiiiii`, `gameCam`,
`levelFlags`, and the ten `worldWater*` words. `norrath_copy_bytes`,
`norrath_memory_compare`, `norrath_factory_strstr`, and
`norrath_player_sscanf` were already defined, so they did not open.
Bodies: custom variables 1360, executor 7996. The other seven units emit no
functions. 447 function bodies, 356 authored inputs, zero COMMON. 214 tool
tests passed. No retail script and no EE execution. The host executor test
does not cover every opcode slot; the notes list the unrun slots.

## agents/cd-archive

Merged the pushed tip. Observed originals stay next to the `norrath_archive_*`
names. Each unit has a host contract that returns failure. No game binary or
disassembly listing was committed. All 13 host contracts passed. This tip does
not define `cdLoad__FPcPvii`; that name stays unresolved for a later pass.

| | |
|---|---|
| Before | 752 |
| After | 763 |
| Closed | 34 |
| Opened | 45 |

Closed: the 12 archive functions and 22 views that were already imports.
`norrath_archive_no_new_vags_view` and
`norrath_archive_cd_command_count_view` are defined and were not previous
imports. Opened: the 45 names in the notes that were not already present,
including `sceCdStream`, the Sony CD/SIF helpers, the six extension inits, and
the three vblank views. `norrath_sce_printf` was already unresolved.
`norrath_player_wait_semaphore`, `norrath_player_signal_semaphore`, and
`norrath_string_compare` were already defined. `norrath_archive_map_animation_sounds`
and `norrath_archive_sound_free_ram` stay unresolved. 459 function bodies,
369 authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/libc-format

Merged. Observed `scePrintf`, `sscanf`, and `strcmp` are the published names.
`scePrintf` is an alias of `norrath_sce_printf` on the EE object. Each unit
has a host contract that returns failure. No game binary or disassembly
listing was committed. All 19 host contracts passed. `memAlloc__FiPc` stays
unresolved.

| | |
|---|---|
| Before | 763 |
| After | 740 |
| Closed | 30 |
| Opened | 7 |

Closed: the 30 names in `docs/agents/libc-format.md`, including `scePrintf`,
`sscanf`, `strcmp`, and `norrath_sce_printf`. `_putchar` is defined and was
not a previous import. Opened: `_printf`, `close`, `deci2Putchar`, `lseek`,
`read`, `serialPutchar`, `write`. The notes' already-defined callees stayed
defined. Bodies match the notes, including both printf names at 104 bytes,
`sscanf` 344, and `strcmp` 40. 472 function bodies, 388 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

## agents/items-player

Merged. Original startup names `initChars__FiPPc`, `inventoryInitNet__Fv`,
`levelName`, `picInit__4itemv`, and `shopInitNet__Fv` are defined. Each plan
unit has a host test. Item, named-item, and proxy-mesh rows assert a 32-bit
layout, so those three host programs do not run on this 64-bit machine; the
EE compile includes the same assert and succeeded. No game binary or
disassembly listing was committed. `memAlloc__FiPc` stays unresolved.

| | |
|---|---|
| Before | 740 |
| After | 692 |
| Closed | 100 |
| Opened | 52 |

Closed: the names in `docs/agents/items-player.md` except `nmPlayers`, which
is defined and was not a previous import. Opened: 52 vtable and message
symbols from that notes list. `cdLoad__FPcPvii`, `exit`, `launchSRS`, `main`,
`memAlloc__FiPc`, `netNetworkMode`, `strcmp`, and `_fbss` were already
unresolved or already defined. 477 function bodies, 421 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

## agents/save-net-anim

Merged. Three view units are on the branch and were not selected, because
those symbols are already in the link. The selected units have host tests
under `host-tests/save-net-anim/`; 48 host tests passed. No game binary or
disassembly listing was committed. `norrath_archive_sound_free_ram` stays
unresolved.

| | |
|---|---|
| Before | 692 |
| After | 738 |
| Closed | 44 |
| Opened | 90 |

Closed: the 44 names that left the unresolved list, including
`norrath_archive_map_animation_sounds`, `sceMcInit`, and the startup names
`commInitNet__Fv` through `textInitNet__Fv`. Opened: 90 names, including the
memory-card helpers, network words, and `sprintf` / `strcpy`. Not selected:
`norrath_archive_animation_extension_view` (cd-archive already defines the
larger extension table), `norrath_mc_pre_start_key_bytes`, and
`norrath_mc_start_key_bytes` (script-amx already defines both keys). 489
function bodies, 464 authored inputs, zero COMMON. 214 tool tests passed.
No EE or game execution.

## agents/engine-init

Merged ahead of the remaining packages. This is the first missing link after
`main`. Both functions have host contracts that returned failure when the
order or the GS and DMA values were wrong. Both contracts passed. No game
binary or disassembly listing was committed. The compiled `engineInit__Ff`
body is 2080 bytes; the notes say that is not an original-byte match of the
1944-byte image. The stack check body is 144 bytes.

| | |
|---|---|
| Before | 738 |
| After | 789 |
| Closed | 2 |
| Opened | 53 |

Closed: `engineInit__Ff`, `_engineCheckStack__FPCci`. Opened: 53 names,
including `floorInit__Fv`, `vblankHandler__Fi`, `memInit__FPvi`, the GS and
DMA calls, and `vblankCount`, `vblankField`, `vblankSemaphore`, and
`vblankSemaphore2`. The cd-archive package already defines separate
`norrath_archive_vblank_*` views and does not alias these original names.
491 function bodies, 466 authored inputs, zero COMMON. 214 tool tests passed.
No EE startup stage ran.

## agents/world-state

All 38 units have a host contract that returns non-zero when the initial
bytes or the route allocation and reclaim path are wrong. All 38 passed.
No game binary or disassembly listing was committed. The route body compiles
to 1048 bytes; the notes say the original body is 572 bytes, so this is not
an original-byte match. Notes that mark an ELF size of zero as inferred stay
labeled that way.

| | |
|---|---|
| Before | 789 |
| After | 756 |
| Closed | 34 |
| Opened | 1 |

The notes list 38 defined names. All 38 are defined. Four were not previous
imports, so they do not change the count: `norrath_route_nodes_bytes`,
`norrath_route_cache_bytes`, `norrath_route_node_used_bytes`, and
`norrath_route_free_root_bytes`. Opened: `norrath_route_unroll_vnode`.
`norrath_world_find_height` was already defined. 492 function bodies, 504
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/render-lights

24 units were selected. Each host contract failed when the reviewed EE
object did not carry the expected symbol, bytes, or behavior. All 24 passed.
No game binary or disassembly listing was committed. The line body is 172
bytes; the notes say the original is 164. The screen-color, cache, and logo
bodies are not original-byte matches.

The two border units were not selected. cd-archive already defines
`norrath_archive_engine_pop_border` and
`norrath_archive_engine_push_border`. Selecting them again would multiply
those symbols.

| | |
|---|---|
| Before | 756 |
| After | 750 |
| Closed | 24 |
| Opened | 18 |

Closed: the 24 selected names that were already imports, including
`norrath_game_line_add`, `engineSetScreenColor__Fiiii`, both texture-cache
functions, both logo functions, the four vtables, the font and halo pointers,
and the clear-color and tag objects. Also defined, and not previous imports:
`compositeR`, `compositeG`, `compositeB`, `uncompCache`, `uncompCacheAlloc`,
`uncompCacheBuffer`, `norrath_game_line_count`, and `norrath_game_lines`.
Opened: 18 names, including `norrath_construct_Line`, `frameNm`,
`textureDrawColor__FP8_textureiiiUi`, and the light and cosmetic vtable
slots. 498 function bodies, 528 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/data-views

63 of 98 view objects were selected. Each selected host check returns non-zero
when the initial scalar, string, or virtual-slot function disagrees. All 63
passed. No game binary or disassembly listing was committed. These units add
data, not function bodies.

35 player-view units were not selected. items-player already defines those
names, including `norrath_player_view_484ca8`. Selecting them again would
multiply the symbols. The files stay in the source snapshot.

| | |
|---|---|
| Before | 750 |
| After | 782 |
| Closed | 63 |
| Opened | 95 |

Closed: `norrath_actor_view_42b118`, `norrath_actor_view_42b768`, `norrath_actor_view_42baa8`, `norrath_actor_view_42c130`, `norrath_actor_view_42c710`, `norrath_actor_view_42d0c0`, `norrath_actor_view_4488f0`, `norrath_actor_view_44b170`, `norrath_actor_view_44b540`, `norrath_actor_view_44c170`, `norrath_actor_view_44e1c0`, `norrath_actor_view_44e6a0`, `norrath_actor_view_44f4c8`, `norrath_actor_view_460710`, `norrath_actor_view_460720`, `norrath_actor_view_460730`, `norrath_actor_view_460740`, `norrath_actor_view_47fb54`, `norrath_actor_view_480eec`, `norrath_actor_view_480ef0`, `norrath_actor_view_48113c`, `norrath_actor_view_481140`, `norrath_actor_view_4813a4`, `norrath_actor_view_4813a8`, `norrath_actor_view_4813ac`, `norrath_actor_view_4816d8`, `norrath_actor_view_481a08`, `norrath_actor_view_481a0c`, `norrath_actor_view_481a10`, `norrath_actor_view_4837a8`, `norrath_actor_view_4837b0`, `norrath_actor_view_4837b8`, `norrath_actor_view_4837c0`, `norrath_actor_view_4837c8`, `norrath_actor_view_4837d0`, `norrath_actor_view_4837d8`, `norrath_actor_view_483ad0`, `norrath_actor_view_484318`, `norrath_actor_view_4844a4`, `norrath_actor_view_484f98`, `norrath_actor_view_485150`, `norrath_actor_view_485410`, `norrath_actor_view_485418`, `norrath_creature_view_47fb58`, `norrath_creature_view_47fb5c`, `norrath_creature_view_47fb60`, `norrath_creature_view_47fb64`, `norrath_creature_view_47fb68`, `norrath_creature_view_4837e0`, `norrath_creature_view_483829`, `norrath_creature_view_483e58`, `norrath_creature_view_484538`, `norrath_creature_view_484cac`, `norrath_creature_view_484cd8`, `norrath_creature_view_48690c`, `norrath_creature_view_486910`, `norrath_next_view_42c650`, `norrath_next_view_4490d0`, `norrath_next_view_449160`, `norrath_next_view_4491f0`, `norrath_next_view_449280`, `norrath_next_view_44c958`, `norrath_next_view_44e820`. The 35 player views were already defined, so they were not imports. Opened: `_$_11SmallSpider`, `_$_13PokeReflector`, `_$_13TriggerParams`, `_$_14GnomeNavigator`, `_$_3Cat`, `_$_4Trap`, `_$_5Lever`, `_$_5Timer`, `_$_7Counter`, `_$_7Mermaid`, `_$_7Natasla`, `_$_8Creature`, `_$_9CdEyeball`, `__tf11SmallSpider`, `__tf13PokeReflector`, `__tf13TriggerParams`, `__tf14GnomeNavigator`, `__tf3Cat`, `__tf4Trap`, `__tf5Lever`, `__tf5Timer`, `__tf7Counter`, `__tf7Mermaid`, `__tf7Natasla`, `__tf8Creature`, `__tf9CdEyeball`, `clone__10GameObject`, `clone__11SmallSpider`, `clone__13PokeReflector`, `clone__13TriggerParams`, `clone__3Cat`, `clone__5Lever`, `clone__5Timer`, `clone__7Counter`, `clone__8Creature`, `clone__9CdEyeball`, `confuse__8Creaturei`, `death__8Creature`, `getMinibossName__8Creature`, `getModel__8Creature`, `getSkillRank__8Creaturei`, `initMiniboss__8Creature`, `isEnemy__8CreatureP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_collisionWorld__8CreatureG6Point3`, `msg_collision__11SmallSpiderP10GameObject`, `msg_collision__3CatP10GameObject`, `msg_collision__8CreatureP10GameObject`, `msg_draw__11SmallSpider`, `msg_draw__14GnomeNavigator`, `msg_draw__3Cat`, `msg_draw__4Trap`, `msg_draw__5Lever`, `msg_draw__7Mermaid`, `msg_draw__7Natasla`, `msg_draw__9CdEyeball`, `msg_getPokeName__13PokeReflectorPUsi`, `msg_getPokeName__5LeverPUsi`, `msg_getPokeName__8CreaturePUsi`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_load__5LeverPc`, `msg_load__7CounterPc`, `msg_load__8CreaturePc`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_poke__13PokeReflectori`, `msg_poke__5Leveri`, `msg_poke__8Creaturei`, `msg_route__8Creaturei`, `msg_run__11SmallSpider`, `msg_run__14GnomeNavigator`, `msg_run__3Cat`, `msg_run__4Trap`, `msg_run__5Lever`, `msg_run__5Timer`, `msg_run__7Mermaid`, `msg_run__7Natasla`, `msg_run__9CdEyeball`, `msg_save__5LeverPc`, `msg_save__7CounterPc`, `msg_save__8CreaturePc`, `msg_trigger__13TriggerParamsi`, `msg_trigger__5Timeri`, `msg_trigger__7Counteri`, `msg_trigger__8Creaturei`, `runSpells__8CreatureG6Point3`, `setActiveChunks__8Creature`, `setState__11SmallSpideri`, `setState__14GnomeNavigatori`, `setState__3Cati`, `setState__7Mermaidi`, `setState__7Nataslai`, `setState__8Creaturei`, `stun__8Creaturei`, `turnToFace__8CreatureiG6Point3`, `updateEnemy__8Creature`. Notes that list a larger slot set include names that were already unresolved or already defined. 498 function
bodies, 591 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/actors-b

All 13 constructors were selected. Each host contract returns non-zero when
the call order, stored vtable, or forwarded result is wrong. All 13 passed.
The host compile defines `NORRATH_ACTORS_B_HOST`, which the shared header
requires where pointers are wider than 32 bits. No game binary or disassembly
listing was committed. The compiled bodies are larger than the image extents
in the notes. GothTrap is 188 bytes here and 116 bytes in the notes.

| | |
|---|---|
| Before | 782 |
| After | 796 |
| Closed | 13 |
| Opened | 27 |

Closed: the 13 `norrath_construct_*` names. Opened: `norrath_construct_Vehicle`,
`norrath_vehicle_reset`, `norrath_goth_trap_set_passable_state`,
`norrath_hate_bridge_set_passable_state`, `norrath_actor_add_medium_run_list`,
`norrath_world_get_door_height`, `norrath_world_check_for_water`, and the 20
`norrath_actors_b_view_*` addresses. `sscanf` is already defined, so it did
not open. 511 function bodies, 604 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

## agents/gameobject-base

All 18 units were selected. Each host check returns non-zero when the copy,
destructor order, clone bytes, or leaf behavior is wrong. All 18 passed.
The destructor host compile defines `NORRATH_GAMEOBJECT_HOST_CHECK`. The
memcpy host check calls the same body under `norrath_memcpy_under_test` so
it does not replace the host library. No game binary or disassembly listing
was committed. `memcpy` compiles to 540 bytes; the notes say the image is
176. The destructor is 736 against 680, type info 72 against 60, and clone
512 against 244. The empty leaves compile to 8 bytes, matching the notes.

The notes' private total of 632 unresolved imports does not reproduce on
this tip.

| | |
|---|---|
| Before | 796 |
| After | 781 |
| Closed | 18 |
| Opened | 3 |

Closed: `memcpy` and the 17 `norrath_vt_*_10GameObject` names. Also defined,
and not a previous import: `norrath_ti_10GameObject`. Opened:
`norrath_object_remove_string`, `norrath_list_head_copy_construct`, and
`norrath_rtti_user`. 529 function bodies, 622 authored inputs, zero COMMON.
214 tool tests passed. No EE or game execution.

## agents/libc-format second pass

The branch gained `strtod` and an `sprintf` alias on the existing format
string. Both host checks passed. `sprintf` is an alias of
`norrath_format_string` on the EE object; each symbol is 172 bytes.
`strtod` is 32 bytes and forwards to the already defined
`norrath_player_strtod_reentrant`. No game binary was committed.

| | |
|---|---|
| Before | 781 |
| After | 779 |
| Closed | 2 |
| Opened | 0 |

Closed: `sprintf`, `strtod`. 531 function bodies, 623 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

## agents/scheduler

All nine units were selected. Host checks passed. Body sizes match the notes:
`engineAddTask__FPFv_viPci` 1140, `engineRemoveTask__FPFv_v` 1088,
`engineRunTasks__Fii` 512, `engineTaskActive__FPc` 360,
`enginePrintTasks__Fv` 120. The four word objects have empty function lists.
No game binary was committed. `strcmp` was already defined, so it did not
open. `currentTask` and `nmTasks` are defined and were not previous imports.

| | |
|---|---|
| Before | 779 |
| After | 775 |
| Closed | 5 |
| Opened | 1 |

Closed: `engineAddTask__FPFv_viPci`, `enginePrintTasks__Fv`,
`norrath_game_bail_from_loop_view`, `norrath_game_frame_number_word_view`,
`norrath_game_pause_word_view`. Opened: `taskList`. 536 function bodies, 632
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/game-boot

All five units were selected. Host checks include the implementation and passed
when compiled alone. Body sizes match the notes: `DLG_Init__Fv` 112,
`gameBoot__Fv` 136, `gameInit__Fv` 888, `debugCheckForPauseMenuStart__Fv` 1184,
`simpleDrawLoop__FPFv_i` 464. `gameBoot__Fv` also defines `_$tmp_0.1090`,
which was not a previous import. No game binary was committed.

| | |
|---|---|
| Before | 775 |
| After | 870 |
| Closed | 5 |
| Opened | 100 |

Closed: `DLG_Init__Fv`, `gameBoot__Fv`, `gameInit__Fv`,
`debugCheckForPauseMenuStart__Fv`, `simpleDrawLoop__FPFv_i`. The notes listed
105 new imports. Five of those were already present or already defined:
`MC_Debug`, `frameNm`, and `gameCam` were already unresolved; `nmPlayers` and
`engineRemoveTask__FPFv_v` are already defined. 541 function bodies, 637
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/subsystem-inits

All ten units were selected. Host checks passed. `engineCurrentLevelName` is
80 zero bytes and `eRandState` is a data object. No game binary was committed.
`memset` and `engineAddTask__FPFv_viPci` were already defined. `pad` was
already unresolved.

| | |
|---|---|
| Before | 870 |
| After | 907 |
| Closed | 10 |
| Opened | 47 |

Closed: `padInit__Fv`, `emathInit__Fv`, `listInit__Fv`, `animInit__Fv`,
`objectInit__Fv`, `worldInit__Fv`, `P_Init__Fv`, `lightInit__Fv`,
`eRandState`, `engineCurrentLevelName`. The notes listed 50 new imports.
`memset` and `engineAddTask__FPFv_viPci` are already defined, and `pad` was
already unresolved, so 47 names opened. 549 function bodies, 647 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/gs-dma-kernel

All 25 units were selected. Nine assembled bodies matched the private image
byte for byte, including `sceGsResetPath` at 104 bytes. The other functions
passed their host contracts. `vblank_state` defines `vblankCount`,
`vblankField`, `vblankHID`, `vblankSemaphore`, and `vblankSemaphore2` and does
not define the cd-archive view names. No game binary was committed.

| | |
|---|---|
| Before | 907 |
| After | 893 |
| Closed | 19 |
| Opened | 5 |

Closed 19 names that were already unresolved, including `vblankHandler__Fi`
and the five vblank words. Also defined, and not previous imports:
`iSignalSema`, `RemoveIntcHandler`, `SetGsCrt`, `GsPutIMR`,
`_GetGsDxDyOffset`, `DisableIntc`, `sceGsGetGParam`, `CheckAddress`,
`memclr`, `WaitDma`, plus the vblank data that was already counted above.
Opened: `checkModelVersion`, `draw`, `sceDmaPause`, `sceDmaPutEnv`,
`specialMpegMode`. 574 function bodies, 672 authored inputs, zero COMMON.
214 tool tests passed. The byte comparisons are host checks. They are not an
exact-match ledger admission. No EE or game execution.

## agents/draw-texture-init

All 18 units were selected. Host checks passed. `initVIF__Fv` and
`drawInit__Fv` compile their MMIO stores as real stores on the EE and as
host callbacks under `NORRATH_HOST_CONTRACT`. Compiled bodies:
`initSupersample__Fv` 1284, `textureInitTexture__FP8_texture` 616,
`drawInit__Fv` 496, `initVIF__Fv` 216, `textureInit__Fii` 108. No game
binary was committed.

| | |
|---|---|
| Before | 893 |
| After | 884 |
| Closed | 17 |
| Opened | 8 |

Closed the 17 names above that were already unresolved, including `dmaGif`,
`dmaVif0`, `dmaVif1`, and `textureBufferStartAddress`. Also defined, and not
previous imports: `textureBufferSize`, `textureMaxBlockSize`, `drawRecords`,
`drawHead`, `endTagVU1`, `writeBuffer`, `nmDrawUsed`, `endDrawSemaphore`,
`vif0HID`, `vif1HID`, `gifHID`, `fullFrameMode`, `packetBuffer`,
`zwritesOn`, `zwritesOff`. Opened: `AddDmacHandler`, `EnableDmac`,
`gsIntHandler__Fi`, `makeSamplePacket__Fb`, `reloadDMA__Fi`, `sceDmaSend`,
`sceGsSyncV`, `textureInternalInit__Fv`. 579 function bodies, 690 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/frame-loop

All nine units were selected. Host checks passed.
`engineBrightnessCorrection` is the initial word `0x3f000000`. Compiled
bodies: `engineFrameEnd__Fb` 1020, `engineFrameStart__FUib` 312,
`engineSetDisplayDisable__Fi` 12. No game binary was committed.

| | |
|---|---|
| Before | 884 |
| After | 888 |
| Closed | 9 |
| Opened | 13 |

Closed: `engineFrameEnd__Fb`, `engineFrameStart__FUib`,
`engineSetDisplayDisable__Fi`, `engineBrightnessCorrection`, `frameNm`,
`glistp`, `gstart`, `GLISTSIZE`, `coreClearColor`. Opened:
`coreLoadingScreenRunning`, `doSuperSample__Fiib`, `drawFrameNm`,
`drawStart__Fv`, `drawWaitFinished__Fv`, `engineSetVideoMode__Fb`,
`frameStartPacket__FUibi`, `lastFrameVBlankCount`,
`lightVU0StoppedProcessing__Fv`, `powerOffMode`, `switchTo30Counter.798`,
`switchToHalfCounter.797`, `texturePackDown__Fv`. 582 function bodies, 699
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/dialog-hud-controls

All 17 units were selected. Host checks passed. The notes heading is
"Imports you opened"; the five summary sections are present. No game binary
was committed. `nmPlayers` and `player` stay unresolved. `engineAddTask__FPFv_viPci`,
`netNetworkMode`, and `scePrintf` were already defined.

| | |
|---|---|
| Before | 888 |
| After | 836 |
| Closed | 59 |
| Opened | 7 |

Closed 59 names, including `DLG_Active__Fv`, `helpMessageInit__Fv`,
`menuSetPos__FP4Menui`, `msgHudSetPlayerBusy__Fcc`, `netGetLocalPlayerNm__Fi`,
`netPlayerIsLocal__Fi`, `soundFadeStream__Fi`, `soundUpdateMusicVolume__Fv`,
`specialSoundHandler__FiG6Point3`, `worldSetLightHaloTexture__FP8_texture`,
and the button, dialog, menu, and volume words those units define. Opened:
`gameFootstepSound__FG6Point3iii`, `helpMessageFrame__Fv`, `lightHaloTexture`,
`musicDuckAmount`, `netSendMessage__FiPUci`, `soundSetStreamVolume__Fif`,
`stream`. 592 function bodies, 716 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

## agents/natives-a

All 40 units were selected. Host contracts passed. Body sizes match the
notes. No game binary was committed.

| | |
|---|---|
| Before | 836 |
| After | 829 |
| Closed | 40 |
| Opened | 33 |

Closed the 40 `amx*` natives this package defines. Opened:
`DLG_FireUpShopOnExit`, `DLG_ForceMutexRelease__Fv`, `DLG_SwoopMode`,
`MC_SaveConfigSilent__Fii`, `P_AddParticle__FP11ParticleDefP6Point3T1f`,
`__eq__C9type_infoRC9type_info`, `__tf8AutoProp`, `activePlayer`,
`addQuestToLog__FPcT0`, `amx_GetAddr`, `amx_GetString`, `charm__8Creaturei`,
`checkForMonsters__FP10GameObject`, `cutStartScene__FP9_cutscenePc`,
`floorAdd__FG6Point3sfii`, `gameCheckPlayerAlive__Fv`,
`gamePlayerLightAttenuate`, `hudAddHelpMessage__FiPUsT1`, `nmPlayersAlive`,
`noMonsters`, `objectFindObject__FPc`, `objectFindObject__FPcPP10GameObjecti`,
`objectIterateGrid__FPFP10GameObject_v`, `objectUpdateInGrid__FP10GameObject`,
`playersAlive`, `queryInventoryItem__FPci`, `scriptAddDeferedCall__FPc`,
`scriptGetGameState__FPc`, `scriptParticles`, `scriptSetGameState__FPci`,
`scriptStart__FPc`, `shopStart__Fiii`, `syncRandState`. 632 function bodies,
756 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

## agents/natives-c

All 47 units were selected. Host checks passed. `gameCam` and `levelFlags`
are data. Two units also emit file-local helpers. No game binary was
committed.

| | |
|---|---|
| Before | 829 |
| After | 826 |
| Closed | 47 |
| Opened | 44 |

Closed the 45 functions this package defines, plus `gameCam` and
`levelFlags`. Opened: `DLG_StartDialog__FPcP9DlgPacketii`,
`DLG_StartWithSwoopDone`, `MC_GetAsyncMode__Fv`, `MC_SaveCheckPoint__Fiii`,
`MC_SaveConfig__Fv`, `MC_SaveGameBrowser__Fi`,
`P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `__tfP4Prop`,
`__tfP6PhProp`, `__tfP8ItemProp`,
`animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`,
`animAdd__FP14AnimationStateP15AnimationHeaderi`,
`animInitState__FP14AnimationState`, `animResetFramestates__FP14AnimationState`,
`animSetToEnd__FP14AnimationState`, `findTagString__10GameObjectPci`,
`floorDrawEnable`, `fxDust`, `gameNetKick__Fi`, `gameNoDraw`, `gameOver`,
`gameRecallInhibit`, `gameSetCameraShake__Ffb`, `hudSetInfoMessage__FPUsiic`,
`hudShowBust__FPcPUsii`, `hudShowCompass__FG6Point3PUs`,
`inventoryGetItemIndex__FiPcPiT2`, `lumpFind__FPvPc`, `objectDrawEnable`,
`objectFindInBox__FG6Point3T0PP10GameObjecti`, `playerSelectionSetup__Fv`,
`playerSelectionStart__Fv`, `removeItemFromInventory__Fiiii`,
`removeQuestFromLog__FPc`, `scriptInPostAction`, `scriptSetPostAction__FPc`,
`shopRunning`, `soundMakeSequenceUnpausable__Fi`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`soundPlaySound__FP10_vagHeaderP6Point3fff`, `soundStartDialogStream__FiPci`,
`soundStartMusic__FPci`, `startEntranceName`, `worldSetFarClip__Ff`.
679 function bodies, 803 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/floor

All three units were selected. Host checks passed. `floorDraw__Fv` is 6400
bytes and a file-local helper is 1396. `floorInit__Fv` is 52. No game
binary was committed. `glistp`, `zwritesOn`, and `zwritesOff` were already
defined, so they did not open.

| | |
|---|---|
| Before | 826 |
| After | 819 |
| Closed | 12 |
| Opened | 5 |

Closed: `floorInit__Fv`, `floorDrawEnable`, and the ten
`norrath_floor_*` views this package defines. `floorDraw__Fv` is defined
and was not a previous import. Opened: `__11ResourceMapPcT1`,
`drawAddToList__FPviP8_textureP11_drawRecordi`, `worldCameraPos`,
`worldClipCubeToFrustum__FPA1_fi`, `worldWorldViewMatrix`. 682 function
bodies, 806 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/engine-restore

All seven units were selected. The pad host contract also checks
`engineDisplayDisable` (value 3) and `skipIntro` (value 0). Compiled bodies:
`srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i` 1200, `frontEndInit__Fi` 540,
`engineRestore__Fv` 192, `engineClear__Fv` 44. No game binary was committed.
The worker's private count of 710 does not reproduce.

| | |
|---|---|
| Before | 819 |
| After | 856 |
| Closed | 7 |
| Opened | 44 |

Closed: `engineClear__Fv`, `engineDisplayDisable`, `engineRestore__Fv`,
`frontEndInit__Fi`, `pad`, `skipIntro`,
`srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i`. Opened:
`CheckNetwork__15VINetworkStatus`, `ClearVRAM__FUcUcUcUc`, `Close__7VITimer`,
`Free__10VILauncher`, `GetDefault__10VILanguage`, `Init__10VILauncherb`,
`Init__7VITimer`, `Initialize__15VINetworkStatus`, `LoadExecPS2`,
`MC_GetReadyForMainMenu__Fv`, `MC_LoadGameBrowser__Fii`,
`SetDefault__10VILanguage12VILanguageID`, `Setup__10VILauncher`,
`Station__10VILauncheriPCcN22i`, `VIDisplayLoadingScreen__Fb`,
`VIMemCardSetProductId__FPCc`, `VIMemCardSetProductTitle__FPCc`,
`_$_10VILauncher`, `_$_15VINetworkStatus`, `__10VILauncher`,
`__15VINetworkStatus`, `attemptLoadGame`, `autodemoTimer`, `creditMenu`,
`creditsPosY`, `drawRemoveInterrupts__Fv`, `drawRestoreInterrupts__Fv`,
`emergencyMusicWaitAbort`, `engineHoldMpeg`, `exitEzNetCnf`, `exitEzNetCtl`,
`expWarnAlpha`, `ezNetCtlDownInterface`, `frontEndRunning`,
`frontEndStartInCredits`, `frontEndSuspend`, `frontMenu`,
`frontendControllerConfigRunning`, `hudNoDraw`, `lineShift`, `musicPlaying`,
`printf`, `runFrontEnd__Fv`, `sceSifInitIopHeap`. 686 function bodies, 813
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/sif-keyboard

All four units were selected. Host contracts passed. `initSif__Fv` is 480
bytes. The query pair is 120 bytes each. No game binary was committed.
`_bind` is defined and was not a previous import.

| | |
|---|---|
| Before | 856 |
| After | 865 |
| Closed | 8 |
| Opened | 17 |

Closed: `initSif__Fv`, `keyboardFree__Fv`, `keyboardRestart__Fv`,
`sceSifQueryMaxFreeMemSize`, `sceSifQueryTotalFreeMemSize`,
`sceSifSearchModuleByName`, `sceSifStopModule`, `sceSifUnloadModule`.
Opened: `Free__10VIKeyboard`, `Init__10VIKeyboard`, `PreparePowerOff__Fv`,
`_ctype_`, `_lf_bind`, `_lf_version`, `commSystemInit__Fv`, `sceCdInit`,
`sceCdMmode`, `sceFsReset`, `sceSifLoadFileReset`, `sceSifLoadModule`,
`sceSifRebootIop`, `sceSifSyncIop`, `strlen`, `strncpy`, `textSystemInit__Fv`.
694 function bodies, 817 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/actors-a

All 11 constructors were selected. `docs/agents/actors-a-compare.py` ran each
one against the private image and matched. That comparison is a host check,
not an exact-match ledger admission. The snapshot checker now accepts a
`docs/agents/*.py` host file. No game binary was committed.

| | |
|---|---|
| Before | 865 |
| After | 882 |
| Closed | 11 |
| Opened | 28 |

Closed the 11 `norrath_construct_*` names this package defines. Opened 24
`norrath_actors_a_view_*` words plus `norrath_light_add`,
`norrath_object_add_to_delete_list`, `norrath_object_add_to_medium_run_list`,
`norrath_world_find_closest_door`, and `norrath_world_set_door_collision`.
705 function bodies, 828 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/cd-archive second pass

Seven new units were selected and four existing units were updated in place.
`cdLoad__FPcPvii` is 400 bytes and `cdInit__Fv` is 204. The border and vblank
functions and seven view names are GAS aliases of the existing
`norrath_archive_*` symbols. The plan `symbol_aliases` list stays empty.
Host checks passed. No game binary was committed.

| | |
|---|---|
| Before | 882 |
| After | 886 |
| Closed | 6 |
| Opened | 10 |

Closed: `cdInit__Fv`, `cdLoad__FPcPvii`, `lumpClear__Fi`,
`lumpFindResource__FPcT0`, `lumpLoad__FPc`, `lumpLock__Fi`. Opened:
`cdWait__Fv`, `issueCommand__Fv`, `loadDirectory__FP9directoryii`,
`lumpUnload__FPv`, `sceCdReadDvdDualInfo`, `sceCdSearchFile`, `sceCdStInit`,
`sceSifAllocIopHeap`, `soundClear__Fi`, `soundLock__Fi`. 715 function
bodies, 835 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/memory second pass

Nine new units were selected. `enableMallocWrapper` is now a second name on
the already selected `norrath_enable_malloc_wrapper_byte_view`. The
same-extent check passed on that object, and the existing byte host check
passed. The plan `symbol_aliases` list stays empty.

`memAlloc__FiPc`, `memFree__FPv`, and `blockAllocInit__Fv` were authored as
aliases on three sources inside the frozen 264-input prefix. Those input
records stay the V43 receipt, so the alias lines were not kept and those
three names stay unresolved. `memory-realloc`, `memory-malloc-state`,
`memory-heap-state`, and `memory-block-storage` were already selected and
were not appended again.

| | |
|---|---|
| Before | 886 |
| After | 879 |
| Closed | 10 |
| Opened | 3 |

Closed: `__wrap_malloc`, `enableMallocWrapper`, `mallinfo`, `memAllocPtr`,
`memAllocSize`, `memClearMark__Fi`, `memDeleteMarked__Fi`, `memInit__FPvi`,
`memScrambleFree__Fv`, `memSetMark__Fi`. Opened: `_impure_ptr`,
`_mallinfo_r`, `malloc`. The notes also listed the first-pass view names and
the three prefix aliases as closed. Those views were already defined, and
the three aliases were not published. 722 function bodies, 844 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/audio second pass

`soundSleep__Fv` was selected. It is 260 bytes. The host contract passed.
`soundInit__Ff` and `soundRestore__Fb` were authored as aliases on two
sources inside the frozen 264-input prefix. Those input records stay the
V43 receipt, so the alias lines were not kept and those two names stay
unresolved. The fifteen earlier audio sources were already selected.

| | |
|---|---|
| Before | 879 |
| After | 879 |
| Closed | 1 |
| Opened | 1 |

Closed: `soundSleep__Fv`. Opened: `sceSifFreeIopHeap`. The notes also listed
`soundInit__Ff`, `soundRestore__Fb`, and the first-pass audio names as
closed. Those names were already defined or were not published. 723 function
bodies, 845 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/script-amx second pass

`scriptInit__Fv` was selected. It is 72 bytes. The host check passed. The
held AMX draft was not selected. `engineAddTask__FPFv_viPci` was already
defined, so it did not open.

| | |
|---|---|
| Before | 879 |
| After | 879 |
| Closed | 1 |
| Opened | 1 |

Closed: `scriptInit__Fv`. Opened: `scriptTask__Fv`. The notes also listed the
first-pass script names as closed. Those were already defined. 724 function
bodies, 846 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/vslots-b

All 19 units were selected. Host checks passed. The notes' plan list named
four Creature methods. The tree also contained fifteen actor methods, and
those host checks passed, so they were selected too. `setState__8Creaturei`
is a normal global. The notes say the original binding was weak.

| | |
|---|---|
| Before | 879 |
| After | 871 |
| Closed | 19 |
| Opened | 11 |

Closed: `msg_draw__11SmallSpider`, `msg_draw__3Cat`, `msg_draw__4Trap`,
`msg_draw__7Mermaid`, `msg_hurt__8CreaturefP10GameObjecti`,
`msg_load__7CounterPc`, `msg_moveCollision__8CreaturePP10GameObject`,
`msg_route__8Creaturei`, `msg_run__5Timer`, `msg_run__7Natasla`,
`msg_save__5LeverPc`, `msg_save__7CounterPc`, `msg_trigger__13TriggerParamsi`,
`msg_trigger__5Timeri`, `msg_trigger__8Creaturei`,
`runSpells__8CreatureG6Point3`, `setActiveChunks__8Creature`,
`setState__8Creaturei`, `stun__8Creaturei`. Opened: `DLG_SetMutex__Fv`,
`creatureAdvanceAnimation__8Creaturef`,
`draw__8CreatureP12_modelHeaderP8_textureT1`,
`everyFrame__8CreatureP12_modelHeader`, `iatan2__Fff`,
`modelGetActiveChunks__FP12_modelHeaderi`, `modelResetOptionalParams__Fv`,
`modelSetShadowBrightness__Ff`, `objectActivationCheck__8Creaturei`,
`objectDoMyActions__FP10GameObjecti`, `objectRemoveFromRunList__FP10GameObject`.
The notes predicted only `modelGetActiveChunks__FP12_modelHeaderi` and
`iatan2__Fff`. 743 function bodies, 865 authored inputs, zero COMMON. 214
tool tests passed. No EE or game execution.

## agents/light-methods

All 17 units were selected. Host checks passed. The four typeinfo units
also define the 12-byte `norrath_ti_*` objects. Those were not previous
imports, so they did not change the count.

| | |
|---|---|
| Before | 871 |
| After | 866 |
| Closed | 17 |
| Opened | 12 |

Closed: `norrath_construct_Line`, the four `norrath_vt_clone_*` names, the
four `norrath_vt_dtor_*` names, `norrath_vt_msg_draw_12CosmeticProp`,
`norrath_vt_msg_load_12CosmeticProp`, `norrath_vt_msg_save_12CosmeticProp`,
the four `norrath_vt_tf_*` names, and `textureDrawColor__FP8_textureiiiUi`.
Opened: `_textureDrawColor__FP8_textureiiiUi`, `getMegatileCenter__FG6Point2`,
`getMegatileOriginalCenter__FG6Point2`, `initializeMe__4Line`,
`matIdent__FP8Matrix33`, `matMakeTransYaw__FP8Matrix34G6Point3s`,
`matRot__FP8Matrix33is`, `matTransform__FP6Point3P8Matrix33T0`,
`modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`,
`norrath_rtti_si`, `objectSaveAddToStringPool__FPCc`,
`objectSaveGetFromStringPool__Fi`. `norrath_list_head_copy_construct`,
`lumpFindResource__FPcT0`, and `memAlloc__FiPc` were already unresolved.
760 function bodies, 882 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/vslots-b msg_poke

`msg_poke__8Creaturei` was selected. It is 572 bytes. The host check passed.

| | |
|---|---|
| Before | 866 |
| After | 866 |
| Closed | 1 |
| Opened | 1 |

Closed: `msg_poke__8Creaturei`. Opened: `gameSomeoneInShop__Fv`. 761 function
bodies, 883 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

## agents/vehicle-actors

Twenty-five units were selected. `norrath_construct_Vehicle` was not
selected: its host check fails on the memset value. The 21 views are data.
`norrath_vehicle_view_44ffd8` was defined and was not a previous import.

| | |
|---|---|
| Before | 866 |
| After | 950 |
| Closed | 24 |
| Opened | 108 |

Closed: `norrath_actor_add_medium_run_list`,
`norrath_goth_trap_set_passable_state`,
`norrath_hate_bridge_set_passable_state`, `norrath_vehicle_reset`, and the
twenty `norrath_actors_b_view_*` names from `448a70` through `4850f8`.
Opened 108 names, mostly the virtual-table callees plus
`worldSetDoorCollision__FP12_worldHeaderii`,
`worldMoveDoor__FP12_worldHeaderiG6Point3s`,
`worldSetDoorHeight__FP12_worldHeaderif`, `Reset__5Wheel`,
`matToQuaternion__FP4QuatP8Matrix34`, `quatToMat__FP8Matrix33P4Quat`, and
`refreshSettings__7Vehicle`. `modelGetActiveChunks__FP12_modelHeaderi` and
`matMakeTransYaw__FP8Matrix34G6Point3s` were already unresolved.
`norrath_construct_Vehicle` stays unresolved. 765 function bodies, 908
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/vslots-b lever, Natasla, and turn

Three units were selected. Host checks passed. None of them opened a new
import.

| | |
|---|---|
| Before | 950 |
| After | 947 |
| Closed | 3 |
| Opened | 0 |

Closed: `msg_load__5LeverPc`, `setState__7Nataslai`,
`turnToFace__8CreatureiG6Point3`. A later notes commit records those names
and does not change the link. 768 function bodies, 911 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/particles

All 45 units were selected. Host checks passed. Ten of them are functions.
The other 35 are data. Eleven data names were not previous imports, so they
did not change the count: `alwaysDrawList`, `bolt`, `descList`,
`modelClippingOn`, `noiseTable`, `objectIgnoreFlags`, `PS`, `psSequenceNm`,
`trail`, `trailSequenceNm`, and `worldCameraMatrix`.

| | |
|---|---|
| Before | 947 |
| After | 929 |
| Closed | 34 |
| Opened | 16 |

Closed includes `objectGrid`, `ghostObjectGrid`, `stringHash`, `objectHash`,
`offTheGrid`, `objectStrings`, `freeNodes`, `worldCameraPos`,
`drawObjectsTask__Fv`, `drawParticlesTask__Fv`, `initNoiseTable__Fv`,
`initParticleSystems__Fv`, `initTrails__Fv`, `lightResetOptionalParams__Fv`,
`listDestroy__FP8ListHead`, and `allocDescriptor__FP11ParticleDef`. Opened:
`P_DrawParticles__FP8Matrix34`, `_$_16ListSafeIterator`,
`__16ListSafeIteratorP8ListHeadi`, `ambientOnly`, `charAmbient`,
`charDirect`, `charDirect2`, `charDirectDir`, `charDirectDir2`,
`charScaleB`, `charScaleG`, `charScaleR`, `disableVolumetricLighting`,
`interfaceMode`, `next__16ListSafeIterator`, and
`worldClipCubeToClipFrustum__FPA1_fi`. 778 function bodies, 956 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/vslots-a

All 48 units were selected. Host checks passed. They are the typeinfo,
destructor, clone, and creature slot bodies in this package.

| | |
|---|---|
| Before | 929 |
| After | 889 |
| Closed | 48 |
| Opened | 8 |

Closed: the twelve `_$_*` destructors, the thirteen `__tf*` typeinfo
functions, the ten `clone__*` functions, and
`confuse__8Creaturei`, `death__8Creature`, `getMinibossName__8Creature`,
`getModel__8Creature`, `getSkillRank__8Creaturei`, `initMiniboss__8Creature`,
`isEnemy__8CreatureP10GameObject`,
`msg_alert__8CreatureifG6Point3P10GameObject`,
`msg_collisionWorld__8CreatureG6Point3`,
`msg_collision__11SmallSpiderP10GameObject`,
`msg_collision__3CatP10GameObject`, `msg_collision__8CreatureP10GameObject`.
Opened: `_$_14AnimationState`, `_$_15FxCreatureState`,
`__15AnimationState8RC15AnimationState8`, `__rtti_si`,
`dropItem__4itemG6Point3Pc`, `objectAddToDeleteList__FP10GameObject`,
`readyToSwitch__3Cat`, `switchToRun__3Cat`. 826 function bodies, 1004
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/link-image-bounds

`__register_frame_info` was selected. It is 92 bytes. The host check
passed. The unit also defines `objects`, which was not a previous import.

| | |
|---|---|
| Before | 889 |
| After | 888 |
| Closed | 1 |
| Opened | 0 |

Closed: `__register_frame_info`. `SignalSema` and `__sce_eh_sema_id` were
already unresolved. `WaitSema` was already defined. 827 function bodies,
1005 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

## agents/ctors-ant-lantern

`norrath_construct_AutoChestLP` was selected. It is 260 bytes. The EE
compile passed the 32-bit pointer assert. The host contract does not compile
on this arm64 machine because that header requires `sizeof(void*) == 4`, so
the behavioral checks did not run here.

| | |
|---|---|
| Before | 888 |
| After | 889 |
| Closed | 1 |
| Opened | 2 |

Closed: `norrath_construct_AutoChestLP`. Opened:
`__11AddParticleG6Point3PPc` and `norrath_ctors_ant_lantern_view_44ea78`.
828 function bodies, 1006 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/sound-rpc-views

All 31 units were selected. They are data objects only; no function was
added. The notes tie each view to a pinned symbol address and size and
separate the caller-proven prefix from the unnamed tail bytes. Each unit has
a host check that returns failure on a wrong size or nonzero byte. All 31
host checks passed on this machine. The previous integrator merged the
branch and left the plan and snapshot edits uncommitted; this link is the
first complete one for that selection.

| | |
|---|---|
| Before | 889 |
| After | 858 |
| Closed | 31 |
| Opened | 0 |

Closed: the 31 `norrath_sound_*_view` names listed in the package notes.
Opened: none. 828 function bodies, 1037 authored inputs, zero COMMON. 214
tool tests passed. No EE or game execution.

## agents/game-state

The previous integrator merged the branch and was cut off before the plan
and snapshot edits. The merge commit is kept; this selection appends its 22
units with fresh `input_order` values 1039 through 1060. All 22 sources
compile under the reviewed `authored_cpp` flags with the function names and
body bytes the notes claim, including `gameMaybeSaveConfig__Fii` at 2216
bytes and `gameSetDefaultGameConfig__Fi` at 1332. All 22 host contracts
passed on this machine. No game binary or disassembly listing was committed.
The notes separate observed addresses from inferred names; the
`0x00484eb8` byte is published as `norrath_game_state_byte_00484eb8` inside
the reset unit and is not an ELF name. `gameLoop__Fv` stays unauthored.
`gameCam` stays the natives-c definition.

| | |
|---|---|
| Before | 858 |
| After | 893 |
| Closed | 22 |
| Opened | 57 |

Closed: `__6Camera`, `coreInterfaceFlare__Fi`, `coreLoading`, `cutMode`,
`exitToMenuMode`, `gameBailFromGameLoop`, `gameCameraTask__Fv`,
`gameClear__Fi`, `gameCombatCamera`, `gameControlPaused`,
`gameDrawWorld__Fv`, `gameInitSkillRamps__Fv`, `gameIsPaused__Fv`,
`gameMaybeSaveConfig__Fii`, `gameMultiPlayerMarkersOn`,
`gameNetReplicateQuestItems`, `gamePause`, `gameResetGameState__Fv`,
`gameReverseCameraYaw`, `gameSetDefaultGameConfig__Fi`,
`jumpGateJumping__Fv`, `mainMenu`.

Opened: `DLG_Clear__Fv`, `Reset__6Cameraii`, `SetMode__6Camerai`,
`Update__6Camera`, `_listCheckAllIntegrity__FPCci`, `bossMeterAlpha`,
`bossMeterCurrent`, `bossMeterMax`, `cdClear__Fv`, `cheatOn`,
`commInit__Fv`, `coreInterfaceBrightness`, `cutClear__Fv`,
`drawAutoMap__Fv`, `drawDome__Fv`, `drawListClear__Fv`, `floorClear__Fv`,
`fxClear__Fv`, `gameAutoMapMode`, `gameCameraShake`, `gameDeathTimer`,
`gameDeathTimerSingle`, `gameHidePlayers`, `gameLineClear__Fv`,
`gameLineDebugDraw__Fv`, `gameShowEnemiesInMap`, `helpMessageOn`,
`hudClearInfoMessage__Fv`, `hudFinishGold__Fv`, `hudNoController`,
`inventoryReset__Fv`, `jumpGateClear__Fv`, `jumping`,
`lastNmLocalPlayersAlive`, `levelSeeds`, `lightClear__Fv`,
`localNmPlayersAlive`, `memCheckHeapReal__FPCci`, `multiHelpMessageOn`,
`objectClear__Fv`, `padClear__Fv`, `playerClearSpellEffect__Fv`,
`playerGlobalInit__Fv`, `removeAllQuestsFromLog__Fv`,
`scriptClearAllGameState__Fv`, `scriptClear__Fv`, `scriptSuperClear__Fv`,
`shopActiveFrame`, `skills`, `textInit__Fv`, `textureClear__Fv`, `world`,
`worldAllocateWaterPatches__Fi`, `worldClear__Fv`,
`worldDrawWorld__FP12_worldHeader`, `worldReset__FP12_worldHeader`,
`worldSetViewCenter__Fv`.

The notes list about a hundred opened names. The rest were already
unresolved or already defined on this tip, including `DLG_Active__Fv`,
`MC_GameConfig`, `MC_SaveConfig__Fv`, `coreLoadingScreenRunning`,
`engineBrightnessCorrection`, `floorDrawEnable`, `gameNoDraw`,
`gameOver`, `netNetworkMode`, `objectDrawEnable`, and `soundMusicVolume`.
839 function bodies, 1059 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/sony-cd

Merged. The notes separate observed addresses from inferred names, and each
selected unit has a host contract that returns failure. No game binary or
disassembly listing was committed. 21 of the 26 units were selected. Five
units were not selected because those symbols are already in the link:
sony-cd-texture-init redefines `textureInitTexture__FP8_texture` from
draw-texture-init, sony-cd-isignal-sema redefines `iSignalSema` and
sony-cd-poll-sema redefines `PollSema` from gs-dma-kernel,
sony-cd-vblank-views redefines the three `norrath_archive_vblank_*` views
from cd-archive, and sony-cd-text-add redefines `table` from
particles-anim-table. Their files stay in the source snapshot. The notes
list those names as closed; they were already defined, so the official
delta is smaller than that list.

20 host contracts passed on this machine. The `ReferThreadStatus` check
compares the assembled body against the pinned executable, which this
machine does not read; the stub assembles to the claimed 16 bytes and the
worker records the match. All 26 sources compile under their reviewed
profiles with the hashes the notes claim. The file-local helpers
`relocate`, `delay_handler`, and `relocate_if_set` are recorded alongside
their functions.

| | |
|---|---|
| Before | 893 |
| After | 884 |
| Closed | 39 |
| Opened | 30 |

Closed: `DelayThread`, `ReferThreadStatus`, `SCE_CD_debug`,
`_sceCd_Read_cur_pos`, `_sceCd_c_cb_sem`, `_sceCd_cd_callback`,
`_sceCd_cd_ncmd`, `_sceCd_cd_read_intr`, `_sceCd_cd_scmd`,
`_sceCd_ee_read_mode`, `_sceCd_ncmd_prechk`, `_sceCd_ncmd_semid`,
`_sceCd_ncmdsdata`, `_sceCd_rd_intr_data`, `_sceCd_scmd_prechk`,
`_sceCd_scmd_semid`, `_sceCd_scmdrdata`, `_sf_bind`, `_sf_data`,
`animInitAnimation__FP15AnimationHeader`, `cdrdsf`, `cmd_sem_init`,
`dum_mode`, `fontInitFont__FP4Font`, `modelInitModel__FPv`, `my_th_info`,
`my_thid`, `ncmd_sema_keep_cmd`, `sceCdNcmdDiskReady`, `sceCdStream`,
`sceSifBindRpc`, `sceSifCallRpc`, `sceSifCheckStatRpc`, `sceSifInitRpc`,
`sceSifWriteBackDCache`, `sfrdata`,
`soundAddSound__FPcP10_vagHeaderb`, `stm_status`,
`worldInitWorld__FP12_worldHeader`.

Also defined, and not previous imports: `_sceCd_ncmdrdata`,
`_sceCd_rcmd_semid`, `_ncmd_bind`, `_scmd_bind`, `scmd_sema_keep_cmd`,
`cb_thid`, `cb_semid`, `sceCdCbfunc`, `sceCdCbfunc_num`,
`sceCdCbfunc_number`, `_sceSifInitCheck`, `_data_table`, `_packet_buffer`,
`_free_buffer`, `_free_buffer2`, `nmSounds`, `effectsBufferSize`,
`soundList`, and `sceCdSyncS`. Not selected, so authored and not in the
link: `textureInitTexture__FP8_texture`, `iSignalSema`, `PollSema`, the
three `norrath_archive_vblank_*` views, `textAddText__FPv`, and `table`;
each name keeps its existing owner.

Opened: `DynamicLightArray`, `ParticleEmitterArray`, `SetTimerAlarm`,
`TimerUSec2BusClock`, `__eq__19ParticleEmitterDataRC19ParticleEmitterData`,
`_request_bind`, `_request_call`, `_request_end`, `_request_rdata`,
`_sceRpcFreePacket`, `_sceRpcGetPacket`, `convertEnd__Fi`,
`earlyInit__3mapP12_worldHeader`, `getStockPitch__FP10_vagHeader`,
`lightDeactivate__FP5Light`, `megatileGenerate__FP12_worldHeader`,
`megatileInit__FP12MegatileRootPv`, `megatile_ramkmax`,
`nmParticleEmitterRecs`, `sceSifAddCmdHandler`, `sceSifGetReg`,
`sceSifGetSreg`, `sceSifInitCmd`, `sceSifSendCmd`, `sceSifSetReg`,
`strcasecmp`, `texmap`, `texmapend`, `texmapstart`,
`transferVAG__FP10_vagHeaderi`. The other names the notes leave open were
already unresolved or already defined, including `cdLoad__FPcPvii`,
`strlen`, `sprintf`, and `_engineCheckStack__FPCci`. 862 function bodies,
1080 authored inputs, zero COMMON. 214 tool tests passed. No EE, CD, SIF, or
game path ran.

## agents/link-image-bounds second pass

Merged. The branch had absorbed merges of other packages; the merge kept
the shared log, plan, and snapshot and took only the new
link-image-bounds files and notes. Five of the six proposed inputs were
selected with fresh `input_order` values 1082 through 1086.
`register_frame_info` was already selected in the first pass and was not
appended again. All five sources compile under `authored_cpp` with the
hashes and body bytes the notes claim (`atexit` 372,
`__deregister_frame_info` 160, the rest data). All five host contracts
passed on this machine. The reference checker needs the private image, so
it did not run here; it lives at `docs/agents/link-image-bounds-check.py`
now, because only `.cpp`, `.hpp`, and `.S` files may sit under
`experimental/sources`. No game binary or disassembly listing was
committed. The rejected `.eh_frame` byte array was never committed. The
notes keep observed addresses separate from the authored names, and the
sema unit adds no alias between `__sce_sema_id` and the malloc view.

| | |
|---|---|
| Before | 884 |
| After | 992 |
| Closed | 6 |
| Opened | 114 |

Closed: `__CTOR_LIST__`, `__DTOR_LIST__`, `__deregister_frame_info`,
`__sce_eh_sema_id`, `__sce_sema_id`, `atexit`. `__register_frame_info`
was already defined. `__wrap_free` and `abort` were already resolved, so
they did not open.

Opened: the exact 114 names listed in the package notes: `__wrap_free`,
`abort`, the 101 `_GLOBAL_$I$*` constructor callbacks, and the 11
`_GLOBAL_$D$*` destructor callbacks. `__register_frame_info` was already
defined and does not change the count. The registrar's `objects` head was
already defined. 864 function bodies, 1085 authored inputs, zero COMMON.
214 tool tests passed. No EE startup stage ran.

## agents/gs-dma-present

This is a partial package: the branch carries 26 units with per-symbol
observed addresses and behavior, but its notes lack the five summary
sections (no closed/opened lists, no plan entries). It is merged because
every committed function has a host test and the tool tests pass. 25 of
the 26 units were selected with fresh `input_order` values 1087 through
1111. gs-dma-present-texture-draw-color was not selected: it redefines
`textureDrawColor__FP8_textureiiiUi` from light-methods, and its file
stays in the source snapshot. All 26 sources compile under their reviewed
profiles. 24 host contracts passed on this machine. The
`AddDmacHandler` check compares the assembled stub against the pinned
executable, which this machine does not read; the stub assembles to the
claimed 16 bytes. No game binary or disassembly listing was committed.

| | |
|---|---|
| Before | 992 |
| After | 1007 |
| Closed | 25 |
| Opened | 40 |

Closed: `AddDmacHandler`, `EnableDmac`, `checkModelVersion`,
`coreLoadingScreenRunning`, `draw`, `drawFrameNm`,
`drawRemoveInterrupts__Fv`, `drawRestoreInterrupts__Fv`,
`drawStart__Fv`, `drawWaitFinished__Fv`, `engineSetVideoMode__Fb`,
`frameStartPacket__FUibi`, `gsIntHandler__Fi`, `lastFrameVBlankCount`,
`makeSamplePacket__Fb`, `powerOffMode`, `sceDmaPause`, `sceDmaPutEnv`,
`sceDmaSend`, `sceGsSyncV`, `specialMpegMode`,
`switchTo30Counter.798`, `switchToHalfCounter.797`,
`textureInternalInit__Fv`, `texturePackDown__Fv`. Also defined, and not
previous imports: `gsHID`.

Opened: `DisableDmac`, `RemoveDmacHandler`, `VSync`, `VSync2`, `atoi`,
`cache`, `cacheClear__Fv`, `cacheHead`, `cacheTail`, `compositeAlpha`,
`copyScreen`, `copyScreenAlpha`, `copyScreenColor`, `copyScreenFrame`,
`decompressStack`, `decompressThreadid`, `dummyTex`, `engineDisable30fps`,
`engineFullFrameMode`, `engineScissory1`, `engineScissory2`,
`engineScreenSlide`, `engineYOffset`, `frameStatePacket`,
`makeUnscrambleTable__Fv`, `sceClose`, `sceDmaCurrentEnv`, `sceOpen`,
`sceRead`, `segmentUsed`, `texDecompressThread__FPv`, `texdecompress`,
`texturePageBlock`, `texturePageLoc`, `texturePageSegLoc`,
`texturePageTable`, `textureUncompBufferLocked`, `trippleBufferMode`,
`uncompMap`, `uncompSpin`. 881 function bodies, 1110 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/actor-view-tables

This is a partial package: the branch carries six data-view commits with
per-symbol observed addresses and behavior, but no summary note, so the
integrator recorded `docs/agents/actor-view-tables.md` from the source
comments. All six units were selected with fresh `input_order` values
1112 through 1117. They are data objects only; no function was added.
Each unit has a host file that returns failure on a wrong size or nonzero
byte; all six passed on this machine (each compiled together with its
implementation). No game binary or disassembly listing was committed. The
existing mentions of these names are `extern` references in the actors-a
constructors, not definitions.

| | |
|---|---|
| Before | 1007 |
| After | 1019 |
| Closed | 6 |
| Opened | 18 |

Closed: `norrath_actor_view_486878`, `norrath_actors_a_view_438e98`,
`norrath_actors_a_view_449430`, `norrath_actors_a_view_44ccb8`,
`norrath_actors_a_view_44d0a8`, `norrath_actors_a_view_485358`.

Opened: the 18 vtable slot targets the three 144-byte tables point at:
`_$_8FireBomb`, `_$_9Container`, `_$_9WorldPart`, `__tf8FireBomb`,
`__tf9Container`, `__tf9WorldPart`, `clone__8FireBomb`,
`clone__9Container`, `clone__9WorldPart`,
`msg_collision__9ContainerP10GameObject`, `msg_draw__9Container`,
`msg_getPokeName__9ContainerPUsi`,
`msg_hurt__9ContainerfP10GameObjecti`, `msg_load__9ContainerPc`,
`msg_poke__9Containeri`, `msg_run__8FireBomb`, `msg_run__9Container`,
`msg_save__9ContainerPc`. 881 function bodies, 1116 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/card-progress

Merged. The notes separate observed bytes from inferred names, and the
host tests fail on wrong behavior. No game binary or disassembly listing
was committed. All 29 units were selected with fresh `input_order`
values 1118 through 1146. The three functions compile to exactly the
claimed bodies (`newMode__Fii` 36, `stopCardQuery__Fv` 164, `sceMcSync`
228). All 40 host tests in `host-tests/card-progress/` passed on this
machine. `flareAlpha`/`flareDelta` are the card words the notes claim;
the same-named locals elsewhere are different objects and stay undefined.
`frontEndWasRunning` and every `MC_`/`MCA_` function stay imports.

| | |
|---|---|
| Before | 1019 |
| After | 999 |
| Closed | 29 |
| Opened | 9 |

Closed: `_3map$autoMapBitsSize`, `actCount`, `actLevelCount`, `allActs`,
`asyncMode`, `bareSlotFootPrint`, `browserState`, `configState`,
`curAct`, `disableMC`, `errorCondition`, `flareAlpha`, `flareDelta`,
`iconHeader`, `keepPlayerPositions`, `killPlayer`, `lastCardPort`,
`lastCardSlot`, `newMode__Fii`, `oldAsyncMode`, `pauseStackPos`,
`saveCheckPointCount`, `sceMcSync`, `slotClusters`, `slotFootPrint`,
`sortedTimes`, `stopCardQuery__Fv`, `totalClusters`, `totalFootPrint`.

Opened: `MCA_FileOpen`, `MCA_Reset__Fv`, `act1`, `act2`, `act3`, `act4`,
`act5`, `mcRunCmdNo`, `syncState`. The other names the notes leave open
were already unresolved, including `MCA_Cmd__Fiiii`, `MCA_Sync__FPi`,
`semaidRegFunc`, `semaid_timerfunc`, `retval`, `PollSema`, `WaitSema`,
and `SignalSema`. 884 function bodies, 1145 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

## agents/ctors-ant-lantern second pass

Merged. Two constructor units were added with fresh `input_order` values
1147 and 1148. Both compile under `authored_cpp` with the bodies the
notes claim (`norrath_construct_AutoContainerLP` 260,
`norrath_construct_Boat` 396). The shared `detail.hpp` gained
declarations; the already-selected `auto_chest_lp` still compiles to its
recorded 260-byte body, and its plan and snapshot header hashes were
refreshed. The host contracts require 32-bit pointers and do not compile
on this arm64 machine, as recorded for the first pass; the EE compilation
passed the header's 32-bit assert. No game binary or disassembly listing
was committed.

| | |
|---|---|
| Before | 999 |
| After | 999 |
| Closed | 2 |
| Opened | 2 |

Closed: `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`.
Opened: `norrath_ctors_ant_lantern_view_44e940`,
`norrath_ctors_ant_lantern_view_453328`. The parent constructors
`norrath_construct_AutoChest` and `norrath_construct_AutoContainer`
stay unresolved for later units. 886 function bodies, 1147 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/game-loop

This is a partial package: the branch carries thirteen units with
per-symbol observed addresses and behavior, but no summary note, so the
integrator recorded `docs/agents/game-loop.md` from the source comments.
Eleven of the thirteen units were selected with fresh `input_order`
values 1149 through 1159. game-loop-draw-particles redefines
`drawParticlesTask__Fv` and game-loop-object-draw-enable redefines
`objectDrawEnable`, both from the particles package; their files stay in
the source snapshot. All thirteen sources compile under `authored_cpp`,
and all thirteen host checks passed on this machine. No game binary or
disassembly listing was committed.

| | |
|---|---|
| Before | 999 |
| After | 1005 |
| Closed | 11 |
| Opened | 17 |

Closed: `frontEndStartInNetMenu__Fv`, `gameCheckPlayerAlive__Fv`,
`gameNetKick__Fi`, `gameNoDraw`, `gameOver`, `gamePlayerLightAttenuate`,
`gameRecallInhibit`, `gameSetCameraShake__Ffb`, `hudNoDraw`,
`runObjectsTask__Fv`, `transformSceneLightTask__Fv`. Also defined, and
not previous imports: the two `runObjectsTask` file-local helpers.

Opened: `Kick__7Vehicle`, `MC_Kick__Fv`, `blockGetNmFree__Fv`,
`debugClearStartMenu__Fv`, `deleteList`, `hudKick__Fv`,
`invShutDown__Fi`, `isNetGame`, `lastNmBlocksFree`,
`matTransform34Vec__FP6Point3P8Matrix34T0`, `mediumRunList`,
`menuNetClientServer`, `padBigRumble__Fiff`, `runList`, `shopKick__Fv`,
`slowRunList`, `worldViewCenter`. 894 function bodies, 1158 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

## agents/memcard-calls

This is a partial package: the branch carries twenty-five units with
per-symbol observed addresses and behavior, but no summary note, so the
integrator recorded `docs/agents/memcard-calls.md` from the source
comments. All twenty-five units were selected with fresh `input_order`
values 1160 through 1184. Twenty-four host contracts passed on this
machine with default flags; the `MCA_Init` contract passes at `-O0` (at
`-O1` this host compiler expands the constant-size `memset` inline and
bypasses the call-counting wrapper, a host-toolchain artifact rather
than a reconstruction defect). No game binary or disassembly listing
was committed. The five `MCA_Init` queue words were local in the image
and are global here so `MCA_Cmd` can share them; `MCA_LastDirCount`
stays a separate object from that local word.

| | |
|---|---|
| Before | 1005 |
| After | 1011 |
| Closed | 25 |
| Opened | 31 |

Closed: `MCA_ClearInfo__Fv`, `MCA_Cmd__Fiiii`, `MCA_Init__Fv`,
`MCA_LastDirCount`, `MCA_Port`, `MCA_Slot`, `MCA_Sync__FPi`,
`MC_Busy__Fv`, `MC_CheckPointAge`, `MC_Debug`, `MC_GameConfig`,
`MC_GetAsyncMode__Fv`, `MC_GetReadyForMainMenu__Fv`, `MC_KickProtect`,
`MC_LastModeSuccessful`, `MC_LoadGameBrowser__Fii`, `MC_NewGame__Fi`,
`MC_SaveCheckPoint__Fiii`, `MC_SaveConfigSilent__Fii`,
`MC_SaveConfig__Fv`, `MC_SaveGameBrowser__Fi`, `MC_SavingOrLoading`,
`mcClientID`, `mcHeartBeatRun__Fv`, `mcHeartBeat__Fv`. Also defined,
and not previous imports: the `MCA_Init` queue words (`clearBytes`,
`queueEnd`, `queueStart`, `checkResults`, `lastDirCount`), the
`mcHeartBeatRun` file-local words, and the `MCA_Init` 16384-byte
`nullBuff`.

Opened: `MCA_Formatted`, `MCA_Present`, `MCA_Process__FPi`,
`MC_HasThisLevelBeenSaved__Fv`, `MC_ResetRamSlot__Fv`,
`MC_SetNewAct__Fi`, `MC_SpawnPoint`, `actionCoolOff`,
`asyncChangePlayers__Fv`, `asyncConfigure__Fv`, `asyncLoadGame__Fv`,
`asyncSaveConfigSilent__Fv`, `asyncSaveConfig__Fv`,
`asyncSaveGame__Fv`, `commandQueue`, `curPlaySeconds`, `delayCount`,
`fullLaunchOK`, `gameGetFullLevelName__FPc`, `getLongLevelName__FPc`,
`interfaceOn`, `netBlankController__Fii`, `pushPause__Fv`,
`ranBrowser`, `ranConfig`, `resetQuickLoad__Fv`, `saveConfigEndGame`,
`strncmp`, `strstr`, `updateRamSlot__Fii`, `whichPad`. 909 function
bodies, 1183 authored inputs, zero COMMON. 214 tool tests passed. No EE
or game execution.

## agents/natives-b

Merged. The middle third of the retail `amx*` natives: all 40 units
were selected with fresh `input_order` values 1185 through 1224. Each
unit compiles under `authored_cpp` with the hash the notes claim, and
each has a host test that returns failure; all 40 passed on this
machine. No game binary or disassembly listing was committed. The notes
separate observed retail names and extents from inferred call shapes.
The two file-local helpers in the random-item unit are recorded
alongside it.

| | |
|---|---|
| Before | 1011 |
| After | 1002 |
| Closed | 40 |
| Opened | 31 |

Closed: the 40 `amx*__FP5__amxPi` natives named in
`docs/agents/natives-b.md` (`amxGetSaveInhibit` through
`amxPutNPCInVehicle`).

Opened: `AddNPCDriver__7VehicleP3NPC`, `DLG_NewWorld`,
`SetHold__7Vehiclei`,
`__11LightEffectG6Point3ffffffffffffP10GameObjecti`,
`__19ParticleMorphEffectP8CreatureT1`,
`__19SpellHornOmenEffectiP8CreatureP5Skill`,
`__22SpellPlanarSteelEffectiP8CreatureP5Skill`,
`__8ItemPropG6Point3PQ24item11descriptioni`,
`addItemToInventory__FiPQ24item11descriptioni`,
`addSpell__8CreatureP5Spell`, `blockAlloc__Fi`,
`createRandom__4itemPQ24item11descriptioniiii`,
`create__4itemPcPQ24item11descriptionii`, `currentRunningObject`,
`doNewWorld__FPc`, `equipItem__FiPQ24item11descriptioni`,
`gameControlDontResetPause`, `gameDeathStart__Fi`,
`gameItemTreasureRamp__Fv`, `gameLevelLoading`,
`getWeight__4itemPQ24item11description`, `givePlayerExperience__Fii`,
`hudFadeScreen__Fi`, `hudResetFadeScreen__Fi`,
`objectFindObjectByClass__FPcPP10GameObjecti`,
`objectRemoveFromGrid__FP10GameObject`, `playerGetFeatRank__Fix`,
`playerPlaySound__Fii`, `playerRecomputeWeight__Fi`,
`playerSelectionRunning__Fv`, `worldDifficulty`. The other names the
notes leave open were already unresolved or already defined, including
`gameBailFromGameLoop`, `gameControlPaused`, `gameHidePlayers`,
`gameRecallInhibit`, `gameNetKick__Fi`, `gameDifficulty`, `levelName`,
`netNetworkMode`, `nmPlayers`, the player/route views, `strcpy`,
`strncmp`, `strstr`, `amx_GetAddr`, `amx_GetString`,
`lumpFindResource__FPcT0`, and `lumpLoad__FPc`. 951 function bodies,
1223 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

## Needs a decision

These two disagreements are still unresolved. Neither package was changed to
force them together.

`norrath_actor_add_slow_run_list` was already selected. It walks 60 heads at
`norrath_actor_view_50bfd0` and the same cycle word. `object-infra` adds
`norrath_object_add_to_slow_run_list` for observed `objectAddToSlowRunList` and
defines `norrath_object_slow_run_list_view` for observed `slowRunList` at
`0x4fbfd0`. Its notes say `norrath_actor_view_50bfd0` is not that list. The
two functions are different symbols, so the link has no duplicate definition.
They do not share a head array. Decide whether those are two lists or one
list under two names.

`norrath_player_malloc_semaphore_view` and `__sce_sema_id` remain two objects.
The memory notes say the original word is one symbol and this package defines
only the view. `__sce_sema_id` is still unresolved. Decide whether the view
should become that symbol.

`norrath_player_view_484ca8` is defined for the original
`gotPlayerStartThisLevel` byte. `norrath_got_player_start_this_level_bytes`
stays unresolved. The items notes say those two names are the same byte and
this package does not alias them. Decide whether they should be one symbol.

`levelName` and `norrath_player_view_34b752` are separate objects. The host
test fails if those pointers are equal. The notes say the view sits inside
the 322-byte `levelName` image. Decide whether the tail should be a second
symbol.

`norrath_archive_animation_extension_view` is the cd-archive extension table.
save-net-anim also defines that name as four bytes, `anm`. That unit was not
selected. Decide which object owns the name.

`norrath_mc_pre_start_key_bytes` and `norrath_mc_start_key_bytes` are the
script-amx keys. save-net-anim defines the same two names again. Those units
were not selected. Decide whether a second definition is wanted.

`vblankCount`, `vblankField`, `vblankHID`, `vblankSemaphore`, and
`vblankSemaphore2` are now defined by gs-dma-kernel. cd-archive still defines
`norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, and
`norrath_archive_vblank_semaphore_view` as separate objects. Decide whether
each pair is one symbol.

`engineCurrentLevelName` is now the 80 zero bytes from subsystem-inits.
world-state still defines `norrath_loader_current_name_view` for that
original name and does not alias it. Decide whether they are one symbol.

particles now defines `objectGrid`, `ghostObjectGrid`, `stringHash`,
`objectHash`, `offTheGrid`, `objectStrings`, and `freeNodes`. object-infra
still defines view names for those objects and does not alias the original
names. Decide whether each pair is one symbol.

`norrath_restore_broadcast_mask_word` is still unresolved. world-state defines
that address once, as `norrath_grid_ignore_flags_view`, and does not emit a
second object. The plan rejects symbol aliases. Decide whether the mask name
should be the view.

`norrath_loader_special_flag_view` is defined for original `noNewVags`.
cd-archive now also publishes `noNewVags` as an alias of
`norrath_archive_no_new_vags_view`. The loader view is still a second object.
Decide whether that view should be the same symbol.

`norrath_archive_engine_pop_border` and `norrath_archive_engine_push_border`
are the selected cd-archive border returns. The original names
`enginePopBorderColor__Fv` and `enginePushBorderColor__FUi` are aliases of
those two. render-lights defines the norrath names again. Those units were
not selected.

items-player already defines 35 `norrath_player_view_*` names. data-views
defines the same 35 names again, including `norrath_player_view_484ca8`.
Those units were not selected. Decide which package owns each name.

draw-texture-init defines the global `dmaGif`, `dmaVif0`, and `dmaVif1`.
`drawInit__Fv` keeps a separate private triple and does not publish it.
engine-init left which object a final link binds unsettled. This link binds
the draw-texture globals. Decide whether the private triple should stay
private.

`norrath_route_world_word_bytes` is the world-state object for original
`world` at `0x004844a0`. vehicle-actors now defines
`norrath_actors_b_view_4844a0` as a separate object and does not alias the
world-state word. Decide whether they are one symbol.

`enableMallocWrapper` is now an alias of the selected
`norrath_enable_malloc_wrapper_byte_view`. `memAlloc__FiPc`, `memFree__FPv`,
and `blockAllocInit__Fv` were authored as aliases of `norrath_mem_alloc`,
`norrath_mem_free`, and `norrath_block_alloc_init` on sources inside the
frozen prefix. Those lines were not kept. The three original names stay
unresolved. Decide whether a later unit outside that prefix should publish
them.

`soundInit__Ff` and `soundRestore__Fb` were authored as aliases of
`norrath_sound_initialize` and `norrath_sound_restore` on sources inside the
frozen prefix. Those lines were not kept. Both original names stay
unresolved. Decide whether a later unit outside that prefix should publish
them.

`norrath_sound_command_dma_destination_word_view` is a separate 4-byte
object for byte 8 of the command table already published as
`norrath_sound_command_state_view`. A store through one does not update the
other. Decide whether the DMA word should become a view inside the command
state object.

`cutMode` is now the 1-byte ELF name from game-state. world-state still
defines `norrath_game_cut_mode_byte_view` and data-views still defines
`norrath_actor_view_484318` for the same address as separate objects.
Decide whether the three should be one symbol.

`coreLoading` is now the 4-byte ELF name from game-state. world-state still
defines `norrath_game_core_loading_word_view` for the same address as a
separate object. Decide whether they are one symbol.

`gameBailFromGameLoop` is now the 4-byte ELF name from game-state.
scheduler still defines `norrath_game_bail_from_loop_view` for the same
address as a separate object. Decide whether they are one symbol.

`table` at `0x006d8fa0` is defined once, by particles-anim-table as
32768 bytes. sony-cd also defines that name as `0x2000` words for the same
span. That unit was not selected. Decide which package owns the name.

## Disagreements

The symbol disagreements are the items under Needs a decision. No selected
symbol was defined twice.

## Still authored, never executed

Every function and data object added by math, audio, memory, object-infra,
megatile, script-amx, cd-archive, libc-format, items-player, save-net-anim,
engine-init, world-state, render-lights, data-views, actors-b, gameobject-base,
scheduler, game-boot, subsystem-inits, gs-dma-kernel, draw-texture-init, and
frame-loop, dialog-hud-controls, natives-a, natives-c, floor, engine-restore, sif-keyboard, actors-a, the cd-archive second pass, the memory second pass, the audio second pass, the script-amx second pass, vslots-b, light-methods, vehicle-actors, particles, vslots-a, link-image-bounds, ctors-ant-lantern, sound-rpc-views, game-state,
sony-cd, and link-image-bounds second pass, and gs-dma-present. The
vehicle constructor is authored and not in the link. The unselected
texture-draw-color unit is authored and not in the link. The six
actor-view-tables views are data, not functions. The 26 card-progress
data words are data, not functions. The shared constructor header now
covers Boat and AutoContainerLP. The eleven selected game-loop units
(eight functions, five words) are in the link; the two unselected
game-loop units are authored and not in the link. The twenty-five
memcard-calls units are in the link. The forty natives-b units are in the link. `engineInit__Ff`,
`_engineCheckStack__FPCci`, and `norrath_route_add_node` are selected and have
not run. The held AMX draft remains uncompiled. The three unselected
save-net-anim view units, the two unselected render-lights border units,
and the 35 unselected data-views player views are authored and not in the link.
Host checks ran on this machine. The reviewed EE compiler produced relocatable
objects and the partial link succeeded. No routine from these packages has
run on the EE, under the original process, or as gameplay. The same is true
of the startup bodies already on the shared branch: they are selected source,
and no startup stage has executed.

## Push log

- 2026-10-11 16:23 CDT — `agents/megatile` — 626 unresolved imports
- 2026-10-11 16:26 CDT — `agents/script-amx` — 752 unresolved imports
- 2026-10-11 16:29 CDT — `agents/cd-archive` — 763 unresolved imports
- 2026-10-11 16:31 CDT — `agents/libc-format` — 740 unresolved imports
- 2026-10-11 16:34 CDT — `agents/items-player` — 692 unresolved imports
- 2026-10-11 16:37 CDT — `agents/save-net-anim` — 738 unresolved imports
- 2026-10-11 16:39 CDT — `agents/engine-init` — 789 unresolved imports
- 2026-10-11 16:44 CDT — `agents/world-state` — 756 unresolved imports
- 2026-10-11 16:47 CDT — `agents/render-lights` — 750 unresolved imports
- 2026-10-11 16:50 CDT — `agents/data-views` — 782 unresolved imports
- 2026-10-11 16:52 CDT — `agents/actors-b` — 796 unresolved imports
- 2026-10-11 16:56 CDT — `agents/gameobject-base` — 781 unresolved imports
- 2026-10-11 16:59 CDT — `agents/libc-format` — 779 unresolved imports
- 2026-10-11 17:08 CDT — `agents/scheduler` — 775 unresolved imports
- 2026-10-11 17:10 CDT — `agents/game-boot` — 870 unresolved imports
- 2026-10-11 17:11 CDT — `agents/subsystem-inits` — 907 unresolved imports
- 2026-10-11 17:13 CDT — `agents/gs-dma-kernel` — 893 unresolved imports
- 2026-10-11 17:15 CDT — `agents/draw-texture-init` — 884 unresolved imports
- 2026-10-11 17:16 CDT — `agents/frame-loop` — 888 unresolved imports
- 2026-10-11 17:17 CDT — `agents/dialog-hud-controls` — 836 unresolved imports
- 2026-10-11 17:19 CDT — `agents/natives-a` — 829 unresolved imports
- 2026-10-11 17:20 CDT — `agents/natives-c` — 826 unresolved imports
- 2026-10-11 17:23 CDT — `agents/floor` — 819 unresolved imports
- 2026-10-11 17:25 CDT — `agents/engine-restore` — 856 unresolved imports
- 2026-10-11 17:26 CDT — `agents/sif-keyboard` — 865 unresolved imports
- 2026-10-11 17:28 CDT — `agents/actors-a` — 882 unresolved imports
- 2026-10-11 17:31 CDT — `agents/cd-archive` — 886 unresolved imports
- 2026-10-11 17:38 CDT — `agents/memory` — 879 unresolved imports
- 2026-10-11 17:41 CDT — `agents/audio` — 879 unresolved imports
- 2026-10-11 17:42 CDT — `agents/script-amx` — 879 unresolved imports
- 2026-10-11 17:44 CDT — `agents/vslots-b` — 871 unresolved imports
- 2026-10-11 17:46 CDT — `agents/light-methods` — 866 unresolved imports
- 2026-10-11 17:47 CDT — `agents/vslots-b` — 866 unresolved imports
- 2026-10-11 17:49 CDT — `agents/vehicle-actors` — 950 unresolved imports
- 2026-10-11 17:51 CDT — `agents/vslots-b` — 947 unresolved imports
- 2026-10-11 17:52 CDT — `agents/vslots-b` — 947 unresolved imports
- 2026-10-11 17:54 CDT — `agents/particles` — 929 unresolved imports
- 2026-10-11 17:56 CDT — `agents/vslots-a` — 889 unresolved imports
- 2026-10-11 17:57 CDT — `agents/link-image-bounds` — 888 unresolved imports
- 2026-10-11 17:59 CDT — `agents/ctors-ant-lantern` — 889 unresolved imports
- 2026-10-11 18:08 CDT — `agents/sound-rpc-views` — 858 unresolved imports
- 2026-10-11 19:12 CDT — `agents/game-state` — 893 unresolved imports
- 2026-10-11 19:20 CDT — `agents/sony-cd` — 884 unresolved imports
- 2026-10-11 19:27 CDT — `agents/link-image-bounds` — 992 unresolved imports
- 2026-10-11 19:34 CDT — `agents/gs-dma-present` — 1007 unresolved imports
- 2026-10-11 19:40 CDT — `agents/actor-view-tables` — 1019 unresolved imports
- 2026-10-11 19:46 CDT — `agents/card-progress` — 999 unresolved imports
- 2026-10-11 19:52 CDT — `agents/ctors-ant-lantern` — 999 unresolved imports
- 2026-10-11 19:58 CDT — `agents/game-loop` — 1005 unresolved imports
- 2026-10-11 20:05 CDT — `agents/memcard-calls` — 1011 unresolved imports