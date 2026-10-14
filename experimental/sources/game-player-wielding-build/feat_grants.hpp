#pragma once
#include "skill_progression.hpp"
extern "C" {
void* norrath_block_memory_set(void*,int,unsigned);
char* norrath_factory_strcpy(char*,const char*);
int norrath_string_compare(const char*,const char*);
// Actual gSkillTree GLOBAL17960@422b90, and spellTable.1065 LOCAL2288@4278b8.
// The two exact minimum literals are original views9@468f00 and4@484a88.
// All remain imports; no candidate storage, descriptor owner or table payload.
extern unsigned char norrath_player_feat_rank_catalog_view[];
extern unsigned char norrath_player_feat_spell_table_view[];
extern unsigned char norrath_player_skill_placeholder_view[];
extern unsigned char norrath_player_feat_end_name_view[];
}
namespace norrath_feat_grants_detail {
using namespace norrath_skill_progression_detail;
using Wide=unsigned long long;
inline unsigned read_byte_fresh(Address a){const volatile unsigned char* p=bytes(a);return *p;}
inline int read_signed_byte_fresh(Address a){return signed_byte(static_cast<unsigned char>(read_byte_fresh(a)));}
inline void write_byte_ordered(Address a,unsigned value){volatile unsigned char* p=bytes(a);*p=static_cast<unsigned char>(value);}
inline Wide read_wide_fresh(Address a){
    const unsigned low=read_word_fresh(a),high=read_word_fresh(a+4u);
    return Wide(low)|(Wide(high)<<32);
}
inline void write_wide_ordered(Address a,Wide value){
    write_word_ordered(a,unsigned(value));write_word_ordered(a+4u,unsigned(value>>32));
}
inline float read_float_fresh(Address a){
    const unsigned bits=read_word_fresh(a);float value;
    unsigned char* p=reinterpret_cast<unsigned char*>(&value);
    p[0]=static_cast<unsigned char>(bits);p[1]=static_cast<unsigned char>(bits>>8);
    p[2]=static_cast<unsigned char>(bits>>16);p[3]=static_cast<unsigned char>(bits>>24);
    return value;
}
inline void write_float_ordered(Address a,float value){
    const unsigned char* p=reinterpret_cast<const unsigned char*>(&value);
    const unsigned bits=unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);
    write_word_ordered(a,bits);
}
inline Address inventory_slot(Address record,int index){return record+21976u+unsigned(index)*104u;}
inline Address rank_catalog(Address record){
    const unsigned character_class=read_word_fresh(record+80u);
    return address(norrath_player_feat_rank_catalog_view)+40u+character_class*3592u;
}
}
