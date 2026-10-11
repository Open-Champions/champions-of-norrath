#include "process_delete.hpp"

extern "C" void norrath_object_process_delete_list() {
    using namespace norrath_process_delete_detail;
    alignas(8) unsigned char iterator[32];
    const int mask = signed_word(read_word(address(norrath_restore_broadcast_mask_word)));
    norrath_list_safe_iterator_construct(iterator, norrath_restore_broadcast_head_view, mask);
    Address current = read_word(address(iterator) + 4u);
    while (current != 0u) {
        const Address object = read_word(current + 12u);
        if (object != 0u) {
            const Address entry = read_word(object + 72u) + 8u;
            const int adjustment = signed_adjustment(entry);
            const Address code = read_word(entry + 4u);
            const Callback callback = reinterpret_cast<Callback>(static_cast<__UINTPTR_TYPE__>(code));
            callback(pointer(object + static_cast<unsigned>(adjustment)), 3);
        }
        // The callback can remove objects or change links. Use the genuine
        // iterator and reread its selected node after advancement.
        norrath_list_safe_iterator_next(iterator);
        current = read_word(address(iterator) + 4u);
    }
    norrath_list_safe_iterator_destroy(iterator, 2);
}
