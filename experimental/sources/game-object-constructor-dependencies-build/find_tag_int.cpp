#include "dependencies.hpp"
extern "C" int norrath_game_object_find_tag_int(void* object_storage, char* tag, int occurrence) {
    using namespace norrath_constructor_dependency_detail;
    const Address object = address(object_storage);
    if (read_word(object + 44u) == 0u) return 0;
    lower_nonempty_tag(address(tag));
    const unsigned length = norrath_factory_strlen(tag);
    Address arguments = read_word(object + 44u);
    if (read_word(arguments) == 0u) return 0;
    unsigned offset = 0u;
    unsigned remaining = static_cast<unsigned>(occurrence);
    for (;;) {
        const Address entry = read_word(arguments + offset);
        const int comparison = norrath_factory_strncmp(text(entry), tag, length);
        // Unlike the pointer-array public query, this method reloads the field
        // and addresses the current cell from the fresh array after every call.
        arguments = read_word(object + 44u);
        if (comparison == 0) {
            const Address fresh_entry = read_word(arguments + offset);
            const Address separator = fresh_entry + length;
            if (signed_byte(*bytes(separator)) == 61) {
                remaining -= 1u;
                if (remaining == 0xffffffffu)
                    return norrath_factory_atoi(text(separator + 1u));
            }
        }
        offset += 4u;
        if (read_word(arguments + offset) == 0u) return 0;
    }
}
