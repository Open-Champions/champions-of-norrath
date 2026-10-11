#include "tables.hpp"

unsigned char norrath_megatile_regions_view[0x140];
unsigned char norrath_megatile_x_view[0x400];
unsigned char norrath_megatile_y_view[0x400];
unsigned char norrath_megatile_grid_origin_view[0x18];
unsigned char norrath_megatile_pm_pointers_view[0x400];
unsigned char norrath_megatile_pm_characters_view[0x1000];
unsigned char norrath_megatile_dynlevels_view[0x1c0];
unsigned char norrath_megatile_level_name_view[0x142];
unsigned char norrath_megatile_counters_view[20];
unsigned char norrath_megatile_world_word[4];

const unsigned char norrath_megatile_scale_view[4] = {0x60, 0x0b, 0xb6, 0x3a};
const unsigned char norrath_megatile_closest_limit_view[4] = {0xeb, 0x78, 0xad, 0x60};

const unsigned char norrath_megatile_name_a_view[] = "zzTileType";
const unsigned char norrath_megatile_name_b_view[] = "EdgeMarker";
const unsigned char norrath_megatile_name_c_view[] = "Directional_Light";
const unsigned char norrath_megatile_name_d_view[] = "Ambient_Light";
const unsigned char norrath_megatile_name_e_view[] = "clearColor";
const unsigned char norrath_megatile_name_f_view[] = "customVars";
const unsigned char norrath_megatile_pm_name_view[] = "Line";
const unsigned char norrath_megatile_pm_a_view[] = "name=barrier";
const unsigned char norrath_megatile_pm_b_view[] = "continous=1";
const unsigned char norrath_megatile_pm_c_view[] = "norotate=1";
const unsigned char norrath_megatile_count_format_view[] = "pcount=-%d";
const unsigned char norrath_megatile_point_format_view[] = "p=%f,%f,0";

const unsigned char norrath_megatile_trigger_text[] = "trigger";
const unsigned char norrath_megatile_width_key[] = "w=";
const unsigned char norrath_megatile_height_key[] = "h=";
const unsigned char norrath_megatile_bang_text[] = "!!!";
const unsigned char norrath_megatile_dynamic_new_world_text[] = "!!!dynamicnewworld";
const unsigned char norrath_megatile_scan_pair_format[] = "%[^,],%d";
const unsigned char norrath_megatile_suffix_number_format[] = "%s%d";
const unsigned char norrath_megatile_level_assignment_format[] = "level=%s";
const unsigned char norrath_megatile_dynamic_shift_text[] = "!!!dynamicshift";
const unsigned char norrath_megatile_pair_join_format[] = "%s_%s";
const unsigned char norrath_megatile_decimal_format[] = "%d";
const unsigned char norrath_megatile_dynamic_orientation_text[] = "!!!dynamicorientation";
const unsigned char norrath_megatile_orientation_format[] = "orientation=%d";
const unsigned char norrath_megatile_closest_level_text[] = "!!!closestdynamiclevel";
const unsigned char norrath_megatile_from_format[] = "from=%s";
const unsigned char norrath_megatile_width_format[] = "w=%d";
const unsigned char norrath_megatile_height_format[] = "h=%d";
const unsigned char norrath_megatile_mspawn_text[] = "MSpawn";
const unsigned char norrath_megatile_push_trigger_text[] = "PushTrigger";
const unsigned char norrath_megatile_kitten_text[] = "kitten";
const unsigned char norrath_megatile_kitten_name_format[] = "name=kitten%d";
