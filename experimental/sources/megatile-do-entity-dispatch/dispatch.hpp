#pragma once

// doEntityDispatch__FP8MTObjDefiP13delayedEntityPib at 0x002695e0, size 0x59c.
// a0 record, a1 region index, a2 delayed-record base, a3 count word, t0 filter.
extern "C" {
extern unsigned char norrath_megatile_regions_view[];
extern unsigned char norrath_megatile_counters_view[];
extern unsigned char norrath_megatile_pm_pointers_view[];
extern unsigned char norrath_restore_filter_word[];
extern unsigned char norrath_factory_ctype_minimum_view[];
extern const unsigned char norrath_megatile_mspawn_text[];
extern const unsigned char norrath_megatile_bang_text[];
extern const unsigned char norrath_megatile_trigger_text[];
extern const unsigned char norrath_megatile_push_trigger_text[];
extern const unsigned char norrath_megatile_kitten_text[];
extern const unsigned char norrath_megatile_kitten_name_format[];

int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_strcasecmp(const char* left, const char* right);
char* norrath_factory_strstr(const char* text, const char* needle);
int norrath_factory_sprintf(char* output, const char* format, ...);
char* norrath_megatile_add_pm_list(char* text);
char** norrath_megatile_cap_pm_pointers();
char** norrath_megatile_specialparse(char** arguments, int selector, int mode,
                                    const unsigned char* point, char* name);
void norrath_monster_spawn(char** arguments, const void* point, short type_word, bool filter);
unsigned char* norrath_object_create_by_name(char* name, const unsigned char* point,
                                            int type_word, char** arguments);

void norrath_megatile_do_entity_dispatch(unsigned char* record, int index,
                                        unsigned char* delayed, int* count, bool filter);
}

#if defined(NORRATH_MEGATILE_HOST)
extern "C" void* norrath_megatile_host_pointer(unsigned id);
extern "C" void norrath_megatile_host_note_word(const unsigned char* cell, unsigned value);
extern "C" void norrath_megatile_host_note_store(const unsigned char* cell, const void* value);
extern "C" int norrath_megatile_host_filter(char* name, unsigned code_word);
#endif
