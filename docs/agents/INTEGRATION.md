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
