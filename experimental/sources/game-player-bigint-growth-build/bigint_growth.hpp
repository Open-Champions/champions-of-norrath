#pragma once
#include "number_parser.hpp"
extern "C" {
unsigned char* norrath_player_bigint_multiply_add(unsigned char*,unsigned char*,int,int);
extern const unsigned char norrath_player_bigint_small_powers5_view[]; // Actual47f4e0/12, three native words only.
}
namespace norrath_bigint_growth_detail {
using namespace norrath_player_number_detail;
inline Word arithmetic_right(Word value,unsigned count){
    return (value>>count)|((value&0x80000000u)?(~0u<<(32u-count)):0u);
}
}
