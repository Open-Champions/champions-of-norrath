#include "floor_add.hpp"

extern "C" void norrath_floor_add(const unsigned char* point, int type_word,
                                   float extent, int kind, int flags) {
    using namespace norrath_floor_add_detail;
    const unsigned blood_level = read_word(address(norrath_floor_blood_level_word_view));
    const int angle = signed_half(static_cast<unsigned>(type_word));
    // The original copies the by-value Point3 before every early exit.
    alignas(4) unsigned char captured_point[12];
    const Address captured = address(captured_point);
    const Address input = address(point);
    const unsigned xy0 = read_word(input);
    const unsigned xy1 = read_word(input + 4u);
    const unsigned z0 = read_word(input + 8u);
    write_word(captured, xy0);
    write_word(captured + 4u, xy1);
    write_word(captured + 8u, z0);
    if (blood_level == 0u && kind == 0) return;
    if (kind == 1 || kind == 2) return;

    const float grid_x = read_float(captured);
    const float grid_y = read_float(captured + 4u);
    const unsigned x_word = ee_cvt_w_s_word(grid_x);
    const unsigned y_word = ee_cvt_w_s_word(grid_y);
    const unsigned x_cell = shift_right_signed_eight(x_word + 3840u);
    const unsigned y_cell = shift_right_signed_eight(y_word + 3840u);
    if (x_cell >= 30u || signed_word(y_cell) < 0 || signed_word(y_cell) >= 30) return;

    const Address grid_word = address(norrath_floor_sprite_grid_view)
                            + 4u * (30u * y_cell + x_cell);
    Address cell = read_word(grid_word);
    if (cell == 0u) {
        const Address free_head = address(norrath_floor_free_list_word_view);
        cell = read_word(free_head);
        if (cell == 0u) return;
        write_word(grid_word, cell);
        const unsigned successor = read_word(cell + 752u);
        write_word(free_head, successor);
        write_word(cell + 752u, 0u);
    }
    const unsigned old_count = read_word(cell + 760u);
    // Preserve the signed comparison, including negative counts when accesses are valid.
    if (signed_word(old_count) >= 31) return;
    const unsigned color = norrath_light_floor_sprite(captured_point);
    const Address entry = cell + 24u * old_count;
    const float lifted_z = ordered_add(read_float(captured + 8u), 3.0f);
    const float y = read_float(captured + 4u);
    const float x = read_float(captured);
    const float quarter_y = ordered_multiply(y, 4.0f);
    const float quarter_x = ordered_multiply(x, 4.0f);
    const float quarter_z = ordered_multiply(lifted_z, 4.0f);
    const unsigned y_position = ee_cvt_w_s_word(quarter_y);
    const unsigned x_position = ee_cvt_w_s_word(quarter_x);
    const unsigned z_position = ee_cvt_w_s_word(quarter_z);
    write_half(entry, color & 255u);
    write_half(entry + 2u, (color >> 8) & 255u);
    write_half(entry + 4u, (color >> 16) & 255u);
    write_half(entry + 6u, 128u);
    write_half(entry + 8u, x_position);
    write_half(entry + 10u, y_position);
    write_half(entry + 12u, z_position);
    write_half(entry + 14u, static_cast<unsigned>(kind) < 3u ? 1u : 0u);

    const float cosine = norrath_integer_cosine(angle);
    const float cosine_extent = ordered_multiply(cosine, extent);
    const float cosine_quarters = ordered_multiply(cosine_extent, 4.0f);
    const unsigned cosine_word = ee_cvt_w_s_word(cosine_quarters);
    // This store occurs before the genuine sine provider, as in the JAL delay slot.
    write_half(entry + 16u, cosine_word);
    const float sine = norrath_integer_sine(angle);
    const float sine_extent = ordered_multiply(sine, extent);
    const unsigned fresh_count = read_word(cell + 760u);
    const unsigned texture_word = 2u * static_cast<unsigned>(kind) + 979u;
    const float original_z = read_float(captured + 8u);
    const unsigned old_total = read_word(cell + 764u);
    write_word(cell + 760u, fresh_count + 1u);
    const unsigned sine_word = ee_cvt_w_s_word(ordered_multiply(sine_extent, 4.0f));
    write_half(entry + 18u, sine_word);
    write_half(entry + 20u, texture_word);

    const int minimum = signed_half(read_half(cell + 756u));
    const int contribution = signed_half(read_half(entry + 14u));
    const bool lower = ee_c_lt_s_word(original_z, static_cast<float>(minimum)) != 0u;
    write_word(cell + 764u, old_total + static_cast<unsigned>(contribution));
    if (lower) write_half(cell + 756u, ee_cvt_w_s_word(original_z));
    const int maximum = signed_half(read_half(cell + 758u));
    const bool higher = ee_c_lt_s_word(static_cast<float>(maximum), original_z) != 0u;
    const Address global_count_word = address(norrath_floor_sprite_count_word_view);
    // The original reads in the comparison branch delay, then rereads after a max store.
    unsigned global_count = read_word(global_count_word);
    if (higher) {
        write_half(cell + 758u, ee_cvt_w_s_word(original_z));
        global_count = read_word(global_count_word);
    }
    write_word(global_count_word, global_count + 1u);
    if (flags == 0) return;

    const Address delayed = address(norrath_floor_expand_list_view);
    unsigned slot = 0u;
    while (slot < 16u) {
        if (read_word(delayed + 20u * slot + 4u) == 0u) break;
        slot += 1u;
    }
    if (slot == 16u) return;
    const Address record = delayed + 20u * slot;
    write_word(record + 12u, 0u);
    write_word(record + 4u, entry + 16u);
    write_float(record + 16u, extent);
    write_half(record + 8u, static_cast<unsigned>(angle));
    bytes(record)[0] = static_cast<unsigned char>(flags);
}
