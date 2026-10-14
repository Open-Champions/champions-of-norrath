#pragma once
#include "tag_queries.hpp"
using NorrathUnsigned64 = unsigned long long;
using NorrathSigned64 = long long;
static_assert(sizeof(NorrathUnsigned64) == 8 && sizeof(NorrathSigned64) == 8, "observed original64-bit parser arithmetic and return ABI");
extern "C" {
// Actual writable _impure_ptr4 at0x448480. Its live pointee is not copied,
// defaulted or assumed to remain the ELF's initial impure_data object.
extern unsigned char norrath_factory_impure_pointer_word[];
NorrathSigned64 norrath_factory_strtol(const char* input, char** end_pointer, int base);
NorrathSigned64 norrath_factory_strtol_reentrant(unsigned char* reent, const char* input, char** end_pointer, int base);
NorrathUnsigned64 norrath_factory_udiv64(NorrathUnsigned64 numerator, NorrathUnsigned64 denominator);
NorrathUnsigned64 norrath_factory_umod64(NorrathUnsigned64 numerator, NorrathUnsigned64 denominator);
NorrathUnsigned64 norrath_factory_mul64(NorrathUnsigned64 left, NorrathUnsigned64 right);
}
namespace norrath_parser_detail {
using namespace norrath_tag_detail;
inline void write_word(Word a, Word value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline int signed_word(Word word) {
    return word <= 0x7fffffffu ? int(word) : -1 - int(0xffffffffu - word);
}
inline NorrathSigned64 signed64_bits(NorrathUnsigned64 bits) {
    return bits <= 0x7fffffffffffffffULL ? NorrathSigned64(bits)
        : -1LL - NorrathSigned64(~bits);
}
inline unsigned classify(int character) {
    return norrath_factory_ctype_minimum_view[unsigned(character + 128)];
}
}
