#include "restoration.hpp"
extern "C" void norrath_megatile_entitize(unsigned char* input, int filter_requested) {
    using namespace norrath_restoration_detail;
    // Observed non-save area of the688-byte original frame. These byte offsets
    // preserve delayed-output, formatting and late-point overlap; no zero-fill.
    alignas(16) unsigned char workspace[576];
    const Address frame = address(workspace);
    const Address start = address(input);
    const Address counters = address(norrath_megatile_counters_view);
    write_word(counters + 16u, 0u); write_word(counters, 0u);
    write_word(counters + 4u, 0u); write_word(counters + 8u, 0u); write_word(counters + 12u, 0u);
    const Address defaults = address(norrath_restore_defaults_view);
    write_word(defaults + 8u, 32u); write_word(defaults + 20u, 68u);
    write_word(frame + 560u, start + 8u);
    write_word(defaults, 32u); write_word(defaults + 4u, 32u);
    const unsigned flags = read_half(start + 2u);
    write_word(defaults + 12u, 68u); write_word(defaults + 16u, 68u);
    const unsigned string_offset = read_word(start + 4u);
    write_word(frame + 564u, flags & 2u);
    const Address string_base = start + string_offset;
    const int count = signed_half(read_half(start));
    write_word(defaults + 32u, 0x3f800000u);
    write_word(frame + 568u, string_base);
    write_word(defaults + 28u, 0xbf800000u);
    write_half(start + 2u, flags | 2u);
    write_word(frame + 448u, 0u);
    write_word(defaults + 24u, 0u);
    if (count > 0) {
        const float scale = read_float(address(norrath_megatile_scale_view));
        unsigned remaining = static_cast<unsigned>(count);
        do {
            const Address record = read_word(frame + 560u);
            const int record_size = signed_half(read_half(record + 4u));
            write_word(frame + 560u, record + static_cast<unsigned>(record_size));
            if (read_word(frame + 564u) == 0u)
                write_word(record, read_word(frame + 568u) + read_word(record));
            const Address argument_start = record + 20u;
            Address cell = argument_start;
            unsigned value = read_word(cell);
            while (value != 0u) {
                if (read_word(frame + 564u) == 0u)
                    write_word(cell, read_word(frame + 568u) + read_word(cell));
                cell += 4u;
                value = read_word(cell);
            }
            bool skip = norrath_factory_strstr(text(read_word(record)),
                                               text(address(norrath_megatile_name_a_view))) != nullptr;
            if (!skip) skip = norrath_factory_strstr(text(read_word(record)),
                                                    text(address(norrath_megatile_name_b_view))) != nullptr;
            if (!skip) {
                bool create_directly = norrath_factory_strstr(text(read_word(record)),
                                                             text(address(norrath_megatile_name_c_view))) != nullptr;
                if (!create_directly) create_directly = norrath_factory_strstr(text(read_word(record)),
                                                                               text(address(norrath_megatile_name_d_view))) != nullptr;
                if (!create_directly) create_directly = norrath_factory_strstr(text(read_word(record)),
                                                                               text(address(norrath_megatile_name_e_view))) != nullptr;
                if (!create_directly) create_directly = norrath_string_compare(text(read_word(record)),
                                                                              text(address(norrath_megatile_name_f_view))) == 0;
                if (create_directly) {
                    copy_point(frame + 464u, record + 8u);
                    if (filter_requested == 0 || filter_allows(read_word(record))) {
                        const int type = signed_half(read_half(record + 6u));
                        norrath_object_create_by_name(text(read_word(record)), bytes(frame + 464u), type,
                                                       arguments(argument_start));
                    }
                } else {
                    const float x_scaled = read_float(record + 8u) * scale;
                    const float x_floor = norrath_entity_floorf(x_scaled);
                    const int x_cell = static_cast<int>(x_floor);
                    const float y_scaled = read_float(record + 12u) * scale;
                    const float y_floor = norrath_entity_floorf(y_scaled);
                    const int y_cell = static_cast<int>(y_floor);
                    const Address regions = address(norrath_megatile_regions_view);
                    unsigned index = 0u;
                    Address row = regions;
                    if (signed_byte(*bytes(row + 2u)) != -1) {
                        do {
                            const int x0 = signed_byte(*bytes(row));
                            bool inside = x_cell >= x0;
                            if (inside) {
                                const int y0 = signed_byte(*bytes(row + 1u));
                                inside = y_cell >= y0;
                                if (inside) inside = x_cell < x0 + signed_byte(*bytes(regions + 6u + 10u * index));
                                if (inside) inside = y_cell < y0 + signed_byte(*bytes(row + 5u));
                            }
                            if (inside)
                                norrath_megatile_do_entity_dispatch(bytes(record), signed_word(index), workspace,
                                    reinterpret_cast<int*>(static_cast<__UINTPTR_TYPE__>(frame + 448u)), filter_requested != 0);
                            index += 1u;
                            row += 10u;
                            // This32 bound and -1 sentinel are actual original controls.
                            if (signed_word(index) >= 32) break;
                        } while (signed_byte(*bytes(row + 2u)) != -1);
                    }
                }
            }
            remaining -= 1u;
        } while (remaining != 0u);
    }
    if (signed_word(read_word(frame + 448u)) > 0) {
        unsigned index = 0u;
        unsigned offset = 0u;
        do {
            const Address delayed = frame + offset;
            offset += 28u;
            const Address saved_arguments = read_word(delayed + 20u);
            const int selector = signed_byte(*bytes(delayed + 24u));
            copy_point(frame + 480u, delayed);
            char** const parsed = norrath_megatile_specialparse(arguments(saved_arguments), selector, 1,
                                                                bytes(frame + 480u), text(read_word(delayed + 12u)));
            if (filter_requested == 0 || filter_allows(read_word(delayed + 12u))) {
                const Address name = read_word(delayed + 12u);
                copy_point(frame + 496u, delayed);
                const int type = signed_half(read_half(delayed + 16u));
                norrath_object_create_by_name(text(name), bytes(frame + 496u), type, parsed);
            }
            index += 1u;
        } while (signed_word(index) < signed_word(read_word(frame + 448u)));
    }
    const Address world = read_word(address(norrath_megatile_world_word));
    const int mode = signed_half(read_half(world + 50u));
    if (mode == 1) {
        const Address x_values = address(norrath_megatile_x_view);
        if (read_float(x_values) != -2.0f) {
            const Address y_values = address(norrath_megatile_y_view);
            const Address grid_origin = address(norrath_megatile_grid_origin_view);
            Address x_cursor = x_values;
            unsigned first = 0u;
            unsigned y_offset = 0u;
            for (;;) {
                const Address entity_name = address(norrath_megatile_add_pm_list(text(address(norrath_megatile_pm_name_view))));
                write_word(frame + 572u, entity_name);
                unsigned end = first;
                if (read_float(x_cursor) != -1.0f) {
                    Address scan = x_values + 4u * first;
                    float marker;
                    do {
                        scan += 4u;
                        marker = read_float(scan);
                        end += 1u;
                    } while (marker != -1.0f);
                }
                append_pm_pointer(address(norrath_megatile_add_pm_list(text(address(norrath_megatile_pm_a_view)))));
                append_pm_pointer(address(norrath_megatile_add_pm_list(text(address(norrath_megatile_pm_b_view)))));
                append_pm_pointer(address(norrath_megatile_add_pm_list(text(address(norrath_megatile_pm_c_view)))));
                norrath_factory_sprintf(text(frame + 512u), text(address(norrath_megatile_count_format_view)),
                                         signed_word(end - first));
                append_pm_pointer(address(norrath_megatile_add_pm_list(text(frame + 512u))));
                while (signed_word(first) < signed_word(end)) {
                    const float original_x = read_float(x_cursor);
                    first += 1u;
                    x_cursor += 4u;
                    const unsigned gx = read_word(grid_origin);
                    const float x_bias = static_cast<float>(signed_word(6u * gx));
                    float x = original_x - 12.0f;
                    x = x + x_bias;
                    x = x * 30.0f;
                    x = x * 4.0f;
                    const unsigned long long x_bits = norrath_entity_fptodp(x);
                    const unsigned gy = read_word(grid_origin + 4u);
                    const float original_y = read_float(y_values + y_offset);
                    y_offset += 4u;
                    const float y_bias = static_cast<float>(signed_word(6u * gy));
                    float y = original_y - 12.0f;
                    y = y + y_bias;
                    y = y * 30.0f;
                    y = y * 4.0f;
                    const unsigned long long y_bits = norrath_entity_fptodp(y);
                    norrath_factory_sprintf(text(frame + 512u), text(address(norrath_megatile_point_format_view)), x_bits, y_bits);
                    append_pm_pointer(address(norrath_megatile_add_pm_list(text(frame + 512u))));
                }
                x_cursor += 4u;
                char** const capped = norrath_megatile_cap_pm_pointers();
                // Actual SP+544 aliases formatting buffer+32. Preserve whatever
                // real formatting wrote there; do not fabricate a zero point.
                norrath_object_create_by_name(text(read_word(frame + 572u)), bytes(frame + 544u), 0, capped);
                const float next = read_float(x_cursor);
                y_offset += 4u;
                first += 1u;
                if (next == -2.0f) break;
            }
        }
    }
    norrath_object_process_delete_list();
}
