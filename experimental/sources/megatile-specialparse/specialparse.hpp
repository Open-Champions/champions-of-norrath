#pragma once

// specialparse__FPPciiG6Point3Pc at 0x00268928, size 0x630.
// Arguments are the observed register transport: a0 argument list, a1 selector,
// a2 mode, a3 pointer to the 12-byte point, t0 name.
extern "C" {
extern unsigned char norrath_megatile_counters_view[];
extern unsigned char norrath_megatile_pm_pointers_view[];
extern unsigned char norrath_megatile_dynlevels_view[];
extern unsigned char norrath_megatile_level_name_view[];
extern const unsigned char norrath_megatile_closest_limit_view[];
extern unsigned char norrath_factory_ctype_minimum_view[];
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

int norrath_factory_strcasecmp(const char* left, const char* right);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_atoi(const char* text);
char* norrath_factory_strstr(const char* text, const char* needle);
char* norrath_factory_strchr(const char* text, int character);
int norrath_factory_sscanf(const char* text, const char* format, ...);
int norrath_factory_sprintf(char* output, const char* format, ...);
char* norrath_factory_strcpy(char* destination, const char* source);
char* norrath_megatile_add_pm_list(char* text);
char** norrath_megatile_cap_pm_pointers();
// Observed callee swap__H1Zi_RX01T0_v exchanges the two addressed words.
void norrath_swap_int(int* left, int* right);

char** norrath_megatile_specialparse(char** arguments, int selector, int mode,
                                    const unsigned char* point, char* name);
}

#if defined(NORRATH_MEGATILE_HOST)
extern "C" void norrath_megatile_host_note_word(const unsigned char* cell, unsigned value);
extern "C" void norrath_megatile_host_note_store(const unsigned char* cell, const void* value);
#endif
