#include "tag_queries.hpp"
extern "C" int norrath_object_find_tag_int(char** arguments, char* tag, int occurrence) {
    using namespace norrath_tag_detail;
    if (arguments == nullptr) return 0;
    lower_in_place(tag);
    const Word length = norrath_factory_strlen(tag);
    Word remaining = static_cast<Word>(occurrence);
    Word cell = address(arguments);
    if (read_word(cell) == 0u) return 0;
    for (;;) {
        const Word current = read_word(cell);
        if (norrath_factory_strncmp(text(current), tag, length) == 0) {
            const Word fresh = read_word(cell);
            const Word separator = fresh + length;
            if (signed_byte(*bytes(separator)) == 61) {
                remaining -= 1u;
                if (remaining == 0xffffffffu) return norrath_factory_atoi(text(separator + 1u));
            }
        }
        cell += 4u;
        if (read_word(cell) == 0u) return 0;
    }
}
