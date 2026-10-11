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

## Disagreements

The two items under Needs a decision are the only symbol disagreements among
the packages selected so far. No symbol in those packages was defined twice.

## Still authored, never executed

Every function and data object added by math, audio, memory, object-infra,
and megatile.
Host checks ran on this machine. The reviewed EE compiler produced relocatable
objects and the partial link succeeded. No routine from these packages has
run on the EE, under the original process, or as gameplay. The same is true
of the startup bodies already on the shared branch: they are selected source,
and no startup stage has executed.

## Push log

- 2026-10-11 16:23 CDT — `agents/megatile` — 626 unresolved imports
