#pragma once
#include "number_parser.hpp"
extern "C" {
unsigned char* norrath_player_bigint_multiply_add(unsigned char*,unsigned char*,int,int);
int norrath_player_bigint_leading_zeros(unsigned);
int norrath_player_bigint_quotient(unsigned char*,unsigned char*);
extern const unsigned char norrath_dtoa_infinity_text_view[];
extern const unsigned char norrath_dtoa_nan_text_view[];
extern const unsigned char norrath_dtoa_zero_text_view[];
extern const unsigned char norrath_dtoa_log_estimate_coefficients_view[];
}
namespace norrath_dtoa_detail {
using namespace norrath_player_number_detail;
inline Word arithmetic_right(Word x,unsigned n){return(x>>n)|(x&0x80000000u?(~0u<<(32u-n)):0u);}
inline Wide power_ten(Word index){return table(norrath_player_number_tens_view,index);}
inline Wide large_power_ten(Word index){return table(norrath_player_number_big_tens_view,index);}
inline Wide integer(Word x){return norrath_player_signed32_to_double(signed_word(x));}
inline Word integer_part(Wide x){return Word(norrath_player_double_to_signed32(x));}
inline Word allocate(Word context,Word size_class){return address(norrath_player_bigint_allocate(object(context),signed_word(size_class)));}
inline void release(Word context,Word block){norrath_player_bigint_release(object(context),object(block));}
inline Word shift_block(Word context,Word block,Word shift){return address(norrath_player_bigint_shift(object(context),object(block),signed_word(shift)));}
inline Word multiply_add(Word context,Word block,Word factor,Word addition){return address(norrath_player_bigint_multiply_add(object(context),object(block),signed_word(factor),signed_word(addition)));}
inline Word power_five(Word context,Word block,Word power){return address(norrath_player_bigint_power5(object(context),object(block),signed_word(power)));}
inline int compare_blocks(Word left,Word right){return norrath_player_bigint_compare(object(left),object(right));}
inline unsigned char* readonly_text(const unsigned char* text){return object(address(text));}
}
