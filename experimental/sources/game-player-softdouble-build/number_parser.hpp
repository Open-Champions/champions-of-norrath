#pragma once
#include "formatter_helpers.hpp"
extern "C" {
NorrathFormatUnsigned64 norrath_player_strtod_reentrant(unsigned char*,const char*,char**);
NorrathFormatUnsigned64 norrath_player_double_add(NorrathFormatUnsigned64,NorrathFormatUnsigned64);
NorrathFormatUnsigned64 norrath_player_double_multiply(NorrathFormatUnsigned64,NorrathFormatUnsigned64);
NorrathFormatUnsigned64 norrath_player_double_divide(NorrathFormatUnsigned64,NorrathFormatUnsigned64);
NorrathFormatUnsigned64 norrath_player_signed32_to_double(int);
NorrathFormatUnsigned64 norrath_player_signed64_to_double(NorrathFormatSigned64);
int norrath_player_double_to_signed32(NorrathFormatUnsigned64);
NorrathFormatSigned64 norrath_player_double_to_signed64(NorrathFormatUnsigned64);
// Complete genuine dependencies: allocation/cache/rounding effects remain real.
NorrathFormatUnsigned64 norrath_player_double_ulp(NorrathFormatUnsigned64);
unsigned char* norrath_player_bigint_from_text(unsigned char*,const char*,int,int,unsigned);
unsigned char* norrath_player_bigint_allocate(unsigned char*,int);
void norrath_player_bigint_release(unsigned char*,unsigned char*);
unsigned char* norrath_player_bigint_from_double(unsigned char*,NorrathFormatUnsigned64,unsigned char*,unsigned char*);
unsigned char* norrath_player_bigint_from_integer(unsigned char*,int);
unsigned char* norrath_player_bigint_power5(unsigned char*,unsigned char*,int);
unsigned char* norrath_player_bigint_multiply(unsigned char*,unsigned char*,unsigned char*);
unsigned char* norrath_player_bigint_shift(unsigned char*,unsigned char*,int);
int norrath_player_bigint_compare(unsigned char*,unsigned char*);
unsigned char* norrath_player_bigint_difference(unsigned char*,unsigned char*,unsigned char*);
NorrathFormatUnsigned64 norrath_player_bigint_ratio(unsigned char*,unsigned char*);
void* norrath_copy_bytes(void*,const void*,__SIZE_TYPE__);
extern const unsigned char norrath_player_number_tens_view[];
extern const unsigned char norrath_player_number_big_tens_view[];
extern const unsigned char norrath_player_number_tiny_tens_view[];
extern const unsigned char norrath_player_number_overflow_view[];
extern const unsigned char norrath_player_number_half_lower_view[];
extern const unsigned char norrath_player_number_half_upper_view[];
extern const unsigned char norrath_player_number_quarter_lower_view[];
}
namespace norrath_player_number_detail {
using namespace norrath_core_format_detail;
inline unsigned char* object(Word a){return bytes(a);}
inline const char* string(Word a){return reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(a));}
inline int character(Word a){return signed_byte(bytes(a)[0]);}
inline bool decimal(int c){return unsigned(c-48)<10u;}
inline Word high(Wide x){return Word(x>>32);}
inline Wide replace_high(Wide x,Word h){return Wide(Word(x))|(Wide(h)<<32);}
inline NorrathFormatSigned64 signed_wide(Wide x){return x<=0x7fffffffffffffffull?NorrathFormatSigned64(x):-1-NorrathFormatSigned64(~x);}
inline Wide add_double(Wide x,Wide y){return norrath_player_double_add(x,y);}
inline Wide subtract_double(Wide x,Wide y){return norrath_factory_double_subtract(x,y);}
inline Wide multiply_double(Wide x,Wide y){return norrath_player_double_multiply(x,y);}
inline int compare_double(Wide x,Wide y){return norrath_factory_double_compare(x,y);}
inline Wide table(const unsigned char* p,Word index){return read_wide(address(p)+index*8u);}
}
