#include "../w9-animation-object-a-common/mem.hpp"
// Observed findTagFloat__10GameObjectPci at 0x001410a0, 328 bytes. The object's tag list
// is the word at object+44: a zero-terminated array of string pointers. A zero word
// there returns 0.0 before the query is touched. Otherwise the query text is lowercased
// in place (ctype bit 0, add 32), its length is taken, and an empty first entry returns
// 0.0. Each entry whose first length bytes match the query (strncmp) and whose next byte
// is '=' counts as one hit; after index + 1 hits the text after that '=' is parsed with
// atof and narrowed to float. Running off the end of the list returns 0.0. The list
// word at +44 and the entry are read again after every compare, as the original does.
// Inferred: this is the float-valued sibling of findTagString__10GameObjectPci (same
// scan; only the final conversion differs). Double values travel as 64-bit words.
// Unknown: the type of the object beyond the tag list word.
extern "C" {
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
extern const unsigned char norrath_factory_ctype_minimum_view[];
unsigned long long norrath_player_atof(const char* text);
float norrath_player_double_to_float(unsigned long long value);
}

extern "C" float findTagFloat__10GameObjectPci(unsigned char* object, char* name, int index) {
    using namespace norrath_ao;
    if (load_word(object + 44) == 0u) return 0.0f;
    for (unsigned char* cursor = reinterpret_cast<unsigned char*>(name); *cursor != 0u; ++cursor) {
        const int character = *cursor < 0x80u ? int(*cursor) : int(*cursor) - 0x100;
        const int folded = (norrath_factory_ctype_minimum_view[static_cast<unsigned>(character + 128)] & 1u) != 0u
                               ? character + 32
                               : character;
        *cursor = static_cast<unsigned char>(folded);
    }
    const unsigned length = norrath_factory_strlen(name);
    unsigned char* tags = load_pointer(object + 44);
    if (load_word(tags) == 0u) return 0.0f;
    unsigned offset = 0;
    for (;;) {
        const int compare = norrath_factory_strncmp(
            reinterpret_cast<const char*>(load_pointer(tags + offset)), name, length);
        tags = load_pointer(object + 44);
        if (compare == 0) {
            const unsigned char* entry = load_pointer(tags + offset);
            offset += 4;
            if (load_signed_byte(entry + length) == '=') {
                index -= 1;
                if (index == -1)
                    return norrath_player_double_to_float(
                        norrath_player_atof(reinterpret_cast<const char*>(entry + length + 1)));
            }
        } else {
            offset += 4;
        }
        if (load_word(tags + offset) == 0u) return 0.0f;
    }
}
