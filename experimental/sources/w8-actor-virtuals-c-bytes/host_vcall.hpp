#pragma once
// Host-check helper (not a plan input): gives a fake creature a virtual table at
// object+72 whose slot at a byte offset holds an adjustment and a function.
#include "bytes.hpp"
namespace w8c {
inline void host_install_slot(unsigned char* object, unsigned char* table, unsigned slot_byte_offset,
                              int adjustment, void* function) {
    write_u16(address(table) + slot_byte_offset, static_cast<unsigned>(adjustment));
    write_word(address(table) + slot_byte_offset + 4u, address_bits(function));
    host_note(function);
    host_note(table);
    write_word(address(object) + 72u, address_bits(table));
}
}
