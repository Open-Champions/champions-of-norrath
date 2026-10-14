#include "player_grid.hpp"
extern "C" void norrath_object_update_in_grid(void* object) {
    using namespace norrath_player_grid_detail;
    const Address owner = address(object);
    const Address node = read_word(read_word(owner + 8u) + 8u);
    const Address previous_slot = read_word(node + 4u);
    const Address next = read_word(node);
    write_word(previous_slot, next);
    const Address fresh_next = read_word(node);
    if (fresh_next != 0u) write_word(fresh_next + 4u, read_word(node + 4u));
    const int x = grid_coordinate(owner + 16u), y = grid_coordinate(owner + 20u);
    Address head;
    if (unsigned(x) >= 120u || y < 0 || y >= 120)
        head = address(norrath_constructor_default_head_view);
    else {
        const Address grid = address((read_word(owner + 12u) & 0x800u)
            ? norrath_constructor_grid_b_view : norrath_constructor_grid_a_view);
        head = grid + (unsigned(y) * 120u + unsigned(x)) * 4u;
    }
    const Address old_head = read_word(head);
    write_word(node + 4u, head); write_word(node, old_head); write_word(head, node);
    const Address inserted_next = read_word(node);
    if (inserted_next != 0u) write_word(inserted_next + 4u, node);
}
