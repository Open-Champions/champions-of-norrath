#pragma once

// Observed initial bytes of the megatile tables. Mutable objects are the
// zero-filled symbol extents. Text objects are the NUL-terminated literals;
// alignment padding between those literals is not part of any symbol.

extern unsigned char norrath_megatile_regions_view[0x140];
extern unsigned char norrath_megatile_x_view[0x400];
extern unsigned char norrath_megatile_y_view[0x400];
extern unsigned char norrath_megatile_grid_origin_view[0x18];
extern unsigned char norrath_megatile_pm_pointers_view[0x400];
extern unsigned char norrath_megatile_pm_characters_view[0x1000];
extern unsigned char norrath_megatile_dynlevels_view[0x1c0];
extern unsigned char norrath_megatile_level_name_view[0x142];
extern unsigned char norrath_megatile_counters_view[20];
extern unsigned char norrath_megatile_world_word[4];
extern const unsigned char norrath_megatile_scale_view[4];
extern const unsigned char norrath_megatile_closest_limit_view[4];

extern const unsigned char norrath_megatile_name_a_view[];
extern const unsigned char norrath_megatile_name_b_view[];
extern const unsigned char norrath_megatile_name_c_view[];
extern const unsigned char norrath_megatile_name_d_view[];
extern const unsigned char norrath_megatile_name_e_view[];
extern const unsigned char norrath_megatile_name_f_view[];
extern const unsigned char norrath_megatile_pm_name_view[];
extern const unsigned char norrath_megatile_pm_a_view[];
extern const unsigned char norrath_megatile_pm_b_view[];
extern const unsigned char norrath_megatile_pm_c_view[];
extern const unsigned char norrath_megatile_count_format_view[];
extern const unsigned char norrath_megatile_point_format_view[];

extern const unsigned char norrath_megatile_trigger_text[];
extern const unsigned char norrath_megatile_width_key[];
extern const unsigned char norrath_megatile_height_key[];
extern const unsigned char norrath_megatile_bang_text[];
extern const unsigned char norrath_megatile_dynamic_new_world_text[];
extern const unsigned char norrath_megatile_scan_pair_format[];
extern const unsigned char norrath_megatile_suffix_number_format[];
extern const unsigned char norrath_megatile_level_assignment_format[];
extern const unsigned char norrath_megatile_dynamic_shift_text[];
extern const unsigned char norrath_megatile_pair_join_format[];
extern const unsigned char norrath_megatile_decimal_format[];
extern const unsigned char norrath_megatile_dynamic_orientation_text[];
extern const unsigned char norrath_megatile_orientation_format[];
extern const unsigned char norrath_megatile_closest_level_text[];
extern const unsigned char norrath_megatile_from_format[];
extern const unsigned char norrath_megatile_width_format[];
extern const unsigned char norrath_megatile_height_format[];
extern const unsigned char norrath_megatile_mspawn_text[];
extern const unsigned char norrath_megatile_push_trigger_text[];
extern const unsigned char norrath_megatile_kitten_text[];
extern const unsigned char norrath_megatile_kitten_name_format[];
