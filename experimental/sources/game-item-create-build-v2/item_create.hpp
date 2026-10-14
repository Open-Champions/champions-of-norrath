#pragma once
#include "constructor.hpp"
extern "C" {
// Exact existing player/factory call: no implicit item instance or hidden return.
void norrath_player_create_item(char* name, void* description, int level, int named_table);
char* norrath_factory_strcpy(char*, const char*);
char* norrath_factory_strchr(const char*, int);
int norrath_string_compare(const char*, const char*);
int norrath_player_sscanf(const char*, const char*, ...);
void* norrath_block_memory_set(void*, int, unsigned);
extern unsigned char norrath_item_table_view[];
extern unsigned char norrath_named_item_table_view[];
// Original format operands remain genuine data imports, with no copied strings.
extern unsigned char norrath_item_format_454428[];
extern unsigned char norrath_item_format_454440[];
extern unsigned char norrath_item_format_454458[];
extern unsigned char norrath_item_format_454468[];
extern unsigned char norrath_item_format_454478[];
extern unsigned char norrath_item_format_454488[];
extern unsigned char norrath_item_format_454498[];
extern unsigned char norrath_item_format_4544a8[];
extern unsigned char norrath_item_format_4544b8[];
extern unsigned char norrath_item_format_4544c8[];
extern unsigned char norrath_item_format_4544e0[];
extern unsigned char norrath_item_format_4544f8[];
extern unsigned char norrath_item_format_454508[];
extern unsigned char norrath_item_format_454518[];
extern unsigned char norrath_item_format_454530[];
extern unsigned char norrath_item_format_454548[];
extern unsigned char norrath_item_format_454560[];
extern unsigned char norrath_item_format_454578[];
extern unsigned char norrath_item_format_454588[];
extern unsigned char norrath_item_format_454598[];
extern unsigned char norrath_item_format_4545b0[];
extern unsigned char norrath_item_format_4545c8[];
extern unsigned char norrath_item_format_4545d8[];
extern unsigned char norrath_item_format_4545e8[];
extern unsigned char norrath_item_format_4545f8[];
extern unsigned char norrath_item_format_454610[];
extern unsigned char norrath_item_format_454628[];
extern unsigned char norrath_item_format_454638[];
extern unsigned char norrath_item_format_454648[];
extern unsigned char norrath_item_format_454658[];
extern unsigned char norrath_item_format_454668[];
extern unsigned char norrath_item_format_454678[];
extern unsigned char norrath_item_format_454688[];
extern unsigned char norrath_item_format_4546a0[];
extern unsigned char norrath_item_format_4546b8[];
extern unsigned char norrath_item_format_4546d0[];
extern unsigned char norrath_item_format_4546e8[];
extern unsigned char norrath_item_format_454700[];
extern unsigned char norrath_item_format_454710[];
extern unsigned char norrath_item_format_454720[];
extern unsigned char norrath_item_format_483260[];
extern unsigned char norrath_item_format_454738[];
}
namespace norrath_item_detail {
using namespace norrath_constructor_detail;
inline unsigned half(Address a) { return unsigned(bytes(a)[0]) | unsigned(bytes(a)[1]) << 8; }
inline void put_half(Address a, unsigned v) {
    bytes(a)[0] = static_cast<unsigned char>(v);
    bytes(a)[1] = static_cast<unsigned char>(v >> 8);
}
inline unsigned long long wide(Address a) {
    return read_word(a) | static_cast<unsigned long long>(read_word(a + 4u)) << 32;
}
inline void put_wide(Address a, unsigned long long v) {
    write_word(a, unsigned(v)); write_word(a + 4u, unsigned(v >> 32));
}
inline unsigned long long signed_low_word(unsigned v) {
    return v | ((v & 0x80000000u) ? 0xffffffff00000000ull : 0ull);
}
// The original snapshots each32-byte group before publishing it. Byte copies
// keep the untouched template bytes untouched; no all-zero descriptor is made.
inline void copy_template(Address destination, Address source) {
    for (unsigned offset = 0u; offset != 64u; offset += 32u) {
        unsigned char snapshot[32];
        for (unsigned i = 0u; i != 32u; ++i) snapshot[i] = bytes(source + offset)[i];
        for (unsigned i = 0u; i != 32u; ++i) bytes(destination + offset)[i] = snapshot[i];
    }
    unsigned char snapshot[8];
    for (unsigned i = 0u; i != 8u; ++i) snapshot[i] = bytes(source + 64u)[i];
    for (unsigned i = 0u; i != 8u; ++i) bytes(destination + 64u)[i] = snapshot[i];
}
// Four original eight-byte option slots, not an invented descriptor class.
inline void set_option(Address description, unsigned type, unsigned first,
                       unsigned second, unsigned third) {
    Address slot = description + 36u;
    for (unsigned i = 0u; i != 4u; ++i, slot += 8u) {
        const unsigned old_type = half(slot);
        if (old_type == 0xffffu || old_type == type) {
            put_half(slot, type);
            put_half(slot + 6u, third);
            put_half(slot + 2u, first);
            put_half(slot + 4u, second);
            return;
        }
    }
}
inline Address lookup(Address base, const char* name) {
    unsigned index = 0u;
    for (;;) {
        const Address row = base + (index << 6u);
        const Address row_name = read_word(row);
        if (row_name == 0u) { index = 0u; break; }
        if (norrath_string_compare(text(row_name), name) == 0) {
            if (read_word(row) == 0u) index = 0u;
            break;
        }
        index += 1u;
    }
    return base + (index << 6u);
}
}
