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

## agents/pre-logo-dialog

This is a partial package: the branch carries fifteen units with
per-symbol observed addresses and behavior, but its notes leave the
closed/opened lists pending, so the measured link delta below stands in
for them. All fifteen units were selected with fresh `input_order`
values 1225 through 1239. Each unit compiles under `authored_cpp`, and
each host contract passed on this machine. No game binary or
disassembly listing was committed.

| | |
|---|---|
| Before | 1002 |
| After | 1035 |
| Closed | 18 |
| Opened | 51 |

Closed: `DLG_FireUpShopOnExit`, `DLG_ForceMutexRelease__Fv`,
`DLG_StartDialog__FPcP9DlgPacketii`, `DLG_StartWithSwoopDone`,
`DLG_SwoopMode`, `animSetSpecialSoundHandler__FPFiG6Point3_v`,
`dialogMsgHandle`, `gameFootstepSound__FG6Point3iii`,
`gameobjInit__Fv`, `helpMessageFrame__Fv`,
`hudAddHelpMessage__FiPUsT1`, `hudMsgCallback__FPUci`, `hudMsgHandle`,
`hudSetInfoMessage__FPUsiic`, `hudShowBust__FPcPUsii`,
`hudShowCompass__FG6Point3PUs`, `lightHaloTexture`,
`netSendMessage__FiPUci`. Also defined, and not a previous import: the
`hudMsgCallback` file-local helper.

Opened: `GetPitch__6Camera`, `GetYaw__6Camera`, `body`,
`colorScale__FUif`, `controlPaused`, `createByName__FPcG6Point3sPPc`,
`debugRecall__Fi`, `dialogMutex`, `drawBustCur`, `drawBustMax`,
`drawBustOn`, `drawBustTexName`, `drawBustTitle`, `drawCompassOn`,
`drawCompassPoint`, `drawCompassTitle`, `fontColor__FUiii`,
`fontEnd__Fv`, `fontPrintCenteredu__FiiPUsi`,
`fontPrintWrapu__FiiPUsiiii`, `fontPrint__FiiPci`,
`fontPrintu__FiiPUsi`, `fontStart__FP4FontiUii`,
`fontStringSizeu__FP4FontPUsi`, `hudAssignQuickSpell__Fiii`,
`hudAssignSpellBookSpell__Fiiil`, `infoMessage`, `infoMessageAlways`,
`infoMessageTimer`, `msgHandler`, `multiBody`, `multiRelease`,
`multiSide`, `multiTitle`, `netBlankControllerButtons__Fiii`,
`netDisconnectPlayer__Fi`, `netLocalPad`,
`neverSaveAlwaysLoadFromLump__FPc`,
`objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`,
`objectSetFilterFunction__FPFPc_i`, `playerDamageHandler__6Player`,
`rectangleDraw__FiiiiiUi`, `saveActivate__9Savepoint`,
`scriptActivatePostAction__Fv`, `soundPlaySequence__FP15AnimationHeaderG6Point3f`,
`specialHandler`, `textureDrawBorder__FiiiiiUiPcii`, `title`,
`ustrcat__FPUsT0`, `ustrcpy__FPUsPc`, `ustrcpy__FPUsT0`. 964 function
bodies, 1238 authored inputs, zero COMMON. 214 tool tests passed. No EE
or game execution.

## agents/simloop-services

This is a partial package: the branch carries eleven units with
per-symbol observed addresses and behavior, but no summary note, so the
integrator recorded `docs/agents/simloop-services.md` from the source
comments. All eleven units were selected with fresh `input_order`
values 1240 through 1250. Eight host contracts passed on this machine.
The `cdProcess`, `soundMapSound`, and `soundResetStream` contracts
assume 32-bit pointers in their harnesses (pointer round-trip through a
32-bit word, 4-byte pointer slots, and a `memset` size that only holds
with 32-bit struct layout) and fail on this 64-bit machine for that
reason, the same documented limitation as the ctors-ant-lantern
contracts; their reviewed EE compilations passed. No game binary or
disassembly listing was committed.

| | |
|---|---|
| Before | 1035 |
| After | 1054 |
| Closed | 11 |
| Opened | 30 |

Closed: `cdDirectoryLookup__FPcPiT1i`, `cdProcess__Fv`,
`padInitPadIndexMap__Fv`, `padProcess__Fv`,
`soundGetStreamPlayStatus__Fi`, `soundMakeSequenceUnpausable__Fi`,
`soundMapSound__FPci`, `soundResetStream__Fi`,
`soundSetStreamVolume__Fif`, `soundStartDialogStream__FiPci`,
`soundStartMusic__FPci`. Also defined, and not a previous import: the
`cdDirectoryLookup` file-local helper `find_row`.

Opened: `currentLayerDir`, `gCd`, `initLump__FPv`, `layer0Dir`,
`layer1Dir`, `lumps`, `memRealloc__FPvi`, `nmLumps`, `padFrame.756`,
`padGetUnprocessedData__FiP18padUnprocessedData`, `padIndexMap`,
`padProcessData__FP3padP18padUnprocessedDataP12padStateData`, `sbuff`,
`sbuffin`, `sceCdGetError`, `sceCdLayerSearchFile`, `sceCdStRead`,
`sceCdSync`, `scePadGetSlotMax`, `sequence`,
`soundFindDialog__FPc`, `soundGetDialogFilename__Fv`,
`soundGetFreeRam__Fv`, `soundStartStream__FiPciiiiii`,
`soundUpdateAdjustments__Fv`, `streamOutput`, `streamSizeLeft`,
`streamVolume`, `streaming`, `tapCount`. 976 function bodies, 1249
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

## agents/gs-dma-present closing pass

Merged. The branch notes now carry the five summary sections: per-symbol
observed addresses with separate inferred/unknown notes, closed and opened
import lists, build-plan entries, an authored-never-executed statement, and
a verification log. Each of the three new units has a host contract that
fails (assert aborts) on a wrong word, order, or call; all three passed on
this machine with ASan/UBSan. No game binary or disassembly listing was
committed. The three sources compile under `authored_cpp` with the hashes
and body bytes the notes claim.

Three of the 29 proposed inputs were selected with fresh `input_order`
values 1251 through 1253. The 25 inherited units were already selected and
were not appended again. gs-dma-present-texture-draw-color is still not
selected: it redefines `textureDrawColor__FP8_textureiiiUi` from
light-methods, and its file stays in the source snapshot. Object names
follow the in-plan convention (`do_super_sample.o`, `light_vu0_stopped.o`,
`reload_dma.o`) rather than the notes' `present_` proposals; there was no
collision either way.

One correction against the notes: their plan entry for do-super-sample
listed only `doSuperSample__Fiib`, but the fresh EE object also emits the
file-local helper `_ZL17float_to_int_wordf` (260 bytes), so both are
recorded, matching 38 existing mangled-helper precedents. The driver
compares the full function multiset, so the helper had to be listed.
`madePacket.769` and the 21 `reloadDMA__Fi` state words are defined and
were not previous imports. The notes flag that `gsIntHandler__Fi`
declares `reloadDMA__Fi` as `void` while the observed body returns 1 and
the new unit declares it `int`; return type is not part of linkage, and
the partial link succeeded.

| | |
|---|---|
| Before | 1054 |
| After | 1056 |
| Closed | 3 |
| Opened | 5 |

Closed: `doSuperSample__Fiib`, `lightVU0StoppedProcessing__Fv`,
`reloadDMA__Fi`.

Opened: `globalCharLightScale`, `textureCacheIn__FP8_texture`,
`textureLock__FP8_texture`, `textureUnlock__FP8_texture`,
`worldCharViewMatrix`. All five are in the notes' opened list; the rest of
that list was already unresolved or already defined on this tip. 980
function bodies, 1252 authored inputs, zero COMMON. 214 tool tests passed.
No EE, GS, DMA, VU, or game execution.

## agents/pre-logo-dialog second pass

Merged. The branch notes now carry the five summary sections: per-symbol
observed addresses with separate inferred/unknown notes, closed and opened
import lists, build-plan entries, an authored-never-executed statement, and
a private-trial verification log. Four new units were selected with fresh
`input_order` values 1254 through 1257. The 15 first-pass units were already
selected and were not appended again. All four sources compile under
`authored_cpp` with the hashes and body bytes the notes claim
(`dialogMsgCallback__FPUci` 3192, `pauseMenuFrameRun__Fv` 4888,
`runDialog__Fv` 2100, `drawDialog__Fv` 1632; verified with `nm
--print-size` on the reviewed EE objects). All four host contracts passed
on this machine with ASan/UBSan. No game binary or disassembly listing was
committed.

The existing `extern` references in game-boot (`runDialog__Fv`,
`drawDialog__Fv`, `pauseMenuFrameRun__Fv`) and save-net-anim
(`dialogMsgCallback__FPUci`) are declarations, not definitions, so the
new definitions do not multiply any symbol. `dialog_msg.o` also defines
the file-local BSS `_ZL9dialogTag`; it is data, not a function, so it is
not in the emitted-function list. `player` and `c.1089` stay unresolved
by design (see Unknown in the package notes).

The worker's private trial reported 22 closed and 157 opened, measured on
that branch's older 845-input tree where the 15 first-pass units were not
yet selected. That absolute count does not reproduce on the shared tip.
The closed and opened names do: every measured name is in the notes'
lists.

| | |
|---|---|
| Before | 1056 |
| After | 1149 |
| Closed | 4 |
| Opened | 97 |

Closed: `dialogMsgCallback__FPUci`, `drawDialog__Fv`,
`pauseMenuFrameRun__Fv`, `runDialog__Fv`.

Opened: the 97 names measured from the link diff, all in the notes'
opened list, including `AdvanceDialogSwoop__6CameraiPf`,
`BuildMatrix__6CamerafffP6Point3`, `MC_ChangePlayers__FiPUsiii`,
`Push__6Camera`, `SetPitch__6Camerai`, `SetYaw__6Camerai`,
`StartDialogSwoop__6CameraP10GameObjecti`, `__tf3NPC`,
`__throw_bad_typeid`, `anim`, `animLoadingComplete`, `animStarted`,
`cancelStreamWait`, `checkBlock`, `cleanUp__Fv`,
`colorLevelNames__FP4Menu`, `coreInterfaceFadeUpVal`,
`coreInterfaceFadeUp__Fi`, `coreStartSwitchScene__Fi`, `correction`,
`cursorTween`, `dialogCreature`, `dialogIdleAnimation`, `dlgArrayPtr`,
`dlgName`, `dlgPos`, `endTimer.1059`, `evalHermite__Ffffff`,
`flaredIn`, `flaredOut`, `fontLastPrintedPosition`,
`fontPrintCentered__FiiPci`, `fontRestrict__Fi`, `fontScissor__Fiiii`,
`frontendInitControllerConfig__Fi`,
`frontendRunControllerConfig__Fv`, `gameCheckNoSaveZone__Fv`,
`gamePopConfig__Fv`, `gamePushConfig__Fv`, `hudFooterUni`,
`hudNetLevelWarp__FPc`, `hudRunYesNo__FPUsiiii`,
`hudShowMsg__FPUsN20iii`, `initAnimation__Fv`, `kickMenu`,
`kickMenuList`, `lastNmQ.1088`, `lastWarpPos`, `letterAtATime`,
`lineToY__Fi`, `loadSampleRate.784`,
`menuDraw__FiiP4MenuUiUiP4FontP8_texturei`,
`menuRun__FP4MenuP3padPi`, `msgHudPlayerExit__Fv`,
`msgHudRecall__Fc`, `nameChars`, `netKickPlayer__Fi`,
`netPlayerIsConnected__Fi`, `netRestoreButtonConfigs__Fv`,
`nextIndex`, `objectFindAnimation__FP10GameObjectPc`, `optTbl`,
`optionCount`, `optionIndex`, `optionLinePos`, `optionsShowing`,
`preConfigure`, `realDlgPos`, `resetTextBody__Fv`, `runAnimation__Fv`,
`sceneTransitionOK.1041`, `scrollEnd`, `scrollSpan`, `scrollTarget`,
`scrollY`, `sentEndMsg`, `setExtraParams__FP8CreaturePc`,
`showCoords__Fv`, `showMore`, `showOptions`,
`sleepMonsters__8creaturev`, `soundContinueStream__Fi`,
`soundGetDialogSize__FPc`, `soundGetNmQueued__Fi`,
`soundGetStreamAmountPlayed__Fi`, `soundGetStreamPreloadDone__Fi`,
`soundSetMusicDuckAmount__Ff`, `soundStartGlobalMusic__FPci`,
`startPacket__Fi`, `swooping`, `textBody`, `textBodyChars`,
`textPageDone`, `usprintf__FPUsT0ffffffff`, `ustrlen__FPUs`,
`waitForSound`, `wakeMonsters__8creaturev`. The rest of the notes'
opened list was already unresolved or already defined on this tip,
including `GetPitch__6Camera`, `GetYaw__6Camera`, `DLG_NewWorld`,
`SetMode__6Camerai`, `Update__6Camera`, `fontStart__FP4FontiUii`,
and `soundGetStreamPlayStatus__Fi`. 984 function bodies, 1256 authored inputs, zero COMMON. 214 tool tests passed. The build-output
objects are byte-identical to the recorded `object_sha256` values. No
EE or game execution.

## agents/iop-module-boot

Merged. The branch notes carry the five summary sections: per-symbol
observed addresses with separate inferred/unknown notes, closed and opened
import lists, build-plan entries, an authored-never-executed statement, and
host checks. The branch was still receiving commits when the integrator
started; it was merged once its notes commit landed. Fifteen units, one
function per commit, each with a host contract beside the source. All
fifteen contracts passed on this machine with ASan/UBSan, and a mutated
`sceSifSyncIop` (second notify value changed) made its contract exit
non-zero, so the contracts can fail. The fifteen sources compile under
`authored_cpp` with the body bytes the notes claim. No game binary or
disassembly listing was committed; the `host_contract.cpp` files are in the
snapshot `files` map and not in the plan.

Fifteen units were selected with fresh `input_order` values 1258 through
1272. The worker's private count reproduces exactly on the shared tip.

| | |
|---|---|
| Before | 1149 |
| After | 1152 |
| Closed | 15 |
| Opened | 18 |

Closed: `Free__10VIKeyboard`, `Init__10VIKeyboard`, `PreparePowerOff__Fv`,
`_lf_bind`, `_lf_version`, `commSystemInit__Fv`, `sceCdInit`, `sceCdMmode`,
`sceFsReset`, `sceSifInitIopHeap`, `sceSifLoadFileReset`,
`sceSifLoadModule`, `sceSifRebootIop`, `sceSifSyncIop`,
`textSystemInit__Fv`.

Opened: `PowerOffCB`, `PowerOffHandler__FPv`, `PowerOffThread__FPv`,
`VIStartKeyboardDriver__Fv`, `_sceCd_scmdsdata`, `_sceSifLoadModule`,
`cdvd_exit`, `lgAudInit`, `lgCodecEnumerate`, `lgCodecInit`, `lgCodecOpen`,
`lgKbdClose`, `memcmp`, `sceCdPOffCallback`, `sceResetttyinit`,
`sceSifExecNotifyHandler`, `sceSifExitRpc`, `sceSifResetIop`. 999 function
bodies, 1271 authored inputs, zero COMMON. 214 tool tests passed. Object
hashes in the snapshot come from the fresh build and were identical on a
second build. No EE or game execution.

The notes ask for three edits to merged `sif-keyboard` sources (use the
exported `norrath_iop_heap_client`, `norrath_loadfile_client`, `_senddata` and
`vik` instead of private statics). They change game code in another package,
so the integrator did not make them; see Needs a decision.

## agents/floor-world-view

Merged, with three of the fourteen units left out of the plan. The notes
carry the five summary sections, though the package never ran
`build_partial.py` or the tool tests, so its closed and opened lists were
not measured; the measured link delta below replaces them. All fourteen
sources compile under `authored_cpp`. Thirteen host checks passed on this
machine with ASan/UBSan. `floor-world-view-program-views` passes without
the sanitizers and exits 2 under them, because its non-MIPS branch declares
two separate arrays and only asserts that the host linker places them 816
bytes apart. The real property holds in the EE object: `nm -S` of the fresh
object puts `norrath_floor_program_end_view` at 0x330 and the start at 0.
Mutating `worldGetDoorHeight`'s record offset made its check exit non-zero.
No game binary or disassembly listing was committed.

Left out (files stay in the source snapshot, with no plan entry):

- `floor-world-view-floor-add` defines `floorAdd__FG6Point3sfii` as an
  eight-byte forward to `norrath_floor_add`. The original is 956 bytes and the
  notes list "whether `floorAdd` in the original differs from
  `norrath_floor_add`" as unknown. Selecting it would close the import with a
  placeholder, which is a symbol alias in all but name; the build driver
  applies no aliases. Left unresolved until someone shows the two are one
  function.
- `floor-world-view-camera-pos` and `floor-world-view-far-clip` define
  `worldCameraPos` and `farClip`, which `world_camera_pos.o` (input 957) and
  `far_clip.o` (input 924) already define. Selecting them made the
  relocatable link fail with multiple definitions. The notes' closed list
  was taken from an older base and is stale on those two names.

Eleven units were selected with fresh `input_order` values 1273 through
1283.

| | |
|---|---|
| Before | 1152 |
| After | 1142 |
| Closed | 12 |
| Opened | 2 |

Closed: `__11ResourceMapPcT1`,
`drawAddToList__FPviP8_textureP11_drawRecordi`,
`norrath_floor_program_end_view`, `norrath_floor_program_start_view`,
`norrath_world_check_for_water`, `norrath_world_find_closest_door`,
`norrath_world_get_door_height`, `norrath_world_set_door_collision`,
`worldClipCubeToFrustum__FPA1_fi`, `worldSetFarClip__Ff`,
`worldSetFrustum__Fffff`, `worldWorldViewMatrix`.

Opened: `tanf`, `worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi`.
1008 function bodies, 1282 authored inputs, zero COMMON. 214 tool tests
passed. Object hashes in the snapshot come from the fresh build and were
identical on a second build. No EE or game execution.

## agents/card-progress and agents/memcard-calls follow-up commits

