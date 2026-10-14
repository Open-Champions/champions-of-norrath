#include "specialparse.hpp"

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
inline void store_address(unsigned char* cell, const void* value) {
    const unsigned bits = static_cast<unsigned>(
        static_cast<unsigned long long>(reinterpret_cast<__UINTPTR_TYPE__>(value)));
    write_word(cell, bits);
#if defined(NORRATH_MEGATILE_HOST)
    norrath_megatile_host_note_store(cell, value);
#endif
}
inline int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value)
                                : -1 - static_cast<int>(0xffffffffu - value);
}
inline int neg32(int value) {
    return static_cast<int>(0u - static_cast<unsigned>(value));
}
inline int signed_byte(unsigned char value) {
    return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256;
}
inline bool digit_class(unsigned char value) {
    const unsigned index = static_cast<unsigned>(signed_byte(value) + 128);
    return (norrath_factory_ctype_minimum_view[index] & 4u) != 0u;
}
inline void copy_bytes(unsigned char* destination, const unsigned char* source, unsigned count) {
    for (unsigned index = 0u; index < count; index += 1u)
        destination[index] = source[index];
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
inline unsigned char* counters() { return norrath_megatile_counters_view; }
inline unsigned char* pointer_bank() { return norrath_megatile_pm_pointers_view; }

// pmlistptrsused is counters+4. The two store orders are both present.
inline void publish_pointer(const void* value, bool count_before_pointer) {
    unsigned char* const count_word = counters() + 4u;
    const unsigned count = read_word(count_word);
    unsigned char* const cell = pointer_bank() + 4u * count;
    if (count_before_pointer) {
        write_word(count_word, count + 1u);
        store_address(cell, value);
    } else {
        store_address(cell, value);
        write_word(count_word, count + 1u);
    }
}

// After an underscore, the cursor stops on the following digit. A missing
// underscore leaves the cursor on the terminating zero.
inline char* shift_number_cursor(char* formatted) {
    unsigned char* cursor = reinterpret_cast<unsigned char*>(formatted);
    if (*cursor == 0u) return formatted;
    if (*cursor != static_cast<unsigned char>('_')) {
        cursor += 1u;
        while (*cursor != 0u && *cursor != static_cast<unsigned char>('_'))
            cursor += 1u;
        if (*cursor == 0u) return reinterpret_cast<char*>(cursor);
    }
    while (*cursor != 0u && !digit_class(*cursor)) cursor += 1u;
    return reinterpret_cast<char*>(cursor);
}

inline void record_dynamic_level(char* copied_name, const unsigned char* point) {
    unsigned char* const count_word = counters() + 12u;
    const unsigned index = read_word(count_word);
    norrath_factory_strcpy(reinterpret_cast<char*>(norrath_megatile_dynlevels_view + 28u * index),
                           copied_name);
    const unsigned fresh = read_word(count_word);
    write_word(count_word, fresh + 1u);
    copy_bytes(norrath_megatile_dynlevels_view + 28u * fresh + 16u, point, 12u);
}
}

