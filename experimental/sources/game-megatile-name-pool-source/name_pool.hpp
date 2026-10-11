#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4,
              "Norrath word and byte model");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(long long) == 8 && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "Norrath little-endian address model");
extern "C" {
extern unsigned char norrath_megatile_counters_view[];
extern unsigned char norrath_megatile_pm_characters_view[];
unsigned norrath_factory_strlen(const char* text);
char* norrath_factory_strcpy(char* destination, const char* source);
char* norrath_megatile_add_pm_list(char* text);
}
namespace norrath_name_pool_detail {
using Address = unsigned;
inline Address address(const void* pointer) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline char* text(Address value) {
    return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(value));
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
inline long long signed_word(unsigned word) {
    return word < 0x80000000u ? static_cast<long long>(word)
                            : static_cast<long long>(word) - 0x100000000LL;
}
}