Merged. Both packages were already selected; these merges add no inputs.
`agents/card-progress` gained one commit that makes the host-test support
module read its EE toolchain from `EE_TOOLCHAIN` (its fallback is another
contributor's absolute path; set `EE_TOOLCHAIN` on this machine). That file is
a host test, not a plan input. `agents/memcard-calls` gained its own package
notes (checks, closed and opened lists, build-plan entries) plus a trim of an
unsupported unknown. The integrator's earlier `docs/agents/memcard-calls.md`
collided with it on merge; both are kept, the worker's text first and the
integrator's selection notes under their own heading at the end.

| | |
|---|---|
| Before | 1142 |
| After | 1142 |
| Closed | 0 |
| Opened | 0 |

1008 function bodies, 1282 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## Baseline re-check before the 2026-10-12 afternoon pass

The shared branch (`ca8050c`) was re-run before merging anything: 214 tool
tests, the snapshot check (2951 files, 1282 authored inputs) and the default
partial build (1282 compilations, one link, 1008 function bodies, zero COMMON)
all passed, and the build reproduced the recorded **1142** unresolved imports.
The generated `private_object_sha256` of the last plan entry also reproduced.
No repo-rule break was found in the added files: no binaries, no disassembly
files, no committed object files. The `.S` files are authored syscall
sequences. One shared-file defect: the 22:19 push-log line was missing its
trailing newline, so the next entry was fused onto it. Fixed below.

## agents/game-loop closing pass

Merged (the branch notes carry the five summary sections). Two functions were
new since the first merge: `drawObjectsTask__Fv` and `gameLoop__Fv`, each with
a host check that exits zero here. Only `gameLoop__Fv` was selected, with
fresh `input_order` 1282. game-loop-draw-objects was **not** selected: the link
fails with a multiple definition of `drawObjectsTask__Fv`, already defined by
particles-draw-objects-task (input 922). Its files stay in the source snapshot
beside game-loop-draw-particles and game-loop-object-draw-enable, which are
still unselected for the same reason. Which body should own the name is a
decision for the authors; the branch notes still say 15 imports closed, which
no longer holds on this tip.

The worker's private trial (879 to 963) is not the official count; this tip
shows a different one.

| | |
|---|---|
| Before | 1142 |
| After | 1199 |
| Closed | 1 |
| Opened | 58 |

Closed: `gameLoop__Fv`.

Opened: `MC_FrontEndImport`, `animFrame__Fv`, `commProcess__Fv`,
`coreGameStep__Fii`, `coreHeadsetResetTimer`, `coreLevelCommentTimer`,
`coreLoadingStart`, `coreSwitchScene`, `coreSwitchSceneStart`,
`cutRunFrame__Fb`, `engineGetAIStepCount__Fv`, `engineLastAITime`,
`engineLoadEnvTex__FP8_texture`, `engineSetBlur__Fi`,
`enumerateAlivePlayers__Fv`, `gameCreatureThinkSpinner`,
`gameMiscTaskSync__Fv`, `gamePlayersPresent`, `gameSetObjectVis__Fv`,
`inventoryInit__Fv`, `loadThreadID`, `loadingScreenRequestExit`,
`lumpCheck__Fv`, `lumpFindByType__FPvPci`, `lumpQuery__FPc`,
`mcProcessKick__Fv`, `musicTimer`, `netBroadCastStart`,
`netClearContact__Fv`, `netGameStep__FbT0`, `netInContact__Fv`, `netInit__Fv`,
`netStartLevel__Fv`, `netStartNetworkGameFrame__Fv`,
`netStartNetworkGameInit__Fv`, `netStart__Fv`, `netTrouble`,
`netWaitForPeersToCatchUp__Fv`, `picProcess__4itemv`,
`playerDeallocateArmorCaches__Fv`, `playerRecalled`, `plotRouteTask__Fv`,
`preCacheArmor__6Player`, `recallEnter`, `runMusic__Fv`, `scriptRun__Fv`,
`scriptSetScript__FPv`, `startNewWorld__Fv`, `subDrawLoop__FPFv_i`,
`superGamestateInit`, `texIsDecompressing__Fv`, `texIsLoading__Fv`,
`texProcessDecompress__Fv`, `texProcessLoad__Fi`, `textMessageDraw__Fv`,
`textMessageProcess__Fv`, `worldDrawEnable`, `worldRunWaterTask__Fv`.
1009 function bodies, 1283 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

## agents/vi-launcher

Merged (the branch notes carry the five summary sections: per-symbol observed
addresses, inferred and unknown notes, closed and opened imports, build-plan
entries, and an authored-never-executed statement). Twenty-two units, one
function per commit, each with a `host_contract.cpp`; all 22 compile and run
here with exit 0. Fresh `input_order` values 1283 through 1304. All 22
sources compile under `authored_cpp` with empty header closure; 22 function
bodies, 2884 body bytes. No game binary or disassembly listing was committed.
Host contracts are in the snapshot `files` map and not in the plan.
1031 function bodies, 1305 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1199 |
| After | 1208 |
| Closed | 22 |
| Opened | 31 |

Closed: `CheckNetwork__15VINetworkStatus`, `ClearVRAM__FUcUcUcUc`,
`Close__7VITimer`, `Free__10VILauncher`, `GetDefault__10VILanguage`,
`Init__10VILauncherb`, `Init__7VITimer`, `Initialize__15VINetworkStatus`,
`LoadExecPS2`, `SetDefault__10VILanguage12VILanguageID`,
`Setup__10VILauncher`, `Station__10VILauncheriPCcN22i`,
`VIDisplayLoadingScreen__Fb`, `VIMemCardSetProductId__FPCc`,
`VIMemCardSetProductTitle__FPCc`, `_$_10VILauncher`, `_$_15VINetworkStatus`,
`__10VILauncher`, `__15VINetworkStatus`, `exitEzNetCnf`, `exitEzNetCtl`,
`ezNetCtlDownInterface`.

Opened: `DeleteThread`, `Handler__7VITimeri`, `Password__C13VIGameOptions`,
`RotateThreadReadyQueue`, `SetArg`, `TerminateLibrary`,
`Username__C13VIGameOptions`, `VIDisplayScreen16__FPCcib`,
`VIGetGameOptions__Fv`, `_LoadExecPS2`, `__builtin_delete`,
`__wrap_memalign`, `dmaGIF`, `initEzNetCtl`, `rand`, `sceCdReadClock`,
`sceGifPkAddGsData`, `sceGifPkInit`, `sceGifPkReset`, `sceGifPkSize`,
`sceGifPkTerminate`, `sceGszbufaddr`, `sceInetCtlGetState`,
`sceInetGetInterfaceList`, `sceLibnetInitialize`, `sceMcEnd`,
`sceMcGetInfo`, `scePadEnd`, `setup_main__Fv`, `srand`,
`station_main__FiPCcPcT2T1T1i`.

## agents/session-roster

Merged (the branch notes carry the five summary sections; its unit count was
corrected in the last commit). Nine functions plus four data units, each with
a `host_check.cpp` that passes here (13 of 13 exit 0). Twelve of the thirteen
units were selected with fresh `input_order` values 1305 through 1316.
session-roster-player-state was **not** selected: the link fails with multiple
definitions of `invisible`, `nmLines` and `nmSingle`, already defined by
other packages (inputs 933, 937 and 938). Its other six words
(`playersAlive`, `startEntranceName`, `syncRandState`, `activePlayer`,
`nmPlayersAlive`, `noMonsters`) therefore stay undefined, and its files stay in
the source snapshot. This is why the notes' closed list (26) is larger than the
18 names actually closed here; `activePlayer` and `invisible` appear in the
notes' list but `activePlayer` is still not closed. The 12 selected sources
compile under `authored_cpp`, 1904 body bytes. No game binary or disassembly
listing was committed. 1040 function bodies, 1317 authored inputs, zero COMMON.
214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1208 |
| After | 1201 |
| Closed | 18 |
| Opened | 11 |

Closed: `broadcastConnection`, `checkForMonsters__FP10GameObject`, `commHandle`,
`commHandler__FPUci`, `commStatusHandle`, `commStatusHandler__FPUci`,
`g_net_buf`, `msgHandler`, `netNmPartners`,
`netRegisterMessageHandler__FPFPUci_v`,
`netRegisterPacketHandler__FPFPUci_v`, `netWaitCallBack__FPUci`,
`netWaitMsgHandler`, `nmPartners`, `partners`,
`playerInit__FiiPcScScScScT2`, `playerSelectionSetup__Fv`,
`playerSelectionStart__Fv`.

Opened: `abilityTable`, `fadeIconY`, `fadeTimer`, `firstTime`, `init__Fv`,
`language`, `lightPushSceneLights__FG6Point3iiiiiiiii`,
`playerRecomputeAbilityModifiers__Fv`, `playerSelectionTask__Fv`,
`recievePacket__FiPUc`, `setupDefaultEquipment__Fi`.

## agents/simloop-services closing pass

Merged (the branch notes carry the five summary sections; the first merge in
this log had only a partial package). Twelve units were new since the first
merge; eight were selected with fresh `input_order` values 1317 through 1324
(11 functions including the three file-local helpers `bind_client`,
`point_at`, `append_command`; 5764 body bytes). Four were **not** selected:
`pad-dma-buffer`, `pad-mtap-state`, `sound-music-state` and
`sound-stream-records` are data-only units with no host check at all, and the
snapshot checker requires host files for every selected package record, so
they stay in the source snapshot, unselected, until each has a check that can
fail. Skipping them is why `scePadInit` and the multitap init now leave the
`norrath_libpad_*` and `norrath_libmtap_*` view words open (the 11 view names
below). Host checks: 15 of 19 exit 0 here. `cdProcess`, `soundMapSound`
(accepted at the first merge for the same reason), and the new
`soundPlaySequence` and `soundComputeSequenceLength` checks segfault on this
64-bit machine because the harnesses assume 32-bit addressable pointers; a
32-bit-pointer run (arm64 pagezero shrink, x86_64 under Rosetta) did not work
here either, so those two new functions are only compiled, not host-tested,
on this machine. The worker's private trial counts are not the official ones.
1051 function bodies, 1325 authored inputs, zero COMMON. 214 tool tests passed.
No EE or game execution.

| | |
|---|---|
| Before | 1201 |
| After | 1227 |
| Closed | 8 |
| Opened | 34 |

Closed: `sceMtapInit`, `sceMtapPortOpen`, `scePadInit`, `scePadPortOpen`,
`soundComputeSequenceLength__FP15AnimationHeader`, `soundFrame__Fv`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`soundPlaySound__FP10_vagHeaderP6Point3fff`.

Opened: `DPRINT`, `batchBuffer`, `delaySoundList`, `disableSoundPlaying`,
`findChannel__Fv`, `listOnewayLink__FP8ListHeadT0Pv`, `nmBatchCommands`,
`norrath_libmtap_change_priority_client_view`,
`norrath_libmtap_get_connection_client_view`,
`norrath_libmtap_module_version_client_view`,
`norrath_libmtap_port_close_client_view`,
`norrath_libmtap_port_open_client_view`, `norrath_libmtap_rpc_buffer_view`,
`norrath_libpad_dma_info_view`, `norrath_libpad_is_init_word`,
`norrath_libpad_is_warning_word`, `norrath_libpad_port_info_view`,
`norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view`,
`pan__FG6Point3ffPfT3`, `pauseProcess__Fv`, `runAmbient__Fv`,
`runMusicDucking__Fv`, `runScreaming__Fv`, `runSequences__Fv`,
`sceMtapGetModVersion`, `scePadGetModVersion`, `scePadInit2`, `sceSdRemote`,
`sequenceInProgress`, `sequenceVolume`, `soundChangeVolume__Fiff`,
`soundFlushCommandBuffer__Fv`, `soundFrameNm`.
## Fixed: agents/frontend-shell contract could not fail

This section was first written as a rejection of `agents/frontend-shell` at
tip `940b753`. Another integrator merged and selected the package a few
minutes later (the "agents/frontend-shell" entry above), so it is on the
shared branch with the defect below unfixed. The finding stands.

`experimental/sources/frontend-shell-run/host_contract.cpp` clears the shared
`error` word at the top of `reset()`, and `main` calls `reset()` before each of
its 45 scenarios. A failing `expect` in any scenario but the last is erased by
the next `reset()`. Mutating `run_front_end.cpp` confirmed it: changing the
title y coordinate from 36 to 37, the skipped-title menu number from 50 to 51,
and the title x base from 320 to 321 all left the contract exiting 0, though
checks 7 and 10 assert those values. With the `error = 0` line deleted from
`reset()` the unmodified source passes all 45 outcomes, so the checks
themselves look sound; the harness hides their failures. Until it is fixed,
`runFrontEnd__Fv` is a function with no test that could fail in the sense of
CONTRIBUTING and the repo rules. The words unit's `host_check.cpp` is sound.

The contract was not edited here (it belongs to the worker's package). The
one-line fix is to stop clearing `error` in `reset()`; the contract already
prints and returns the first failing code. That the package was merged anyway
is also a reason to read the worker's host contracts before trusting the
"every committed function has a test" test: the vi-launcher contracts were
checked by mutation and do catch changes; this one does not. The notes also
record that `runFrontEnd` binds the original's `menu` and `menuNm` locals to
the same-named globals of `dialog-hud-controls`; that aliasing still needs a
decision.

Fixed by the integrator after the parallel merge: the `error = 0;` line was
removed from `reset()` in `frontend-shell-run/host_contract.cpp` (test harness
only; no game source touched) and its snapshot hash updated. Re-checked: the
unmodified source still exits 0, and a copy of `run_front_end.cpp` with the
title x base changed from 320 to 321 now exits 7 (check 7). The `menu` and
`menuNm` aliasing against dialog-hud-controls still needs an owner decision.
The shared branch stands at 1208 unresolved imports with that fix, 214 tool
tests and the snapshot check passing.

## agents/kernel-patches

Merged (the branch notes carry the five summary sections). Forty-three units
with fresh `input_order` values 1325 through 1367: sixteen are authored
assembly (eleven syscall wrappers, `kFindAddress`, and four `kCopy`
pattern bodies), eight are C++ functions with a `host_contract.cpp`
(`iGetTimerSystemTime`, `_InitAlarm`, `PatchIsNeeded`, `topThread`,
`SetNextComp`, `cbTimerHandler`, `UnlinkAlarm`, `InsertAlarm`), and nineteen
are data words with a Python test under `host-tests/kernel-patches`. The eight
host contracts passed here and the 19 data tests pass (19 of 19). The sixteen
assembly units are checked by the byte comparer in
`gs-dma-kernel-body-check`, which needs the private executable; that
comparison was **not** re-run by the integrator (integration does not read
the executable), so the byte-match statements in the notes are the worker's
and are recorded in the snapshot only as a host file reference, as for
gs-dma-kernel. One shared-test fix: `host-tests/kernel-patches/support.py`
hard-coded another contributor's absolute toolchain path, so all 19 data
tests errored here; it now reads `EE_TOOLCHAIN` with the same default the
card-progress tests use (the same fix as the earlier card-progress commit).
No game code was edited. The worker's claim (1149 to 1120 on an older tip) is
not the official delta. 1075 function bodies, 1368 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1227 |
| After | 1197 |
| Closed | 36 |
| Opened | 6 |

Closed: `PatchIsNeeded`, `SetNextComp`, `SignalSema`, `SysExecPS2Entry`,
`_InitAlarm`, `_SyscallEntry`, `_kExecArg`, `cbTimerHandler`,
`g_CounterBuf`, `g_SysRebootBuf`, `g_Timer`, `g_iUserRebootSize`,
`g_pUserRebootBuf`, `iGetTimerSystemTime`, `kFindAddress`,
`kernel_alarm_copy`, `kernel_alarm_get_entry`, `kernel_alarm_setup`,
`kernel_alarm_sysentry`, `kernel_execps2_copy`, `kernel_execps2_get_entry`,
`kernel_execps2_setup`, `kernel_syscall_pattern_kcopy`,
`kernel_syscall_sysentry`, `kernel_syscall_table_setup`, `kernel_tlb_copy`,
`kernel_tlb_get_entry`, `kernel_tlb_setup`, `kernel_tlb_sysentry`,
`semaidRegFunc`, `semaid_timerfunc`, `sifParamOrd`, `topArg`, `topId`,
`topSema`, `topThread`.

Opened: `GetOsdConfigParam`, `SetOsdConfigParam`, `SuspendThread`, `WakeupThread`,
`_kExitTLBHandler`, `kprintf`.

## agents/frontend-shell

Merged as a **partial package**: the notes carry the observed, inferred and
unknown separation, the build-plan table and the import lists, but not the
five separate summary sections (no standalone "Authored but never executed"
or "Unknown" headings; unknowns are inline). Every committed function has a
test: `runFrontEnd__Fv` has `host_contract.cpp` and the data words have
`host_check.cpp`; both pass here (2 of 2) and 214 tool tests pass. Three units
with fresh `input_order` values 1368 through 1370; one function (7060 body
bytes, not an original-byte match; the original is 5656 bytes). No game binary
or disassembly listing was committed. The notes say `menu` and `menuNm` are
also defined by dialog-hud-controls; the link has no duplicate definition
(they are externs here), so nothing was dropped. The worker's private
trial (879 to 928) is not the official delta. 1076 function bodies, 1371
authored inputs, zero COMMON. No EE or game execution.

| | |
|---|---|
| Before | 1197 |
| After | 1217 |
| Closed | 14 |
| Opened | 34 |

Closed: `attemptLoadGame`, `autodemoTimer`, `creditMenu`, `creditsPosY`,
`engineHoldMpeg`, `expWarnAlpha`, `frontEndRunning`,
`frontEndStartInCredits`, `frontEndSuspend`, `frontEndWasRunning`,
`frontMenu`, `frontendControllerConfigRunning`, `lineShift`,
`runFrontEnd__Fv`.

Opened: `commMenu`, `coreClearNetLogFuck`, `creditsText`, `curFocus`,
`difficultyMenu`, `endInit`, `endTimer`, `engineReboot__Fv`,
`frontEndCleanUp__Fi`, `gameTypeMenu`, `hudRunNoControllers__Fi`,
`launchPlayerSelect__Fv`, `menuModeRebootQuestion`,
`menuNetImportOrNewGame`, `menuNetImportOrNewPlayer`, `musicStart`,
`netAttemptLaunch`, `netDoNewLaunch__Fv`, `optionMenu`, `paramRunning__Fv`,
`playerMenu`, `playerSelectCount`, `playerSelectEnforce`,
`playerSelectExitMessage`, `playerSelectNextName`, `playerSelectPrevName`,
`playerSelectStartPlayer`, `playerSelectSuccessful`, `setSRSInfo__Fv`,
`soundClearStreamQue__Fi`, `startNetLaunch__Fv`, `statusPtr`,
`stopCredits__Fv`, `textureSmoothBorder__FiiiiiUiiii`.

## agents/loop-grid-fx

Merged. The notes carry the five summary sections: per-symbol addresses with
separate observed, inferred and unknown parts, closed and opened lists,
build-plan entries, an authored-never-executed statement and unknowns. The
branch had been quiet for about half an hour. Twenty-one units, each with a
`host_check.cpp` beside the source; the checks that read pointer words build
with `-DNORRATH_HOST_CONTRACT`, which the EE build never defines. All 21
passed on this machine with ASan/UBSan. A scripted mutation pass (one
numeric literal changed at a time, up to ten per source) was killed by the
contract in every unit that has literals (for example 10/10 `P_AddParticle`,
10/10 `P_Clear`, 9/10 `norrath_light_add`, 8/10 `objectFindObject`, 3/10
`norrath_object_remove_string`), so the checks can fail. No game binary or
disassembly listing was committed.

Twenty-one units were selected with fresh `input_order` values 1373 through
1393; the five data units emit no functions. Their link has no duplicate
definition. The notes' "duplicates" section is about name pairs, not link
collisions, and stays open: `objectUpdateInGrid__FP10GameObject` and
`norrath_object_update_in_grid`, `objectFindInBox__FG6Point3T0PP10GameObjecti`
and `norrath_object_find_in_box`, and `norrath_object_add_to_medium_run_list`
and `norrath_actor_add_medium_run_list` are each the same routine under two
names in two packages. All are selected; callers still bind to one name.

The worker's private trial (950 to 940 on an older tip) is not the official count. The
closed list reproduces exactly.

| | |
|---|---|
| Before | 1217 |
| After | 1204 |
| Closed | 20 |
| Opened | 7 |

Closed: `P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_Clear__Fv`,
`P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `P_Update__Fv`,
`findTagString__10GameObjectPci`, `fxDust`,
`norrath_factory_particle_emitter_tag_bytes`,
`norrath_has_light_or_particle_number`, `norrath_light_add`,
`norrath_list_head_copy_construct`, `norrath_object_add_to_delete_list`,
`norrath_object_add_to_medium_run_list`, `norrath_object_remove_string`,
`objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectFindObject__FPc`,
`objectFindObject__FPcPP10GameObjecti`, `objectIterateGrid__FPFP10GameObject_v`,
`objectUpdateInGrid__FP10GameObject`, `scriptParticles`, `taskList`.

Opened: `P_DeleteParticleSystem__Fi`, `P_Pop__Fv`,
`checkCanAllocDescriptor__Fv`, `frameNumWrapped__Fv`,
`setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v`,
`updatePS__FP14ParticleSystem`, `updateTrails__Fv`. 1096 function bodies,
1392 authored inputs, zero COMMON. 214 tool tests passed. Object hashes in
the snapshot come from the fresh build. No EE or game execution.

## agents/combat-reactions

Merged. The notes carry the five summary sections: evidence with observed
addresses and separate inferred and unknown parts, closed and opened lists,
build-plan entries, an authored-never-executed statement and unknowns. The
branch had been quiet for about forty minutes. Fifteen units, each with a
`host_check.cpp`. All 15 passed on this machine with ASan/UBSan. A scripted
mutation pass (one numeric literal changed at a time) was killed by the
checks in every unit that has literals: 9/10 for the Cat, Gnome and Mermaid
`setState` bodies, 7/10 for the Spider, 5/10 `updateEnemy__8Creature`, 3/6
`msg_collision__4Gold`, 1/1 `charm__8Creaturei`. The notes also describe a
comparison against the pinned executable
(`docs/agents/combat-reactions-compare.py`, 1500 seeds per function, zero
mismatches); that script reads the private executable, which integration
does not, so it was not re-run and the result is the worker's. No game binary
or disassembly listing was committed.

Fifteen units were selected with fresh `input_order` values 1394 through
1408; the four data units emit no functions. The seven code units include
`sources/combat-reactions-bytes/bytes.hpp`, recorded in their header closure;
`host_vcall.hpp` in the same directory is a host-only header and is in the
snapshot `files` map only. All 15 source hashes and compiled body bytes
match the notes. The six names the notes say other packages closed first
(`death`, the Creature `msg_alert`, the Cat and SmallSpider collisions,
`setState__7Nataslai`, `turnToFace`) are not defined here and not duplicated.

The worker's count (1142 to 1133) was measured on an older tip; the closed
and opened names reproduce exactly.

| | |
|---|---|
| Before | 1204 |
| After | 1195 |
| Closed | 15 |
| Opened | 6 |

Closed: `charm__8Creaturei`, `msg_alert__10GameObjectifG6Point3P10GameObject`,
`msg_collisionWorld__10GameObjectG6Point3`,
`msg_collision__10GameObjectP10GameObject`,
`msg_collision__4GoldP10GameObject`, `msg_hurt__10GameObjectfP10GameObjecti`,
`norrath_collision_frame_bytes`, `norrath_collision_grid_scale_bytes`,
`norrath_spell_blessed_type_name_view`, `norrath_spell_type_name_view`,
`setState__11SmallSpideri`, `setState__14GnomeNavigatori`, `setState__3Cati`,
`setState__7Mermaidi`, `updateEnemy__8Creature`.

Opened: `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`,
`animGlobalSync__FP14AnimationState`,
`getSmartRandomDestination__8Creaturei`, `hudGivePlayerGold__FiG6Point3i`,
`resetRouting__8Creature`, `setStateProcessing__8CreaturePi`. 1107 function
bodies, 1407 authored inputs, zero COMMON. 214 tool tests passed. Object
hashes in the snapshot come from the fresh build. No EE or game execution.

## agents/actor-view-tables second pass

Merged. The branch notes carry the evidence, closed and opened lists,
build-plan hashes, an authored-never-executed statement and unknowns. The
branch had been quiet for about forty minutes. The integrator's earlier
`docs/agents/actor-view-tables.md` collided with the worker's on merge; both
are kept, the worker's text first and the integrator's selection notes under
their own heading at the end. Nineteen new data views (virtual tables, tag
strings, resource maps and the slow-run-list heads), each with a host file
under `actor-view-tables-views/host/` that returns failure on a wrong size or
nonzero byte; all 25 host files (six from the first pass) pass on this
machine. They are data objects; no function was added, and the checks are
size and zero-fill checks only. No game binary or disassembly listing was
committed.

Nineteen units were selected with fresh `input_order` values 1409 through
1427, component `actor-view-tables-views`. The six first-pass units were
already selected and were not appended again. The virtual tables
each name sized functions by assembler label, so closing the names
opens new function imports; the worker did not measure the link and the
real net count goes up.

The notes settle half of a "Needs a decision" item: `norrath_actor_view_50bfd0`
is the 240-byte `slowRunList` object at `0x4fbfd0` (the import name carries a
sign-extension misread of `lui 0x50 / addiu 0xbfd0`), which contradicts the
`object-infra` note that it is not that list. `object-infra` still defines
`norrath_object_slow_run_list_view` for the same original object, so the link
now has two definitions of one object under two names. They are different
symbols and no duplicate definition arises; which name should survive is still
open.

| | |
|---|---|
| Before | 1195 |
| After | 1221 |
| Closed | 19 |
| Opened | 45 |

Closed: `norrath_actor_view_50bfd0`, `norrath_actors_a_view_449790`,
`norrath_actors_a_view_4498b0`, `norrath_actors_a_view_4499d8`,
`norrath_actors_a_view_44a930`, `norrath_actors_a_view_44a9c0`,
`norrath_actors_a_view_44c830`, `norrath_actors_a_view_44f1f8`,
`norrath_actors_a_view_44f2a0`, `norrath_actors_a_view_470830`,
`norrath_actors_a_view_471310`, `norrath_actors_a_view_477758`,
`norrath_actors_a_view_481980`, `norrath_actors_a_view_483e5c`,
`norrath_actors_a_view_485360`, `norrath_actors_a_view_485400`,
`norrath_actors_a_view_485408`, `norrath_actors_a_view_4860b8`,
`norrath_actors_a_view_4860c0`.

Opened (all function labels from the virtual tables): `_$_10DoorSecret`,
`_$_11FloorSwitch`, `_$_6Candle`, `_$_7AntTrap`, `_$_7Blocker`,
`_$_7Candle2`, `_$_7EggSack`, `_$_9DoorSwing`, `__tf10DoorSecret`,
`__tf11FloorSwitch`, `__tf6Candle`, `__tf7AntTrap`, `__tf7Blocker`,
`__tf7Candle2`, `__tf7EggSack`, `__tf9DoorSwing`, `clone__10DoorSecret`,
`clone__11FloorSwitch`, `clone__6Candle`, `clone__7Blocker`,
`clone__7Candle2`, `clone__9DoorSwing`, `msg_draw__10AutoPropLP`,
`msg_draw__6Candle`, `msg_draw__7Candle2`, `msg_getPokeName__10DoorSecretPUsi`,
`msg_getPokeName__9DoorSwingPUsi`, `msg_load__10DoorSecretPc`,
`msg_load__11FloorSwitchPc`, `msg_load__9DoorSwingPc`,
`msg_poke__10DoorSecreti`, `msg_poke__9DoorSwingi`, `msg_presave__9DoorSwing`,
`msg_run__10AutoPropLP`, `msg_run__10DoorSecret`, `msg_run__11FloorSwitch`,
`msg_run__6Candle`, `msg_run__7AntTrap`, `msg_run__7Candle2`,
`msg_run__9DoorSwing`, `msg_save__10DoorSecretPc`, `msg_save__11FloorSwitchPc`,
`msg_save__9DoorSwingPc`, `msg_trigger__10DoorSecreti`,
`msg_trigger__9DoorSwingi`. 1107 function bodies, 1426 authored inputs, zero
COMMON. 214 tool tests passed. Object hashes in the snapshot come from the
fresh build. No EE or game execution.

## agents/level-anim-load

Merged, with four of the 35 units left out of the plan. The notes carry the
five summary sections: functions and data with original addresses and sizes,
separate observed, inferred and unknown parts, closed and opened lists,
build-plan entries, an authored-never-executed statement and unknowns. The
branch had been quiet for about forty minutes. Twelve function units have a
`host_contract.cpp` and the storage units have a `storage_contract.cpp`. All
31 selected contracts passed on this machine with ASan/UBSan
(`-DNORRATH_HOST_ARENA` maps target words into an arena on the host only). A
scripted mutation pass was killed by the contracts in every function unit
that has numeric literals (10/10 `animAddTransitionTo`, `animSetToEnd`,
`animResetFramestates` and `routeUnrollVnode`; 9/10 `animAdd` and
`cutStartScene`; 8/10 `monsterSpawn` and `lumpFind`). No game binary or
disassembly listing was committed. The 12 compiled body sizes match the notes.

Left out (files stay in the source snapshot, with no plan entry):

- `level-anim-load-loader-failure-word` defines `norrath_loader_failure_word_view`
  as a global absolute symbol with value 0 through a basic `asm` block, so
  the loader's store to address 0 resolves. PORTABLE_BUILD says the link
  imports no absolute game addresses and applies no aliases; an absolute
  symbol in the partial object breaks that statement, the unit has no host
  contract, and the notes themselves ask for a decision. The import stays
  open until the owner decides.
- `level-anim-load-anim-results`, `level-anim-load-frame-states` and
  `level-anim-load-scene-state` define `animResults`, `frameStates` and
  `scene`, which `anim_results.o` (input 915), `frame_states.o` (input 925)
  and `scene.o` (input 951) already define. Selecting them made the
  relocatable link fail with multiple definitions; the notes' closed list is
  stale on those three names.

Thirty-one units were selected with fresh `input_order` values 1428
through 1458 (the 12 function units and 19 storage units). The host-only headers in `level-anim-load-common`
and the files of the four left-out units are in the snapshot `files` map.

Two same-address pairs from the notes stay open as two separate words:
`norrath_factory_world_pointer_word` (the word `world`, already defined as
`norrath_route_world_word_bytes` by world-state) and
`norrath_factory_world_flags_word` (`levelFlags`, already defined by
natives-c). They are different link symbols, so the link has no duplicate
definition but the two copies of each word do not agree. `cutMode` stays an
import under its original name.

The worker's private count (950 to 937 on an older tip) is not the official
one. Eighteen of the 22 closed names in the notes reproduce; the others are
the four left out above.

| | |
|---|---|
| Before | 1221 |
| After | 1208 |
| Closed | 18 |
| Opened | 5 |

Closed: `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`,
`animAdd__FP14AnimationStateP15AnimationHeaderi`,
`animInitState__FP14AnimationState`, `animResetFramestates__FP14AnimationState`,
`animSetToEnd__FP14AnimationState`, `cd`, `cutMsgHandler`,
`cutMsgHandlerFunction__FPUci`, `cutStartScene__FP9_cutscenePc`,
`lumpFind__FPvPc`, `norrath_archive_sound_free_ram`,
`norrath_factory_world_flags_word`, `norrath_factory_world_pointer_word`,
`norrath_find_height_orientation_scale_bytes`,
`norrath_forbidden_orientation_scale_bytes`, `norrath_monster_spawn`,
`norrath_route_unroll_vnode`, `norrath_swap_int`.

Opened: `coreStartLoading__Fv`, `cutDrawFrame__Fv`, `cutRunFrameTask__Fv`,
`spawn__FG6Point3iisPcbPPci`, `updateAnimation__FP13animComponent`. 1119
function bodies, 1457 authored inputs, zero COMMON. 214 tool tests passed.
Object hashes in the snapshot come from the fresh build. No EE or game
execution.

## agents/ctors-ant-lantern closing pass

Merged (the branch notes carry the five summary sections). Twenty-nine new
constructors since the first merge (the first merge had three: `AutoChestLP`,
`AutoContainerLP`, `Boat`), selected with fresh `input_order` values 1371
through 1399; all 32 package constructors are now in the plan. Each has a
`host/<name>_contract.cpp`. Built with `-DNORRATH_CTORS_ANT_LANTERN_HOST`
together with its source, all 32 contracts exit 0 on this 64-bit machine
(32 of 32; the earlier note that three contracts need 32-bit pointers does
not apply to these). Compiled bodies total 43,344 bytes for the 29 and are
not original-byte matches. The branch extended `detail.hpp` (declarations
only, no code), so its snapshot and plan hashes were refreshed; the three
earlier constructors still compile to the same function sizes. No game binary
or disassembly listing was committed.

The count rises because each constructor calls many game-object helpers and
reads many original data words that no package defines yet; the opened list
below is dominated by the `norrath_ctors_ant_lantern_view_*` data views. The
notes' "imports opened" list is the worker's and is not the official one.
1148 function bodies, 1486 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1208 |
| After | 1516 |
| Closed | 29 |
| Opened | 337 |

Closed: `norrath_construct_Ant`, `norrath_construct_AntQueen`,
`norrath_construct_Arenabeast`, `norrath_construct_AutoChest`,
`norrath_construct_AutoContainer`, `norrath_construct_AutoProp`,
`norrath_construct_AutoPropLP`, `norrath_construct_Badger`,
`norrath_construct_BlackWidow`, `norrath_construct_Catapult`,
`norrath_construct_Chest`, `norrath_construct_CloudGiant`,
`norrath_construct_Critter`, `norrath_construct_Cthulu`,
`norrath_construct_Cyclops`, `norrath_construct_Demon`,
`norrath_construct_FemaleDarkElf`, `norrath_construct_FireBeetle`,
`norrath_construct_FireFly`, `norrath_construct_Froglock`,
`norrath_construct_Generator`, `norrath_construct_Ghoul`,
`norrath_construct_Gnome`, `norrath_construct_Goblin`,
`norrath_construct_Ice`, `norrath_construct_Innoruuk`,
`norrath_construct_JumpGate`, `norrath_construct_Lamp`,
`norrath_construct_Lantern`.

Opened: `__11AddParticleG6Point3PPci`, `__3Webii`, `addIcon__3mapP6Point3iPciT3N33`,
`dpadd`, `dptofp`, `gameLineInitPathState__FP17gameLinePathStatePcP6Point3`,
`hasLightOrParticleNumber__FPPci`, `initAttackVariant__5Demon`,
`initAttackVariant__8Froglock`, `initType__13FemaleDarkElfb`,
`initType__5Gnome`, `initType__6Goblinb`, `lightActivate__FP5Light`,
`norrath_construct_AutoObject`, `norrath_ctors_ant_lantern_view_34b474`,
`norrath_ctors_ant_lantern_view_34b6b1`,
`norrath_ctors_ant_lantern_view_42ae60`,
`norrath_ctors_ant_lantern_view_42af70`,
`norrath_ctors_ant_lantern_view_42b1d0`,
`norrath_ctors_ant_lantern_view_42b548`,
`norrath_ctors_ant_lantern_view_42b630`,
`norrath_ctors_ant_lantern_view_42b870`,
`norrath_ctors_ant_lantern_view_42b958`,
`norrath_ctors_ant_lantern_view_42ba00`,
`norrath_ctors_ant_lantern_view_42bb08`,
`norrath_ctors_ant_lantern_view_42be08`,
`norrath_ctors_ant_lantern_view_42beb0`,
`norrath_ctors_ant_lantern_view_42bf58`,
`norrath_ctors_ant_lantern_view_42c188`,
`norrath_ctors_ant_lantern_view_42c308`,
`norrath_ctors_ant_lantern_view_42c8a0`,
`norrath_ctors_ant_lantern_view_42cef0`,
`norrath_ctors_ant_lantern_view_42cfc8`,
`norrath_ctors_ant_lantern_view_42d200`,
`norrath_ctors_ant_lantern_view_438a38`,
`norrath_ctors_ant_lantern_view_438b28`,
`norrath_ctors_ant_lantern_view_438eb8`,
`norrath_ctors_ant_lantern_view_4494c0`,
`norrath_ctors_ant_lantern_view_44a810`,
`norrath_ctors_ant_lantern_view_44a8a0`,
`norrath_ctors_ant_lantern_view_44b900`,
`norrath_ctors_ant_lantern_view_44b9f0`,
`norrath_ctors_ant_lantern_view_44bae0`,
`norrath_ctors_ant_lantern_view_44bbd0`,
`norrath_ctors_ant_lantern_view_44bcc0`,
`norrath_ctors_ant_lantern_view_44bdb0`,
`norrath_ctors_ant_lantern_view_44bea0`,
`norrath_ctors_ant_lantern_view_44bf90`,
`norrath_ctors_ant_lantern_view_44c080`,
`norrath_ctors_ant_lantern_view_44c260`,
`norrath_ctors_ant_lantern_view_44c350`,
`norrath_ctors_ant_lantern_view_44c440`,
`norrath_ctors_ant_lantern_view_44c530`,
`norrath_ctors_ant_lantern_view_44cb98`,
`norrath_ctors_ant_lantern_view_44cc28`,
`norrath_ctors_ant_lantern_view_44dbe8`,
`norrath_ctors_ant_lantern_view_44dd68`,
`norrath_ctors_ant_lantern_view_44e550`,
`norrath_ctors_ant_lantern_view_44e9d0`,
`norrath_ctors_ant_lantern_view_44ebb0`,
`norrath_ctors_ant_lantern_view_44ec40`,
`norrath_ctors_ant_lantern_view_44ece8`,
`norrath_ctors_ant_lantern_view_44f6a8`,
`norrath_ctors_ant_lantern_view_450160`,
`norrath_ctors_ant_lantern_view_450530`,
`norrath_ctors_ant_lantern_view_4506b0`,
`norrath_ctors_ant_lantern_view_46d070`,
`norrath_ctors_ant_lantern_view_46d300`,
`norrath_ctors_ant_lantern_view_46dbe0`,
`norrath_ctors_ant_lantern_view_46dc90`,
`norrath_ctors_ant_lantern_view_46dca0`,
`norrath_ctors_ant_lantern_view_46dcb0`,
`norrath_ctors_ant_lantern_view_46dcc0`,
`norrath_ctors_ant_lantern_view_46dcd0`,
`norrath_ctors_ant_lantern_view_46dce0`,
`norrath_ctors_ant_lantern_view_46dcf0`,
`norrath_ctors_ant_lantern_view_46dd00`,
`norrath_ctors_ant_lantern_view_46dd10`,
`norrath_ctors_ant_lantern_view_46dd20`,
`norrath_ctors_ant_lantern_view_46dd30`,
`norrath_ctors_ant_lantern_view_46dd40`,
`norrath_ctors_ant_lantern_view_46dd50`,
`norrath_ctors_ant_lantern_view_46dd60`,
`norrath_ctors_ant_lantern_view_46dd70`,
`norrath_ctors_ant_lantern_view_46ded0`,
`norrath_ctors_ant_lantern_view_46dee0`,
`norrath_ctors_ant_lantern_view_46def0`,
`norrath_ctors_ant_lantern_view_46edb0`,
`norrath_ctors_ant_lantern_view_46f120`,
`norrath_ctors_ant_lantern_view_46f130`,
`norrath_ctors_ant_lantern_view_46f310`,
`norrath_ctors_ant_lantern_view_46f320`,
`norrath_ctors_ant_lantern_view_46f330`,
`norrath_ctors_ant_lantern_view_46fa28`,
`norrath_ctors_ant_lantern_view_46fa38`,
`norrath_ctors_ant_lantern_view_46fa48`,
`norrath_ctors_ant_lantern_view_46fa58`,
`norrath_ctors_ant_lantern_view_46fd38`,
`norrath_ctors_ant_lantern_view_46ffe0`,
`norrath_ctors_ant_lantern_view_470df8`,
`norrath_ctors_ant_lantern_view_470e08`,
`norrath_ctors_ant_lantern_view_470e18`,
`norrath_ctors_ant_lantern_view_470e28`,
`norrath_ctors_ant_lantern_view_470e38`,
`norrath_ctors_ant_lantern_view_470e48`,
`norrath_ctors_ant_lantern_view_470e58`,
`norrath_ctors_ant_lantern_view_470e68`,
`norrath_ctors_ant_lantern_view_470e78`,
`norrath_ctors_ant_lantern_view_470e88`,
`norrath_ctors_ant_lantern_view_470e98`,
`norrath_ctors_ant_lantern_view_470ea8`,
`norrath_ctors_ant_lantern_view_470eb8`,
`norrath_ctors_ant_lantern_view_470ff0`,
`norrath_ctors_ant_lantern_view_471000`,
`norrath_ctors_ant_lantern_view_471010`,
`norrath_ctors_ant_lantern_view_471020`,
`norrath_ctors_ant_lantern_view_471030`,
`norrath_ctors_ant_lantern_view_471150`,
`norrath_ctors_ant_lantern_view_4711f0`,
`norrath_ctors_ant_lantern_view_471208`,
`norrath_ctors_ant_lantern_view_471218`,
`norrath_ctors_ant_lantern_view_471230`,
`norrath_ctors_ant_lantern_view_471248`,
`norrath_ctors_ant_lantern_view_471260`,
`norrath_ctors_ant_lantern_view_471278`,
`norrath_ctors_ant_lantern_view_471ed0`,
`norrath_ctors_ant_lantern_view_471ee0`,
`norrath_ctors_ant_lantern_view_471ef0`,
`norrath_ctors_ant_lantern_view_471f00`,
`norrath_ctors_ant_lantern_view_471f10`,
`norrath_ctors_ant_lantern_view_471f20`,
`norrath_ctors_ant_lantern_view_476ac8`,
`norrath_ctors_ant_lantern_view_476b70`,
`norrath_ctors_ant_lantern_view_476b80`,
`norrath_ctors_ant_lantern_view_476b90`,
`norrath_ctors_ant_lantern_view_4770d0`,
`norrath_ctors_ant_lantern_view_4770e8`,
`norrath_ctors_ant_lantern_view_477798`,
`norrath_ctors_ant_lantern_view_4777c8`,
`norrath_ctors_ant_lantern_view_4777d8`,
`norrath_ctors_ant_lantern_view_480b10`,
`norrath_ctors_ant_lantern_view_480b14`,
`norrath_ctors_ant_lantern_view_480b20`,
`norrath_ctors_ant_lantern_view_480b24`,
`norrath_ctors_ant_lantern_view_480b28`,
`norrath_ctors_ant_lantern_view_480b2c`,
`norrath_ctors_ant_lantern_view_480b30`,
`norrath_ctors_ant_lantern_view_480b34`,
`norrath_ctors_ant_lantern_view_480b38`,
`norrath_ctors_ant_lantern_view_480b3c`,
`norrath_ctors_ant_lantern_view_480b40`,
`norrath_ctors_ant_lantern_view_480b44`,
`norrath_ctors_ant_lantern_view_480b48`,
`norrath_ctors_ant_lantern_view_480b4c`,
`norrath_ctors_ant_lantern_view_480ba8`,
`norrath_ctors_ant_lantern_view_480bac`,
`norrath_ctors_ant_lantern_view_480bb0`,
`norrath_ctors_ant_lantern_view_480bb4`,
`norrath_ctors_ant_lantern_view_480cc8`,
`norrath_ctors_ant_lantern_view_480ccc`,
`norrath_ctors_ant_lantern_view_480cd0`,
`norrath_ctors_ant_lantern_view_480cd4`,
`norrath_ctors_ant_lantern_view_480cd8`,
`norrath_ctors_ant_lantern_view_480cdc`,
`norrath_ctors_ant_lantern_view_480ce0`,
`norrath_ctors_ant_lantern_view_480ce4`,
`norrath_ctors_ant_lantern_view_480ce8`,
`norrath_ctors_ant_lantern_view_480cec`,
`norrath_ctors_ant_lantern_view_480d28`,
`norrath_ctors_ant_lantern_view_480e58`,
`norrath_ctors_ant_lantern_view_480e5c`,
`norrath_ctors_ant_lantern_view_480e60`,
`norrath_ctors_ant_lantern_view_480e64`,
`norrath_ctors_ant_lantern_view_480e68`,
`norrath_ctors_ant_lantern_view_480e6c`,
`norrath_ctors_ant_lantern_view_480e70`,
`norrath_ctors_ant_lantern_view_480f4c`,
`norrath_ctors_ant_lantern_view_480f50`,
`norrath_ctors_ant_lantern_view_480f54`,
`norrath_ctors_ant_lantern_view_480f58`,
`norrath_ctors_ant_lantern_view_480f5c`,
`norrath_ctors_ant_lantern_view_480f60`,
`norrath_ctors_ant_lantern_view_480f64`,
`norrath_ctors_ant_lantern_view_480f68`,
`norrath_ctors_ant_lantern_view_480f6c`,
`norrath_ctors_ant_lantern_view_480f70`,
`norrath_ctors_ant_lantern_view_480f74`,
`norrath_ctors_ant_lantern_view_480f78`,
`norrath_ctors_ant_lantern_view_480f7c`,
`norrath_ctors_ant_lantern_view_480f80`,
`norrath_ctors_ant_lantern_view_480f84`,
`norrath_ctors_ant_lantern_view_48109c`,
`norrath_ctors_ant_lantern_view_4810a0`,
`norrath_ctors_ant_lantern_view_4810a4`,
`norrath_ctors_ant_lantern_view_4810a8`,
`norrath_ctors_ant_lantern_view_4810ac`,
`norrath_ctors_ant_lantern_view_4810b0`,
`norrath_ctors_ant_lantern_view_481160`,
`norrath_ctors_ant_lantern_view_481164`,
`norrath_ctors_ant_lantern_view_481168`,
`norrath_ctors_ant_lantern_view_48116c`,
`norrath_ctors_ant_lantern_view_481170`,
`norrath_ctors_ant_lantern_view_481174`,
`norrath_ctors_ant_lantern_view_481178`,
`norrath_ctors_ant_lantern_view_48117c`,
`norrath_ctors_ant_lantern_view_48124c`,
`norrath_ctors_ant_lantern_view_481250`,
`norrath_ctors_ant_lantern_view_481254`,
`norrath_ctors_ant_lantern_view_481258`,
`norrath_ctors_ant_lantern_view_48125c`,
`norrath_ctors_ant_lantern_view_481260`,
`norrath_ctors_ant_lantern_view_481264`,
`norrath_ctors_ant_lantern_view_481268`,
`norrath_ctors_ant_lantern_view_48126c`,
`norrath_ctors_ant_lantern_view_481270`,
`norrath_ctors_ant_lantern_view_481274`,
`norrath_ctors_ant_lantern_view_481278`,
`norrath_ctors_ant_lantern_view_48127c`,
`norrath_ctors_ant_lantern_view_481280`,
`norrath_ctors_ant_lantern_view_481284`,
`norrath_ctors_ant_lantern_view_481288`,
`norrath_ctors_ant_lantern_view_48128c`,
`norrath_ctors_ant_lantern_view_481290`,
`norrath_ctors_ant_lantern_view_481294`,
`norrath_ctors_ant_lantern_view_481298`,
`norrath_ctors_ant_lantern_view_4812d8`,
`norrath_ctors_ant_lantern_view_4812dc`,
`norrath_ctors_ant_lantern_view_4812e0`,
`norrath_ctors_ant_lantern_view_4812e4`,
`norrath_ctors_ant_lantern_view_4812e8`,
`norrath_ctors_ant_lantern_view_4812ec`,
`norrath_ctors_ant_lantern_view_4813bc`,
`norrath_ctors_ant_lantern_view_4813c0`,
`norrath_ctors_ant_lantern_view_4813c4`,
`norrath_ctors_ant_lantern_view_4813c8`,
`norrath_ctors_ant_lantern_view_4813cc`,
`norrath_ctors_ant_lantern_view_4813d0`,
`norrath_ctors_ant_lantern_view_4813d4`,
`norrath_ctors_ant_lantern_view_4814e4`,
`norrath_ctors_ant_lantern_view_4814e8`,
`norrath_ctors_ant_lantern_view_4814ec`,
`norrath_ctors_ant_lantern_view_4814f0`,
`norrath_ctors_ant_lantern_view_4814f4`,
`norrath_ctors_ant_lantern_view_4814f8`,
`norrath_ctors_ant_lantern_view_481540`,
`norrath_ctors_ant_lantern_view_481544`,
`norrath_ctors_ant_lantern_view_481704`,
`norrath_ctors_ant_lantern_view_481708`,
`norrath_ctors_ant_lantern_view_481890`,
`norrath_ctors_ant_lantern_view_481894`,
`norrath_ctors_ant_lantern_view_481898`,
`norrath_ctors_ant_lantern_view_48189c`,
`norrath_ctors_ant_lantern_view_4818a0`,
`norrath_ctors_ant_lantern_view_4818a4`,
`norrath_ctors_ant_lantern_view_4818a8`,
`norrath_ctors_ant_lantern_view_48198c`,
`norrath_ctors_ant_lantern_view_481990`,
`norrath_ctors_ant_lantern_view_481ab4`,
`norrath_ctors_ant_lantern_view_481ab8`,
`norrath_ctors_ant_lantern_view_481abc`,
`norrath_ctors_ant_lantern_view_481ac0`,
`norrath_ctors_ant_lantern_view_481ac4`,
`norrath_ctors_ant_lantern_view_481ac8`,
`norrath_ctors_ant_lantern_view_482328`,
`norrath_ctors_ant_lantern_view_48232c`,
`norrath_ctors_ant_lantern_view_482330`,
`norrath_ctors_ant_lantern_view_482334`,
`norrath_ctors_ant_lantern_view_482338`,
`norrath_ctors_ant_lantern_view_48233c`,
`norrath_ctors_ant_lantern_view_482340`,
`norrath_ctors_ant_lantern_view_48239c`,
`norrath_ctors_ant_lantern_view_482414`,
`norrath_ctors_ant_lantern_view_482418`,
`norrath_ctors_ant_lantern_view_48241c`,
`norrath_ctors_ant_lantern_view_482420`,
`norrath_ctors_ant_lantern_view_4825bc`,
`norrath_ctors_ant_lantern_view_4825c0`,
`norrath_ctors_ant_lantern_view_483e5c`,
`norrath_ctors_ant_lantern_view_483f88`,
`norrath_ctors_ant_lantern_view_4844c0`,
`norrath_ctors_ant_lantern_view_485158`,
`norrath_ctors_ant_lantern_view_485160`,
`norrath_ctors_ant_lantern_view_485168`,
`norrath_ctors_ant_lantern_view_485170`,
`norrath_ctors_ant_lantern_view_485178`,
`norrath_ctors_ant_lantern_view_485180`,
`norrath_ctors_ant_lantern_view_485188`,
`norrath_ctors_ant_lantern_view_485190`,
`norrath_ctors_ant_lantern_view_485198`,
`norrath_ctors_ant_lantern_view_485230`,
`norrath_ctors_ant_lantern_view_485270`,
`norrath_ctors_ant_lantern_view_4852d8`,
`norrath_ctors_ant_lantern_view_4852e0`,
`norrath_ctors_ant_lantern_view_485398`,
`norrath_ctors_ant_lantern_view_485420`,
`norrath_ctors_ant_lantern_view_485438`,
`norrath_ctors_ant_lantern_view_485440`,
`norrath_ctors_ant_lantern_view_485480`,
`norrath_ctors_ant_lantern_view_485488`,
`norrath_ctors_ant_lantern_view_4854c8`,
`norrath_ctors_ant_lantern_view_4854d0`,
`norrath_ctors_ant_lantern_view_4854d8`,
`norrath_ctors_ant_lantern_view_4854e0`,
`norrath_ctors_ant_lantern_view_4854e8`,
`norrath_ctors_ant_lantern_view_4854f0`,
`norrath_ctors_ant_lantern_view_4854f8`,
`norrath_ctors_ant_lantern_view_485500`,
`norrath_ctors_ant_lantern_view_485530`,
`norrath_ctors_ant_lantern_view_485538`,
`norrath_ctors_ant_lantern_view_485540`,
`norrath_ctors_ant_lantern_view_485548`,
`norrath_ctors_ant_lantern_view_485560`,
`norrath_ctors_ant_lantern_view_485568`,
`norrath_ctors_ant_lantern_view_485570`,
`norrath_ctors_ant_lantern_view_485578`,
`norrath_ctors_ant_lantern_view_485580`,
`norrath_ctors_ant_lantern_view_485760`,
`norrath_ctors_ant_lantern_view_485768`,
`norrath_ctors_ant_lantern_view_486030`,
`norrath_ctors_ant_lantern_view_486038`,
`norrath_ctors_ant_lantern_view_486040`,
`norrath_ctors_ant_lantern_view_486048`,
`norrath_ctors_ant_lantern_view_486050`,
`norrath_ctors_ant_lantern_view_486058`,
`norrath_ctors_ant_lantern_view_486060`,
`norrath_ctors_ant_lantern_view_4860e0`,
`norrath_ctors_ant_lantern_view_4860e8`,
`norrath_ctors_ant_lantern_view_4860f0`,
`norrath_ctors_ant_lantern_view_4860f8`,
`norrath_ctors_ant_lantern_view_486da4`,
`objectFindEmptySpot__FG6Point3iiPc`,
`setAnimEventFlags__8Creatureififififi`, `setState__7Critteri`,
`setWeapons__6Goblin`, `specificInit__7CritterPcT1`, `strcat`,
`strticmp__FPCcT0`.

## agents/ctors-lava-wraith

Merged (the branch notes carry the five summary sections). Thirty-three
constructors from `LavaMonster` through `Wraith` with fresh `input_order`
values 1400 through 1432, 43,004 compiled body bytes (not original-byte
matches). Each has `host/contract.cpp`; the 33 contracts exit 0 on this
machine (32 build as written; the Switch contract lacks the
`NORRATH_CTORS_LAVA_WRAITH_HOST` define its siblings set, so it only builds
with `-DNORRATH_CTORS_LAVA_WRAITH_HOST=1`, and passes that way). The worker's
differential emulator runs are private and were not reproduced here. No game
binary or disassembly listing was committed. As with ctors-ant-lantern, the
count rises because the constructors reference many game-object helpers and
original data words (`norrath_ctors_lava_wraith_view_*`) that no package
defines yet. 1181 function bodies, 1519 authored inputs, zero COMMON. 214
tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1516 |
| After | 1671 |
| Closed | 33 |
| Opened | 188 |

Closed: `norrath_construct_LavaMonster`, `norrath_construct_MaleDarkElf`,
`norrath_construct_Minion`, `norrath_construct_Mummy`,
`norrath_construct_MummyKing`, `norrath_construct_NPC`,
`norrath_construct_Orc`, `norrath_construct_PushPhysicsProp`,
`norrath_construct_PushTrigger`, `norrath_construct_RollDoor`,
`norrath_construct_Rondo`, `norrath_construct_Savepoint`,
`norrath_construct_Scorpion`, `norrath_construct_SeaMonster`,
`norrath_construct_Skeleton`, `norrath_construct_Skelight`,
`norrath_construct_Soul`, `norrath_construct_SpiderQueen`,
`norrath_construct_StaticFire`, `norrath_construct_SuperOrc`,
`norrath_construct_Switch`, `norrath_construct_Torch`,
`norrath_construct_Ulthork`, `norrath_construct_UndeadKnight`,
`norrath_construct_UndeadKnightClone`, `norrath_construct_Vampire`,
`norrath_construct_VampireLord`, `norrath_construct_WaterSpout`,
`norrath_construct_WaveMaker`, `norrath_construct_WeaponRack`,
`norrath_construct_WebTrap`, `norrath_construct_WoodElfSoldier`,
`norrath_construct_Wraith`.

Opened: `norrath_body_init`, `norrath_creature_finish_construction`,
`norrath_creature_set_anim_event_flags`,
`norrath_creature_set_anim_event_flags_res_map`,
`norrath_creature_set_parameters`, `norrath_ctors_lava_wraith_view_34b474`,
`norrath_ctors_lava_wraith_view_42b010`,
`norrath_ctors_lava_wraith_view_42b330`,
`norrath_ctors_lava_wraith_view_42b3d8`,
`norrath_ctors_lava_wraith_view_42b478`,
`norrath_ctors_lava_wraith_view_42b7e8`,
`norrath_ctors_lava_wraith_view_42bcb8`,
`norrath_ctors_lava_wraith_view_42bd18`,
`norrath_ctors_lava_wraith_view_42c228`,
`norrath_ctors_lava_wraith_view_42c570`,
`norrath_ctors_lava_wraith_view_42c630`,
`norrath_ctors_lava_wraith_view_42c8e0`,
`norrath_ctors_lava_wraith_view_42ca58`,
`norrath_ctors_lava_wraith_view_42cbd0`,
`norrath_ctors_lava_wraith_view_42ccf0`,
`norrath_ctors_lava_wraith_view_42ce60`,
`norrath_ctors_lava_wraith_view_42d048`,
`norrath_ctors_lava_wraith_view_42d138`,
`norrath_ctors_lava_wraith_view_42d2b0`,
`norrath_ctors_lava_wraith_view_42d480`,
`norrath_ctors_lava_wraith_view_42d578`,
`norrath_ctors_lava_wraith_view_42d650`,
`norrath_ctors_lava_wraith_view_42d700`,
`norrath_ctors_lava_wraith_view_42d7d0`,
`norrath_ctors_lava_wraith_view_42d8d0`,
`norrath_ctors_lava_wraith_view_42dbf0`,
`norrath_ctors_lava_wraith_view_42dc40`,
`norrath_ctors_lava_wraith_view_438c70`,
`norrath_ctors_lava_wraith_view_448fb0`,
`norrath_ctors_lava_wraith_view_449310`,
`norrath_ctors_lava_wraith_view_449550`,
`norrath_ctors_lava_wraith_view_449820`,
`norrath_ctors_lava_wraith_view_449940`,
`norrath_ctors_lava_wraith_view_449c20`,
`norrath_ctors_lava_wraith_view_44a390`,
`norrath_ctors_lava_wraith_view_44ab70`,
`norrath_ctors_lava_wraith_view_44adb0`,
`norrath_ctors_lava_wraith_view_44aea0`,
`norrath_ctors_lava_wraith_view_44af90`,
`norrath_ctors_lava_wraith_view_44b080`,
`norrath_ctors_lava_wraith_view_44b260`,
`norrath_ctors_lava_wraith_view_44b350`,
`norrath_ctors_lava_wraith_view_44b630`,
`norrath_ctors_lava_wraith_view_44b720`,
`norrath_ctors_lava_wraith_view_44b810`,
`norrath_ctors_lava_wraith_view_44c710`,
`norrath_ctors_lava_wraith_view_44c7a0`,
`norrath_ctors_lava_wraith_view_44def0`,
`norrath_ctors_lava_wraith_view_44dfe0`,
`norrath_ctors_lava_wraith_view_44e2b0`,
`norrath_ctors_lava_wraith_view_44f3d8`,
`norrath_ctors_lava_wraith_view_44f5b8`,
`norrath_ctors_lava_wraith_view_44f798`,
`norrath_ctors_lava_wraith_view_44fa38`,
`norrath_ctors_lava_wraith_view_44fb28`,
`norrath_ctors_lava_wraith_view_44fc18`,
`norrath_ctors_lava_wraith_view_44fd08`,
`norrath_ctors_lava_wraith_view_44fdf8`,
`norrath_ctors_lava_wraith_view_44fee8`,
`norrath_ctors_lava_wraith_view_450440`,
`norrath_ctors_lava_wraith_view_46cd20`,
`norrath_ctors_lava_wraith_view_46cd30`,
`norrath_ctors_lava_wraith_view_46cd40`,
`norrath_ctors_lava_wraith_view_46cd50`,
`norrath_ctors_lava_wraith_view_46cd60`,
`norrath_ctors_lava_wraith_view_46cd70`,
`norrath_ctors_lava_wraith_view_46d7b8`,
`norrath_ctors_lava_wraith_view_470358`,
`norrath_ctors_lava_wraith_view_4718e8`,
`norrath_ctors_lava_wraith_view_4718f8`,
`norrath_ctors_lava_wraith_view_471908`,
`norrath_ctors_lava_wraith_view_471918`,
`norrath_ctors_lava_wraith_view_471928`,
`norrath_ctors_lava_wraith_view_471938`,
`norrath_ctors_lava_wraith_view_471948`,
`norrath_ctors_lava_wraith_view_471958`,
`norrath_ctors_lava_wraith_view_471968`,
`norrath_ctors_lava_wraith_view_471978`,
`norrath_ctors_lava_wraith_view_471988`,
`norrath_ctors_lava_wraith_view_471998`,
`norrath_ctors_lava_wraith_view_472040`,
`norrath_ctors_lava_wraith_view_472050`,
`norrath_ctors_lava_wraith_view_472060`,
`norrath_ctors_lava_wraith_view_472c10`,
`norrath_ctors_lava_wraith_view_472c28`,
`norrath_ctors_lava_wraith_view_472c38`,
`norrath_ctors_lava_wraith_view_472c50`,
`norrath_ctors_lava_wraith_view_472c60`,
`norrath_ctors_lava_wraith_view_472c78`,
`norrath_ctors_lava_wraith_view_472c88`,
`norrath_ctors_lava_wraith_view_472ca0`,
`norrath_ctors_lava_wraith_view_472cb0`,
`norrath_ctors_lava_wraith_view_472cc8`,
`norrath_ctors_lava_wraith_view_473f20`,
`norrath_ctors_lava_wraith_view_473f30`,
`norrath_ctors_lava_wraith_view_473f40`,
`norrath_ctors_lava_wraith_view_473f58`,
`norrath_ctors_lava_wraith_view_473f70`,
`norrath_ctors_lava_wraith_view_473f88`,
`norrath_ctors_lava_wraith_view_473fa0`,
`norrath_ctors_lava_wraith_view_473fb0`,
`norrath_ctors_lava_wraith_view_473fc0`,
`norrath_ctors_lava_wraith_view_473fd8`,
`norrath_ctors_lava_wraith_view_473ff0`,
`norrath_ctors_lava_wraith_view_474000`,
`norrath_ctors_lava_wraith_view_474010`,
`norrath_ctors_lava_wraith_view_474028`,
`norrath_ctors_lava_wraith_view_474040`,
`norrath_ctors_lava_wraith_view_474050`,
`norrath_ctors_lava_wraith_view_474060`,
`norrath_ctors_lava_wraith_view_474540`,
`norrath_ctors_lava_wraith_view_474550`,
`norrath_ctors_lava_wraith_view_474560`,
`norrath_ctors_lava_wraith_view_474570`,
`norrath_ctors_lava_wraith_view_477568`,
`norrath_ctors_lava_wraith_view_477580`,
`norrath_ctors_lava_wraith_view_477598`,
`norrath_ctors_lava_wraith_view_4775b0`,
`norrath_ctors_lava_wraith_view_4775c8`,
`norrath_ctors_lava_wraith_view_4775d8`,
`norrath_ctors_lava_wraith_view_4778c8`,
`norrath_ctors_lava_wraith_view_4778d8`,
`norrath_ctors_lava_wraith_view_483e28`,
`norrath_ctors_lava_wraith_view_483f88`,
`norrath_ctors_lava_wraith_view_484fa0`,
`norrath_ctors_lava_wraith_view_484fa8`,
`norrath_ctors_lava_wraith_view_485240`,
`norrath_ctors_lava_wraith_view_4852a8`,
`norrath_ctors_lava_wraith_view_485330`,
`norrath_ctors_lava_wraith_view_4853b8`,
`norrath_ctors_lava_wraith_view_4853d0`,
`norrath_ctors_lava_wraith_view_4853d8`,
`norrath_ctors_lava_wraith_view_4857e8`,
`norrath_ctors_lava_wraith_view_4857f0`,
`norrath_ctors_lava_wraith_view_4857f8`,
`norrath_ctors_lava_wraith_view_485800`,
`norrath_ctors_lava_wraith_view_485808`,
`norrath_ctors_lava_wraith_view_485810`,
`norrath_ctors_lava_wraith_view_485818`,
`norrath_ctors_lava_wraith_view_485820`,
`norrath_ctors_lava_wraith_view_485828`,
`norrath_ctors_lava_wraith_view_4858b8`,
`norrath_ctors_lava_wraith_view_4858c0`,
`norrath_ctors_lava_wraith_view_4858c8`,
`norrath_ctors_lava_wraith_view_486118`,
`norrath_ctors_lava_wraith_view_486120`,
`norrath_ctors_lava_wraith_view_486128`,
`norrath_ctors_lava_wraith_view_486130`,
`norrath_ctors_lava_wraith_view_601030`,
`norrath_ctors_lava_wraith_view_601fd0`, `norrath_game_line_get`,
`norrath_game_line_get_ptr`, `norrath_game_object_e_rand`,
`norrath_game_object_find_tag_float`, `norrath_item_create`,
`norrath_item_create_random`, `norrath_lava_monster_get_model`,
`norrath_light_deactivate`, `norrath_male_dark_elf_init_type`,
`norrath_male_dark_elf_set_state`, `norrath_map_add_icon`,
`norrath_mat_transform`, `norrath_mummy_init_type`,
`norrath_mummy_set_state`, `norrath_mummy_set_weapon`,
`norrath_object_find_animation`, `norrath_orc_init_type`,
`norrath_orc_set_weapon`, `norrath_skeleton_init_type`,
`norrath_skeleton_set_state`, `norrath_skelight_init_weapon`,
`norrath_skelight_set_state`, `norrath_softdouble_add_2fb5c0`,
`norrath_softdouble_multiply_2fb680`, `norrath_softdouble_to_float_2fbc20`,
`norrath_soul_set_state`, `norrath_static_fire_init`, `norrath_strticmp`,
`norrath_wave_maker_get_wavelet_mat`, `norrath_web_construct`,
`norrath_wood_elf_soldier_init_type`, `norrath_wood_elf_soldier_set_state`,
`norrath_world_hitscan`.

## agents/inventory-items

Merged (the branch notes carry the five summary sections). The notes list the
plan table twice (the second copy adds three units); rows were de-duplicated
by source and their hashes agree. Twenty-seven units with fresh `input_order`
values 1433 through 1459: 23 functions (including the file-local `fill`
helper of `shopStart`, 10,640 compiled body bytes, not original-byte matches)
plus data and view units. Each unit has a `host_check.cpp`, `host_test.cpp`
or `host/check.cpp`; all 25 host programs build and exit 0 on this machine
(25 of 25). The notes' "imports closed" list is the worker's; the official
closed list is below. No game binary or disassembly listing was committed.
1204 function bodies, 1546 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1671 |
| After | 1686 |
| Closed | 26 |
| Opened | 41 |

Closed: `_$_4Gold`, `_$_8ItemProp`, `__tf4Gold`, `__tf8ItemProp`, `__tfP8ItemProp`,
`clone__4Gold`, `clone__8ItemProp`, `invMsgCallback__FPUci`,
`inventoryGetItemIndex__FiPcPiT2`, `msg_draw__4Gold`, `msg_draw__8ItemProp`,
`msg_getPokeName__8ItemPropPUsi`, `msg_load__4GoldPc`,
`msg_load__8ItemPropPc`, `msg_poke__8ItemPropi`, `msg_run__4Gold`,
`msg_save__4GoldPc`, `msg_save__8ItemPropPc`,
`norrath_inventory_ability_sound_lump_view`,
`norrath_inventory_ability_sound_resource_view`,
`norrath_inventory_reset_resource_names_view`, `queryInventoryItem__FPci`,
`removeItemFromInventory__Fiiii`, `shopMsgCallback__FPUci`, `shopRunning`,
`shopStart__Fiii`.

Opened: `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `Pop__6Camera`,
`__14AnimationStateRC14AnimationState`,
`animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`, `animState`,
`animStatePtr`, `coins`,
`compareToItem__Q24item11descriptionPQ24item11description`,
`drinkPotion__6Playeriiii`, `getName__4itemPQ24item11descriptioni`,
`getValue__4itemPQ24item11description`, `get__11ResourceMap`, `invGrid`,
`inventory`, `inventoryAdjustAbility__Fii`, `inventoryAdjustSkill__Ficx`,
`inventoryExit__Fiii`, `inventoryGridDropItem__Fiiciiii`,
`modelSetHalo__FP7HaloDefi`, `norrath_rtti_ptr`, `norrath_ti_4Prop`,
`pack__4itemPQ24item11description`, `playerLogInventory__Fi`,
`playerMaybeGetItem__FiPQ24item11description`, `playerNetDead__Fv`,
`resetPlayerAnimations__Fi`, `setupHalo__4itemPQ24item11descriptionf`,
`shop`, `shopBuy__FcPQ24item11description`, `shopDisableMouse`,
`shopFrame__Fv`, `shopIntro`,
`shopItemSetAttributes__FiiPcPQ24item11description`, `shopList`,
`shopMouseDecay`, `shopTick`, `startFrame`,
`unequipItem__FiPQ24item11description`,
`unpack__4itemPQ24item11description`, `ustrcat__FPUsPc`,
`waitingForStreamSpool`.


Second check by another pass over the same merge: the closed (26), opened (41)
and official counts (1671 to 1686) reproduce independently on this tip. One
host check is unsound under the sanitizers although it exits 0 without them:
`inventory-items-msg-load-itemprop/host_check.cpp` stores to `order[steps++]`
past the end of its own 4-entry array (the Prop loader stub is called more
than four times), which ASan/UBSan flag at line 25. It is a defect in the check's
stub, not in the source, and does not change the verdict. A scripted mutation
pass (one numeric literal changed at a time) was killed by the checks in
every unit that has literals: 10/10 `msg_load__8ItemPropPc` and `_$_4Gold`,
9/10 `msg_draw__4Gold`, `removeItemFromInventory__Fiiii` and `shopStart__Fiii`,
5/10 `invMsgCallback__FPUci`, 4/10 `msg_run__4Gold`.

## agents/save-and-quests

Merged (the branch notes carry the five summary sections). Twenty-two of the
23 units were selected with fresh `input_order` values 1546 through 1567: 20
functions (including the file-local `call_slot` helper of `msg_load__8Creature`;
6012 compiled body bytes, not original-byte matches) and three data words.
Each has a `host_check.cpp`; all 23 host programs build and exit 0 here.
save-and-quests-lever-load was **not** selected: the link fails with a
multiple definition of `msg_load__5LeverPc`, already defined by input 910
(an earlier package). Its files stay in the source snapshot. Which body should
own the name is for the authors to decide. No game binary or disassembly
listing was committed. 1224 function bodies, 1568 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1686 |
| After | 1679 |
| Closed | 22 |
| Opened | 15 |

Closed: `AsciiString2Sjis__FPUcPUs`, `addQuestToLog__FPcT0`, `amx_GetAddr`,
`amx_GetString`, `dataHandle`, `msg_load__10GameObjectPc`,
`msg_load__16CosmeticPropAnimPc`, `msg_load__4PropPc`,
`msg_load__8CreaturePc`, `msg_save__10GameObjectPc`,
`msg_save__16CosmeticPropAnimPc`, `msg_save__4PropPc`,
`msg_save__8CreaturePc`, `removeQuestFromLog__FPc`,
`scriptAddDeferedCall__FPc`, `scriptGetGameState__FPc`,
`scriptInPostAction`, `scriptSetGameState__FPci`,
`scriptSetPostAction__FPc`, `scriptStart__FPc`, `stateData`,
`textHandler__FPUci`.

Opened: `Ascii2Sjis__FUc`, `__tf3Orc`, `amx_Exec`, `amx_FindPublic`, `deferedCall`,
`inMsg`, `lilog2__Fl`, `msgTimeout`, `nextNmDeferedCalls`,
`objectAddToSlowRunList__FP10GameObject`, `outMsg`,
`quatNormalize__FP4Quat`, `quatToMat__FP8Matrix34P4Quat`, `scriptFrame`,
`waiting`.

## agents/prop-poke-clone

Merged (the branch notes carry the five summary sections). Twenty-seven of
the 37 units were selected with fresh `input_order` values 1568 through 1594
(27 functions, 7456 compiled body bytes, not original-byte matches). Ten
`clone__*` units were **not** selected: `clone__3Cat`, `clone__9CdEyeball`,
`clone__7Counter`, `clone__8Creature`, `clone__10GameObject`, `clone__5Lever`,
`clone__13PokeReflector`, `clone__11SmallSpider`, `clone__5Timer` and
`clone__13TriggerParams` are already defined by inputs 958 through 967 from an
earlier package, so selecting both would be a multiple definition. Their files
stay in the source snapshot. Which body should own those names is for the
authors to decide. All 37 `host_check.cpp` programs build and exit 0 here.
No game binary or disassembly listing was committed. 1251 function bodies,
1595 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1679 |
| After | 1656 |
| Closed | 28 |
| Opened | 5 |

Closed: `_$_13UserParamProp`, `_$_16CosmeticPropAnim`, `_$_4Prop`, `_$_7Player1`,
`__tf13UserParamProp`, `__tf16CosmeticPropAnim`, `__tf4Prop`,
`__tf7Player1`, `__tf8AutoProp`, `__tfP4Prop`, `__tfP6PhProp`,
`clone__13UserParamProp`, `clone__16CosmeticPropAnim`, `clone__4Prop`,
`clone__7Player1`, `getModel__10GameObject`,
`msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__13PokeReflectorPUsi`,
`msg_getPokeName__13UserParamPropPUsi`, `msg_getPokeName__5LeverPUsi`,
`msg_getPokeName__8CreaturePUsi`,
`msg_moveCollision__10GameObjectPP10GameObject`, `msg_poke__10GameObjecti`,
`msg_poke__13PokeReflectori`, `msg_poke__13UserParamPropi`,
`msg_poke__5Leveri`, `msg_route__10GameObjecti`, `norrath_ti_4Prop`.

Opened: `__tf6PhProp`, `__tf9MummyKing`, `eRand__10GameObject`,
`findTagFloat__10GameObjectPci`, `norrath_ti_6PhProp`.

## agents/prop-run-draw

Merged (the branch notes carry the five summary sections, plus a host-check
gaps section). Twenty-one units with fresh `input_order` values 1595 through
1615: 23 emitted functions (including two file-local `set_state` helpers;
10,600 compiled body bytes, not original-byte matches). No duplicate
definition was found. Each unit has a `host_check.cpp`; all 21 build and exit
0 here. The notes' own "host-check gaps" section is the author's account of
what the checks do not cover and still applies. No game binary or
disassembly listing was committed. 1274 function bodies, 1616 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1656 |
| After | 1658 |
| Closed | 21 |
| Opened | 23 |

Closed: `msg_draw__10GameObject`, `msg_draw__13UserParamProp`,
`msg_draw__14GnomeNavigator`, `msg_draw__16CosmeticPropAnim`,
`msg_draw__4Prop`, `msg_draw__5Lever`, `msg_draw__7Natasla`,
`msg_draw__9CdEyeball`, `msg_run__10GameObject`, `msg_run__11SmallSpider`,
`msg_run__13UserParamProp`, `msg_run__14GnomeNavigator`,
`msg_run__16CosmeticPropAnim`, `msg_run__3Cat`, `msg_run__4Prop`,
`msg_run__4Trap`, `msg_run__5Lever`, `msg_run__7Mermaid`,
`msg_run__9CdEyeball`, `msg_trigger__10GameObjecti`,
`msg_trigger__7Counteri`.

Opened: `addTurnAnimation__3Cat`, `bExitStatePersueIdle__8Creature`,
`bFarFromHome__8Creature`,
`gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`,
`lightSetAmbientOffset__FPs`, `matIdent__FP8Matrix34`,
`matMakeTransYawScale__FP8Matrix34G6Point3sf`, `matRot__FP8Matrix34is`,
`matTrans__FP8Matrix34fff`, `matTransform__FP6Point3P8Matrix44T0`,
`modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`, `modelGetC
harAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`,
`move2__8CreatureG6Point3`, `newDamageHandler__8CreatureP12_modelHeader`,
`objectActiveProcessing__FP10GameObjectii`, `slideBack__8Creaturef`,
`smartWander__8Creature`, `steerAndRouteToPosition__8CreatureG6Point3`,
`subTurnAnimation__3Cat`, `switchToWalk__3Cat`,
`worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3`,
`worldFindStandHeight__FP12_worldHeaderG6Point3f`, `worldViewMatrix`.

## agents/w8-actor-virtuals-a

Merged (the branch notes carry the five summary sections). Thirty units with
fresh `input_order` values 1616 through 1645, all with a `host_check.cpp`:
13 destructors, 14 type-info functions, `_$_14AnimationState`,
`_$_15FxCreatureState`, and one more destructor (30 functions in total, 3320
compiled body bytes, not original-byte matches). All 30 host programs pass
here (30 of 30); `_$_14AnimationState`'s check only builds as intended with
`-DNORRATH_W8AV_HOST` (without it the unit reads raw 32-bit words and
segfaults on a 64-bit host, which is a harness build detail, not a failure of
the check). The worker's claim (30 closed, 3 opened) reproduces exactly. No
game binary or disassembly listing was committed. 1304 function bodies, 1646
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1658 |
| After | 1631 |
| Closed | 30 |
| Opened | 3 |

Closed: `_$_10HateBridge`, `_$_10Teleporter`, `_$_11MissileTrap`,
`_$_14AnimationState`, `_$_15FxCreatureState`, `_$_15PushPhysicsProp`,
`_$_7Shooter`, `_$_7Trigger`, `_$_8FireBomb`, `_$_8GothTrap`,
`_$_8HackDoor`, `_$_8HateGate`, `_$_8HateHole`, `_$_8HateInto`,
`_$_9Container`, `_$_9WorldPart`, `__tf10HateBridge`, `__tf10Teleporter`,
`__tf11MissileTrap`, `__tf15PushPhysicsProp`, `__tf3NPC`, `__tf7Shooter`,
`__tf7Trigger`, `__tf8FireBomb`, `__tf8GothTrap`, `__tf8HackDoor`,
`__tf8HateGate`, `__tf8HateHole`, `__tf8HateInto`, `__tf9Container`.

Opened: `_$_8AutoProp`, `__ti8AutoProp`, `worldReleaseDoor__FP12_worldHeaderi`.

## agents/w8-actor-virtuals-b

Merged (the branch notes carry the five summary sections). Thirty units with
fresh `input_order` values 1646 through 1675 (31 emitted functions, 8204
compiled body bytes, not original-byte matches): clone, collision, draw,
hurt, load, poke, run and type-info methods of the actor classes. Each has a
`host_check.cpp`; all 30 build and exit 0 here (30 of 30). No duplicate
definition was found at link time. No game binary or disassembly listing was
committed. 1335 function bodies, 1676 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1631 |
| After | 1613 |
| Closed | 30 |
| Opened | 12 |

Closed: `__tf9WorldPart`, `advanceState__8AutoProp`, `clone__10Teleporter`,
`clone__11MissileTrap`, `clone__15PushPhysicsProp`, `clone__7Shooter`,
`clone__7Trigger`, `clone__8FireBomb`, `clone__9Container`,
`clone__9WorldPart`, `getModel__10AutoObject`,
`msg_collision__15PushPhysicsPropP10GameObject`,
`msg_collision__9ContainerP10GameObject`, `msg_draw__10AutoObject`,
`msg_draw__10Teleporter`, `msg_draw__15PushPhysicsProp`,
`msg_draw__9Container`, `msg_getPokeName__8AutoPropPUsi`,
`msg_getPokeName__9ContainerPUsi`, `msg_hurt__8AutoPropfP10GameObjecti`,
`msg_hurt__9ContainerfP10GameObjecti`, `msg_load__15PushPhysicsPropPc`,
`msg_load__7ShooterPc`, `msg_load__7TriggerPc`, `msg_load__8AutoPropPc`,
`msg_load__9ContainerPc`, `msg_poke__8AutoPropi`, `msg_poke__9Containeri`,
`msg_run__10HateBridge`, `msg_run__10Teleporter`.

Opened: `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `addEffect__FiG6Point3`,
`bodyInit__FP5_bodyfffff`, `bodyUpdateMatrix__FP5_body`,
`lightSetAmbientColor__FPs`, `lightSetAmbientOnly__Fv`,
`modelSetShadowScale__Ff`, `objectAddToRunList__FP10GameObject`,
`objectAlwaysDraw__FP10GameObject`, `objectCancelAllAlwaysDraw__Fv`,
`teleportParticle1`, `teleportParticle2`.

## agents/w8-core-loop-a

Merged (the branch notes carry the five summary sections). Thirty-four units
with fresh `input_order` values 1676 through 1709: 19 emitted functions (3484
compiled body bytes, not original-byte matches, including static-initializer
functions) and data words. Each unit has a host check; all 34 build and exit
0 here (34 of 34). No duplicate definition was found at link time. No game
binary or disassembly listing was committed. 1354 function bodies, 1710
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1613 |
| After | 1592 |
| Closed | 31 |
| Opened | 10 |

Closed: `_GLOBAL_$I$_3map$mapOffset`,
`_GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState`,
`_GLOBAL_$I$cutMode`, `_GLOBAL_$I$engineLastAITime`,
`_GLOBAL_$I$mcDisableStartupScript`, `_GLOBAL_$I$netInterfaceIsUp`,
`_GLOBAL_$I$noAmbientOffset`, `_GLOBAL_$I$route_generation`, `act1`, `act2`,
`act3`, `act4`, `act5`, `actionCoolOff`, `ambientOnly`, `anim`,
`animFrame__Fv`, `animLoadingComplete`, `animStarted`, `c.1089`,
`cleanUp__Fv`, `compositeAlpha`, `convertEnd__Fi`, `copyScreen`,
`copyScreenAlpha`, `copyScreenColor`, `copyScreenFrame`,
`coreClearNetLogFuck`, `coreGameStep__Fii`, `coreHeadsetResetTimer`,
`coreInterfaceBrightness`.

Opened: `__tf11PushTrigger`, `__tf9Savepoint`, `compImmediateCompressor`,
`compImmediateDecompressor`, `compLocalCompressor`, `compLocalDecompressor`,
`gameCheckForDeathScreen__Fv`, `groupVolumeScale`, `padRunFrame__Fv`,
`tempPlayerStruct`.

## agents/w8-core-loop-b

Merged (the branch notes carry the five summary sections). Ten units with
fresh `input_order` values 1710 through 1719 (the notes' plan entries are
pretty-printed JSON blocks without `input_order`; those entries were taken in
listed order): 9 emitted functions, 5848 compiled body bytes, not
original-byte matches, plus a data unit. Each unit has a host check; all 10
build and exit 0 here (10 of 10). No duplicate definition was found at link
time. No game binary or disassembly listing was committed. 1363 function
bodies, 1720 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

| | |
|---|---|
| Before | 1592 |
| After | 1609 |
| Closed | 31 |
| Opened | 48 |

Closed: `coreInterfaceFadeUpVal`, `coreInterfaceFadeUp__Fi`,
`coreLevelCommentTimer`, `coreLoadingStart`, `coreStartSwitchScene__Fi`,
`coreSwitchScene`, `coreSwitchSceneStart`, `correction`, `curFocus`,
`curPlaySeconds`, `currentLayerDir`, `currentRunningObject`, `cutClear__Fv`,
`cutRunFrame__Fb`, `delayCount`, `disableVolumetricLighting`,
`doNewWorld__FPc`, `drawAutoMap__Fv`, `drawBustCur`, `drawBustMax`,
`drawBustOn`, `drawBustTexName`, `drawBustTitle`, `drawCompassOn`,
`drawCompassPoint`, `drawCompassTitle`, `drawDome__Fv`, `drawListClear__Fv`,
`dummyTex`, `earlyInit__3mapP12_worldHeader`, `endInit`.

Opened: `GetTargetNum__6Camera`, `SetCutWasActive__6Camera`, `_3map$mapOffset`,
`_3map$mapheight`, `_3map$maptiles`, `_3map$mapwidth`, `advanceFrame__Ff`,
`animationLoadPos`, `animationLoading`, `cdIsBusy__Fv`,
`cdLoadAsync__FPcPvT0ii`, `cutCameraPos`, `cutInitScene__FP9_cutscene`,
`cutStop__Fv`, `dialogPacket`, `displayDialogPos`, `dlgDir`, `domeLump`,
`drawCharacterCallBack__FiP6Point3PsPiN23`,
`draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is`, `flags`,
`isRevealed__3mapv`, `lightDisableLocalLights__Fv`,
`lightGlobalCharLightScale__Ff`, `mapButtonWasDown`, `mapObjList`,
`mapScale`, `mapScissorX`, `mapScissorY`, `mapX`, `mapY`,
`matMakeTrans__FP8Matrix34G6Point3`, `modelSetNoZWrite__Fv`, `myFOV`,
`myPitch`, `myRoll`, `myYaw`, `netGetLocalObject__FP8Creature`, `nmActions`,
`nmDialogNodes`, `norrath_softdouble_compare_2fba48`,
`revealMap__3mapG6Point3`, `scaleScale`, `setCamera__Ff`,
`thereIsACutPlayer`, `validLocalPlayer`, `willDraw__3mapP12_worldHeaderf`,
`worldSetViewMatricies__FP6Point3P8Matrix34`.

## agents/w8-core-loop-c

Merged (the branch notes carry the five summary sections). Thirty-one units
with fresh `input_order` values 1720 through 1750: 11 emitted functions (1004
compiled body bytes, not original-byte matches) and 20 data-only units. Each
unit has a host check; all 31 build and exit 0 here (31 of 31). No duplicate
definition was found at link time. No game binary or disassembly listing was
committed. 1374 function bodies, 1751 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1609 |
| After | 1593 |
| Closed | 31 |
| Opened | 15 |

Closed: `endTimer`, `endTimer.1059`, `engineDisable30fps`, `engineFullFrameMode`,
`engineGetAIStepCount__Fv`, `engineLastAITime`,
`engineLoadEnvTex__FP8_texture`, `engineReboot__Fv`, `engineScissory1`,
`engineScissory2`, `engineScreenSlide`, `engineSetBlur__Fi`,
`engineYOffset`, `fadeIconY`, `fadeTimer`, `firstTime`, `flaredIn`,
`flaredOut`, `frameNumWrapped__Fv`, `gameAutoMapMode`, `gameCameraShake`,
`gameCheckNoSaveZone__Fv`, `gameCreatureThinkSpinner`, `gameDeathStart__Fi`,
`gameDeathTimer`, `gameDeathTimerSingle`, `gameGetFullLevelName__FPc`,
`gameItemTreasureRamp__Fv`, `gameLevelLoading`, `gameLineClear__Fv`,
`gameLineDebugDraw__Fv`.

Opened: `UnpreparePowerOff__Fv`, `_$_4Line`, `checkSave__FP10GameObject`,
`deathNetHack`, `deathState`, `frameNumWrappedPS__FP14ParticleSystem`,
`gameDeath__Fv`, `gameLineCount`, `gameLines`, `nextFrameDrop`,
`objectIterateRunLists__FPFP10GameObject_v`, `saveOk`, `sceSifExitCmd`,
`soundQuiet__Fv`, `texTerminateThread__Fv`.

## agents/w8-core-loop-d

Merged (the branch notes carry the five summary sections). Thirty-two units
with fresh `input_order` values 1751 through 1782: 16 emitted functions (4972
compiled body bytes, not original-byte matches) and data words. All 18 host
programs under the package build and exit 0 here; the snapshot checker
accepts a host reference for every unit. No duplicate definition was found at
link time. No game binary or disassembly listing was committed. 1390 function
bodies, 1783 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

| | |
|---|---|
| Before | 1593 |
| After | 1629 |
| Closed | 31 |
| Opened | 67 |

Closed: `gameMiscTaskSync__Fv`, `gamePopConfig__Fv`, `gamePushConfig__Fv`,
`gameSetObjectVis__Fv`, `gameShowEnemiesInMap`, `gameSomeoneInShop__Fv`,
`getLongLevelName__FPc`, `initAnimation__Fv`, `initLump__FPv`, `init__Fv`,
`interfaceMode`, `interfaceOn`, `jumpGateClear__Fv`, `jumping`,
`lastWarpPos`, `layer0Dir`, `layer1Dir`, `levelSeeds`,
`loadingScreenRequestExit`, `makeUnscrambleTable__Fv`, `noMonsters`,
`norrath_ctors_ant_lantern_view_44e940`,
`norrath_ctors_ant_lantern_view_44ea78`,
`norrath_ctors_ant_lantern_view_453328`, `norrath_factory_prop_vtable`,
`optTbl`, `pan__FG6Point3ffPfT3`, `paramRunning__Fv`, `plotRouteTask__Fv`,
`rejectForeignModules__Fv`, `resetTextBody__Fv`.

Opened: `DLG_GetDialogCreature__Fv`, `_$_11AutoChestLP`, `_$_15AutoContainerLP`,
`_$_4Boat`, `__3NPCG6Point3sPcN33ifPPc`, `__tf11AutoChestLP`,
`__tf15AutoContainerLP`, `__tf4Boat`,
`animAddToEnd__FP14AnimationStateP15AnimationHeaderi`,
`animMapAnimationSounds__FP15AnimationHeader`, `animUseBuffer`, `bodyAlpha`,
`changeBeard__Fii`, `changeHairColor__Fii`, `changeHairStyle__Fii`,
`changeSkin__Fii`, `curPlayer`, `doMyUniqueForces__4Boat`, `dummyList`,
`getHairAndSkin__Fi`, `headerAlpha`, `hermDeltaScale`, `hermiteTime`,
`hpos`, `hvecs`, `initFile__FPcPvi`, `initLightInfo__Fv`, `initNamePad__Fv`,
`initPreSelect__Fv`, `jumpGateTaskDraw__Fv`, `jumpGateTaskRun__Fv`,
`lightMats`, `memmove`, `msg_collision__13AutoContainerP10GameObject`,
`msg_draw__11AutoChestLP`, `msg_draw__15AutoContainerLP`,
`msg_getPokeName__13AutoContainerPUsi`, `msg_getPokeName__9AutoChestPUsi`,
`msg_hurt__13AutoContainerfP10GameObjecti`, `msg_load__13AutoContainerPc`,
`msg_load__9AutoChestPc`, `msg_poke__11AutoChestLPi`,
`msg_poke__13AutoContaineri`, `msg_run__11AutoChestLP`,
`msg_run__15AutoContainerLP`, `msg_save__13AutoContainerPc`,
`msg_save__9AutoChestPc`, `nameTags`, `needEnforce`, `npcLumpName`,
`objectVisCount`, `objectVisPoints`, `objectVisUseCamCenter`,
`playerCheckRecallHotKey__Fv`, `playerRecallSpell__Fv`, `playerSelectInfo`,
`pnums`, `requestToExit`, `set__4Boat`, `startExitFade`, `strrchr`,
`targetObjs`, `targets`, `textBodyLen`, `unscrambleTable`,
`worldPlotRouteProcess__Fi`, `yaws`.

## agents/w8-frontend-menu-a

Merged (the branch notes carry the five summary sections). Thirteen units
with fresh `input_order` values 1783 through 1795: 11 emitted functions (3516
compiled body bytes, not original-byte matches) and data words. The notes'
plan table lists paths without the `sources/` prefix; the prefix was added.
All 13 host checks build and exit 0 here (13 of 13). The notes claim 24 of 25
named imports closed; the official link shows 24 names leaving the list.
No game binary or disassembly listing was committed. 1401 function bodies,
1796 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1629 |
| After | 1608 |
| Closed | 24 |
| Opened | 3 |

Closed: `Password__C13VIGameOptions`, `Username__C13VIGameOptions`,
`_GLOBAL_$D$optionMenu`, `_GLOBAL_$I$optionMenu`, `body`, `cheatOn`,
`colorLevelNames__FP4Menu`, `controlPaused`, `cursorTween`,
`debugClearStartMenu__Fv`, `debugRecall__Fi`, `difficultyMenu`,
`frontEndCleanUp__Fi`, `fullLaunchOK`, `gameControlDontResetPause`,
`gameTypeMenu`, `helpMessageOn`, `hudNoController`,
`hudRunNoControllers__Fi`, `hudRunYesNo__FPUsiiii`, `kickMenu`,
`kickMenuList`, `language`, `letterAtATime`.

Opened: `MC_NameToAct__FPc`, `__12RecallEffectG6Point3`,
`fontPrintWrapCenteredu__FiiPUsiiii`.

## agents/w8-frontend-menu-b

Merged as a **partial package**: the notes carry the observed/inferred
separation, the plan table and a summary with closed and opened imports, but
not separate "authored but never executed" and "unknown" sections. Every
selected function has a `host_check.cpp`; all 9 build and exit 0 here. Nine
of the 10 units were selected with fresh `input_order` values 1796 through
1804 (7 emitted functions, 5004 compiled body bytes, not original-byte
matches; the notes' plan table has no object names, so `<stem>.o` was used).
w8-frontend-menu-b-menu-words is a data-only unit with no host check at all,
and the snapshot checker requires a host file for every selected record, so
it stays in the snapshot, unselected, until a check exists; that is why
`norrath_menu_local_t_004844e8` and `norrath_menu_word_00484514` appear as
opened. No game binary or disassembly listing was committed. 1408 function
bodies, 1805 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

| | |
|---|---|
| Before | 1608 |
| After | 1589 |
| Closed | 24 |
| Opened | 5 |

Closed: `menuDraw__FiiP4MenuUiUiP4FontP8_texturei`, `menuModeRebootQuestion`,
`menuNetClientServer`, `menuNetImportOrNewGame`, `menuNetImportOrNewPlayer`,
`menuRun__FP4MenuP3padPi`, `nameChars`, `optionCount`, `optionIndex`,
`optionLinePos`, `optionMenu`, `optionsShowing`, `pauseProcess__Fv`,
`pushPause__Fv`, `ranBrowser`, `ranConfig`, `scrollEnd`, `scrollSpan`,
`scrollTarget`, `scrollY`, `showMore`, `showOptions`, `stopCredits__Fv`,
`title`.

Opened: `lerpColor__FUiUif`, `norrath_menu_local_t_004844e8`,
`norrath_menu_word_00484514`, `pauseStack`, `paused`.

## agents/w8-net-play-a

Merged (the branch notes carry the five summary sections). Eighteen units
with fresh `input_order` values 1805 through 1822: 12 emitted functions (2692
compiled body bytes, not original-byte matches) and data words. All 18 host
checks build and exit 0 here (18 of 18). No duplicate definition was found at
link time. No game binary or disassembly listing was committed. 1420 function
bodies, 1823 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

| | |
|---|---|
| Before | 1589 |
| After | 1560 |
| Closed | 35 |
| Opened | 6 |

Closed: `_GLOBAL_$I$nmPlayers`, `_GLOBAL_$I$playerSelectSuccessful`,
`_GLOBAL_$I$playerStartPos`, `_GLOBAL_$I$wrathHammerCount`, `abilityTable`,
`activePlayer`, `addSpell__8CreatureP5Spell`, `charAmbient`, `charDirect`,
`charDirect2`, `charDirectDir`, `charDirectDir2`, `charScaleB`,
`charScaleG`, `charScaleR`, `commInit__Fv`, `commMenu`, `commProcess__Fv`,
`commandQueue`, `enumerateAlivePlayers__Fv`, `frameStatePacket`,
`gameHidePlayers`, `gamePlayersPresent`, `givePlayerExperience__Fii`,
`globalCharLightScale`, `hudNetLevelWarp__FPc`, `isNetGame`,
`issueCommand__Fv`, `lastNmLocalPlayersAlive`, `launchPlayerSelect__Fv`,
`localNmPlayersAlive`, `nmPlayersAlive`, `playerSelectSuccessful`,
`playersAlive`, `validLocalPlayer`.

Opened: `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `commProcessReal__Fv`, `lgAudClose`,
`netGetLocalPlayerNmSafe__Fi`, `sceCdRead`, `sceCdReadIOPm`.

## agents/w8-pad-memcard-a

Merged (the branch notes carry the five summary sections). Twenty-seven units
with fresh `input_order` values 1823 through 1849: 20 emitted functions
(18,264 compiled body bytes, not original-byte matches) and data words. Each
unit has a `host_contract.cpp`; all 27 build and exit 0 here (27 of 27). Two
of them (`mc-reset-ram-slot`, `async-load-game`) need `-fno-builtin`, as the
notes say; without it one of them fails to run. No duplicate definition was
found at link time. The notes' plan entries carried their own `input_order`
values; fresh ones were assigned. No game binary or disassembly listing was
committed. The count rises because the memory-card calls pull in many
original data words and `sceMc*` imports that no package defines yet. 1440
function bodies, 1850 authored inputs, zero COMMON. 214 tool tests passed. No
EE or game execution.

| | |
|---|---|
| Before | 1560 |
| After | 1607 |
| Closed | 26 |
| Opened | 73 |

Closed: `MCA_FileOpen`, `MCA_Formatted`, `MCA_Present`, `MCA_Process__FPi`,
`MCA_Reset__Fv`, `MC_ChangePlayers__FiPUsiii`, `MC_FrontEndImport`,
`MC_HasThisLevelBeenSaved__Fv`, `MC_Kick__Fv`, `MC_ResetRamSlot__Fv`,
`MC_SetNewAct__Fi`, `MC_SpawnPoint`, `asyncChangePlayers__Fv`,
`asyncConfigure__Fv`, `asyncLoadGame__Fv`, `asyncSaveConfigSilent__Fv`,
`asyncSaveConfig__Fv`, `asyncSaveGame__Fv`,
`frontendInitControllerConfig__Fi`, `frontendRunControllerConfig__Fv`,
`mcProcessKick__Fv`, `mcRunCmdNo`, `netBlankControllerButtons__Fiii`,
`netBlankController__Fii`, `netLocalPad`, `neverSaveAlwaysLoadFromLump__FPc`.

Opened: `MCA_Busy__Fv`, `MCA_DirContents`, `MCA_Result`, `__extendsfdf2`,
`askedForConfigLoad`, `browserSlot`, `buttonValues`, `cardType`,
`commandQueueEmpty__Fv`, `configFileName`, `configFormatted`,
`configLumpLoaded`, `configMode`, `configStrings`, `curSel`,
`drawProgress__Fv`, `finishFrameCount`, `fontPrintWrapCentered__FiiPciiii`,
`getBestSlotName__FPc`, `hudHeaderUni`, `inGameConfigShuttingDown`,
`initializedLump`, `jumpGateHubReset__Fi`, `loadName`,
`loadPlayerInfo__FiPiP12playerStruct`, `loadRamSlot__Fv`, `lumpAdd__FPvPCc`,
`memcardTitle__FPUs`, `modifyRamSlotForNetSave__Fv`, `myLumpBuffer`,
`neverSave`, `newSel`, `nullBuff`, `pcLaunch`, `pcNmPlayers`, `pcTitle`,
`playerChangeOldPos`, `playerChangeOldYaw`, `playerGetMarkerColor__Fii`,
`playerRace`, `playerStatus`, `popPause__Fi`, `prepareBrowserUndo__Fv`,
`quickLoadStamp`, `raceStrings`, `ramSlot`, `resetWithClose__Fv`,
`runBrowser__FPUs`, `runConfigure__FiPUsi`, `sceMcChdir`, `sceMcClose`,
`sceMcDelete`, `sceMcFormat`, `sceMcGetDir`, `sceMcMkdir`, `sceMcOpen`,
`sceMcRead`, `sceMcSeek`, `sceMcWrite`, `serviceResult__FP8queueCmdPi`,
`slotInfoHeaders`, `sortedDir`, `sortedDirCount`, `startProgress__Fv`,
`strchr`, `t`, `tempBuffer`, `texStrings`, `textureLockCacheBuffer__Fv`,
`texturePrepareToLockCacheBuffer__Fv`, `textureReleaseCacheBuffer__Fv`,
`unModifyRamSlotForNetSave__Fv`, `undoBrowser__Fv`.

## agents/w8-pad-memcard-b

Merged (the branch notes carry the five summary sections; their plan table is
plain text without backticks). Eleven units with fresh `input_order` values
1850 through 1860: 6 emitted functions (4488 compiled body bytes, not
original-byte matches) and data words. All 11 host checks build and exit 0
here (11 of 11). No duplicate definition was found at link time. No game
binary or disassembly listing was committed. 1446 function bodies, 1861
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1607 |
| After | 1615 |
| Closed | 11 |
| Opened | 19 |

Closed: `padBigRumble__Fiff`, `padClear__Fv`, `padFrame.756`,
`padGetUnprocessedData__FiP18padUnprocessedData`, `padIndexMap`,
`padProcessData__FP3padP18padUnprocessedDataP12padStateData`,
`resetQuickLoad__Fv`, `saveConfigEndGame`, `tapCount`, `updateRamSlot__Fii`,
`whichPad`.

Opened: `MC_SaveLevelEntities__FPc`, `Save__6CameraPc`, `actAlignData`,
`actSendData`, `convertPointersToIndicies__Fi`, `jumpGateSave__FPc`,
`needActAlign`, `padFilter`, `processAnalogInput__FiiPfT2`, `rdata`,
`rumphole`, `scePadEnterPressMode`, `scePadInfoPressMode`, `scePadRead`,
`scePadSetActAlign`, `scePadSetActDirect`, `scePadSetMainMode`,
`scriptGameStateEntry__FiPi`, `startInhibit`.

## agents/w8-runtime-libc-a

Merged (the branch notes carry the five summary sections). Twenty-three units
with fresh `input_order` values 1861 through 1883: 17 emitted functions (3568
compiled body bytes, not original-byte matches) and data views. All 23 host
checks build and exit 0 here (23 of 23). No duplicate definition was found at
link time in the default build. One caveat: the package defines `floorf` and
`ceilf` itself, as its notes say, so the optional Newlib inputs (plan entries
85 and 89, only built with `--newlib-root`) would now define the same two
names; that optional build was not run and would fail to link until one of
the two sets is dropped. No game binary or disassembly listing was
committed. 1463 function bodies, 1884 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1615 |
| After | 1610 |
| Closed | 23 |
| Opened | 18 |

Closed: `__assert__FPCcUiT0i`, `__builtin_delete`, `__eq__C9type_infoRC9type_info`,
`__pure_virtual`, `__rtti_si`, `_listCheckAllIntegrity__FPCci`, `_printf`,
`abort`, `atoi`, `ceilf`, `close`, `exit`, `floorf`, `lseek`, `malloc`,
`memcmp`, `norrath_assert_context_word_view`, `norrath_assert_format_view`,
`norrath_factory_sscanf`,
`norrath_legacy_single_inheritance_type_info_vtable_view`,
`norrath_legacy_user_type_info_vtable_view`,
`norrath_player_bigint_small_powers5_view`,
`norrath_player_number_big_tens_view`.

Opened: `_$_14__si_type_info`, `_$_16__user_type_info`, `__divdi3`, `__moddi3`,
`__terminate`, `__tf14__si_type_info`, `__tf16__user_type_info`,
`__udivdi3`, `__umoddi3`, `_exit`, `do_dyncast__C14__si_type_infoiQ216__user
_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result`,
`do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPv
T3T4RQ216__user_type_info14dyncast_result`,
`do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3`,
`do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3`, `do_upcast__C
14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_
info13upcast_result`, `do_upcast__C16__user_type_infoQ216__user_type_info8su
b_kindRC9type_infoPvRQ216__user_type_info13upcast_result`, `printfloat`,
`raise`.

## agents/w8-runtime-libc-b

Merged (the branch notes carry the five summary sections). Twenty-four units
with fresh `input_order` values 1884 through 1907: 16 emitted functions (1368
compiled body bytes, not original-byte matches) and data views. Each unit has
a `*_host_test.cpp`. All 24 build and exit 0 here (24 of 24) once the source
is built with `-D<name>=w8_<name>` for the names that collide with the host C
library, as the notes instruct (`strlen`, `strncmp` and `strncpy` need the
source and the test compiled separately). No duplicate definition was found
at link time. No game binary or disassembly listing was committed. 1479
function bodies, 1908 authored inputs, zero COMMON. 214 tool tests passed.
No EE or game execution.

| | |
|---|---|
| Before | 1610 |
| After | 1593 |
| Closed | 24 |
| Opened | 7 |

Closed: `norrath_player_number_half_lower_view`,
`norrath_player_number_half_upper_view`,
`norrath_player_number_overflow_view`,
`norrath_player_number_quarter_lower_view`,
`norrath_player_number_tens_view`, `norrath_player_number_tiny_tens_view`,
`norrath_player_scan_base_normalization_view`,
`norrath_player_scan_exponent_format_view`, `norrath_rtti_si`,
`norrath_rtti_user`, `powf`, `printf`, `rand`, `read`,
`readyToSwitch__3Cat`, `srand`, `strcasecmp`, `strcpy`, `strlen`, `strncmp`,
`strncpy`, `strstr`, `tanf`, `write`.

Opened: `__ieee754_powf`, `__ieee754_rem_pio2f`, `__kernel_tanf`, `sceTtyInit`,
`sceTtyRead`, `sceTtyWrite`, `ttyinit`.

## Not merged: agents/w8-spell-static-init-b

Rejected at tip `ed20fab` (30 units, 54 emitted functions: 25 static
initializer entries, the `skills` table and its initializer, three
constructors and an `AnimationState` copy). The notes carry the five summary
sections and the sources compile and link (selecting them took the count
from 1593 to 1633 here, with 30 names closed and 70 opened), but none of the
30 units has a host check, host contract or any other test, and the notes
mention only the private build as verification. The snapshot checker
therefore refuses the package (every selected record needs a host file), and
by the repo rule a function with no test that could fail cannot be accepted.

Not merged, and no files were edited. A re-merge needs a check beside each
unit that fails on a wrong call order, constant or store; then 30 plan
entries starting at the integrator's current end of plan. The worker's own
count (1195 to 1236) was measured on an older base and is not the official
delta.

## agents/w8-startup-sce-a

Merged (the branch notes carry the five summary sections). Thirty-four of the
35 units with fresh `input_order` values 1908 through 1941: 34 emitted
functions (2984 compiled body bytes, not original-byte matches). The package
has two kinds of check. Twenty `host_contract.cpp` programs run the C++ bodies
against mocks; all 20 build and exit 0 here. Fourteen `host_check.cpp`
programs (syscall stubs and one assembly body) are byte comparers that take
the private executable as an argument; the integrator does not read the
executable, so those 14 were **not** run, and the byte-match statements in
the notes are the worker's only. They are recorded in the snapshot as host
file references, like the earlier gs-dma-kernel stubs. w8-startup-sce-a-ctype-table
(`_ctype_`) was **not** selected: its only check, `host_check_data.py`, is not
in the branch (the file does not exist in the tree), so the unit has no
committed test and the snapshot checker rejects it; its source stays in the
snapshot, unselected. The worker did not run a full partial build; the
official counts are below. No game binary or disassembly listing was
committed. 1513 function bodies, 1942 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1593 |
| After | 1586 |
| Closed | 25 |
| Opened | 18 |

Closed: `DPRINT`, `DeleteThread`, `DisableDmac`, `GetOsdConfigParam`,
`Handler__7VITimeri`, `PowerOffCB`, `PowerOffHandler__FPv`,
`PowerOffThread__FPv`, `RemoveDmacHandler`, `RotateThreadReadyQueue`,
`SetArg`, `SetOsdConfigParam`, `SetTimerAlarm`, `SuspendThread`,
`TerminateLibrary`, `TimerUSec2BusClock`, `VIDisplayScreen16__FPCcib`,
`VIGetGameOptions__Fv`, `VIStartKeyboardDriver__Fv`, `VSync`, `VSync2`,
`WakeupThread`, `_LoadExecPS2`, `__wrap_free`, `__wrap_memalign`.

Opened: `AllocTimerCounter`, `InitTLB`, `SetTimerHandler`, `StartTimerCounter`,
`free`, `gCdFile`, `lgKbMInit`, `memalign`, `sCdRmode`, `sGameOptions`,
`sceCdCbPoff_Gp`, `sceCdDiskReady`, `sceCdPoffCbdata`, `sceCdPoffCbfunc`,
`sceCdPowerOff`, `sceDevctl`, `sceGifPkEnd`, `sceGifPkRefLoadImage`.

## agents/w8-startup-sce-b

Merged (the branch notes carry the five summary sections). Eighteen of the 19
units were selected with fresh `input_order` values 1942 through 1959 (18
emitted functions, 3456 compiled body bytes; not
original-byte matches). All 18 host checks build and exit 0 here (18 of 18).
w8-startup-sce-b-k-exit-tlb-handler (`_kExitTLBHandler`, assembly) was **not**
selected: the notes' only evidence is a private link compared with the
executable, with no check committed in its directory, so the snapshot
checker rejects it (no host file); its source stays in the snapshot,
unselected, which leaves `_kExitTLBHandler` open (kernel-patches' TLB table
still points at it). No duplicate definition was found at link time. No game
binary or disassembly listing was committed. 1531 function bodies, 1960
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1586 |
| After | 1580 |
| Closed | 24 |
| Opened | 18 |

Closed: `_mallinfo_r`, `_request_bind`, `_request_call`, `_request_end`,
`_request_rdata`, `_sceCd_scmdsdata`, `_sceRpcFreePacket`,
`_sceRpcGetPacket`, `_sceSifLoadModule`, `batchBuffer`, `cancelStreamWait`,
`cdClear__Fv`, `cdWait__Fv`, `cdvd_exit`, `deci2Putchar`,
`decompressThreadid`, `dmaGIF`, `eenull`, `gCd`, `initEzNetCtl`, `kprintf`,
`lgAudInit`, `lgCodecEnumerate`, `lgCodecInit`.

Opened: `InvalidDCache`, `__malloc_current_mallinfo`, `__malloc_lock`,
`__malloc_unlock`, `__malloc_update_mallinfo`, `_alarm_bind`,
`_alarm_rdata`, `_sceRpcGetFPacket`, `_sceRpcGetFPacket2`, `_search_svdata`,
`iSetTimerAlarm`, `iWakeupThread`, `isceSifSendCmd`, `kputs`,
`sceCdStPause`, `sceCdStStop`, `sceSifRemoveCmdHandler`, `streamPaused`.

## agents/w8-startup-sce-c

Merged (the branch notes carry the five summary sections). Twenty-seven
units with fresh `input_order` values 1960 through 1986: 23 emitted functions
(4748 compiled body bytes, not original-byte matches) and data words. All 27
host checks build and exit 0 here (27 of 27). No duplicate definition was
found at link time. No game binary or disassembly listing was committed.
1554 function bodies, 1987 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1580 |
| After | 1563 |
| Closed | 30 |
| Opened | 13 |

Closed: `lgCodecOpen`, `lgKbdClose`, `loadThreadID`, `preConfigure`, `sbuff`,
`sbuffin`, `sceCdCbPoff_Gp`, `sceCdGetError`, `sceCdLayerSearchFile`,
`sceCdPOffCallback`, `sceCdPoffCbdata`, `sceCdPoffCbfunc`, `sceCdReadClock`,
`sceCdReadDvdDualInfo`, `sceCdSearchFile`, `sceCdStInit`, `sceCdStRead`,
`sceCdSync`, `sceClose`, `sceGifPkAddGsData`, `sceGifPkInit`,
`sceGifPkReset`, `sceGifPkSize`, `sceGifPkTerminate`, `sceGszbufaddr`,
`sceInetClose`, `sceInetCtlDownInterface`, `sceInetCtlGetState`,
`sceInetGetInterfaceList`, `sceLibnetInitialize`.

Opened: `CallRpc`, `GLibData`, `MemcpyS`, `_cd`, `_iob`, `_rcv_data_rpc`,
`_sceFsSigSema`, `_sceFsWaitS`, `_send_data`, `get_iob`, `gsm_create`,
`gsm_option`, `sceSifMBindRpcParam`.

## agents/w8-startup-sce-d

Merged (the branch notes carry the five summary sections). Twenty-seven of
the 29 units were selected with fresh `input_order` values 1987 through 2013
(23 emitted functions, 6580 compiled body bytes,
not original-byte matches). Twenty-five host programs run here; the
`sif-get-reg` and `sif-set-reg` checks are byte comparers that take the
private executable as an argument, so those two (like the 14 in
startup-sce-a) were **not** run by the integrator and only exist as
references. Two units were **not** selected because they have no committed
check at all and the snapshot checker rejects a record without a host file:
w8-startup-sce-d-reset-tty-init (`sceResetttyinit`, `ttyinit`) and
w8-startup-sce-d-sif-get-sreg (`sceSifGetSreg`); their sources stay in the
snapshot, unselected. No duplicate definition was found at link time. No game
binary or disassembly listing was committed. 1577 function bodies, 2014
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1563 |
| After | 1551 |
| Closed | 29 |
| Opened | 17 |

Closed: `_cd`, `_iob`, `_rcv_data_rpc`, `_send_data`, `sceLibnetTerminate`,
`sceMcEnd`, `sceMcGetInfo`, `sceMtapGetModVersion`, `sceOpen`, `scePadEnd`,
`scePadGetModVersion`, `scePadGetSlotMax`, `scePadInit2`, `sceRead`,
`sceSdRemote`, `sceSifAddCmdHandler`, `sceSifAllocIopHeap`,
`sceSifExecNotifyHandler`, `sceSifExitRpc`, `sceSifFreeIopHeap`,
`sceSifGetReg`, `sceSifInitCmd`, `sceSifResetIop`, `sceSifSendCmd`,
`sceSifSetReg`, `sceneTransitionOK.1041`, `serialPutchar`, `setSRSInfo__Fv`,
`specialHandler`.

Opened: `MC_ByteCopy__FPvT0i`, `MC_GetNetGameInfo__FPiT0Pc`,
`MC_GetRamPlayerPtr__Fi`, `MC_GetTempPlayerStorage__Fi`, `_fs_version`,
`_sceSifSendCmd`, `kputchar`, `mceGetInfoApdx`, `new_iob`,
`norrath_sce_d_race_strings_view_4293e8`,
`norrath_sce_d_unknown_sdata_word_view_4852d8`, `sceFsInit`,
`sceSifFreeSysMemory`, `sceSifMUnBindRpc`, `sceSifSetDChain`,
`sceSifSetDma`, `sceSifStopDma`.

## agents/w8-actor-virtuals-c

Merged (the branch notes carry the five summary sections). Seventeen of the
29 units were selected with fresh `input_order` values 2014 through 2030 (19
emitted functions, 22,416 compiled body bytes, not original-byte matches).
The 17 host checks build and exit 0 here (17 of 17). Twelve units were **not**
selected because they have no check in the repository: the notes' evidence
for them is a comparison script that ran the bodies in a private R5900
interpreter against the executable, which is not committed, so there is no
test the integrator can run, and the snapshot checker rejects a record with no
host file. They are `auto-prop-save`, `goth-trap-passable`, `goth-trap-run`,
`hack-door-passable`, `hate-bridge-passable`, `hate-hole-run`,
`missile-trap-trigger`, `save-activate`, `shooter-save`,
`teleporter-set-active-chunks`, `teleporter-set-state` and `trigger-save`;
their sources stay in the snapshot, unselected. The notes' count of 29
closed imports therefore does not apply. No duplicate definition was found at
link time. No game binary or disassembly listing was committed. 1596
function bodies, 2031 authored inputs, zero COMMON. 214 tool tests passed.
No EE or game execution.

| | |
|---|---|
| Before | 1551 |
| After | 1562 |
| Closed | 17 |
| Opened | 28 |

Closed: `msg_run__11MissileTrap`, `msg_run__15PushPhysicsProp`, `msg_run__7Shooter`,
`msg_run__7Trigger`, `msg_run__8AutoProp`, `msg_run__8FireBomb`,
`msg_run__8HateGate`, `msg_run__8HateInto`, `msg_run__9Container`,
`msg_save__15PushPhysicsPropPc`, `msg_save__9ContainerPc`,
`msg_trigger__8AutoPropi`, `setPassableState__8AutoPropb`,
`setPassableState__8HateHoleb`, `setState__8AutoPropi`,
`sleepMonsters__8creaturev`, `wakeMonsters__8creaturev`.

Opened: `__11FireBombletG6Point3sPcT1P6Point3i`, `__13MissileWeaponiP12_modelHeaderP
8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`,
`__4GoldG6Point3i`, `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii`,
`bodyAddForce__FP5_bodyP6Point3N31`, `bodyApplyForce__FP5_bodyG6Point3T1f`,
`bodyApplyImpulse__FP5_bodyG6Point3T1f`,
`bodyGetVelocity__FP5_bodyP6Point3T1`, `gameDifficultyScale__Fv`,
`gamePositionVisible__FP6Point3f`, `gameTreasureRamp__Fv`,
`genericMove__10GameObjectG6Point3P10GameObject`,
`getProxyMeshId__4itemPQ24item11description`, `initStuff__6PhPropi`,
`makeExplosion__FG6Point3f`, `makeRandomPotion__4itemPQ24item11description`,
`matUntransform__FP6Point3P8Matrix33T0`,
`modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHe
aderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`,
`norrath_anttrap_msg_run_FireParticle_bytes`,
`norrath_anttrap_msg_run_HateGateParticle_bytes`,
`norrath_container_msg_run_fuseParticle_bytes`,
`norrath_container_msg_run_smokeParticle_bytes`,
`objectRadialDamage__FP10GameObjectG6Point3iffiii`, `soundStopSound__Fi`,
`worldCheckForWater__FP12_worldHeaderG6Point3fPf`,
`worldHitscan__FP12_worldHeaderG6Point3T1fi`,
`worldPerturbWater__FP12_worldHeaderG6Point3ff`.

## agents/w8-audio-calls

Merged (the branch notes carry the five summary sections). Thirty of the 32
units were selected with fresh `input_order` values 2031 through 2060 (30
emitted functions, 5696 compiled body bytes, not original-byte matches). All
30 host checks build and exit 0 here (30 of 30). The two data units
`sound_state.cpp` and `sound_state_extra.cpp` (19 data words) have no host
check, and the snapshot checker rejects a record without a host file, so
they stay in the snapshot, unselected; that is why `desiredMusicDuckAmount`
and `nextChannel` appear as opened and 17 of the notes' closed names remain
open. No duplicate definition was found at link time. No game binary or
disassembly listing was committed. 1626 function bodies, 2061 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1562 |
| After | 1540 |
| Closed | 26 |
| Opened | 4 |

Closed: `_GLOBAL_$D$soundSfxVolume`, `_GLOBAL_$I$soundSfxVolume`, `findChannel__Fv`,
`getStockPitch__FP10_vagHeader`, `soundChangeVolume__Fiff`,
`soundClearStreamQue__Fi`, `soundClear__Fi`, `soundContinueStream__Fi`,
`soundFindDialog__FPc`, `soundFlushCommandBuffer__Fv`,
`soundGetDialogFilename__Fv`, `soundGetDialogSize__FPc`,
`soundGetFreeRam__Fv`, `soundGetNmQueued__Fi`,
`soundGetStreamAmountPlayed__Fi`, `soundGetStreamPreloadDone__Fi`,
`soundInit__Ff`, `soundLock__Fi`,
`soundPlaySequence__FP15AnimationHeaderG6Point3f`, `soundRestore__Fb`,
`soundSetMusicDuckAmount__Ff`, `soundStartGlobalMusic__FPci`,
`soundStartStream__FiPciiiiii`, `soundStopSound__Fi`,
`soundUpdateAdjustments__Fv`, `transferVAG__FP10_vagHeaderi`.

Opened: `desiredMusicDuckAmount`, `nextChannel`, `sceSdTransToIOP`,
`soundSetAudioEffect__Fiiiii`.

## agents/w8-camera-vehicle-a

Merged (the branch notes carry the five summary sections). Twenty-five units
with fresh `input_order` values 2061 through 2085: 25 emitted functions (3900
compiled body bytes, not original-byte matches). All 25 host checks build and
exit 0 here (25 of 25). No duplicate definition was found at link time. No
game binary or disassembly listing was committed. 1651 function bodies, 2086
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1540 |
| After | 1537 |
| Closed | 25 |
| Opened | 22 |

Closed: `AddNPCDriver__7VehicleP3NPC`, `BuildMatrix__6CamerafffP6Point3`,
`GetNPCPosition__7VehicleP6Point3Ps`, `GetPitch__6Camera`,
`GetPlayerPosition__7VehicleiP6Point3Ps`,
`GetPositionAndYaw__7VehicleP6Point3T1Ps`, `GetYaw__6Camera`,
`Kick__7Vehicle`, `Push__6Camera`, `Reset__5Wheel`, `Reset__6Cameraii`,
`SetHold__7Vehiclei`, `SetMode__6Camerai`, `SetPitch__6Camerai`,
`SetYaw__6Camerai`, `Update__6Camera`, `_$_11LavaTractor`, `_$_7Vehicle`,
`_$_8SkulBoat`, `_GLOBAL_$I$__7VehicleG6Point3sPPci`, `__tf11LavaTractor`,
`__tf7Vehicle`, `__tf8SkulBoat`, `doMyUniqueForces__7Vehicle`,
`doMyUniqueForces__8SkulBoat`.

Opened: `GetZoomDistance__6Camera`, `SetZoomDistance__6Cameraf`, `_$_10GameObject`,
`__tf10GameObject`, `__tf11LavaVehicle`, `__ti10GameObject`,
`__ti11LavaVehicle`, `_vt$11LavaVehicle`, `_vt$7Vehicle`, `_vt$8SkulBoat`,
`accumForceAndTorque__7VehicleP6Point3T1i`, `blockFree__FPv`,
`doDebugCam__6Camera`, `doInnorukCam__6Camera`, `doNormalCam__6Camera`,
`getSeatPosition__7VehicleiP6Point3`, `hudDisplayNetCamToggle`,
`playerLocalPlayerBusy__Fv`, `removePassenger__7Vehiclei`,
`revealEntireMap__3mapv`, `vehicleCount`, `vehicleList`.

## agents/w8-camera-vehicle-b

Merged with two units removed (the branch notes carry the five summary
sections). Twenty-two units were selected with fresh `input_order` values
2086 through 2107 (22 emitted functions, not original-byte matches). All 22
host checks build and exit 0 here (22 of 22). The units `lava-set` and
`skul-set` (`set__11LavaTractor`, `set__8SkulBoat`) were **not** accepted:
each unit's `host/contract.cpp` does `#include "table.inc"`, and no
`table.inc` exists in the branch, so neither contract can be built (and the
snapshot checker, which follows includes, fails on them). They have no
runnable test, so the integrator deleted both unit directories in the merge
commit's follow-up commit rather than recording files that cannot build; the
branch history still has them, and a re-merge needs the missing table file
committed. This is why the notes' count (24) does not apply. No duplicate
definition was found at link time. No game binary or disassembly listing was
committed. 1673 function bodies, 2108 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1537 |
| After | 1541 |
| Closed | 22 |
| Opened | 26 |

Closed: `eventActivate__11LavaTractor`, `eventActivate__7Vehicle`,
`eventCollisionGround__11LavaTractor`, `eventCollisionGround__7Vehicle`,
`eventCollisionWall__11LavaTractorP6Point3T1`,
`eventCollisionWall__7VehicleP6Point3T1`, `eventCurRPM__11LavaTractor`,
`eventCurRPM__7Vehicle`, `eventDeActivate__11LavaTractor`,
`eventDeActivate__7Vehicle`, `eventGasOff__11LavaTractor`,
`eventGasOff__7Vehicle`, `eventGasOn__11LavaTractor`,
`eventGasOn__7Vehicle`, `eventSteeringChange__11LavaTractor`,
`eventSteeringChange__7Vehicle`, `msg_collision__7VehicleP10GameObject`,
`msg_draw__7Vehicle`, `msg_getPokeName__7VehiclePUsi`,
`msg_poke__7Vehiclei`, `msg_run__7Vehicle`, `refreshSettings__7Vehicle`.

Opened: `addPassenger__7Vehiclei`, `backup__7Vehicle`,
`buildCollisionInfo__7Vehicle`, `checkVehicleCollision__7Vehicle`,
`checkWallCollision__7Vehicle`, `doAI__7Vehicle`, `doBrakes__7Vehicle`,
`doDriveline__7Vehicle`, `doMisc__7Vehicle`, `doSteering__7Vehicle`,
`doSuspension__7Vehicle`, `doThrottle__7Vehicle`, `dpcmp`, `dpsub`,
`finalizePosition__7Vehicle`, `fptodp`, `gameButtonToIcon__Fi`,
`gameLineUpdatePosition__FP17gameLinePathState`,
`getNewRotAndPos__7VehicleP6Point3T1fii`, `haveRoom__7Vehicle`,
`hudInfoMessageShowing__Fv`, `isPassenger__7Vehiclei`,
`objectIsOffGrid__FP10GameObject`, `soundChangeSequenceVolume__Fif`,
`soundMakePersistant__Fi`, `startForces__7Vehicle`.

## agents/w8-core-loop-e

Merged (the branch notes carry the five summary sections). Thirty units
with fresh `input_order` values 2108 through 2137: 20 emitted functions
(14,164 compiled body bytes, not original-byte matches) and data words. All
30 host checks build and exit 0 here (30 of 30). No duplicate definition was
found at link time. No game binary or disassembly listing was committed.
1693 function bodies, 2138 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1541 |
| After | 1560 |
| Closed | 30 |
| Opened | 49 |

Closed: `retval`, `runAmbient__Fv`, `runAnimation__Fv`, `runMusicDucking__Fv`,
`runMusic__Fv`, `runScreaming__Fv`, `runSequences__Fv`,
`scriptActivatePostAction__Fv`, `scriptClearAllGameState__Fv`,
`scriptClear__Fv`, `scriptRun__Fv`, `scriptSetScript__FPv`,
`scriptSuperClear__Fv`, `scriptTask__Fv`, `sentEndMsg`,
`setExtraParams__FP8CreaturePc`, `setup_main__Fv`, `showCoords__Fv`,
`stack.15`, `startEntranceName`, `startNewWorld__Fv`,
`station_main__FiPCcPcT2T1T1i`, `statusPtr`, `subDrawLoop__FPFv_i`,
`superGamestateInit`, `switchToRun__3Cat`, `swooping`, `trippleBufferMode`,
`waitForSound`, `worldDifficulty`.

Opened: `Free__7VISetup`, `Free__9VIStation`, `Init__7VISetupi`,
`Init__9VIStationiPCcPcT3T2T2i`, `MC_CheckPlayerImportOrChange__Fv`,
`MC_SetActByLevelName__FPc`, `Process__7VISetup`, `Process__9VIStation`,
`RestartConnect__7VISetup`, `RunPatcher__7VISetup`, `_$_7VISetup`,
`_$_9VIStation`, `__6PlayerG6Point3sii`, `__7VISetup`, `__9VIStation`,
`__builtin_new`, `ambientGroup`,
`animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi`,
`animFindComponentWithFlags__FP14AnimationStatei`,
`animGetVolumeScale__Fii`, `cdCheckForFileInCurrentLayer__FPc`,
`cdLoadAsyncIOP__FPciT0ii`, `currActivityScore`, `currScorePos`,
`dialogLumpName`, `gameLoadWorld__FPc`,
`getMegatileOriginalPosition__FG6Point3`, `gotPlayerStartThisLevel`,
`initEzNetCnf`, `lastMusicState`, `megatileRot`, `megatileSeed`,
`nmAmbientGroups`, `nmDeferedCalls`, `norrath_music_activity_ring_view`,
`numEnvironments`, `playerSelectionEnforce__Fv`, `playerStartYaw`,
`recallPos`, `runStreamQueing__Fv`, `sceSifMExitRpc`, `soundAllFlags__Fi`,
`soundDisable__Ff`, `soundGetGlobalDialogDir__Fv`,
`soundStartDialogStream__Fiii`, `startScorePos`, `timeLastTransition`,
`unet_shutdown__Fv`, `worldFindHeight__FP12_worldHeaderG6Point3f`.

## agents/w8-fx-particles

Merged (the branch notes carry the five summary sections). Thirty-eight units
with fresh `input_order` values 2138 through 2175: 38 emitted functions
(18,088 compiled body bytes, not original-byte matches). All 38 host checks
build and exit 0 here (38 of 38). No duplicate definition was found at link
time. No game binary or disassembly listing was committed. 1731 function
bodies, 2176 authored inputs, zero COMMON. 214 tool tests passed. No EE or
game execution.

| | |
|---|---|
| Before | 1560 |
| After | 1538 |
| Closed | 40 |
| Opened | 18 |

Closed: `DynamicLightArray`, `P_DeleteParticleSystem__Fi`,
`P_DrawParticles__FP8Matrix34`, `P_Pop__Fv`, `ParticleEmitterArray`,
`_GLOBAL_$D$orc1ResourceMap`, `_GLOBAL_$D$wesResourceMap`,
`_GLOBAL_$D$wraithResourceMap`, `_GLOBAL_$I$antResourceMap`,
`_GLOBAL_$I$cloudgiantResourceMap`, `_GLOBAL_$I$cyclopsResourceMap`,
`_GLOBAL_$I$fireBeetleResourceMap`, `_GLOBAL_$I$fxResourceMap`,
`_GLOBAL_$I$goblinResourceMap`,
`_GLOBAL_$I$hasLightOrParticleNumber__FPPci`,
`_GLOBAL_$I$innoruukResourceMap`, `_GLOBAL_$I$lavaMonsterResourceMap`,
`_GLOBAL_$I$mummyResourceMap`, `_GLOBAL_$I$orc1ResourceMap`,
`_GLOBAL_$I$particlesPaused`, `_GLOBAL_$I$scriptParticles`,
`_GLOBAL_$I$soulResourceMap`, `_GLOBAL_$I$spiderResourceMap`,
`_GLOBAL_$I$udkResourceMap`, `_GLOBAL_$I$vampireResourceMap`,
`_GLOBAL_$I$wesResourceMap`, `_GLOBAL_$I$wraithResourceMap`,
`__11AddParticleG6Point3PPc`,
`__11LightEffectG6Point3ffffffffffffP10GameObjecti`,
`__11ParticleDefiiiiiiiifffiiiPFPv_v`,
`__19ParticleMorphEffectP8CreatureT1`,
`__eq__19ParticleEmitterDataRC19ParticleEmitterData`, `fxClear__Fv`,
`lightClear__Fv`, `lightDeactivate__FP5Light`,
`lightPushSceneLights__FG6Point3iiiiiiiii`, `nmParticleEmitterRecs`,
`setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v`,
`updatePS__FP14ParticleSystem`, `updateTrails__Fv`.

Opened: `_14WoodElfSoldier$m_commanderList`, `_3Orc$m_commanderList`,
`_6Wraith$s_rWraithList`, `__11ParticleDef`, `_vt$11AddParticle`,
`_vt$11LightEffect`, `_vt$19ParticleMorphEffect`,
`drawDistortionParticles__Fv`, `drawLines__Fv`,
`drawPS__FR6PacketP14ParticleSystem`, `drawSingleParticles__Fi`,
`drawTrails__Fv`, `lightInit__11AddParticlePPc`,
`matMakeViewMatrix__FP8Matrix44fffffff`, `matMul__FP8Matrix34N20`,
`matMul__FP8Matrix44P8Matrix34T0`, `particleInit__11AddParticlePPc`,
`savedScene`.

## agents/w8-hud-text-items-a

Merged (the branch notes carry the five summary sections). Fifteen of the 16
units were selected with fresh `input_order` values 2176 through 2190 (22
emitted functions, 6516 compiled body bytes, not original-byte matches). All
16 host checks build and exit 0 here (16 of 16). `boss-meter-words` was **not**
selected: it defines `hudNoDraw`, already defined by the game-loop package's
`hud_no_draw.cpp`, so the link fails with a multiple definition; its files
stay in the snapshot, unselected, and `bossMeterCurrent`, `bossMeterMax` and
`bossMeterAlpha` stay open. No game binary or disassembly listing was
committed. 1753 function bodies, 2191 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1538 |
| After | 1526 |
| Closed | 25 |
| Opened | 13 |

Closed: `AdvanceDialogSwoop__6CameraiPf`, `DLG_Clear__Fv`, `DLG_NewWorld`,
`DLG_SetMutex__Fv`, `StartDialogSwoop__6CameraP10GameObjecti`,
`_GLOBAL_$D$displayQuestInfo__FP11questStructi`,
`_GLOBAL_$D$inventoryLoad__Fv`, `_GLOBAL_$D$shopActiveFrame`,
`_GLOBAL_$I$_fontStringSize__FP4FontPci`,
`_GLOBAL_$I$displayQuestInfo__FP11questStructi`,
`_GLOBAL_$I$inventoryLoad__Fv`, `_GLOBAL_$I$shopActiveFrame`,
`addItemToInventory__FiPQ24item11descriptioni`, `cacheClear__Fv`,
`colorScale__FUif`, `dialogCreature`, `dialogIdleAnimation`, `dialogMutex`,
`dlgArrayPtr`, `dlgName`, `dlgPos`, `dropItem__4itemG6Point3Pc`,
`equipItem__FiPQ24item11descriptioni`, `fontColor__FUiii`, `shopList`.

Opened: `distanceToPlane__FP6Point3N20`, `fontCharAlpha`,
`inventoryCheckWieldingStatus__FiPQ24item11description`,
`inventorySetAnim__FiP14AnimationStatei`, `lastFontCharAlpha`,
`lastFontColor`, `lerpShortRot__Fssfff`, `multiHudAddHelpMessage__FiPUsT1i`,
`nmChars`, `nmDirectQWords`, `p`, `playerGetPotionsLeft__Fii`,
`reglistLoopCount`.

## agents/w8-hud-text-items-b

Merged (the branch notes carry the five summary sections). Twenty-eight units
with fresh `input_order` values 2191 through 2218: 25 emitted functions (6888
compiled body bytes, not original-byte matches). All 28 host checks build and
exit 0 here (28 of 28). No duplicate definition was found at link time. The
official count is unchanged at 1526 because 30 names closed and 30 opened
(the font and inventory-load routines these functions call). No game binary
or disassembly listing was committed. 1778 function bodies, 2219 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1526 |
| After | 1526 |
| Closed | 30 |
| Opened | 30 |

Closed: `fontEnd__Fv`, `fontLastPrintedPosition`, `fontPrintCentered__FiiPci`,
`fontPrintCenteredu__FiiPUsi`, `fontPrintWrapu__FiiPUsiiii`,
`fontPrint__FiiPci`, `fontPrintu__FiiPUsi`, `fontRestrict__Fi`,
`fontScissor__Fiiii`, `fontStart__FP4FontiUii`,
`fontStringSizeu__FP4FontPUsi`, `getWeight__4itemPQ24item11description`,
`hudAssignQuickSpell__Fiii`, `hudAssignSpellBookSpell__Fiiil`,
`hudClearInfoMessage__Fv`, `hudFadeScreen__Fi`, `hudFinishGold__Fv`,
`hudFooterUni`, `hudKick__Fv`, `hudResetFadeScreen__Fi`,
`hudShowMsg__FPUsN20iii`, `infoMessage`, `infoMessageAlways`,
`infoMessageTimer`, `invShutDown__Fi`, `inventoryInit__Fv`,
`inventoryReset__Fv`, `msgHudPlayerExit__Fv`, `msgHudRecall__Fc`,
`picProcess__4itemv`.

Opened: `_4item$picBuffer`, `_4item$picInProgress`, `_4item$picList`,
`_4item$picLoadBuffer`, `_4item$picLoadFinished`, `_4item$picsToLoad`,
`_fontPrintu__FiiPUsi`, `_fontStringSize__FP4FontPci`,
`_fontStringSizeu__FP4FontPUsi`, `countUp`, `curFont`, `drawBucket`,
`fadeDownVal`, `givePlayerFeat__Fxii`, `goldAlpha`, `goldNumber`,
`inventoryFrameDraw__Fv`, `inventoryFrameRun__Fv`, `inventoryLoadState`,
`inventoryLoad__Fv`, `inventoryLump`, `lightPopSceneLights__Fv`,
`picFind__4itemPc`, `picKickOneOut__4itemv`, `playerGetSkillRank__Fii`,
`playerGettinIt`, `restrictChars`, `scissored`, `shopGrid`,
`vifDirectQWordCount`.

## agents/w8-hud-text-items-c

Merged (the branch notes carry the five summary sections; their plan table
gives hash prefixes, so each full hash was taken from the file and checked
against the prefix). Twenty-one units with fresh `input_order` values 2219
through 2239: 20 emitted functions (7096 compiled body bytes, not
original-byte matches) and a data unit. All 21 host checks build and exit 0
here (21 of 21). No duplicate definition was found at link time. No game
binary or disassembly listing was committed. 1798 function bodies, 2240
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1526 |
| After | 1516 |
| Closed | 30 |
| Opened | 20 |

Closed: `realDlgPos`, `rectangleDraw__FiiiiiUi`, `removeAllQuestsFromLog__Fv`,
`shopActiveFrame`, `shopKick__Fv`, `textAddText__FPv`, `textBody`,
`textBodyChars`, `textGetString__FPc`, `textInit__Fv`,
`textMessageDraw__Fv`, `textMessageProcess__Fv`, `textPageDone`,
`textureCacheIn__FP8_texture`, `textureClear__Fv`,
`textureDrawBorder__FiiiiiUiPcii`, `textureLock__FP8_texture`,
`texturePageBlock`, `texturePageLoc`, `texturePageSegLoc`,
`texturePageTable`, `textureSmoothBorder__FiiiiiUiiii`,
`textureUncompBufferLocked`, `textureUnlock__FP8_texture`,
`usprintf__FPUsT0ffffffff`, `ustrcat__FPUsT0`, `ustrcpy__FPUsPc`,
`ustrcpy__FPUsT0`, `ustrcpy__FPcPUs`, `ustrlen__FPUs`.

Opened: `TerminateThread`, `alwaysLoadedPage`, `decodeQHead`, `decodeQTail`,
`drawUpFrom__FiiPCUcT2iUi`, `hudNetWaitMsgReady__Fv`, `inventoryUnload__Fv`,
`keyboardGetNextKey__Fv`, `keyboardGetNextTranslatedKey__Fv`,
`loadInProgress`, `nmAlwaysPages`, `norrath_text_hash_slots`,
`norrath_text_missing_cursor`, `norrath_text_missing_entries`, `sellMode`,
`texDecodeStarted`, `textSend__Fv`,
`textureDrawSuperColor__FP8_textureiiiUiiiiicc`, `texturesAreDecompressing`,
`texturesAreLoading`.

## agents/w8-net-play-c

Merged (the branch notes carry the five summary sections). Twelve of the 14
units were selected with fresh `input_order` values 2240 through 2251 (13
emitted functions, 8540 compiled body bytes, not original-byte matches).
Seven host checks build and run directly; five (`player-damage-handler`,
`player-recompute-ability-modifiers`, `player-recompute-weight`,
`pre-cache-armor`, `start-packet`) map dialog data at the fixed address
0x20000000 and only work in the build their header comment prescribes
(`-arch x86_64 -Wl,-pagezero_size,0x10000`); built that way, all five exit 0
here, so all 12 pass. Two units were **not** selected because the link
reported multiple definitions: `alive-sync-state` (`playersAlive`,
`syncRandState`) and `player-select-state` (`playerMenu`) are already defined
by earlier packages; their files stay in the snapshot, unselected. No game
binary or disassembly listing was committed. 1811 function bodies, 2252
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1516 |
| After | 1533 |
| Closed | 12 |
| Opened | 29 |

Closed: `playerDamageHandler__6Player`, `playerDeallocateArmorCaches__Fv`,
`playerGetFeatRank__Fix`, `playerGlobalInit__Fv`, `playerPlaySound__Fii`,
`playerRecomputeAbilityModifiers__Fv`, `playerRecomputeWeight__Fi`,
`playerSelectionRunning__Fv`, `preCacheArmor__6Player`,
`recievePacket__FiPUc`, `startNetLaunch__Fv`, `startPacket__Fi`.

Opened: `ClericSkillTree`, `RangerSkillTree`, `ShadowKnightSkillTree`,
`WarriorSkillTree`, `WizardSkillTree`, `_4item$globalItem`,
`__8GutChunkG6Point3iif`, `cacheCdDone`,
`cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture`,
`cacheLoadCache`, `cheatInvulnerability`, `cheatMode`, `curDlgStart`, `disin
tegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi`
, `eqLumpHeader`, `gSkillTree`, `getPicName__4itemPciPQ24item11description`,
`lastYAnchor`, `netForceLocalUpdate`,
`norrath_player_armor_cache_view_7cb770`, `packetbuffer`, `packetshift`,
`playSoundForDlg__FP9DlgPacket`, `playerIsAnimPlaying__6PlayerPc`,
`recomputePaperDollModifiers__Fi`, `soundReallyAllFlags__Fi`,
`soundStartGlobalDialogStreamRandom__FiPci`,
`soundStartGlobalDialogStream__FiPci`, `wLumpHeader`.

## agents/w8-object-list-math

Merged (the branch notes carry the five summary sections). Forty-two units
with fresh `input_order` values 2252 through 2293: 45 emitted functions
(7808 compiled body bytes, not original-byte matches, including file-local
helpers). All 42 `host_check.cpp` programs build and exit 0 here (42 of 42),
with two build notes: six checks need the unit linked with sibling units of
the package that define their callees (`object-clear`, `find-object-by-class`,
`mat-make-trans-yaw`, `mat-to-quaternion`, `safe-iterator-next` and
`safe-iterator-destroy`), and `mat-rot33` compares floats bit for bit, so it
only passes with `-ffp-contract=off` (on this arm64 host the default fused
multiply-add changes the last bit; the EE has no fused form). No duplicate
definition was found at link time. No game binary or disassembly listing was
committed. 1856 function bodies, 2294 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1533 |
| After | 1491 |
| Closed | 44 |
| Opened | 2 |

Closed: `_$_16ListSafeIterator`, `_GLOBAL_$D$lastkey`,
`_GLOBAL_$D$objectIgnoreFlags`, `_GLOBAL_$I$addpmlist__FPc`,
`_GLOBAL_$I$lastkey`, `_GLOBAL_$I$objectIgnoreFlags`,
`__16ListSafeIteratorP8ListHeadi`, `createByName__FPcG6Point3sPPc`,
`deleteList`, `iatan2__Fff`, `icos__Fi`, `initializeMe__4Line`, `isin__Fi`,
`lineToY__Fi`, `listOnewayLink__FP8ListHeadT0Pv`, `matIdent__FP8Matrix33`,
`matMakeTransYaw__FP8Matrix34G6Point3s`, `matRot__FP8Matrix33is`,
`matToQuaternion__FP4QuatP8Matrix34`,
`matTransform34Vec__FP6Point3P8Matrix34T0`,
`matTransform__FP6Point3P8Matrix33T0`, `mediumRunList`, `memAlloc__FiPc`,
`memCheckHeapReal__FPCci`, `memFree__FPv`, `memRealloc__FPvi`, `nextIndex`,
`next__16ListSafeIterator`, `objectActivationCheck__8Creaturei`,
`objectAddToDeleteList__FP10GameObject`, `objectClear__Fv`,
`objectDoMyActions__FP10GameObjecti`,
`objectFindAnimation__FP10GameObjectPc`,
`objectFindObjectByClass__FPcPP10GameObjecti`,
`objectInside__7TriggerP10GameObject`,
`objectRemoveFromGrid__FP10GameObject`,
`objectRemoveFromRunList__FP10GameObject`,
`objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`,
`objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`,
`objectSetFilterFunction__FPFPc_i`, `quatToMat__FP8Matrix33P4Quat`,
`runList`, `slowRunList`.

Opened: `_$_10VIKeyboard`, `__10VIKeyboard`.

## agents/w8-render-world-b

Merged (the branch notes carry the five summary sections). Twenty-two units
with fresh `input_order` values 2294 through 2315: 20 emitted functions
(11,584 compiled body bytes, not original-byte matches) and data units. All
22 host checks build and exit 0 here (22 of 22). No duplicate definition was
found at link time. No game binary or disassembly listing was committed.
1876 function bodies, 2316 authored inputs, zero COMMON. 214 tool tests
passed. No EE or game execution.

| | |
|---|---|
| Before | 1491 |
| After | 1511 |
| Closed | 32 |
| Opened | 52 |

Closed: `megatile_ramkmax`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix3
4P14AnimationStateUlP11_drawRecordi`,
`modelGetActiveChunks__FP12_modelHeaderi`, `modelResetOptionalParams__Fv`,
`modelSetShadowBrightness__Ff`, `nmLumps`, `segmentUsed`,
`texDecompressThread__FPv`, `texIsDecompressing__Fv`, `texIsLoading__Fv`,
`texProcessDecompress__Fv`, `texProcessLoad__Fi`, `texdecompress`, `texmap`,
`texmapend`, `texmapstart`, `uncompMap`, `uncompSpin`, `world`,
`worldAllocateWaterPatches__Fi`, `worldClear__Fv`,
`worldClipCubeToClipFrustum__FPA1_fi`, `worldDrawEnable`,
`worldDrawWorld__FP12_worldHeader`,
`worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi`,
`worldMoveDoor__FP12_worldHeaderiG6Point3s`, `worldReset__FP12_worldHeader`,
`worldRunWaterTask__Fv`, `worldSetDoorCollision__FP12_worldHeaderii`,
`worldSetDoorHeight__FP12_worldHeaderif`, `worldSetViewCenter__Fv`,
`worldViewCenter`.

Opened: `SleepThread`,
`animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps`,
`animLoadBoneMatricies__FP6PacketP14AnimationState`, `cdEndStream__Fv`,
`cdIsBusyStreamingNonwithstanding__Fv`, `cdStartStream__FPci`,
`cdStreamLoadAsync__FPcT0i`, `clearing`, `clipFrustumNormalSign`, `decodeQ`,
`defaultHalos`, `drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi`,
`dumpMem__FPvi`, `globalShadowBrightness`, `halos`, `loadCompleteFlag`,
`loadKillList`, `loadKillListSize`, `loadPage`, `loadSegmentsLoaded`,
`loadSlot`, `modelDrawFade`,
`modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi`, `modelOption`,
`modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34`,
`modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc`,
`modelShadowScale`, `nmHalos`, `nmWaterPatches`,
`putBlock__FP8_textureP6PacketPPP9uncompMapPPii`, `reallyLoad.873`,
`routeClearStart`, `routeEnable`, `rqHead`, `rqTail`, `scx1`, `scx2`,
`scy1`, `scy2`, `shadowBrightness`, `texCacheData__FP8_texturei`,
`texLastDecompressionTime`, `textureFreeTexture__FP8_texture`,
`textureInitTexture__FP12_worldHeaderP8_texturePUcPc`,
`vqDecompress__FP8_texturei`, `waterInit__Fv`, `waterPatch`,
`worldAnimation`, `worldClipFrustum`, `worldLastDrawSucceded`,
`worldRunWater__FP12_worldHeader`, `worldWaterRainTimer`.

## agents/w8-spell-static-init-a

Merged (the branch notes carry the five summary sections). Thirty-one units
with fresh `input_order` values 2316 through 2346: 31 emitted functions
(10,056 compiled body bytes, not original-byte matches). All 31 host checks
build and exit 0 here (31 of 31). No duplicate definition was found at link
time, and the package opened no new import. No game binary or disassembly
listing was committed. 1907 function bodies, 2347 authored inputs, zero
COMMON. 214 tool tests passed. No EE or game execution. (Its sibling
`agents/w8-spell-static-init-b` has no host checks and is not merged; see its
entry above.)

| | |
|---|---|
| Before | 1511 |
| After | 1478 |
| Closed | 33 |
| Opened | 0 |

Closed: `_GLOBAL_$I$__10HolyStrikesiG6Point3P10GameObjectP5Skill`,
`_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`,
`_GLOBAL_$I$__10StaticFireG6Point3fif`,
`_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`,
`_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill`,
`_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill`,
`_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill`,
`_GLOBAL_$I$__15SpellConeOfFireiP8CreatureP5Skill`,
`_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill`,
`_GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill`,
`_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill`,
`_GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill`,
`_GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill`,
`_GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill`,
`_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill`,
`_GLOBAL_$I$__19BlindingLightEffectG6Point3iP8CreatureP5Skill`,
`_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`_GLOBAL_$I$__22SpellPlanarSteelEffectiP8CreatureP5Skill`,
`_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc`, `_GLOBAL_$I$__5TorchG6Point3PPci`,
`_GLOBAL_$I$__6SwitchG6Point3PPc`, `_GLOBAL_$I$__8FireBombG6Point3sPPc`,
`_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill`,
`_GLOBAL_$I$__8JumpGateG6Point3sPPc`,
`_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2`,
`_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature`,
`_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s`,
`_GLOBAL_$I$cast__13SkillEntangleiP8CreatureG6Point3s`,
`_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s`,
`_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s`,
`_GLOBAL_$I$cast__18SkillRepulseUndeadiP8CreatureG6Point3s`,
`teleportParticle1`, `teleportParticle2`.

Opened: none

## agents/w8-net-play-b

Merged (the branch notes carry the five summary sections). Nineteen of the 20
units were selected with fresh `input_order` values 2347 through 2365 (16
emitted functions, 20,208 compiled body bytes, not original-byte matches).
All 19 host checks pass here (19 of 19); `init` and `kick-player` only build
as their header comment says, with `-fno-builtin` and the host check alone.
`launch-state` was **not** selected: it defines `nmPlayersAlive`, already
defined by w8-net-play-a, so the link fails with a multiple definition; its
files stay in the snapshot, unselected, and `netBroadCastStart`,
`netAttemptLaunch`, `netTrouble` and `multiHelpMessageOn` stay open. No game
binary or disassembly listing was committed. 1923 function bodies, 2366
authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1478 |
| After | 1518 |
| Closed | 19 |
| Opened | 59 |

Closed: `netClearContact__Fv`, `netDisconnectPlayer__Fi`, `netDoNewLaunch__Fv`,
`netGameStep__FbT0`, `netInContact__Fv`, `netInit__Fv`, `netKickPlayer__Fi`,
`netPlayerIsConnected__Fi`, `netRestoreButtonConfigs__Fv`,
`netStartLevel__Fv`, `netStartNetworkGameFrame__Fv`,
`netStartNetworkGameInit__Fv`, `netStart__Fv`,
`netWaitForPeersToCatchUp__Fv`, `norrath_player_heap_pointer_word`,
`norrath_player_proxy_mesh_id`, `norrath_player_services_view_48068c`,
`player`, `playerClearSpellEffect__Fv`.

Opened: `MC_BuildPackedPlayer__FP11playerEntryi`, `MC_ConvertTempPlayerToReal__Fi`,
`MC_GetRamSlot__FPi`, `MC_NetPack__Fv`, `MC_NetUnPack__Fv`,
`MC_ReplicateFirstPlayersImportantThings__Fv`, `MC_SetRamNmPlayers__Fi`,
`advanceRealState__Fi`,
`analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci`,
`broadcastConnectionFlags`, `compressSize`, `currentTime`, `derivedFrameNm`,
`hostForcedLaunch`, `hostLaunchMask`, `hostStartedWithoutMe`, `isModem`,
`kickState`, `lastPingTime`, `launchPeers`, `lightInc`, `lightIntensity`,
`lnet_buf`, `localNetFrameNm`, `localNetSentFrameNm`, `localPlayerPadState`,
`localPredictedPlayer`, `logChecksum`, `msgBufferSent`, `myaddr`,
`netCameraYaw`, `netKickedOut`, `netLaunchExpirationCount`,
`netLaunchTimer`, `netLocalPadState`, `netNewLaunchMaybeLaunch`,
`netNewLaunchMaybeQuit`, `noAdvanceTime`, `packetSendCycleLength`,
`packetSendCycleTime`, `peerSendDataMsg__FPciiP14sceInetAddressi`,
`playerSpecStorage`, `receivePackets__Fv`, `runPrediction__Fi`,
`sceInetCreate`, `sceInetInterfaceControl`, `sceInetOpen`,
`sceInetRecvFrom`, `sceInetSendTo`, `sceSifMInitRpc`,
`sendControlPacket__FiiiiiR9CCompressb`, `sentStatePacketFrameNm`,
`showNetPlayerStatus__Fv`, `spellAlpha`, `spellAlphaInc`,
`totallyDisconnected`, `trueNetFrameNm`, `udpLaunchConnection`,
`worldPlotRouteSetEnable__Fi`.

## agents/w8-render-world-a

Merged (the branch notes carry the five summary sections). Twenty-five units
with fresh `input_order` values 2366 through 2390: 27 emitted functions
(11,796 compiled body bytes, not original-byte matches, including file-local
helpers). All 25 host checks build and exit 0 here (25 of 25). No duplicate
definition was found at link time. No game binary or disassembly listing was
committed. 1950 function bodies, 2391 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1518 |
| After | 1514 |
| Closed | 30 |
| Opened | 26 |

Closed: `_GLOBAL_$D$worldTileEntityLightScale`, `_GLOBAL_$I$edgearray`,
`_GLOBAL_$I$modelClippingOn`, `_GLOBAL_$I$worldTileEntityLightScale`,
`_textureDrawColor__FP8_textureiiiUi`, `blockAllocInit__Fv`,
`blockAlloc__Fi`, `blockGetNmFree__Fv`, `cache`, `cacheHead`, `cacheTail`,
`checkBlock`, `checkCanAllocDescriptor__Fv`,
`creatureAdvanceAnimation__8Creaturef`, `decompressStack`,
`draw__8CreatureP12_modelHeaderP8_textureT1`, `evalHermite__Ffffff`,
`everyFrame__8CreatureP12_modelHeader`, `floorClear__Fv`,
`getMegatileCenter__FG6Point2`, `getMegatileOriginalCenter__FG6Point2`,
`lastNmBlocksFree`, `lastNmQ.1088`, `loadDirectory__FP9directoryii`,
`loadSampleRate.784`, `lumpCheck__Fv`, `lumpFindByType__FPvPci`,
`lumpQuery__FPc`, `lumpUnload__FPv`, `megatileInit__FP12MegatileRootPv`.

Opened: `ParticleEmitterTestParticle`, `_$_5Light`, `bossMeterName`,
`drawTexture__FR6PacketP8_textureiiiiii`, `droplet`, `ef1Noise__Fi`,
`freeLump__FPv`, `fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Poin
t3sP15FxCreatureStateffff`, `fxRunCreatureGlow__FP10GameObjectP12_modelHeade
rP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi`, `fxRunCreatureShoc
k__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi`,
`gameLineGetVert__Fii`, `gameLineGet__FPc`,
`getClosestLivingPlayer__FG6Point3f`, `initPosition__FP17gameLinePathState`,
`isBoss__FP10GameObject`, `lightSetColorScale__Ffff`,
`lightSetPrimaryDirectional__FG6Point3Ps`,
`lightSetSecondaryDirectional__FG6Point3Ps`,
`megatileBankInit__FP16MegatileInstBankPv`,
`megatileItemInit__FP8MegatilePv`, `mtcInitialize__Fv`,
`netCheckActivation__FG6Point3f`, `rainSplashParticle`, `routeQueue`,
`textRemoveText__FPv`, `worldFreeWorld__FP12_worldHeader`.

## agents/w9-startup-crt-a

Merged (the branch notes carry the five summary sections). Twenty-two units with
fresh `input_order` values 2393 through 2414: 18 function units and four data
units (commander lists, item global table, item picture state, map data) that
emit no function. Sources are authored C++/assembly; no game binary or
disassembly listing was committed. Host contracts: 20 of 22 run here. The
`add-particle-ctor-numbered` check fails to link on this machine only because
macOS prefixes the `__asm__` label of `_vt$11AddParticle` with an underscore (a
platform artifact, not a source defect). The `terminate-thread` check compares
against the pinned executable and was not run. The data units' only comparison
is `docs/agents/w9-startup-crt-a-check.py`, which needs the private executable
and was not run; the item-name table is an identifier table, not dialogue.
The worker's claimed closed list reproduces (32 closed); 17 opened. 1968
function bodies, 2413 authored inputs, zero COMMON. 214 tool tests passed. No EE
or game execution.

| | |
|---|---|
| Before | 1514 |
| After | 1499 |
| Closed | 32 |
| Opened | 17 |

Closed: `AllocTimerCounter`, `CallRpc`, `InitTLB`, `InvalidDCache`, `MemcpyS`,
`SetTimerHandler`, `StartTimerCounter`, `TerminateThread`,
`UnpreparePowerOff__Fv`, `_14WoodElfSoldier$m_commanderList`,
`_3Orc$m_commanderList`, `_3map$mapOffset`, `_3map$mapheight`,
`_3map$maptiles`, `_3map$mapwidth`, `_4item$globalItem`, `_4item$picBuffer`,
`_4item$picInProgress`, `_4item$picList`, `_4item$picLoadBuffer`,
`_4item$picLoadFinished`, `_4item$picsToLoad`, `_6Wraith$s_rWraithList`,
`_GLOBAL_$I$hudNoDraw`, `__11AddParticleG6Point3PPci`,
`__11FireBombletG6Point3sPcT1P6Point3i`, `__11ParticleDef`,
`__12RecallEffectG6Point3`,
`__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`,
`__14AnimationStateRC14AnimationState`, `__3NPCG6Point3sPcN33ifPPc`,
`__3Webii`

Opened: `GetMemorySize`, `InitTLB32MB`, `P_ExtendTrail__FiG6Point3`,
`P_StartTrail__Fiifiiiii`, `_InitTLB`, `__15GlowingCylinderG6Point3iiii`,
`_sceIDC`, `_vt$10Projectile`, `_vt$11FireBomblet`, `_vt$12RecallEffect`,
`_vt$13MissileWeapon`, `_vt$3NPC`, `iAllocTimerCounter`, `iSetTimerHandler`,
`iStartTimerCounter`, `playedStinger`, `sceSifMCallRpc`

## agents/w9-startup-crt-c

Merged (the branch notes carry the five summary sections). Twenty-seven units
with fresh `input_order` values 2415 through 2441, using the unique object
names the notes asked for (three `typeinfo.cpp` units and `font_string_sizeu`
would otherwise collide). Six virtual-table units and `impure_data` emit no
function. No game binary or disassembly listing was committed. 29 host checks:
28 run as is here; `k-exit-tlb-check` needs its two path arguments and passes
with them (61 instructions). Both of its checks, and `ctype-check`, guard units
that were already in the snapshot but unselected (`_ctype_`,
`_kExitTLBHandler`); I did not select those units in this pass. The worker made
no full-build claim; the real delta is above. 1988 function bodies, 2440
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1499 |
| After | 1496 |
| Closed | 27 |
| Opened | 24 |

Closed: `__tf11LavaVehicle`, `__tf9MummyKing`, `__tf9Savepoint`,
`__throw_bad_typeid`, `__ti11LavaVehicle`, `__udivdi3`, `__umoddi3`,
`_alarm_bind`, `_alarm_rdata`, `_exit`, `_fontPrintu__FiiPUsi`,
`_fontStringSize__FP4FontPci`, `_fontStringSizeu__FP4FontPUsi`,
`_fs_version`, `_impure_ptr`, `_sceFsSigSema`, `_sceFsWaitS`,
`_sceRpcGetFPacket`, `_sceRpcGetFPacket2`, `_sceSifSendCmd`,
`_search_svdata`, `_vt$11AddParticle`, `_vt$11LavaVehicle`,
`_vt$11LightEffect`, `_vt$19ParticleMorphEffect`, `_vt$7Vehicle`,
`_vt$8SkulBoat`

Opened: `Exit`, `_$_10bad_typeid`, `_$_11AddParticle`, `_$_11LavaVehicle`,
`_$_11LightEffect`, `_$_19ParticleMorphEffect`, `__cp_push_exception`,
`__eh_alloc`, `__ps2_klibinfo__`, `__tf10bad_typeid`, `__tf11AddParticle`,
`__tf11LightEffect`, `__tf19ParticleMorphEffect`, `__throw`,
`_vt$10bad_typeid`, `clone__11LightEffect`, `debugReport__11AddParticle`,
`draw__11AddParticleG6Point3P6Point3i`, `isceSifSetDma`,
`msg_run__11LightEffect`, `msg_run__19ParticleMorphEffect`,
`run__11AddParticleG6Point3`, `scanMask`, `subfontPrintu__FiiPUsi`

## agents/w9-startup-crt-e

Merged (the branch notes carry the five summary sections). Twenty-three units
with fresh `input_order` values 2442 through 2464: 17 function units, one
four-entry soft-double pass-through unit, and four storage/data-view units
that emit no function (the notes list a table, not JSON, so the units were read
from the files and cross-checked against the notes' table: same 23). All 23
host contracts build and exit 0 here (23 of 23). The notes' compiled sizes are
from a private trial; the plan build is authoritative. The worker's
`w8-startup-sce-d-set-srs-info` remark (its host stub text for the race table
does not match the image) is the worker's observation and I did not touch that
package. No game binary or disassembly listing was committed. 2011 function
bodies, 2463 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1496 |
| After | 1484 |
| Closed | 29 |
| Opened | 17 |

Closed: `norrath_sce_d_race_strings_view_4293e8`,
`norrath_sce_d_unknown_sdata_word_view_4852d8`,
`norrath_softdouble_add_2fb5c0`, `norrath_softdouble_compare_2fba48`,
`norrath_softdouble_multiply_2fb680`, `norrath_softdouble_to_float_2fbc20`,
`norrath_strticmp`, `padFilter`, `printfloat`, `raise`, `rdata`,
`reglistLoopCount`, `sceCdDiskReady`, `sceCdPowerOff`, `sceCdRead`,
`sceCdReadIOPm`, `sceCdStPause`, `sceCdStStop`, `sceDevctl`,
`sceDmaCurrentEnv`, `sceFsInit`, `sceGifPkEnd`, `sceGifPkRefLoadImage`,
`sceMcChdir`, `sceMcClose`, `sceMcDelete`, `sceMcFormat`, `sceMcGetDir`,
`sceMcMkdir`

Opened: `__fixunsdfdi`, `_raise_r`, `_sceFsIobSemaMK`, `_sceFs_Poff_Intr`,
`_sceFs_Rcv_Intr`, `currentDir`, `ftoi`, `mceSifRpcEndFunc`, `mceStorePwd`,
`sceCdDiskReady_old`, `sceGifPkAddGsAD`, `sceGifPkCloseGifTag`,
`sceGifPkCnt`, `sceGifPkOpenGifTag`, `sceGifPkRef`, `sceGifPkReserve`,
`sifParamFname`

## agents/w9-audio

Merged (the branch notes carry the five summary sections). Fifteen function
units with fresh `input_order` values 2465 through 2479. The worker's
paired-run interpreter results are private and were not reproduced here; what
I ran is the host contracts, all 15 of which build and exit 0 (15 of 15). The
data symbols the notes list as unauthored stay open. No game binary or
disassembly listing was committed. 2026 function bodies, 2478 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1484 |
| After | 1473 |
| Closed | 14 |
| Opened | 3 |

Closed: `animGetVolumeScale__Fii`, `playSoundForDlg__FP9DlgPacket`,
`runStreamQueing__Fv`, `soundAllFlags__Fi`,
`soundChangeSequenceVolume__Fif`, `soundDisable__Ff`,
`soundGetGlobalDialogDir__Fv`, `soundMakePersistant__Fi`, `soundQuiet__Fv`,
`soundReallyAllFlags__Fi`, `soundSetAudioEffect__Fiiiii`,
`soundStartDialogStream__Fiii`, `soundStartGlobalDialogStreamRandom__FiPci`,
`soundStartGlobalDialogStream__FiPci`

Opened: `animLoadBuffer`, `effectModeSize`, `pitchTab`

## agents/w9-frontend-c

Merged (the branch notes carry the five summary sections). Twelve units with
fresh `input_order` values 2480 through 2491: eight function units and four
data units (card buffers, pause words, player-change words, select words) that
emit no function. The notes list the plan entries as JSON, so the units were
read from the files and checked against the notes' count of 12. All 12 host
checks build and exit 0 here (12 of 12). The paired-run interpreter results
the notes cite are private and were not reproduced. Array lengths inferred only
from the distance to the next symbol are the worker's own caveat. No game binary
or disassembly listing was committed. 2034 function bodies, 2490 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1473 |
| After | 1453 |
| Closed | 23 |
| Opened | 3 |

Closed: `newSel`, `nmActions`, `nmDialogNodes`, `padRunFrame__Fv`, `pauseStack`,
`paused`, `pcLaunch`, `pcNmPlayers`, `pcTitle`, `playerChangeOldPos`,
`playerChangeOldYaw`, `playerLocalPlayerBusy__Fv`, `playerNetDead__Fv`,
`playerRace`, `playerRecallSpell__Fv`, `playerSelectInfo`,
`playerSelectionEnforce__Fv`, `playerStatus`, `popPause__Fi`,
`prepareBrowserUndo__Fv`, `processAnalogInput__FiiPfT2`, `quickLoadStamp`,
`ramSlot`

Opened: `castLightColor`, `haloColor`, `undoQuickLoadStamp`

## agents/w9-other-a

Merged (the branch notes carry the five summary sections). Twenty-five of the
26 units were selected with fresh `input_order` values 2492 through 2516; 19
of them emit no function (cheat flags, compressor/decompressor storage and
similar data views), seven function units were selected. `line-to-y` was
**not** selected: it defines `lineToY__Fi`, which `w8-object-list-math`
already supplies (input 2268), so the link fails with a multiple definition.
Its files stay in the snapshot, unselected. The two sources are independent
authorings of the same observed function (both read 18 * line + 295); which
one to keep is for the owners. All 26 host checks build and exit 0 here (26 of
26). No game binary or disassembly listing was committed. 2040 function
bodies, 2515 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1453 |
| After | 1461 |
| Closed | 25 |
| Opened | 33 |

Closed: `DLG_GetDialogCreature__Fv`, `GLibData`, `advanceFrame__Ff`,
`cheatInvulnerability`, `cheatMode`, `commProcessReal__Fv`,
`commandQueueEmpty__Fv`, `compImmediateCompressor`,
`compImmediateDecompressor`, `compLocalCompressor`, `compLocalDecompressor`,
`curDlgStart`, `curPlayer`, `decodeQHead`, `decodeQTail`, `deferedCall`,
`displayDialogPos`, `dlgDir`, `flags`, `get__11ResourceMap`, `headerAlpha`,
`hpos`, `initLightInfo__Fv`, `lastYAnchor`, `nextFrameDrop`

Opened: `action`, `actor`, `buffr`, `buffrfull`, `buffrmode`, `buffrout`,
`commCVol`, `commNameMask`, `commRVInternalLast`,
`executeAction__FP6Action`, `framestart`, `lastCE`, `lastlastCE`,
`lgAudARead`, `lgAudASync`, `lgAudAWrite`, `lgAudEnumHint`,
`lgAudEnumerate`, `lgAudOpen`, `lgAudSetRecordingVolume`,
`lgAudStartPlayback`, `lgAudStartRecording`, `lgCodecDecode`,
`lgCodecEncode`, `lightInfo`, `netGetNmConnectedPartners__Fv`,
`netIsModem__Fv`, `netSendPacket__FiiPUci`, `nmActors`, `readcount`,
`resourceFrame`, `timeout`, `writecount`

## agents/w9-other-b

Merged (the branch notes carry the five summary sections; the tip was four
hours old). Twenty-six units with fresh `input_order` values 2517 through
2542, 25 of them data definitions (memory-card error text pointer, packet
buffers and counters, environment tables and similar) and one function unit,
so the function-body count moves by one. The notes list plan entries as
JSON/prose, so the units were read from the files. All 26 host checks build and
exit 0 here (26 of 26); for the data units those checks assert sizes and
initial values only. No game binary or disassembly listing was committed. No
duplicate definition against `w9-other-a` or earlier packages. 2041 function
bodies, 2541 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1461 |
| After | 1437 |
| Closed | 25 |
| Opened | 1 |

Closed: `nextNmDeferedCalls`, `nmDeferedCalls`, `nmDirectQWords`, `nullBuff`,
`numEnvironments`, `p`, `packetbuffer`, `packetshift`, `pnums`,
`raceStrings`, `recallEnter`, `recallPos`, `scaleScale`,
`serviceResult__FP8queueCmdPi`, `sortedDir`, `sortedDirCount`,
`syncRandState`, `syncState`, `t`, `targetObjs`, `targets`, `tempBuffer`,
`thereIsACutPlayer`, `timeLastTransition`, `unscrambleTable`

Opened: `file`

## agents/runtime-tables

Merged, but only one of its 23 units is selected (the notes carry the five
summary sections; the tip was an hour old). The branch was started from an old
shared tip, and 22 of its 23 units define symbols the shared branch already
supplies from earlier packages (`w8-startup-sce-a`, `w8-runtime-libc-b`,
`w8-spell-static-init-a` and others: for example `arList`, `eenull`,
`norrath_assert_format_view`, `norrath_factory_prop_vtable`, the number and
bigint table views, `icos`/`isin` and `heap_pointer`). With all 23 selected,
the link fails with multiple definitions, so I selected only
`runtime-tables-srcfile` (input 2543, a data unit with no emitted function, host
test passes). The other 22 sources stay in the snapshot, unselected, with their
host tests; the three link-style host tests (`rtti-user`, `prop-vtable`,
`legacy-vtables`) pass when built with their implementation, and the other 20
pass as is. The worker's 947-to-923 claim is against its own old tip and does
not apply to this branch. Whether the independent duplicates agree byte for
byte with the already-selected ones is not checked here. No game binary or
disassembly listing was committed. 2041 function bodies, 2542 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1437 |
| After | 1436 |
| Closed | 1 |
| Opened | 0 |

Closed: `srcfile`

Opened: none

## agents/w9-startup-crt-b

Merged (the branch notes carry the five summary sections; the tip was 33
minutes old, so this reflects the tip at that time). Thirty-three of its 34
units were selected with fresh `input_order` values 2544 through 2576: the
newlib-style float kernels and the allocator, constructor and type-info
pieces, one data-only unit. `tf-11lavavehicle` was **not** selected: it
defines `__tf11LavaVehicle` and `__ti11LavaVehicle`, which
`w9-startup-crt-c` already supplies, so the link failed with a multiple
definition; its files stay in the snapshot, unselected. Host checks: 34 of 34
pass here once built as each file's header says. Three float checks
(`ieee754-powf`, `kernel-tanf`, `rem-pio2f`) compare recorded result bits and
fail by one or two ulps on this machine unless built with
`-ffp-contract=off` (the host compiler fuses multiply-add); with it they pass
(992 vectors, 1.00 ulp worst, and the recorded reduction cases). Their recorded
vectors come from the worker's private interpreter run and were not reproduced.
Five link-style checks (`builtin-new`, `malloc-lock`, `malloc-unlock`,
`terminate`, `update-mallinfo`) need their implementation file on the
command line (two also need a `-D`); they pass that way. The net import count
rises because the new constructor, vtable and `norrath_w9_startup_crt_b_view_*`
units reference more names (base constructors of the `VI*` classes, `__tf`/`__ti`
objects, float helpers such as `fabsf` and `__ieee754_sqrtf`, and the view
addresses) than they close. No game binary or disassembly listing was
committed. 2073 function bodies, 2575 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1436 |
| After | 1472 |
| Closed | 32 |
| Opened | 68 |

Closed: `__4GoldG6Point3i`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`,
`__6PlayerG6Point3sii`, `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii`,
`__7VISetup`, `__8GutChunkG6Point3iif`, `__9VIStation`, `__builtin_new`,
`__ieee754_powf`, `__ieee754_rem_pio2f`, `__kernel_tanf`, `__malloc_lock`,
`__malloc_unlock`, `__malloc_update_mallinfo`, `__terminate`,
`__tf10DoorSecret`, `__tf10GameObject`, `__tf11AutoChestLP`,
`__tf11FloorSwitch`, `__tf11PushTrigger`, `__tf14__si_type_info`,
`__tf15AutoContainerLP`, `__tf16__user_type_info`, `__tf3Orc`, `__tf4Boat`,
`__tf6Candle`, `__tf6PhProp`, `__tf7AntTrap`, `__tf7Blocker`,
`__tf7Candle2`, `__tf7EggSack`, `__tf9DoorSwing`

Opened: `_$_9bad_alloc`, `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`,
`__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap`,
`__10VIWndLegal`, `__11VIWndSplash`, `__12VIDictionary`, `__12VIWndPatcher`,
`__13VISoundDevice`, `__15VIWindowManager`, `__20VIWndGenericRenderer`,
`__4PropG6Point3PcN22li`, `__4VIUI`, `__5VIPad`, `__6VIFont`, `__7VIFader`,
`__7VIScene`, `__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc`,
`__8VIRaster`, `__9VICollide`, `__9VIWndDnas`, `__9VIWndEula`,
`__ieee754_sqrtf`, `__kernel_rem_pio2f`, `__new_handler`, `__rtti_user`,
`__terminate_func`, `__tf10AutoPropLP`, `__tf9bad_alloc`, `__tf9type_info`,
`__ti10AutoPropLP`, `__ti9type_info`, `_vt$4Gold`, `_vt$6PhProp`,
`_vt$6Player`, `_vt$6Sparks`, `_vt$8GutChunk`, `_vt$9bad_alloc`,
`coreForceSmallCharacters`, `fabsf`, `fxResourceMap`, `goldAnm`,
`lightAdd__FP5Light`, `norrath_w9_startup_crt_b_view_11c3ae8`,
`norrath_w9_startup_crt_b_view_462260`,
`norrath_w9_startup_crt_b_view_4644c0`,
`norrath_w9_startup_crt_b_view_464ab8`,
`norrath_w9_startup_crt_b_view_464ac8`,
`norrath_w9_startup_crt_b_view_464ad8`,
`norrath_w9_startup_crt_b_view_464ae8`,
`norrath_w9_startup_crt_b_view_464af8`,
`norrath_w9_startup_crt_b_view_464b10`,
`norrath_w9_startup_crt_b_view_464b20`,
`norrath_w9_startup_crt_b_view_464b30`,
`norrath_w9_startup_crt_b_view_464b48`,
`norrath_w9_startup_crt_b_view_464b58`,
`norrath_w9_startup_crt_b_view_464b68`,
`norrath_w9_startup_crt_b_view_464b78`,
`norrath_w9_startup_crt_b_view_464b88`,
`norrath_w9_startup_crt_b_view_464b98`,
`norrath_w9_startup_crt_b_view_4708d0`,
`norrath_w9_startup_crt_b_view_4801f8`,
`norrath_w9_startup_crt_b_view_4816dc`,
`norrath_w9_startup_crt_b_view_483b70`,
`norrath_w9_startup_crt_b_view_485380`, `racePrefix`, `scalbnf`,
`selectLumpName`, `typeToResource`

## agents/w9-startup-crt-d

Merged as a **partial package**: its notes have only the per-function notes and
a summary, not the five summary sections, but the tip was 68 minutes old, every
committed function has a host test, the tool tests pass, and the partial build
passes. The summary's build-plan list was used to read the 22 units (cross
checked against the files: same 22), selected with fresh `input_order` values
2577 through 2598. All 22 host tests build and exit 0 here (22 of 22), including
the soft-double `dpadd`/`dpsub`/`dpcmp`/`dptofp`/`fptodp` set. The branch also
added a `blockFree__FPv` alias to the existing
`block-allocator-lifecycle-build/block_free.cpp`. That file is input 6 of the
264-input historical prefix, whose record digest `check_source_snapshot.py`
pins, and the alias would also change the pinned function inventory, so I
restored the file to its shared-branch contents in a separate commit and did not
select that alias; `blockFree__FPv` stays open until its owner supplies a
standalone unit. The worker's per-function compiled sizes (for example `free`
64, `memmove` 132) come from its own trial and were not used. No game binary or
disassembly listing was committed. 2095 function bodies, 2597 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1472 |
| After | 1467 |
| Closed | 22 |
| Opened | 17 |

Closed: `amx_FindPublic`, `dpadd`, `dpcmp`, `dpsub`, `dptofp`, `fptodp`, `free`,
`get_iob`, `gsm_create`, `gsm_option`, `iSetTimerAlarm`, `iWakeupThread`,
`initEzNetCnf`, `isceSifSendCmd`, `kputchar`, `kputs`, `lgAudClose`,
`lgKbMInit`, `mceGetInfoApdx`, `memalign`, `memmove`, `new_iob`

Opened: `AlarmHandler`, `CreateReaderThread`, `Deci2Call`, `KbdMapTable`,
`__fpcmp_parts_d`, `__make_dp`, `__make_fp`, `__pack_d`, `__unpack_d`,
`__unpack_f`, `_fpadd_parts`, `_iWakeupThread`, `_memalign_r`,
`connect_to_iop_server`, `get_time`, `init_ee_timer`,
`qsort_translate_table`

## agents/w9-startup-crt-f

Merged (the branch notes carry the five summary sections; the tip was 66
minutes old). Twenty-two of its 30 units were selected with fresh
`input_order` values 2599 through 2620, including three assembler stubs
(`authored_entry_asm`). Eight units were **not** selected because the branch
commits no host test for them; the notes say they were checked only in a
private paired run outside git, which this repository cannot re-run:
`mc-read`, `mc-write`, `pad-read`, `pad-set-act-direct`, `tty-init`,
`tty-read`, `tty-write` and the `tty-state` data unit that the three tty
functions depend on. Their sources stay in the snapshot, unselected, and
`sceMcRead`, `sceMcWrite`, `scePadRead`, `scePadSetActDirect` and the tty
functions stay open until a committed test exists. Of the 22 selected, 19 host
tests build and exit 0 here (the link-style ones need their implementation file
on the command line, two need a `-D` from the file's first lines). The three
assembler comparisons `sif-set-d-chain`, `sif-set-dma` and `sif-stop-dma` take
the private executable as an argument and were not run. No game binary or
disassembly listing was committed (the notes explain that `srcfile` and
`tlbsrc` are machine code and were deliberately not authored). 2117 function
bodies, 2619 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1467 |
| After | 1455 |
| Closed | 22 |
| Opened | 10 |

Closed: `sceMcOpen`, `sceMcSeek`, `scePadEnterPressMode`, `scePadInfoPressMode`,
`scePadSetActAlign`, `scePadSetMainMode`, `sceSdTransToIOP`,
`sceSifExitCmd`, `sceSifFreeSysMemory`, `sceSifMBindRpcParam`,
`sceSifMExitRpc`, `sceSifMUnBindRpc`, `sceSifRemoveCmdHandler`,
`sceSifSetDChain`, `sceSifSetDma`, `sceSifStopDma`, `strcat`, `strchr`,
`strrchr`, `strticmp__FPCcT0`, `unet_shutdown__Fv`, `ustrcat__FPUsPc`

Opened: `VIFreeUSocketExApi__Fv`, `_sceSifMInitCheck`, `net_buf`,
`norrath_sif_m_rpc_packet_pool_view`, `sLibNetInitialized`,
`scePadGetButtonMask`, `scePadSetButtonInfo`, `scePadSetReqState`,
`sceSifDmaStat`, `sceSifRemoveRebootNotifyHandler`

## agents/w9-frontend-b

Merged (the branch notes carry the five summary sections; the tip was 61
minutes old). Twenty-five units with fresh `input_order` values 2621 through
2645: 22 function units (front-end loading, death screen, jump gates, progress
drawing, HUD and similar) and three data units (`cd-file`, `config-strings`
and `state-words`). The `config-strings` table holds nine controller-row key
identifiers (`c_attack` and so on), not dialogue. The notes list plan entries
in prose, so the units were read from the files. All 25 host checks build and
exit 0 here (25 of 25). The notes name the opened symbols and ask the
integrator to confirm them; the real opened set is the 76 listed below: they
are other packages' functions and data (`HUDInit__Fv`, `Load__6Camera`,
`MC_LoadRamSlot__Fv`, `lumpLoadGlob__FPc`, the `jumpGate*` and light/model
helpers and so on). The count rises because these loading and HUD functions
call into areas not yet reconstructed. No game binary or disassembly listing
was committed. 2139 function bodies, 2644 authored inputs, zero COMMON. 214
tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1455 |
| After | 1495 |
| Closed | 36 |
| Opened | 76 |

Closed: `configMode`, `configStrings`, `coreStartLoading__Fv`, `curSel`,
`deathNetHack`, `deathState`, `dialogPacket`, `drawProgress__Fv`,
`emergencyMusicWaitAbort`, `gCdFile`, `gameCheckForDeathScreen__Fv`,
`gameDeath__Fv`, `gameLoadWorld__FPc`, `gotPlayerStartThisLevel`,
`hudDisplayNetCamToggle`, `hudNetWaitMsgReady__Fv`,
`inGameConfigShuttingDown`, `initNamePad__Fv`, `initPreSelect__Fv`,
`initStuff__6PhPropi`, `jumpGateHubReset__Fi`, `jumpGateSave__FPc`,
`jumpGateTaskDraw__Fv`, `jumpGateTaskRun__Fv`, `keyboardGetNextKey__Fv`,
`keyboardGetNextTranslatedKey__Fv`, `loadPlayerInfo__FiPiP12playerStruct`,
`loadRamSlot__Fv`, `memcardTitle__FPUs`, `modifyRamSlotForNetSave__Fv`,
`multiHudAddHelpMessage__FiPUsT1i`, `needActAlign`, `needEnforce`,
`netForceLocalUpdate`, `netGetLocalObject__FP8Creature`,
`netGetLocalPlayerNmSafe__Fi`

Opened: `GetSaveFootPrint__6Camera`, `HUDInit__Fv`, `Load__6CameraPc`,
`MC_GetLoadName__Fv`, `MC_LoadLevelEntities__FPcT0`, `MC_LoadRamSlot__Fv`,
`MC_ResetAutoCheckPointSave__FPc`, `Read__10VIKeyboardR12VIConsoleKey`,
`__7Weatheri`, `boxAdd__FP3BoxT0`, `boxSetEmpty__FP3Box`,
`browserOverwrite`, `convertIndiciesToPointers__Fi`,
`coreStartLoadingScreen__Fv`, `curDest`, `cursorTime`,
`debugStartMenuIsOn__Fv`, `delta`, `doJumpEffect__Fi`, `fontGrid`,
`framedone`, `gameDeathHeight`, `gameInitPostFrontend__Fv`,
`gameLineFuse__Fv`, `gameMaxCamHeight`, `gameMinCamHeight`,
`gameSetMusicTimer__Fi`, `getSlotName__FiiPc`, `groundType`, `hubNames`,
`importInfoHeader`, `inesc`, `initJumpEffect__Fv`, `initPosition__Fii`,
`init__3mapi`, `init__4itemv`, `inventorySyncQuestItems__Fi`,
`jumpGateDestCount`, `jumpGateEntries`, `jumpGateHurtDest`,
`jumpGateLoad__FPc`, `jumpGateSaveFootPrint__Fv`, `jumpGateState`,
`jumpGateTime`, `jumpGateWho`, `lastDest`, `lastkey`, `lasttime`,
`lightSetSceneLights__FG6Point3iiiiiiiii`, `lightStart`, `list.1073`,
`loadPlayerLumps__Fv`, `loc`, `lockAlpha`, `lumpLoadGlob__FPc`,
`modelGetBoundingBox__FP12_modelHeaderlPiN52`,
`modelSetGlobalShadowBrightness__Ff`, `nameGridCount`, `nameGridW`,
`originalPos`, `originalYaw`, `progress`, `routeClear__Fv`,
`routeMakeLattice__Fv`, `scale`, `scissor`, `selectMode`,
`skirmishInit__Fv`, `soundSetAmbientData__FPc`, `thisLevel`,
`worldActivityLevel`, `worldAllocateSegments__FP12_worldHeader`,
`worldBoundingBox`, `worldDeactivateUntakenDoors__FP12_worldHeader`,
`worldSetWaterProperties__FP8_texture`, `worldTileEntityLightScale`

## agents/w9-frontend-d

Merged (the branch notes carry the five summary sections; the tip was 45
minutes old). Sixteen units with fresh `input_order` values 2646 through 2661:
twelve function units and four data units (`buffers`, `launcher-statics`,
`state-words`, `tex-strings`). The twelve function units' host checks build and
exit 0 here (12 of 12). The four data units have no source-side check; their
test is `host-tests/w9-frontend-d/test_data.py`, which compiles each with the
EE toolchain and checks symbol sizes, contents and relocations. It runs
(4 tests, OK) and is recorded as their host file. The branch also touched
`.gitignore` to track that test directory's `.py` files only; that is the only
non-source file besides the notes. The worker's remark about differences from
the original (see its "Deliberate differences" section) stands as written and
was not independently checked. No game binary or disassembly listing was
committed. 2154 function bodies, 2660 authored inputs, zero COMMON. 214 tool
tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1495 |
| After | 1478 |
| Closed | 32 |
| Opened | 15 |

Closed: `requestToExit`, `resetWithClose__Fv`, `restrictChars`, `rumphole`,
`runBrowser__FPUs`, `runConfigure__FiPUsi`, `sCdRmode`, `sGameOptions`,
`saveOk`, `savedScene`, `scriptFrame`, `scriptGameStateEntry__FiPi`,
`slotInfoHeaders`, `startExitFade`, `startFrame`, `startInhibit`,
`startProgress__Fv`, `startScorePos`, `tempPlayerStruct`, `texStrings`,
`textBodyLen`, `textSend__Fv`,
`textureDrawSuperColor__FP8_textureiiiUiiiiicc`,
`textureLockCacheBuffer__Fv`, `texturePrepareToLockCacheBuffer__Fv`,
`textureReleaseCacheBuffer__Fv`, `texturesAreDecompressing`,
`texturesAreLoading`, `unModifyRamSlotForNetSave__Fv`, `undoBrowser__Fv`,
`waiting`, `waitingForStreamSpool`

Opened: `blocksFree`, `browserDelete`, `dirEntryIsQuickLoad__FP14sceMcTblGetDir`,
`engineGetField__Fv`, `gameStateData`,
`generateSortedListAndInfoHeaders__Fv`, `markFileName`,
`needToClearCacheBuffer`, `patchSortedListWithLocalTime__Fv`, `sessionMark`,
`slotsCorrupted`, `sortedPlayerRace`, `swap__H1Zi_RX01T0_v`,
`usprintf__FPUsPcffffffff`, `wasFormatting`

## agents/w9-frontend-a

Merged (the branch notes carry the five summary sections; the tip was 22
minutes old). Twenty-five of its 29 units were selected with fresh
`input_order` values 2662 through 2686: 21 function units and four data units
(act-align, button values, cache cd-done, memory-card state). Four function
units were **not** selected because the notes themselves say they have no
committed host contract and were checked only in a private comparison:
`Process__9VIStation`, `Init__9VIStationiPCcPcT3T2T2i`, `RunPatcher__7VISetup`
and `MC_CheckPlayerImportOrChange__Fv`. Their sources stay in the snapshot,
unselected. The 25 selected units' host checks build and exit 0 here (25 of
25). The opened count rises because the memory-card, cd and VI functions call
into areas not yet reconstructed; the notes name `sAlarmCallback__FiUsPv`,
`MainLoopBegin__7VISetup` and `MainLoopEnd__7VISetup` as belonging to other
packages. No game binary or disassembly listing was committed. 2175 function
bodies, 2685 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1478 |
| After | 1525 |
| Closed | 33 |
| Opened | 80 |

Closed: `Ascii2Sjis__FUc`, `Free__7VISetup`, `Free__9VIStation`, `Init__7VISetupi`,
`MCA_Busy__Fv`, `MCA_DirContents`, `MCA_Result`, `MC_ByteCopy__FPvT0i`,
`MC_GetNetGameInfo__FPiT0Pc`, `MC_GetRamPlayerPtr__Fi`,
`MC_GetTempPlayerStorage__Fi`, `MC_NameToAct__FPc`,
`MC_SaveLevelEntities__FPc`, `MC_SetActByLevelName__FPc`,
`Process__7VISetup`, `RestartConnect__7VISetup`, `_$_7VISetup`,
`_$_9VIStation`, `actAlignData`, `actSendData`, `askedForConfigLoad`,
`browserSlot`, `buttonValues`, `cacheCdDone`, `cardType`,
`cdCheckForFileInCurrentLayer__FPc`, `cdIsBusy__Fv`,
`cdLoadAsyncIOP__FPciT0ii`, `cdLoadAsync__FPcPvT0ii`,
`checkSave__FP10GameObject`, `configFileName`, `configFormatted`,
`configLumpLoaded`

Opened: `BeginScene__8VIRaster`, `Begin__7VIFaderff`, `CleanUp__11VIGUIObject`,
`Clear__12VIDictionary`, `Clear__7VIScene`, `Close__13VISoundDevice`,
`Close__4VIUI`, `Close__5VIPad`, `Close__8VIRaster`, `Close__9VICollide`,
`EndScene__8VIRaster`, `Flush__13VISoundDevice`, `Free__10VIWndLegal`,
`Free__11VIWndSplash`, `Free__12VIWndPatcher`, `Free__15VIWindowManager`,
`Free__7VIFader`, `Free__9VIWndDnas`, `Free__9VIWndEula`,
`GetTime__7VITimer`, `Init__10VIWndLegal`, `Init__11VIWndSplashP7VIScene`,
`Init__12VIDictionaryi`, `Init__12VIWndPatcher`, `Init__13VISoundDevice`,
`Init__15VIWindowManagerP5VIPadP10VIKeyboard`,
`Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary`, `Init__5VIPadii`,
`Init__7VIFaderP8VIRasterP12VIDictionaryf`,
`Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide`,
`Init__8VICameraR8VIRasterRC7VIVect3T2ffff`, `Init__8VIRasterPviii`,
`Init__9VICollide`, `Init__9VIWndDnasP7VIScene`, `Init__9VIWndEula`,
`Load__13VIGameOptions`, `Parse__9VICSFFilePCcR10VIESFParseR7VIScene`,
`Process__13VISoundDevice`, `Read__5VIPadR12VIConsolePad`,
`Render__7VISceneR8VICamera`, `RestartConnect__11VIWndSplash`,
`SetAppPtr__5VIWndPv`, `SetClearColor__8VIRasterRC10VIColor32F`,
`SetFocus__5VIWndP5VIWndb`, `SetRoot__15VIWindowManagerP5VIWnd`,
`SetTimeOfDay__7VIScenef`, `SetTime__7VIScenel`, `SetWireframe__8VIRasteri`,
`StartAmbientSoundtrack__7VISetup`, `StopAmbientSoundtrack__7VISetup`,
`Update__15VIWindowManager`, `Update__7VIFader`, `VISetWindowUI__FP4VIUI`,
`VISplashFxFree__Fv`, `VISplashFxInit__FP8VIRasterP12VIDictionary`,
`_$_10VIESFParse`, `_$_10VIMatrix44`, `_$_10VIWndLegal`, `_$_11VIWndSplash`,
`_$_12VIDictionary`, `_$_12VIWndPatcher`, `_$_13VISoundDevice`,
`_$_15VIWindowManager`, `_$_20VIWndGenericRenderer`, `_$_4VIUI`,
`_$_5VIPad`, `_$_7VIFader`, `_$_7VIScene`, `_$_8VIRaster`, `_$_9VICollide`,
`_$_9VIWndDnas`, `_$_9VIWndEula`, `__10VIESFParse`, `__9VICSFFile`,
`__cl__8VICameraR7VIVect3T1`, `ascii_special`, `ascii_table`,
`getSaveTableEntry__FPc`, `sPad`, `saveLevelDataASync__FPci`

## agents/w9-camera-map

Merged (the branch notes carry the five summary sections; the tip was 52
minutes old). Twenty of its 21 units were selected with fresh `input_order`
values 2687 through 2706: camera push/pop/save, cut-scene routines, debug and
Innoruk cameras and one storage unit that emits no function. `web-construct`
was **not** selected: it defines `__3Webii`, which `w9-startup-crt-a` already
supplies (input 2414), so the link failed with a multiple definition; its
files stay in the snapshot, unselected. Host checks: 20 of 21 build and exit 0
as is; `cut-init-scene` stores 32-bit object pointers and, as its header says,
needs an x86_64 build with `-Wl,-pagezero_size,0x10000`; built that way it
passes. The notes say the checks were written from the worker's reading of the
original; they are the worker's, not independent evidence. No game binary or
disassembly listing was committed. 2197 function bodies, 2705 authored inputs,
zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1525 |
| After | 1527 |
| Closed | 24 |
| Opened | 26 |

Closed: `GetTargetNum__6Camera`, `GetZoomDistance__6Camera`, `Pop__6Camera`,
`Save__6CameraPc`, `SetCutWasActive__6Camera`, `SetZoomDistance__6Cameraf`,
`addIcon__3mapP6Point3iPciT3N33`, `cutDrawFrame__Fv`,
`cutInitScene__FP9_cutscene`, `cutRunFrameTask__Fv`, `cutStop__Fv`,
`doDebugCam__6Camera`, `doInnorukCam__6Camera`, `doNormalCam__6Camera`,
`isRevealed__3mapv`, `mapButtonWasDown`, `mapObjList`, `mapScale`, `mapX`,
`mapY`, `norrath_map_add_icon`, `revealEntireMap__3mapv`,
`revealMap__3mapG6Point3`, `willDraw__3mapP12_worldHeaderf`

Opened: `_3map$iconList`, `_3map$iconListCount`, `_3map$revealed`,
`animRemoveNonpermanentAnimations__FP14AnimationState`,
`checkYawReset__6Camera`, `cutLoadedDialogStream`, `cutStartMusic`,
`determineBestController__6Camera`, `displayMusicPos`, `displayTextDisable`,
`doAutoPitch__6Camera`, `doUserYaw__6Camera`, `doUserZoom__6Camera`,
`enforceMaxSpeed__6CameraP6Point3`, `findCutDialog__Fv`,
`hideHiddenObjects__FP10GameObject`, `hudLetterBox__Fv`,
`objectCreateByName__FPcG6Point3sPPc`, `obtainTarget__6CameraP6Point3`,
`playerCountChanged__6Camera`, `rayHit__FP6Point3N30`,
`removeMonster__FP10GameObject`, `resetScissor__Fv`,
`showHiddenObjects__FP10GameObject`, `soundStartMusic__Fii`,
`unremoveMonster__FP10GameObject`

## agents/w9-hud-inventory-items-b

Merged (the branch notes carry the five summary sections; the tip was 48
minutes old). Thirty-three of its 34 units were selected with fresh
`input_order` values 2707 through 2739: HUD and inventory functions and 12
or so data definitions that emit no function (gold alpha, Hermite time, HUD
header, inventory grid, name tags and similar). The 34th unit,
`view-exit-sound-resource` (a 21-byte resource-name string view), was **not**
selected: it has no host check, only a `static_assert` on its size, so a wrong
byte in it would not be noticed; it stays in the snapshot, unselected. The
selected units' host checks build and exit 0 here (33 of 33; the grid-drop check
takes `-DNORRATH_INVENTORY_HOST=1`). The worker reports 1507 unresolved imports
for its private plan copy; on the shared branch the real count after this
merge is 1523 (the worker's plan did not contain the later packages). No game
binary or disassembly listing was committed. 2217 function bodies, 2738
authored inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1527 |
| After | 1523 |
| Closed | 33 |
| Opened | 29 |

Closed: `getName__4itemPQ24item11descriptioni`,
`getPicName__4itemPciPQ24item11description`,
`getProxyMeshId__4itemPQ24item11description`,
`getValue__4itemPQ24item11description`, `givePlayerFeat__Fxii`, `goldAlpha`,
`goldNumber`, `hermDeltaScale`, `hermiteTime`,
`hudGivePlayerGold__FiG6Point3i`, `hudHeaderUni`,
`hudInfoMessageShowing__Fv`, `invGrid`, `inventory`,
`inventoryAdjustAbility__Fii`, `inventoryAdjustSkill__Ficx`,
`inventoryCheckWieldingStatus__FiPQ24item11description`,
`inventoryExit__Fiii`, `inventoryFrameDraw__Fv`, `inventoryFrameRun__Fv`,
`inventoryGridDropItem__Fiiciiii`, `inventoryLoadState`,
`inventoryLoad__Fv`, `inventoryLump`,
`inventorySetAnim__FiP14AnimationStatei`, `inventoryUnload__Fv`,
`lastFontCharAlpha`, `lastFontColor`, `lerpColor__FUiUif`,
`lerpShortRot__Fssfff`, `makeRandomPotion__4itemPQ24item11description`,
`msgTimeout`, `nameTags`

Opened: `_4item$prefixTable`, `_4item$suffixTable`, `_shopCheck__Fi`,
`animGetAnimComponent__FP14AnimationStateP15AnimationHeader`, `bevel`,
`buySellVertSpacing`, `findAndStripGender__4itemPUs`,
`getArmorBonus__4itemPQ24item11description`, `getAttributeName__4itemiPc`,
`getDamageRangeAll__4itemPQ24item11descriptionPiT2i`,
`givePlayerSkill__Fii`, `invHeader`, `invInitSharedInterfaceTextures__Fv`,
`inventoryDrawDelay`, `inventoryDrawPlayerColor__Fiii`,
`inventoryDrawPlayer__FiiP14AnimationStatei`, `inventoryDraw__Fi`,
`inventoryRun__Fi`, `inventoryUnpack__Fiib`, `marble`,
`norrath_inventory_exit_sound_resource_view`, `playerGoldx`, `playerGoldy`,
`playerSetFeatRank__Fixi`, `playerSetSkillRank__Fiii`,
`resetMenu__FP12screenStruct`, `shopUnpackToGrid__Fiii`, `stage`,
`statsScreen`

## agents/w9-render-math-b

Merged (the branch notes carry the five summary sections; the tip was 54
minutes old). Twenty-five units with fresh `input_order` values 2740 through
2764: 22 function units (render and model math, scissor and megatile helpers)
and three data units (camera angles, map scissor, megatile seed rotation) that
emit no function. All 25 host checks build and exit 0 here (25 of 25). The
notes' "Strings kept" section says the only literals are one engine level
identifier and two resource file names in `modelDrawShadow` (engine
identifiers, not dialogue); I read that section but did not grep every unit. No game binary
or disassembly listing was committed. 2241 function bodies, 2763 authored
inputs, zero COMMON. 214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1523 |
| After | 1502 |
| Closed | 29 |
| Opened | 8 |

Closed: `lightSetAmbientColor__FPs`, `lightSetAmbientOffset__FPs`,
`lightSetAmbientOnly__Fv`, `lilog2__Fl`, `makeExplosion__FG6Point3f`,
`mapScissorX`, `mapScissorY`, `matIdent__FP8Matrix34`,
`matMakeTransYawScale__FP8Matrix34G6Point3sf`,
`matMakeTrans__FP8Matrix34G6Point3`,
`matMakeViewMatrix__FP8Matrix44fffffff`, `matMul__FP8Matrix34N20`,
`matMul__FP8Matrix44P8Matrix34T0`, `matRot__FP8Matrix34is`,
`matTrans__FP8Matrix34fff`, `matTransform__FP6Point3P8Matrix44T0`,
`matUntransform__FP6Point3P8Matrix33T0`, `megatileRot`, `megatileSeed`,
`modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`,
`modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`,
`modelGetNmShadowPoints__FP12_modelHeader`,
`modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`,
`modelSetHalo__FP7HaloDefi`, `modelSetNoZWrite__Fv`,
`modelSetShadowScale__Ff`, `myFOV`, `myPitch`, `myRoll`

Opened: `animGetBoneMatrix__FP14AnimationStateiP8Matrix34`, `ballParticle`,
`lightCharacterShadow__FG6Point3P11LightResult`,
`matPostRot__FP8Matrix33is`, `matScale__FP8Matrix44f`,
`matTrans__FP8Matrix44fff`, `matTransform__FP6Point3P8Matrix34T0`,
`trailParticle`

## agents/w9-animation-object-a

Merged (the branch notes carry the five summary sections; the tip was 43
minutes old). Twenty-one units with fresh `input_order` values 2765 through
2785: 17 function units and four data units (animation state, game line,
in-message and lump data) that emit no function. The branch also carries
`docs/agents/w9-animation-object-a-plan-entries.py`, a helper that prints plan
entries and writes a private plan copy; I did not use or run it (the entries
here come from compiling each unit). All 21 host checks build and exit 0 here
(21 of 21). No game binary or disassembly listing was committed. 2258 function
bodies, 2784 authored inputs, zero COMMON. 214 tool tests passed. No EE or game
execution.

| | |
|---|---|
| Before | 1502 |
| After | 1478 |
| Closed | 28 |
| Opened | 4 |

Closed: `animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi`,
`animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`,
`animAddToEnd__FP14AnimationStateP15AnimationHeaderi`,
`animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`,
`animFindComponentWithFlags__FP14AnimationStatei`,
`animGlobalSync__FP14AnimationState`,
`animMapAnimationSounds__FP15AnimationHeader`, `animState`, `animStatePtr`,
`animUseBuffer`, `animationLoadPos`, `animationLoading`,
`bExitStatePersueIdle__8Creature`, `bFarFromHome__8Creature`,
`convertPointersToIndicies__Fi`, `dialogLumpName`, `domeLump`,
`eRand__10GameObject`, `eqLumpHeader`, `findTagFloat__10GameObjectPci`,
`gameLineCount`, `gameLineInitPathState__FP17gameLinePathStatePcP6Point3`,
`gameLineUpdatePosition__FP17gameLinePathState`, `gameLines`,
`genericMove__10GameObjectG6Point3P10GameObject`,
`getSmartRandomDestination__8Creaturei`, `hasLightOrParticleNumber__FPPci`,
`inMsg`

Opened: `fmodf`, `gameLineGetNextPoint__FP17gameLinePathStateiP6Point3`,
`objectMoveWithWorldCollision__FP6Point3G6Point3fiT0`,
`playerPrintPaperDoll__Fi`

## agents/w9-ctors-lantern-a

Merged (the branch notes carry the five summary sections; the tip was 31
minutes old). Thirty-eight units with fresh `input_order` values 2786 through
2823: 22 resource maps, 14 virtual tables and two pointer/list tables, all data
(no function bodies, so the body count stays at 2258). Each unit has a host
comparison under `views/host/`; the notes' build-plan section is a table of
sources and hashes, and the same 38 units were found in the files. All 38 host comparisons build
(linked against the unit, no source change) and exit 0 here (38 of 38). The
count of opened imports is large because each virtual table points at its
class's slot functions, which are not defined in this package, so every slot
function becomes an unresolved import; the notes do not claim otherwise. The
worker's per-object size equality with the original symbol was checked at EE
compile time by the worker and was not re-measured here. The worker did not
define the three offsets it lists as blocked (they are interior offsets of
the `skills` object, not separate objects). No game binary or disassembly
listing was committed. 2258 function bodies, 2822 authored inputs, zero COMMON.
214 tool tests passed. No EE or game execution.

| | |
|---|---|
| Before | 1478 |
| After | 1584 |
| Closed | 38 |
| Opened | 144 |

Closed: `norrath_ctors_ant_lantern_view_42ae60`,
`norrath_ctors_ant_lantern_view_42af70`,
`norrath_ctors_ant_lantern_view_42b1d0`,
`norrath_ctors_ant_lantern_view_42b548`,
`norrath_ctors_ant_lantern_view_42b630`,
`norrath_ctors_ant_lantern_view_42b870`,
`norrath_ctors_ant_lantern_view_42b958`,
`norrath_ctors_ant_lantern_view_42ba00`,
`norrath_ctors_ant_lantern_view_42bb08`,
`norrath_ctors_ant_lantern_view_42be08`,
`norrath_ctors_ant_lantern_view_42beb0`,
`norrath_ctors_ant_lantern_view_42bf58`,
`norrath_ctors_ant_lantern_view_42c188`,
`norrath_ctors_ant_lantern_view_42c308`,
`norrath_ctors_ant_lantern_view_42c8a0`,
`norrath_ctors_ant_lantern_view_42cef0`,
`norrath_ctors_ant_lantern_view_42cfc8`,
`norrath_ctors_ant_lantern_view_42d200`,
`norrath_ctors_ant_lantern_view_438a38`,
`norrath_ctors_ant_lantern_view_438b28`,
`norrath_ctors_ant_lantern_view_438eb8`,
`norrath_ctors_ant_lantern_view_4494c0`,
`norrath_ctors_ant_lantern_view_44a810`,
`norrath_ctors_ant_lantern_view_44a8a0`,
`norrath_ctors_ant_lantern_view_44b900`,
`norrath_ctors_ant_lantern_view_44b9f0`,
`norrath_ctors_ant_lantern_view_44bae0`,
`norrath_ctors_ant_lantern_view_44bbd0`,
`norrath_ctors_ant_lantern_view_44bcc0`,
`norrath_ctors_ant_lantern_view_44bdb0`,
`norrath_ctors_ant_lantern_view_44bea0`,
`norrath_ctors_ant_lantern_view_44bf90`,
`norrath_ctors_ant_lantern_view_44c080`,
`norrath_ctors_ant_lantern_view_44c260`,
`norrath_ctors_ant_lantern_view_44c350`,
`norrath_ctors_ant_lantern_view_44c440`,
`norrath_ctors_ant_lantern_view_44c530`,
`norrath_ctors_ant_lantern_view_44cb98`

Opened: `_$_10BlackWidow`, `_$_10FireBeetle`, `_$_3Ant`, `_$_4Lamp`, `_$_5Chest`,
`_$_5Ghoul`, `_$_5Gnome`, `_$_6Badger`, `_$_6Goblin`, `_$_7Critter`,
`_$_7Cyclops`, `_$_7FireFly`, `_$_7Lantern`, `_$_8AntQueen`, `_$_8Froglock`,
`_$_8JumpGate`, `_$_9Generator`, `__tf10BlackWidow`, `__tf10FireBeetle`,
`__tf3Ant`, `__tf4Lamp`, `__tf5Chest`, `__tf5Ghoul`, `__tf5Gnome`,
`__tf6Badger`, `__tf6Goblin`, `__tf7Critter`, `__tf7Cyclops`,
`__tf7FireFly`, `__tf7Lantern`, `__tf8AntQueen`, `__tf8Froglock`,
`__tf8JumpGate`, `__tf9Generator`, `clone__10BlackWidow`,
`clone__10FireBeetle`, `clone__3Ant`, `clone__4Lamp`, `clone__5Chest`,
`clone__5Ghoul`, `clone__5Gnome`, `clone__6Badger`, `clone__6Goblin`,
`clone__7Critter`, `clone__7Cyclops`, `clone__7FireFly`, `clone__7Lantern`,
`clone__8AntQueen`, `clone__8Froglock`, `clone__8JumpGate`,
`clone__9Generator`, `getMinibossName__6Goblin`,
`getMinibossName__7Cyclops`, `getModel__7Critter`, `getModel__9Generator`,
`initMiniboss__6Goblin`, `initMiniboss__7Cyclops`,
`msg_alert__5GhoulifG6Point3P10GameObject`,
`msg_alert__6GoblinifG6Point3P10GameObject`,
`msg_collisionWorld__6GoblinG6Point3`, `msg_collision__5GnomeP10GameObject`,
`msg_collision__7CritterP10GameObject`, `msg_draw__10BlackWidow`,
`msg_draw__10FireBeetle`, `msg_draw__3Ant`, `msg_draw__4Lamp`,
`msg_draw__5Chest`, `msg_draw__5Ghoul`, `msg_draw__5Gnome`,
`msg_draw__6Badger`, `msg_draw__6Goblin`, `msg_draw__7Critter`,
`msg_draw__7Cyclops`, `msg_draw__7FireFly`, `msg_draw__7Lantern`,
`msg_draw__8AntQueen`, `msg_draw__8Froglock`, `msg_draw__8JumpGate`,
`msg_draw__9Generator`, `msg_getPokeName__5ChestPUsi`,
`msg_getPokeName__8JumpGatePUsi`, `msg_hurt__7CritterfP10GameObjecti`,
`msg_hurt__8AntQueenfP10GameObjecti`, `msg_load__3AntPc`,
`msg_load__5ChestPc`, `msg_load__6GoblinPc`, `msg_load__7CritterPc`,
`msg_load__7CyclopsPc`, `msg_load__8FroglockPc`, `msg_load__8JumpGatePc`,
`msg_load__9GeneratorPc`, `msg_moveCollision__10FireBeetlePP10GameObject`,
`msg_moveCollision__5GnomePP10GameObject`,
`msg_moveCollision__6BadgerPP10GameObject`,
`msg_moveCollision__6GoblinPP10GameObject`,
`msg_moveCollision__7FireFlyPP10GameObject`, `msg_poke__5Chesti`,
`msg_poke__8JumpGatei`, `msg_run__10BlackWidow`, `msg_run__10FireBeetle`,
`msg_run__3Ant`, `msg_run__4Lamp`, `msg_run__5Chest`, `msg_run__5Ghoul`,
`msg_run__5Gnome`, `msg_run__6Badger`, `msg_run__6Goblin`,
`msg_run__7Critter`, `msg_run__7Cyclops`, `msg_run__7FireFly`,
`msg_run__7Lantern`, `msg_run__8AntQueen`, `msg_run__8Froglock`,
`msg_run__8JumpGate`, `msg_run__9Generator`, `msg_save__3AntPc`,
`msg_save__5ChestPc`, `msg_save__6GoblinPc`, `msg_save__7CritterPc`,
`msg_save__8FroglockPc`, `msg_save__8JumpGatePc`, `msg_save__9GeneratorPc`,
`msg_trigger__8AntQueeni`, `msg_trigger__9Generatori`,
`norrath_ctors_ant_lantern_view_42c760`,
`norrath_ctors_ant_lantern_view_42c788`,
`norrath_ctors_ant_lantern_view_42c7b0`,
`norrath_ctors_ant_lantern_view_42c7d8`,
`norrath_ctors_ant_lantern_view_42c800`,
`norrath_ctors_ant_lantern_view_42c828`,
`norrath_ctors_ant_lantern_view_42c850`,
`norrath_ctors_ant_lantern_view_42c878`, `setState__10BlackWidowi`,
`setState__10FireBeetlei`, `setState__3Anti`, `setState__5Ghouli`,
`setState__5Gnomei`, `setState__6Badgeri`, `setState__6Goblini`,
`setState__7Cyclopsi`, `setState__7FireFlyi`, `setState__8AntQueeni`,
`setState__8Froglocki`, `setState__9Generatori`

## Second-pass audit of merged host checks

Added by the integrator that worked beside the one that merged most of the
`w8-*` packages. At tip `ef62a71a` it ran every host check in the 577 source
directories added since `5744679` (the tip when this pass started) on this
machine, and changed one numeric literal at a time in each source (up to six
mutants) to see whether the check notices. It did not read the executable, so
the byte-comparison checks that need `SLUS_205.65` were not run.

- Base failures, all already disclosed in the entries above: the 64-bit
  pointer harness failures of `cdProcess`, `soundMapSound`,
  `soundPlaySequence` and `soundComputeSequenceLength`; the
  `_$_14AnimationState`, `MC_ResetRamSlot` and `sceCdGetError` checks, which
  need `-fno-builtin` or a 32-bit pointer layout to run as intended.
- Checks that could not run without the executable: the fourteen
  `w8-startup-sce-a` syscall-stub comparisons and `SetArg`, and the
  kernel-patches and gs-dma-kernel assembly units (their statements are the
  workers').
- Weak checks, which pass but would not notice a changed literal in the
  source: `w8-runtime-libc-a-exit` and `w8-startup-sce-b-request-call` (0 of 6
  mutants noticed; their structural literals are guarded by `static_assert`
  at EE compile time instead, which a host mutation does not exercise).
  Nineteen `tf`/type-info units and a few word tables noticed only 1 or 2 of 6
  mutants.
- Everything else: every unit with literals noticed at least half of its
  mutants; `w8-spell-static-init-a` noticed 6 of 6 in all 31 units.

The packages already rejected (`frontend-shell` before its fix and
`w8-spell-static-init-b`) were the only ones found with checks that cannot
fail or do not exist.

## Not merged: codex/norrath-runtime-binding

`origin/codex/norrath-runtime-binding` (owner, one commit) adds an
opt-in `--defsym=memset=norrath_block_memory_set` binding to
`tools/build_partial.py` with hash-checked provider/consumers plus docs.
It is not an `agents/*` worker package, carries no agent notes, and
changes the build driver itself, so it is left for the startup lead or
owner to merge. The default authored partial link used for the official
counts above does not take the opt-in flag and is unaffected.

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

`anim` is claimed twice: pre-logo-dialog imports the ELF name used by the
dialog callback at `0x007d0dc8`, while a same-named local symbol owns
`0x00790f60`. This package defines neither object. Decide whether they
are one symbol.

`preConfigure` exists twice (80 bytes at `0x0037f150`, used by the pause
menu, and at `0x004289d0`). This package defines neither object. Decide
whether they are one symbol.

`player` is imported by pre-logo-dialog. items-player already defines the
95,936 zero bytes at `0x00408600` as
`norrath_player_progression_records_view` and does not alias the ELF
name. Decide whether the view should gain that alias.

`c.1089` at `0x007d0988` has ELF size 0 and stays unresolved. The
Camera-owning package should settle its extent before anything defines
it.

`iop-module-boot` exports `norrath_iop_heap_client`, `norrath_loadfile_client`,
`_senddata` and `vik`, and its notes ask `sif-keyboard` to use them. Today
`sif-keyboard-iop-heap-query`, `sif-keyboard-module-calls` and
`sif-keyboard-keyboard` keep private `static` copies, so the link has no
duplicate definition, but the module calls use a client that `sceSifInitIopHeap`
and `_lf_bind` never bind, and `Init__10VIKeyboard` and `textSystemInit__Fv` initialise a
different `vik` than `keyboardFree__Fv` frees. The package owner should make
the three edits listed in `docs/agents/iop-module-boot.md`; they were not
made here because they change game code.

The `engine-restore-srs-main` host contract has the same harness flaw that
kept `agents/frontend-shell` out: `reset()` clears `error`, and `main` returns
`error` only after the first two scenarios and at the end. A failure in the
scenarios at lines 249 to 273 of `host_contract.cpp` is erased by the next
`reset()`. This was found by the integrator after that package had merged; it
is a harness defect, not a source defect, and was not edited here. The owner
should keep the first failing code across resets.

## Disagreements

The symbol disagreements are the items under Needs a decision. No selected
symbol was defined twice.

## Still authored, never executed

Every function and data object added by math, audio, memory, object-infra,
megatile, script-amx, cd-archive, libc-format, items-player, save-net-anim,
engine-init, world-state, render-lights, data-views, actors-b, gameobject-base,
scheduler, game-boot, subsystem-inits, gs-dma-kernel, draw-texture-init, and
frame-loop, dialog-hud-controls, natives-a, natives-c, floor, engine-restore, sif-keyboard, actors-a, the cd-archive second pass, the memory second pass, the audio second pass, the script-amx second pass, vslots-b, light-methods, vehicle-actors, particles, vslots-a, link-image-bounds, ctors-ant-lantern, sound-rpc-views, game-state,
sony-cd, and link-image-bounds second pass, and gs-dma-present, and the
pre-logo-dialog second pass. The
vehicle constructor is authored and not in the link. The unselected
texture-draw-color unit is authored and not in the link. The six
actor-view-tables views are data, not functions. The 26 card-progress
data words are data, not functions. The shared constructor header now
covers Boat and AutoContainerLP. The eleven selected game-loop units
(eight functions, five words) are in the link; the two unselected
game-loop units are authored and not in the link. The twenty-five
memcard-calls units are in the link. The forty natives-b units are in
the link. The fifteen pre-logo-dialog units are in the link. The eleven
simloop-services units are in the link. The three closing gs-dma-present
units (three functions plus one file-local helper) are in the link; the
texture-draw-color unit stays authored and not in the link. The four
pre-logo-dialog second-pass units (four functions plus the file-local
`_ZL9dialogTag` BSS) are in the link.
`engineInit__Ff`,
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
- 2026-10-11 20:12 CDT — `agents/natives-b` — 1002 unresolved imports
- 2026-10-11 20:20 CDT — `agents/pre-logo-dialog` — 1035 unresolved imports
- 2026-10-11 20:28 CDT — `agents/simloop-services` — 1054 unresolved imports
- 2026-10-11 21:21 CDT — `agents/gs-dma-present` — 1056 unresolved imports
- 2026-10-11 22:19 CDT — `agents/pre-logo-dialog` — 1149 unresolved imports
- 2026-10-12 12:31 CDT — `agents/iop-module-boot` — 1152 unresolved imports
- 2026-10-12 12:39 CDT — `agents/floor-world-view` — 1142 unresolved imports
- 2026-10-12 12:41 CDT — `agents/card-progress`, `agents/memcard-calls` (follow-up commits) — 1142 unresolved imports
- 2026-10-12 12:59 CDT — `agents/game-loop` (gameLoop only) — 1199 unresolved imports
- 2026-10-12 13:01 CDT — `agents/vi-launcher` — 1208 unresolved imports
- 2026-10-12 13:05 CDT — `agents/session-roster` — 1201 unresolved imports
- 2026-10-12 13:10 CDT — `agents/simloop-services` — 1227 unresolved imports
- 2026-10-12 13:13 CDT — `agents/kernel-patches` — 1197 unresolved imports
- 2026-10-12 13:15 CDT — `agents/frontend-shell` — 1217 unresolved imports
- 2026-10-12 13:12 CDT — `agents/frontend-shell` (rejected, not merged; log notes only) — 1201 unresolved imports
- 2026-10-12 13:33 CDT — `agents/loop-grid-fx` — 1204 unresolved imports
- 2026-10-12 13:43 CDT — `agents/combat-reactions` — 1195 unresolved imports
- 2026-10-12 13:55 CDT — `agents/actor-view-tables` — 1221 unresolved imports
- 2026-10-12 14:09 CDT — `agents/level-anim-load` — 1208 unresolved imports
- 2026-10-12 14:19 CDT — `frontend-shell contract fix` — 1208 unresolved imports
- 2026-10-12 14:24 CDT — `agents/ctors-ant-lantern` — 1516 unresolved imports
- 2026-10-12 14:26 CDT — `agents/ctors-lava-wraith` — 1671 unresolved imports
- 2026-10-12 14:28 CDT — `agents/inventory-items` — 1686 unresolved imports
- 2026-10-12 14:31 CDT — `agents/save-and-quests` — 1679 unresolved imports
- 2026-10-12 14:36 CDT — `agents/prop-poke-clone` — 1656 unresolved imports
- 2026-10-12 14:38 CDT — `agents/prop-run-draw` — 1658 unresolved imports
- 2026-10-12 14:40 CDT — `agents/w8-actor-virtuals-a` — 1631 unresolved imports
- 2026-10-12 14:42 CDT — `agents/w8-actor-virtuals-b` — 1613 unresolved imports
- 2026-10-12 14:44 CDT — `agents/w8-core-loop-a` — 1592 unresolved imports
- 2026-10-12 14:47 CDT — `agents/w8-core-loop-b` — 1609 unresolved imports
- 2026-10-12 14:48 CDT — `agents/w8-core-loop-c` — 1593 unresolved imports
- 2026-10-12 14:50 CDT — `agents/w8-core-loop-d` — 1629 unresolved imports
- 2026-10-12 14:52 CDT — `agents/w8-frontend-menu-a` — 1608 unresolved imports
- 2026-10-12 14:56 CDT — `agents/w8-frontend-menu-b` — 1589 unresolved imports
- 2026-10-12 14:58 CDT — `agents/w8-net-play-a` — 1560 unresolved imports
- 2026-10-12 15:00 CDT — `agents/w8-pad-memcard-a` — 1607 unresolved imports
- 2026-10-12 15:03 CDT — `agents/w8-pad-memcard-b` — 1615 unresolved imports
- 2026-10-12 15:05 CDT — `agents/w8-runtime-libc-a` — 1610 unresolved imports
- 2026-10-12 15:07 CDT — `agents/w8-runtime-libc-b` — 1593 unresolved imports
- 2026-10-12 15:09 CDT — `agents/w8-spell-static-init-b (rejected, not merged; log notes only)` — 1593 unresolved imports
- 2026-10-12 15:12 CDT — `agents/w8-startup-sce-a` — 1586 unresolved imports
- 2026-10-12 15:15 CDT — `agents/w8-startup-sce-b` — 1580 unresolved imports
- 2026-10-12 15:17 CDT — `agents/w8-startup-sce-c` — 1563 unresolved imports
- 2026-10-12 15:19 CDT — `agents/w8-startup-sce-d` — 1551 unresolved imports
- 2026-10-12 15:24 CDT — `agents/w8-actor-virtuals-c` — 1562 unresolved imports
- 2026-10-12 15:27 CDT — `agents/w8-audio-calls` — 1540 unresolved imports
- 2026-10-12 15:28 CDT — `agents/w8-camera-vehicle-a` — 1537 unresolved imports
- 2026-10-12 15:34 CDT — `agents/w8-camera-vehicle-b` — 1541 unresolved imports
- 2026-10-12 15:37 CDT — `agents/w8-core-loop-e` — 1560 unresolved imports
- 2026-10-12 15:38 CDT — `agents/w8-fx-particles` — 1538 unresolved imports
- 2026-10-12 15:41 CDT — `agents/w8-hud-text-items-a` — 1526 unresolved imports
- 2026-10-12 15:42 CDT — `agents/w8-hud-text-items-b` — 1526 unresolved imports
- 2026-10-12 15:45 CDT — `agents/w8-hud-text-items-c` — 1516 unresolved imports
- 2026-10-12 15:48 CDT — `agents/w8-net-play-c` — 1533 unresolved imports
- 2026-10-12 15:51 CDT — `agents/w8-object-list-math` — 1491 unresolved imports
- 2026-10-12 15:53 CDT — `agents/w8-render-world-b` — 1511 unresolved imports
- 2026-10-12 15:56 CDT — `agents/w8-spell-static-init-a` — 1478 unresolved imports
- 2026-10-12 16:15 CDT — `agents/w8-render-world-a` — 1514 unresolved imports
- 2026-10-12 20:02 CDT — `agents/w9-startup-crt-a` — 1499 unresolved imports
- 2026-10-12 20:05 CDT — `agents/w9-startup-crt-c` — 1496 unresolved imports
- 2026-10-12 20:10 CDT — `agents/w9-startup-crt-e` — 1484 unresolved imports
- 2026-10-12 20:16 CDT — `agents/w9-audio` — 1473 unresolved imports
- 2026-10-12 20:23 CDT — `agents/w9-frontend-c` — 1453 unresolved imports
- 2026-10-12 20:32 CDT — `agents/w9-other-a` — 1461 unresolved imports
- 2026-10-12 20:37 CDT — `agents/w9-other-b` — 1437 unresolved imports
- 2026-10-12 20:45 CDT — `agents/runtime-tables` — 1436 unresolved imports
- 2026-10-12 20:59 CDT — `agents/w9-startup-crt-b` — 1472 unresolved imports
- 2026-10-12 21:06 CDT — `agents/w9-startup-crt-d` — 1467 unresolved imports
- 2026-10-12 21:10 CDT — `agents/w9-startup-crt-f` — 1455 unresolved imports
- 2026-10-12 21:13 CDT — `agents/w9-frontend-b` — 1495 unresolved imports
- 2026-10-12 21:16 CDT — `agents/w9-frontend-d` — 1478 unresolved imports
- 2026-10-12 21:19 CDT — `agents/w9-frontend-a` — 1525 unresolved imports
- 2026-10-12 21:27 CDT — `agents/w9-camera-map` — 1527 unresolved imports
- 2026-10-12 21:31 CDT — `agents/w9-hud-inventory-items-b` — 1523 unresolved imports
- 2026-10-12 21:34 CDT — `agents/w9-render-math-b` — 1502 unresolved imports
- 2026-10-12 21:37 CDT — `agents/w9-animation-object-a` — 1478 unresolved imports
- 2026-10-12 21:40 CDT — `agents/w9-ctors-lantern-a` — 1584 unresolved imports
