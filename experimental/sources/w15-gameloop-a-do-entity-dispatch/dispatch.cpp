// Observed doEntityDispatch__FP8MTObjDefiP13delayedEntityPib at 0x002695e0, 1436 bytes. Arguments: a record (word at +0 is
// the name string pointer, half at +6 a type value, floats at +8, +12 and +16 a position, and from +20 an inline list of
// pointer words ended by a zero word), a region row index into mt_tiledata (10-byte rows of signed bytes), the base of an
// array of 28-byte delayed records, a pointer to its count, and a filter flag.
//  1. The position is moved by the row: x and y lose 720 times row bytes 0 and 1; row byte 2 (the kind) 1, 2 or 3 swaps or
//     mirrors them with row bytes 3 and 4 (720 each); then x and y gain 720 times bytes 7 and 8 and z gains 60 times byte 9.
//  2. A name starting with "MSpawn" (strncmp 6) goes to monsterSpawn(list, point, type + (kind << 14), filter) and returns.
//  3. "special" is set when any list entry contains "!!!" or the name equals "trigger" (case-insensitive); a name equal to
//     "PushTrigger" (case-insensitive) clears the half at +6. When special, specialparse(list, kind, 0, point, name) gives
//     the list to use.
//  4. A name with the case-insensitive prefix "kitten" increments the word kittencount, formats "name=kitten%d" with the old
//     value, publishes the pooled copy and every list entry into pmlistptrs (count in pmlistptrsused) and takes the capped
//     list from addpmlistptrscap.
//  5. A null list writes a 28-byte delayed record (the point, the name word at +12, the type half at +16, the original list
//     address at +20) at the current count and increments it. Otherwise, with the filter flag, nothing is created when the
//     function word userFilterFunction is zero or calls it with the name and gets zero; objectCreateByName(name, point,
//     type + (kind << 14), list) creates the object.
// Inferred: this places one level-file entity record into the world, with special handling for monster spawns, triggers and
// the "kitten" debug objects. This unit names the image's own symbols; megatile-do-entity-dispatch defines the same body
// under norrath_ alias names. The paired run against the image showed that unit read the list as a pointer at +20; the
// image uses +20 itself as the inline list, which is corrected here.
// Unknown: the layout of MTObjDef beyond the fields read, the meaning of the region row bytes beyond their use, what the
// type half encodes, and the remaining bytes of the delayed record (byte 24 and up are not written).
#include "dispatch.hpp"

namespace {
const unsigned char kMSpawn[] = "MSpawn";
const unsigned char kBang[] = "!!!";
const unsigned char kTrigger[] = "trigger";
const unsigned char kPushTrigger[] = "PushTrigger";
const unsigned char kKittenPrefix[] = "kitten";
const unsigned char kKittenFormat[] = "name=kitten%d";
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
    const unsigned flags = _ctype_[1 + character];
    return (flags & 1u) != 0u ? character + 32 : character;
}
inline short combined_type(unsigned type_half, int row_kind) {
    const unsigned shifted = static_cast<unsigned>(row_kind) << 14;
    return static_cast<short>(type_half + shifted);
}
inline bool kitten_prefix(const unsigned char* name) {
    const unsigned char* pattern = kKittenPrefix;
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
// A char** list is an array of 32-bit pointer words ended by a zero word; entries are read as words so that the host check
// can run with 64-bit pointers.
inline unsigned list_word(char** list, unsigned index) {
    return read_word(reinterpret_cast<unsigned char*>(list) + 4u * index);
}
inline char* list_entry(char** list, unsigned index) {
    return static_cast<char*>(load_address(reinterpret_cast<unsigned char*>(list) + 4u * index));
}
inline void publish_pointer(const void* value) {
    unsigned char* const count_word = pmlistptrsused;
    const unsigned count = read_word(count_word);
    write_word(count_word, count + 1u);
    store_address(pmlistptrs + 4u * count, value);
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

extern "C" void doEntityDispatch__FP8MTObjDefiP13delayedEntityPib(unsigned char* record, int index,
                                                    unsigned char* delayed, int* count, bool filter) {
    unsigned char* const row = mt_tiledata + static_cast<unsigned>(index) * 10u;
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
    // The argument list is an array of pointer words stored inline in the record at +20.
    char** const arguments = reinterpret_cast<char**>(record + 20u);
    if (strncmp_(name, literal(kMSpawn), 6u) == 0) {
        const short type_word = combined_type(read_half(record + 6u), row_kind);
        monsterSpawn__FPPcG6Point3sb(arguments, point, type_word, filter);
        return;
    }

    bool special = false;
    if (list_word(arguments, 0u) != 0u) {
        unsigned cursor = 0u;
        do {
            if (strstr_(list_entry(arguments, cursor), literal(kBang)) != nullptr)
                special = true;
            cursor += 1u;
        } while (list_word(arguments, cursor) != 0u);
    }
    if (strcasecmp_(name, literal(kTrigger)) == 0)
        special = true;
    if (strcasecmp_(name, literal(kPushTrigger)) == 0)
        write_half(record + 6u, 0u);

    char** parsed = arguments;
    if (special)
        parsed = specialparse__FPPciiG6Point3Pc(arguments, row_kind, 0, point, name);

    if (kitten_prefix(reinterpret_cast<const unsigned char*>(name))) {
        unsigned char* const kitten_word = kittencount;
        const unsigned kitten_count = read_word(kitten_word);
        write_word(kitten_word, kitten_count + 1u);
        char buffer[96];
        sprintf_(buffer, literal(kKittenFormat),
                                static_cast<int>(kitten_count));
        publish_pointer(addpmlist__FPc(buffer));
        if (list_word(parsed, 0u) != 0u) {
            unsigned cursor = 0u;
            do {
                char* const argument = list_entry(parsed, cursor);
                cursor += 1u;
                publish_pointer(argument);
            } while (list_word(parsed, cursor) != 0u);
        }
        parsed = addpmlistptrscap__Fv();
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

    const unsigned filter_code = read_word(userFilterFunction);
    if (filter) {
        if (filter_code == 0u) return;
        if (call_filter(filter_code, name) == 0) return;
    }
    const short type_word = combined_type(read_half(record + 6u), row_kind);
    objectCreateByName__FPcG6Point3sPPc(name, point, type_word, parsed);
}
