#pragma once
#include "core_formatter.hpp"
#include "constructor.hpp"
extern "C" {
float norrath_player_find_tag_float(void*,char*,int);
NorrathFormatUnsigned64 norrath_player_atof(const char*);
NorrathFormatUnsigned64 norrath_player_strtod(const char*,char**);
NorrathFormatUnsigned64 norrath_player_strtod_reentrant(unsigned char*,const char*,char**);
float norrath_player_double_to_float(NorrathFormatUnsigned64);
// Full software representation helpers, genuine source rather than host casts.
void norrath_player_unpack_double(const unsigned char*,unsigned char*);
float norrath_player_make_float(unsigned,int,int,unsigned);
float norrath_player_pack_float(const unsigned char*);
}
namespace norrath_float_tags_detail {
using namespace norrath_constructor_detail;
using Wide=NorrathFormatUnsigned64;
inline Wide wide(Address a){return Wide(read_word(a))|Wide(read_word(a+4u))<<32;}
inline void put_wide(Address a,Wide word){write_word(a,unsigned(word));write_word(a+4u,unsigned(word>>32));}
inline float float_bits(unsigned word){float value;write_word(address(&value),word);return value;}
}
