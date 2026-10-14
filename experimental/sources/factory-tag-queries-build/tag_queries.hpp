#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4,
              "target32 scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 byte addresses");
extern "C" {
// Original classification center is byte128 of this mapped384-byte minimum view.
// This declaration does not claim a384-byte allocation or emit its payload.
extern const unsigned char norrath_factory_ctype_minimum_view[];
// Genuine original tag-byte views, mutated by hasLightOrParticle itself.
extern unsigned char norrath_factory_particle_emitter_tag_bytes[];
extern unsigned char norrath_factory_dynamic_light_tag_bytes[];
extern unsigned char norrath_factory_halo_tag_bytes[];
// These must resolve to complete genuine reconstructed callees, never host responses.
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_atoi(const char* text);
int norrath_has_light_or_particle_number(char** arguments, int number);
int norrath_object_find_tag_int(char** arguments, char* tag, int occurrence);
char* norrath_object_find_tag_string(char** arguments, char* tag, int occurrence);
int norrath_has_light_or_particle(char** arguments);
}
namespace norrath_tag_detail {
using Word = unsigned;
inline Word address(const void* p) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(Word a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline char* text(Word a) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(a)); }
inline Word read_word(Word a) {
    return Word(bytes(a)[0]) | (Word(bytes(a)[1]) << 8) |
           (Word(bytes(a)[2]) << 16) | (Word(bytes(a)[3]) << 24);
}
inline int signed_byte(unsigned char b) { return b < 128u ? int(b) : int(b) - 256; }
inline void lower_in_place(char* tag) {
    Word cursor = address(tag);
    unsigned char raw = *bytes(cursor);
    while (raw != 0u) {
        const int character = signed_byte(raw);
        const unsigned flags = norrath_factory_ctype_minimum_view[unsigned(character + 128)];
        const int folded = (flags & 1u) != 0u ? character + 32 : character;
        *bytes(cursor) = static_cast<unsigned char>(folded);
        cursor += 1u;
        raw = *bytes(cursor);
    }
}
// One first matching numeric tag, preserving fresh pointer-cell loads after comparison.
// Fixed count is from the actual hasLightOrParticle immediate; no strlen is inserted.
inline int first_fixed_integer(char** arguments, unsigned char* tag, Word length) {
    if (arguments == nullptr) return 0;
    lower_in_place(reinterpret_cast<char*>(tag));
    Word cell = address(arguments);
    if (read_word(cell) == 0u) return 0;
    for (;;) {
        const Word current = read_word(cell);
        if (norrath_factory_strncmp(text(current), reinterpret_cast<char*>(tag), length) == 0) {
            const Word fresh = read_word(cell);
            const Word separator = fresh + length;
            if (signed_byte(*bytes(separator)) == 61) return norrath_factory_atoi(text(separator + 1u));
        }
        cell += 4u;
        if (read_word(cell) == 0u) return 0;
    }
}
}
