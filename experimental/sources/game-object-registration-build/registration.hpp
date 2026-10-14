#pragma once
#include "factory.hpp"
using NorrathNameFilter = int (*)(char*);
extern "C" {
// Genuine original callback owner: userFilterFunction4, initially zero.
extern unsigned char norrath_user_filter_function_word[];
// Actual mutable LOCAL-sized76-byte neverSave pointer table remains an import.
extern unsigned char norrath_game_never_save_table_view[];
void norrath_object_set_filter_function(NorrathNameFilter function);
int norrath_game_never_save_always_load_from_lump(char* name);
void norrath_game_object_registration_init();
}
namespace norrath_registration_detail {
using Word = unsigned;
inline Word address(const void* p) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(Word a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline Word read_word(Word a) {
    const unsigned char* p = bytes(a);
    return Word(p[0]) | Word(p[1]) << 8 | Word(p[2]) << 16 | Word(p[3]) << 24;
}
inline void write_word(Word a, Word value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

namespace norrath_registration_detail {
template<class Left, class Right> struct SameType { static constexpr bool value = false; };
template<class Type> struct SameType<Type, Type> { static constexpr bool value = true; };
static_assert(SameType<NorrathCreateByName, decltype(&norrath_game_create_by_name)>::value,
    "registered factory must match the frozen Point3-by-value callback type");
static_assert(SameType<NorrathNameFilter, decltype(&norrath_game_never_save_always_load_from_lump)>::value,
    "registered filter must match its actual char-pointer/int callback type");
}
