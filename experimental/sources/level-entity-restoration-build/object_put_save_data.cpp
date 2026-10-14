#include "restoration.hpp"
extern "C" unsigned norrath_object_put_save_data(unsigned char* input, int filter_requested) {
    using namespace norrath_restoration_detail;
    const Address start = address(input);
    Address next_record = start + 8u;
    initial_integer_defaults();
    const unsigned flags = read_half(start + 2u);
    const unsigned string_offset = read_word(start + 4u);
    const int count = signed_half(read_half(start));
    const Address defaults = address(norrath_restore_defaults_view);
    write_word(defaults + 32u, 0x3f800000u);
    write_half(start + 2u, flags | 2u);
    write_word(defaults + 28u, 0xbf800000u);
    write_word(address(norrath_restore_string_base_word), start + string_offset);
    write_word(defaults + 24u, 0u);
    if (count > 0) {
        unsigned remaining = static_cast<unsigned>(count);
        do {
            const Address record = next_record;
            const int size = signed_half(read_half(record + 4u));
            const unsigned name_offset = read_word(record);
            next_record += static_cast<unsigned>(size);
            Address name = 0u;
            if (signed_word(name_offset) >= 0)
                name = read_word(address(norrath_restore_string_base_word)) + name_offset;
            if (filter_requested == 0 || filter_allows(name)) {
                NorrathPoint3 point;
                copy_point(address(&point), record + 8u);
                const Address argument_start = record + 20u;
                Address cell = argument_start;
                unsigned argument_count = 0u;
                unsigned value = read_word(cell);
                while (value != 0u) {
                    if ((flags & 2u) == 0u) {
                        value = read_word(cell);
                        unsigned relocated = 0u;
                        if (signed_word(value) >= 0)
                            relocated = read_word(address(norrath_restore_string_base_word)) + value;
                        write_word(cell, relocated);
                    }
                    cell += 4u;
                    value = read_word(cell);
                    argument_count += 1u;
                }
                unsigned char* object = nullptr;
                const Address target = read_word(address(norrath_create_by_name_function_word));
                const short type = static_cast<short>(signed_half(read_half(record + 6u)));
                if (target != 0u) {
                    const NorrathCreateByName create = reinterpret_cast<NorrathCreateByName>(static_cast<__UINTPTR_TYPE__>(target));
                    object = create(text(name), point, type, arguments(argument_start));
                    if (object != nullptr) write_word(address(object) + 52u, name);
                }
                if ((flags & 1u) != 0u && object != nullptr) {
                    const Address virtual_entry = read_word(address(object) + 72u) + 64u;
                    const int adjustment = signed_half(read_half(virtual_entry));
                    const Address callback = read_word(virtual_entry + 4u);
                    const SaveVirtual method = reinterpret_cast<SaveVirtual>(static_cast<__UINTPTR_TYPE__>(callback));
                    const Address payload = (record + 4u * argument_count + 31u) & 0xfffffff8u;
                    method(bytes(address(object) + static_cast<unsigned>(adjustment)), bytes(payload));
                }
            }
            remaining -= 1u;
        } while (remaining != 0u);
    }
    // Real safe iterator temporarily changes links while callbacks run.
    alignas(16) unsigned char iterator[32];
    const int mask = signed_word(read_word(address(norrath_restore_broadcast_mask_word)));
    norrath_list_safe_iterator_construct(iterator, norrath_restore_broadcast_head_view, mask);
    Address current = read_word(address(iterator) + 4u);
    while (current != 0u) {
        const Address object = read_word(current + 12u);
        if (object != 0u) {
            const Address entry = read_word(object + 72u) + 8u;
            const int adjustment = signed_half(read_half(entry));
            const BroadcastVirtual method = reinterpret_cast<BroadcastVirtual>(static_cast<__UINTPTR_TYPE__>(read_word(entry + 4u)));
            method(bytes(object + static_cast<unsigned>(adjustment)), 3);
        }
        norrath_list_safe_iterator_next(iterator);
        current = read_word(address(iterator) + 4u);
    }
    norrath_list_safe_iterator_destroy(iterator, 2);
    return next_record - start;
}
