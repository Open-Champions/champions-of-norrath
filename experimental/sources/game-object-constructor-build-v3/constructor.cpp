#include "constructor.hpp"
extern "C" unsigned norrath_game_object_construct(void* object_storage, const void* point_storage,
                                                  void* resource_map, char** incoming_arguments) {
    using namespace norrath_constructor_detail;
    const Address object = address(object_storage);
    const Address point = address(point_storage);
    const Address object_head = object + 8u;
    const Address incoming = address(incoming_arguments);
    // Snapshot all actual point bits before the first object store, including
    // when caller point storage overlaps this writable object view.
    const unsigned point_x = read_word(point);
    const unsigned point_y = read_word(point + 4u);
    const unsigned point_z = read_word(point + 8u);
    write_word(object, 0u);
    write_word(object + 4u, 0u);
    write_word(object + 72u, address(norrath_game_object_vtable_view));
    write_word(object_head, 0u);
    write_word(object + 28u, 0x41c00000u);
    write_word(object + 32u, 0x42900000u);
    write_word(object + 12u, 0u);
    *bytes(object + 40u) = 25u;
    *bytes(object + 41u) = 73u;
    write_word(object + 16u, point_x);
    write_word(object + 20u, point_y);
    write_word(object + 24u, point_z);
    write_word(object + 52u, 0u);
    Address map = address(resource_map);
    write_word(object + 36u, map);
    if (map != 0u) {
        const unsigned map_generation = read_word(map + 4u);
        const unsigned generation = read_word(address(norrath_constructor_resource_frame_view));
        if (map_generation != generation) {
            if (read_word(map + 8u) == 0u) {
                write_word(map + 4u, generation);
            } else {
                unsigned offset = 8u;
                for (;;) {
                    const Address lump_name = read_word(map);
                    const Address resource_name = read_word(map + offset);
                    const Address resource = address(norrath_lump_find_resource(text(lump_name), text(resource_name)));
                    // The real call can affect state: original code reloads map
                    // from object36 before both the result store and next cell.
                    map = read_word(object + 36u);
                    write_word(map + offset + 4u, resource);
                    offset += 8u;
                    if (read_word(map + offset) == 0u) break;
                }
                const unsigned fresh_generation = read_word(address(norrath_constructor_resource_frame_view));
                write_word(map + 4u, fresh_generation);
            }
        }
    }
    const int x = grid_coordinate(object + 16u);
    const int y = grid_coordinate(object + 20u);
    const unsigned flags = read_word(object + 12u);
    Address head = address(norrath_constructor_default_head_view);
    if ((flags & 2u) == 0u && x >= 0 && x < 120 && y >= 0 && y < 120) {
        const Address grid = (flags & 0x800u) != 0u ? address(norrath_constructor_grid_b_view)
                                                  : address(norrath_constructor_grid_a_view);
        head = grid + 4u * (120u * static_cast<unsigned>(y) + static_cast<unsigned>(x));
    }
    norrath_list_oneway_link(pointer(head), pointer(object_head), object_storage);
    // This actual query observes existing object44. Incoming arguments have not
    // been stored there yet; do not replace it with a query of incoming_arguments.
    const int move_to_default = norrath_game_object_find_tag_int(object_storage,
                                                               text(address(norrath_constructor_tag_a_view)), 0);
    unsigned seed = read_word(address(norrath_constructor_random_state_view));
    if (move_to_default != 0) {
        const Address secondary = read_word(object_head);
        const Address primary = read_word(secondary + 8u);
        const Address previous = read_word(primary + 4u);
        const Address next = read_word(primary);
        write_word(previous, next);
        const Address fresh_next = read_word(primary);
        if (fresh_next != 0u) write_word(fresh_next + 4u, read_word(primary + 4u));
        const Address default_head = address(norrath_constructor_default_head_view);
        const Address old_head = read_word(default_head);
        write_word(primary + 4u, default_head);
        write_word(primary, old_head);
        write_word(default_head, primary);
        const Address inserted_next = read_word(primary);
        if (inserted_next != 0u) write_word(inserted_next + 4u, primary);
        seed = read_word(address(norrath_constructor_random_state_view));
    }
    write_word(object + 44u, incoming);
    seed = seed * 1664525u + 1013904223u;
    write_word(address(norrath_constructor_random_state_view), seed);
    write_word(object + 68u, seed >> 16);
    unsigned occurrence = 0u;
    for (;;) {
        const Address arguments = read_word(object + 44u);
        const ScanResult first = find_nth_string(object, arguments, occurrence);
        if (first.selected == 0u) return 0u;
        // The first selected pointer is only a presence test. Original code
        // repeats the full scan and passes its independent result, even null.
        const ScanResult second = find_nth_string(object, first.arguments, occurrence);
        occurrence += 1u;
        norrath_object_add_string(text(second.selected), object_storage);
    }
}
