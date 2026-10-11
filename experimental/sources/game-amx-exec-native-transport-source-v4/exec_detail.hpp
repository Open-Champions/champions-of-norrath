#pragma once
#include "constructor.hpp"
namespace norrath_exec_detail {
using Word = unsigned;
using Register = unsigned long long;
using SignedRegister = long long;
using Address = norrath_constructor_detail::Address;
inline Word low(Register x) { return static_cast<Word>(x); }
inline Register extend(Word x) {
    return static_cast<Register>(static_cast<SignedRegister>(norrath_constructor_detail::signed_word(x)));
}
inline SignedRegister signed_register(Register x) {
    return x <= 0x7fffffffffffffffull ? static_cast<SignedRegister>(x)
         : -1ll - static_cast<SignedRegister>(~x);
}
inline Register add_word(Register x, Register y) { return extend(low(x) + low(y)); }
inline Register sub_word(Register x, Register y) { return extend(low(x) - low(y)); }
inline Register shift_left(Register x, Register count) { return extend(low(x) << (low(count) & 31u)); }
inline Register shift_right(Register x, Register count) { return extend(low(x) >> (low(count) & 31u)); }
inline Register shift_signed(Register x, Register count) {
    const unsigned bits = low(count) & 31u;
    const Word word = low(x);
    const Word shifted = word >> bits;
    return extend(shifted | ((bits != 0u && (word & 0x80000000u)) ? (~0u << (32u-bits)) : 0u));
}
inline Register load_word(Address p) { return extend(norrath_constructor_detail::read_word(p)); }
inline Register load_half(Address p) {
    using namespace norrath_constructor_detail;
    return unsigned(*bytes(p)) | (unsigned(*bytes(p+1u)) << 8);
}
inline Register load_signed_half(Address p) {
    const unsigned x = static_cast<unsigned>(load_half(p));
    return extend((x & 0x8000u) ? x | 0xffff0000u : x);
}
inline Register load_byte(Address p) { return *norrath_constructor_detail::bytes(p); }
inline void store_word(Address p, Register x) { norrath_constructor_detail::write_word(p, low(x)); }
inline void store_half(Address p, Register x) {
    *norrath_constructor_detail::bytes(p) = static_cast<unsigned char>(x);
    *norrath_constructor_detail::bytes(p+1u) = static_cast<unsigned char>(x >> 8);
}
inline void store_byte(Address p, Register x) { *norrath_constructor_detail::bytes(p) = static_cast<unsigned char>(x); }
// Native arithmetic and both HI/LO banks are defined in special.hpp.
}
