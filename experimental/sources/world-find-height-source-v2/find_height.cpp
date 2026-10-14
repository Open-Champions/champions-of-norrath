#include "find_height.hpp"
namespace {
using Word = unsigned;
Word address(const unsigned char* p) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
unsigned char* bytes(Word a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
Word read_half(Word a) {
    const unsigned char* p = bytes(a);
    return Word(p[0]) | (Word(p[1]) << 8);
}
Word read_word(Word a) {
    const unsigned char* p = bytes(a);
    return Word(p[0]) | (Word(p[1]) << 8) | (Word(p[2]) << 16) |
           (Word(p[3]) << 24);
}
void write_word(Word a, Word value) {
    for (unsigned i = 0; i != 4; ++i)
        bytes(a)[i] = static_cast<unsigned char>(value >> (8 * i));
}
float float_from_bytes(const unsigned char* p) {
    float value;
    auto representation = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i != 4; ++i) representation[i] = p[i];
    return value;
}
float read_float(Word a) { return float_from_bytes(bytes(a)); }
int signed_half(Word value) {
    return value < 32768u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
int signed_word(Word value) {
    return value < 0x80000000u ? static_cast<int>(value) : -1 - static_cast<int>(~value);
}
int sample_height(Word value) {
    const Word shifted = value >> 4;
    return value < 32768u ? static_cast<int>(shifted) : static_cast<int>(shifted) - 4096;
}
}
extern "C" float norrath_world_find_height(unsigned char* world,
                                            const unsigned char* point_pointer,
                                            float radius) {
    unsigned char point[12];
    for (unsigned i = 0; i != 12; ++i) point[i] = point_pointer[i];
    // The real dependency fills at most 63 pointer words followed by zero.
    alignas(4) unsigned char collision_list[64 * 4];
    const Word frame_address = address(norrath_collision_frame_bytes);
    const Word frame = read_word(frame_address);
    const float x = float_from_bytes(point);
    const float y = float_from_bytes(point + 4);
    const float high_x = x + radius;
    write_word(frame_address, frame + 1u);
    const float high_y = y + radius;
    const float low_x = x - radius;
    const float low_y = y - radius;
    const float quarter_radius = ee_mul_ordered(radius, 0.25f);
    (void)norrath_make_collision_list(world, low_x, low_y, high_x, high_y,
                                     collision_list, 64);
    const float radius_squared = ee_mul_ordered(quarter_radius, quarter_radius);
    const Word reach = ee_cvt_w_s_word(ceilf(quarter_radius));
    int best = -1000000;
    Word list_offset = 0;
    Word part = read_word(address(collision_list));
    if (part != 0u) {
        const float orientation_scale =
            read_float(address(norrath_find_height_orientation_scale_bytes));
        do {
            const Word map = read_word(part + 12u);
            const Word initial_translation_z = read_word(part + 28u);
            const Word maximum = Word(signed_half(read_half(map + 18u))) +
                                 initial_translation_z;
            if (best < signed_word(maximum)) {
                const float translation_x =
                    ee_collision_int_to_float(signed_word(read_word(part + 20u)));
                const float translation_y =
                    ee_collision_int_to_float(signed_word(read_word(part + 24u)));
                const Word orientation_a_word = read_half(part + 32u);
                const Word orientation_b_word = read_half(part + 34u);
                const float dx = x - translation_x;
                const Word flags = read_half(part + 16u);
                const float dy = y - translation_y;
                const float orientation_a =
                    ee_collision_int_to_float(signed_half(orientation_a_word));
                const float orientation_b =
                    ee_collision_int_to_float(signed_half(orientation_b_word));
                const float a = ee_mul_ordered(orientation_a, orientation_scale);
                const float b = ee_mul_ordered(orientation_b, orientation_scale);
                float local_x, local_y;
                if (flags & 1u) {
                    const float dy_a = ee_mul_ordered(dy, a);
                    const float dx_a = ee_mul_ordered(dx, a);
                    const float dx_b = ee_mul_ordered(dx, b);
                    const float dy_b = ee_mul_ordered(dy, b);
                    local_y = dx_b - dy_a;
                    local_x = dx_a + dy_b;
                } else {
                    const float negative_dx = -dx;
                    const float dy_a = ee_mul_ordered(dy, a);
                    const float dx_a = ee_mul_ordered(dx, a);
                    const float dy_b = ee_mul_ordered(dy, b);
                    const float negative_dx_b = ee_mul_ordered(negative_dx, b);
                    local_x = dx_a + dy_b;
                    local_y = negative_dx_b + dy_a;
                }
                const float grid_x = ee_mul_ordered(local_x, 0.25f);
                const float grid_y = ee_mul_ordered(local_y, 0.25f);
                const Word center_x = ee_cvt_w_s_word(floorf(grid_x));
                const float floor_y = floorf(grid_y);
                const Word origin_x = read_word(map);
                const Word center_y = ee_cvt_w_s_word(floor_y);
                const Word origin_y = read_word(map + 4u);
                const Word start_x = center_x - reach;
                const Word end_x = center_x + reach + 1u;
                const Word start_y = center_y - reach;
                const Word end_y = center_y + reach + 1u;
                // This read occurs even if the signed outer range is empty.
                const Word width = read_word(map + 8u);
                bool maximum_reached = false;
                for (Word row = start_y;
                     signed_word(row) <= signed_word(end_y); ++row) {
                    const Word relative_row = row - origin_y;
                    if (signed_word(relative_row) < 0) continue;
                    if (signed_word(relative_row) >= signed_word(read_word(map + 12u)))
                        continue;
                    if (signed_word(end_x) < signed_word(start_x)) continue;
                    Word sample = map + 20u +
                        2u * (relative_row * width - origin_x + start_x);
                    for (Word column = start_x;
                         signed_word(column) <= signed_word(end_x);
                         ++column, sample += 2u) {
                        const Word relative_column = column - origin_x;
                        if (signed_word(relative_column) < 0 ||
                            signed_word(relative_column) >= signed_word(width)) continue;
                        const Word raw_sample = read_half(sample);
                        const Word translation_z = read_word(part + 28u);
                        const int height = signed_word(Word(sample_height(raw_sample)) +
                                                       translation_z);
                        if (best >= height) continue;
                        const float sample_x = ee_collision_int_to_float(signed_word(column));
                        const float sample_y = ee_collision_int_to_float(signed_word(row));
                        const float distance_x = sample_x - grid_x;
                        const float distance_y = sample_y - grid_y;
                        const float x_squared = ee_mul_ordered(distance_x, distance_x);
                        const float y_squared = ee_mul_ordered(distance_y, distance_y);
                        const float distance_squared = x_squared + y_squared;
                        if (!(distance_squared < radius_squared)) continue;
                        const Word current_maximum = Word(signed_half(read_half(map + 18u))) +
                                                     translation_z;
                        best = height;
                        if (best >= signed_word(current_maximum)) {
                            maximum_reached = true;
                            break;
                        }
                    }
                    if (maximum_reached) break;
                }
            }
            list_offset += 4u;
            part = read_word(address(collision_list) + list_offset);
        } while (part != 0u);
    }
    return ee_collision_int_to_float(best);
}
