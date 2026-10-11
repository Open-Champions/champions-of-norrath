#pragma once
#include "health_regeneration.hpp"
extern "C" {
void norrath_player_regenerate_energy(void* actor);
void* norrath_creature_find_running_spell(void* actor,const void* type);
int norrath_spell_blessed_regeneration(void* spell);
int norrath_skill_regeneration_amount(void* descriptor,int rank);
const void* norrath_spell_blessed_type_info();
int norrath_type_info_equal(const void* left,const void* right);
void norrath_type_info_init_user(void* storage,const char* name);
void norrath_type_info_init_single_inheritance(void* storage,const char* name,const void* base);
int norrath_string_compare(const char* left,const char* right);
// Six original anonymous GP words remain observations, not new owned constants.
extern const unsigned char norrath_player_energy_coefficients_view[];
extern const unsigned char norrath_spell_type_name_view[];
extern const unsigned char norrath_spell_blessed_type_name_view[];
extern const unsigned char norrath_legacy_user_type_info_vtable_view[];
extern const unsigned char norrath_legacy_single_inheritance_type_info_vtable_view[];
// Exact genuine NOBITS owners; no host type_info layout or allocator inferred.
extern unsigned char norrath_spell_type_info_state[8];
extern unsigned char norrath_spell_blessed_type_info_state[12];
}
