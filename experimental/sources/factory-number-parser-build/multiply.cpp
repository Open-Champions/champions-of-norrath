#include "number_parser.hpp"
extern "C" NorrathUnsigned64 norrath_factory_mul64(NorrathUnsigned64 left, NorrathUnsigned64 right) {
    // Complete low64 result, independent of signed interpretation of either
    // bit pattern. Unsigned shifts/additions preserve modulo2^64 arithmetic.
    NorrathUnsigned64 product = 0;
    while (right != 0ULL) {
        if ((right & 1ULL) != 0ULL) product += left;
        left <<= 1;
        right >>= 1;
    }
    return product;
}
