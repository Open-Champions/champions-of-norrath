#pragma once
#include "core_formatter.hpp"
using NorrathScannerSigned64 = long long;
static_assert(sizeof(void*)==4 && sizeof(int)==4 && sizeof(long long)==8,"original target32 words/full64 scanner values");
extern "C" {
int norrath_player_sscanf(const char*,const char*,...);
int norrath_player_stream_scan(unsigned char*,const char*,const unsigned char*);
// Original LOCAL eofread8 is source-only private callback, not original public alias.
int norrath_player_string_eof_read(unsigned char*,char*,int);
int norrath_player_stream_refill(unsigned char*);
unsigned norrath_player_stream_read_elements(void*,unsigned,unsigned,unsigned char*);
int norrath_player_stream_unget(int,unsigned char*);
const char* norrath_player_scan_character_class(unsigned char*,const char*);
NorrathScannerSigned64 norrath_player_atol(const char*);
NorrathScannerSigned64 norrath_factory_strtol(const char*,char**,int);
NorrathFormatUnsigned64 norrath_player_strtoul(const char*,char**,int);
NorrathFormatUnsigned64 norrath_player_atof(const char*);
float norrath_player_double_to_float(NorrathFormatUnsigned64);
extern const unsigned char norrath_factory_ctype_minimum_view[];
// Actual readonly base-normalization table17 signed halves and literal e%ld.
extern const unsigned char norrath_player_scan_base_normalization_view[];
extern const unsigned char norrath_player_scan_exponent_format_view[];
}
namespace norrath_scanner_detail {
using namespace norrath_core_format_detail;
inline char* text(Word a){return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(a));}
inline unsigned classify(int c){return norrath_factory_ctype_minimum_view[Word(c+128)];}
inline int available(Word file){return signed_word(read_word(file+4u));}
inline Word next_destination(Word& arguments){const Word slot=arguments;arguments+=8u;return read_word(slot);}
inline int signed_half(Word a){Word h=read_half(a);return h<0x8000u?int(h):int(h)-0x10000;}
inline NorrathScannerSigned64 signed_wide(Wide w){return w<=0x7fffffffffffffffULL?NorrathScannerSigned64(w):-1LL-NorrathScannerSigned64(~w);}
inline void store_float(Word a,float value){write_word(a,read_word(address(&value)));}
// Original integer/float whitespace paths store decremented count first. At
// exhaustion they call the genuine refill with the old cursor still present.
inline bool consume_to_refill(unsigned char* stream,Word file){
    Word count=read_word(file+4u)-1u;write_word(file+4u,count);
    if(signed_word(count)>0){write_word(file,read_word(file)+1u);return true;}
    return norrath_player_stream_refill(stream)==0;
}
}
