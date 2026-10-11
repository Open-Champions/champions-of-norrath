#include "load_entities.hpp"
namespace {
using Word = unsigned;
Word address(const unsigned char* p) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
unsigned char* bytes(Word a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
char* characters(unsigned char* p) { return reinterpret_cast<char*>(p); }
Word read_word(Word a) {
    return Word(bytes(a)[0]) | (Word(bytes(a)[1]) << 8) |
           (Word(bytes(a)[2]) << 16) | (Word(bytes(a)[3]) << 24);
}
}
extern "C" void norrath_mc_load_level_entities(char* lump_name, char* saved_level_name) {
    const Word index_base = address(norrath_mc_saved_level_index_bytes);
    Word index = 0, saved_offset = 0;
    while (index < 26u) {
        const Word record = index_base + 24u * index;
        if (bytes(record)[0] == 0) break;
        if (norrath_string_compare(characters(bytes(record)), saved_level_name) == 0) break;
        // Capture this length after the real comparison returns.
        saved_offset += read_word(record + 20u);
        ++index;
    }
    // Entry 26's first byte is still read if all twenty-six comparisons fail.
    const int has_saved_level = bytes(index_base + 24u * index)[0] != 0;
    alignas(4) char filename[80];
    norrath_format_string(filename, characters(norrath_mc_lump_format_bytes), lump_name);
    unsigned char* const lump = static_cast<unsigned char*>(norrath_lump_load(filename));

    bool installed_script = false;
    if (lump != nullptr) {
        unsigned char* const first = norrath_lump_find(lump, characters(norrath_mc_script_section_key_bytes));
        if (first != nullptr) {
            unsigned char* const second = norrath_lump_find(lump, characters(norrath_mc_script_section_key_bytes));
            if (read_word(address(second) + 8u) != 0) {
                unsigned char* const third = norrath_lump_find(lump, characters(norrath_mc_script_section_key_bytes));
                norrath_script_set_script(bytes(read_word(address(third) + 4u)));
                installed_script = true;
            }
        }
    }
    if (!installed_script) norrath_script_set_script(nullptr);

    if (norrath_keep_player_positions_bytes[0] != 0) {
        norrath_keep_player_positions_bytes[0] = 0;
        norrath_got_player_start_this_level_bytes[0] = 3;
    } else {
        norrath_got_player_start_this_level_bytes[0] = 0;
    }
    if (norrath_mc_disable_startup_script_bytes[0] == 0)
        norrath_script_start(characters(norrath_mc_pre_start_key_bytes));

    if (has_saved_level) {
        const Word payload = address(norrath_mc_saved_level_payload_bytes);
        const Word load_buffer = address(norrath_mc_load_buffer_bytes);
        const Word object_bytes = read_word(payload + saved_offset);
        norrath_copy_bytes(bytes(load_buffer), bytes(payload + saved_offset + 8u), object_bytes);
        // The second header is read after the first genuine copy completes.
        const Word floor_bytes = read_word(payload + saved_offset + 8u + object_bytes);
        norrath_copy_bytes(bytes(load_buffer + object_bytes),
                           bytes(payload + saved_offset + 16u + object_bytes), floor_bytes);
        norrath_map_read_saved_bytes(bytes(payload + saved_offset + 16u + object_bytes + floor_bytes));
        norrath_object_put_save_data(bytes(load_buffer), 0);
        norrath_floor_put_save_info(bytes(load_buffer + object_bytes));
    }
    // The original delay slot reads this word even on the null-lump path.
    const Word world = read_word(address(norrath_route_world_word_bytes));
    if (lump != nullptr) {
        const Word megatile_mode = read_word(world + 112u);
        unsigned char* const object_section = norrath_lump_find(lump, characters(norrath_mc_objects_section_key_bytes));
        unsigned char* const objects = bytes(read_word(address(object_section) + 4u));
        if (megatile_mode != 0) norrath_megatile_entitize(objects, has_saved_level);
        else norrath_object_put_save_data(objects, has_saved_level);
    }
    // Startup scripts can change this flag, so it is read again at the late site.
    if (norrath_mc_disable_startup_script_bytes[0] == 0)
        norrath_script_start(characters(norrath_mc_start_key_bytes));
}
