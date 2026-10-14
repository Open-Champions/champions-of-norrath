#include "registration.hpp"
extern "C" int norrath_game_never_save_always_load_from_lump(char* name) {
    using namespace norrath_registration_detail;
    if (name == nullptr) return 0;
    Word cursor = address(norrath_game_never_save_table_view);
    if (read_word(cursor) == 0u) return 0;
    for (;;) {
        // Actual first entry is freshly reloaded after the initial nonzero test.
        // Each subsequent entry is reloaded after its post-comparison test.
        Word entry = read_word(cursor);
        cursor += 4u;
        if (norrath_factory_strcasecmp(name, reinterpret_cast<const char*>(bytes(entry))) == 0)
            return 1;
        if (read_word(cursor) == 0u) return 0;
    }
}
