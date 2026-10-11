#include "dependencies.hpp"
extern "C" void norrath_object_add_string(char* string, void* object_storage) {
    using namespace norrath_constructor_dependency_detail;
    const Address original = address(string);
    if (signed_byte(*bytes(original)) == 34) {
        // Actual frame has512 bytes before saved registers. No bounded-copy
        // check or zero-fill is introduced; overflow/padding is a proof boundary.
        alignas(16) char temporary[512];
        norrath_factory_strcpy(temporary, text(original + 1u));
        if (norrath_factory_strchr(temporary, 34) != nullptr) {
            char* closing = norrath_factory_strchr(temporary, 34);
            *closing = 0;
        }
        norrath_factory_strcpy(string, temporary);
    }
    const Address counter = address(norrath_constructor_string_count_view);
    write_word(counter, read_word(counter) + 1u);
    const unsigned length = norrath_factory_strlen(string);
    unsigned hash = 0u;
    if (signed_word(length) > 0) {
        unsigned offset = 0u;
        for (;;) {
            const int character = signed_byte(*bytes(original + offset));
            offset += 1u;
            const unsigned flags = norrath_factory_ctype_minimum_view[unsigned(character + 128)];
            const int folded = (flags & 1u) != 0u ? character + 32 : character;
            // Hash the signed/folded value without mutating source characters.
            hash += static_cast<unsigned>(folded);
            hash += hash << 10;
            hash ^= arithmetic_shift(hash, 6u);
            if (!(signed_word(offset) < signed_word(length))) break;
        }
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    unsigned slot = hash & 511u;
    const Address names = address(norrath_constructor_string_names_view);
    const Address objects = address(norrath_constructor_string_objects_view);
    for (;;) {
        const Address name_cell = names + 4u * slot;
        const Address object_cell = objects + 4u * slot;
        const Address resident = read_word(name_cell);
        if (resident == 0u) {
            write_word(name_cell, original);
            write_word(object_cell, address(object_storage));
            return;
        }
        if (read_word(object_cell) == 0u &&
            norrath_factory_strcasecmp(text(resident), string) == 0) {
            write_word(name_cell, original);
            write_word(object_cell, address(object_storage));
            return;
        }
        slot = (slot + 1u) & 511u;
    }
}
