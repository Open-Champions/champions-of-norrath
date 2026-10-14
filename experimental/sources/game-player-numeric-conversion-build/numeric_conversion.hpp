#pragma once
#include "softdouble.hpp"
extern "C" {
NorrathFormatUnsigned64 norrath_player_double_to_unsigned64(NorrathFormatUnsigned64);
// The original returns sign-extended low32 bits in V0, including all-ones.
NorrathFormatUnsigned64 norrath_player_double_to_unsigned32_bits(NorrathFormatUnsigned64);
}
namespace norrath_numeric_conversion_detail {
using namespace norrath_player_number_detail;
}
