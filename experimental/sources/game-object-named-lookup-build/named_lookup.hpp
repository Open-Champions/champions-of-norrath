#pragma once
#include "constructor.hpp"
#include "dependencies.hpp"
static_assert(sizeof(unsigned long long) == 8, "observed native64 return and capacity lanes");
extern "C" {
// Exact indexed LOCAL NOTYPE address views; no table owner or data initializer.
extern unsigned char norrath_object_name_hash_slots_view[];
extern unsigned char norrath_object_pointer_hash_slots_view[];
// Machine return is the sign extension of the terminating-slot address.
unsigned long long norrath_object_find_named_list(
    char* name, void** output, unsigned long long capacity);
// Machine return is the sign extension of the selected object word, or zero.
unsigned long long norrath_object_find_named(char* name);
}
