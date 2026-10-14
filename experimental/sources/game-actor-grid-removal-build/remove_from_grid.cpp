#include "actors.hpp"
#include "constructor.hpp"

extern "C" void norrath_actor_remove_from_grid(void* object) {
    using namespace norrath_constructor_detail;
    const Address node = read_word(read_word(address(object) + 8u) + 8u);
    const Address previous_slot = read_word(node + 4u);
    const Address next = read_word(node);
    write_word(previous_slot, next);

    // The previous-slot write can change this node's fields.
    const Address fresh_next = read_word(node);
    if (fresh_next != 0u)
        write_word(fresh_next + 4u, read_word(node + 4u));

    const Address head = address(norrath_constructor_default_head_view);
    const Address old_head = read_word(head);
    write_word(node + 4u, head);
    write_word(node, old_head);
    write_word(head, node);

    // Reload after all three insertion writes, including possible aliases.
    const Address inserted_next = read_word(node);
    if (inserted_next != 0u)
        write_word(inserted_next + 4u, node);
}
