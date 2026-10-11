#pragma once
#include "constructor.hpp"
extern "C" {
extern unsigned char norrath_constructor_lump_count_view[];
extern unsigned char norrath_constructor_lump_cache_view[];
extern unsigned char norrath_constructor_list_pool_view[];
extern unsigned char norrath_constructor_string_count_view[];
extern unsigned char norrath_constructor_string_names_view[];
extern unsigned char norrath_constructor_string_objects_view[];
int norrath_string_compare(const char* left, const char* right);
void* norrath_lump_load(char* lump_name);
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_atoi(const char* text);
int norrath_factory_strcasecmp(const char* left, const char* right);
char* norrath_factory_strcpy(char* destination, const char* source);
char* norrath_factory_strchr(const char* text, int character);
}
namespace norrath_constructor_dependency_detail {
using namespace norrath_constructor_detail;
inline unsigned read_half(Address a) {
    return unsigned(bytes(a)[0]) | (unsigned(bytes(a)[1]) << 8);
}
inline void write_half(Address a, unsigned value) {
    bytes(a)[0] = static_cast<unsigned char>(value);
    bytes(a)[1] = static_cast<unsigned char>(value >> 8);
}
inline unsigned arithmetic_shift(unsigned value, unsigned bits) {
    return (value >> bits) | ((value & 0x80000000u) != 0u ? (~0u << (32u - bits)) : 0u);
}
}
