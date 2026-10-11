#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 representation");
extern "C" {
// Genuine data address views; no payload or inferred allocation is emitted.
extern const unsigned char norrath_game_object_vtable_view[];
extern unsigned char norrath_constructor_grid_a_view[];
extern unsigned char norrath_constructor_grid_b_view[];
extern unsigned char norrath_constructor_default_head_view[];
extern unsigned char norrath_constructor_resource_frame_view[];
extern unsigned char norrath_constructor_random_state_view[];
extern unsigned char norrath_constructor_tag_a_view[];
extern unsigned char norrath_constructor_tag_b_view[];
extern const unsigned char norrath_factory_ctype_minimum_view[];
// These imports require complete genuine callees, not host responses or stubs.
void* norrath_lump_find_resource(char* lump_name, char* resource_name);
void* norrath_list_oneway_link(void* head, void* object_head, void* value);
int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
void norrath_object_add_string(char* string, void* object);
// Point is the actual12-byte caller storage passed by address. No class layout.
// All terminating original paths leave V0=0; this is not a self-pointer result.
unsigned norrath_game_object_construct(void* object, const void* point,
                                       void* resource_map, char** incoming_arguments);
}
namespace norrath_constructor_detail {
using Address = unsigned;
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline unsigned char* bytes(Address a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline void* pointer(Address a) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline char* text(Address a) {
    return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
inline void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value)
                              : -1 - static_cast<int>(0xffffffffu - value);
}
inline int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
inline float read_float(Address a) {
    const unsigned word = read_word(a);
    float value;
    unsigned char* p = reinterpret_cast<unsigned char*>(&value);
    p[0] = static_cast<unsigned char>(word);
    p[1] = static_cast<unsigned char>(word >> 8);
    p[2] = static_cast<unsigned char>(word >> 16);
    p[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
inline int grid_coordinate(Address a) {
    // Actual CVT.W.S truncates toward zero. Initial proof admits finite normal
    // values/zeros whose truncated result fits signed32; no saturation or FCSR
    // semantics are silently supplied. The bias is actual low32 ADDIU wrapping.
    const unsigned biased = static_cast<unsigned>(static_cast<int>(read_float(a))) + 15360u;
    const unsigned shifted = (biased >> 8) | ((biased & 0x80000000u) != 0u ? 0xff000000u : 0u);
    return signed_word(shifted);
}
inline bool lower_nonempty_tag(Address tag) {
    unsigned char raw = *bytes(tag);
    if (raw == 0u) return false;
    do {
        const int character = signed_byte(raw);
        const unsigned flags = norrath_factory_ctype_minimum_view[unsigned(character + 128)];
        const int folded = (flags & 1u) != 0u ? character + 32 : character;
        *bytes(tag) = static_cast<unsigned char>(folded);
        tag += 1u;
        raw = *bytes(tag);
    } while (raw != 0u);
    return true;
}
struct ScanResult { Address selected; Address arguments; };
inline ScanResult find_nth_string(Address object, Address arguments, unsigned occurrence) {
    if (arguments == 0u) return ScanResult{0u, arguments};
    const Address tag = address(norrath_constructor_tag_b_view);
    // Both actual scans retain captured arguments for an initially empty tag;
    // otherwise they reload object44 after in-place tag mutation.
    if (lower_nonempty_tag(tag)) arguments = read_word(object + 44u);
    if (read_word(arguments) == 0u) return ScanResult{0u, arguments};
    unsigned current_offset = 0u;
    unsigned next_offset = 0u;
    for (;;) {
        const Address entry = read_word(arguments + next_offset);
        next_offset += 4u;
        const int comparison = norrath_factory_strncmp(text(entry), text(tag), 4u);
        arguments = read_word(object + 44u);
        if (comparison == 0) {
            const Address fresh_entry = read_word(arguments + current_offset);
            if (signed_byte(*bytes(fresh_entry + 4u)) == 61) {
                occurrence -= 1u;
                if (occurrence == 0xffffffffu)
                    return ScanResult{fresh_entry + 5u, arguments};
            }
        }
        if (read_word(arguments + next_offset) == 0u)
            return ScanResult{0u, arguments};
        current_offset = next_offset;
    }
}
}
