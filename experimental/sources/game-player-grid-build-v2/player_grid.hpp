#pragma once
#include "dispatch.hpp"
#include "constructor.hpp"
extern "C" {
void norrath_object_update_in_grid(void* object);
NorrathPoint3* norrath_object_find_empty_spot(NorrathPoint3* destination, NorrathPoint3 origin, int distance, int clearance, char* result_flag);
void** norrath_object_find_in_box(NorrathPoint3 minimum, NorrathPoint3 maximum, void** output, int capacity);
// Genuine world helpers use the existing observed byte-pointer ABI.
float norrath_world_find_height(unsigned char* world, const unsigned char* point, float radius);
float norrath_world_test_forbidden(unsigned char* world, const unsigned char* point, float radius);
extern unsigned char norrath_factory_world_pointer_word[];
extern unsigned char norrath_grid_ignore_flags_view[];
extern const unsigned char norrath_grid_vertical_extent_view[];
}
namespace norrath_player_grid_detail {
using namespace norrath_constructor_detail;
inline void put_float(Address a, float value) { write_word(a, read_word(address(&value))); }
inline unsigned shifted(unsigned value, unsigned bits) {
    return (value >> bits) | ((value & 0x80000000u) ? (~0u << (32u - bits)) : 0u);
}
inline int biased_cell(float value, unsigned bias) {
    return signed_word(shifted(unsigned(static_cast<int>(value)) + bias, 8u));
}
inline void copy_point(Address destination, Address source) {
    const unsigned x = read_word(source), y = read_word(source + 4u), z = read_word(source + 8u);
    write_word(destination, x); write_word(destination + 4u, y); write_word(destination + 8u, z);
}
inline NorrathPoint3 point_value(Address source) {
    NorrathPoint3 value; copy_point(address(&value), source); return value;
}
inline unsigned random_step(unsigned seed) { return seed * 0x0019660du + 0x3c6ef35fu; }
inline float constant(unsigned word) { float value; write_word(address(&value), word); return value; }
inline float random_component(unsigned seed) {
    const float unit = static_cast<float>(seed >> 16) * constant(0x37800000u);
    const float doubled = unit + unit;
    return doubled - 1.0f;
}
}
