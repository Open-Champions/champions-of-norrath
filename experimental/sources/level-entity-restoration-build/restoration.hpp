#pragma once
#include "dispatch.hpp"
#include "constructor.hpp"
static_assert(sizeof(unsigned long long) == 8, "observed software-double GPR transport");
extern "C" {
extern unsigned char norrath_restore_defaults_view[];
extern unsigned char norrath_restore_string_base_word[];
extern unsigned char norrath_restore_filter_word[];
extern unsigned char norrath_restore_broadcast_head_view[];
extern unsigned char norrath_restore_broadcast_mask_word[];
extern unsigned char norrath_megatile_counters_view[];
extern const unsigned char norrath_megatile_scale_view[];
extern unsigned char norrath_megatile_regions_view[];
extern unsigned char norrath_megatile_world_word[];
extern unsigned char norrath_megatile_x_view[];
extern unsigned char norrath_megatile_y_view[];
extern unsigned char norrath_megatile_grid_origin_view[];
extern unsigned char norrath_megatile_pm_pointers_view[];
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
// Honest genuine direct dependencies; no original-code bindings or responses.
void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
void norrath_list_safe_iterator_next(unsigned char* iterator);
void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
void norrath_floor_clear();
void norrath_floor_add(const unsigned char* point, int type_word, float extent, int kind, int flags);
char* norrath_factory_strstr(const char* text, const char* needle);
int norrath_string_compare(const char* left, const char* right);
float norrath_entity_floorf(float value);
void norrath_megatile_do_entity_dispatch(unsigned char* record, int index,
                                        unsigned char* delayed, int* count, bool filter);
char** norrath_megatile_specialparse(char** arguments, int selector, int mode,
                                    const unsigned char* point, char* name);
char* norrath_megatile_add_pm_list(char* text);
char** norrath_megatile_cap_pm_pointers();
int norrath_factory_sprintf(char* output, const char* format, ...);
// Original single F12 -> raw software-double64 in GPR V0; no host cast.
unsigned long long norrath_entity_fptodp(float value);
void norrath_object_process_delete_list();
unsigned norrath_object_put_save_data(unsigned char* input, int filter_requested);
unsigned norrath_floor_put_save_info(unsigned char* input);
void norrath_megatile_entitize(unsigned char* input, int filter_requested);
}
namespace norrath_restoration_detail {
using namespace norrath_constructor_detail;
inline unsigned read_half(Address a) { return unsigned(bytes(a)[0]) | (unsigned(bytes(a)[1]) << 8); }
inline int signed_half(unsigned value) { return value < 0x8000u ? int(value) : int(value) - 65536; }
inline void write_half(Address a, unsigned value) {
    bytes(a)[0] = static_cast<unsigned char>(value);
    bytes(a)[1] = static_cast<unsigned char>(value >> 8);
}
inline char** arguments(Address a) { return reinterpret_cast<char**>(static_cast<__UINTPTR_TYPE__>(a)); }
inline void copy_point(Address destination, Address source) {
    const unsigned x = read_word(source), y = read_word(source + 4u), z = read_word(source + 8u);
    write_word(destination, x); write_word(destination + 4u, y); write_word(destination + 8u, z);
}
inline void write_float(Address a, float value) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&value);
    const unsigned word = unsigned(p[0]) | (unsigned(p[1]) << 8) |
                          (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
    write_word(a, word);
}
using Filter = int (*)(char*);
using SaveVirtual = void (*)(unsigned char*, unsigned char*);
using BroadcastVirtual = void (*)(unsigned char*, int);
inline bool filter_allows(Address name) {
    const Address target = read_word(address(norrath_restore_filter_word));
    if (target == 0u) return false;
    const Filter function = reinterpret_cast<Filter>(static_cast<__UINTPTR_TYPE__>(target));
    return function(text(name)) != 0;
}
inline void initial_integer_defaults() {
    const Address d = address(norrath_restore_defaults_view);
    write_word(d + 8u, 32u); write_word(d + 20u, 68u);
    write_word(d, 32u); write_word(d + 4u, 32u);
    write_word(d + 12u, 68u); write_word(d + 16u, 68u);
}
inline void append_pm_pointer(Address value) {
    const Address count_word = address(norrath_megatile_counters_view) + 4u;
    const unsigned count = read_word(count_word);
    const Address cell = address(norrath_megatile_pm_pointers_view) + 4u * count;
    write_word(count_word, count + 1u);
    write_word(cell, value);
}
}
