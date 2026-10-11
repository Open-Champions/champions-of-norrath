#include "dispatch.hpp"

namespace {
inline const char* literal(const unsigned char* bytes) {
    return reinterpret_cast<const char*>(bytes);
}
inline unsigned read_word(const unsigned char* cell) {
    return unsigned(cell[0]) | (unsigned(cell[1]) << 8) |
           (unsigned(cell[2]) << 16) | (unsigned(cell[3]) << 24);
}
inline void write_word(unsigned char* cell, unsigned value) {
    cell[0] = static_cast<unsigned char>(value);
    cell[1] = static_cast<unsigned char>(value >> 8);
    cell[2] = static_cast<unsigned char>(value >> 16);
    cell[3] = static_cast<unsigned char>(value >> 24);
#if defined(NORRATH_MEGATILE_HOST)
    norrath_megatile_host_note_word(cell, value);
#endif
}
inline unsigned read_half(const unsigned char* cell) {
    return unsigned(cell[0]) | (unsigned(cell[1]) << 8);
}
inline void write_half(unsigned char* cell, unsigned value) {
    cell[0] = static_cast<unsigned char>(value);
    cell[1] = static_cast<unsigned char>(value >> 8);
}
inline int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
inline void store_address(unsigned char* cell, const void* value) {
    const unsigned bits = static_cast<unsigned>(
        static_cast<unsigned long long>(reinterpret_cast<__UINTPTR_TYPE__>(value)));
    write_word(cell, bits);
#if defined(NORRATH_MEGATILE_HOST)
    norrath_megatile_host_note_store(cell, value);
#endif
}
inline void* load_address(const unsigned char* cell) {
    const unsigned bits = read_word(cell);
#if defined(NORRATH_MEGATILE_HOST)
    return norrath_megatile_host_pointer(bits);
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline void write_float(unsigned char* cell, float value) {
    unsigned word;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    word = unsigned(bytes[0]) | (unsigned(bytes[1]) << 8) |
           (unsigned(bytes[2]) << 16) | (unsigned(bytes[3]) << 24);
    write_word(cell, word);
}
inline float read_float(const unsigned char* cell) {
    const unsigned word = read_word(cell);
    float value;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    bytes[0] = static_cast<unsigned char>(word);
    bytes[1] = static_cast<unsigned char>(word >> 8);
    bytes[2] = static_cast<unsigned char>(word >> 16);
    bytes[3] = static_cast<unsigned char>(word >> 24);
    return value;
}
inline void copy_bytes(unsigned char* destination, const unsigned char* source, unsigned count) {
    for (unsigned index = 0u; index < count; index += 1u) destination[index] = source[index];
}
inline float scaled(int value, int factor) {
    return static_cast<float>(value * factor);
}
inline int folded(int character) {
    const unsigned flags = norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)];
    return (flags & 1u) != 0u ? character + 32 : character;
}
inline short combined_type(unsigned type_half, int row_kind) {
    const unsigned shifted = static_cast<unsigned>(row_kind) << 14;
    return static_cast<short>(type_half + shifted);
}
inline bool kitten_prefix(const unsigned char* name) {
    const unsigned char* pattern = norrath_megatile_kitten_text;
    if (*pattern == 0u) return true;
    for (;;) {
        const int name_character = signed_byte(*name);
        name += 1u;
        const int pattern_character = signed_byte(*pattern);
        if (folded(name_character) != folded(pattern_character)) return false;
        pattern += 1u;
        if (*pattern == 0u) return true;
    }
}
inline void publish_pointer(const void* value) {
    unsigned char* const count_word = norrath_megatile_counters_view + 4u;
    const unsigned count = read_word(count_word);
    write_word(count_word, count + 1u);
    store_address(norrath_megatile_pm_pointers_view + 4u * count, value);
}
inline int call_filter(unsigned code_word, char* name) {
#if defined(NORRATH_MEGATILE_HOST)
    return norrath_megatile_host_filter(name, code_word);
#else
    using Filter = int (*)(char*);
    const Filter function = reinterpret_cast<Filter>(static_cast<__UINTPTR_TYPE__>(code_word));
    return function(name);
#endif
}
}

