#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "EE word/address representation");
namespace norrath_safe_iterator_detail {
using Address = unsigned;
inline Address address(const void* pointer) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline unsigned char* bytes(Address location) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(location));
}
// Ordinary-RAM value access. Physical LW/SW widths and fault traces are not claimed.
inline unsigned read_word(Address location) {
    const unsigned char* p = bytes(location);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(Address location, unsigned value) {
    unsigned char* p = bytes(location);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline void insert_after(Address iterator, Address selected) {
    const Address successor = read_word(selected);
    write_word(iterator + 4u, selected);
    write_word(iterator, successor);
    if (successor != 0u) write_word(successor + 4u, iterator);
    // The successor store can alias iterator storage: reload the previous link.
    const Address previous = read_word(iterator + 4u);
    write_word(previous, iterator);
}
}
