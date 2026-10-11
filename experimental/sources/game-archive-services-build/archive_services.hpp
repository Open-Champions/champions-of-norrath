#pragma once
#include "loader_primitives.hpp"
static_assert(sizeof(__SIZE_TYPE__) == 4, "original memory byte counts are target32");
extern "C" {
void norrath_cd_wait();
void norrath_mem_check_heap_real(const char*, int);
void* norrath_mem_realloc(void*, int);
void* norrath_copy_bytes(void*, const void*, __SIZE_TYPE__);
void* norrath_archive_move_bytes(void*, const void*, unsigned);
char* norrath_archive_last_character(const char*, int);
unsigned norrath_archive_initialize_file(char*, void*, int);
void norrath_archive_map_animation_sounds(void*);
unsigned norrath_archive_sound_free_ram();
void norrath_archive_engine_push_border(unsigned);
void norrath_archive_engine_pop_border();
void norrath_archive_engine_sync_vblank();
int norrath_archive_cd_layer_search(unsigned char*, const char*, int);
int norrath_archive_cd_read(unsigned, unsigned, void*, const unsigned char*);
int norrath_archive_cd_read_iop(unsigned, unsigned, void*, const unsigned char*);
int norrath_archive_cd_sync(int);
int norrath_archive_cd_error();
int norrath_archive_cd_stream_read(unsigned, void*, int, int*);
int norrath_archive_cd_stream_pause();
int norrath_factory_sprintf(char*, const char*, ...);
unsigned norrath_factory_strlen(const char*);
int norrath_factory_strncmp(const char*, const char*, unsigned);
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern unsigned char norrath_archive_directory_pointer_view[]; // 483ff0
extern unsigned char norrath_archive_directory_switch_view[]; // 483ff4
extern unsigned char norrath_archive_layer_bias_view[]; // 483ff8
extern unsigned char norrath_archive_read_active_view[]; // 483ffc
extern unsigned char norrath_archive_stream_active_view[]; // 483ffd
extern unsigned char norrath_archive_stream_destination_view[]; // 484008
extern unsigned char norrath_archive_stream_remaining_view[]; // 48400c
extern unsigned char norrath_archive_stream_restart_view[]; // 484010
extern unsigned char norrath_archive_read_mode_view[]; // 486920; byte1 is the lookup's layer flag
extern unsigned char norrath_archive_completion_pointer_view[]; // 486924
extern unsigned char norrath_archive_permanent_halt_flag_view[]; // 4865f8
extern unsigned char norrath_archive_command_view[]; // 6d86b0 four words
extern unsigned char norrath_archive_directory_a_view[]; // 6cc6a0 rows32/count24576
extern unsigned char norrath_archive_directory_b_view[]; // 6d26a8 rows32/count24576
extern const unsigned char norrath_archive_lookup_format_view[]; // 484000
extern const unsigned char norrath_archive_directory_name_view[]; // 4633e0
extern const unsigned char norrath_archive_load_failure_format_view[]; // 4633f0
extern const unsigned char norrath_archive_zero_record_format_view[]; // 4633c0
extern const unsigned char norrath_archive_check_file_view[]; // 483fd8
extern const unsigned char norrath_archive_prefix_five_view[]; // 483fe0
extern const unsigned char norrath_archive_prefix_four_view[]; // 483fe8
extern const unsigned char norrath_archive_animation_extension_view[]; // 483f98
}