extern "C" void norrath_megatile_do_entity_dispatch(unsigned char* record, int index,
                                                    unsigned char* delayed, int* count, bool filter) {
    unsigned char* const row = norrath_megatile_regions_view + static_cast<unsigned>(index) * 10u;
    const int row_kind = signed_byte(row[2]);
    // Bytes 0/1/3/4/7/8 are signed and scaled by 720. Byte 9 is scaled by 60.
    float x = read_float(record + 8u) - scaled(signed_byte(row[0]), 720);
    float y = read_float(record + 12u) - scaled(signed_byte(row[1]), 720);
    float z = read_float(record + 16u);
    if (row_kind == 1) {
        const float adjusted_x = x;
        const float adjusted_y = y;
        y = adjusted_x;
        x = scaled(signed_byte(row[4]), 720) - adjusted_y;
    } else if (row_kind == 2) {
        const float adjusted_x = x;
        const float adjusted_y = y;
        y = scaled(signed_byte(row[3]), 720) - adjusted_y;
        x = scaled(signed_byte(row[4]), 720) - adjusted_x;
    } else if (row_kind == 3) {
        const float adjusted_x = x;
        const float adjusted_y = y;
        x = adjusted_y;
        y = scaled(signed_byte(row[3]), 720) - adjusted_x;
    }
    x += scaled(signed_byte(row[7]), 720);
    y += scaled(signed_byte(row[8]), 720);
    z += scaled(signed_byte(row[9]), 60);
    unsigned char point[12];
    write_float(point, x);
    write_float(point + 4u, y);
    write_float(point + 8u, z);

    char* const name = static_cast<char*>(load_address(record));
    char** const arguments = static_cast<char**>(load_address(record + 20u));
    if (norrath_factory_strncmp(name, literal(norrath_megatile_mspawn_text), 6u) == 0) {
        const short type_word = combined_type(read_half(record + 6u), row_kind);
        norrath_monster_spawn(arguments, point, type_word, filter);
        return;
    }

    bool special = false;
    if (*arguments != nullptr) {
        char** cursor = arguments;
        do {
            if (norrath_factory_strstr(*cursor, literal(norrath_megatile_bang_text)) != nullptr)
                special = true;
            cursor += 1;
        } while (*cursor != nullptr);
    }
    if (norrath_factory_strcasecmp(name, literal(norrath_megatile_trigger_text)) == 0)
        special = true;
    if (norrath_factory_strcasecmp(name, literal(norrath_megatile_push_trigger_text)) == 0)
        write_half(record + 6u, 0u);

    char** parsed = arguments;
    if (special)
        parsed = norrath_megatile_specialparse(arguments, row_kind, 0, point, name);

    if (kitten_prefix(reinterpret_cast<const unsigned char*>(name))) {
        unsigned char* const kitten_word = norrath_megatile_counters_view + 16u;
        const unsigned kitten_count = read_word(kitten_word);
        write_word(kitten_word, kitten_count + 1u);
        char buffer[96];
        norrath_factory_sprintf(buffer, literal(norrath_megatile_kitten_name_format),
                                static_cast<int>(kitten_count));
        publish_pointer(norrath_megatile_add_pm_list(buffer));
        if (*parsed != nullptr) {
            char** cursor = parsed;
            do {
                char* const argument = *cursor;
                cursor += 1;
                publish_pointer(argument);
            } while (*cursor != nullptr);
        }
        parsed = norrath_megatile_cap_pm_pointers();
    }

    if (parsed == nullptr) {
        const unsigned placed = static_cast<unsigned>(*count);
        unsigned char* const point_destination = delayed + 28u * placed;
        copy_bytes(point_destination, point, 12u);
        const unsigned fresh = static_cast<unsigned>(*count);
        unsigned char* const body = delayed + 28u * fresh;
        store_address(body + 12u, name);
        store_address(body + 20u, arguments);
        write_half(body + 16u, static_cast<unsigned>(combined_type(read_half(record + 6u), row_kind)));
        *count = static_cast<int>(fresh + 1u);
        write_half(body + 16u, static_cast<unsigned>(row_kind));
        return;
    }

    const unsigned filter_code = read_word(norrath_restore_filter_word);
    if (filter) {
        if (filter_code == 0u) return;
        if (call_filter(filter_code, name) == 0) return;
    }
    const short type_word = combined_type(read_half(record + 6u), row_kind);
    norrath_object_create_by_name(name, point, type_word, parsed);
}
