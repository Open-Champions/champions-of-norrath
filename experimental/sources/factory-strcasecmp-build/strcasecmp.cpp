#include "strcasecmp.hpp"

namespace {
inline int signed_byte(unsigned char byte) {
    return byte < 128u ? int(byte) : int(byte) - 256;
}
inline int fold_signed(int character) {
    const unsigned index = unsigned(character + 128);
    return (norrath_factory_ctype_minimum_view[index] & 1u)
        ? character + 32 : character;
}
inline int fold_unsigned(unsigned character) {
    return (norrath_factory_ctype_minimum_view[character + 128u] & 1u)
        ? int(character) + 32 : int(character);
}
}

extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const auto* b = reinterpret_cast<const unsigned char*>(right);
    int a_signed = signed_byte(*a);
    while (a_signed != 0) {
        const int b_signed = signed_byte(*b);
        const int a_folded = fold_signed(a_signed);
        const int b_folded = fold_signed(b_signed);
        if (a_folded != b_folded) break;
        ++a;
        ++b;
        a_signed = signed_byte(*a);
    }
    // The original final path rereads both bytes UNSIGNED and classifies
    // them again; its signed loop's folded values are not the return value.
    const unsigned a_unsigned = *a;
    const unsigned b_unsigned = *b;
    const int a_folded = fold_unsigned(a_unsigned);
    const int b_folded = fold_unsigned(b_unsigned);
    return a_folded - b_folded;
}