extern "C" char** norrath_megatile_specialparse(char** arguments, int selector, int mode,
                                                const unsigned char* point, char* name) {
    // Observed 1040-byte frame. Slots 16/20 start at -1. The point is copied
    // before any call. Buffer gaps are the original 112-byte separations.
    alignas(16) unsigned char frame[1040];
    unsigned char point_copy[12];
    copy_bytes(point_copy, point, 12u);
    int* const slot_w = reinterpret_cast<int*>(frame + 16u);
    int* const slot_h = reinterpret_cast<int*>(frame + 20u);
    *slot_w = -1;
    *slot_h = -1;
    char* const scanned = reinterpret_cast<char*>(frame + 32u);
    int* const scanned_count = reinterpret_cast<int*>(frame + 144u);
    char* const level_argument = reinterpret_cast<char*>(frame + 160u);
    char* const suffixed = reinterpret_cast<char*>(frame + 272u);
    char* const joined = reinterpret_cast<char*>(frame + 384u);
    char* const shift_level = reinterpret_cast<char*>(frame + 496u);
    char* const orientation = reinterpret_cast<char*>(frame + 608u);
    char* const closest = reinterpret_cast<char*>(frame + 720u);
    char* const measure = reinterpret_cast<char*>(frame + 832u);

    char** cursor = arguments;
    if (*cursor != nullptr) {
        do {
            char* const argument = *cursor;
            if (norrath_factory_strcasecmp(name, literal(norrath_megatile_trigger_text)) == 0) {
                const int width_key = norrath_factory_strncmp(
                    argument, literal(norrath_megatile_width_key), 2u);
                if (width_key != 0) {
                    const int height_key = norrath_factory_strncmp(
                        argument, literal(norrath_megatile_height_key), 2u);
                    if (height_key != 0) goto ordinary;
                }
                if (norrath_factory_strncmp(argument, literal(norrath_megatile_width_key), 2u) == 0)
                    *slot_w = norrath_factory_atoi(argument + 2);
                else
                    *slot_h = norrath_factory_atoi(argument + 2);
                goto next_argument;
            }
        ordinary:
            if (norrath_factory_strstr(argument, literal(norrath_megatile_bang_text)) == nullptr) {
                publish_pointer(argument, true);
                goto next_argument;
            }
            if (norrath_factory_strstr(argument, literal(norrath_megatile_dynamic_new_world_text)) != nullptr) {
                char* const paren = norrath_factory_strchr(argument, '(');
                norrath_factory_sscanf(paren + 1, literal(norrath_megatile_scan_pair_format),
                                       scanned, scanned_count);
                const unsigned previous_suffix = read_word(counters());
                norrath_factory_sprintf(suffixed, literal(norrath_megatile_suffix_number_format),
                                        scanned, static_cast<int>(previous_suffix));
                norrath_factory_sprintf(level_argument, literal(norrath_megatile_level_assignment_format),
                                        suffixed);
                const unsigned fresh_suffix = read_word(counters());
                write_word(counters(), fresh_suffix + static_cast<unsigned>(*scanned_count));
                char* const added = norrath_megatile_add_pm_list(level_argument);
                publish_pointer(added, false);
                record_dynamic_level(suffixed, point_copy);
            } else if (norrath_factory_strstr(argument, literal(norrath_megatile_dynamic_shift_text)) != nullptr) {
                // Interior bytes 81 and 161 of levelName. 161 is 80 bytes after 81.
                char* const left = reinterpret_cast<char*>(norrath_megatile_level_name_view + 81u);
                char* const right = left + 80;
                norrath_factory_sprintf(joined, literal(norrath_megatile_pair_join_format), left, right);
                char* const number = shift_number_cursor(joined);
                char* const paren = norrath_factory_strchr(argument, '(');
                const int head = norrath_factory_atoi(number);
                const int tail = norrath_factory_atoi(paren + 1);
                const int sum = static_cast<int>(static_cast<unsigned>(head) + static_cast<unsigned>(tail));
                norrath_factory_sprintf(number, literal(norrath_megatile_decimal_format), sum);
                norrath_factory_sprintf(shift_level, literal(norrath_megatile_level_assignment_format), joined);
                char* const added = norrath_megatile_add_pm_list(shift_level);
                publish_pointer(added, false);
                record_dynamic_level(joined, point_copy);
            } else if (norrath_factory_strstr(argument, literal(norrath_megatile_dynamic_orientation_text)) != nullptr) {
                norrath_factory_sprintf(orientation, literal(norrath_megatile_orientation_format), selector);
                char* const added = norrath_megatile_add_pm_list(orientation);
                publish_pointer(added, true);
            } else if (norrath_factory_strstr(argument, literal(norrath_megatile_closest_level_text)) != nullptr) {
                // mode <= 0 leaves the remaining arguments unparsed and skips the cap.
                if (mode <= 0) return nullptr;
                const unsigned char* best_name = nullptr;
                float best = read_float(norrath_megatile_closest_limit_view);
                int remaining = signed_word(read_word(counters() + 12u));
                if (remaining > 0) {
                    const float origin_x = read_float(point_copy);
                    const float origin_y = read_float(point_copy + 4u);
                    unsigned offset = 0u;
                    do {
                        const unsigned char* const record_point =
                            norrath_megatile_dynlevels_view + 16u + offset;
                        const float dx = read_float(record_point) - origin_x;
                        const float dy = read_float(record_point + 4u) - origin_y;
                        const float squared = dx * dx + dy * dy;
                        remaining -= 1;
                        if (squared < best) {
                            best = squared;
                            best_name = norrath_megatile_dynlevels_view + offset;
                        }
                        offset += 28u;
                    } while (remaining != 0);
                }
                norrath_factory_sprintf(closest, literal(norrath_megatile_from_format),
                                        reinterpret_cast<const char*>(best_name));
                char* const added = norrath_megatile_add_pm_list(closest);
                publish_pointer(added, true);
            }
        next_argument:
            cursor += 1;
        } while (*cursor != nullptr);
    }

    if (norrath_factory_strcasecmp(name, literal(norrath_megatile_trigger_text)) == 0) {
        if (selector == 1) {
            norrath_swap_int(slot_w, slot_h);
            *slot_h = neg32(*slot_h);
        } else if (selector == 2) {
            *slot_w = neg32(*slot_w);
            *slot_h = neg32(*slot_h);
        } else if (selector == 3) {
            norrath_swap_int(slot_w, slot_h);
            *slot_w = neg32(*slot_w);
        }
        norrath_factory_sprintf(measure, literal(norrath_megatile_width_format), *slot_w);
        char* added = norrath_megatile_add_pm_list(measure);
        publish_pointer(added, false);
        norrath_factory_sprintf(measure, literal(norrath_megatile_height_format), *slot_h);
        added = norrath_megatile_add_pm_list(measure);
        publish_pointer(added, true);
    }
    return norrath_megatile_cap_pm_pointers();
}
