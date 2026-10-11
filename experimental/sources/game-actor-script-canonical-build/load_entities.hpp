#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4,
              "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 byte-view address bridge");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
extern unsigned char norrath_mc_saved_level_index_bytes[];
extern unsigned char norrath_mc_saved_level_payload_bytes[];
extern unsigned char norrath_mc_load_buffer_bytes[];
extern unsigned char norrath_mc_lump_format_bytes[];
extern unsigned char norrath_mc_script_section_key_bytes[];
extern unsigned char norrath_mc_pre_start_key_bytes[];
extern unsigned char norrath_mc_objects_section_key_bytes[];
extern unsigned char norrath_mc_start_key_bytes[];
extern unsigned char norrath_keep_player_positions_bytes[];
extern unsigned char norrath_got_player_start_this_level_bytes[];
extern unsigned char norrath_mc_disable_startup_script_bytes[];
extern unsigned char norrath_route_world_word_bytes[];
int norrath_string_compare(const char*, const char*);
int norrath_format_string(char*, const char*, ...);
void* norrath_lump_load(char*);
unsigned char* norrath_lump_find(unsigned char*, char*);
void norrath_script_set_script(unsigned char*);
int norrath_script_start(char*);
void* norrath_copy_bytes(void*, const void*, __SIZE_TYPE__);
// Observed argument lanes, deliberately independent of the old mangled class name.
void norrath_map_read_saved_bytes(unsigned char*);
unsigned norrath_object_put_save_data(unsigned char*, int);
unsigned norrath_floor_put_save_info(unsigned char*);
void norrath_megatile_entitize(unsigned char*, int);
void norrath_mc_load_level_entities(char* lump_name, char* saved_level_name);
}
