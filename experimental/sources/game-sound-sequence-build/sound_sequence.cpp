#include "sound_sequence.hpp"
using namespace norrath_sound_sequence_detail;

extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain) {
    if (animation_header == nullptr) return -1;
    const Address header = address(animation_header);
    const unsigned group = read_byte_fresh(header + 60u);
    const Address records = address(norrath_sound_sequence_records_view);
    const Address group_limit = address(norrath_sound_group_limits_view) + group;
    int selected = -1;
    int matching = 0;
    // The original scans all32 entries and remembers the LAST unused entry.
    // Existing group matches can reject even when an unused slot was seen.
    for (int index = 0; index < 32; ++index) {
        const Address record = records + unsigned(index) * 76u;
        const unsigned occupied = read_byte_fresh(record + 4u);
        if (occupied == 0u) {
            selected = index;
        } else {
            const Address active_header = read_word_fresh(record + 8u);
            const unsigned active_group = read_byte_fresh(active_header + 60u);
            if (active_group == group) {
                ++matching;
                if (group != 0u) {
                    const int limit = signed_byte(static_cast<unsigned char>(read_byte_fresh(group_limit)));
                    if (!(matching < limit)) return -1;
                }
            }
        }
    }
    if (selected == -1) return -1;
    const Address record = records + unsigned(selected) * 76u;
    write_half_ordered(record + 6u, 0u);
    norrath_actor_animation_add(pointer(record), animation_header, 4);
    if (object != nullptr) {
        const Address object_address = address(object);
        const Address head = record + 40u;
        norrath_list_destroy(pointer(head));
        norrath_list_oneway_link(pointer(head), object, object);
        // Both source chunks are captured AFTER the real callbacks and BEFORE
        // either destination chunk is stored, including overlapping objects.
        const Wide first = read_wide_fresh(object_address + 16u);
        const unsigned last = read_word_fresh(object_address + 24u);
        write_wide_ordered(record + 44u, first);
        write_word_ordered(record + 52u, last);
        write_byte_ordered(record + 58u, 0u);
    } else {
        write_byte_ordered(record + 58u, 1u);
    }
    const int paused = norrath_game_is_paused();
    const Address spinner = address(norrath_sound_sequence_id_spinner_view);
    const unsigned previous_id = read_half_fresh(spinner);
    write_half_ordered(record + 56u, previous_id);
    write_byte_ordered(record + 59u, unsigned(paused));
    write_half_ordered(spinner, (previous_id + 1u) & 127u);
    write_byte_ordered(record + 60u, 0u);
    write_float_ordered(record + 64u, gain);
    write_float_ordered(record + 68u, 360.0f);
    write_float_ordered(record + 72u, 840.0f);
    return signed_word(unsigned(selected) | (previous_id << 8) | 0x8000u);
}
