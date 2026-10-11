#include "named_lookup.hpp"
extern "C" unsigned long long norrath_object_find_named_list(
    char* name, void** output, unsigned long long capacity) {
    using namespace norrath_constructor_detail;
    using norrath_constructor_dependency_detail::arithmetic_shift;
    const unsigned length = norrath_factory_strlen(name);
    const Address query = address(name);
    unsigned hash = 0u;
    unsigned cursor = 0u;
    if (signed_word(length) > 0) {
        do {
            const int character = signed_byte(*bytes(query + cursor));
            cursor += 1u;
            const unsigned flags = norrath_factory_ctype_minimum_view[
                static_cast<unsigned>(character + 128)];
            const int folded = (flags & 1u) != 0u ? character + 32 : character;
            hash += static_cast<unsigned>(folded);
            hash += hash << 10;
            hash ^= arithmetic_shift(hash, 6u);
        } while (signed_word(cursor) < signed_word(length));
    }
    hash += hash << 3;
    hash ^= arithmetic_shift(hash, 11u);
    unsigned slot = hash & 511u;
    unsigned offset = slot << 2;
    const Address names = address(norrath_object_name_hash_slots_view);
    const Address objects = address(norrath_object_pointer_hash_slots_view);
    const Address destination = address(output);
    unsigned found = 0u;
    Address write_cursor = destination;
    if (read_word(names + offset) != 0u) {
        // ADDIU computes a word result after the original native64 capture.
        const unsigned stop_count = static_cast<unsigned>(capacity) - 1u;
        for (;;) {
            const int comparison = norrath_factory_strcasecmp(
                name, text(read_word(names + offset)));
            slot = (slot + 1u) & 511u;
            const Address current_object_slot = objects + offset;
            offset = slot << 2;
            if (comparison == 0) {
                const unsigned object = read_word(current_object_slot);
                if (object != 0u) {
                    found += 1u;
                    write_word(write_cursor, object);
                    write_cursor += 4u;
                    if (found == stop_count) break;
                }
            }
            // The prior comparison/output store may change this next slot.
            if (read_word(names + offset) == 0u) break;
        }
    }
    const Address terminator = destination + (found << 2);
    write_word(terminator, 0u);
    return static_cast<unsigned long long>(
        static_cast<long long>(signed_word(terminator)));
}
