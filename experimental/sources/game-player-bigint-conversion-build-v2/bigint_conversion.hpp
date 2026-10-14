#pragma once
#include "number_parser.hpp"
extern "C" {
int norrath_player_bigint_leading_zeros(unsigned);
int norrath_player_bigint_trailing_zeros(unsigned char*);
NorrathFormatUnsigned64 norrath_player_bigint_to_double(unsigned char*,unsigned char*);
}
namespace norrath_bigint_conversion_detail {
using namespace norrath_player_number_detail;
inline Word arithmetic_right16(Word value) {
    return (value>>16)|(value&0x80000000u?0xffff0000u:0u);
}
}
