#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(long long) == 8 && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "EE byte, word and pointer model");
extern "C" {
// Exact existing restoration declarations; views own no storage.
extern unsigned char norrath_megatile_counters_view[];
extern unsigned char norrath_megatile_pm_pointers_view[];
char** norrath_megatile_cap_pm_pointers();
}
namespace norrath_pointer_cap_detail {
using Address = unsigned;
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline volatile unsigned char* bytes(Address value) {
    return reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
}
inline unsigned read_word(Address value) {
    const unsigned a = bytes(value)[0];
    const unsigned b = bytes(value)[1];
    const unsigned c = bytes(value)[2];
    const unsigned d = bytes(value)[3];
    return a | (b << 8) | (c << 16) | (d << 24);
}
inline void write_word(Address value, unsigned word) {
    bytes(value)[0] = static_cast<unsigned char>(word);
    bytes(value)[1] = static_cast<unsigned char>(word >> 8);
    bytes(value)[2] = static_cast<unsigned char>(word >> 16);
    bytes(value)[3] = static_cast<unsigned char>(word >> 24);
}
inline long long signed_word(unsigned value) {
    return value < 0x80000000u ? static_cast<long long>(value)
                             : static_cast<long long>(value) - 0x100000000LL;
}
inline char** pointers(Address value) {
    return reinterpret_cast<char**>(static_cast<__UINTPTR_TYPE__>(value));
}
}
