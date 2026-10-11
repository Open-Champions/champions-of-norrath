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

`vblankCount`, `vblankField`, and `vblankSemaphore2` are now imports from
`engineInit`. cd-archive defines `norrath_archive_vblank_count_view`,
`norrath_archive_vblank_field_view`, and
`norrath_archive_vblank_semaphore_view` as separate objects. Decide whether
each pair is one symbol.

`engineCurrentLevelName` is still an import from `engineInit`. world-state
defines `norrath_loader_current_name_view` for that original name and does
not alias it. Decide whether they are one symbol.

`norrath_restore_broadcast_mask_word` is still unresolved. world-state defines
that address once, as `norrath_grid_ignore_flags_view`, and does not emit a
second object. The plan rejects symbol aliases. Decide whether the mask name
should be the view.

`norrath_loader_special_flag_view` is defined for original `noNewVags`.
cd-archive already defines `norrath_archive_no_new_vags_view`. Decide whether
those two views are one symbol.

`norrath_archive_engine_pop_border` and `norrath_archive_engine_push_border`
are the cd-archive border returns. render-lights defines the same two names
again. Those units were not selected. Decide which package owns the names.

items-player already defines 35 `norrath_player_view_*` names. data-views
defines the same 35 names again, including `norrath_player_view_484ca8`.
Those units were not selected. Decide which package owns each name.

`norrath_route_world_word_bytes` is the world-state object for original
`world` at `0x004844a0`. actors-b leaves that address unresolved as
`norrath_actors_b_view_4844a0` and does not alias it. Decide whether they
are one symbol.

## Disagreements

The symbol disagreements are the items under Needs a decision. No selected
symbol was defined twice.

## Still authored, never executed

Every function and data object added by math, audio, memory, object-infra,
megatile, script-amx, cd-archive, libc-format, items-player, save-net-anim,
engine-init, world-state, render-lights, data-views, actors-b, and gameobject-base. `engineInit__Ff`,
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
