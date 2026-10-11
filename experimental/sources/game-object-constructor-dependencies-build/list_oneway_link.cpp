#include "dependencies.hpp"
extern "C" void* norrath_list_oneway_link(void* head_storage, void* object_head_storage,
                                           void* value_storage) {
    using namespace norrath_constructor_dependency_detail;
    const Address pool = address(norrath_constructor_list_pool_view);
    const Address head = address(head_storage);
    const Address object_head = address(object_head_storage);
    Address cursor = read_word(pool);
    Address primary = 0u;
    if (cursor != 0u) {
        primary = cursor;
        cursor = read_word(cursor);
        write_word(pool, cursor);
        write_half(primary + 16u, 0u);
    }
    Address secondary = 0u;
    if (cursor != 0u) {
        secondary = cursor;
        cursor = read_word(cursor);
        write_word(pool, cursor);
        write_half(secondary + 16u, 0u);
    }
    // Original execution does not create replacement nodes or add a failure
    // return when the pool is short. Every actual dereference remains present.
    write_word(primary, read_word(head));
    const Address fresh_head = read_word(head);
    if (fresh_head != 0u) write_word(fresh_head + 4u, primary);
    write_word(primary + 4u, head);
    write_word(head, primary);
    write_word(secondary, read_word(object_head));
    const Address fresh_object_head = read_word(object_head);
    if (fresh_object_head != 0u) write_word(fresh_object_head + 4u, secondary);
    write_word(secondary + 4u, object_head);
    write_word(object_head, secondary);
    write_word(primary + 12u, address(value_storage));
    write_word(primary + 8u, secondary);
    write_word(secondary + 12u, 0u);
    write_word(secondary + 8u, primary);
    return pointer(primary);
}
