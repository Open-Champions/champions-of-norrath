#include "division.hpp"
extern "C" NorrathUnsigned64 norrath_factory_umod64(NorrathUnsigned64 numerator, NorrathUnsigned64 denominator) {
    return norrath_parser_arithmetic::divide_nonzero(numerator, denominator).remainder;
}
